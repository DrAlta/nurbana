/*************************************
 * Author: Justin Shumaker
 * Date: 07-08-2001
 * Nurbana Stochastic Ray Tracer
 *************************************/

#include <stdio.h>
#include <stdlib.h>
#include "NRT_RayTracer.h"

NRT_RayTracer RT(256,256);

void DrawScene() {
    RT.Camera(1, -6, 3);
    RT.CameraFocusPt(0, 0, 0);

//    RT.SetUpVector(0, 0, 1);
    RT.FOV(52);

    RT.BGColor(0x00, 0x33, 0x66);

    RT.Lights -> CreateLight(1,NRT_LIGHT_SPHERE,0,-2,4);
    RT.Lights -> Radius(1,.1);
    RT.Lights -> Intensity(1,1,1,1);

//    RT.Lights -> CreateLight(2,NRT_LIGHT_SPHERE,3,3,3);
//    RT.Lights -> Radius(2,.1);
//    RT.Lights -> Intensity(2,0,.8,0);

//    RT.Lights -> CreateLight(3,NRT_LIGHT_SPHERE,-3,3,3);
//    RT.Lights -> Radius(3,.1);
//    RT.Lights -> Intensity(3,0,0,.8);

//    RT.Lights -> CreateLight(4,NRT_LIGHT_SPHERE,0,0,2);
//    RT.Lights -> Radius(4,.1);
//    RT.Lights -> Intensity(4,.25,.25,25);

    RT.SetAmbient(0.0, 0.0, 0.0);

//    RT.Objects -> CreateObject(1,NRT_OBJECT_SPHERE);
//    RT.Objects -> Position(1,-1,-0.75,0.5);
//    RT.Objects -> Radius(1,.25);
//    RT.Objects -> Diffuse(1,1,.5,.5);
//    RT.Objects -> Specular(1,.5,.5,.5);
//    RT.Objects -> Reflective(1,1);

//    RT.Objects -> CreateObject(2,NRT_OBJECT_SPHERE);
//    RT.Objects -> Position(2,1,-0.75,0.5);
//    RT.Objects -> Radius(2,.25);
//    RT.Objects -> Diffuse(2,.5,1,.5);
//    RT.Objects -> Specular(2,.5,.5,.5);
//    RT.Objects -> Reflective(2,1);

//    RT.Objects -> CreateObject(8,NRT_OBJECT_SPHERE);
//    RT.Objects -> Position(8,0,1.25,0.5);
//    RT.Objects -> Radius(8,.25);
//    RT.Objects -> Diffuse(8,.5,.5,1);
//    RT.Objects -> Specular(8,.5,.5,.5);
//    RT.Objects -> Reflective(8,1);

//    RT.Objects -> CreateObject(3,NRT_OBJECT_SPHERE);
//    RT.Objects -> Position(3,0.0,-1,1.25);
//    RT.Objects -> Radius(3,.5);
//    RT.Objects -> Diffuse(3,.15,.15,.15);
//    RT.Objects -> Transparent(3,1);
//    RT.Objects -> RefractiveIndex(3,.666);

    RT.Objects -> CreateObject(3,NRT_OBJECT_NURBS);
    RT.Objects -> Diffuse(3,1,0,0);
//    RT.Objects -> Specular(3,.50,.50,.50);
//    RT.Objects -> Reflective(3,1);

    RT.Objects -> CreateObject(4,NRT_OBJECT_TRIANGLE);
    RT.Objects -> Diffuse(4,1.0,1.0,1.0);
    RT.Objects -> SetPt(4,0,3,-4,-.25);
    RT.Objects -> SetPt(4,1,0,4,-.25);
    RT.Objects -> SetPt(4,2,-3,-4,-.25);
    RT.Objects -> Specular(4,.25,.25,.25);
    RT.Objects -> Reflective(4,1);

} //eof DrawScene()


int main(void) {
  RT.MaxDepth(5);
//  RT.SetConstantAtt(1.0);
//  RT.SetLinearAtt(0.01);

  DrawScene();

  RT.RenderScene();

  RT.WriteFile("image.ppm");

  return(1);
} //eof main
