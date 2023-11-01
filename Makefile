###############################################################

# If your make doesn't use .cpp for C++ extensions, 
# then uncomment the suffixes line.
.SUFFIXES : .c .cpp

RM = /bin/rm

AR = ar qvl

# If your system doesn't use ranlib, uncomment the "echo" define.
RANLIB = /usr/local/bin/ranlib
# RANLIB = echo

###############################################################
# Gnu tools

# C compiler and flags
CC = gcc
CFLAGS = -O3 -Wall -pipe

# C++ compiler and flags
CCC = g++
# older versions of g++ may require the "-fhandle-exceptions" option
CCFLAGS = -O3 -Wall -I /usr/X11R6/include -I/usr/local/include/SDL11 -I/usr/local/include -D_REENTRANT

###############################################################
# SGI IRIX tools

# C compiler and flags
#CC = gcc
#CFLAGS = -O -Wall -Wmissing-prototypes

# C++ compiler and flags
#CCC = g++
# older versions of g++ may require the "-fhandle-exceptions" option
#CCFLAGS = -O -fhandle-exceptions -Wall -Wmissing-prototypes -I.
#CCFLAGS = -O -Wall -Wmissing-prototypes -I.

###############################################################

.cpp.o :
	$(CCC) $(CCFLAGS) -c $*.cpp -o $*.o

.c.o :
	$(CC) $(CFLAGS) -c $*.c -o $*.o

###############################################################

#	./FileIO/ON_IO.o \
#	./NWM/NWM_ControlPanel.o \
#	./NWM/NWM_MainMenu.o \
#	./NWM/NWM_MainPanel.o \

NURBANA_OBJ = BorderSelect.o \
        ./InvertBuffer.o \
	./Endian.o \
	./KeyTable.o \
	./MaterialList.o \
	./ModelView.o \
	./NGUtils.o \
	./Nurbana.o \
	./NurbanaMath.o \
	./NTimer.o \
	./ProjectUtility.o \
	./Trackball.o \
        ./VertexOps.o \
        ./NIdle.o \
	./Cursors/Cursors.o \
	./FileIO/DXF_IO.o \
	./FileIO/FileIO.o \
	./FileIO/NURBS_IO.o \
	./FileIO/UnixFileSystem.o \
	./Objects/Bezier.o \
	./Objects/DrawFaces.o \
	./Objects/Object_Base.o \
	./Objects/Object_Draw.o \
	./Objects/Object_List.o \
	./Objects/Object_Manager.o \
	./Objects/Object_Normals.o \
	./Objects/Object_NURBS.o \
	./Objects/Object_Primitives.o \
	./Objects/Object_WC.o \
	./Objects/NURBS_TrimCurve.o \
	./Objects/NURBS/NURBS.o \
	./Objects/NURBS/NURBS_Construct.o \
	./Objects/NURBS/NURBS_Degree.o \
	./Objects/NURBS/NURBS_Fitting.o \
	./Objects/NURBS/NURBS_Generate.o \
	./Objects/NURBS/NURBS_Knot.o \
	./NRT/NRT_Light_Base.o \
	./NRT/NRT_Light_List.o \
	./NRT/NRT_Light_Manager.o \
	./NRT/NRT_Light_Sphere.o \
	./NRT/NRT_Object_Base.o \
	./NRT/NRT_Object_List.o \
	./NRT/NRT_Object_NURBS.o \
	./NRT/NRT_Object_Manager.o \
	./NRT/NRT_Object_Sphere.o \
	./NRT/NRT_Object_Triangle.o \
	./NRT/NRT_RayTracer.o \
	./NRT/NRT_Render.o \
	./NTK/ntku_Button.o \
	./NTK/ntku_Bresenham.o \
	./NTK/ntku_Glyphic.o \
	./NTK/ntku_Math.o \
	./NTK/ntku_XPM.o \
	./NTK/NTK_Base.o \
	./NTK/NTK_Button.o \
	./NTK/NTK_Container.o \
	./NTK/NTK_Group.o \
	./NTK/NTK_Label.o \
	./NTK/NTK_List.o \
	./NTK/NTK_Menu.o \
	./NTK/NTK_ScrollBar.o \
	./NTK/NTK_Slider_Button.o \
	./NTK/NTK_Slider_Horz.o \
	./NTK/NTK_Slider_Vert.o \
	./NTK/NTK_Text.o \
	./NTK/NTK_TextArea.o \
	./NTK/NTK_TextInput.o \
	./NTK/NTK_Valuator.o \
	./NTK/NTK_Window.o \
	./NWM/NWM.o \
	./NWM/NWM_Base.o \
	./NWM/NWM_FileManager.o \
	./NWM/NWM_Handle.o \
	./NWM/NWM_KnotMenu.o \
	./NWM/NWM_List.o \
	./NWM/NWM_Menu.o \
	./NWM/NWM_ModelView.o \
	./NWM/NWM_Panel_Construction.o \
	./NWM/NWM_Panel_Edit.o \
	./NWM/NWM_Panel_Materials.o \
	./NWM/NWM_Panel_Primitives.o \
	./NWM/NWM_Panel_Render.o \
	./NWM/NWM_Panel_Settings.o \
	./NWM/NWM_SceneNotes.o \
	./NWM/NWM_Selector.o \
	./NWM/NWM_StatusBar.o \
	./NWM/nwmu_Handle.o

#	$(CCC) $(NURBANA_OBJ) -L/usr/local/lib -I/usr/local/include -lGL -lX11 -lm -lXmu -o $@ #IRIX
#	$(CCC) $(NURBANA_OBJ) -L/usr/local/lib -I/usr/local/include ;5B-lGL -lX11 -lm -lXmu -lXext -o $@ #SunOS
#	$(CCC) $(NURBANA_OBJ) -L/usr/local/lib -I/usr/local/include -lGL -lX11 -lm -lXmu -o $@ #BEOS
#	$(CCC) $(NURBANA_OBJ) -L/usr/local/lib -I/usr/local/include -lGL -lm -o $@ #Linux
#	$(CCC) $(NURBANA_OBJ) -L/usr/local/lib -I/usr/local/include -lGL -lX11 -lm -lXmu -o $@ #X11

NURBANA = .nurbanabin

all : $(NURBANA)
$(NURBANA) : $(NURBANA_OBJ)
	$(CCC) $(NURBANA_OBJ) -L/usr/X11R6/lib/ -L/usr/local/lib -Wl,-rpath,/usr/local/lib -lSDL-1.1 -pthread -lGL -lm -o $@ #IRIX

clean :
	-$(RM) $(NURBANA)
	-$(RM) $(NURBANA_OBJ)
