#include "NTK_Text.h"
#include "../FileIO/FileIO.h"


NTK_Text::NTK_Text(char *Path) {
  FILE			*fileptr= NULL;
  int			Pts[6], i, n;
  Descriptor		Block;
  FontDNode		*tmp;
  char			*TPath;
  short			ConvertEndian;
  Endian::EV		e;
  unsigned	char	ec;


  e.i= 1;

  for (i= 0; i < 128; i++) _Text[i]= NULL;

  // Get Current Working Path
  TPath= (char*)malloc(sizeof(char)*(strlen(Path)+8));
  strcpy(TPath,Path);
  strcat(TPath,"/.Nfont");
  fileptr= fopen(TPath,"rb");

  if (fileptr) {
    // Read in first 2 bytes [short], Endian footprint
    fread(&ec,sizeof(unsigned char),1,fileptr);
    ConvertEndian= (e.c != ec);

    for (i= 0; i < 128; i++) {
      _Text[i]= NULL;
      fread(&Block,sizeof(Block),1,fileptr);
      if (ConvertEndian) {
        Endian::RB(Block.Pieces);
        Endian::RB(Block.P1X);
        Endian::RB(Block.P2X);
        Endian::RB(Block.P1Y);
        Endian::RB(Block.P2Y);
      } //fi

      for (n= 0; n < Block.Pieces; n++) {
        if (_Text[i]) {
          tmp= _Text[i];
          _Text[i]= NULL;
          _Text[i]= new FontDNode;
          _Text[i] -> Next= tmp;
        } else {
          _Text[i]= new FontDNode;
          _Text[i] -> Next= NULL;
        } //fi

        fread(&Pts,sizeof(int),6,fileptr);
        if (ConvertEndian) Endian::RB(Pts[0]);
        if (ConvertEndian) Endian::RB(Pts[1]);
        if (ConvertEndian) Endian::RB(Pts[2]);
        if (ConvertEndian) Endian::RB(Pts[3]);
        if (ConvertEndian) Endian::RB(Pts[4]);
        if (ConvertEndian) Endian::RB(Pts[5]);

        _Text[i] -> Pts[0][0]= Pts[0];
        _Text[i] -> Pts[0][1]= Pts[1];

        _Text[i] -> Pts[1][0]= Pts[2];
        _Text[i] -> Pts[1][1]= Pts[3];

        _Text[i] -> Pts[2][0]= Pts[4];
        _Text[i] -> Pts[2][1]= Pts[5];
      } //eof

      // This is put on last because this is a LIFO Stack
      if (_Text[i]) {
        tmp= _Text[i];
        _Text[i]= NULL;
        _Text[i]= new FontDNode;
        _Text[i] -> Next= tmp;
      } else {
        _Text[i]= new FontDNode;
        _Text[i] -> Next= NULL;
      } //fi

      _Text[i] -> Pts[0][0]= Block.P1X;
      _Text[i] -> Pts[0][1]= Block.P1Y;
      _Text[i] -> Pts[1][0]= Block.P2X;
      _Text[i] -> Pts[1][1]= Block.P2Y;
    } //eof
    fclose(fileptr);
  } //fi

  free(TPath);
  _FontCacheHead= NULL;
} //eof NTK_Text::NTK_Text()



NTK_Text::~NTK_Text() {
  FontCacheNode		*fcn;
  FontDNode		*cur;
  int			i;

  // Purge Textures
  while (_FontCacheHead) {
    // Unbind Texture from Video Memory
    for (i= 0; i < 128; i++)
      glDeleteTextures(1,&_FontCacheHead -> TexIDS[i]);

    // Delete Node
    fcn= _FontCacheHead -> Next;
    delete _FontCacheHead;
    _FontCacheHead= fcn;
  } //eof

  // Purge Text Data
  for (i= 0; i < 128; i++) {
    while (_Text[i]) {
      cur= _Text[i] -> Next;
      delete _Text[i];
      _Text[i]= cur;
    } //eof
  } //eof
} //eof NTK_Text::~NTK_Text()



void NTK_Text::GenerateFont(int W, int H) {
  FontCacheNode		*fcn;
  FontDNode		*cur;
  Point3d		*BP,*BezierCurve;
  unsigned	char	*Texture,*CharTex;
  int			i,n,k,P1X,P1Y,P2X,P2Y,TexCW,TexCH;

  if (_FontCacheHead) {
    fcn= _FontCacheHead;
    _FontCacheHead= new FontCacheNode;
    _FontCacheHead -> Next= fcn;
  } else {
    _FontCacheHead= new FontCacheNode;
    _FontCacheHead -> Next= NULL;
  } //fi
  _FontCacheHead -> FontSize= H;

  // Allocate memory for BP and BezierCurve
  BP= (Point3d*)malloc(sizeof(Point3d)*3);
  BezierCurve= (Point3d*)malloc(sizeof(Point3d)*(H/2));

  // Allocate space for texture
  TexCW= 1; while (TexCW < W) TexCW*= 2;
  TexCH= 1; while (TexCH < H) TexCH*= 2;

  Texture= (unsigned char*)malloc(sizeof(unsigned char)*TexCW*TexCH);
  CharTex= (unsigned char*)malloc(sizeof(unsigned char)*W*H);


  // Generate characters 0 - 127 and place into Texture
  for (i= 0; i < 128; i++) {
    for (n= 0; n < W*H; n++)
      CharTex[n]= 0;

    for (n= 0; n < TexCW*TexCH; n++)
      Texture[n]= 0;

    cur= _Text[i];

    if (cur) {
      //Extract Boundary Info
      P1X= cur -> Pts[0][0];
      P1Y= cur -> Pts[0][1];
      P2X= cur -> Pts[1][0];
      P2Y= cur -> Pts[1][1];

      cur= cur -> Next;

      // For each piece
      while (cur) {
        BP[0].z= BP[1].z= BP[2].z= 0;
        for (n= 0; n < 3; n++) {
          BP[n].x= cur -> Pts[n][0];
          BP[n].y= cur -> Pts[n][1];

          BP[n].x-= P1X;
          BP[n].y-= P1Y;

          BP[n].x*= double(W)/double(P2X-P1X);
          BP[n].y*= double(H)/double(P2Y-P1Y);
        } //eof

        // Calculate Curve
        Bezier::GetBezierCurve(3, BP, H/2, BezierCurve);

        // Render Curve
        for (n= 0; n < H/2-1; n++)
          ntku_Bresenham::Render(CharTex,W,H,int(BezierCurve[n].x),int(BezierCurve[n].y),int(BezierCurve[n+1].x),int(BezierCurve[n+1].y),255);

        // Assign CharTex to Texture
        k= 0;
        for (n= 0; n < W*H; n++) {
          Texture[k]= CharTex[n];
          if (!(n%W)) {
            if (n)
              k+= TexCW-W+1;
          } else {
            k++;
          } //fi
        } //eof
        cur= cur -> Next;
      } //eow
    } //fi

       // Now that Texture has been generated throw it in the Texture Memory
       // and store the Texture ID in the new Node
       glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
       glGenTextures(1, &_FontCacheHead -> TexIDS[i]);
       glBindTexture(GL_TEXTURE_2D, _FontCacheHead -> TexIDS[i]);

       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);

       glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, TexCW, TexCH, 0, GL_ALPHA, GL_UNSIGNED_BYTE, Texture);
//       glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, TexCW*128, TexCH, 0, GL_ALPHA, GL_UNSIGNED_BYTE, Texture);
//       printf("Tex: %dx%d, %d\n",TexCW,TexCH,_FontCacheHead -> TexID);

  } //eof


  free(Texture);
  free(CharTex);
  free(BP);
  free(BezierCurve);
} //eof NTK_Text::GenerateFont()



int NTK_Text::Draw(int PosX, int PosY, int SizX, int SizY, char Text[], int Align, char Color[3], bool State) {
  int			i,W,H,SW,LocX,LocY,TexCW,TexCH,FontSize;
  FontCacheNode		*fcn;

  if (SizX >= SizY) {
    H= (SizY*5)/8;
    SW= W= (H*2)/3;
  } else {
    H= (SizX*5)/8;
    SW= W= (H*2)/3;
  } //fi

  FontSize= H;
  if (H < 12) FontSize= 12;

  TexCW= 1; while (TexCW < W) TexCW*= 2;
  TexCH= 1; while (TexCH < H) TexCH*= 2;

  // Check to see if Font of Size H is cached, otherwise generate
  // and cache the font for future usage.
  fcn= _FontCacheHead;
  while (fcn) {
    if (fcn -> FontSize == FontSize) break;
    fcn= fcn -> Next;
  } //eow

  if (!fcn) {
    if (H != FontSize) {
      GenerateFont(8,12);
    } else {
      GenerateFont(W,H);
    } //fi

    fcn= _FontCacheHead;
    while (fcn) {
      if (fcn -> FontSize == FontSize) break;
      fcn= fcn -> Next;
    } //eow
  } //fi

  if (((SizX-4)-(int(strlen(Text))*SW)) >= -20) {
    for (i= 0; i < int(strlen(Text)); i++) {
      
      LocX= LocY= 0;
      switch( Align ) {
        case NTK_JUSTIFY_LEFT:
          LocX= 2+PosX+State+(i*SW);
          LocY= PosY+SizY+State-(SizY-H)/2;
          break;

        case NTK_JUSTIFY_CENTER:
          LocX= 2+PosX+State+(i*SW)+((SizX-4)-int(strlen(Text)*SW))/2;
          LocY= PosY+SizY+State-(SizY-H)/2;
          break;

        case NTK_JUSTIFY_RIGHT:
          break;

        default:
          break;
      } //eos


      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D, fcn -> TexIDS[Text[i]]);
//      printf("TexID: %d\n",fcn -> TexID);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glBegin(GL_QUADS);
        glColor4f(State,State,State,0.85f);
        glTexCoord2f(W*1.0f/TexCW,0);
          glVertex2i(LocX+W,LocY);
        glTexCoord2f(W*1.0f/TexCW,H*1.0f/TexCH);
          glVertex2i(LocX+W,LocY-H);
        glTexCoord2f(0,H*1.0f/TexCH);
          glVertex2i(LocX,LocY-H);
        glTexCoord2f(0,0);
          glVertex2i(LocX,LocY);
      glEnd();
      glDisable(GL_BLEND);
      glDisable(GL_TEXTURE_2D);
      } //fi
    } //eof

    return( W*int(strlen(Text)) );
} //eof NTK_Text::Draw()



int NTK_Text::CharWidth(int SizX, int SizY) {
  if (!SizX || SizX >= SizY) {
    return( (((SizY*5)/8)*2)/3 );
  } else {
    return( (((SizX*5)/8)*2)/3 );
  } //fi
} //eof NTK_Text::CharWidth()
