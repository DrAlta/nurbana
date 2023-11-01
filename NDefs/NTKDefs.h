#define	NTK_OFF				0
#define	NTK_ON				1

// Widget Types
#define	NTK_WT_Button			0
#define	NTK_WT_Label			1
#define	NTK_WT_Menu			2
#define	NTK_WT_ScrollBar		3
#define	NTK_WT_TextInput		4
#define	NTK_WT_Slider_Horz		5
#define	NTK_WT_Slider_Vert		6
#define	NTK_WT_Slider_Button		7
#define	NTK_WT_List			8
#define	NTK_WT_TextArea		9


// Justification
#define NTK_JUSTIFY_LEFT		0
#define NTK_JUSTIFY_CENTER		1
#define NTK_JUSTIFY_RIGHT		2

// Event Types
#define NTK_EVENT_MOUSE			0
#define NTK_EVENT_MOUSE_UP		1
#define NTK_EVENT_MOUSE_DOWN		2
#define	NTK_EVENT_MOTION		3
#define	NTK_EVENT_KEY			4
#define	NTK_EVENT_PASSIVE		5
#define	NTK_EVENT_UPDATE_COMPLETE	6

// Callback Trigger Types
#define NTK_CT_MOUSE_DOWN		1
#define NTK_CT_MOUSE_UP			2
#define NTK_CT_MOTION			4
#define	NTK_CT_KEY			8
#define	NTK_CT_UPDATE_COMPLETE		16


#define	NTK_DYN_LEFT		1
#define	NTK_DYN_RIGHT		2
#define	NTK_DYN_TOP		4
#define	NTK_DYN_BOTTOM		8
#define	NTK_DYN_WIDTH		1
#define	NTK_DYN_HEIGHT		2

#ifndef NTK_Glyphic_S
#define NTK_Glyphic_S

struct NTK_Glyphic {
  unsigned	int	GW;
  unsigned	int	GH;
  unsigned	int	TexW;
  unsigned	int	TexH;
  unsigned	int	TexID;
};

#endif
