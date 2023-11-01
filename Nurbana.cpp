/*******************************************
 * Nurbana 1.03.1 alpha
 * Start Date: October 1, 2001
 *******************************************/

#include <stdio.h>
#include <stdlib.h>

#include "KeyTable.h"
#include "NWM/NWM.h"
#include <SDL.h>
#include "Cursors/Cursors.h"
#include "NIdle.h"

#include "resource.h"
//--- GLOBALS ------------
NWM		*NWM1;
bool		Reshape= 1;
char		CommandLineFile[256];
int		ScreenWidth= 720, ScreenHeight= 540, BPP= 0, Flags= 0;
//------------------------


void reshape(int width, int height) {
  char		Path[256];

  if (Reshape) {
    // Get Path
    FileIO::GetCwd(Path);
    NWM1 -> Init(width,height,Path,CommandLineFile);
    SDL_WarpMouse(width/2,height/2);
//    glutWarpPointer(width/2,height/2);
    Reshape= 0;
  } //fi

  printf("res: %dx%d\n",width,height);
  if (height > 1) NWM1 -> Reshape(width,height);
} //eof reshape()

void visible(int visible) {
  if (!Reshape) NWM1 -> Visibility(visible);
} //eof visible()

void display(void) {
  NWM1 -> Update();
} //eof display()

void key(SDL_keysym keysym, int x, int y, bool UD) {
  KeyTable::Update(keysym);
  NWM1 -> Key(x,y,UD);
} //eof special()

void mouse(int button, int state, int x, int y) {
//  printf("Mouse: %d,%d,%d,%d,%d,%d\n",button,state,x,y,SDL_MOUSEBUTTONDOWN,SDL_MOUSEBUTTONUP);
  NWM1 -> Mouse(button,state,x,y);
} //eof mouse()

void motion(int x, int y) {
//  printf("Motion: %d,%d\n",x,y);
  NWM1 -> Motion(x,y);
} //eof motion()

void passive(int x, int y) {
  NWM1 -> PassiveMotion(x,y);
} //eof MainPassiveMotion()



void ProcessEvents() {
  SDL_Event		Event;
  static	int	State;
  static	int	X,Y;

  // Assumed that upon SDL_Event instantiation, Event is 0 or NULL
  if (!NIdle::Callback())
    SDL_WaitEvent(&Event);

  do {
    switch( Event.type ) {
      case SDL_QUIT:
        exit(0);
        break;

      case SDL_MOUSEBUTTONDOWN:
      case SDL_MOUSEBUTTONUP:
        mouse(Event.button.button,Event.button.state,Event.button.x,Event.button.y);
        State= Event.button.state;
        break;

      case SDL_MOUSEMOTION:
        if (State) {
          motion(Event.motion.x,Event. motion.y);
        } else {
          X= Event.motion.x;
          Y= Event.motion.y;
          passive(X,Y);
        } //fi
        break;

      case SDL_KEYDOWN:
//        printf("key: %d,%d,%d,%d\n",KMOD_RSHIFT,Event.key.keysym.sym,Event.key.state,(int)Event.key.keysym.unicode);
        key(Event.key.keysym,X,Y,Event.key.state);
        break;

      case SDL_KEYUP:
        KeyTable::KS.sym= SDLK_UNKNOWN;
//        printf("key: %d,%d,%d\n",Event.key.keysym.type,Event.key.state,(int)Event.key.keysym.unicode);
        break;

      case SDL_VIDEORESIZE:
//        printf("new size: %d,%d\n",Event.resize.w,Event.resize.h);
        if (SDL_SetVideoMode(Event.resize.w,Event.resize.h,BPP,Flags) == 0 ) {
          fprintf(stderr, "Video mode set failed: %s\n", SDL_GetError());
        } else {
          ScreenWidth=Event.resize.w;
          ScreenHeight= Event.resize.h;
          reshape(Event.resize.w,Event.resize.h);
        } //fi
        break;

      case SDL_ACTIVEEVENT:
      case SDL_VIDEOEXPOSE:
        visible(1);
        break;

      default:
        break;
    } //eos
  } while(SDL_PollEvent(&Event));

  display();
} //eof ProcessEvents()



int main(int argc, char** argv) {
  const	SDL_VideoInfo	*info= NULL;
  SDL_Surface		*display_info= NULL;
  int			i= 1, Fail= 0;


  // Initialize SDL video subsystem.
  if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
    fprintf( stderr, "Video initialization failed: %s\n", SDL_GetError() );
    exit(0);
  } //fi

  // Get Video Info
  info= SDL_GetVideoInfo();
  BPP= info -> vfmt -> BitsPerPixel;

  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);


  // Parse Arguments at Command Line
  CommandLineFile[0]= 0;
  if (argv[1]) {
    while (argv[i]) {
      if (!strcmp(argv[i],"-p")) {
        i++;
        if (argv[i]) { ScreenWidth= atoi(argv[i]);  i++; } else { Fail= 1; } //fi
        if (argv[i]) { ScreenHeight= atoi(argv[i]); } else { Fail= 1; } //fi
      } else if (!strcmp(argv[i],"-v")) {
        printf("Version:  1.04 alpha\n");
        printf("Released: October 1, 2002\n\n");
        Fail= 2;
      } else if (!strcmp(argv[i],"-f")) {
        i++;
        if (argv[i]) { ScreenWidth= atoi(argv[i]);  i++; } else { Fail= 1; } //fi
        if (argv[i]) { ScreenHeight= atoi(argv[i]); } else { Fail= 1; } //fi
        Flags= SDL_FULLSCREEN;
      } else if (!strcmp(argv[i],"-l")) {
        i++;
        if (argv[i]) {
          strcpy(CommandLineFile,argv[i]);
        } else {
          Fail= 1;
        } //fi
      } else {
        Fail= 1;
      } //fi
      i++;
    } //eow
  } //fi

  Flags|= SDL_OPENGL | SDL_RESIZABLE;


  // Proceed if Arguments were valid
  if (!Fail) {
    NWM1= new NWM();

    SDL_WM_SetCaption("Nurbana 1.04 Alpha",NULL);
    SDL_WM_SetIcon(SDL_LoadBMP("icon.bmp"), NULL);
    SDL_EnableKeyRepeat(500, 30);

    if (SDL_SetVideoMode(ScreenWidth,ScreenHeight,BPP,Flags) == 0 ) {
      fprintf(stderr, "Video mode set failed: %s\n", SDL_GetError());
      exit(0);
    } //fi


    SDL_EnableUNICODE (1);
    Cursors::SetCursor(CURSOR_ARROW);
    NIdle::Init();

    display_info= SDL_GetVideoSurface();

    reshape(display_info -> w,display_info -> h);
    display();

//    printf ("%s\n%s\n%s\n%s", glGetString(GL_VERSION),glGetString(GL_VENDOR),glGetString(GL_RENDERER),glGetString(GL_EXTENSIONS));

    while(1) ProcessEvents();

  } else if (Fail == 1) {
    printf("Website: www.nurbana.cx\n");
    printf("Usage: nurbana [options ...]\n\n");
    printf("Options:\n");
    printf("\t-l\tFilename\t\t\t(Load File)\n");
    printf("\t-p\tWidth Height\t\t\t(Default 720x540)\n");
    printf("\t-f\tWidth Height\t\t\t(Full Screen Mode)\n");
    printf("\t-v\tVersion and Release Date\n");
  } //fi


  return 0;
} //eof main
