# Microsoft Developer Studio Project File - Name="wxq2" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102
# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=wxq2 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "wxq2.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "wxq2.mak" CFG="wxq2 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "wxq2 - Win32 Release DLL" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "wxq2 - Win32 Debug DLL" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "wxq2 - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "wxq2 - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "wxq2 - Win32 Release DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseDll"
# PROP BASE Intermediate_Dir "wx\ReleaseDll"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseDll"
# PROP Intermediate_Dir "wx\ReleaseDll"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /MD /W4 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "WXQ2_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /O2 /I "$(WXWIN)/include" /I "$(WXWIN)\lib\mswdll" /D "_USRDLL" /D WINVER=0x0400 /D "STRICT" /D "__WXWINDOWS__" /D "REF_HARD_LINKED" /D "WXQ2_EXPORTS" /D "NDEBUG" /D "WIN32" /D "IML_Q2_EXTENSIONS" /FD /c
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib advapi32.lib comdlg32.lib shell32.lib ole32.lib oleaut32.lib odbc32.lib uuid.lib rpcrt4.lib comctl32.lib wsock32.lib winmm.lib $(WXWIN)\lib\wxmsw250.lib /nologo /dll /machine:I386

!ELSEIF  "$(CFG)" == "wxq2 - Win32 Debug DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DebugDll"
# PROP BASE Intermediate_Dir "wx\DebugDll"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugDll"
# PROP Intermediate_Dir "wx\DebugDll"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /MDd /W4 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "WXQ2_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /Zi /Od /I "$(WXWIN)/include" /I "$(WXWIN)\lib\mswdlld" /D "_USRDLL" /D "__WXDEBUG__" /D WINVER=0x0400 /D "STRICT" /D "__WXWINDOWS__" /D "REF_HARD_LINKED" /D "WXQ2_EXPORTS" /D "WIN32" /D "_DEBUG" /D "IML_Q2_EXTENSIONS" /FD /c
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /dll /debug /machine:I386 /out:"DebugDll/wxq2d.dll" /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib advapi32.lib comdlg32.lib shell32.lib ole32.lib oleaut32.lib odbc32.lib uuid.lib rpcrt4.lib comctl32.lib wsock32.lib winmm.lib $(WXWIN)\lib\wxmsw250d.lib /nologo /dll /debug /machine:I386 /out:"DebugDll/wxq2d.dll" /pdbtype:sept

!ELSEIF  "$(CFG)" == "wxq2 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "wx\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "wx\Release"
# PROP Target_Dir ""
MTL=midl.exe
LINK32=link.exe
CPP=cl.exe
# ADD BASE CPP /nologo /MD /W4 /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /GR /GX /O2 /I "$(WXWIN)/include" /I "$(WXWIN)\lib\msw" /D "WIN32" /D "NDEBUG" /D WINVER=0x0400 /D "STRICT" /D "__WXWINDOWS__" /D "REF_HARD_LINKED" /D "IML_Q2_EXTENSIONS" /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "wxq2 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "wx\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "wx\Debug"
# PROP Target_Dir ""
MTL=midl.exe
LINK32=link.exe
CPP=cl.exe
# ADD BASE CPP /nologo /MDd /W4 /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /Zi /Od /I "$(WXWIN)/include" /I "$(WXWIN)\lib\mswd" /D "WIN32" /D "_DEBUG" /D "__WXDEBUG__" /D WINVER=0x0400 /D "STRICT" /D "__WXWINDOWS__" /D "REF_HARD_LINKED" /D "IML_Q2_EXTENSIONS" /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Debug\wxq2d.lib"

!ENDIF 

# Begin Target

# Name "wxq2 - Win32 Release DLL"
# Name "wxq2 - Win32 Debug DLL"
# Name "wxq2 - Win32 Release"
# Name "wxq2 - Win32 Debug"
# Begin Group "quake"

# PROP Default_Filter ""
# Begin Group "common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\qcommon\cmd.c
# End Source File
# Begin Source File

SOURCE=.\qcommon\cmodel.c
# End Source File
# Begin Source File

SOURCE=.\qcommon\common.c
# End Source File
# Begin Source File

SOURCE=.\qcommon\crc.c
# End Source File
# Begin Source File

SOURCE=.\qcommon\cvar.c
# End Source File
# Begin Source File

SOURCE=.\qcommon\files.c
# End Source File
# Begin Source File

SOURCE=.\game\m_flash.c
# End Source File
# Begin Source File

SOURCE=.\qcommon\md4.c
# End Source File
# Begin Source File

SOURCE=.\qcommon\net_chan.c
# End Source File
# Begin Source File

SOURCE=.\qcommon\pmove.c
# End Source File
# Begin Source File

SOURCE=.\game\q_shared.c
# End Source File
# End Group
# Begin Group "client"

# PROP Default_Filter "*.c"
# Begin Source File

SOURCE=.\client\cl_cin.c
# End Source File
# Begin Source File

SOURCE=.\client\cl_ents.c
# End Source File
# Begin Source File

SOURCE=.\client\cl_fx.c
# End Source File
# Begin Source File

SOURCE=.\client\cl_input.c
# End Source File
# Begin Source File

SOURCE=.\client\cl_inv.c
# End Source File
# Begin Source File

SOURCE=.\client\cl_main.c
# End Source File
# Begin Source File

SOURCE=.\client\cl_newfx.c
# End Source File
# Begin Source File

SOURCE=.\client\cl_parse.c
# End Source File
# Begin Source File

SOURCE=.\client\cl_pred.c
# End Source File
# Begin Source File

SOURCE=.\client\cl_scrn.c
# End Source File
# Begin Source File

SOURCE=.\client\cl_tent.c
# End Source File
# Begin Source File

SOURCE=.\client\cl_view.c
# End Source File
# Begin Source File

SOURCE=.\client\console.c
# End Source File
# Begin Source File

SOURCE=.\client\keys.c
# End Source File
# Begin Source File

SOURCE=.\client\menu.c
# End Source File
# Begin Source File

SOURCE=.\client\qmenu.c
# End Source File
# Begin Source File

SOURCE=.\client\snd_dma.c
# End Source File
# Begin Source File

SOURCE=.\client\snd_mem.c
# End Source File
# Begin Source File

SOURCE=.\client\snd_mix.c
# End Source File
# Begin Source File

SOURCE=.\client\x86.c
# End Source File
# End Group
# Begin Group "server"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\server\sv_ccmds.c
# End Source File
# Begin Source File

SOURCE=.\server\sv_ents.c
# End Source File
# Begin Source File

SOURCE=.\server\sv_game.c
# End Source File
# Begin Source File

SOURCE=.\server\sv_init.c
# End Source File
# Begin Source File

SOURCE=.\server\sv_main.c
# End Source File
# Begin Source File

SOURCE=.\server\sv_send.c
# End Source File
# Begin Source File

SOURCE=.\server\sv_user.c
# End Source File
# Begin Source File

SOURCE=.\server\sv_world.c
# End Source File
# End Group
# Begin Group "ref_soft"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ref_soft\r_aclip.c
# End Source File
# Begin Source File

SOURCE=.\ref_soft\r_aclipa.asm

!IF  "$(CFG)" == "wxq2 - Win32 Release DLL"

# Begin Custom Build
OutDir=.\ReleaseDll
InputPath=.\ref_soft\r_aclipa.asm
InputName=r_aclipa

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"D:\Asm\Masm 6.15\ml.exe" /c /Cp /coff /Fo$(OutDir)\$(InputName).obj /Zm /Zi $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "wxq2 - Win32 Debug DLL"

# Begin Custom Build
OutDir=.\DebugDll
InputPath=.\ref_soft\r_aclipa.asm
InputName=r_aclipa

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"D:\Asm\Masm 6.15\ml.exe" /nologo /c /Cp /coff /Fo$(OutDir)\$(InputName).obj /Zm /Zi $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "wxq2 - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=.\ref_soft\r_aclipa.asm
InputName=r_aclipa

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	ml.exe /nologo /c /Cp /coff /Fo$(OutDir)\$(InputName).obj /Zm /Zi $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "wxq2 - Win32 Debug"

# Begin Custom Build
OutDir=.\Debug
InputPath=.\ref_soft\r_aclipa.asm
InputName=r_aclipa

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	ml.exe /nologo /c /Cp /coff /Fo$(OutDir)\$(InputName).obj /Zm /Zi $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ref_soft\r_alias.c
# End Source File
# Begin Source File

SOURCE=.\ref_soft\r_bsp.c
# End Source File
# Begin Source File

SOURCE=.\ref_soft\r_draw.c
# End Source File
# Begin Source File

SOURCE=.\ref_soft\r_draw16.asm

!IF  "$(CFG)" == "wxq2 - Win32 Release DLL"

# Begin Custom Build
OutDir=.\ReleaseDll
InputPath=.\ref_soft\r_draw16.asm
InputName=r_draw16

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"D:\Asm\Masm 6.15\ml.exe" /nologo /c /Cp /coff /Fo$(OutDir)\$(InputName).obj /Zm /Zi $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "wxq2 - Win32 Debug DLL"

# Begin Custom Build
OutDir=.\DebugDll
InputPath=.\ref_soft\r_draw16.asm
InputName=r_draw16

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"D:\Asm\Masm 6.15\ml.exe" /nologo /c /Cp /coff /Fo$(OutDir)\$(InputName).obj /Zm /Zi $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "wxq2 - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=.\ref_soft\r_draw16.asm
InputName=r_draw16

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	ml.exe /nologo /c /Cp /coff /Fo$(OutDir)\$(InputName).obj /Zm /Zi $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "wxq2 - Win32 Debug"

# Begin Custom Build
OutDir=.\Debug
InputPath=.\ref_soft\r_draw16.asm
InputName=r_draw16

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	ml.exe /nologo /c /Cp /coff /Fo$(OutDir)\$(InputName).obj /Zm /Zi $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ref_soft\r_drawa.asm

!IF  "$(CFG)" == "wxq2 - Win32 Release DLL"

# Begin Custom Build
OutDir=.\ReleaseDll
InputPath=.\ref_soft\r_drawa.asm
InputName=r_drawa

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"D:\Asm\Masm 6.15\ml.exe" /nologo /c /Cp /coff /Fo$(OutDir)\$(InputName).obj /Zm /Zi $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "wxq2 - Win32 Debug DLL"

# Begin Custom Build
OutDir=.\DebugDll
InputPath=.\ref_soft\r_drawa.asm
InputName=r_drawa

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"D:\Asm\Masm 6.15\ml.exe" /nologo /c /Cp /coff /Fo$(OutDir)\$(InputName).obj /Zm /Zi $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "wxq2 - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=.\ref_soft\r_drawa.asm
InputName=r_drawa

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	ml.exe /nologo /c /Cp /coff /Fo$(OutDir)\$(InputName).obj /Zm /Zi $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "wxq2 - Win32 Debug"

# Begin Custom Build
OutDir=.\Debug
InputPath=.\ref_soft\r_drawa.asm
InputName=r_drawa

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	ml.exe /nologo /c /Cp /coff /Fo$(OutDir)\$(InputName).obj /Zm /Zi $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ref_soft\r_edge.c
# End Source File
# Begin Source File

SOURCE=.\ref_soft\r_edgea.asm

!IF  "$(CFG)" == "wxq2 - Win32 Release DLL"

# Begin Custom Build
OutDir=.\ReleaseDll
InputPath=.\ref_soft\r_edgea.asm
InputName=r_edgea

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"D:\Asm\Masm 6.15\ml.exe" /nologo /c /Cp /coff /Fo$(OutDir)\$(InputName).obj /Zm /Zi $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "wxq2 - Win32 Debug DLL"

# Begin Custom Build
OutDir=.\DebugDll
InputPath=.\ref_soft\r_edgea.asm
InputName=r_edgea

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"D:\Asm\Masm 6.15\ml.exe" /nologo /c /Cp /coff /Fo$(OutDir)\$(InputName).obj /Zm /Zi $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "wxq2 - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=.\ref_soft\r_edgea.asm
InputName=r_edgea

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	ml.exe /nologo /c /Cp /coff /Fo$(OutDir)\$(InputName).obj /Zm /Zi $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "wxq2 - Win32 Debug"

# Begin Custom Build
OutDir=.\Debug
InputPath=.\ref_soft\r_edgea.asm
InputName=r_edgea

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	ml.exe /nologo /c /Cp /coff /Fo$(OutDir)\$(InputName).obj /Zm /Zi $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ref_soft\r_image.c
# End Source File
# Begin Source File

SOURCE=.\ref_soft\r_light.c
# End Source File
# Begin Source File

SOURCE=.\ref_soft\r_main.c
# End Source File
# Begin Source File

SOURCE=.\ref_soft\r_misc.c
# End Source File
# Begin Source File

SOURCE=.\ref_soft\r_model.c
# End Source File
# Begin Source File

SOURCE=.\ref_soft\r_part.c
# End Source File
# Begin Source File

SOURCE=.\ref_soft\r_poly.c
# End Source File
# Begin Source File

SOURCE=.\ref_soft\r_polysa.asm

!IF  "$(CFG)" == "wxq2 - Win32 Release DLL"

# Begin Custom Build
OutDir=.\ReleaseDll
InputPath=.\ref_soft\r_polysa.asm
InputName=r_polysa

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"D:\Asm\Masm 6.15\ml.exe" /nologo /c /Cp /coff /Fo$(OutDir)\$(InputName).obj /Zm /Zi $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "wxq2 - Win32 Debug DLL"

# Begin Custom Build
OutDir=.\DebugDll
InputPath=.\ref_soft\r_polysa.asm
InputName=r_polysa

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"D:\Asm\Masm 6.15\ml.exe" /nologo /c /Cp /coff /Fo$(OutDir)\$(InputName).obj /Zm /Zi $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "wxq2 - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=.\ref_soft\r_polysa.asm
InputName=r_polysa

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	ml.exe /nologo /c /Cp /coff /Fo$(OutDir)\$(InputName).obj /Zm /Zi $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "wxq2 - Win32 Debug"

# Begin Custom Build
OutDir=.\Debug
InputPath=.\ref_soft\r_polysa.asm
InputName=r_polysa

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	ml.exe /nologo /c /Cp /coff /Fo$(OutDir)\$(InputName).obj /Zm /Zi $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ref_soft\r_polyse.c
# End Source File
# Begin Source File

SOURCE=.\ref_soft\r_rast.c
# End Source File
# Begin Source File

SOURCE=.\ref_soft\r_scan.c
# End Source File
# Begin Source File

SOURCE=.\ref_soft\r_scana.asm

!IF  "$(CFG)" == "wxq2 - Win32 Release DLL"

# Begin Custom Build
OutDir=.\ReleaseDll
InputPath=.\ref_soft\r_scana.asm
InputName=r_scana

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"D:\Asm\Masm 6.15\ml.exe" /nologo /c /Cp /coff /Fo$(OutDir)\$(InputName).obj /Zm /Zi $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "wxq2 - Win32 Debug DLL"

# Begin Custom Build
OutDir=.\DebugDll
InputPath=.\ref_soft\r_scana.asm
InputName=r_scana

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"D:\Asm\Masm 6.15\ml.exe" /nologo /c /Cp /coff /Fo$(OutDir)\$(InputName).obj /Zm /Zi $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "wxq2 - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=.\ref_soft\r_scana.asm
InputName=r_scana

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	ml.exe /nologo /c /Cp /coff /Fo$(OutDir)\$(InputName).obj /Zm /Zi $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "wxq2 - Win32 Debug"

# Begin Custom Build
OutDir=.\Debug
InputPath=.\ref_soft\r_scana.asm
InputName=r_scana

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	ml.exe /nologo /c /Cp /coff /Fo$(OutDir)\$(InputName).obj /Zm /Zi $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ref_soft\r_spr8.asm

!IF  "$(CFG)" == "wxq2 - Win32 Release DLL"

# Begin Custom Build
OutDir=.\ReleaseDll
InputPath=.\ref_soft\r_spr8.asm
InputName=r_spr8

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"D:\Asm\Masm 6.15\ml.exe" /nologo /c /Cp /coff /Fo$(OutDir)\$(InputName).obj /Zm /Zi $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "wxq2 - Win32 Debug DLL"

# Begin Custom Build
OutDir=.\DebugDll
InputPath=.\ref_soft\r_spr8.asm
InputName=r_spr8

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"D:\Asm\Masm 6.15\ml.exe" /nologo /c /Cp /coff /Fo$(OutDir)\$(InputName).obj /Zm /Zi $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "wxq2 - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=.\ref_soft\r_spr8.asm
InputName=r_spr8

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	ml.exe /nologo /c /Cp /coff /Fo$(OutDir)\$(InputName).obj /Zm /Zi $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "wxq2 - Win32 Debug"

# Begin Custom Build
OutDir=.\Debug
InputPath=.\ref_soft\r_spr8.asm
InputName=r_spr8

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	ml.exe /nologo /c /Cp /coff /Fo$(OutDir)\$(InputName).obj /Zm /Zi $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ref_soft\r_sprite.c
# End Source File
# Begin Source File

SOURCE=.\ref_soft\r_surf.c
# End Source File
# Begin Source File

SOURCE=.\ref_soft\r_surf8.asm

!IF  "$(CFG)" == "wxq2 - Win32 Release DLL"

# Begin Custom Build
OutDir=.\ReleaseDll
InputPath=.\ref_soft\r_surf8.asm
InputName=r_surf8

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"D:\Asm\Masm 6.15\ml.exe" /nologo /c /Cp /coff /Fo$(OutDir)\$(InputName).obj /Zm /Zi $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "wxq2 - Win32 Debug DLL"

# Begin Custom Build
OutDir=.\DebugDll
InputPath=.\ref_soft\r_surf8.asm
InputName=r_surf8

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"D:\Asm\Masm 6.15\ml.exe" /nologo /c /Cp /coff /Fo$(OutDir)\$(InputName).obj /Zm /Zi $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "wxq2 - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=.\ref_soft\r_surf8.asm
InputName=r_surf8

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	ml.exe /nologo /c /Cp /coff /Fo$(OutDir)\$(InputName).obj /Zm /Zi $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "wxq2 - Win32 Debug"

# Begin Custom Build
OutDir=.\Debug
InputPath=.\ref_soft\r_surf8.asm
InputName=r_surf8

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	ml.exe /nologo /c /Cp /coff /Fo$(OutDir)\$(InputName).obj /Zm /Zi $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ref_soft\r_varsa.asm

!IF  "$(CFG)" == "wxq2 - Win32 Release DLL"

# Begin Custom Build
OutDir=.\ReleaseDll
InputPath=.\ref_soft\r_varsa.asm
InputName=r_varsa

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"D:\Asm\Masm 6.15\ml.exe" /nologo /c /Cp /coff /Fo$(OutDir)\$(InputName).obj /Zm /Zi $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "wxq2 - Win32 Debug DLL"

# Begin Custom Build
OutDir=.\DebugDll
InputPath=.\ref_soft\r_varsa.asm
InputName=r_varsa

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"D:\Asm\Masm 6.15\ml.exe" /nologo /c /Cp /coff /Fo$(OutDir)\$(InputName).obj /Zm /Zi $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "wxq2 - Win32 Release"

# Begin Custom Build
OutDir=.\Release
InputPath=.\ref_soft\r_varsa.asm
InputName=r_varsa

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	ml.exe /nologo /c /Cp /coff /Fo$(OutDir)\$(InputName).obj /Zm /Zi $(InputPath)

# End Custom Build

!ELSEIF  "$(CFG)" == "wxq2 - Win32 Debug"

# Begin Custom Build
OutDir=.\Debug
InputPath=.\ref_soft\r_varsa.asm
InputName=r_varsa

"$(OutDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	ml.exe /nologo /c /Cp /coff /Fo$(OutDir)\$(InputName).obj /Zm /Zi $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "win32"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\win32\cd_win.c
# End Source File
# Begin Source File

SOURCE=.\win32\conproc.c
# End Source File
# Begin Source File

SOURCE=.\win32\in_win.c
# End Source File
# Begin Source File

SOURCE=.\win32\net_wins.c
# End Source File
# Begin Source File

SOURCE=.\win32\q_shwin.c
# End Source File
# Begin Source File

SOURCE=.\win32\rw_ddraw.c
# End Source File
# Begin Source File

SOURCE=.\win32\rw_dib.c
# End Source File
# Begin Source File

SOURCE=.\win32\rw_imp.c
# End Source File
# Begin Source File

SOURCE=.\win32\snd_win.c
# End Source File
# Begin Source File

SOURCE=.\win32\sys_win.c
# End Source File
# Begin Source File

SOURCE=.\win32\vid_dll.c
# End Source File
# Begin Source File

SOURCE=.\win32\vid_menu.c
# End Source File
# End Group
# End Group
# Begin Source File

SOURCE=.\wx\wxquake2.cpp

!IF  "$(CFG)" == "wxq2 - Win32 Release DLL"

# ADD CPP /W4 /D "WXUSINGDLL"

!ELSEIF  "$(CFG)" == "wxq2 - Win32 Debug DLL"

# ADD CPP /W4 /D "WXUSINGDLL"

!ELSEIF  "$(CFG)" == "wxq2 - Win32 Release"

# ADD CPP /W4

!ELSEIF  "$(CFG)" == "wxq2 - Win32 Debug"

# ADD CPP /W4

!ENDIF 

# End Source File
# End Target
# End Project
