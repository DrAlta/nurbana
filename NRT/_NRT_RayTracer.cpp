#include "NRT_RayTracer.h"
#include <time.h>


NRT_RayTracer::NRT_RayTracer(int Width, int Height) {
  _Width= Width;
  _Height= Height;
  FB= new ppmGL2d(_Width, _Height);
  _m.Identity();
  _Stochastic= 0;
  _StochasticRadius= 1;
  _Jitter[0]= _Jitter[1]= 2;

  Objects= new NRT_Object_Manager;
  Lights= new NRT_Light_Manager(Objects);
} //eof NRT_RayTracer::NRT_RayTracer()


NRT_RayTracer::~NRT_RayTracer() {}


void NRT_RayTracer::RenderScene() {
  int		i,n,j,k;
  float		dTanX;
  Vec3d		V_TopL, V_BotL, V_TopR, U, V, N, UP;
  Vec3d		StepX, StepY, DeltaX, DeltaY;
  Vec3d		InitPt, PixelPt, ResetY;
  Vec3d		RayPt, RayDir, Color1, Color2, Color3, Color4;
  Vec3d		Color;

  UP.Set(0,0,1);

  // Determine the Top Left, Bottom Left, and Top Right viewing
  // area in the scene to determine the range the rays need to
  // sweep across
  dTanX= tan(M_PI * _FOV / 360.0);

  V_TopL.Set(-dTanX,dTanX,-1);
  V_BotL.Set(-dTanX,-dTanX,-1);
  V_TopR.Set(dTanX,dTanX,-1);


  // Create a Viewing (Projection) Matrix
  N= (_Camera - _CameraFocusPt).Normalize();
  V= (UP - _CameraFocusPt); 
  V-= N * (V * N);
  V.Normalize();
  U= V/N;

  _m.Identity();

  _m.M[0]=  U.X;
  _m.M[1]=  U.Y;
  _m.M[2]=  U.Z;
  _m.M[4]=  V.X;
  _m.M[5]=  V.Y;
  _m.M[6]=  V.Z;
  _m.M[8]=  N.X;
  _m.M[9]=  N.Y;
  _m.M[10]= N.Z;
  _m.M[3]=  -(U * _Camera);
  _m.M[7]=  -(V * _Camera);
  _m.M[11]= -(N * _Camera);
    _m.Transpose();

  // Multiply V_TopL, V_BotL, V_TopR by Inverted Projection Matrix
  _m.Invert();


  V_TopL= _m * V_TopL;
  V_BotL= _m * V_BotL;
  V_TopR= _m * V_TopR;

  StepX= V_TopR - V_TopL;
  StepY= V_BotL - V_TopL;
  StepX= StepX / _Width;
  StepY= StepY / _Height;

  // Create ResetY Vector for each iteration
  ResetY= -StepY * _Height;

  // Set a random seed
  srand((unsigned int) time(NULL));
//  srand(0);

  // Trace n Jittered Rays through each pixel
  for (i= 0; i < _Width; i++) {
    printf("[H[2J");
    printf("[%d/%d]\n",i,_Width);
    for (n= 0; n < _Height; n++) {


      if (_Stochastic) {
        // Calculate Jitters
        DeltaX= StepX;
        DeltaY= StepY;

        // Calculate Initial Pt
        InitPt= V_TopL + (DeltaX/2.0) + (DeltaY/2.0);


        // From center of pixel, shoot out
        // # of defined rays randomly radius (_StochasticRadius)
        for (j= 0; j < _Jitter[0] * _Jitter[1]; j++) {
          PixelPt= StepX*(rand()/NRT_RMI)*_StochasticRadius;
          PixelPt+= StepY*(rand()/NRT_RMI)*_StochasticRadius;
          if (int((rand()*2)/NRT_RMI)%2) PixelPt.X*= -1;
          if (int((rand()*2)/NRT_RMI)%2) PixelPt.Y*= -1;
          PixelPt+= (StepX*i);
          PixelPt+= (StepY*n);
          PixelPt+= InitPt;

          RayDir= PixelPt - _Camera;
          RayDir.Normalize();
          TraceRay(_Camera, RayDir, 0, Color1);
          Color+= Color1;
        } //eof
      } else {
        // Calculate Jitters
        DeltaX= StepX * double(1.0/_Jitter[0]);
        DeltaY= StepY * double(1.0/_Jitter[1]);

        // Calculate Initial Pt
        InitPt= V_TopL + (DeltaX/2.0) + (DeltaY/2.0);


        // Traditional equal intervals
        for (j= 0; j < _Jitter[0]; j++) {
          PixelPt= InitPt + (StepX * i) + (StepY * n) + (DeltaY * j);
          for (k= 0; k < _Jitter[1]; k++) {
            // Determine Location of pixel
            PixelPt+= (DeltaX * (double)k);
            RayDir= PixelPt - _Camera;
            RayDir.Normalize();
            TraceRay(_Camera, RayDir, 0, Color1);
            Color+= Color1;
          } //eof
        } //eof
      } //fi

      Color= Color * (1.0/(_Jitter[0]*_Jitter[1]));

      // Set the Frame Buffer Color
      FB -> SetForeground((float) Color.X, (float) Color.Y, (float) Color.Z);
      FB -> SetPixel(i, n);
      Color.Set(0,0,0);
    } //eof

  } //eof
} //eof NRT_RayTracer::RenderScene()



void NRT_RayTracer::ComputeLocalColor(Vec3d &Color) {
  // Initialize the Color
  Color.Set(0,0,0);

  // Determine the contribution the light has on the object
  Lights -> CalculateLighting(Color);

  // Factor in the Ambient Color
  Color+= _Ambient;

  if (Color.X > 1) Color.X= 1;
  if (Color.Y > 1) Color.Y= 1;
  if (Color.Z > 1) Color.Z= 1;
} //eof NRT_RayTracer::ComputerLocalColor()



bool NRT_RayTracer::RayHit(Vec3d &StartPt, Vec3d &RayDir) {
  // Cycle through each of the objects and check to see if there
  // is an intersection by calling the objects respective
  // intersection function.  If there is no intersection then just
  // return 0...

  if (Objects -> FindIntersection(StartPt,RayDir)) {
    return(1);
  } //fi

  return(0);
} //eof NRT_RayTracer::RayHit()



void CalculateReflection(Vec3d N, Vec3d V, Vec3d &R) {
  double	c1;

  // c1 is Dot Product between the Normal and the Ray Direction
  // Reflected Direction is then the RayDir plus (2 * c1)

  c1= -(N * V);
  R= V + (N * 2 * c1);
} //eof CalculateReflection()


bool CalculateRefraction(Vec3d &T, Vec3d N, Vec3d I, double n) {
  // Snells law gives us:
  // T= (n(N.I) - sqrt(I - n^2(1 - (N.I)^2)) * N - nI
  // where:
  // T: refracted vector
  // N: Normal
  // I: Ray Direction
  // n: Refraction Index
  // where n= sin(0t)/cos(0i)
  double	NdotI,Radicand;

  NdotI= -(N * I);
  Radicand= 1 - NurbanaMath::Sq(n) * (1 - NurbanaMath::Sq(NdotI));
  // Total internal reflection occurs when the square root
  // is imaginary, or the Radicand is < 0
  if (Radicand < 0)
    return(0);

  T= N * (n * NdotI - sqrt(Radicand)) - (I * n);
  return(1);
} //eof CalculateReflection()



void NRT_RayTracer::TraceRay(Vec3d &StartPt, Vec3d &RayDir, int Depth, Vec3d &Color) {
  Vec3d		LocalColor,ReflectedColor,ReflectedColor2,RefractionColor;
  Vec3d		ReflectedDirection,RefractedDirection;
  Vec3d		Specular,IPt,IN;

  // if Depth exceeds User defined level set it to black
  if (_MaxDepth > -1) {
    if (Depth > _MaxDepth) {
      Color.Set(0,0,0);
      return;
    } //fi
  } //fi

  // Check to see if the ray intsects any objects, if it doesn't
  // then set the color to the background color
//  NRT_Object_Base	Object;

  if (RayHit(StartPt,RayDir)) {
    // Calculate contribution of local color at intersection point
    ComputeLocalColor(LocalColor);

    // If object is reflective, compute specularity
    if (Objects -> Reflective(Objects -> IntersectedObjectID)) {
      // Fetch Specular Color of Object, Normal
      Objects -> Specular(Objects -> IntersectedObjectID,Specular);
      Objects -> IntersectNormal(Objects -> IntersectedObjectID,IN);
      Objects -> IntersectPt(Objects -> IntersectedObjectID,IPt);

      CalculateReflection(IN,RayDir,ReflectedDirection);

//      ReflectedDirection2= IPt + ReflectedDirection

      TraceRay(IPt, ReflectedDirection, Depth + 1, ReflectedColor);



      ReflectedColor.X= ReflectedColor.X * Specular.X;
      ReflectedColor.Y= ReflectedColor.Y * Specular.Y;
      ReflectedColor.Z= ReflectedColor.Z * Specular.Z;
    } //fi


    if (Objects -> Transparent(Objects -> IntersectedObjectID)) {
      Objects -> IntersectNormal(Objects -> IntersectedObjectID,IN);
      Objects -> IntersectPt(Objects -> IntersectedObjectID,IPt);

      if (CalculateRefraction(RefractedDirection,IN,RayDir,Objects -> RefractiveIndex(Objects -> IntersectedObjectID))) {
        TraceRay(IPt, RefractedDirection, Depth + 1, RefractionColor);
      } else {
        // Total Internal Reflection Occured
        RefractionColor.Set(0,0,0);
      } //fi
    } //fi

    Color= LocalColor + ReflectedColor + RefractionColor;
    if (Color.X > 1) Color.X= 1;
    if (Color.Y > 1) Color.Y= 1;
    if (Color.Z > 1) Color.Z= 1;
    

  } else {
    Color.Set(_BGColor[0]/255.0,_BGColor[1]/255.0,_BGColor[2]/255.0);
  } //fi
} //eof NRT_RayTracer::TraceRay()


void NRT_RayTracer::BGColor(char R, char G, char B) {
  _BGColor[0]= R;
  _BGColor[1]= G;
  _BGColor[2]= B;
} //eof NRT_RayTracer::BGColor()


void NRT_RayTracer::Camera(float X, float Y, float Z) {
  _Camera.Set(X,Y,Z);
} //eof NRT_RayTracer::Camera()


void NRT_RayTracer::CameraFocusPt(float X, float Y, float Z) {
  _CameraFocusPt.Set(X,Y,Z);
} //eof NRT_RayTracer::CameraFocusPt()


void NRT_RayTracer::FOV(int V) {
  _FOV= V;
} //eof NRT_RayTracer::FOV()


void NRT_RayTracer::MaxDepth(int V) {
  _MaxDepth= V;
} //eof NRT_RayTracer::MaxDepth()


void NRT_RayTracer::SetAmbient(double R, double G, double B) {
  _Ambient.X= R;
  _Ambient.Y= G;
  _Ambient.Z= B;
} //eof NRT_RayTracer::SetAmbient()


void NRT_RayTracer::WriteFile(char *FileName) {
  int	i,n;

  FB -> MakePPM(FileName);
} //eof NRT_RayTracer::WriteFile();
