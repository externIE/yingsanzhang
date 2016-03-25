# Microsoft Developer Studio Project File - Name="ZgsaAssitSvr" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=ZgsaAssitSvr - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ZgsaAssitSvr.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ZgsaAssitSvr.mak" CFG="ZgsaAssitSvr - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ZgsaAssitSvr - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "ZgsaAssitSvr - Win32 ReleaseS" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Library/XYGAME1.0/xygame", PPEBAAAA"
# PROP Scc_LocalPath "..\xygame"
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ZgsaAssitSvr - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MD /W3 /Gm /GX /ZI /Od /D "UWL_TRACE" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 uwld.lib uwlntd.lib uwltrace.lib tcnsd.lib xyapid.lib jsoncppd.lib TcyCommond.lib /nologo /subsystem:console /debug /machine:I386 /out:"Debug/ZgsaAssitSvrcd.exe" /pdbtype:sept
# Begin Special Build Tool
OutDir=.\Debug
SOURCE="$(InputPath)"
PostBuild_Desc=de
PostBuild_Cmds=del $(OutDir)\*.sbr
# End Special Build Tool

!ELSEIF  "$(CFG)" == "ZgsaAssitSvr - Win32 ReleaseS"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseS"
# PROP BASE Intermediate_Dir "ReleaseS"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseS"
# PROP Intermediate_Dir "ReleaseS"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "UWL_SERVICE" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 uwl.lib uwlnt.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 uwl.lib uwlnt.lib uwltrace.lib tcns.lib xyapi.lib jsoncpp.lib TcyCommon.lib /nologo /subsystem:console /machine:I386
# SUBTRACT LINK32 /debug
# Begin Special Build Tool
OutDir=.\ReleaseS
SOURCE="$(InputPath)"
PostBuild_Cmds=copy $(OutDir)\*.exe .\..\..\bin
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "ZgsaAssitSvr - Win32 Debug"
# Name "ZgsaAssitSvr - Win32 ReleaseS"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Main.cpp

!IF  "$(CFG)" == "ZgsaAssitSvr - Win32 Debug"

!ELSEIF  "$(CFG)" == "ZgsaAssitSvr - Win32 ReleaseS"

# ADD CPP /YX"stdafx.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MainServer.cpp

!IF  "$(CFG)" == "ZgsaAssitSvr - Win32 Debug"

!ELSEIF  "$(CFG)" == "ZgsaAssitSvr - Win32 ReleaseS"

# ADD CPP /YX"stdafx.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ZgsaAssitSvr.rc
# End Source File
# Begin Source File

SOURCE=.\SockClient.cpp

!IF  "$(CFG)" == "ZgsaAssitSvr - Win32 Debug"

!ELSEIF  "$(CFG)" == "ZgsaAssitSvr - Win32 ReleaseS"

# ADD CPP /YX"stdafx.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SockSvr.cpp

!IF  "$(CFG)" == "ZgsaAssitSvr - Win32 Debug"

!ELSEIF  "$(CFG)" == "ZgsaAssitSvr - Win32 ReleaseS"

# ADD CPP /YX"stdafx.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "ZgsaAssitSvr - Win32 Debug"

# ADD CPP /Yc

!ELSEIF  "$(CFG)" == "ZgsaAssitSvr - Win32 ReleaseS"

# ADD CPP /YX"stdafx.h"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AssitDef.h
# End Source File
# Begin Source File

SOURCE=.\AssitReq.h
# End Source File
# Begin Source File

SOURCE=..\COMMON\zgsa\CommonDef.h
# End Source File
# Begin Source File

SOURCE=..\COMMON\zgsa\CommonReq.h
# End Source File
# Begin Source File

SOURCE=.\Main.h
# End Source File
# Begin Source File

SOURCE=.\MainServer.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SockClient.h
# End Source File
# Begin Source File

SOURCE=.\SockSvr.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
