/****************************
 * Window Management Class
 * August 02, 2000
 * Justin Shumaker
 *
 * Description:
 * Manages all Windows
 ****************************/


#ifndef WindowManager_H
#define WindowManager_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../GLUT/glut.h"

#include "../MaterialList.h"

// Load NRT for Rendering
#include "../NRT/NRT_RayTracer.h"


#include "../Objects/Object_Manager.h"
#include "../ModelView.h"
#include "../KeyTable.h"
#include "../BorderSelect.h"

// Load the Header files for the Widgets we'll be using
#include "../NTK/NTK_Window.h"
#include "../NTK/NTK_Button.h"
#include "../NTK/NTK_Container.h"
#include "../NTK/NTK_FileManager.h"
#include "../NTK/NTK_Label.h"
#include "../NTK/NTK_List.h"
#include "../NTK/NTK_Menu.h"
#include "../NTK/NTK_ScrollBar.h"
#include "../NTK/NTK_Slider_Vert.h"
#include "../NTK/NTK_Slider_Horz.h"
#include "../NTK/NTK_TextInput.h"

// ***** DEFINITIONS *****
#define	WM_KEY			0
#define	WM_MOUSE		1
#define	WM_MOTION		2
#define	WM_PASSIVE		3

#define	WM_FULL_WINDOW		0
#define	NTK_WINDOW		1
#define MODELVIEW		2

#define	WM_MODE_ZOOM		0
#define	WM_MODE_TRANSLATE	1
#define	WM_MODE_TRACKBALL	2
#define	WM_MODE_UPDATE_MV	3
#define	WM_MODE_MENU		4
#define	WM_MODE_TEXTINPUT	5
#define	WM_MODE_FILEMANAGER	6

#define	WM_MENU_PADDING		10
// ***********************


class WindowManager {
  // ******************
  // * PUBLIC SECTION *
  // ******************
  public:
    static	WindowManager*	Access() {
      return(_WMself);
    } //eof


    struct SubWindowObject {
      //Only used by internal functions
      int		WID;		//Window ID (Internal use Only)

      //Is the Window of type Widget, ModelView etc..?
      int		Type;		//Type specified in #define section

      //The Object ID of that Type (To connec this linked list to the others)
      int		TID;		//Type ID

      //Groups of aggregate or nonaggregate types can be assigned for simultaneous updating
      int		GID;		//Group ID - if 0 no group

      int		Visible;	//Windows Visibility 0 or 1
      int		Update;

      //Specific Pos/Size in pixels
      int		sPosX;
      int		sPosY;
      int		sSizX;
      int		sSizY;

      //Proportional Pos/Size in % of Screen
      float		pPosX;
      float		pPosY;
      float		pSizX;
      float		pSizY;
  
      //Actual Pos/Size of Window in Pixels (Sum of Specific and Proportional)
      int		APosX;
      int		APosY;
      int		ASizX;
      int		ASizY;

      //Border Existance, and in what direction
      int		TopB;
      int		BotB;
      int		LefB;
      int		RigB;

      SubWindowObject	*next;
    };

    struct NTK_WindowObject {
      int		ID;
      NTK_Window	*Object;
      NTK_WindowObject	*next;
    };

    struct ModelViewObject {
      int		ID;
      ModelView		*Object;
      ModelViewObject	*next;
    };


    struct UpdateQueue {
      int		GID;	//Type ID of Update
      int		SWAP;	//Should the SubWindow SwapBuffers after drawing?
      UpdateQueue	*next;	//Pointer to next UpdateQueue node
      UpdateQueue	*prev;	//Pointer to previous UpdateQueue node
    };


    struct FunctionQueue {
      char		Type;		//Function Type
      int		param[4];	//Function Parameters
      FunctionQueue	*next;		//Pointer to next UpdateQueue node
      FunctionQueue	*prev;		//Pointer to previous UpdateQueue node
    };


    WindowManager();
    ~WindowManager();

    int			WindowID() { return( _WindowID ); }
    void		WindowID(int ID) { _WindowID= ID; }

    void		Visibility(int);
    void		Update();
    void		Reshape(int w, int h);
    void		Key(int x, int y, bool UD);
    void		Mouse(int button, int state, int x, int y);
    void		Motion(int x, int y);
    void		PassiveMotion(int x, int y);
    void		DefaultLayout(int width, int height);
    void		_DefaultLayout(int width, int height, char CommandLineFile[]);

    // Render Window Functions
    void		RenderWindowID(bool V) { _RenderWindowID= V; }
    int			RenderWindowID() { return( _RenderWindowID ); }
    void		RenderUpdate(bool V) { _RenderUpdate= V; }
    bool		RenderUpdate() { return( _RenderUpdate ); }
    void		RenderWindow(bool V) { _RenderWindow= V; }
    bool		RenderWindow() { return( _RenderWindow ); }

    // ***************************************
    // * Required Public, Callback Functions *
    // ***************************************
    UpdateQueue*	GetUpdateQueue(bool HT) { return( _UpdateQueue[HT] ); }
    int			MoveY() { return( _MoveY ); }
    void		SubUpdateQueue(int GID, bool SWAP);
    void		ClearMenu(int pX, int pY, int sX, int sY);
    MaterialList*	GetMaterialList() { return( _MaterialList ); }
    void		InverseVisible(int id, bool on);
    void		ActiveMaterial(int V) { _ACTIVE_MATERIAL= V; }
    int			ActiveMaterial() { return( _ACTIVE_MATERIAL ); }
    int			MousePosition(bool XY) { return( _MOUSE_POSITION[XY] ); }
    SubWindowObject*	GetSubWindowObject(int GID);
    ModelViewObject*	GetGIDModelViewObject(int GID);
    NTK_WindowObject*	GetGIDNTK_WindowObject(int GID);
    Object_Manager*	GetObjectList() { return( _ObjectList ); }

    void		ModeReset(void);
    bool		ModeStatus(void);
    bool		GetMode(int ModeID);
    void		SetMode(char Mode, bool State);

    Focus*		GetFocus() { return( _Focus ); }
    int			Screen(bool V) { return(_ScreenWH[V]); }

  private:
    // *************
    // * Functions *
    // *************
    static		WindowManager	*_WMself;

    int			CreateModelViewObject();
    void		PushFunctionQueue(char Type, int p1, int p2, int p3, int p4);
    void		AddUpdateQueueNode(UpdateQueue *Node);
    void		RemoveNode(UpdateQueue *Node);
    void		PurgeUpdateQueue(void);
    void		SubUpdateDirect(int GID, int Swap);
    void		SetViewport(int GID);
    void		RevertVisible();
    void		ModesOff(int Update);
    void		_Mouse(int button, int state, int x, int y);
    void		_Motion(int x, int y);
    void		_Passive(int x, int y);
    void		AddFunctionQueueNode(FunctionQueue *Node);
    void		RemoveFunctionNode(FunctionQueue *Node);
    int			CreateNTK_WindowObject();
    SubWindowObject*	CreateSubWindow();
    NTK_WindowObject*	GetNTK_WindowObject(int ID);
    ModelViewObject*	GetModelViewObject(int ID);



    // *************
    // * Variables *
    // *************
    UpdateQueue		*_UpdateQueue[2];	// [0] == Head, [1] == Tail

    int			_WindowID;		// GLUT Window ID
    int			_RenderWindowID;	// GLUT Render Window ID

    bool		_RenderWindow;		// Open or Closed
    bool		_RenderUpdate;		// Update the Render Window or Not

    int			_UpdateMax;
    int			_ScreenWH[2];
    int			_MoveY;
    int			_FunctionMax;
    int			_ReshapeDefaultLayout;
    int			_ACTIVE_MATERIAL;
    int			_MOUSE_POSITION[2];
    int			_FunctionCount;
    int			_SUBWINDOW_ID_COUNTER;
    int			_NTK_WINDOW_ID_COUNTER;
    int			_MODELVIEW_ID_COUNTER;

    int			_Mode;
    int			_WindowInfo[10];

    Focus		*_Focus;

    MaterialList	*_MaterialList;
    Object_Manager	*_ObjectList;
    FunctionQueue	*_FunctionQueue[2];

    SubWindowObject	*_SubWindowHead;
    NTK_WindowObject	*_NTK_WindowHead;
    ModelViewObject	*_ModelViewHead;
}; //eof class WindowManager
#endif
