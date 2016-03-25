# Microsoft Developer Studio Project File - Name="RoomSvrZgSa" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=RoomSvrZgSa - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "RoomSvrZgSa.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RoomSvrZgSa.mak" CFG="RoomSvrZgSa - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RoomSvrZgSa - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "RoomSvrZgSa - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "RoomSvrZgSa - Win32 DebugS" (based on "Win32 (x86) Console Application")
!MESSAGE "RoomSvrZgSa - Win32 ReleaseS" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RoomSvrZgSa - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "UWL_TRACE" /D "_AFXDLL" /D "_MBCS" /Fr /Yu"stdafx.h" /GZ /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 uwld.lib uwlntd.lib uwltrace.lib /nologo /subsystem:console /pdb:"$(OutDir)/RoomSvrZgSa.pdb" /debug /machine:IX86 /out:"$(OutDir)/RoomSvrZgSaDC.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 tcnsd.lib uwld.lib uwlntd.lib uwltrace.lib xyapid.lib TcyCommond.lib RoomOpend.lib RoomBasicd.lib /nologo /subsystem:console /incremental:no /pdb:"$(OutDir)/RoomSvrZgSa.pdb" /map /debug /machine:IX86 /out:".\Debug/RoomSvrZgSaDC.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "RoomSvrZgSa - Win32 Release"

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
# ADD BASE LINK32 uwl.lib uwlnt.lib /nologo /subsystem:console /debug /machine:IX86 /out:"$(OutDir)/RoomSvrZgSaC.exe" /pdbtype:sept /opt:ref /opt:icf
# ADD LINK32 tcns.lib uwl.lib uwlnt.lib uwltrace.lib xyapi.lib TcyCommon.lib RoomOpen.lib RoomBasic.lib /nologo /subsystem:console /debug /machine:IX86 /out:".\Release/RoomSvrZgSaC.exe" /pdbtype:sept /opt:ref /opt:icf
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
OutDir=.\Release
SOURCE="$(InputPath)"
PostBuild_Cmds=copy $(OutDir)\*.exe .\..\..\bin
# End Special Build Tool

!ELSEIF  "$(CFG)" == "RoomSvrZgSa - Win32 DebugS"

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
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 uwld.lib uwlntd.lib uwltrace.lib /nologo /subsystem:console /pdb:"$(OutDir)/RoomSvrZgSa.pdb" /debug /machine:IX86 /out:"$(OutDir)/RoomSvrZgSaD.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 uwld.lib uwlntd.lib uwltrace.lib /nologo /subsystem:console /pdb:"$(OutDir)/RoomSvrZgSa.pdb" /debug /machine:IX86 /out:".\DebugS/RoomSvrZgSaD.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
OutDir=.\DebugS
SOURCE="$(InputPath)"
PostBuild_Cmds=copy $(OutDir)\*.exe .\..\..\bin
# End Special Build Tool

!ELSEIF  "$(CFG)" == "RoomSvrZgSa - Win32 ReleaseS"

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
# ADD CPP /nologo /MD /W3 /GX /Zi /Od /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "UWL_SERVICE" /D "_AFXDLL" /D "_MBCS" /FR /Yc"stdafx.h" /GF /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 uwl.lib uwlnt.lib /nologo /subsystem:console /debug /machine:IX86 /out:"$(OutDir)/RoomSvrZgSa.exe" /pdbtype:sept /opt:ref /opt:icf
# ADD LINK32 tcns.lib uwl.lib uwlnt.lib xyapi.lib TcyCommon.lib RoomBasic.lib RoomOpen.lib /nologo /subsystem:console /map /debug /machine:IX86 /out:".\ReleaseS/RoomSvrZgSa.exe" /pdbtype:sept /opt:ref /opt:icf
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "RoomSvrZgSa - Win32 Debug"
# Name "RoomSvrZgSa - Win32 Release"
# Name "RoomSvrZgSa - Win32 DebugS"
# Name "RoomSvrZgSa - Win32 ReleaseS"
# Begin Group "源"

# PROP Default_Filter "cpp;c;cxx;def;odl;idl;hpj;bat;asm"
# Begin Source File

SOURCE=Main.cpp
DEP_CPP_MAIN_=\
	".\define.h"\
	".\Main.h"\
	".\MainServer.h"\
	".\RoomData.h"\
	".\RoomServer.h"\
	".\Service.h"\
	".\SockClient.h"\
	".\SockServer.h"\
	".\stdafx.h"\
	{$(INCLUDE)}"comapi.h"\
	{$(INCLUDE)}"CommonData.h"\
	{$(INCLUDE)}"CommReq-.h"\
	{$(INCLUDE)}"CommReqD.h"\
	{$(INCLUDE)}"CoreReq2.h"\
	{$(INCLUDE)}"DbgHelp.h"\
	{$(INCLUDE)}"DefIocpClient.h"\
	{$(INCLUDE)}"DefIocpServer.h"\
	{$(INCLUDE)}"DefSocketClient.h"\
	{$(INCLUDE)}"FileVersionInfo.h"\
	{$(INCLUDE)}"GameDef.h"\
	{$(INCLUDE)}"GameReq-.h"\
	{$(INCLUDE)}"InsideReq-.h"\
	{$(INCLUDE)}"ipexport.h"\
	{$(INCLUDE)}"iphlpapi.h"\
	{$(INCLUDE)}"IPPort.h"\
	{$(INCLUDE)}"iptypes.h"\
	{$(INCLUDE)}"MainOpenServer.h"\
	{$(INCLUDE)}"matchcore.h"\
	{$(INCLUDE)}"matchdef.h"\
	{$(INCLUDE)}"matchgamereq.h"\
	{$(INCLUDE)}"matchreq.h"\
	{$(INCLUDE)}"matchroomreq.h"\
	{$(INCLUDE)}"MobileReq-.h"\
	{$(INCLUDE)}"msado15.tlh"\
	{$(INCLUDE)}"msado15.tli"\
	{$(INCLUDE)}"msjro.tlh"\
	{$(INCLUDE)}"msjro.tli"\
	{$(INCLUDE)}"ReqData.h"\
	{$(INCLUDE)}"RoomBasic.h"\
	{$(INCLUDE)}"roomdata.h"\
	{$(INCLUDE)}"RoomDef.h"\
	{$(INCLUDE)}"RoomOpen.h"\
	{$(INCLUDE)}"roomOpenData.h"\
	{$(INCLUDE)}"RoomOpenServer.h"\
	{$(INCLUDE)}"RoomSvr.h"\
	{$(INCLUDE)}"Server.h"\
	{$(INCLUDE)}"SockClt.h"\
	{$(INCLUDE)}"SockOpenClient.h"\
	{$(INCLUDE)}"SockOpenServer.h"\
	{$(INCLUDE)}"SockSvr.h"\
	{$(INCLUDE)}"TaskReq.h"\
	{$(INCLUDE)}"tblField.h"\
	{$(INCLUDE)}"tciocp.h"\
	{$(INCLUDE)}"tcMatch.h"\
	{$(INCLUDE)}"tcns.h"\
	{$(INCLUDE)}"tcudp.h"\
	{$(INCLUDE)}"TcyCommon.h"\
	{$(INCLUDE)}"tools.h"\
	{$(INCLUDE)}"uwl.h"\
	{$(INCLUDE)}"uwl64cod.h"\
	{$(INCLUDE)}"uwlaes.h"\
	{$(INCLUDE)}"uwlclint.h"\
	{$(INCLUDE)}"uwlcombi.h"\
	{$(INCLUDE)}"uwlcrc32.h"\
	{$(INCLUDE)}"uwldef.h"\
	{$(INCLUDE)}"uwlerr.h"\
	{$(INCLUDE)}"uwlflate.h"\
	{$(INCLUDE)}"uwllic.h"\
	{$(INCLUDE)}"uwllog.h"\
	{$(INCLUDE)}"uwlmac.h"\
	{$(INCLUDE)}"uwlmain.h"\
	{$(INCLUDE)}"uwlmd5.h"\
	{$(INCLUDE)}"uwlnt.h"\
	{$(INCLUDE)}"uwlpack.h"\
	{$(INCLUDE)}"uwlproxy.h"\
	{$(INCLUDE)}"uwlreq.h"\
	{$(INCLUDE)}"uwlsock.h"\
	{$(INCLUDE)}"uwlsvc.h"\
	{$(INCLUDE)}"uwlsvcmg.h"\
	{$(INCLUDE)}"uwlsvcwd.h"\
	{$(INCLUDE)}"uwlsvr.h"\
	{$(INCLUDE)}"uwltrace.h"\
	{$(INCLUDE)}"uwludpcl.h"\
	{$(INCLUDE)}"uwlutil.h"\
	{$(INCLUDE)}"uwlwork.h"\
	{$(INCLUDE)}"xyapi.h"\
	{$(INCLUDE)}"xygext.h"\
	{$(INCLUDE)}"yuepai.h"\
	{$(INCLUDE)}"yuepaidef.h"\
	{$(INCLUDE)}"yuepaireq.h"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	

!IF  "$(CFG)" == "RoomSvrZgSa - Win32 Debug"

# ADD CPP /YX"stdafx.h"

!ELSEIF  "$(CFG)" == "RoomSvrZgSa - Win32 Release"

# ADD CPP /YX"stdafx.h"

!ELSEIF  "$(CFG)" == "RoomSvrZgSa - Win32 DebugS"

# ADD CPP /YX"stdafx.h"

!ELSEIF  "$(CFG)" == "RoomSvrZgSa - Win32 ReleaseS"

# SUBTRACT CPP /YX /Yc

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MainServer.cpp
DEP_CPP_MAINS=\
	".\define.h"\
	".\Main.h"\
	".\MainServer.h"\
	".\RoomData.h"\
	".\RoomServer.h"\
	".\Service.h"\
	".\SockClient.h"\
	".\SockServer.h"\
	".\stdafx.h"\
	{$(INCLUDE)}"comapi.h"\
	{$(INCLUDE)}"CommonData.h"\
	{$(INCLUDE)}"CommReq-.h"\
	{$(INCLUDE)}"CommReqD.h"\
	{$(INCLUDE)}"CoreReq2.h"\
	{$(INCLUDE)}"DefIocpClient.h"\
	{$(INCLUDE)}"DefIocpServer.h"\
	{$(INCLUDE)}"DefSocketClient.h"\
	{$(INCLUDE)}"FileVersionInfo.h"\
	{$(INCLUDE)}"GameDef.h"\
	{$(INCLUDE)}"GameReq-.h"\
	{$(INCLUDE)}"InsideReq-.h"\
	{$(INCLUDE)}"ipexport.h"\
	{$(INCLUDE)}"iphlpapi.h"\
	{$(INCLUDE)}"IPPort.h"\
	{$(INCLUDE)}"iptypes.h"\
	{$(INCLUDE)}"MainOpenServer.h"\
	{$(INCLUDE)}"matchcore.h"\
	{$(INCLUDE)}"matchdef.h"\
	{$(INCLUDE)}"matchgamereq.h"\
	{$(INCLUDE)}"matchreq.h"\
	{$(INCLUDE)}"matchroomreq.h"\
	{$(INCLUDE)}"MobileReq-.h"\
	{$(INCLUDE)}"msado15.tlh"\
	{$(INCLUDE)}"msado15.tli"\
	{$(INCLUDE)}"msjro.tlh"\
	{$(INCLUDE)}"msjro.tli"\
	{$(INCLUDE)}"ReqData.h"\
	{$(INCLUDE)}"RoomBasic.h"\
	{$(INCLUDE)}"roomdata.h"\
	{$(INCLUDE)}"RoomDef.h"\
	{$(INCLUDE)}"RoomOpen.h"\
	{$(INCLUDE)}"roomOpenData.h"\
	{$(INCLUDE)}"RoomOpenServer.h"\
	{$(INCLUDE)}"RoomSvr.h"\
	{$(INCLUDE)}"Server.h"\
	{$(INCLUDE)}"SockClt.h"\
	{$(INCLUDE)}"SockOpenClient.h"\
	{$(INCLUDE)}"SockOpenServer.h"\
	{$(INCLUDE)}"SockSvr.h"\
	{$(INCLUDE)}"TaskReq.h"\
	{$(INCLUDE)}"tblField.h"\
	{$(INCLUDE)}"tciocp.h"\
	{$(INCLUDE)}"tcMatch.h"\
	{$(INCLUDE)}"tcns.h"\
	{$(INCLUDE)}"tcudp.h"\
	{$(INCLUDE)}"TcyCommon.h"\
	{$(INCLUDE)}"tools.h"\
	{$(INCLUDE)}"uwl.h"\
	{$(INCLUDE)}"uwl64cod.h"\
	{$(INCLUDE)}"uwlaes.h"\
	{$(INCLUDE)}"uwlclint.h"\
	{$(INCLUDE)}"uwlcombi.h"\
	{$(INCLUDE)}"uwlcrc32.h"\
	{$(INCLUDE)}"uwldef.h"\
	{$(INCLUDE)}"uwlerr.h"\
	{$(INCLUDE)}"uwlflate.h"\
	{$(INCLUDE)}"uwllic.h"\
	{$(INCLUDE)}"uwllog.h"\
	{$(INCLUDE)}"uwlmac.h"\
	{$(INCLUDE)}"uwlmain.h"\
	{$(INCLUDE)}"uwlmd5.h"\
	{$(INCLUDE)}"uwlnt.h"\
	{$(INCLUDE)}"uwlpack.h"\
	{$(INCLUDE)}"uwlproxy.h"\
	{$(INCLUDE)}"uwlreq.h"\
	{$(INCLUDE)}"uwlsock.h"\
	{$(INCLUDE)}"uwlsvc.h"\
	{$(INCLUDE)}"uwlsvcmg.h"\
	{$(INCLUDE)}"uwlsvcwd.h"\
	{$(INCLUDE)}"uwlsvr.h"\
	{$(INCLUDE)}"uwltrace.h"\
	{$(INCLUDE)}"uwludpcl.h"\
	{$(INCLUDE)}"uwlutil.h"\
	{$(INCLUDE)}"uwlwork.h"\
	{$(INCLUDE)}"xyapi.h"\
	{$(INCLUDE)}"xygext.h"\
	{$(INCLUDE)}"yuepai.h"\
	{$(INCLUDE)}"yuepaidef.h"\
	{$(INCLUDE)}"yuepaireq.h"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	

!IF  "$(CFG)" == "RoomSvrZgSa - Win32 Debug"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "RoomSvrZgSa - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "RoomSvrZgSa - Win32 DebugS"

!ELSEIF  "$(CFG)" == "RoomSvrZgSa - Win32 ReleaseS"

# SUBTRACT CPP /YX /Yc

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RoomData.cpp
DEP_CPP_ROOMD=\
	".\define.h"\
	".\Main.h"\
	".\MainServer.h"\
	".\RoomData.h"\
	".\RoomServer.h"\
	".\Service.h"\
	".\SockClient.h"\
	".\SockServer.h"\
	".\stdafx.h"\
	{$(INCLUDE)}"comapi.h"\
	{$(INCLUDE)}"CommonData.h"\
	{$(INCLUDE)}"CommReq-.h"\
	{$(INCLUDE)}"CommReqD.h"\
	{$(INCLUDE)}"CoreReq2.h"\
	{$(INCLUDE)}"DefIocpClient.h"\
	{$(INCLUDE)}"DefIocpServer.h"\
	{$(INCLUDE)}"DefSocketClient.h"\
	{$(INCLUDE)}"FileVersionInfo.h"\
	{$(INCLUDE)}"GameDef.h"\
	{$(INCLUDE)}"GameReq-.h"\
	{$(INCLUDE)}"InsideReq-.h"\
	{$(INCLUDE)}"ipexport.h"\
	{$(INCLUDE)}"iphlpapi.h"\
	{$(INCLUDE)}"IPPort.h"\
	{$(INCLUDE)}"iptypes.h"\
	{$(INCLUDE)}"MainOpenServer.h"\
	{$(INCLUDE)}"matchcore.h"\
	{$(INCLUDE)}"matchdef.h"\
	{$(INCLUDE)}"matchgamereq.h"\
	{$(INCLUDE)}"matchreq.h"\
	{$(INCLUDE)}"matchroomreq.h"\
	{$(INCLUDE)}"MobileReq-.h"\
	{$(INCLUDE)}"msado15.tlh"\
	{$(INCLUDE)}"msado15.tli"\
	{$(INCLUDE)}"msjro.tlh"\
	{$(INCLUDE)}"msjro.tli"\
	{$(INCLUDE)}"ReqData.h"\
	{$(INCLUDE)}"RoomBasic.h"\
	{$(INCLUDE)}"roomdata.h"\
	{$(INCLUDE)}"RoomDef.h"\
	{$(INCLUDE)}"RoomOpen.h"\
	{$(INCLUDE)}"roomOpenData.h"\
	{$(INCLUDE)}"RoomOpenServer.h"\
	{$(INCLUDE)}"RoomSvr.h"\
	{$(INCLUDE)}"Server.h"\
	{$(INCLUDE)}"SockClt.h"\
	{$(INCLUDE)}"SockOpenClient.h"\
	{$(INCLUDE)}"SockOpenServer.h"\
	{$(INCLUDE)}"SockSvr.h"\
	{$(INCLUDE)}"TaskReq.h"\
	{$(INCLUDE)}"tblField.h"\
	{$(INCLUDE)}"tciocp.h"\
	{$(INCLUDE)}"tcMatch.h"\
	{$(INCLUDE)}"tcns.h"\
	{$(INCLUDE)}"tcudp.h"\
	{$(INCLUDE)}"TcyCommon.h"\
	{$(INCLUDE)}"tools.h"\
	{$(INCLUDE)}"uwl.h"\
	{$(INCLUDE)}"uwl64cod.h"\
	{$(INCLUDE)}"uwlaes.h"\
	{$(INCLUDE)}"uwlclint.h"\
	{$(INCLUDE)}"uwlcombi.h"\
	{$(INCLUDE)}"uwlcrc32.h"\
	{$(INCLUDE)}"uwldef.h"\
	{$(INCLUDE)}"uwlerr.h"\
	{$(INCLUDE)}"uwlflate.h"\
	{$(INCLUDE)}"uwllic.h"\
	{$(INCLUDE)}"uwllog.h"\
	{$(INCLUDE)}"uwlmac.h"\
	{$(INCLUDE)}"uwlmain.h"\
	{$(INCLUDE)}"uwlmd5.h"\
	{$(INCLUDE)}"uwlnt.h"\
	{$(INCLUDE)}"uwlpack.h"\
	{$(INCLUDE)}"uwlproxy.h"\
	{$(INCLUDE)}"uwlreq.h"\
	{$(INCLUDE)}"uwlsock.h"\
	{$(INCLUDE)}"uwlsvc.h"\
	{$(INCLUDE)}"uwlsvcmg.h"\
	{$(INCLUDE)}"uwlsvcwd.h"\
	{$(INCLUDE)}"uwlsvr.h"\
	{$(INCLUDE)}"uwltrace.h"\
	{$(INCLUDE)}"uwludpcl.h"\
	{$(INCLUDE)}"uwlutil.h"\
	{$(INCLUDE)}"uwlwork.h"\
	{$(INCLUDE)}"xyapi.h"\
	{$(INCLUDE)}"xygext.h"\
	{$(INCLUDE)}"yuepai.h"\
	{$(INCLUDE)}"yuepaidef.h"\
	{$(INCLUDE)}"yuepaireq.h"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	

!IF  "$(CFG)" == "RoomSvrZgSa - Win32 Debug"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "RoomSvrZgSa - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "RoomSvrZgSa - Win32 DebugS"

!ELSEIF  "$(CFG)" == "RoomSvrZgSa - Win32 ReleaseS"

# SUBTRACT CPP /YX /Yc

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RoomServer.cpp
DEP_CPP_ROOMS=\
	".\define.h"\
	".\Main.h"\
	".\MainServer.h"\
	".\RoomData.h"\
	".\RoomServer.h"\
	".\Service.h"\
	".\SockClient.h"\
	".\SockServer.h"\
	".\stdafx.h"\
	{$(INCLUDE)}"comapi.h"\
	{$(INCLUDE)}"CommonData.h"\
	{$(INCLUDE)}"CommReq-.h"\
	{$(INCLUDE)}"CommReqD.h"\
	{$(INCLUDE)}"CoreReq2.h"\
	{$(INCLUDE)}"DefIocpClient.h"\
	{$(INCLUDE)}"DefIocpServer.h"\
	{$(INCLUDE)}"DefSocketClient.h"\
	{$(INCLUDE)}"FileVersionInfo.h"\
	{$(INCLUDE)}"GameDef.h"\
	{$(INCLUDE)}"GameReq-.h"\
	{$(INCLUDE)}"InsideReq-.h"\
	{$(INCLUDE)}"ipexport.h"\
	{$(INCLUDE)}"iphlpapi.h"\
	{$(INCLUDE)}"IPPort.h"\
	{$(INCLUDE)}"iptypes.h"\
	{$(INCLUDE)}"MainOpenServer.h"\
	{$(INCLUDE)}"matchcore.h"\
	{$(INCLUDE)}"matchdef.h"\
	{$(INCLUDE)}"matchgamereq.h"\
	{$(INCLUDE)}"matchreq.h"\
	{$(INCLUDE)}"matchroomreq.h"\
	{$(INCLUDE)}"MobileReq-.h"\
	{$(INCLUDE)}"msado15.tlh"\
	{$(INCLUDE)}"msado15.tli"\
	{$(INCLUDE)}"msjro.tlh"\
	{$(INCLUDE)}"msjro.tli"\
	{$(INCLUDE)}"ReqData.h"\
	{$(INCLUDE)}"RoomBasic.h"\
	{$(INCLUDE)}"roomdata.h"\
	{$(INCLUDE)}"RoomDef.h"\
	{$(INCLUDE)}"RoomOpen.h"\
	{$(INCLUDE)}"roomOpenData.h"\
	{$(INCLUDE)}"RoomOpenServer.h"\
	{$(INCLUDE)}"RoomSvr.h"\
	{$(INCLUDE)}"Server.h"\
	{$(INCLUDE)}"SockClt.h"\
	{$(INCLUDE)}"SockOpenClient.h"\
	{$(INCLUDE)}"SockOpenServer.h"\
	{$(INCLUDE)}"SockSvr.h"\
	{$(INCLUDE)}"TaskReq.h"\
	{$(INCLUDE)}"tblField.h"\
	{$(INCLUDE)}"tciocp.h"\
	{$(INCLUDE)}"tcMatch.h"\
	{$(INCLUDE)}"tcns.h"\
	{$(INCLUDE)}"tcudp.h"\
	{$(INCLUDE)}"TcyCommon.h"\
	{$(INCLUDE)}"tools.h"\
	{$(INCLUDE)}"uwl.h"\
	{$(INCLUDE)}"uwl64cod.h"\
	{$(INCLUDE)}"uwlaes.h"\
	{$(INCLUDE)}"uwlclint.h"\
	{$(INCLUDE)}"uwlcombi.h"\
	{$(INCLUDE)}"uwlcrc32.h"\
	{$(INCLUDE)}"uwldef.h"\
	{$(INCLUDE)}"uwlerr.h"\
	{$(INCLUDE)}"uwlflate.h"\
	{$(INCLUDE)}"uwllic.h"\
	{$(INCLUDE)}"uwllog.h"\
	{$(INCLUDE)}"uwlmac.h"\
	{$(INCLUDE)}"uwlmain.h"\
	{$(INCLUDE)}"uwlmd5.h"\
	{$(INCLUDE)}"uwlnt.h"\
	{$(INCLUDE)}"uwlpack.h"\
	{$(INCLUDE)}"uwlproxy.h"\
	{$(INCLUDE)}"uwlreq.h"\
	{$(INCLUDE)}"uwlsock.h"\
	{$(INCLUDE)}"uwlsvc.h"\
	{$(INCLUDE)}"uwlsvcmg.h"\
	{$(INCLUDE)}"uwlsvcwd.h"\
	{$(INCLUDE)}"uwlsvr.h"\
	{$(INCLUDE)}"uwltrace.h"\
	{$(INCLUDE)}"uwludpcl.h"\
	{$(INCLUDE)}"uwlutil.h"\
	{$(INCLUDE)}"uwlwork.h"\
	{$(INCLUDE)}"xyapi.h"\
	{$(INCLUDE)}"xygext.h"\
	{$(INCLUDE)}"yuepai.h"\
	{$(INCLUDE)}"yuepaidef.h"\
	{$(INCLUDE)}"yuepaireq.h"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	

!IF  "$(CFG)" == "RoomSvrZgSa - Win32 Debug"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "RoomSvrZgSa - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "RoomSvrZgSa - Win32 DebugS"

!ELSEIF  "$(CFG)" == "RoomSvrZgSa - Win32 ReleaseS"

# SUBTRACT CPP /YX /Yc

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Service.cpp
DEP_CPP_SERVI=\
	".\define.h"\
	".\Main.h"\
	".\MainServer.h"\
	".\RoomData.h"\
	".\RoomServer.h"\
	".\Service.h"\
	".\SockClient.h"\
	".\SockServer.h"\
	".\stdafx.h"\
	{$(INCLUDE)}"comapi.h"\
	{$(INCLUDE)}"CommonData.h"\
	{$(INCLUDE)}"CommReq-.h"\
	{$(INCLUDE)}"CommReqD.h"\
	{$(INCLUDE)}"CoreReq2.h"\
	{$(INCLUDE)}"DefIocpClient.h"\
	{$(INCLUDE)}"DefIocpServer.h"\
	{$(INCLUDE)}"DefSocketClient.h"\
	{$(INCLUDE)}"FileVersionInfo.h"\
	{$(INCLUDE)}"GameDef.h"\
	{$(INCLUDE)}"GameReq-.h"\
	{$(INCLUDE)}"InsideReq-.h"\
	{$(INCLUDE)}"ipexport.h"\
	{$(INCLUDE)}"iphlpapi.h"\
	{$(INCLUDE)}"IPPort.h"\
	{$(INCLUDE)}"iptypes.h"\
	{$(INCLUDE)}"MainOpenServer.h"\
	{$(INCLUDE)}"matchcore.h"\
	{$(INCLUDE)}"matchdef.h"\
	{$(INCLUDE)}"matchgamereq.h"\
	{$(INCLUDE)}"matchreq.h"\
	{$(INCLUDE)}"matchroomreq.h"\
	{$(INCLUDE)}"MobileReq-.h"\
	{$(INCLUDE)}"msado15.tlh"\
	{$(INCLUDE)}"msado15.tli"\
	{$(INCLUDE)}"msjro.tlh"\
	{$(INCLUDE)}"msjro.tli"\
	{$(INCLUDE)}"ReqData.h"\
	{$(INCLUDE)}"RoomBasic.h"\
	{$(INCLUDE)}"roomdata.h"\
	{$(INCLUDE)}"RoomDef.h"\
	{$(INCLUDE)}"RoomOpen.h"\
	{$(INCLUDE)}"roomOpenData.h"\
	{$(INCLUDE)}"RoomOpenServer.h"\
	{$(INCLUDE)}"RoomSvr.h"\
	{$(INCLUDE)}"Server.h"\
	{$(INCLUDE)}"SockClt.h"\
	{$(INCLUDE)}"SockOpenClient.h"\
	{$(INCLUDE)}"SockOpenServer.h"\
	{$(INCLUDE)}"SockSvr.h"\
	{$(INCLUDE)}"TaskReq.h"\
	{$(INCLUDE)}"tblField.h"\
	{$(INCLUDE)}"tciocp.h"\
	{$(INCLUDE)}"tcMatch.h"\
	{$(INCLUDE)}"tcns.h"\
	{$(INCLUDE)}"tcudp.h"\
	{$(INCLUDE)}"TcyCommon.h"\
	{$(INCLUDE)}"tools.h"\
	{$(INCLUDE)}"uwl.h"\
	{$(INCLUDE)}"uwl64cod.h"\
	{$(INCLUDE)}"uwlaes.h"\
	{$(INCLUDE)}"uwlclint.h"\
	{$(INCLUDE)}"uwlcombi.h"\
	{$(INCLUDE)}"uwlcrc32.h"\
	{$(INCLUDE)}"uwldef.h"\
	{$(INCLUDE)}"uwlerr.h"\
	{$(INCLUDE)}"uwlflate.h"\
	{$(INCLUDE)}"uwllic.h"\
	{$(INCLUDE)}"uwllog.h"\
	{$(INCLUDE)}"uwlmac.h"\
	{$(INCLUDE)}"uwlmain.h"\
	{$(INCLUDE)}"uwlmd5.h"\
	{$(INCLUDE)}"uwlnt.h"\
	{$(INCLUDE)}"uwlpack.h"\
	{$(INCLUDE)}"uwlproxy.h"\
	{$(INCLUDE)}"uwlreq.h"\
	{$(INCLUDE)}"uwlsock.h"\
	{$(INCLUDE)}"uwlsvc.h"\
	{$(INCLUDE)}"uwlsvcmg.h"\
	{$(INCLUDE)}"uwlsvcwd.h"\
	{$(INCLUDE)}"uwlsvr.h"\
	{$(INCLUDE)}"uwltrace.h"\
	{$(INCLUDE)}"uwludpcl.h"\
	{$(INCLUDE)}"uwlutil.h"\
	{$(INCLUDE)}"uwlwork.h"\
	{$(INCLUDE)}"xyapi.h"\
	{$(INCLUDE)}"xygext.h"\
	{$(INCLUDE)}"yuepai.h"\
	{$(INCLUDE)}"yuepaidef.h"\
	{$(INCLUDE)}"yuepaireq.h"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	

!IF  "$(CFG)" == "RoomSvrZgSa - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "RoomSvrZgSa - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "RoomSvrZgSa - Win32 DebugS"

!ELSEIF  "$(CFG)" == "RoomSvrZgSa - Win32 ReleaseS"

# SUBTRACT CPP /YX /Yc

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SockClient.cpp
DEP_CPP_SOCKC=\
	".\define.h"\
	".\Main.h"\
	".\MainServer.h"\
	".\RoomData.h"\
	".\RoomServer.h"\
	".\Service.h"\
	".\SockClient.h"\
	".\SockServer.h"\
	".\stdafx.h"\
	{$(INCLUDE)}"comapi.h"\
	{$(INCLUDE)}"CommonData.h"\
	{$(INCLUDE)}"CommReq-.h"\
	{$(INCLUDE)}"CommReqD.h"\
	{$(INCLUDE)}"CoreReq2.h"\
	{$(INCLUDE)}"DefIocpClient.h"\
	{$(INCLUDE)}"DefIocpServer.h"\
	{$(INCLUDE)}"DefSocketClient.h"\
	{$(INCLUDE)}"FileVersionInfo.h"\
	{$(INCLUDE)}"GameDef.h"\
	{$(INCLUDE)}"GameReq-.h"\
	{$(INCLUDE)}"InsideReq-.h"\
	{$(INCLUDE)}"ipexport.h"\
	{$(INCLUDE)}"iphlpapi.h"\
	{$(INCLUDE)}"IPPort.h"\
	{$(INCLUDE)}"iptypes.h"\
	{$(INCLUDE)}"MainOpenServer.h"\
	{$(INCLUDE)}"matchcore.h"\
	{$(INCLUDE)}"matchdef.h"\
	{$(INCLUDE)}"matchgamereq.h"\
	{$(INCLUDE)}"matchreq.h"\
	{$(INCLUDE)}"matchroomreq.h"\
	{$(INCLUDE)}"MobileReq-.h"\
	{$(INCLUDE)}"msado15.tlh"\
	{$(INCLUDE)}"msado15.tli"\
	{$(INCLUDE)}"msjro.tlh"\
	{$(INCLUDE)}"msjro.tli"\
	{$(INCLUDE)}"ReqData.h"\
	{$(INCLUDE)}"RoomBasic.h"\
	{$(INCLUDE)}"roomdata.h"\
	{$(INCLUDE)}"RoomDef.h"\
	{$(INCLUDE)}"RoomOpen.h"\
	{$(INCLUDE)}"roomOpenData.h"\
	{$(INCLUDE)}"RoomOpenServer.h"\
	{$(INCLUDE)}"RoomSvr.h"\
	{$(INCLUDE)}"Server.h"\
	{$(INCLUDE)}"SockClt.h"\
	{$(INCLUDE)}"SockOpenClient.h"\
	{$(INCLUDE)}"SockOpenServer.h"\
	{$(INCLUDE)}"SockSvr.h"\
	{$(INCLUDE)}"TaskReq.h"\
	{$(INCLUDE)}"tblField.h"\
	{$(INCLUDE)}"tciocp.h"\
	{$(INCLUDE)}"tcMatch.h"\
	{$(INCLUDE)}"tcns.h"\
	{$(INCLUDE)}"tcudp.h"\
	{$(INCLUDE)}"TcyCommon.h"\
	{$(INCLUDE)}"tools.h"\
	{$(INCLUDE)}"uwl.h"\
	{$(INCLUDE)}"uwl64cod.h"\
	{$(INCLUDE)}"uwlaes.h"\
	{$(INCLUDE)}"uwlclint.h"\
	{$(INCLUDE)}"uwlcombi.h"\
	{$(INCLUDE)}"uwlcrc32.h"\
	{$(INCLUDE)}"uwldef.h"\
	{$(INCLUDE)}"uwlerr.h"\
	{$(INCLUDE)}"uwlflate.h"\
	{$(INCLUDE)}"uwllic.h"\
	{$(INCLUDE)}"uwllog.h"\
	{$(INCLUDE)}"uwlmac.h"\
	{$(INCLUDE)}"uwlmain.h"\
	{$(INCLUDE)}"uwlmd5.h"\
	{$(INCLUDE)}"uwlnt.h"\
	{$(INCLUDE)}"uwlpack.h"\
	{$(INCLUDE)}"uwlproxy.h"\
	{$(INCLUDE)}"uwlreq.h"\
	{$(INCLUDE)}"uwlsock.h"\
	{$(INCLUDE)}"uwlsvc.h"\
	{$(INCLUDE)}"uwlsvcmg.h"\
	{$(INCLUDE)}"uwlsvcwd.h"\
	{$(INCLUDE)}"uwlsvr.h"\
	{$(INCLUDE)}"uwltrace.h"\
	{$(INCLUDE)}"uwludpcl.h"\
	{$(INCLUDE)}"uwlutil.h"\
	{$(INCLUDE)}"uwlwork.h"\
	{$(INCLUDE)}"xyapi.h"\
	{$(INCLUDE)}"xygext.h"\
	{$(INCLUDE)}"yuepai.h"\
	{$(INCLUDE)}"yuepaidef.h"\
	{$(INCLUDE)}"yuepaireq.h"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	

!IF  "$(CFG)" == "RoomSvrZgSa - Win32 Debug"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "RoomSvrZgSa - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "RoomSvrZgSa - Win32 DebugS"

!ELSEIF  "$(CFG)" == "RoomSvrZgSa - Win32 ReleaseS"

# SUBTRACT CPP /YX /Yc

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SockServer.cpp
DEP_CPP_SOCKS=\
	".\define.h"\
	".\Main.h"\
	".\MainServer.h"\
	".\RoomData.h"\
	".\RoomServer.h"\
	".\Service.h"\
	".\SockClient.h"\
	".\SockServer.h"\
	".\stdafx.h"\
	{$(INCLUDE)}"comapi.h"\
	{$(INCLUDE)}"CommonData.h"\
	{$(INCLUDE)}"CommReq-.h"\
	{$(INCLUDE)}"CommReqD.h"\
	{$(INCLUDE)}"CoreReq2.h"\
	{$(INCLUDE)}"DefIocpClient.h"\
	{$(INCLUDE)}"DefIocpServer.h"\
	{$(INCLUDE)}"DefSocketClient.h"\
	{$(INCLUDE)}"FileVersionInfo.h"\
	{$(INCLUDE)}"GameDef.h"\
	{$(INCLUDE)}"GameReq-.h"\
	{$(INCLUDE)}"InsideReq-.h"\
	{$(INCLUDE)}"ipexport.h"\
	{$(INCLUDE)}"iphlpapi.h"\
	{$(INCLUDE)}"IPPort.h"\
	{$(INCLUDE)}"iptypes.h"\
	{$(INCLUDE)}"MainOpenServer.h"\
	{$(INCLUDE)}"matchcore.h"\
	{$(INCLUDE)}"matchdef.h"\
	{$(INCLUDE)}"matchgamereq.h"\
	{$(INCLUDE)}"matchreq.h"\
	{$(INCLUDE)}"matchroomreq.h"\
	{$(INCLUDE)}"MobileReq-.h"\
	{$(INCLUDE)}"msado15.tlh"\
	{$(INCLUDE)}"msado15.tli"\
	{$(INCLUDE)}"msjro.tlh"\
	{$(INCLUDE)}"msjro.tli"\
	{$(INCLUDE)}"ReqData.h"\
	{$(INCLUDE)}"RoomBasic.h"\
	{$(INCLUDE)}"roomdata.h"\
	{$(INCLUDE)}"RoomDef.h"\
	{$(INCLUDE)}"RoomOpen.h"\
	{$(INCLUDE)}"roomOpenData.h"\
	{$(INCLUDE)}"RoomOpenServer.h"\
	{$(INCLUDE)}"RoomSvr.h"\
	{$(INCLUDE)}"Server.h"\
	{$(INCLUDE)}"SockClt.h"\
	{$(INCLUDE)}"SockOpenClient.h"\
	{$(INCLUDE)}"SockOpenServer.h"\
	{$(INCLUDE)}"SockSvr.h"\
	{$(INCLUDE)}"TaskReq.h"\
	{$(INCLUDE)}"tblField.h"\
	{$(INCLUDE)}"tciocp.h"\
	{$(INCLUDE)}"tcMatch.h"\
	{$(INCLUDE)}"tcns.h"\
	{$(INCLUDE)}"tcudp.h"\
	{$(INCLUDE)}"TcyCommon.h"\
	{$(INCLUDE)}"tools.h"\
	{$(INCLUDE)}"uwl.h"\
	{$(INCLUDE)}"uwl64cod.h"\
	{$(INCLUDE)}"uwlaes.h"\
	{$(INCLUDE)}"uwlclint.h"\
	{$(INCLUDE)}"uwlcombi.h"\
	{$(INCLUDE)}"uwlcrc32.h"\
	{$(INCLUDE)}"uwldef.h"\
	{$(INCLUDE)}"uwlerr.h"\
	{$(INCLUDE)}"uwlflate.h"\
	{$(INCLUDE)}"uwllic.h"\
	{$(INCLUDE)}"uwllog.h"\
	{$(INCLUDE)}"uwlmac.h"\
	{$(INCLUDE)}"uwlmain.h"\
	{$(INCLUDE)}"uwlmd5.h"\
	{$(INCLUDE)}"uwlnt.h"\
	{$(INCLUDE)}"uwlpack.h"\
	{$(INCLUDE)}"uwlproxy.h"\
	{$(INCLUDE)}"uwlreq.h"\
	{$(INCLUDE)}"uwlsock.h"\
	{$(INCLUDE)}"uwlsvc.h"\
	{$(INCLUDE)}"uwlsvcmg.h"\
	{$(INCLUDE)}"uwlsvcwd.h"\
	{$(INCLUDE)}"uwlsvr.h"\
	{$(INCLUDE)}"uwltrace.h"\
	{$(INCLUDE)}"uwludpcl.h"\
	{$(INCLUDE)}"uwlutil.h"\
	{$(INCLUDE)}"uwlwork.h"\
	{$(INCLUDE)}"xyapi.h"\
	{$(INCLUDE)}"xygext.h"\
	{$(INCLUDE)}"yuepai.h"\
	{$(INCLUDE)}"yuepaidef.h"\
	{$(INCLUDE)}"yuepaireq.h"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	

!IF  "$(CFG)" == "RoomSvrZgSa - Win32 Debug"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "RoomSvrZgSa - Win32 Release"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "RoomSvrZgSa - Win32 DebugS"

!ELSEIF  "$(CFG)" == "RoomSvrZgSa - Win32 ReleaseS"

# SUBTRACT CPP /YX /Yc

!ENDIF 

# End Source File
# Begin Source File

SOURCE=stdafx.cpp
DEP_CPP_STDAF=\
	".\define.h"\
	".\Main.h"\
	".\MainServer.h"\
	".\RoomData.h"\
	".\RoomServer.h"\
	".\Service.h"\
	".\SockClient.h"\
	".\SockServer.h"\
	".\stdafx.h"\
	{$(INCLUDE)}"comapi.h"\
	{$(INCLUDE)}"CommonData.h"\
	{$(INCLUDE)}"CommReq-.h"\
	{$(INCLUDE)}"CommReqD.h"\
	{$(INCLUDE)}"CoreReq2.h"\
	{$(INCLUDE)}"DefIocpClient.h"\
	{$(INCLUDE)}"DefIocpServer.h"\
	{$(INCLUDE)}"DefSocketClient.h"\
	{$(INCLUDE)}"FileVersionInfo.h"\
	{$(INCLUDE)}"GameDef.h"\
	{$(INCLUDE)}"GameReq-.h"\
	{$(INCLUDE)}"InsideReq-.h"\
	{$(INCLUDE)}"ipexport.h"\
	{$(INCLUDE)}"iphlpapi.h"\
	{$(INCLUDE)}"IPPort.h"\
	{$(INCLUDE)}"iptypes.h"\
	{$(INCLUDE)}"MainOpenServer.h"\
	{$(INCLUDE)}"matchcore.h"\
	{$(INCLUDE)}"matchdef.h"\
	{$(INCLUDE)}"matchgamereq.h"\
	{$(INCLUDE)}"matchreq.h"\
	{$(INCLUDE)}"matchroomreq.h"\
	{$(INCLUDE)}"MobileReq-.h"\
	{$(INCLUDE)}"msado15.tlh"\
	{$(INCLUDE)}"msado15.tli"\
	{$(INCLUDE)}"msjro.tlh"\
	{$(INCLUDE)}"msjro.tli"\
	{$(INCLUDE)}"ReqData.h"\
	{$(INCLUDE)}"RoomBasic.h"\
	{$(INCLUDE)}"roomdata.h"\
	{$(INCLUDE)}"RoomDef.h"\
	{$(INCLUDE)}"RoomOpen.h"\
	{$(INCLUDE)}"roomOpenData.h"\
	{$(INCLUDE)}"RoomOpenServer.h"\
	{$(INCLUDE)}"RoomSvr.h"\
	{$(INCLUDE)}"Server.h"\
	{$(INCLUDE)}"SockClt.h"\
	{$(INCLUDE)}"SockOpenClient.h"\
	{$(INCLUDE)}"SockOpenServer.h"\
	{$(INCLUDE)}"SockSvr.h"\
	{$(INCLUDE)}"TaskReq.h"\
	{$(INCLUDE)}"tblField.h"\
	{$(INCLUDE)}"tciocp.h"\
	{$(INCLUDE)}"tcMatch.h"\
	{$(INCLUDE)}"tcns.h"\
	{$(INCLUDE)}"tcudp.h"\
	{$(INCLUDE)}"TcyCommon.h"\
	{$(INCLUDE)}"tools.h"\
	{$(INCLUDE)}"uwl.h"\
	{$(INCLUDE)}"uwl64cod.h"\
	{$(INCLUDE)}"uwlaes.h"\
	{$(INCLUDE)}"uwlclint.h"\
	{$(INCLUDE)}"uwlcombi.h"\
	{$(INCLUDE)}"uwlcrc32.h"\
	{$(INCLUDE)}"uwldef.h"\
	{$(INCLUDE)}"uwlerr.h"\
	{$(INCLUDE)}"uwlflate.h"\
	{$(INCLUDE)}"uwllic.h"\
	{$(INCLUDE)}"uwllog.h"\
	{$(INCLUDE)}"uwlmac.h"\
	{$(INCLUDE)}"uwlmain.h"\
	{$(INCLUDE)}"uwlmd5.h"\
	{$(INCLUDE)}"uwlnt.h"\
	{$(INCLUDE)}"uwlpack.h"\
	{$(INCLUDE)}"uwlproxy.h"\
	{$(INCLUDE)}"uwlreq.h"\
	{$(INCLUDE)}"uwlsock.h"\
	{$(INCLUDE)}"uwlsvc.h"\
	{$(INCLUDE)}"uwlsvcmg.h"\
	{$(INCLUDE)}"uwlsvcwd.h"\
	{$(INCLUDE)}"uwlsvr.h"\
	{$(INCLUDE)}"uwltrace.h"\
	{$(INCLUDE)}"uwludpcl.h"\
	{$(INCLUDE)}"uwlutil.h"\
	{$(INCLUDE)}"uwlwork.h"\
	{$(INCLUDE)}"xyapi.h"\
	{$(INCLUDE)}"xygext.h"\
	{$(INCLUDE)}"yuepai.h"\
	{$(INCLUDE)}"yuepaidef.h"\
	{$(INCLUDE)}"yuepaireq.h"\
	{$(INCLUDE)}"zconf.h"\
	{$(INCLUDE)}"zlib.h"\
	

!IF  "$(CFG)" == "RoomSvrZgSa - Win32 Debug"

# ADD CPP /YX"stdafx.h"

!ELSEIF  "$(CFG)" == "RoomSvrZgSa - Win32 Release"

# ADD CPP /YX"stdafx.h"

!ELSEIF  "$(CFG)" == "RoomSvrZgSa - Win32 DebugS"

# ADD CPP /YX"stdafx.h"

!ELSEIF  "$(CFG)" == "RoomSvrZgSa - Win32 ReleaseS"

# SUBTRACT CPP /YX /Yc

!ENDIF 

# End Source File
# End Group
# Begin Group "头"

# PROP Default_Filter "h;hpp;hxx;hm;inl;inc"
# Begin Source File

SOURCE=.\define.h
# End Source File
# Begin Source File

SOURCE=Main.h
# End Source File
# Begin Source File

SOURCE=.\MainServer.h
# End Source File
# Begin Source File

SOURCE=Resource.h
# End Source File
# Begin Source File

SOURCE=.\RoomData.h
# End Source File
# Begin Source File

SOURCE=.\RoomServer.h
# End Source File
# Begin Source File

SOURCE=.\Service.h
# End Source File
# Begin Source File

SOURCE=.\SockClient.h
# End Source File
# Begin Source File

SOURCE=.\SockServer.h
# End Source File
# Begin Source File

SOURCE=stdafx.h
# End Source File
# End Group
# Begin Group "资源"

# PROP Default_Filter "rc;ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=RoomSvrZgSa.rc
# End Source File
# End Group
# End Target
# End Project
