#include "NWM_FileManager.h"

// +-----------------+
// | Local Callbacks |
// +-----------------+

void ScrollBarCallback(char E) {
  NWM_FileManager::Access -> _Listing -> Value(NWM_FileManager::Access -> _ScrollBar -> Value());
} //eof ScrollBarCallback()

void TextBoxCallback(char E) {
  // Check for change in Directories by doing strcmp on _Directory[]
  // and what's in _TextBox -> FullText()
  char		TextBoxText[256];

  NWM_FileManager::Access -> _TextBox -> GetText(TextBoxText);
  // If the contents of the TextBox are different from _Directory
  // check to see if the Text within the TextBox contains a new
  // Directory, and if so then update _Directory and _Listing as
  // well.
  if (strcmp(TextBoxText,NWM_FileManager::Access -> _Directory)) {
    // Check TextBox Text if they are a new directory
    NWM_FileManager::Access -> _TextBox -> FullText(TextBoxText);
    if (FileIO::UpdatePath(TextBoxText)) {
      NWM_FileManager::Access -> _TextBox -> FullText(TextBoxText);
      // Add '/' to the end of TextBox if not already there
      if (TextBoxText[strlen(TextBoxText)-1] != 47) {
        NWM_FileManager::Access -> _TextBox -> GetText(TextBoxText);
        strcat(TextBoxText,"/");
      } else {
        NWM_FileManager::Access -> _TextBox -> GetText(TextBoxText);
      } //fi

      // Store new Directory (without leading '/')
      strcpy(NWM_FileManager::Access -> _Directory,TextBoxText);

      // Update TextBox Test
      NWM_FileManager::Access -> _TextBox -> Text(TextBoxText);

      // Update Listing
      NWM_FileManager::Access -> _Listing -> PurgeList();
      while (NWM_FileManager::Access -> _Listing -> Push(FileIO::DirectoryListing())) {} //eow

      // Reset Scroll Bar
      NWM_FileManager::Access -> _ScrollBar -> SmallScroll(2.0/NWM_FileManager::Access -> _Listing -> Total());
      NWM_FileManager::Access -> _ScrollBar -> Value(0);
      NWM_FileManager::Access -> _Listing -> Value(NWM_FileManager::Access -> _ScrollBar -> Value());
    } //fi
  } //fi
} //eof TextBoxCallback()

void ListingCallback(char E) {
  // Update TextBox with _Directory + TextBox -> Selection()
  char		TextBoxText[256],Selection[256];
  int		i;

  strcpy(TextBoxText,NWM_FileManager::Access -> _Directory);
  NWM_FileManager::Access -> _Listing -> Selection(Selection);
  // If ".." was selected, go back a directory
  if (!strcmp("..",Selection)) {
    // Make sure we're not already at ROOT /
    if (strlen(TextBoxText)) {
      // - 2 because /usr/bin/ you wanna get rid of bin"/"
      for (i= strlen(TextBoxText)-2; (i > -1) && (TextBoxText[i] != 47); i--)
        TextBoxText[i]= NULL;
      TextBoxText[i]= NULL; // Lastly remove one more
      strcpy(NWM_FileManager::Access -> _Directory,TextBoxText);
    } //fi
  } else {
    if (strcmp(".",Selection)) // Do nothing on "."
      strcat(TextBoxText,Selection);
  } //fi

  NWM_FileManager::Access -> _TextBox -> Text(TextBoxText);

  // Test to see if _Directory + _Listing -> Selection() is
  // A new directory to update list with
  NWM_FileManager::Access -> _TextBox -> FullText(TextBoxText);
  if (FileIO::UpdatePath(TextBoxText)) {
    // Update _Directory
    NWM_FileManager::Access -> _TextBox -> GetText(TextBoxText);

    // Add '/' to end if there is not already one
    if (strlen(TextBoxText)) {
      if (TextBoxText[strlen(TextBoxText)-1] != 47)
        strcat(TextBoxText,"/");
      strcpy(NWM_FileManager::Access -> _Directory,TextBoxText);
    } //fi

    // Update TextBox Text
    NWM_FileManager::Access -> _TextBox -> Text(TextBoxText);

    // Update Listing
    NWM_FileManager::Access -> _Listing -> PurgeList();
    while (NWM_FileManager::Access -> _Listing -> Push(FileIO::DirectoryListing())) {} //eow

    // Reset Scroll Bar
    NWM_FileManager::Access -> _ScrollBar -> SmallScroll(2.0/NWM_FileManager::Access -> _Listing -> Total());
    NWM_FileManager::Access -> _ScrollBar -> Value(0);
    NWM_FileManager::Access -> _Listing -> Value(NWM_FileManager::Access -> _ScrollBar -> Value());
  } //fi
} //eof ListingCallback()

void ParentCallback(char E) {
  strcpy(NWM_FileManager::Access -> _Directory,"");
  NWM_FileManager::Access -> _TextBox -> Text("");

  // Update Listing
  FileIO::UpdatePath("/");
  NWM_FileManager::Access -> _Listing -> PurgeList();
  while (NWM_FileManager::Access -> _Listing -> Push(FileIO::DirectoryListing())) {} //eow

  // Reset Scroll Bar
  NWM_FileManager::Access -> _ScrollBar -> SmallScroll(2.0/NWM_FileManager::Access -> _Listing -> Total());
  NWM_FileManager::Access -> _ScrollBar -> Value(0);
  NWM_FileManager::Access -> _Listing -> Value(NWM_FileManager::Access -> _ScrollBar -> Value());
} //eof ParentCallback()


void FileManagerCallback(char E) {
} //eof FileManagerCallback()

void LoadCallback(char E) {
  char	FileName[256];

  NWM_FileManager::Access -> _TextBox -> FullText(FileName);
  if (FileName[strlen(FileName)-1] != '/') {
    if (NWM_FileManager::Access -> _NURBS -> Status()) {
      FileIO::OpenFile(FileName, NWM_FileManager::Access -> _ObjectList, NWM_FileManager::Access -> _MaterialList, NURBANA_FILETYPE_NURBS);
    } else if (NWM_FileManager::Access -> _3DM -> Status()) {
      FileIO::OpenFile(FileName, NWM_FileManager::Access -> _ObjectList, NWM_FileManager::Access -> _MaterialList, NURBANA_FILETYPE_3DM);
    } else if (NWM_FileManager::Access -> _DXF -> Status()) {
      FileIO::OpenFile(FileName, NWM_FileManager::Access -> _ObjectList, NWM_FileManager::Access -> _MaterialList, NURBANA_FILETYPE_DXFASC);
    } //fi
  } //fi
  NWM_FileManager::Access -> SelfDestruct(1);
} //eof LoadCallback()

void SaveCallback(char E) {
  char	FileName[256];

  NWM_FileManager::Access -> _TextBox -> FullText(FileName);
  if (FileName[strlen(FileName)-1] != '/') {
    if (NWM_FileManager::Access -> _NURBS -> Status()) {
      FileIO::SaveFile(FileName, NWM_FileManager::Access -> _ObjectList, NWM_FileManager::Access -> _MaterialList, NURBANA_FILETYPE_NURBS);
    } else if (NWM_FileManager::Access -> _3DM -> Status()) {
      FileIO::SaveFile(FileName, NWM_FileManager::Access -> _ObjectList, NWM_FileManager::Access -> _MaterialList, NURBANA_FILETYPE_3DM);
    } else if (NWM_FileManager::Access -> _DXF -> Status()) {
      FileIO::SaveFile(FileName, NWM_FileManager::Access -> _ObjectList, NWM_FileManager::Access -> _MaterialList, NURBANA_FILETYPE_DXFASC);
    } //fi
  } //fi
  NWM_FileManager::Access -> SelfDestruct(1);
} //eof SaveCallback()

void CancelCallback(char E) {
  NWM_FileManager::Access -> SelfDestruct(1);
} //eof CancelCallback()

// +---------------------+
// | EOF Local Callbacks |
// +---------------------+


NWM_FileManager* NWM_FileManager::Access= 0;
NWM_FileManager::NWM_FileManager(int H1, int H2, int H3, int H4, NWM *NWMPtr, NWM_List *obj, Object_Manager *ObjectList, MaterialList *MatList, bool LS, char *Path) : NWM_Base(obj,NWM_WT_FileManager) {
  _self= this;
  Access= _self;

  _Path= (char*)malloc(sizeof(char)*(strlen(Path)+1));
  strcpy(_Path,Path);

  // Remove C:\ for windows
  for (int i= 0; i < strlen(_Path)-2; i++) {
    _Path[i]= _Path[i+3];
    if (_Path[i] == 92) _Path[i]= 47;
  } //eof
  strcat(_Path,"/");

  _NWM= NWMPtr;
  _ObjectList= ObjectList;
  _MaterialList= MatList;

  // Assign Handles
  _H1= H1;
  _H2= H2;
  _H3= H3;
  _H4= H4;

  _FMWindow= new NTK_Window(_NWM -> Path());

  //
  // Configure Widgets
  //
  strcpy(_Directory,"/");
  strcat(_Directory,_Path);
  FileIO::UpdatePath(_Directory);
  strcpy(_Directory,_Path);

  _Button= new NTK_Button(_FMWindow,0,0,100,25);
  _Button -> Type(NTK_BUTTON_TEXT);
  _Button -> Color(0x98,0x98,0x98);
  _Button -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
  if (LS) {
    _Button -> Text("Save File");
    _Button -> Callback((NTK_Callback*)SaveCallback);
  } else {
    _Button -> Text("Load File");
    _Button -> Callback((NTK_Callback*)LoadCallback);
  } //fi
  _Button -> DynPos(NTK_DYN_RIGHT | NTK_DYN_BOTTOM);
  _Button -> DynPosX(150);
  _Button -> DynPosY(10);

  _CancelButton= new NTK_Button(_FMWindow,0,0,100,25);
  _CancelButton -> Type(NTK_BUTTON_TEXT);
  _CancelButton -> Text("Cancel");
  _CancelButton -> Color(0x98,0x98,0x98);
  _CancelButton -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
  _CancelButton -> Callback((NTK_Callback*)CancelCallback);
  _CancelButton -> DynPos(NTK_DYN_RIGHT | NTK_DYN_BOTTOM);
  _CancelButton -> DynPosX(40);
  _CancelButton -> DynPosY(10);

  _Listing= new NTK_List(_FMWindow,0,0,0,0);
  _Listing -> Color(0x98,0x98,0x98);
  _Listing -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
  _Listing -> Callback((NTK_Callback*)ListingCallback);
  _Listing -> PurgeList();
  _Listing -> DynPos(NTK_DYN_LEFT | NTK_DYN_TOP);
  _Listing -> DynPosX(10);
  _Listing -> DynPosY(10);
  _Listing -> DynSiz(NTK_DYN_WIDTH | NTK_DYN_HEIGHT);
  _Listing -> DynSizX(40);
  _Listing -> DynSizY(100);

  _ScrollBar= new NTK_ScrollBar(_FMWindow,0,0,20,200);
  _ScrollBar -> CallbackTrigger(NTK_CT_MOTION | NTK_CT_MOUSE_DOWN);
  _ScrollBar -> Callback((NTK_Callback*)ScrollBarCallback);
  _ScrollBar -> DynPos(NTK_DYN_RIGHT | NTK_DYN_TOP);
  _ScrollBar -> DynPosX(10);
  _ScrollBar -> DynPosY(10);
  _ScrollBar -> DynSiz(NTK_DYN_HEIGHT);
  _ScrollBar -> DynSizY(100);

  _TextBox= new NTK_TextInput(_FMWindow,10,10,0,25);
  _TextBox -> StaticText("/");
  _TextBox -> Text(_Path);
  _TextBox -> Color(0x98,0x98,0x98);
  _TextBox -> CallbackTrigger(NTK_CT_MOUSE_DOWN | NTK_CT_KEY);
  _TextBox -> Callback((NTK_Callback*)TextBoxCallback);
  _TextBox -> DynPos(NTK_DYN_BOTTOM);
  _TextBox -> DynPosY(40);
  _TextBox -> DynSiz(NTK_DYN_WIDTH);
  _TextBox -> DynSizX(65);

  _Parent= new NTK_Button(_FMWindow,0,0,25,25);
  _Parent -> Type(NTK_BUTTON_TEXT);
  _Parent -> Text("P");
  _Parent -> Color(0x80,0x90,0xA0);
  _Parent -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
  _Parent -> Callback((NTK_Callback*)ParentCallback);
  _Parent -> DynPos(NTK_DYN_RIGHT | NTK_DYN_BOTTOM);
  _Parent -> DynPosX(40);
  _Parent -> DynPosY(40);

  _FileType= new NTK_Label(_FMWindow,0,0,100,25);
  _FileType -> Text("File Type:");
  _FileType -> Color(0x66,0x66,0x66);
  _FileType -> DynPos(NTK_DYN_LEFT | NTK_DYN_BOTTOM);
  _FileType -> DynPosX(10);
  _FileType -> DynPosY(10);

  _FileTypeGroup= new NTK_Group();

  _NURBS= new NTK_Button(_FMWindow,0,0,75,25);
  _NURBS -> Status(NTK_ON);
  _NURBS -> Type(NTK_BUTTON_TEXT);
  _NURBS -> Text("nurbs");
  _NURBS -> Color(0x80,0x90,0xA0);
  _NURBS -> Group(_FileTypeGroup);
  _NURBS -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
//  _NURBS -> Callback((NTK_Callback*)ParentCallback);
  _NURBS -> DynPos(NTK_DYN_LEFT | NTK_DYN_BOTTOM);
  _NURBS -> DynPosX(120);
  _NURBS -> DynPosY(10);

  _3DM= new NTK_Button(_FMWindow,0,0,75,25);
  _3DM -> Type(NTK_BUTTON_TEXT);
  _3DM -> Text("3dm");
  _3DM -> Color(0x80,0x90,0xA0);
  _3DM -> Group(_FileTypeGroup);
  _3DM -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
//  _3DM -> Callback((NTK_Callback*)ParentCallback);
  _3DM -> DynPos(NTK_DYN_LEFT | NTK_DYN_BOTTOM);
  _3DM -> DynPosX(195);
  _3DM -> DynPosY(10);

  if (LS) {
  _DXF= new NTK_Button(_FMWindow,0,0,75,25);
  _DXF -> Type(NTK_BUTTON_TEXT);
  _DXF -> Text("DXF");
  _DXF -> Color(0x80,0x90,0xA0);
  _DXF -> Group(_FileTypeGroup);
  _DXF -> CallbackTrigger(NTK_CT_MOUSE_DOWN);
  _DXF -> DynPos(NTK_DYN_LEFT | NTK_DYN_BOTTOM);
  _DXF -> DynPosX(270);
  _DXF -> DynPosY(10);
//    _DXF -> Color(0xA0,0x90,0xA0);
  } //fi
  while (_Listing -> Push(FileIO::DirectoryListing())) {} //eow

  _ScrollBar -> SmallScroll(3.0/_Listing -> Total());
} //eof NWM_FileManager::NWM_FileManager()


NWM_FileManager::~NWM_FileManager() {
  delete _FMWindow;
} //eof NWM_FileManager::~NWM_FileManager()


void NWM_FileManager::Update() {
  glEnable(GL_SCISSOR_TEST);
    glViewport(_Viewport[0],_Viewport[1],_Viewport[2],_Viewport[3]);
    glScissor(_Viewport[0],_Viewport[1],_Viewport[2],_Viewport[3]);
      _FMWindow -> Draw();
  glDisable(GL_SCISSOR_TEST);
} //eof NWM_FileManager::Update()


void NWM_FileManager::Refresh() {
  _FMWindow -> SetClear(2);
  _FMWindow -> UpdateAll();
} //eof NWM_FileManager::Refresh()


void NWM_FileManager::Reshape(int w, int h) {
  // Construct File Manager Viewport
  _Viewport[0]= _PosX;
  _Viewport[1]= h-(_PosY+_SizY);
  _Viewport[2]= _SizX;
  _Viewport[3]= _SizY;

  _FMWindow -> ScreenSize(_SizX,_SizY);
} //eof NWM_FileManager::Reshape()


char NWM_FileManager::Key(int x, int y) {
  _FMWindow -> Handle(NTK_EVENT_KEY,x,y,0,0);
  _Update= 2;

  return(NWM_NOTHING);
} //eof NWM_FileManager::Key()


char NWM_FileManager::Mouse(int button, int state, int x, int y) {
  _FMWindow -> Handle(NTK_EVENT_MOUSE,button,state,x,y);
  _Update= 2;

  if (state) {
    return(NWM_LOCK);
  } else {
    if (!_FMWindow ->TypeActive(NTK_WT_TextInput))
      return(NWM_UNLOCK);
  } //fi

  return(NWM_NOTHING);
} //eof NWM_FileManager::Mouse()


char NWM_FileManager::Motion(int x, int y) {
  _FMWindow -> Handle(NTK_EVENT_MOTION,x,y,0,0);
  _Update= 1;

  return(NWM_NOTHING);
} //eof NWM_FileManager::Motion()


char NWM_FileManager::Passive(int x, int y) {
  return(NWM_NOTHING);
} //eof NWM_FileManager::Passive()


void NWM_FileManager::Active(bool A) {
} //eof NWM_FileManager::Active()
