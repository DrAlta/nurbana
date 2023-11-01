# Microsoft Developer Studio Project File - Name="Nurbana" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Nurbana - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Nurbana.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Nurbana.mak" CFG="Nurbana - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Nurbana - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Nurbana - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Nurbana - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "Nurbana - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MT /W3 /GX /Zd /O2 /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib msvcrt.lib user32.lib advapi32.lib opennurbs_static_vc60d.lib zlib_vc60d.lib opengl32.lib SDLmain.lib SDL.lib /nologo /subsystem:console /machine:I386 /nodefaultlib:"LIBCMT" /nodefaultlib:"LIBCD" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none /incremental:no /debug

!ENDIF 

# Begin Target

# Name "Nurbana - Win32 Release"
# Name "Nurbana - Win32 Debug"
# Begin Group "NTK"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Ntk\NTK_Base.cpp
# End Source File
# Begin Source File

SOURCE=.\Ntk\NTK_Button.cpp
# End Source File
# Begin Source File

SOURCE=.\Ntk\NTK_Container.cpp
# End Source File
# Begin Source File

SOURCE=.\Ntk\NTK_Group.cpp
# End Source File
# Begin Source File

SOURCE=.\Ntk\NTK_Label.cpp
# End Source File
# Begin Source File

SOURCE=.\Ntk\NTK_List.cpp
# End Source File
# Begin Source File

SOURCE=.\Ntk\NTK_Menu.cpp
# End Source File
# Begin Source File

SOURCE=.\Ntk\NTK_ScrollBar.cpp
# End Source File
# Begin Source File

SOURCE=.\ntk\NTK_Slider_Button.cpp
# End Source File
# Begin Source File

SOURCE=.\Ntk\NTK_Slider_Horz.cpp
# End Source File
# Begin Source File

SOURCE=.\Ntk\NTK_Slider_Vert.cpp
# End Source File
# Begin Source File

SOURCE=.\NTK\NTK_Text.cpp
# End Source File
# Begin Source File

SOURCE=.\NTK\NTK_TextArea.cpp
# End Source File
# Begin Source File

SOURCE=.\Ntk\NTK_TextInput.cpp
# End Source File
# Begin Source File

SOURCE=.\Ntk\NTK_Valuator.cpp
# End Source File
# Begin Source File

SOURCE=.\Ntk\NTK_Window.cpp
# End Source File
# Begin Source File

SOURCE=.\NTK\ntku_Bresenham.cpp
# End Source File
# Begin Source File

SOURCE=.\Ntk\ntku_Button.cpp
# End Source File
# Begin Source File

SOURCE=.\Ntk\ntku_Glyphic.cpp
# End Source File
# Begin Source File

SOURCE=.\Ntk\ntku_Math.cpp
# End Source File
# Begin Source File

SOURCE=.\Ntk\ntku_XPM.cpp
# End Source File
# End Group
# Begin Group "Objects"

# PROP Default_Filter ""
# Begin Group "NURBS"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Objects\NURBS\NURBS.cpp
# End Source File
# Begin Source File

SOURCE=.\Objects\NURBS\NURBS_Construct.cpp
# End Source File
# Begin Source File

SOURCE=.\Objects\NURBS\NURBS_Degree.cpp
# End Source File
# Begin Source File

SOURCE=.\Objects\nurbs\NURBS_Fitting.cpp
# End Source File
# Begin Source File

SOURCE=.\Objects\NURBS\NURBS_Generate.cpp
# End Source File
# Begin Source File

SOURCE=.\Objects\NURBS\NURBS_Knot.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\Objects\Bezier.cpp
# End Source File
# Begin Source File

SOURCE=.\Objects\DrawFaces.cpp
# End Source File
# Begin Source File

SOURCE=.\Objects\NURBS_TrimCurve.cpp
# End Source File
# Begin Source File

SOURCE=.\Objects\Object_Base.cpp
# End Source File
# Begin Source File

SOURCE=.\Objects\Object_Draw.cpp
# End Source File
# Begin Source File

SOURCE=.\Objects\Object_List.cpp
# End Source File
# Begin Source File

SOURCE=.\Objects\Object_Manager.cpp
# End Source File
# Begin Source File

SOURCE=.\Objects\Object_Normals.cpp
# End Source File
# Begin Source File

SOURCE=.\Objects\Object_NURBS.cpp
# End Source File
# Begin Source File

SOURCE=.\Objects\Object_Primitives.cpp
# End Source File
# Begin Source File

SOURCE=.\Objects\Object_WC.cpp
# End Source File
# End Group
# Begin Group "NWM"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\NWM\NWM.cpp
# End Source File
# Begin Source File

SOURCE=.\NWM\NWM_Base.cpp
# End Source File
# Begin Source File

SOURCE=.\NWM\NWM_FileManager.cpp
# End Source File
# Begin Source File

SOURCE=.\NWM\NWM_Handle.cpp
# End Source File
# Begin Source File

SOURCE=.\NWM\NWM_KnotMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\NWM\NWM_List.cpp
# End Source File
# Begin Source File

SOURCE=.\NWM\NWM_Menu.cpp
# End Source File
# Begin Source File

SOURCE=.\NWM\NWM_ModelView.cpp
# End Source File
# Begin Source File

SOURCE=.\nwm\NWM_Panel_Construction.cpp
# End Source File
# Begin Source File

SOURCE=.\nwm\NWM_Panel_Edit.cpp
# End Source File
# Begin Source File

SOURCE=.\nwm\NWM_Panel_Materials.cpp
# End Source File
# Begin Source File

SOURCE=.\nwm\NWM_Panel_Primitives.cpp
# End Source File
# Begin Source File

SOURCE=.\nwm\NWM_Panel_Render.cpp
# End Source File
# Begin Source File

SOURCE=.\nwm\NWM_Panel_Settings.cpp
# End Source File
# Begin Source File

SOURCE=.\NWM\NWM_SceneNotes.cpp
# End Source File
# Begin Source File

SOURCE=.\nwm\NWM_Selector.cpp
# End Source File
# Begin Source File

SOURCE=.\nwm\NWM_StatusBar.cpp
# End Source File
# Begin Source File

SOURCE=.\NWM\nwmu_Handle.cpp
# End Source File
# End Group
# Begin Group "NRT"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\NRT\NRT_Light_Base.cpp
# End Source File
# Begin Source File

SOURCE=.\NRT\NRT_Light_List.cpp
# End Source File
# Begin Source File

SOURCE=.\NRT\NRT_Light_Manager.cpp
# End Source File
# Begin Source File

SOURCE=.\NRT\NRT_Light_Sphere.cpp
# End Source File
# Begin Source File

SOURCE=.\NRT\NRT_Object_Base.cpp
# End Source File
# Begin Source File

SOURCE=.\NRT\NRT_Object_List.cpp
# End Source File
# Begin Source File

SOURCE=.\NRT\NRT_Object_Manager.cpp
# End Source File
# Begin Source File

SOURCE=.\NRT\NRT_Object_NURBS.cpp
# End Source File
# Begin Source File

SOURCE=.\NRT\NRT_Object_Sphere.cpp
# End Source File
# Begin Source File

SOURCE=.\NRT\NRT_Object_Triangle.cpp
# End Source File
# Begin Source File

SOURCE=.\NRT\NRT_RayTracer.cpp
# End Source File
# Begin Source File

SOURCE=.\NRT\NRT_Render.cpp
# End Source File
# End Group
# Begin Group "FileIO"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FileIO\DXF_IO.cpp
# End Source File
# Begin Source File

SOURCE=.\FileIO\FileIO.cpp
# End Source File
# Begin Source File

SOURCE=.\FileIO\NURBS_IO.cpp
# End Source File
# Begin Source File

SOURCE=.\FileIO\ON_IO.cpp
# End Source File
# Begin Source File

SOURCE=.\FileIO\WinFileSystem.cpp
# End Source File
# End Group
# Begin Group "Cursors"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Cursors\Cursors.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\BorderSelect.cpp
# End Source File
# Begin Source File

SOURCE=.\Endian.cpp
# End Source File
# Begin Source File

SOURCE=.\InvertBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\KeyTable.cpp
# End Source File
# Begin Source File

SOURCE=.\MaterialList.cpp
# End Source File
# Begin Source File

SOURCE=.\ModelView.cpp
# End Source File
# Begin Source File

SOURCE=.\NGUtils.cpp
# End Source File
# Begin Source File

SOURCE=.\NIdle.cpp
# End Source File
# Begin Source File

SOURCE=.\NTimer.cpp
# End Source File
# Begin Source File

SOURCE=.\Nurbana.cpp
# End Source File
# Begin Source File

SOURCE=.\NurbanaMath.cpp
# End Source File
# Begin Source File

SOURCE=.\ProjectUtility.cpp
# End Source File
# Begin Source File

SOURCE=.\Trackball.cpp
# End Source File
# Begin Source File

SOURCE=.\VertexOps.cpp
# End Source File
# End Target
# End Project
