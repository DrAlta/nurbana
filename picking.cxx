void ModelView::processHitsOne(int hits, int *buffer) {
   int i,Closest[2];

   if (!_ObjectList -> GetMode(OBJECT_MODE_EDIT))
     if (KeyTable::Key() != SDLK_LSHIFT && KeyTable::Key() != SDLK_RSHIFT)
       _ObjectList -> SetObjActAll(0);

   // If there are hits and the shift key is not held down
   if (hits && KeyTable::Key() != SDLK_LSHIFT && KeyTable::Key() != SDLK_RSHIFT) _ObjectList -> SetCPActAll(0);
   if ((!hits) && (!_ObjectList -> GetMode(OBJECT_MODE_EDIT))) _ObjectList -> SetActiveID(0);
   // If we're not in edit-mode assign the Active Object

   // Prevent the Extra ID thrown in for "everything else" from interfering with the # of
   // verts if in edit mode
   Closest[1]= 0;
   for (i= 0; i < hits; i++) {
     if (_ObjectList -> GetMode(OBJECT_MODE_EDIT))
       if (buffer[i*4+3] > _ObjectList -> GetLength(0) * _ObjectList -> GetLength(1))
         i++;

     if (i < hits)
       if ((buffer[i*4+1] < Closest[0] || !Closest[1]) && buffer[i*4+3] != 0) {
         Closest[0]= buffer[i*4+1];
         Closest[1]= buffer[i*4+3];
       } //fi
   } //eof

   if (Closest[1]) {
     if (_ObjectList -> GetMode(OBJECT_MODE_EDIT)) {
       if (KeyTable::Mod(KMOD_SHIFT)) {
         _ObjectList -> SetCPAct(Closest[1]-1,!_ObjectList -> GetCPAct(Closest[1]-1));
       } else {
         _ObjectList -> SetCPAct(Closest[1]-1,1);
       } //fi
     } else {
       _ObjectList -> SetActiveID(Closest[1]);
       if (KeyTable::Key() == SDLK_LSHIFT || KeyTable::Key() == SDLK_RSHIFT) {
         _ObjectList -> SetActive(Closest[1],0);
       } else {
         _ObjectList -> SetActive(Closest[1],1);
       } //fi
     } //fi
   } //fi
} //eof processHitsOne()



void ModelView::processHitsRegion(int hits, int *buffer) {
   int i,SORT_LIST[MAX_OBJECTS][2];

   // If there are hits and the shift key is not held down
   if ((!hits) && (!_ObjectList -> GetMode(OBJECT_MODE_EDIT))) _ObjectList -> SetActiveID(0);
   // If we're not in edit-mode assign the Active Object

   // Distance, ID
   for (i= 0; i < hits; i++) {
     SORT_LIST[i][0]= buffer[i*4+1];
     SORT_LIST[i][1]= buffer[i*4+3];
   } //eof

   for (i= 0; i < hits; i++) {
     if (SORT_LIST[i][1]!=0) {
       if (_ObjectList -> GetMode(OBJECT_MODE_EDIT))  { 
         if (BorderSelect::MouseButton() == SDL_BUTTON_MIDDLE) {
           _ObjectList -> SetCPAct(SORT_LIST[i][1]-1,0);
         } else {
           _ObjectList -> SetCPAct(SORT_LIST[i][1]-1,1);
         } //fi
       } else {
         if (BorderSelect::MouseButton() == SDL_BUTTON_MIDDLE) {
           _ObjectList -> SetActive(0,0);
         } else {
           _ObjectList -> SetActiveID(SORT_LIST[i][1]);
           _ObjectList -> SetActive(1,0);
         } //fi
       } //fi
     } //fi
   } //eof   
} //eof processHitsRegion()


void ModelView::processHitsEdge(int hits, int *buffer) {
   unsigned	int	ID;
   int			i,Closest[2];

   // Prevent the Extra ID thrown in for "everything else" from interfering with the # of
   // verts if in edit mode
   Closest[1]= 0;
   for (i= 0; i < hits; i++) {
     if (i < hits)
       if ((buffer[i*4+1] < Closest[0] || !Closest[1]) && buffer[i*4+3] != 0) {
         Closest[0]= buffer[i*4+1];
         Closest[1]= buffer[i*4+3];
       } //fi
   } //eof

//   printf("Hits: %d, Closest: %d, Object ID: %d, Side: %d\n",hits, Closest[1], Closest[1]/4, Closest[1]%4);

   // Requires integer division
   ID= _ObjectList -> GetActiveID();
   _ObjectList -> SetActiveID(Closest[1]/4);
   _ObjectList -> SetEdge(Closest[1]%4,!_ObjectList -> GetEdge(Closest[1]%4));
   _ObjectList -> SetActiveID(ID);
} //eof ModelView::processHitsEdge()


void ModelView::Select(int x, int y, int SelWidth, int SelHeight, int Type) {
  int	selectBuf[BUFSIZE];
  int	hits;

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0,0,_ZoomLevel);
  rotate(_Angle);
  if (!_SM) glMultMatrixd(_m);
  glTranslatef(_Tran[0],_Tran[1],_Tran[2]);

   
  glSelectBuffer(BUFSIZE, (GLuint*)selectBuf);
  glRenderMode(GL_SELECT);
  glInitNames();
  glPushName(0);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();


   //Selection is based on the viewport sent, and bottom,left=0 basis
//   ProjectUtility::PickMatrix((GLdouble) x, (GLdouble) (ModelView::viewport[3] - y + ModelView::viewport[1]), SelWidth, SelHeight, ModelView::viewport);
  ProjectUtility::PickMatrix((GLdouble) (_Viewport[0] + x), (GLdouble) (_Viewport[3] - y + _Viewport[1]), SelWidth, SelHeight, _Viewport);
  ViewMode();

  if (Type == 2) {
    _DrawC();
  } else {
    _DrawB();
  } //fi

  glPopMatrix();


  hits= glRenderMode(GL_RENDER);
//   printf("hits: %d\n", hits);
  if (hits) {
    switch( Type ) {
      case 0:
        processHitsRegion(hits, selectBuf);
        break;

      case 1:
        processHitsOne(hits, selectBuf);
        break;

      case 2:
        processHitsEdge(hits, selectBuf);
        break;

      default:
        break;
    } //eos
  } //fi
} //eof ModelView::Select()
