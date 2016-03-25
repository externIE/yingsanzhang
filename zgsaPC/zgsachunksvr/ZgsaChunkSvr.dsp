# Microsoft Developer Studio Project File - Name="ZgsaChunkSvr" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=ZgsaChunkSvr - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ZgsaChunkSvr.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ZgsaChunkSvr.mak" CFG="ZgsaChunkSvr - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ZgsaChunkSvr - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "ZgsaChunkSvr - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "ZgsaChunkSvr - Win32 DebugS" (based on "Win32 (x86) Console Application")
!MESSAGE "ZgsaChunkSvr - Win32 ReleaseS" (based on "Win32 (x86) Console Application")
!MESSAGE "ZgsaChunkSvr - Win32 ReleaseS125" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Project/GameChannel/1.00/SRC/ZgsaCHUNKSVR", ZQSAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ZgsaChunkSvr - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /win32
# ADD MTL /nologo /win32
# ADD BASE CPP /nologo /MDd /W3 /Gm /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "UWL_TRACE" /D "_AFXDLL" /D "_MBCS" /Yc"stdafx.h" /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "_RS125" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "UWL_TRACE" /D "UWL_TRACE_SQL" /D "_AFXDLL" /D "_MBCS" /Fr /Yc"stdafx.h" /GZ /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 uwld.lib uwlntd.lib uwltrace.lib /nologo /subsystem:console /pdb:"$(OutDir)/ZgsaChunkSvr.pdb" /debug /machine:IX86 /out:"$(OutDir)/ZgsaChunkSvrDC.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 tcnsd.lib uwld.lib uwlntd.lib uwltrace.lib xyapid.lib TcyCommond.lib /nologo /subsystem:console /pdb:"$(OutDir)/ZgsaChunkSvr.pdb" /debug /machine:IX86 /out:".\Debug/ZgsaChunkSvrDC.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "ZgsaChunkSvr - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /win32
# ADD MTL /nologo /win32
# ADD BASE CPP /nologo /MD /W3 /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_AFXDLL" /D "_MBCS" /Yc"stdafx.h" /GF /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_AFXDLL" /D "_MBCS" /Yc"stdafx.h" /GF /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 uwl.lib uwlnt.lib /nologo /subsystem:console /debug /machine:IX86 /out:"$(OutDir)/ZgsaChunkSvrC.exe" /pdbtype:sept /opt:ref /opt:icf
# ADD LINK32 uwl.lib uwlnt.lib /nologo /subsystem:console /debug /machine:IX86 /out:".\Release/ZgsaChunkSvrC.exe" /pdbtype:sept /opt:ref /opt:icf
# Begin Special Build Tool
OutDir=.\Release
SOURCE="$(InputPath)"
PostBuild_Cmds=copy $(OutDir)\*.exe .\..\..\bin
# End Special Build Tool

!ELSEIF  "$(CFG)" == "ZgsaChunkSvr - Win32 DebugS"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DebugS"
# PROP BASE Intermediate_Dir "DebugS"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugS"
# PROP Intermediate_Dir "DebugS"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /win32
# ADD MTL /nologo /win32
# ADD BASE CPP /nologo /MDd /W3 /Gm /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "UWL_TRACE" /D "UWL_SERVICE" /D "_AFXDLL" /D "_MBCS" /Yc"stdafx.h" /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "UWL_TRACE" /D "UWL_SERVICE" /D "_AFXDLL" /D "_MBCS" /Yc"stdafx.h" /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 uwld.lib uwlntd.lib uwltrace.lib /nologo /subsystem:console /pdb:"$(OutDir)/ZgsaChunkSvr.pdb" /debug /machine:IX86 /out:"$(OutDir)/ZgsaChunkSvrD.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 uwld.lib uwlntd.lib uwltrace.lib /nologo /subsystem:console /pdb:"$(OutDir)/ZgsaChunkSvr.pdb" /debug /machine:IX86 /out:".\DebugS/ZgsaChunkSvrD.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
OutDir=.\DebugS
SOURCE="$(InputPath)"
PostBuild_Cmds=copy $(OutDir)\*.exe .\..\..\bin
# End Special Build Tool

!ELSEIF  "$(CFG)" == "ZgsaChunkSvr - Win32 ReleaseS"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseS"
# PROP BASE Intermediate_Dir "ReleaseS"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseS"
# PROP Intermediate_Dir "ReleaseS"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /win32
# ADD MTL /nologo /win32
# ADD BASE CPP /nologo /MD /W3 /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "UWL_SERVICE" /D "_AFXDLL" /D "_MBCS" /Yc"stdafx.h" /GF /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "UWL_SERVICE" /D "_AFXDLL" /D "_MBCS" /FR /Yc"stdafx.h" /GF /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 uwl.lib uwlnt.lib /nologo /subsystem:console /debug /machine:IX86 /out:"$(OutDir)/ZgsaChunkSvr.exe" /pdbtype:sept /opt:ref /opt:icf
# ADD LINK32 tcns.lib uwl.lib uwlnt.lib xyapi.lib TcyCommon.lib /nologo /subsystem:console /debug /machine:IX86 /out:".\ReleaseS/ZgsaChunkSvr.exe" /pdbtype:sept /opt:ref /opt:icf

!ELSEIF  "$(CFG)" == "ZgsaChunkSvr - Win32 ReleaseS125"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ZgsaChunkSvr___Win32_ReleaseS125"
# PROP BASE Intermediate_Dir "ZgsaChunkSvr___Win32_ReleaseS125"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseS125"
# PROP Intermediate_Dir "ReleaseS125"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /win32
# ADD MTL /nologo /win32
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "UWL_SERVICE" /D "_AFXDLL" /D "_MBCS" /FR /Yc"stdafx.h" /GF /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /D "_RS125" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "UWL_SERVICE" /D "_AFXDLL" /D "_MBCS" /FR /Yc"stdafx.h" /GF /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 tcns.lib uwl.lib uwlnt.lib xyapi.lib /nologo /subsystem:console /debug /machine:IX86 /out:".\ReleaseS/ZgsaChunkSvr.exe" /pdbtype:sept /opt:ref /opt:icf
# ADD LINK32 tcns.lib uwl.lib uwlnt.lib xyapi.lib /nologo /subsystem:console /debug /machine:IX86 /out:".\ReleaseS125/ZgsaChunkSvr.exe" /pdbtype:sept /opt:ref /opt:icf

!ENDIF 

# Begin Target

# Name "ZgsaChunkSvr - Win32 Debug"
# Name "ZgsaChunkSvr - Win32 Release"
# Name "ZgsaChunkSvr - Win32 DebugS"
# Name "ZgsaChunkSvr - Win32 ReleaseS"
# Name "ZgsaChunkSvr - Win32 ReleaseS125"
# Begin Group "源"

# PROP Default_Filter "cpp;c;cxx;def;odl;idl;hpj;bat;asm"
# Begin Source File

SOURCE=.\DBCommon.cpp
DEP_CPP_DBCOM=\
	".\ChunkDef.h"\
	".\DBCommon.h"\
	".\DBOperate.h"\
	".\Main.h"\
	".\Server.h"\
	".\Service.h"\
	".\SockClient.h"\
	".\SockSvr.h"\
	".\stdafx.h"\
	
NODEP_CPP_DBCOM=\
	"..\COMMON\zgsa\CommonDef.h"\
	"..\COMMON\zgsa\CommonReq.h"\
	
# ADD CPP /YX"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\DBExample.cpp
DEP_CPP_DBEXA=\
	".\ChunkDef.h"\
	".\DBCommon.h"\
	".\DBOperate.h"\
	".\Main.h"\
	".\Server.h"\
	".\Service.h"\
	".\SockClient.h"\
	".\SockSvr.h"\
	".\stdafx.h"\
	
NODEP_CPP_DBEXA=\
	"..\COMMON\zgsa\CommonDef.h"\
	"..\COMMON\zgsa\CommonReq.h"\
	

!IF  "$(CFG)" == "ZgsaChunkSvr - Win32 Debug"

# ADD CPP /YX"stdafx.h"

!ELSEIF  "$(CFG)" == "ZgsaChunkSvr - Win32 Release"

!ELSEIF  "$(CFG)" == "ZgsaChunkSvr - Win32 DebugS"

!ELSEIF  "$(CFG)" == "ZgsaChunkSvr - Win32 ReleaseS"

!ELSEIF  "$(CFG)" == "ZgsaChunkSvr - Win32 ReleaseS125"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DBOperate.cpp
DEP_CPP_DBOPE=\
	".\ChunkDef.h"\
	".\DBCommon.h"\
	".\DBOperate.h"\
	".\Main.h"\
	".\Server.h"\
	".\Service.h"\
	".\SockClient.h"\
	".\SockSvr.h"\
	".\stdafx.h"\
	
NODEP_CPP_DBOPE=\
	"..\COMMON\zgsa\CommonDef.h"\
	"..\COMMON\zgsa\CommonReq.h"\
	

!IF  "$(CFG)" == "ZgsaChunkSvr - Win32 Debug"

# ADD CPP /YX"stdafx.h"

!ELSEIF  "$(CFG)" == "ZgsaChunkSvr - Win32 Release"

!ELSEIF  "$(CFG)" == "ZgsaChunkSvr - Win32 DebugS"

!ELSEIF  "$(CFG)" == "ZgsaChunkSvr - Win32 ReleaseS"

# ADD CPP /YX"stdafx.h"

!ELSEIF  "$(CFG)" == "ZgsaChunkSvr - Win32 ReleaseS125"

# ADD CPP /YX"stdafx.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=Main.cpp
DEP_CPP_MAIN_=\
	".\ChunkDef.h"\
	".\DBCommon.h"\
	".\DBOperate.h"\
	".\Main.h"\
	".\Server.h"\
	".\Service.h"\
	".\SockClient.h"\
	".\SockSvr.h"\
	".\stdafx.h"\
	
NODEP_CPP_MAIN_=\
	"..\COMMON\zgsa\CommonDef.h"\
	"..\COMMON\zgsa\CommonReq.h"\
	
# ADD CPP /YX"stdafx.h"
# End Source File
# Begin Source File

SOURCE=Server.cpp
DEP_CPP_SERVE=\
	".\ChunkDef.h"\
	".\DBCommon.h"\
	".\DBOperate.h"\
	".\Main.h"\
	".\Server.h"\
	".\Service.h"\
	".\SockClient.h"\
	".\SockSvr.h"\
	".\stdafx.h"\
	
NODEP_CPP_SERVE=\
	"..\COMMON\zgsa\CommonDef.h"\
	"..\COMMON\zgsa\CommonReq.h"\
	

!IF  "$(CFG)" == "ZgsaChunkSvr - Win32 Debug"

# ADD CPP /YX"stdafx.h"

!ELSEIF  "$(CFG)" == "ZgsaChunkSvr - Win32 Release"

# ADD CPP /YX"stdafx.h"

!ELSEIF  "$(CFG)" == "ZgsaChunkSvr - Win32 DebugS"

# ADD CPP /YX"stdafx.h"

!ELSEIF  "$(CFG)" == "ZgsaChunkSvr - Win32 ReleaseS"

# ADD CPP /YX"stdafx.h"

!ELSEIF  "$(CFG)" == "ZgsaChunkSvr - Win32 ReleaseS125"

# ADD CPP /D "_RSDBLOG" /YX"stdafx.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=Service.cpp
DEP_CPP_SERVI=\
	".\ChunkDef.h"\
	".\DBCommon.h"\
	".\DBOperate.h"\
	".\Main.h"\
	".\Server.h"\
	".\Service.h"\
	".\SockClient.h"\
	".\SockSvr.h"\
	".\stdafx.h"\
	
NODEP_CPP_SERVI=\
	"..\COMMON\zgsa\CommonDef.h"\
	"..\COMMON\zgsa\CommonReq.h"\
	
# ADD CPP /YX"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SockClient.cpp
DEP_CPP_SOCKC=\
	".\ChunkDef.h"\
	".\DBCommon.h"\
	".\DBOperate.h"\
	".\Main.h"\
	".\Server.h"\
	".\Service.h"\
	".\SockClient.h"\
	".\SockSvr.h"\
	".\stdafx.h"\
	
NODEP_CPP_SOCKC=\
	"..\COMMON\zgsa\CommonDef.h"\
	"..\COMMON\zgsa\CommonReq.h"\
	

!IF  "$(CFG)" == "ZgsaChunkSvr - Win32 Debug"

!ELSEIF  "$(CFG)" == "ZgsaChunkSvr - Win32 Release"

!ELSEIF  "$(CFG)" == "ZgsaChunkSvr - Win32 DebugS"

!ELSEIF  "$(CFG)" == "ZgsaChunkSvr - Win32 ReleaseS"

# ADD CPP /YX"stdafx.h"

!ELSEIF  "$(CFG)" == "ZgsaChunkSvr - Win32 ReleaseS125"

# ADD CPP /YX"stdafx.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=SockSvr.cpp
DEP_CPP_SOCKS=\
	".\ChunkDef.h"\
	".\DBCommon.h"\
	".\DBOperate.h"\
	".\Main.h"\
	".\Server.h"\
	".\Service.h"\
	".\SockClient.h"\
	".\SockSvr.h"\
	".\stdafx.h"\
	
NODEP_CPP_SOCKS=\
	"..\COMMON\zgsa\CommonDef.h"\
	"..\COMMON\zgsa\CommonReq.h"\
	
# ADD CPP /YX"stdafx.h"
# End Source File
# Begin Source File

SOURCE=stdafx.cpp
DEP_CPP_STDAF=\
	".\ChunkDef.h"\
	".\DBCommon.h"\
	".\DBOperate.h"\
	".\Main.h"\
	".\Server.h"\
	".\Service.h"\
	".\SockClient.h"\
	".\SockSvr.h"\
	".\stdafx.h"\
	
NODEP_CPP_STDAF=\
	"..\COMMON\zgsa\CommonDef.h"\
	"..\COMMON\zgsa\CommonReq.h"\
	
# ADD CPP /YX"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\UnitTest.cpp
DEP_CPP_UNITT=\
	".\ChunkDef.h"\
	".\DBCommon.h"\
	".\DBOperate.h"\
	".\Main.h"\
	".\Server.h"\
	".\Service.h"\
	".\SockClient.h"\
	".\SockSvr.h"\
	".\stdafx.h"\
	".\UnitTest.h"\
	
NODEP_CPP_UNITT=\
	"..\COMMON\zgsa\CommonDef.h"\
	"..\COMMON\zgsa\CommonReq.h"\
	

!IF  "$(CFG)" == "ZgsaChunkSvr - Win32 Debug"

# ADD CPP /YX"stdafx.h"

!ELSEIF  "$(CFG)" == "ZgsaChunkSvr - Win32 Release"

!ELSEIF  "$(CFG)" == "ZgsaChunkSvr - Win32 DebugS"

!ELSEIF  "$(CFG)" == "ZgsaChunkSvr - Win32 ReleaseS"

# ADD CPP /YX"stdafx.h"

!ELSEIF  "$(CFG)" == "ZgsaChunkSvr - Win32 ReleaseS125"

# ADD CPP /YX"stdafx.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\User.cpp
DEP_CPP_USER_=\
	".\ChunkDef.h"\
	".\DBCommon.h"\
	".\DBOperate.h"\
	".\Main.h"\
	".\Server.h"\
	".\Service.h"\
	".\SockClient.h"\
	".\SockSvr.h"\
	".\stdafx.h"\
	
NODEP_CPP_USER_=\
	"..\COMMON\zgsa\CommonDef.h"\
	"..\COMMON\zgsa\CommonReq.h"\
	
# ADD CPP /YX"stdafx.h"
# End Source File
# End Group
# Begin Group "头"

# PROP Default_Filter "h;hpp;hxx;hm;inl;inc"
# Begin Source File

SOURCE=.\ChunkDef.h
# End Source File
# Begin Source File

SOURCE=..\COMMON\zgsa\CommonDef.h
# End Source File
# Begin Source File

SOURCE=..\COMMON\zgsa\CommonReq.h
# End Source File
# Begin Source File

SOURCE=.\DBCommon.h
# End Source File
# Begin Source File

SOURCE=.\DBOperate.h
# End Source File
# Begin Source File

SOURCE=Main.h
# End Source File
# Begin Source File

SOURCE=Resource.h
# End Source File
# Begin Source File

SOURCE=Server.h
# End Source File
# Begin Source File

SOURCE=Service.h
# End Source File
# Begin Source File

SOURCE=.\SockClient.h
# End Source File
# Begin Source File

SOURCE=SockSvr.h
# End Source File
# Begin Source File

SOURCE=.\stdafx.h
# End Source File
# Begin Source File

SOURCE=.\UnitTest.h
# End Source File
# End Group
# Begin Group "资源"

# PROP Default_Filter "rc;ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=ZgsaChunkSvr.rc
# End Source File
# End Group
# End Target
# End Project
