#include "Vector.h"
#include "Matrix.h"
#include "NRT_Object_Manager.h"
#include "NRT_Light_Manager.h"

#ifndef _RenderData_
#define	_RenderData_
struct RenderData {
  bool			Claim;
  bool			Stochastic;
  int			Width,Height;
  int			Rays[2];
  int			StochasticRadius;
  Vec3d			StepX,StepY;

  unsigned	int	*PacketSize;
  unsigned	int	*PacketCount;
  float			*PacketTime;
  float			*PacketTimeStart;
  bool			PacketType;

  bool			*ImageState;

  int			FOV;
  int			MaxDepth;

  Vec3d			Camera;
  Vec3d			CameraFocusPt;
  Vec3d			Ambient;
  Vec3d			V_TopL;

  char			BGColor[3];

  unsigned	char	*Image;

  NRT_Object_Manager	*Objects;
  NRT_Light_Manager	*Lights;
};
#endif
