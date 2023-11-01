/****************************************************
 * NRT - Ray Tracer
 * Start Date: Jul 8, 2001
 * Author: Justin Shumaker
 *
 * Description:
 *
 ****************************************************/


#ifndef NRT_RayTracer_H
#define NRT_RayTracer_H

#include <stdlib.h>
#include <time.h>

#include <SDL.h>
#include <SDL_thread.h>

#include <math.h>
//#include "ppmGL2d.h"
#include "NRT_Common.h"
#include "NRT_Render.h"

#ifndef M_PI
#define	M_PI	3.14159265358979323846
#endif

class NRT_RayTracer {
  public:
    NRT_RayTracer(int Width, int Height);
    ~NRT_RayTracer();


    static	NRT_RayTracer	*Access;
    bool			RenderScene();

    void			Threads(unsigned short T) { _Threads= T; }
    void			PacketSize(unsigned int P) { _PacketSize= P; _PacketType= 0; }
    void			PacketTime(float T) { _PacketTime= T; _PacketType= 1; }

    bool			RayHit(Vec3d &StartPt, Vec3d &RayDir);

    unsigned	char*		Image() { return( _Image ); }

    void			BGColor(char R, char G, char B);
    void			Camera(float X, float Y, float Z);
    void			CameraFocusPt(float X, float Y, float Z);
    void			CameraFOV(int V);
    void			MaxDepth(int V);
    void			SetAmbient(double R, double G, double B);

    void			Rays(int R) { _Rays[0]= _Rays[1]= R; }
    void			ViewMatrix();
    void			Size(int Width, int Height);
    int				Size(bool WH) { if (WH) { return( _Height ); } else { return( _Width ); } }

    void			Reset();
    NRT_Object_Manager		*Objects;
    NRT_Light_Manager		*Lights;

    RenderData			RD[2];
    NRT_Render			Render[2];

  private:
    // -----------------
    // --- Functions ---
    // -----------------
    void			Populate(RenderData &RD);

    // ------------
    // --- Vars ---
    // ------------
    unsigned	short		_Threads;
    unsigned	int		_PacketSize;
    unsigned	int		_PacketCount;
    float			_PacketTime;
    float			_PacketTimeStart;
    bool			_PacketType;

    bool			_Stochastic;
    double			_StochasticRadius;

    int				_Width;
    int				_Height;

    int				_Rays[2];		// # of rays in X and Y direction

    int				_FOV;
    int				_MaxDepth;

    char			_BGColor[3];

    Mat44d			_m;

    Vec3d			_Camera;
    Vec3d			_CameraFocusPt;
    Vec3d			_Ambient;

//    ppmGL2d*		FB;
    unsigned	char		*_Image;
    bool			*_ImageState;

    SDL_Thread			*_Thread[2];


    Vec3d			_StepX,_StepY,_V_TopL;
    float			_time[16];
}; //eof class NRT_RayTracer
#endif
