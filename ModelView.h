/*===========================================
 * Title: Nurbana View Implementation Class
 * Start Date: 01-25-2000
 * Version: who knows?
 * Author: Justin Shumaker
 *===========================================
 */

#ifndef ModelView_H
#define ModelView_H

#include <stdio.h>
#include <stdlib.h>

//#include "GLUT/glut.h"

#include "NDefs/NWMDefs.h"

#include "Objects/Object_Manager.h"
#include "Objects/Object_NURBS.h"
#include "Objects/Object_Bezier.h"

#include "BorderSelect.h"
#include "KeyTable.h"
#include "NurbanaMath.h"
#include "NTimer.h"
#include "ProjectUtility.h"
#include "Trackball.h"
#include "VertexOps.h"
#include "Cursors/Cursors.h"


#define ON			1
#define	OFF			0

#define	MV_GRID			1
#define	MV_KEY_ROTATION_ANGLE	10

#define	BUFSIZE			2048
#define	MAX_OBJECTS		4096
#define	Z_CLIP_NEAR_MAX		.1
#define	Z_CLIP_FAR_MAX		10000
#define	GRID_XY			64
#define	GRID_SIZE		(GRID_XY+1)*4


class ModelView {
  public:
    ModelView(Object_Manager *ObjectList);
    ~ModelView();

    void	Draw();
    void	Reshape(int,int);
    char	Mouse(int button, int state, int x, int y);
    char	Motion(int x, int y);
    char	Passive(int x, int y);
    char	Key(int x,int y);
    void	Init();

    void	MouseZoom(int Z);
    void	MouseTranslate(int x, int y, bool init);
    void	MouseGrab(int,int);
    void	MouseRotate(int,int);
    void	MouseSize(int,int);
    void	MouseBorderSelect(int Data[4]);

    void	ScreenSize(int w, int h) { _ScreenWidth= w; _ScreenHeight= h; }
    void	Viewport(int V0, int V1, int V2, int V3) { _Viewport[0]= V0; _Viewport[1]= V1; _Viewport[2]= V2; _Viewport[3]= V3; }



  private:
    // *************
    // * FUNCTIONS *
    // *************
    void		Translate(int TransID, int x, int y);
    void		ViewMode();
    void		Scene();
    void		_DrawA();
    void		_DrawB();
    void		_DrawC();
    void		Select(int x, int y, int SelWidth, int SelHeight, int Type);
    void		Zoom(float level);

    void		processHitsOne(int hits, int *buffer);
    void		processHitsRegion(int hits, int *buffer);
    void		processHitsEdge(int hits, int *buffer);

    void		Grid();
    void		GenerateGrid();

    // *************
    // * VARIABLES *
    // *************
    int			_SM;
    bool		_zBuffer;
    bool		_View;
    bool		_Axes;

    int			_ScreenHeight;
    int			_ScreenWidth;
    int			_Viewport[4];

    float		_Last[2][3];

    float		_ZoomLevel;
    float		_Z_CLIP_NEAR;
    float		_Z_CLIP_FAR;
    float		_AxesV[6][3];
    float		_GridV[GRID_SIZE][3];

    float		_Tran[3];
    double		_m[16];
    double		_Angle[3];

    Object_Manager	*_ObjectList;

}; //eof ModelView class
#endif
