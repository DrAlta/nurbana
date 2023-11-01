#include "NRT_Render.h"


NRT_Render::NRT_Render() {
} //eof NRT_Render::NRT_Render()


NRT_Render::~NRT_Render() {
} //eof NRT_Render::~NRT_Render()


int NRT_Render::RenderThread() {
  int		i,n,j,k;
  Vec3d		DeltaX, DeltaY, RayDir;
  Vec3d		InitPt, PixelPt;
  Vec3d		Color,Color1;


  // Trace _Width*_Height Jittered Rays through each pixel
  for (i= 0; i < _RenderData.Width; i++) {
    for (n= 0; n < _RenderData.Height; n++) {
      if (_RenderData.ImageState[i*_RenderData.Height+n]) {
        if (_RenderData.PacketType) {
          if (((clock()*1.0)-*_RenderData.PacketTimeStart)/CLOCKS_PER_SEC > *_RenderData.PacketTime) return(1);
        } else {
          if (*_RenderData.PacketCount >= *_RenderData.PacketSize) return(1);
          (*_RenderData.PacketCount)++;
        } //fi
        _RenderData.ImageState[i*_RenderData.Height+n]= 0;


        Color.Set(0,0,0);

        if (_RenderData.Stochastic) {
          // Calculate Jitters
          DeltaX= _RenderData.StepX;
          DeltaY= _RenderData.StepY;

          // Calculate Initial Pt
          InitPt= _RenderData.V_TopL + (DeltaX/2.0) + (DeltaY/2.0);

          // From center of pixel, shoot out
          // # of defined rays randomly radius (_StochasticRadius)
          for (j= 0; j < _RenderData.Rays[0] * _RenderData.Rays[1]; j++) {
            PixelPt= _RenderData.StepX*(rand()/NRT_RMI) * _RenderData.StochasticRadius;
            PixelPt+= _RenderData.StepY*(rand()/NRT_RMI) * _RenderData.StochasticRadius;
            if (int((rand()*2)/NRT_RMI)%2) PixelPt.X*= -1;
            if (int((rand()*2)/NRT_RMI)%2) PixelPt.Y*= -1;
            PixelPt+= (_RenderData.StepX*i);
            PixelPt+= (_RenderData.StepY*n);
            PixelPt+= InitPt;

            RayDir= PixelPt - _RenderData.Camera;
            RayDir.Normalize();
            TraceRay(_RenderData.Camera, RayDir, 0, Color1);
            Color+= Color1;
          } //eof
        } else {
          // Calculate Jitters
          DeltaX= _RenderData.StepX * double(1.0/_RenderData.Rays[0]);
          DeltaY= _RenderData.StepY * double(1.0/_RenderData.Rays[1]);

          // Calculate Initial Pt
          InitPt= _RenderData.V_TopL + (DeltaX/2.0) + (DeltaY/2.0);

          // Traditional pixel sub sampling
          for (j= 0; j < _RenderData.Rays[0]; j++) {
            PixelPt= InitPt + (_RenderData.StepX * i) + (_RenderData.StepY * n) + (DeltaY * j);
            for (k= 0; k < _RenderData.Rays[1]; k++) {
              // Determine Location of pixel
              PixelPt+= (DeltaX * (double)k);
              RayDir= PixelPt - _RenderData.Camera;
              RayDir.Normalize();

              TraceRay(_RenderData.Camera, RayDir, 0, Color1);

              Color+= Color1;
            } //eof
          } //eof
        } //fi

        Color= Color * (1.0/(_RenderData.Rays[0]*_RenderData.Rays[1]));

        // Set the Frame Buffer Color

        k= (i+((_RenderData.Height-n-1)*_RenderData.Width))*4;
        _RenderData.Image[k+0]= int(Color.X*255);
        _RenderData.Image[k+1]= int(Color.Y*255);
        _RenderData.Image[k+2]= int(Color.Z*255);
        _RenderData.Image[k+3]= 255;

//        FB -> SetForeground((float) Color.X, (float) Color.Y, (float) Color.Z);
//        FB -> SetPixel(i, n);
      } //fi
    } //eof
  } //eof
//  FB -> MakePPM("test.ppm");
  return(1);
} //eof NRT_RayRender::RenderThread()



void NRT_Render::ComputeLocalColor(Vec3d &Color, Vec3d &RayDir) {
  float	time1,time2;
  time1= clock();
  // Initialize the Color
  Color.Set(0,0,0);

  // Determine the contribution the light has on the object
  _RenderData.Lights -> CalculateLighting(Color,_RenderData.Rays[0]*_RenderData.Rays[1], RayDir);

  // Factor in the Ambient Color
  Color+= _RenderData.Ambient;

  if (Color.X > 1) Color.X= 1;
  if (Color.Y > 1) Color.Y= 1;
  if (Color.Z > 1) Color.Z= 1;
} //eof NRT_Render::ComputerLocalColor()



bool NRT_Render::RayHit(Vec3d &StartPt, Vec3d &RayDir) {
  // Cycle through each of the objects and check to see if there
  // is an intersection by calling the objects respective
  // intersection function.  If there is no intersection then just
  // return 0...
  float	time1,time2;

//  StartPt.Dump();
//  RayDir.Dump();
  time1= clock();
  if (_RenderData.Objects -> FindIntersection(StartPt,RayDir)) {
    return(1);
  } //fi

  return(0);
} //eof NRT_Render::RayHit()



void NRT_Render::CalculateReflection(Vec3d N, Vec3d V, Vec3d &R, double Ref) {
  float	time1,time2;
  time1= clock();

  double	c1;

  // c1 is Dot Product between the Normal and the Ray Direction
  // Reflected Direction is then the RayDir plus (2 * c1)

  c1= -(N * V);
  R= V + (N * 2 * c1);

  // Perturb the ray some...
  Point3d	temp;
  double	rx,ry,rz;

//  rx= Ref*(float)rand()/RAND_MAX;
//  ry= Ref*(float)rand()/RAND_MAX;
//  rz= Ref*(float)rand()/RAND_MAX;

  rx= Ref*2*(float)rand()/RAND_MAX-Ref;
  ry= Ref*2*(float)rand()/RAND_MAX-Ref;
  rz= Ref*2*(float)rand()/RAND_MAX-Ref;

  temp.x= R.X;
  temp.y= R.Y;
  temp.z= R.Z;
  VertexOps::RotateVertex(rx,ry,rz,temp);
  R.X= temp.x;
  R.Y= temp.y;
  R.Z= temp.z;
} //eof NRT_Render::CalculateReflection()



bool NRT_Render::CalculateRefraction(Vec3d &T, Vec3d N, Vec3d I, double n) {
  float	time1,time2;
  time1= clock();

  // Snells law gives us:
  // T= (n(N.I) - sqrt(I - n^2(1 - (N.I)^2)) * N - nI
  // where:
  // T: refracted vector
  // N: Normal
  // I: Ray Direction
  // n: Refraction Index
  // where n= sin(0t)/cos(0i)
  double	NdotI,Radicand;

//  n= (1.0/n);
  NdotI= -(N * I);
  Radicand= 1 - NurbanaMath::Sq(n) * (1 - NurbanaMath::Sq(NdotI));
  // Total internal reflection occurs when the square root
  // is imaginary, or the Radicand is < 0
  if (Radicand < 0)
    return(0);

//  T= N * (n * NdotI - sqrt(Radicand)) - (I * n);
  T= (I * n) + N * (n * NdotI - sqrt(Radicand));
//  T= N * (n * NdotI - sqrt(Radicand)) - (I * n);
  return(1);
} //eof NRT_Render::CalculateReflection()



void NRT_Render::TraceRay(Vec3d &StartPt, Vec3d &RayDir, int Depth, Vec3d &Color) {
  Vec3d		LocalColor,ReflectedColor,ReflectedColor2,RefractionColor;
  Vec3d		ReflectedDirection,RefractedDirection;
  Vec3d		Specular,IPt,IN;
  float		time1,time2;

  LocalColor.Set(0,0,0);
  ReflectedColor.Set(0,0,0);
  ReflectedColor2.Set(0,0,0);
  RefractionColor.Set(0,0,0);

  // if Depth exceeds User defined level set it to black
  if (_RenderData.MaxDepth > -1) {
    if (Depth > _RenderData.MaxDepth) {
      Color.Set(0,0,0);
      return;
    } //fi
  } //fi

  // Check to see if the ray intsects any objects, if it doesn't
  // then set the color to the background color
//  NRT_Object_Base	Object;

  if (RayHit(StartPt,RayDir)) {
    // Calculate contribution of local color at intersection point

    ComputeLocalColor(LocalColor,RayDir);

    // If object is reflective, compute specularity
    if (_RenderData.Objects -> Reflective(_RenderData.Objects -> IntersectedObjectID)) {
      // Fetch Specular Color of Object, Normal
      _RenderData.Objects -> Specular(_RenderData.Objects -> IntersectedObjectID,Specular);
      _RenderData.Objects -> IntersectNormal(_RenderData.Objects -> IntersectedObjectID,IN);
      _RenderData.Objects -> IntersectPt(_RenderData.Objects -> IntersectedObjectID,IPt);

      CalculateReflection(IN,RayDir,ReflectedDirection,_RenderData.Objects -> ReflectiveIndex(_RenderData.Objects -> IntersectedObjectID));

//      ReflectedDirection2= IPt + ReflectedDirection

      TraceRay(IPt, ReflectedDirection, Depth + 1, ReflectedColor);

      ReflectedColor.X= ReflectedColor.X * Specular.X;
      ReflectedColor.Y= ReflectedColor.Y * Specular.Y;
      ReflectedColor.Z= ReflectedColor.Z * Specular.Z;
    } //fi


    if (_RenderData.Objects -> Transparent(_RenderData.Objects -> IntersectedObjectID)) {
      _RenderData.Objects -> IntersectNormal(_RenderData.Objects -> IntersectedObjectID,IN);
      _RenderData.Objects -> IntersectPt(_RenderData.Objects -> IntersectedObjectID,IPt);

      if (CalculateRefraction(RefractedDirection,IN,RayDir,_RenderData.Objects -> RefractiveIndex(_RenderData.Objects -> IntersectedObjectID))) {
        _RenderData.Objects -> IntersectPt(_RenderData.Objects -> IntersectedObjectID,IPt);
        if (RayHit(IPt,RefractedDirection)) {
          if (CalculateRefraction(RefractedDirection,IN,RayDir,_RenderData.Objects -> RefractiveIndex(_RenderData.Objects -> IntersectedObjectID))) {
//            printf("yes\n");
            TraceRay(IPt, RefractedDirection, Depth + 1, RefractionColor);
          } else {
            // Total Internal Reflection Occured
//            printf("bahA\n");
            RefractionColor.Set(0,0,0);
          } //fi
        } //fi
      } else {
        // Total Internal Reflection Occured
//        printf("bahB\n");
        RefractionColor.Set(0,0,0);
      } //fi
    } //fi

    Color= LocalColor + ReflectedColor + RefractionColor;
    if (Color.X > 1) Color.X= 1;
    if (Color.Y > 1) Color.Y= 1;
    if (Color.Z > 1) Color.Z= 1;
    

  } else {
    Color.Set(_RenderData.BGColor[0]/255.0,_RenderData.BGColor[1]/255.0,_RenderData.BGColor[2]/255.0);
  } //fi
} //eof NRT_Render::TraceRay()

void NRT_Render::WriteFile(char *FileName) {
//  FB -> MakePPM(FileName);
} //eof NRT_Render::WriteFile();
