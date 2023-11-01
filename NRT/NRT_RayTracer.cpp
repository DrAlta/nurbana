#include "NRT_RayTracer.h"
#include <time.h>


NRT_RayTracer*	NRT_RayTracer::Access= 0;
NRT_RayTracer::NRT_RayTracer(int Width, int Height) {
  _Threads= 2;

  _Width= Width;
  _Height= Height;
//  FB= new ppmGL2d(_Width, _Height);

  _Stochastic= 0;
  _StochasticRadius= 1;
  _Rays[0]= _Rays[1]= 1;

  Objects= new NRT_Object_Manager(_Threads);
  Lights= new NRT_Light_Manager(_Threads);

//  Objects= new NRT_Object_Manager(0);
//  Lights= new NRT_Light_Manager(0);

  // Initialize Row for Rendering
  _Image= (unsigned char*)malloc(sizeof(unsigned char)*_Width*_Height*4);
  _ImageState= (bool*)malloc(sizeof(bool)*_Width*_Height);

  for (int i= 0; i < Width*Height*4; i+= 4) {
    _ImageState[i/4]= 1;
    _Image[i+0]= _Image[i+1]= _Image[i+2]= int(255*(float)rand()/RAND_MAX);
    _Image[i+3]= 255;
  } //eof
//  for (i= 0; i < Width*Height; i++)

  _PacketSize= _Height;
  _PacketCount= 0;
  _PacketTime= 1;
  Access= this;
} //eof NRT_RayTracer::NRT_RayTracer()


NRT_RayTracer::~NRT_RayTracer() {
  free(_Image);
  free(_ImageState);
  delete Objects;
  delete Lights;
} //eof NRT_RayTracer::~NRT_RayTracer()


void NRT_RayTracer::Populate(RenderData &RD) {
  RD.Ambient= _Ambient;

  RD.BGColor[0]= _BGColor[0];
  RD.BGColor[1]= _BGColor[1];
  RD.BGColor[2]= _BGColor[2];

  RD.Camera= _Camera;
  RD.CameraFocusPt= _CameraFocusPt;
  RD.FOV= _FOV;

  RD.Width= _Width;
  RD.Height= _Height;

  RD.PacketSize= &_PacketSize;
  RD.PacketCount= &_PacketCount;
  RD.PacketTime= &_PacketTime;
  RD.PacketTimeStart= &_PacketTimeStart;
  RD.PacketType= _PacketType;

  RD.MaxDepth= _MaxDepth;
  RD.Rays[0]= _Rays[0];
  RD.Rays[1]= _Rays[1];

  RD.StepX= _StepX;
  RD.StepY= _StepY;

  RD.V_TopL= _V_TopL;
  RD.Image= _Image;
  RD.ImageState= _ImageState;

  RD.Stochastic= _Stochastic;
  RD.StochasticRadius= _StochasticRadius;
} //eof NRT_RayTracer::Populate()



typedef	int	(Thread)(void*);
int test(void) {
  if (!NRT_RayTracer::Access -> RD[0].Claim) {
    NRT_RayTracer::Access -> RD[0].Claim= 1;
    NRT_RayTracer::Access -> Render[0].RenderThread();
  } else {
    NRT_RayTracer::Access -> RD[1].Claim= 1;
    NRT_RayTracer::Access -> Render[1].RenderThread();
  } //fi

  return(1);
} //eof test()


bool NRT_RayTracer::RenderScene() {
  Thread	*proc;

  proc= (Thread*)test;

  _PacketCount= 0;
  if (_PacketType)
    _PacketTimeStart= clock()*1.0;

  Populate(RD[0]);
  Populate(RD[1]);

  RD[0].Lights= Lights -> List(0);
  RD[0].Objects= Objects -> List(0);
  RD[0].Lights -> OM(RD[0].Objects);
  RD[0].Claim= 0;
  Render[0].RD(RD[0]);

  RD[1].Lights= Lights -> List(1);
  RD[1].Objects= Objects -> List(1);
  RD[1].Lights -> OM(RD[1].Objects);
  RD[1].Claim= 0;
  Render[1].RD(RD[1]);

  _Thread[0]= SDL_CreateThread(proc,NULL);
  if (_Threads > 1) _Thread[1]= SDL_CreateThread(proc,NULL);

  SDL_WaitThread(_Thread[0],NULL);
  if (_Threads > 1) SDL_WaitThread(_Thread[1],NULL);

  if (!_ImageState[_Width*_Height-1]) {
    return(1);
  } else {
    return(0);
  } //fi
} //eof 


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


void NRT_RayTracer::CameraFOV(int V) {
  _FOV= V;
} //eof NRT_RayTracer::CameraFOV()


void NRT_RayTracer::MaxDepth(int V) {
  _MaxDepth= V;
} //eof NRT_RayTracer::MaxDepth()


void NRT_RayTracer::SetAmbient(double R, double G, double B) {
  _Ambient.X= R;
  _Ambient.Y= G;
  _Ambient.Z= B;
} //eof NRT_RayTracer::SetAmbient()


void NRT_RayTracer::ViewMatrix() {
  // -----------------
  // Setup View Matrix
  // -----------------
  float		dTanX;
  Vec3d		V_BotL, V_TopR, U, V, N, UP;
  Vec3d		DeltaX, DeltaY;

  UP.Set(0,0,1);

  // Determine the Top Left, Bottom Left, and Top Right viewing
  // area in the scene to determine the range the rays need to
  // sweep across
  dTanX= tan(M_PI * _FOV / 360.0);

  _V_TopL.Set(-dTanX,dTanX,-1);
  V_BotL.Set(-dTanX,-dTanX,-1);
  V_TopR.Set(dTanX,dTanX,-1);


  // Create a Viewing (Projection) Matrix
  N= (_Camera - _CameraFocusPt).Normalize();
//  V= (UP - _CameraFocusPt); 
  V= (UP);
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


  _V_TopL= _m * _V_TopL;
  V_BotL= _m * V_BotL;
  V_TopR= _m * V_TopR;

  _StepX= V_TopR - _V_TopL;
  _StepY= V_BotL - _V_TopL;
  _StepX= _StepX / _Width;
  _StepY= _StepY / _Height;

  // Create ResetY Vector for each iteration
//  ResetY= -_StepY * _Height;

  // Set a random seed
  srand((unsigned int) time(NULL));
//  srand(0);
} //eof NRT_RayTracer::ViewMatrix()


void NRT_RayTracer::Size(int Width, int Height) {
  _Width= Width;
  _Height= Height;

  free(_Image);
  free(_ImageState);

  _Image= (unsigned char*)malloc(sizeof(unsigned char)*_Width*_Height*4);
  _ImageState= (bool*)malloc(sizeof(bool)*_Width*_Height);

  ViewMatrix();
} //eof NRT_RayTracer::Size()


void NRT_RayTracer::Reset() {
  int		i;

  for (i= 0; i < 16; i++)
    _time[i]= 0;

  for (i= 0; i < _Width*_Height*4; i+= 4) {
    _ImageState[i/4]= 1;
    _Image[i+0]= _Image[i+1]= _Image[i+2]= int(255*(float)rand()/RAND_MAX);
    _Image[i+3]= 255;
  } //eof

  _PacketCount= 0;
  Access= this;
} //eof NRT_RayTracer::Reset()
