/****************************************************
 * NRT - Ray Tracer
 * Start Date: Jul 8, 2001
 * Author: Justin Shumaker
 *
 * Description:
 *
 ****************************************************/


#ifndef NRT_Render_H
#define NRT_Render_H

#include <stdlib.h>
#include <time.h>
#include <math.h>

//#include "ppmGL2d.h"
#include "NRT_Common.h"


class NRT_Render {
  public:
    NRT_Render();
    ~NRT_Render();


    int				RenderThread();

    bool			RayHit(Vec3d &StartPt, Vec3d &RayDir);

    void			ComputeLocalColor(Vec3d &Color, Vec3d &RayDir);

    void			WriteFile(char *FileName);

    void			RD(RenderData V) { _RenderData= V; }

  private:
    // -----------------
    // --- Functions ---
    // -----------------
    void			CalculateReflection(Vec3d N, Vec3d V, Vec3d &R, double Ref);
    bool			CalculateRefraction(Vec3d &T, Vec3d N, Vec3d I, double n);
    void			TraceRay(Vec3d &StartPt, Vec3d &RayDir, int Depth, Vec3d &Color);


    // ------------
    // --- Vars ---
    // ------------
    RenderData			_RenderData;

}; //eof class NRT_Render
#endif
