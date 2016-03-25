#include "roombasic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define LICENSE_FILE		_T("license.dat")
#define PRODUCT_NAME		_T("roomsvr")
#define	PRODUCT_VERSION		_T("6.00")

#define DEF_STAT_INTERVAL		60  // seconds

#define DEF_TIMER_INTERVAL		15  // minitues
#define DEF_PULSE_INTERVAL		5  // seconds
#define DEF_MATCH_INTERVAL		1  // seconds

#define DEF_KICKOFF_ELAPSE		(DEF_TIMER_INTERVAL * 12) // minitues
#define	DEF_KICKOFF_DEADTIME	30	// minutes
#define	DEF_KICKOFF_STIFFTIME	300	// seconds
#define	DEF_KICKOFF_STATIC		1 //剔除不活动玩家

#define DEF_KICKOFF_MODE1   1   //踢人模式 默认定时器模式
#define DEF_KICKOFF_MODE2   2   //每天凌晨6点

#define DEF_SERVERPULSE_INTERVAL    60 //seconds 定时检查服务端发过来的脉搏(服务端发送间隔默认是10秒)



CBaseServer::CBaseServer()
{
   	

	g_hExitServer = NULL;

	g_uiThrdSend = 0;
	g_uiThrdRecv = 0;
	g_uiThrdTimer = 0;
	g_uiThrdPulse = 0;
	g_uiThrdMessage=0;
	g_uiThrdConnect=0;
	g_uiThrdMatch=0;
	g_uiThrdStat = 0;
	g_uiThrdSvrPulse=0;
 
	ZeroMemory(&g_ServerPulseInfo,sizeof(g_ServerPulseInfo));
	int nDate=0;
	int nTime=0;
	UwlGetCurrentDateTime(nDate,nTime);
    g_ServerPulseInfo.nCurrentDate =nDate;
}

CBaseServer::~CBaseServer()
{
	SAFE_DELETE(g_pSockClient);
	SAFE_DELETE(g_pSockServer);
	SAFE_DELETE(g_pRoomServer);

}
CBaseSockServer* CBaseServer::CreateCSockServer() {return NULL;}
CBaseSockClient* CBaseServer::CreateCSockClient() {return NULL;}
CBaseRoomServer* CBaseServer::CreateCRoomServer() {return NULL;}

CString CBaseServer::GetProductName()
{
	return PRODUCT_NAME;
}
BOOL CBaseServer::CheckLicense()
{
 	TCHAR szFullName[MAX_PATH];
	GetModuleFileName(GetModuleHandle(NULL), szFullName, sizeof(szFullName));

	// get license file fullpath
	UwlSplitPath(szFullName, SPLIT_DRIVE_DIR, g_szLicFile);
	lstrcat(g_szLicFile, LICENSE_FILE);

	UwlTrace(_T("license file path: %s"), g_szLicFile);

	// initialize seeds for license
	UwlInitSeeds(GetUWLSeedLicense());

	// check license file
	int nError = 0;
	LICSETTINGS	 lic;
	if(!UwlCheckLicense(g_szLicFile, GetProductName(), PRODUCT_VERSION, nError, &lic)){
		CString strMsg;
		UwlGetLicErrMsg(nError, strMsg);
		UwlTrace(_T("license check error! code = %d, msg = %s."), nError, (LPCTSTR)strMsg);
		UwlLogFile(_T("license check error! code = %d, msg = %s."), nError, (LPCTSTR)strMsg);
		
		CString strErr;
		UwlGetErrMsg(ERR_SERVER_ABORTED, strErr);
		strErr += strMsg;
		UwlLogFile(_T("%s"), strErr);
		return FALSE;
	}
	CString strClientID;
	if(!UwlGetLicFeature(g_szLicFile, _T("clientid"), strClientID, nError)){
		CString strMsg;
		UwlGetLicErrMsg(nError, strMsg);
		UwlTrace(_T("license clientid error! code = %d, msg = %s."), nError, (LPCTSTR)strMsg);
		UwlLogFile(_T("license clientid error! code = %d, msg = %s."), nError, (LPCTSTR)strMsg);
		
		CString strErr;
		UwlGetErrMsg(ERR_SERVER_ABORTED, strErr);
		strErr += strMsg;
		UwlLogFile(_T("%s"), strErr);
		return FALSE;
	}
	g_nClientID = atoi(LPCTSTR(strClientID));
	if(0 == g_nClientID)
	{
		UwlTrace(_T("invalid client id!"));
		UwlLogFile(_T("invalid client id!"));
		return FALSE;
	}
	else
	{
		UwlTrace(_T("client id=%d!"),g_nClientID);
		UwlLogFile(_T("client id=%d!"),g_nClientID);
	}

	//////////////////机器编号
	g_nHostID=GetPrivateProfileInt(_T("HOST"),_T("ID"),0, GetYxpdsvrIniFile());

	if(0 == g_nHostID)
	{
		UwlTrace(_T("invalid host id!"));
		UwlLogFile(_T("invalid host id!"));
		return FALSE;
	}
	else
	{
		UwlTrace(_T("host id=%d!"),g_nHostID);
		UwlLogFile(_T("host id=%d!"),g_nHostID);
	}

	return TRUE;
}
BOOL CBaseServer::InitStart()
{ 	
	g_hExitServer = CreateEvent(NULL, TRUE, FALSE, NULL);

 	TCHAR szPath[MAX_PATH];
	memset(szPath,0,sizeof(szPath));
 	GetModuleFileName(GetModuleHandle(NULL), szPath, sizeof(szPath));

	CString sFullName;
	sFullName.Format(_T("%s"),szPath);

	UwlSplitPath(szPath, SPLIT_DRIVE_DIR, szPath);

	lstrcpy(g_szIPManFile,szPath);
	lstrcat(g_szIPManFile,_T("ipman.dat"));

	lstrcpy(g_szIniFile,szPath);
 	lstrcat(g_szIniFile, GetProductName());
	lstrcat(g_szIniFile, _T(".ini"));
  
	lstrcpy(g_szKickManFile,szPath);
	lstrcat(g_szKickManFile,_T("kickman.dat"));

 	WritePrivateProfileString(_T("listen"),_T("pid"),xyConvertIntToStr(GetCurrentProcessId()),g_szIniFile);
 	g_nRoomPort = GetPrivateProfileInt(_T("listen"),_T("port"),PORT_ROOM_SERVER,g_szIniFile);
	g_nSysFindCD = GetPrivateProfileInt (_T("PrivateRoom"),_T("SysFindCD"),10,g_szIniFile);
	g_nSysFindScale = GetPrivateProfileInt (_T("PrivateRoom"),_T("SysFindScale"),3,g_szIniFile);
	g_nModeMoneyDiff = GetPrivateProfileInt (_T("PrivateRoom"),_T("ModeMoneyDiff"),50,g_szIniFile);
	g_nModeBountDiff = GetPrivateProfileInt (_T("PrivateRoom"),_T("ModeBountDiff"),50,g_szIniFile);
	g_fModeWinsDiff = GetPrivateProfileInt (_T("PrivateRoom"),_T("ModeWinsDiff"),50,g_szIniFile);
	
 	WritePrivateProfileString(_T("SVRPATH"),xyConvertIntToStr(g_nRoomPort),sFullName,GetYxpdsvrIniFile());

	CString sBackup;
	sBackup.Format(_T("%sBackupINI\\"),szPath);
	::CreateDirectory((LPCTSTR)sBackup ,NULL);

	sBackup.Format(_T("%sBackupIPMan\\"),szPath);
	::CreateDirectory((LPCTSTR)sBackup ,NULL);


 	char   name[256];
	memset(name,0,sizeof(name));
  	if(   gethostname   (   name,   sizeof(name))   ==   0) 
	{      
		char   *ip;
		PHOSTENT   phe;                  
		if((phe   =   gethostbyname(name))   !=   NULL)   
		{    
			for(int i=0;phe->h_addr_list[i]!=0;++i)
			{
				struct in_addr addr;
				memcpy(&addr,phe->h_addr_list[i],sizeof(struct in_addr));
				ip=inet_ntoa(addr);
				DWORD dwIP=IP127001;
				UwlNameToAddr( ip,dwIP); 
				g_mapLocalIP.SetAt(dwIP,dwIP);
			}
		}               
	}                 
    g_mapLocalIP.SetAt(IP127001,IP127001);

	//受信指定的roommpsvr代理服务器ip地址
	TCHAR szKey[32];
	TCHAR szServerIP[MAX_SERVERIP_LEN];
	memset(szKey, 0, sizeof(szKey));
	memset(szServerIP, 0, sizeof(szServerIP));
	int nCount = GetPrivateProfileInt(_T("MPServer"),_T("count"),0,g_szIniFile);
	for (int i = 0; i<nCount; i++)
	{
		sprintf(szKey, _T("IP%d"), i);
		GetPrivateProfileString(_T("MPServer"),szKey,_T(""),szServerIP, sizeof(szServerIP),g_szIniFile);
		DWORD dwIP=IP127001;
		if (UwlNameToAddr( szServerIP,dwIP))
			g_mapLocalIP.SetAt(dwIP,dwIP);
	}

	return TRUE;
}
BOOL CBaseServer::ConnectCheckSvr()
{
	TCHAR szCheckIP[MAX_SERVERIP_LEN];
	memset(szCheckIP,0,sizeof(szCheckIP));

 	GetPrivateProfileString(_T("CheckServer"),_T("name"),	_T(""),	szCheckIP,sizeof(szCheckIP) ,g_szIniFile);
	if(!lstrcmpi(szCheckIP, _T(""))){
		CString strMsg;
		UwlGetErrMsg(ERR_SERVER_NOTFOUND, strMsg);

		CString strErr;
		UwlGetErrMsg(ERR_SERVER_ABORTED, strErr);
		strErr += strMsg;
		UwlLogFile(_T("%s"), strErr);
		return FALSE;
	}
	UwlTrace(_T("check server name: %s"), szCheckIP);
	
	// start socket client to checkserver
	// create send and recv thread for client
	g_hThrdSend = (HANDLE)_beginthreadex(NULL,			// Security
										0,						// Stack size - use default
										SendThreadFunc,     	// Thread fn entry point
										(void*) this,			// Param for thread
										0,						// Init flag
										&g_uiThrdSend);			// Thread address;


	g_hThrdConnect = (HANDLE)_beginthreadex(NULL,			// Security
										0,						// Stack size - use default
										ConnectThreadFunc,     	// Thread fn entry point
										(void*) this,			// Param for thread
										0,						// Init flag
										&g_uiThrdConnect);			// Thread address;
	if(FALSE == g_pSockClient->Create(szCheckIP, PORT_CHECK_SERVER, 30, 
									TRUE, GetHelloData(), GetHelloLength() + 1,
									1, 10)){  //5->1 
 		UwlTrace(_T("Can not connect to check server."));
		UwlLogFile(_T("Can not connect to check server."));

		CString strMsg;
		UwlGetErrMsg(ERR_CONNECT_FAILED, strMsg);

		CString strErr;
		UwlGetErrMsg(ERR_SERVER_ABORTED, strErr);
		strErr += strMsg;
		UwlLogFile(_T("%s"), strErr);
		return FALSE;
	}

	return TRUE;
}
BOOL CBaseServer::Initialize()
{  
	UwlLogFile(_T("server starting..."));
 	// initialize ole environment
	if(S_FALSE == ::CoInitialize(NULL)) {
		return FALSE;
	}
 	
	g_pSockServer = CreateCSockServer();
	g_pSockClient = CreateCSockClient();
	g_pRoomServer = CreateCRoomServer();

    if(!CheckLicense())
		return FALSE;
	if(!InitStart())
		return FALSE;

	if(!ConnectCheckSvr()) 
		return FALSE;
 
	if(!ValidateClientEx())
	{
 		UwlTrace(_T("Can not pass Client verify."));
		UwlLogFile(_T("Can not pass Client verify."));
		return FALSE;
	}
	ValidateClientInfo(g_nClientID);
 
	if(!GetFilterWord())	// get filterWord info
	{
 		UwlTrace(_T("Can not get filter word  from check server."));
		UwlLogFile(_T("Can not get filter  word from check server."));
		return FALSE;
	}

	if(!GetMutexHard())	// get mutexhard info
	{
 		UwlTrace(_T("Can not get mutex hard  from check server."));
		UwlLogFile(_T("Can not get mutex hard from check server."));
		return FALSE;
	}
	
	if(!GetChatForbid())// get chatforbid info
	{
 		UwlTrace(_T("Can not get chatforbid info  from check server."));
		UwlLogFile(_T("Can not get chatforbid info from check server."));
		return FALSE;
	} 
 
	CreateTimerThread();
	CreatePulseThread();
	CreateMessageThread();
//	CreateStatThread();
	CreateServerPulseThread();

	int nMatchEnable = GetPrivateProfileInt(_T("Match"),_T("Enable"), 0, g_szIniFile);
	if(nMatchEnable > 0)
 		CreateMatchThread();

	UwlTrace(_T("Server start up OK."));
	UwlLogFile(_T("Server start up OK."));

 	BOOL bRet= g_pSockServer->Initialize(_T(""), g_nRoomPort, GetHelloData(), GetHelloLength() + 1,600, 1,1000);
 	g_pSockServer->SetNotifyWindow(g_hMessageWnd);

	//注意一台机器上只能开一个侦听，后面开的将失败
	if(nMatchEnable)
		g_pRoomServer->Initialize(_T(""), PORT_RTG_SERVER, GetHelloData(), GetHelloLength() + 1,0, 10);

	return bRet;
}

void CBaseServer::Shutdown()
{
	SetEvent(g_hExitServer);

	PostThreadMessage(g_uiThrdSend, WM_QUIT, 0, 0);
	WaitForSingleObject(g_hThrdSend, WAITTIME_EXIT);
	if(g_hThrdSend){
		CloseHandle(g_hThrdSend);
		g_hThrdSend = NULL;
	}

	PostThreadMessage(g_uiThrdTimer, WM_QUIT, 0, 0);
	WaitForSingleObject(g_hThrdTimer, WAITTIME_EXIT);	
	if(g_hThrdTimer){
		CloseHandle(g_hThrdTimer);
		g_hThrdTimer = NULL;
	}

	PostThreadMessage(g_uiThrdPulse, WM_QUIT, 0, 0);
	WaitForSingleObject(g_hThrdPulse, WAITTIME_EXIT);	
	if(g_hThrdPulse){
		CloseHandle(g_hThrdPulse);
		g_hThrdPulse = NULL;
	}

	PostThreadMessage(g_uiThrdConnect, WM_QUIT, 0, 0);
	WaitForSingleObject(g_hThrdConnect, WAITTIME_EXIT);	
	if(g_hThrdConnect){
		CloseHandle(g_hThrdConnect);
		g_hThrdConnect = NULL;
	}
	
	PostThreadMessage(g_uiThrdMessage, WM_QUIT, 0, 0);
	WaitForSingleObject(g_hThrdMessage, WAITTIME_EXIT);	
	if(g_hThrdMessage){
		CloseHandle(g_hThrdMessage);
		g_hThrdMessage = NULL;
	}

	PostThreadMessage(g_uiThrdMatch, WM_QUIT, 0, 0);
	WaitForSingleObject(g_hThrdMatch, WAITTIME_EXIT);	
	if(g_hThrdMatch){
		CloseHandle(g_hThrdMatch);
		g_hThrdMatch = NULL;
	}

	PostThreadMessage(g_uiThrdStat, WM_QUIT, 0, 0);
	WaitForSingleObject(g_hThrdStat, WAITTIME_EXIT);	
	if(g_hThrdStat){
		CloseHandle(g_hThrdStat);
		g_hThrdStat = NULL;
	}

	PostThreadMessage(g_uiThrdSvrPulse, WM_QUIT, 0, 0);
	WaitForSingleObject(g_hThrdSvrPulse, WAITTIME_EXIT);	
	if(g_hThrdSvrPulse){
		CloseHandle(g_hThrdSvrPulse);
		g_hThrdSvrPulse = NULL;
	}

	g_pSockClient->Destroy();
	g_pSockServer->Shutdown();
	g_pRoomServer->Shutdown();

	if(g_hExitServer){
		CloseHandle(g_hExitServer);
		g_hExitServer = NULL;
	}
	ClearRoomDataMap();
		
	ClearPlayerPool();

	ClearCallAction();
	ClearChatAction();

	ClearPlayerKickList();
	ClearKickoffRecordMap();

	::CoUninitialize();

	UwlLogFile(_T("server exited."));
}

unsigned CBaseServer::SendThreadFunc(LPVOID lpVoid)
{
	CBaseServer * pThread = (CBaseServer *) lpVoid;

    return pThread->SendThreadProc();
} 

unsigned CBaseServer::SendThreadProc()
{
	UwlTrace(_T("send thread started. id = %d"), GetCurrentThreadId());
	
	MSG msg;
	while (GetMessage(&msg, 0, 0, 0)){
		if(UM_DATA_TOSEND == msg.message){
			if(0 == msg.wParam){
				LPREQUEST pRequest = LPREQUEST(msg.lParam);
		
				CONTEXT_HEAD context;
				memset(&context, 0, sizeof(context));
						
				BOOL bTimeout = FALSE;
				BOOL bSendOK = g_pSockClient->SendRequest(&context, pRequest, NULL, bTimeout);

				UwlClearRequest(pRequest);
				SAFE_DELETE(pRequest);
			}else{
				LPCONTEXT_HEAD pContext = LPCONTEXT_HEAD(msg.wParam);
				LPREQUEST pRequest = LPREQUEST(msg.lParam);
				
				int nLen = sizeof(CONTEXT_HEAD) + pRequest->nDataLen;
				PBYTE pData = new BYTE[nLen];
				memcpy(pData, pContext, sizeof(CONTEXT_HEAD));
				memcpy(pData + sizeof(CONTEXT_HEAD), pRequest->pDataPtr, pRequest->nDataLen);
				
				REQUEST request;
				memset(&request, 0, sizeof(request));
				request.nDataLen = nLen;
				request.pDataPtr = pData;
				request.head.nRequest = pRequest->head.nRequest;
				request.head.nRepeated = 1;
				if(request.head.nRequest==GR_ENTER_MATCH_TOCHECK)
					request.head.nRepeated=2;
				request.head.nSubReq=pRequest->head.nSubReq;//注意这里存放子请求号

				UwlClearRequest(pRequest);
				SAFE_DELETE(pContext);
				SAFE_DELETE(pRequest);

				CONTEXT_HEAD context;
				memset(&context, 0, sizeof(context));
					
				BOOL bTimeout = FALSE;
				BOOL bSendOK = g_pSockClient->SendRequest(&context, &request, NULL, bTimeout);

				UwlClearRequest(&request);
			}
		}else{
			DispatchMessage(&msg);
		}
	}
	UwlTrace(_T("send thread exiting. id = %d"), GetCurrentThreadId());
	UwlLogFile(_T("send thread exiting. id = %d"), GetCurrentThreadId());

   	return 0;
} 

unsigned CBaseServer::TimerThreadFunc(LPVOID lpVoid)
{
	CBaseServer * pThread = (CBaseServer *) lpVoid;

    return pThread->TimerThreadProc();
} 
unsigned CBaseServer::PulseThreadFunc(LPVOID lpVoid)
{
	CBaseServer * pThread = (CBaseServer *) lpVoid;

    return pThread->PulseThreadProc();
} 
unsigned CBaseServer::TimerThreadProc()
{
	UwlTrace(_T("timer thread started. id = %d"), GetCurrentThreadId());

	int sameHour = 0;
	int lastHour = -1;
	int cycleMin = 0;

	int nLastestClear = UwlGetCurrentSeconds();
	
	// 启动时就读取
	g_nOverSeeUserId = GetPrivateProfileInt(
		_T("kickoff"),			// section name
		_T("OverSeeUserId"),	// key name
		0,		// default int
		g_szIniFile				// initialization file name
												);

	while(TRUE){
		int interval = GetPrivateProfileInt(_T("Timer"), _T("Interval"), DEF_TIMER_INTERVAL, g_szIniFile);
		int nKickoffResetHour = GetPrivateProfileInt(_T("VipKickoff"), _T("ResetHour"), 0, g_szIniFile);
	 
		DWORD dwRet = WaitForSingleObject(g_hExitServer, interval * 60 * 1000);
		if(WAIT_OBJECT_0 == dwRet){ // exit event
			break;
		}
		if(WAIT_TIMEOUT == dwRet){ // timeout
			UwlTrace(_T("timer thread triggered. do something. interval = %ld minutes."), interval);
//			UwlLogFile(_T("timer thread triggered. do something. interval = %ld minutes."), interval);
			
			{
				// 读取监视玩家的id
				g_nOverSeeUserId = GetPrivateProfileInt(
												_T("kickoff"),			// section name
												_T("OverSeeUserId"),	// key name
												0,		// default int
												g_szIniFile				// initialization file name
												);
				g_bOpenOverSeeUser = GetPrivateProfileInt(
												_T("kickoff"),			// section name
												_T("OpenOverSeeUser"),	// key name
												0,		// default int
												g_szIniFile				// initialization file name
												);

				int timingOK = 0;
				int mode = GetPrivateProfileInt(
												_T("KickOff"),			// section name
												_T("Mode"),				// key name
												DEF_KICKOFF_MODE2,		// default int
												g_szIniFile				// initialization file name
												);
				int	timing = GetPrivateProfileInt(
												_T("KickOff"),			// section name
												_T("Timing"),			// key name
												6,						// default int 早上六点钟
												g_szIniFile				// initialization file name
												);
				int	elapse = GetPrivateProfileInt(
												_T("KickOff"),			// section name
												_T("Elapse"),			// key name
												DEF_KICKOFF_ELAPSE,		// default int
												g_szIniFile				// initialization file name
												);
				

 				SYSTEMTIME time;
				GetLocalTime(&time);
				if(time.wHour != lastHour)
 					sameHour = 0;
 				else
 					sameHour++;
 				lastHour = time.wHour;
				if(time.wHour == timing)
				{
					if(0 == sameHour)
 						timingOK = 1;
				}
				if(timingOK || mode==DEF_KICKOFF_MODE1)
				{
 						int count = g_pSockServer->ReleaseSockBufPool();
						UwlLogFile(_T("release server buffer pool done. count = %ld."), count);
 			 
					    count = g_pSockClient->ReleaseSockBufPool();
						UwlLogFile(_T("release client buffer pool done. count = %ld."), count);
				 
						int deadtime = GetPrivateProfileInt(_T("KickOff"),_T("DeadTime"),DEF_KICKOFF_DEADTIME, 	g_szIniFile	);
						int nStatic = GetPrivateProfileInt(_T("KickOff"),_T("static"),DEF_KICKOFF_STATIC, 	g_szIniFile	);
						KickOffDeadAtMorning(deadtime, nStatic);
						UwlLogFile(_T("KickOffDeadAtMorning() called. deadtime = %ld mins, static = %ld."), deadtime, nStatic);
						// 

					 	CDWordArray aryToken;
						int stales = g_pSockServer->CleanStales(360 * 60, aryToken);//6小时
						UwlLogFile(_T("CleanStales() called. deadtime = %ld mins, count = %ld."), 360, stales);
	 

						int stifftime = DEF_KICKOFF_STIFFTIME ;
						int stiffs = g_pSockServer->CloseStiffs(stifftime);
						UwlLogFile(_T("CloseStiffs() called. stifftime = %ld secs, count = %ld."), stifftime, stiffs);

					    count=ClearChatAction();
						UwlLogFile(_T("clear chat action done. count = %ld."), count);
				
					    count=ClearCallAction();
						UwlLogFile(_T("clear call action done. count = %ld."), count);

						count=ClearTablePwdAction();
						UwlLogFile(_T("clear tablepwd action done. count = %ld."), count);
 				
						count=ClearAddFriendAction();
						UwlLogFile(_T("clear addfriend action done. count = %ld."), count);
					
						count=ClearInviteAction();
						UwlLogFile(_T("clear invite action done. count = %ld."), count);

						////////////////////////////////////////////////////////////////


				}
		
				//每天定时清理个人踢人统计，让第二天还能踢
				int timingKickoffReset = 0;
				if(time.wHour == nKickoffResetHour)
				{
					if(0 == sameHour)
						timingKickoffReset = 1;
				}
				if (timingKickoffReset)
				{
					ClearKickoffRecordMap();
				}

				//隔段时间，清理一次超时的踢人明细
				int nNowSeconds = UwlGetCurrentSeconds();
				int nMaxIsolate = GetPrivateProfileInt(_T("VipKickoff"), _T("MaxIsolate"), 3*3600, g_szIniFile);
				if (nNowSeconds - nLastestClear >= nMaxIsolate)
				{
					ClearOverdueKickList(nNowSeconds, nMaxIsolate);
					nLastestClear = nNowSeconds;
				}
			}

			LONG forbid = 0;
			InterlockedExchange(&forbid, g_lClientForbid);
			if(forbid){
				g_pSockClient->Destroy();				
				InterlockedExchange(&g_lClientForbid, 0);
			}
		}
	}
	UwlTrace(_T("timer thread exiting. id = %d"), GetCurrentThreadId());
	UwlLogFile(_T("timer thread exiting. id = %d"), GetCurrentThreadId());

   	return 0;
} 


unsigned CBaseServer::PulseThreadProc()
{
	UwlTrace(_T("pulse thread started. id = %d"), GetCurrentThreadId());
	
	int interval = GetPrivateProfileInt(
					_T("Pulse"),			// section name
					_T("Interval"),			// key name
					DEF_PULSE_INTERVAL,		// default int
					g_szIniFile				// initialization file name
					);

	while(TRUE)
	{
		DWORD dwRet = WaitForSingleObject(g_hExitServer, interval  * 1000);
		if(WAIT_OBJECT_0 == dwRet)// exit event
		{ 
			break;
		}
		if(WAIT_TIMEOUT == dwRet)// timeout
		{ 
  			SendPulseData();
			SendRoomSvrPulse();
		}
	 
	}
	UwlTrace(_T("pulse thread exiting. id = %d"), GetCurrentThreadId());
	UwlLogFile(_T("pulse thread exiting. id = %d"), GetCurrentThreadId());

   	return 0;
} 


unsigned CBaseServer::ConnectThreadFunc(LPVOID lpVoid)
{
	CBaseServer * pThread = (CBaseServer *) lpVoid;

    return pThread->ConnectThreadProc();
} 

unsigned CBaseServer::ConnectThreadProc()
{
	UwlTrace(_T("connect thread started. id = %d"), GetCurrentThreadId());
	
	MSG msg;
	while (GetMessage(&msg, 0, 0, 0)){
		if(UM_DATA_TOSEND == msg.message){
			if(UR_SOCKET_CLOSE == msg.wParam){
				while(TRUE){
		 			g_pSockClient->CloseSockets();	//注意前提是g_pSockClient->Create()时确定1个连接
 					GetPrivateProfileString(_T("CheckServer"),_T("Name"),_T(""),g_szCheckServer,sizeof(g_szCheckServer) ,g_szIniFile);
					if(FALSE == g_pSockClient->BeginConnect(g_szCheckServer, PORT_CHECK_SERVER, 1)){
						UwlTrace(_T("Can not connect to checkserver."));
						UwlLogFile(_T("Can not connect to checkserver."));
						if(!g_hExitServer || 
							WAIT_OBJECT_0 == WaitForSingleObject(g_hExitServer, 0)){
							break;
						}
					}else{
						UwlTrace(_T("reconnect to checkserver OK."));
						UwlLogFile(_T("reconnect to checkserver OK."));
						// validate client id
						ValidateClientInfo(g_nClientID);
						break;
					}
				}
			}
			else if(UR_RECONNECT_SVR == msg.wParam)
			{	
				int nNowTick=GetTickCount();
				UwlLogFile(_T("[%u] reconnect msg arrived,msg send time [%u],interval=%d "),nNowTick,msg.lParam ,nNowTick - msg.lParam);
				if( abs(nNowTick - msg.lParam) < 200 ) //当前发送的重连消息才处理
				{
					g_pSockClient->CloseSockets();	
					GetPrivateProfileString(_T("CheckServer"),_T("Name"),_T(""),g_szCheckServer,sizeof(g_szCheckServer) ,g_szIniFile);	
					if(FALSE == g_pSockClient->BeginConnect(g_szCheckServer, PORT_CHECK_SERVER, 1)){  //5->1 
						UwlTrace(_T("Can not reconnect to checkserver as received cmd."));
						UwlLogFile(_T("Can not reconnect to checkserver as received cmd."));
					}else{
						UwlTrace(_T("reconnect to checkserver OK as received cmd."));
						UwlLogFile(_T("reconnect to checkserver OK as received cmd."));
						// validate client id
						ValidateClientInfo(g_nClientID);
					}
				}
			}				
		}else{
			DispatchMessage(&msg);
		}
	}
	UwlTrace(_T("connect thread exiting. id = %d"), GetCurrentThreadId());
	UwlLogFile(_T("connect thread exiting. id = %d"), GetCurrentThreadId());

   	return 0;
} 


unsigned CBaseServer::MatchThreadProc()
{
	UwlTrace(_T("match thread started. id = %d"), GetCurrentThreadId());
	
	int interval = GetPrivateProfileInt(
					_T("Match"),			// section name
					_T("Interval"),			// key name
					DEF_MATCH_INTERVAL,		// default int
					g_szIniFile				// initialization file name
					);

	while(TRUE)
	{
		DWORD dwRet = WaitForSingleObject(g_hExitServer, interval  * 1000);
		if(WAIT_OBJECT_0 == dwRet)// exit event
		{ 
			break;
		}
		if(WAIT_TIMEOUT == dwRet)// timeout
		{ 
   			OnMatchTimer();
		}
	 
	}
	UwlTrace(_T("match thread exiting. id = %d"), GetCurrentThreadId());
	UwlLogFile(_T("match thread exiting. id = %d"), GetCurrentThreadId());

   	return 0;
} 


static LRESULT CALLBACK MessageWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
							{return DefWindowProc(hwnd, uMsg, wParam, lParam);}

BOOL RegMessageWnd(int nPort)
{
	TCHAR szPort[MAX_PATH];
	memset(szPort, 0, sizeof(szPort));
	TCHAR szClsName[MAX_PATH];
	memset(szClsName, 0, sizeof(szClsName));

	_stprintf(szPort, _T("%ld"), nPort);

	lstrcat(szClsName, ROOMSVR_CLSNAME);
	lstrcat(szClsName, szPort);

	WNDCLASS clsSockWnd;

	ZeroMemory(&clsSockWnd, sizeof(clsSockWnd));

	clsSockWnd.hCursor = LoadCursor(NULL,IDC_ARROW);
	clsSockWnd.hIcon = NULL;
	clsSockWnd.lpszMenuName=NULL; // Don't want any menu
	clsSockWnd.lpszClassName = szClsName;
	clsSockWnd.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	clsSockWnd.hInstance = GetModuleHandle(NULL);
	clsSockWnd.style = 0;
	clsSockWnd.lpfnWndProc = (WNDPROC)MessageWndProc;
	clsSockWnd.cbWndExtra = 0;
	clsSockWnd.cbClsExtra = 0;

	if (!RegisterClass(&clsSockWnd)){
		UwlTrace(_T("RegisterClass() error. errcode: %ld, errstr: %s.\n"), 
					GetLastError(), UwlGetAPIErrStr(GetLastError()));
		return FALSE;
	}
	return TRUE;
}

void UnregMessageWnd(int nPort)
{
	TCHAR szPort[MAX_PATH];
	memset(szPort, 0, sizeof(szPort));
	TCHAR szClsName[MAX_PATH];
	memset(szClsName, 0, sizeof(szClsName));

	_stprintf(szPort, _T("%ld"), nPort);

	lstrcat(szClsName, ROOMSVR_CLSNAME);
	lstrcat(szClsName, szPort);

	UnregisterClass(szClsName, GetModuleHandle(NULL));
}

HWND CreateMessageWnd(int nPort)
{
	TCHAR szPort[MAX_PATH];
	memset(szPort, 0, sizeof(szPort));
	TCHAR szClsName[MAX_PATH];
	memset(szClsName, 0, sizeof(szClsName));
	TCHAR szWndName[MAX_PATH];
	memset(szWndName, 0, sizeof(szWndName));

	_stprintf(szPort, _T("%ld"), nPort);

	lstrcat(szClsName, ROOMSVR_CLSNAME);
	lstrcat(szClsName, szPort);
	lstrcat(szWndName, ROOMSVR_WNDNAME);
	lstrcat(szWndName, szPort);

	HWND hWnd = CreateWindowEx(0,
			    szClsName,	// Class name
			    szWndName,		// Caption
				WS_POPUP,		// Style bits
				0, 0, 0, 0,
			    (HWND)NULL,         // Parent window (no parent)
			    (HMENU)NULL,        // use class menu
			    GetModuleHandle(NULL),		// handle to window instance
			    (LPSTR)NULL         // no params to pass on
			   );
	if(!hWnd){
		UwlTrace(_T("CreateWindowEx() error. errcode: %ld, errstr: %s"), 
					GetLastError(), UwlGetAPIErrStr(GetLastError()));
		return NULL;
	}
	return hWnd;
}

unsigned CBaseServer::MatchThreadFunc(LPVOID lpVoid)
{
	CBaseServer * pThread = (CBaseServer *) lpVoid;

    return pThread->MatchThreadProc();
} 


unsigned CBaseServer::MessageThreadProc()
{
	UwlTrace(_T("message thread started. id = %d"), GetCurrentThreadId());
  	RegMessageWnd(g_nRoomPort);
    g_hMessageWnd = CreateMessageWnd(g_nRoomPort);

 	WritePrivateProfileString(_T("listen"),	 _T("hwnd"), xyConvertIntToStr((int)g_hMessageWnd),g_szIniFile	);
  	
	if(!g_hMessageWnd){
		UwlLogFile(_T("can not create message windows. we exit."));
		return 0;
	}

	MSG msg;
	while (GetMessage(&msg, 0, 0, 0)){
		if(g_hMessageWnd != msg.hwnd) continue; //
		if(UM_SOCKET_NOBUFFER == msg.message)
		{
			InterlockedIncrement(&g_lNoBufferCount);
			InterlockedExchange(&g_lNoBufferLatest, UwlGetCurrentSeconds());
		}
		else if(WM_GTR_ENTERGAMEOK == msg.message)
		{
			OnUserEnterGameOK(msg.wParam ,msg.lParam);
		}
		else if(WM_GTR_ENTERGAMEOK_VERIFIED == msg.message)
		{
			OnUserEnterGameOKVerified(msg.wParam ,msg.lParam);
		}
		else if(WM_GTR_LEAVEGAMEOK == msg.message)
		{
			OnUserLeaveGameOK(msg.wParam ,msg.lParam);
		}
		else if(WM_GTR_LEAVEGAMEOK_VERIFIED == msg.message)
		{
			OnUserLeaveGameOKVerified(msg.wParam ,msg.lParam);
		}
		else if(WM_GTR_VERIFYSTART == msg.message)
		{
			OnUserVerifyStart(msg.wParam ,msg.lParam);
		}
		else if(WM_GTR_GAMESTARTUP == msg.message)
		{
			OnGameStartUp(msg.wParam ,msg.lParam);
		}
		else if(WM_GTR_GAMEBOUTEND == msg.message)
		{
			OnGameBoutEnd(msg.wParam ,msg.lParam);
		}
		else if(WM_GTR_CLOSESOLOTABLE == msg.message)
		{
			OnCloseSoloTable(msg.wParam ,msg.lParam);
		}
	 	else if(WM_GTR_ASKNEWTABLE == msg.message)
		{
			OnAskNewTable(msg.wParam ,msg.lParam);
		}
		else if(WM_GTR_USERBOUTEND == msg.message)
		{
			OnUserBoutEnd(msg.wParam ,msg.lParam);
		}
		else if(WM_GTR_FINDVPN == msg.message)
		{
 			OnFindVPN(msg.wParam ,msg.lParam);
		}
		else if(WM_GTR_REMOVEVPN == msg.message)
		{
			OnRemoveVPN(msg.wParam ,msg.lParam);
		}
		else if(WM_GTR_CHANGETABLE == msg.message)
		{
			OnChangeTable(msg.wParam ,msg.lParam);
		}
		else if(WM_GTR_CHANGECHAIR == msg.message)
		{
			OnChangeChair(msg.wParam ,msg.lParam);
		}
		else if (WM_GTR_HOMEUSERCHANGED==msg.message)
		{
			OnHomeUserChanged(msg.wParam ,msg.lParam);
		}
		else if(WM_GTR_GAMESVRPULSE == msg.message)
		{
			OnGameSvrPulse(msg.wParam ,(HWND)msg.lParam);
		}
		else if(WM_GTR_USERMATCHSCORE == msg.message)
		{
			OnUserMatchScore(msg.wParam ,msg.lParam);
		}
		else if(WM_GTR_WELFAREDEPOSIT == msg.message)
		{
			OnUserWelfareDeposit(msg.wParam ,msg.lParam);
		}
		else if(WM_GTR_ADD_SOCRE == msg.message)
		{
			OnUserAddScore(msg.wParam ,msg.lParam);
		}
		else if(WM_GTR_MINUS_SOCRE == msg.message)
		{
			OnUserMinusScore(msg.wParam ,msg.lParam);
		}
		else if(WM_GTR_CHANGE_SPECIFYTABLE == msg.message)
		{
			OnChangeSpecifyTable(msg.wParam ,msg.lParam);
		}
		else if(WM_GTR_PLAYING_CHANGECHAIR == msg.message)
		{
			OnPlayingChangeChair(msg.wParam,msg.lParam);
		}
		else if(WM_GTR_ADD_DEPOSIT == msg.message)
		{
			OnUserAddDeposit(msg.wParam ,msg.lParam);
		}
		else if(WM_GTR_MINUS_DEPOSIT == msg.message)
		{
			OnUserMinusDeposit(msg.wParam ,msg.lParam);
		}
		else if(WM_GTR_PLAYER_PLAYING == msg.message)
		{
			OnUserStatusPlaying(msg.wParam ,msg.lParam);
		}
		else
		{
			if(!OnCustomWndMsg(msg.message,msg.wParam ,msg.lParam))
			{
				DispatchMessage(&msg);
			}
		}
	}
	if(g_hMessageWnd){
		DestroyWindow(g_hMessageWnd);
		g_hMessageWnd = NULL;
	}
	UnregMessageWnd(g_nRoomPort);

	UwlTrace(_T("message thread exiting. id = %d"), GetCurrentThreadId());
	UwlLogFile(_T("message thread exiting. id = %d"), GetCurrentThreadId());
   	
	return 0;
} 

unsigned CBaseServer::MessageThreadFunc(LPVOID lpVoid)
{
	CBaseServer * pThread = (CBaseServer *) lpVoid;

    return pThread->MessageThreadProc();
} 



BOOL CBaseServer::CreatePulseThread()
{
	g_hThrdPulse = (HANDLE)_beginthreadex(NULL,			// Security
										0,						// Stack size - use default
										PulseThreadFunc,     	// Thread fn entry point
										(void*) this,			// Param for thread
										0,						// Init flag
										&g_uiThrdPulse);	// Thread address;

	return TRUE;
}
BOOL CBaseServer::CreateTimerThread()
{
	g_hThrdTimer = (HANDLE)_beginthreadex(NULL,			// Security
										0,						// Stack size - use default
										TimerThreadFunc,     	// Thread fn entry point
										(void*) this,			// Param for thread
										0,						// Init flag
										&g_uiThrdTimer);			// Thread address;
		return TRUE;
}

BOOL    CBaseServer::CreateMessageThread()
{
	g_hThrdMessage = (HANDLE)_beginthreadex(NULL,			// Security
										0,						// Stack size - use default
										MessageThreadFunc,     	// Thread fn entry point
										(void*) this,			// Param for thread
										0,						// Init flag
										&g_uiThrdMessage);			// Thread address;
	return TRUE;
}

BOOL CBaseServer::CreateMatchThread()
{
	g_hThrdMatch = (HANDLE)_beginthreadex(NULL,			// Security
										0,						// Stack size - use default
										MatchThreadFunc,     	// Thread fn entry point
										(void*) this,			// Param for thread
										0,						// Init flag
										&g_uiThrdMatch);	// Thread address;

	return TRUE;
}



BOOL CBaseServer::CreateStatThread()
{
	g_hThrdStat = (HANDLE)_beginthreadex(NULL,			// Security
										0,						// Stack size - use default
										StatThreadFunc,     	// Thread fn entry point
										(void*) this,			// Param for thread
										0,						// Init flag
										&g_uiThrdStat);			// Thread address;
		return TRUE;
}
unsigned CBaseServer::StatThreadFunc(LPVOID lpVoid)
{
	CBaseServer * pThread = (CBaseServer *) lpVoid;

    return pThread->StatThreadProc();
} 



unsigned CBaseServer::StatThreadProc()
{
	UwlTrace(_T("stat thread started. id = %d"), GetCurrentThreadId());

	int sameHour = 0;
	int lastHour = -1;
	int cycleMin = 0;
	
	while(TRUE){
 	 	int interval = GetPrivateProfileInt(_T("Stat"), _T("Interval"),DEF_STAT_INTERVAL, g_szIniFile );
		if(interval < 1){ // 至少1秒周期
			interval = 1;
		}
 		DWORD dwRet = WaitForSingleObject(g_hExitServer, interval * 1000);
 		if(WAIT_OBJECT_0 == dwRet){ // exit event
			break;
		}
		if(WAIT_TIMEOUT == dwRet){ // timeout
			UwlTrace(_T("stat thread triggered. do something. interval = %ld minutes."), interval);
//			UwlLogFile(_T("timer thread triggered. do something. interval = %ld minutes."), interval);
		 
 

		}
	}
	UwlTrace(_T("stat thread exiting. id = %d"), GetCurrentThreadId());
	UwlLogFile(_T("stat thread exiting. id = %d"), GetCurrentThreadId());

   	return 0;
} 


BOOL CBaseServer::CreateServerPulseThread()
{
 
	g_hThrdSvrPulse = (HANDLE)_beginthreadex(NULL,				// Security
										0,					// Stack size - use default
										ServerPulseThreadFunc,    // Thread fn entry point
										(void*) this,		// Param for thread
										0,					// Init flag
										&g_uiThrdSvrPulse);	// Thread address;
	return TRUE;
}

unsigned CBaseServer::ServerPulseThreadFunc(LPVOID lpVoid)
{
	CBaseServer * pThread = (CBaseServer *) lpVoid;

    return pThread->ServerPulseThreadProc();
} 



unsigned CBaseServer::ServerPulseThreadProc()
{
 	UwlTrace(_T("serverpulse thread started. id = %d"), GetCurrentThreadId());
	//重连失败大概耗时31秒左右，这样定时间隔60秒，即可实现重连失败之后定时重连
	while(TRUE)
	{
		int interval = GetPrivateProfileInt(_T("ServerPulse"),_T("Interval"),DEF_SERVERPULSE_INTERVAL,g_szIniFile);
		DWORD dwRet = WaitForSingleObject(g_hExitServer, interval  * 1000);
		if(WAIT_OBJECT_0 == dwRet)// exit event
		{ 
			break;
		}
		if(WAIT_TIMEOUT == dwRet)// timeout
		{ 	
			int nCheck = GetPrivateProfileInt(_T("ServerPulse"),_T("Check"),1,g_szIniFile);//默认=1表示要检查
			if(nCheck==0)
				continue;
		
			CAutoLock lock(&g_csServerPulseInfo);
			int nLatest=g_ServerPulseInfo.nLatestTime;

			if(nLatest!=0)
			{
				int nNow=UwlGetCurrentSeconds();
				if(nNow-nLatest > interval )
				{
					int nDate,nTime;
					UwlGetCurrentDateTime(nDate,nTime);

				    if(g_ServerPulseInfo.nCurrentDate == nDate )
					{
						g_ServerPulseInfo.nReconnectCount ++;
					}
					else
					{
						g_ServerPulseInfo.nCurrentDate=nDate;
						g_ServerPulseInfo.nReconnectCount=0;
					}
					
					int nNowTick=GetTickCount();
					UwlLogFile(_T("[%u] Begin reconnect as server pulse lost [%d] 秒."),nNowTick,nNow-nLatest);
  
			    	PostThreadMessage(g_uiThrdConnect, UM_DATA_TOSEND, UR_RECONNECT_SVR, nNowTick);
				
				}
			}
		}
	 
	}
	
	UwlTrace(_T("serverpulse thread exiting. id = %d"), GetCurrentThreadId());
	UwlLogFile(_T("serverpulse thread exiting. id = %d"), GetCurrentThreadId());

   	return 0;
} 

void CBaseServer::OnMatchTimer()
{
	int roomid = 0;
	CBaseRoomData* lpRoomData = NULL;
	CRoomDataExMap mapRoomDataEx;
	{
		CAutoLock lock(&g_csRoomData);	
		POSITION pos = g_mapRoomDataEx.GetStartPosition();
		while (pos)	{
			g_mapRoomDataEx.GetNextAssoc(pos, roomid, lpRoomData);
			mapRoomDataEx.SetAt(roomid, lpRoomData);
		}
	}
	
	POSITION pos = mapRoomDataEx.GetStartPosition();
	while (pos)	{
		mapRoomDataEx.GetNextAssoc(pos, roomid, lpRoomData);
		if(lpRoomData)
		{
			CAutoLock lock(&(lpRoomData->m_csRoomData));
			if(!lpRoomData->IsMatchRoom())
				continue;
			lpRoomData->AT_OnMatchTrigger();
		}
	}
}

BOOL CBaseServer::OnUserVerifyStart(DWORD dwRoomTableChair,int nUserID)
{
	int nRoomID =  0;
	int nTableNO = 0;
	int nChairNO = 0;
	
	ParseRoomTableChair(dwRoomTableChair,nRoomID,nTableNO,nChairNO);
	
	UINT nMsgID=WM_RTG_STARTINVALID;
	
	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);	
		g_mapRoomDataEx.Lookup(nRoomID, lpRoomData);
	}
	
	if(lpRoomData)
	{	 
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		
		if(nChairNO<0 || nChairNO >=lpRoomData->GetChairCount())
			goto VERIFYRESULT;
		
		LPTABLE pTable = NULL;
		if(!lpRoomData->m_mapTable.Lookup(nTableNO, pTable)) 
			goto VERIFYRESULT;
		
		if(pTable->nPlayerAry[nChairNO]!=nUserID)
			goto VERIFYRESULT;
		
		LPPLAYER pPlayer = NULL;
		if(!lpRoomData->m_mapPlayer.Lookup( nUserID, pPlayer)) 
			goto VERIFYRESULT;
		
		pPlayer->pe.dwLatestTime = GetTickCount();
		
		if(!lpRoomData->IsSoloRoom())
		{
			if(pTable->th.nStatus != TABLE_STATUS_PLAYING)
				goto VERIFYRESULT;
		}
		
		nMsgID=WM_RTG_STARTVALID;
		
	}
	
	
VERIFYRESULT:
	if(lpRoomData)
	{
		HWND hWnd=lpRoomData->FindGameSvrWindow();
		if(IsWindow(hWnd))
			PostMessage(hWnd,nMsgID,dwRoomTableChair, nUserID);
	}
	return TRUE;
	
}

BOOL CBaseServer::OnGameStartUp(int nRoomID,DWORD dwTableChairStatus)
{  
	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);	
     	g_mapRoomDataEx.Lookup(nRoomID, lpRoomData);
	}

	if(lpRoomData)
	{	 
		CAutoLock lock(&(lpRoomData->m_csRoomData));
 		
		int nTableNO=dwTableChairStatus;
		int nChairCount=lpRoomData->GetChairCount();
		int nChairStatus[MAX_CHAIR_COUNT];
		memset(nChairStatus,0,sizeof(nChairStatus));
	
		if(lpRoomData->IsVariableChair())
		{
			ParseTableChairStatus(dwTableChairStatus,nTableNO,nChairStatus,nChairCount);//得到实际的nTableNO
		}
		else
		{
			for(int i=0;i<nChairCount;i++)
			{
				nChairStatus[i]=1;
			}
		}

		LPTABLE pTable = NULL;
		if(!lpRoomData->m_mapTable.Lookup(nTableNO, pTable))
			return FALSE;

 		pTable->hGameSvrWnd =lpRoomData->FindGameSvrWindow();//保存当前游戏服务端窗口句柄

		if(pTable->th.nStatus == TABLE_STATUS_PLAYING) //已经是玩的状态
		{
			for(int i = 0; i < pTable->th.nPlayerCount; i++)
			{
				LPPLAYER lpSeated = NULL;
				if(lpRoomData->m_mapPlayer.Lookup(pTable->nPlayerAry[i], lpSeated))
				{
  					lpSeated->pe.bRealPlaying =TRUE;	
				}
			}
            
		}
		else//
		{
			for(int i = 0; i < nChairCount; i++)
			{
				LPPLAYER lpSeated = NULL;
				if(nChairStatus[i]==1 )//这个位置上有人，并且参与了游戏
				{
					if(lpRoomData->m_mapPlayer.Lookup(pTable->nPlayerAry[i], lpSeated))
					{
 						lpSeated->nStatus = PLAYER_STATUS_PLAYING;
						lpSeated->pe.bRealPlaying =TRUE;
					}
				}
			}
			pTable->th.nStatus = TABLE_STATUS_PLAYING;
			pTable->nStartupTime = UwlGetCurrentSeconds();

			NTF_GAMESTARTUP  ng;
			memset(&ng, 0, sizeof(ng));
			ng.nRoomID =nRoomID;
 			ng.nTableNO=nTableNO;
			ng.dwTableChairStatus = (lpRoomData->IsVariableChair() ? dwTableChairStatus:0 );//非可变椅子房间,dwTableChairStatus必须=0
  			memcpy(ng.nPlayerAry, pTable->nPlayerAry, sizeof(ng.nPlayerAry));
  		
			if(lpRoomData->IsCloakingRoom())
			{
				for(int i = 0; i < nChairCount; i++)
				{
 					if(nChairStatus[i]==1 )
					{
						NotifyOnePlayer(lpRoomData,pTable->nPlayerAry[i], GR_PLAYER_GAMESTARTUP, &ng, sizeof(ng));
					}
				}
				NotifyDarkLooker(lpRoomData,0, GR_PLAYER_GAMESTARTUP, &ng, sizeof(ng));
			}
			else
			{
				NotifyRoomPlayers(lpRoomData, GR_PLAYER_GAMESTARTUP, &ng, sizeof(ng));
			}	

			OverSeeUserData(-1,"OnGameStartUp::CommitTPS_VARI");
			lpRoomData->CommitTPS_VARI(pTable,PS_PLAYING,nChairStatus);
		}
	}
 
	return TRUE;
}


BOOL CBaseServer::OnGameBoutEnd(int nRoomID,DWORD dwTableChairStatus)
{
 	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);	
     	g_mapRoomDataEx.Lookup(nRoomID, lpRoomData);
	}
   
	if(lpRoomData)
	{	 
 		CAutoLock lock(&(lpRoomData->m_csRoomData));
 
		int nTableNO=dwTableChairStatus;
		int nChairCount=lpRoomData->GetChairCount();
		int nChairStatus[MAX_CHAIR_COUNT];
		memset(nChairStatus,0,sizeof(nChairStatus));
	
		if(lpRoomData->IsVariableChair())
		{
			ParseTableChairStatus(dwTableChairStatus,nTableNO,nChairStatus,nChairCount);//得到实际的nTableNO
		}
		else
		{
			for(int i=0;i<nChairCount;i++)
			{
				nChairStatus[i]=1;
			}
		}

		LPTABLE pTable = NULL;
		if(!lpRoomData->m_mapTable.Lookup(nTableNO, pTable))
			return FALSE;
	
		//某些类型的房间，本桌一局结束后，继续保持正在玩的状态
		BOOL bSetStatus=FALSE;
		if(lpRoomData->IsSoloRoom() && lpRoomData->IsLeaveAlone())
		{
 		 	bSetStatus=TRUE;
		}
   
		if(bSetStatus)
		{
			for(int i = 0; i < nChairCount; i++)
			{
				LPPLAYER lpSeated = NULL;
				if(nChairStatus[i]==1 )//这个位置上有人，并且参与了游戏
				{
					if(lpRoomData->m_mapPlayer.Lookup(pTable->nPlayerAry[i], lpSeated))
					{
 						lpSeated->pe.dwLatestTime = GetTickCount();
 						lpSeated->nStatus = PLAYER_STATUS_WAITING;
					}
				}
			}
			pTable->dwBreakCount =0;
			pTable->th.nStatus = TABLE_STATUS_STATIC;
 
			NTF_GAMESTARTUP  ng;
			memset(&ng, 0, sizeof(ng));
			ng.nRoomID =nRoomID;
 			ng.nTableNO=nTableNO;
			ng.dwTableChairStatus =(lpRoomData->IsVariableChair() ? dwTableChairStatus:0 );;
  			memcpy(ng.nPlayerAry, pTable->nPlayerAry, sizeof(ng.nPlayerAry));
  		
			if(lpRoomData->IsCloakingRoom())
			{
				for(int i = 0; i < nChairCount; i++)
				{
 					if(nChairStatus[i]==1 )
					{
						NotifyOnePlayer(lpRoomData,pTable->nPlayerAry[i], GR_PLAYER_GAMEBOUTEND, &ng, sizeof(ng));
					}
				}
				NotifyDarkLooker(lpRoomData,0, GR_PLAYER_GAMEBOUTEND, &ng, sizeof(ng));
			}
			else 
			{
				NotifyRoomPlayers(lpRoomData, GR_PLAYER_GAMEBOUTEND, &ng, sizeof(ng));
			}

		}
		else  
		{
 			for(int i = 0; i < pTable->th.nPlayerCount; i++)
			{
				LPPLAYER lpSeated = NULL;
				if(lpRoomData->m_mapPlayer.Lookup(pTable->nPlayerAry[i], lpSeated))
				{
  					lpSeated->pe.bRealPlaying =FALSE;	
				}
			}
		}

	}
 
	return TRUE;
}
BOOL CBaseServer::OnUserMatchScore(DWORD nRoomScore,int nUserID)
{
	int nRoomID = HIWORD(nRoomScore);
	int nScore  = LOWORD(nRoomScore);
	
	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);	
		g_mapRoomDataEx.Lookup(nRoomID, lpRoomData);
		
		if(!g_mapRoomDataEx.Lookup(nRoomID, lpRoomData))
			return FALSE;
	}
	
	if(lpRoomData)
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		
		if(lpRoomData->IsOldMatchRoom()
			&&lpRoomData->IsLeagueMachRoom())//超级联赛专用
		{
			PLAYER_SCORE ps;
			ZeroMemory(&ps, sizeof(PLAYER_SCORE));
			
			ps.nUserID = nUserID;
			ps.nScore = nScore;
			
			LPPLAYER lpPlayer = NULL;
			if(lpRoomData->m_mapPlayer.Lookup(nUserID, lpPlayer) && lpPlayer)
			{
				lpPlayer->nScore = nScore;
				
				if(lpRoomData->IsDarkRoom() || lpRoomData->IsCloakingRoom())
				{
					NotifyOnePlayer(lpRoomData,ps.nUserID,GR_PLAYER_SCORE, &ps, sizeof(ps));
					NotifyDarkLooker(lpRoomData,ps.nUserID,GR_PLAYER_SCORE, &ps, sizeof(ps));
				}
				else 
					NotifyRoomPlayers(lpRoomData, GR_PLAYER_SCORE, &ps, sizeof(ps));
			}
		}
	}
	return TRUE;
}

BOOL CBaseServer::OnUserWelfareDeposit(DWORD nRoomDeposit,int nUserID)
{
	int nRoomID = HIWORD(nRoomDeposit);
	int nDeposit  = LOWORD(nRoomDeposit);

	if (nRoomID<=0 || nRoomID>65535
		|| nDeposit<0 || nDeposit>65535)
	{
		return FALSE;
	}
	
	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);	
		g_mapRoomDataEx.Lookup(nRoomID, lpRoomData);
		
		if(!g_mapRoomDataEx.Lookup(nRoomID, lpRoomData))
			return FALSE;
	}
	
	if(lpRoomData)
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));

		if(!lpRoomData->IsOldMatchRoom())
		{
			PLAYER_DEPOSIT pd;
			ZeroMemory(&pd, sizeof(pd));
			
			LPPLAYER lpPlayer = NULL;
			if(lpRoomData->m_mapPlayer.Lookup(nUserID, lpPlayer) && lpPlayer)
			{
				lpPlayer->nDeposit = nDeposit;
				pd.nUserID = nUserID;
				pd.nDeposit = lpPlayer->nDeposit;
				if(lpRoomData->IsDarkRoom() || lpRoomData->IsCloakingRoom())
				{
					NotifyOnePlayer(lpRoomData,pd.nUserID,GR_PLAYER_DEPOSIT, &pd, sizeof(pd));
					NotifyDarkLooker(lpRoomData,pd.nUserID,GR_PLAYER_DEPOSIT, &pd, sizeof(pd));
				}
				else
					NotifyRoomPlayers(lpRoomData, GR_PLAYER_DEPOSIT, &pd, sizeof(pd));
			}
		}
	}
	return TRUE;
}

BOOL CBaseServer::OnUserAddScore(DWORD nRoomScore,int nUserID)
{
	int nRoomID = HIWORD(nRoomScore);
	int nScore  = LOWORD(nRoomScore);
	
	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);	
		g_mapRoomDataEx.Lookup(nRoomID, lpRoomData);
		
		if(!g_mapRoomDataEx.Lookup(nRoomID, lpRoomData))
			return FALSE;
	}
	
	if(lpRoomData)
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		
//		if(!lpRoomData->IsOldMatchRoom())	//所有房间均可使用
		{
			PLAYER_SCORE ps;
			ZeroMemory(&ps, sizeof(PLAYER_SCORE));
			
			ps.nUserID = nUserID;
			LPPLAYER lpPlayer = NULL;
			if(lpRoomData->m_mapPlayer.Lookup(nUserID, lpPlayer) && lpPlayer)
			{
				lpPlayer->nScore += nScore;	//增加相应积分
				ps.nScore = lpPlayer->nScore;

				if(lpRoomData->IsDarkRoom() || lpRoomData->IsCloakingRoom())
				{
					NotifyOnePlayer(lpRoomData,ps.nUserID,GR_PLAYER_SCORE, &ps, sizeof(ps));
					NotifyDarkLooker(lpRoomData,ps.nUserID,GR_PLAYER_SCORE, &ps, sizeof(ps));
				}
				else 
					NotifyRoomPlayers(lpRoomData, GR_PLAYER_SCORE, &ps, sizeof(ps));
			}
		}
	}
	return TRUE;
}

BOOL CBaseServer::OnUserMinusScore(DWORD nRoomScore,int nUserID)
{
	int nRoomID = HIWORD(nRoomScore);
	int nScore  = LOWORD(nRoomScore);
	
	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);	
		g_mapRoomDataEx.Lookup(nRoomID, lpRoomData);
		
		if(!g_mapRoomDataEx.Lookup(nRoomID, lpRoomData))
			return FALSE;
	}
	
	if(lpRoomData)
	{
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		
//		if(!lpRoomData->IsOldMatchRoom())	//所有房间均可使用
		{
			PLAYER_SCORE ps;
			ZeroMemory(&ps, sizeof(PLAYER_SCORE));
			
			ps.nUserID = nUserID;
			LPPLAYER lpPlayer = NULL;
			if(lpRoomData->m_mapPlayer.Lookup(nUserID, lpPlayer) && lpPlayer)
			{
				if (lpPlayer->nScore < nScore)
					return TRUE;

				lpPlayer->nScore -= nScore;	//减少相应积分
				ps.nScore = lpPlayer->nScore;
				
				if(lpRoomData->IsDarkRoom() || lpRoomData->IsCloakingRoom())
				{
					NotifyOnePlayer(lpRoomData,ps.nUserID,GR_PLAYER_SCORE, &ps, sizeof(ps));
					NotifyDarkLooker(lpRoomData,ps.nUserID,GR_PLAYER_SCORE, &ps, sizeof(ps));
				}
				else 
					NotifyRoomPlayers(lpRoomData, GR_PLAYER_SCORE, &ps, sizeof(ps));
			}
		}
	}
	return TRUE;
}


BOOL CBaseServer::OnCloseSoloTable(int nRoomID,int nTableNO)
{
 	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);	
     	g_mapRoomDataEx.Lookup(nRoomID, lpRoomData);
	}
   
	if(lpRoomData)
	{	 
 		CAutoLock lock(&(lpRoomData->m_csRoomData));
 /*     tmp文件可以不校验
 		CString sTmpFile;
		sTmpFile.Format(_T("%ssolo\\%d_%d.tmp"),lpRoomData->GetGameSvrFolder(), nRoomID, nTableNO);
  
  		HANDLE hFile=CreateFile(sTmpFile, 
						  GENERIC_READ,
						  FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,
						  OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
		if(hFile==INVALID_HANDLE_VALUE)
			return FALSE;

 		START_SOLOTABLE sst;
		memset(&sst,0,sizeof(sst));

		DWORD dwRead=0;
  		SetFilePointer(hFile,0,NULL,FILE_BEGIN);
		ReadFile(hFile,&sst,sizeof(sst),&dwRead,NULL);
  		CloseHandle(hFile);
	    if(dwRead!=sizeof(sst))
			return FALSE;
  */
		LPTABLE pTable = NULL;
		if(!lpRoomData->m_mapTable.Lookup(nTableNO, pTable))
			return FALSE;
		if(pTable->th.nStatus!=TABLE_STATUS_PLAYING)
			return FALSE;
		/*
		if(memcmp(sst.nUserIDs,pTable->nPlayerAry,sizeof(sst.nUserIDs)))
			return FALSE; 
 		*/
 		LPPLAYER pPlayer = NULL;

		for(int i=0;i<lpRoomData->GetChairCount();i++)//找一个正在玩的
		{
			if(pTable->nPlayerAry[i]!=0)
			{
				if(!lpRoomData->m_mapPlayer.Lookup(pTable->nPlayerAry[i], pPlayer))
 					continue;
				if(pPlayer->nStatus ==PLAYER_STATUS_PLAYING)
  					break;
				else
					pPlayer=NULL;
			}
		}
		if(pPlayer==NULL)
			return FALSE;

		SOLOTABLE_CLOSED sc;
		memset(&sc,0,sizeof(sc));
		sc.nRoomID=nRoomID;
 		sc.nTableNO =nTableNO;
 		sc.nUserCount=pTable->th.nPlayerCount;
		memcpy(sc.nUserIDs, pTable->nPlayerAry, sizeof(sc.nUserIDs));
	
		OverSeeUserData(pPlayer->nUserID,"OnCloseSoloTable::CommitTPS");
		lpRoomData->CommitTPS(pTable,PS_WALKAROUND);

 	    SetTableAsEmptyOrLeave(lpRoomData, pPlayer);
	
		if(lpRoomData->IsCloakingRoom())
		{
			for(int i=0;i<lpRoomData->GetChairCount();i++)
			{
				if(sc.nUserIDs[i]!=0)
				{
					NotifyOnePlayer(lpRoomData,sc.nUserIDs[i], GR_SOLOTABLE_CLOSED, &sc, sizeof(sc));
				}
			}
			NotifyDarkLooker(lpRoomData,0, GR_SOLOTABLE_CLOSED, &sc, sizeof(sc));

		}
		else
		{
 			NotifyRoomPlayers(lpRoomData, GR_SOLOTABLE_CLOSED, &sc, sizeof(sc));
		}
	}
 
	return TRUE;
}






BOOL CBaseServer::OnGameSvrPulse(int nGamePort,HWND hWnd)
{
 	CAutoLock lock(&g_csGameSvrInfo);

	GAMESVR_INFO gi;
	memset(&gi,0,sizeof(gi));

	g_mapGameSvrInfo.Lookup(nGamePort,gi );

	gi.nGamePort =nGamePort;
	gi.hWnd=hWnd;
	gi.nLatestTime=UwlGetCurrentSeconds();
 
	g_mapGameSvrInfo.SetAt(nGamePort,gi );

//	UwlLogFile(_T("gamesvr pulse. gamePort=%d,hwnd=%d."),nGamePort,(int)hWnd);
	return TRUE;
}
BOOL CBaseServer::OnFindVPN(DWORD dwIPAddr,DWORD dwPort)
{
 	CAutoLock lock(&g_csProxyIP);
    g_mapProxyIP.SetAt(dwIPAddr,dwPort );
 	return TRUE;
}
BOOL CBaseServer::OnRemoveVPN(DWORD dwIPAddr,DWORD dwPort)
{
 	CAutoLock lock(&g_csProxyIP);
    g_mapProxyIP.RemoveKey(dwIPAddr);
 	return TRUE;
}

BOOL CBaseServer::OnUserBoutEnd(int nRoomID,int nUserID)
{
	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);	
		g_mapRoomDataEx.Lookup(nRoomID, lpRoomData);
	}
	
	if(lpRoomData)
	{	 
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		if(lpRoomData->IsSoloRoom())
		{
			LPPLAYER pPlayer = NULL;
			if(!lpRoomData->m_mapPlayer.Lookup( nUserID, pPlayer)) 
				return FALSE;
			pPlayer->pe.dwLatestTime = GetTickCount();
			
			if(pPlayer->nStatus != PLAYER_STATUS_PLAYING)
				return FALSE;
			
			
			PLAYER_POSITION pp;
			memset(&pp,0,sizeof(pp));
			pp.nUserID=pPlayer->nUserID;
			pp.nTableNO=pPlayer->nTableNO;
			pp.nChairNO=pPlayer->nChairNO;
			pp.nNetDelay=pPlayer->nNetSpeed;
			NotifyRoomPlayers(lpRoomData, GR_PLAYER_BOUTEND, &pp, sizeof(pp)); 
			
			pPlayer->nStatus = PLAYER_STATUS_WAITING;
			
		}
		
	}
	return TRUE;
}

BOOL CBaseServer::OnChangeChair(DWORD dwRoomTableChair,int nUserID)
{
	int nRoomID =  0;
	int nTableNO = 0;
	int nChairNO = 0;
	ParseRoomTableChair(dwRoomTableChair,nRoomID,nTableNO,nChairNO);
	
	if(nRoomID <= 0 || nTableNO < 0 || nUserID <= 0 || nChairNO < 0 || nChairNO >=MAX_CHAIR_COUNT)	
		return FALSE;
	
	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);	
		g_mapRoomDataEx.Lookup(nRoomID, lpRoomData);
		
		if(!g_mapRoomDataEx.Lookup(nRoomID, lpRoomData))
			return FALSE;
	}
	if(lpRoomData)
	{	 
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		
		if (!lpRoomData->IsSoloRoom() || lpRoomData->IsRandomRoom())
			return FALSE;
		
		LPPLAYER pPlayer = NULL;
		if(!lpRoomData->m_mapPlayer.Lookup( nUserID, pPlayer)) 
			return FALSE;
		
		if(pPlayer->nStatus != PLAYER_STATUS_WAITING)
			return FALSE;

		if (pPlayer->nChairNO == nChairNO)
			return FALSE;

		
		
 		pPlayer->pe.dwLatestTime = GetTickCount();

		LPTABLE lpTable = NULL;
		lpRoomData->m_mapTable.Lookup(nTableNO,lpTable);
		if (!lpTable)
			return FALSE;
		if (lpTable->nPlayerAry[nChairNO] != 0)//目标位置有人
			return FALSE;

		// 交换数据begin
		lpTable->nPlayerAry[nChairNO] = lpTable->nPlayerAry[pPlayer->nChairNO]; lpTable->nPlayerAry[pPlayer->nChairNO]=0;
		lpTable->dwIPAddrs[nChairNO] = lpTable->dwIPAddrs[pPlayer->nChairNO];  lpTable->dwIPAddrs[pPlayer->nChairNO]  = 0;
		lpTable->nIPConfigs[nChairNO] = lpTable->nIPConfigs[pPlayer->nChairNO];lpTable->nIPConfigs[pPlayer->nChairNO] = 0;
		lpTable->fBreakOffs[nChairNO] = lpTable->fBreakOffs[pPlayer->nChairNO];lpTable->fBreakOffs[pPlayer->nChairNO] = 0;
		lpTable->nNetSpeeds[nChairNO] = lpTable->nNetSpeeds[pPlayer->nChairNO];lpTable->nNetSpeeds[pPlayer->nChairNO] = 0;
		lpTable->nSpeedReqs[nChairNO] = lpTable->nSpeedReqs[pPlayer->nChairNO];lpTable->nSpeedReqs[pPlayer->nChairNO] = 0;
		lpTable->nMinScores[nChairNO] = lpTable->nMinScores[pPlayer->nChairNO];lpTable->nMinScores[pPlayer->nChairNO] = 0;
		lpTable->nMinDeposits[nChairNO]= lpTable->nMinDeposits[pPlayer->nChairNO];lpTable->nMinDeposits[pPlayer->nChairNO] = 0;
		// 交换数据end

		pPlayer->nStatus = PLAYER_STATUS_WAITING;
		pPlayer->nChairNO = nChairNO;
		
		//Room table chair
		lpRoomData->PostRoomTableChair(nUserID,nTableNO,nChairNO);
		lpRoomData->PostNewChair(nUserID,nTableNO,nChairNO);

		// 如何通知roomplayers呢？ 通知其他玩家 同步这个位置变化
		NTF_GET_NEWTABLE ngn;
		memset(&ngn,0,sizeof(ngn));
		ngn.pp.nUserID=nUserID;
		ngn.pp.nTableNO=nTableNO;
		ngn.pp.nChairNO=nChairNO;
		ngn.pp.nNetDelay=pPlayer->nNetSpeed;
		ngn.nMinScore =lpTable->th.nMinScore ;//
		ngn.nMinDeposit =lpTable->th.nMinDeposit ;//
		ngn.nFirstSeatedPlayer=lpTable->th.nFirstSeatedPlayer;
		
		NotifyRoomPlayers(lpRoomData, GR_PLAYER_NEWTABLE, &ngn,  sizeof(ngn));
	}
	return TRUE;
}
BOOL CBaseServer::OnHomeUserChanged(int nUserID, DWORD dwRoomTableChair)
{
	int nRoomID =  0;
	int nTableNO = 0;
	int nChairNO = 0;
	ParseRoomTableChair(dwRoomTableChair,nRoomID,nTableNO,nChairNO);
	
	if(nRoomID <= 0 || nTableNO < 0 || nUserID <= 0 )	
		return FALSE;
	CBaseRoomData* lpRoomData = NULL;
	{
		CAutoLock lock(&g_csRoomData);	
		if(!g_mapRoomDataEx.Lookup(nRoomID, lpRoomData))
			return FALSE;
	}
	if(lpRoomData)
	{	 
		CAutoLock lock(&(lpRoomData->m_csRoomData));
		
		if (!lpRoomData->IsSoloRoom() || lpRoomData->IsRandomRoom())
			return FALSE;
		if(!lpRoomData->IsPrivateRoom())
			return FALSE;
		
		LPPLAYER pPlayer = NULL;
		if(!lpRoomData->m_mapPlayer.Lookup( nUserID, pPlayer)) 
			return FALSE;
		
		LPTABLE lpTable = NULL;
		lpRoomData->m_mapTable.Lookup(nTableNO,lpTable);
		if (!lpTable)
			return FALSE;
		if(lpRoomData->FindPlayerOnTable(lpTable,nUserID))
			lpTable->nHomeUserID = nUserID;
	}
	return TRUE;
}
BOOL CBaseServer::ValidateClientEx()
{
	REQUEST request;
	memset(&request, 0, sizeof(request));
	REQUEST response;
	memset(&response, 0, sizeof(response));
	
	CONTEXT_HEAD context;
	context.hSocket = g_pSockClient->GetSocket();
	context.lSession = 0;
	context.bNeedEcho = TRUE;
	
	VALIDATE_CLIENT_EX vce;
	ZeroMemory(&vce,sizeof(vce));
	vce.nClientID = g_nClientID; // 
	vce.nClientType = CLIENT_TYPE_ROOM; // 
	xyGetHardID(vce.szHardID);
	xyGetVolumeID(vce.szVolumeID);
	xyGetMachineID(vce.szMachineID);
	request.head.nRequest = GR_VALIDATE_CLIENT_EX;
	request.nDataLen = sizeof(vce);
	request.pDataPtr =&vce;
	
	BOOL bTimeout = FALSE;
	BOOL bSendOK = g_pSockClient->SendRequest(&context, &request, &response, bTimeout, 10000);
	if(!bSendOK || UR_FETCH_SUCCEEDED != response.head.nRequest)
	{
		UwlLogFile(_T("ValidateClientEx() failed!"));
		UwlClearRequest(&response);
		return FALSE;
	}
	
	//	LPFILTER_WORD lpFilterWord = LPFILTER_WORD(response.pDataPtr);
	
	UwlClearRequest(&response);
	UwlLogFile(_T("ValidateClientEx() OK!"));
	return TRUE;
}

void CBaseServer::ClearRoomDataMap()
{
	CAutoLock lock(&g_csRoomData);
	
	int roomid = 0;
	CBaseRoomData* lpRoomData = NULL;
	POSITION pos = g_mapRoomDataEx.GetStartPosition();
	while (pos)	{
		g_mapRoomDataEx.GetNextAssoc(pos, roomid, lpRoomData);
		if(lpRoomData){
			CAutoLock lock(&(lpRoomData->m_csRoomData));
			
			int userid = 0;
			LPPLAYER lpPlayer = NULL;
			POSITION posPlayer = lpRoomData->m_mapPlayer.GetStartPosition();
			while(posPlayer){
				lpRoomData->m_mapPlayer.GetNextAssoc(posPlayer, userid, lpPlayer);
				FreePlayerBlock(lpPlayer);
			}
			lpRoomData->m_mapPlayer.RemoveAll();
			
			int tableno = 0;
			LPTABLE lpTable = NULL;
			POSITION posTable = lpRoomData->m_mapTable.GetStartPosition();
			while(posTable){
				lpRoomData->m_mapTable.GetNextAssoc(posTable, tableno, lpTable);
				SAFE_DELETE(lpTable);
			}
			lpRoomData->m_mapTable.RemoveAll();
			lpRoomData->m_mapPrivateTableInfo.RemoveAll();
		}
		SAFE_DELETE(lpRoomData);
	}
	g_mapRoomDataEx.RemoveAll();
	
	return;
}

void CBaseServer::ClearPlayerPool()
{
	CAutoLock lock(&g_csPlayerList);
	
	while(!g_PoolOfPlayer.IsEmpty()){
		PLAYER* pPlayer = (PLAYER*)(g_PoolOfPlayer.RemoveHead());
		SAFE_DELETE(pPlayer);
	}
}


void 	CBaseServer::ClearPlayerKickList()
{
	CAutoLock lock(&g_csPlayerKickList);
	
	int nUserID = 0;
	CKickoffDetailMap* pDetailMap = NULL;
	POSITION pos = g_mapPlayerKickList.GetStartPosition();
	while (pos)	{
		g_mapPlayerKickList.GetNextAssoc(pos, nUserID, pDetailMap);
		if (pDetailMap)
		{
			pDetailMap->RemoveAll();
			SAFE_DELETE(pDetailMap);
		}
	}
	g_mapPlayerKickList.RemoveAll();
}

void	CBaseServer::ClearKickoffRecordMap()
{
	int nCount = 0;
	{	
		CAutoLock lock(&g_csKickoffRecord);
		nCount = g_mapKickoffRecord.GetCount();
		g_mapKickoffRecord.RemoveAll();
	}
	
	UwlLogFile(_T("clear kick-record done. count = %ld."), nCount);
}

BOOL CBaseServer::ClearOverdueKickDetailMap(CKickoffDetailMap*  pKickDetailMap, int nNow, int nMaxIsolate, int& nCount)
{
	if (!pKickDetailMap)
		return TRUE;
	
	int nOther = 0;
	KICKOFF_DETAIL kd;
	ZeroMemory(&kd, sizeof(KICKOFF_DETAIL));
	
	POSITION pos = pKickDetailMap->GetStartPosition();
	while (pos)	{
		pKickDetailMap->GetNextAssoc(pos, nOther, kd);
		if (nNow-kd.nKickTime>=nMaxIsolate)
		{
			pKickDetailMap->RemoveKey(nOther);
			nCount++;
		}
	}
	
	return TRUE;
}

void CBaseServer::SendRoomSvrPulse()
{
	REQUEST request;
	memset(&request, 0, sizeof(request));
	REQUEST response;
	memset(&response, 0, sizeof(response));
	
	CONTEXT_HEAD context;
	memset(&context, 0, sizeof(context));
	context.hSocket = g_pSockClient->GetSocket();
	
	ROOMSVR_PULSE rp;
	ZeroMemory(&rp,sizeof(ROOMSVR_PULSE));
	rp.nRoomSvrID = g_nClientID; // 
	
	request.head.nRequest = GR_ROOMSVR_PULSE;
	request.nDataLen = sizeof(ROOMSVR_PULSE);
	request.pDataPtr = &rp;
	
	BOOL bTimeout = FALSE;
	BOOL bSendOK = g_pSockClient->SendRequest(&context, &request, &response, 
		bTimeout);
	
}
void CBaseServer::SendPulseData()
{
	REQUEST request;
	memset(&request, 0, sizeof(request));
	REQUEST response;
	memset(&response, 0, sizeof(response));
	
	CONTEXT_HEAD context;
	memset(&context, 0, sizeof(context));
	context.hSocket = g_pSockClient->GetSocket();
	
	request.head.nRequest = GR_SEND_PULSE;
	
	BOOL bSendOK = g_pSockClient->SendCast(&context, &request, &response);
}

void  CBaseServer::ParseRoomTableChair(DWORD dwRoomTableChair,int& nRoomID,int& nTableNO,int& nChairNO)
{
	//2012.11.7  TableNo站位从256扩大到1024
	// 	nRoomID = HIWORD(dwRoomTableChair);
	// 	
	// 	nChairNO = (LOBYTE(LOWORD(dwRoomTableChair)))&0x3f;
	// 	
	// 	nTableNO = (int)(MAKEWORD(HIBYTE(LOWORD(dwRoomTableChair)), LOBYTE(LOWORD(dwRoomTableChair))>>6));
	
	//2014.3.18  TableNo站位从1024扩大到4096
	nRoomID = HIWORD(dwRoomTableChair);
	
	nChairNO = (LOBYTE(LOWORD(dwRoomTableChair)))&0xf;
	
	nTableNO = (int)(MAKEWORD(
		HIBYTE(LOWORD(dwRoomTableChair)), 
		(LOBYTE(LOWORD(dwRoomTableChair))>>6)|(((LOBYTE(LOWORD(dwRoomTableChair))&0x30))<<2)>>4)
		);
}

void CBaseServer::ParseTableChairStatus(DWORD dwTableChairStatus, int &nTableNO, int nChairStatus[], int nChairCount)
{
	ASSERT(nChairCount<=16);
	nTableNO = HIWORD(dwTableChairStatus);
	WORD wChairStatus = LOWORD(dwTableChairStatus);
	for (int i=0;i<nChairCount;i++)
	{
		nChairStatus[i] = ((wChairStatus>>i)&1);
	}
}

void 	CBaseServer::ClearOverdueKickList(int nNowSeconds, int nMaxIsolate)
{
	int nClearMapCount = 0;
	int nClearDetailCount = 0;
	
	{
		CAutoLock lock(&g_csPlayerKickList);
		
		int nUserID = 0;
		CKickoffDetailMap* pDetailMap = NULL;
		POSITION pos = g_mapPlayerKickList.GetStartPosition();
		while (pos)	{
			g_mapPlayerKickList.GetNextAssoc(pos, nUserID, pDetailMap);
			
			if (!pDetailMap)
			{
				g_mapPlayerKickList.RemoveKey(nUserID);
				nClearMapCount++;
			}
			else
			{
				ClearOverdueKickDetailMap(pDetailMap, nNowSeconds, nMaxIsolate, nClearDetailCount/*out: clear count*/);
				if (pDetailMap->GetCount()==0)
				{
					SAFE_DELETE(pDetailMap);
					g_mapPlayerKickList.RemoveKey(nUserID);
					nClearMapCount++;
				}
			}
		}
	}
	
	UwlLogFile(_T("clear overdue kick-detail done. Clear map count = %ld. Clear detail count = %ld"), nClearMapCount, nClearDetailCount);
}

BOOL CBaseServer::KickOffDeadAtMorning(int deadtime, BOOL bStatic)
{
	int roomid = 0;
	CBaseRoomData* lpRoomData = NULL;
	CRoomDataExMap mapRoomDataEx;
	{
		CAutoLock lock(&g_csRoomData);	
		POSITION pos = g_mapRoomDataEx.GetStartPosition();
		while (pos)	{
			g_mapRoomDataEx.GetNextAssoc(pos, roomid, lpRoomData);
			mapRoomDataEx.SetAt(roomid, lpRoomData);
		}
	}
	POSITION pos = mapRoomDataEx.GetStartPosition();
	while (pos)	{
		mapRoomDataEx.GetNextAssoc(pos, roomid, lpRoomData);
		if(lpRoomData){
			CAutoLock lock(&(lpRoomData->m_csRoomData));
			
			KickOffDeadPlayersInRoom(lpRoomData, deadtime, bStatic, TRUE);
			KickOffTablePlaying(lpRoomData);
			ClearRoomChat(lpRoomData);
		}
	}
	return TRUE;
}

BOOL CBaseServer::KickOffTablePlaying(CBaseRoomData* lpRoomData)
{
	int nBeginRandomTableNO=lpRoomData->GetBeginRandomTableNO(); 
	int isClearInterHall= GetPrivateProfileInt(
		_T("KickOff"),			// section name
		_T("isClearInterHall"),			// key name
		1,		// default int
		g_szIniFile				// initialization file name
										);
	int tableno = 0;
	LPTABLE pTable = NULL;
	POSITION posTable = lpRoomData->m_mapTable.GetStartPosition();
	while(posTable){
		lpRoomData->m_mapTable.GetNextAssoc(posTable, tableno, pTable);
		
		if (!pTable)
			continue;
		if (tableno>=nBeginRandomTableNO)
			continue;
		if(pTable->th.nStatus!=TABLE_STATUS_PLAYING)
			continue;
		if(pTable->th.nPlayerCount<=1) //单机游戏不踢
			continue;
		if(UwlGetCurrentSeconds()- pTable->nStartupTime <= MAX_TABLEPLAYING_SECONDS )//这一桌开启不超过8小时
			continue;
		
		int nChairNO=xyGetRandomDigit(pTable->th.nPlayerCount-1);
		if(nChairNO>=MAX_CHAIR_COUNT)
			continue;
        int nPlayerID=pTable->nPlayerAry[nChairNO];
		if(nPlayerID==0)
			continue;
		// 
		if(isClearInterHall==0)
		{
			bool bHaveInterPlayer = false;
			for (int n = 0 ; n< pTable->th.nPlayerCount;n++)
			{
				int nUserID=pTable->nPlayerAry[nChairNO];
				LPPLAYER lpPlayer = NULL;
				lpRoomData->m_mapPlayer.Lookup(nUserID, lpPlayer);
				if (lpPlayer && IS_BIT_SET(lpPlayer->pe.dwEnterFlags,FLAG_ENTERROOM_INTER))//内部大厅进的不踢
				{
					bHaveInterPlayer = true;
					break;
				}
			}
			if(bHaveInterPlayer)
				continue;
		}
		
		CONTEXT_HEAD context;
		ZeroMemory(&context ,sizeof(context));
		context.dwFlags|=CH_FLAG_SYSTEM_EJECT;		
		KickOffRoomPlayer(&context, lpRoomData, nPlayerID,TRUE); //踢出
		
		KICKOFF_ROOM_PLAYER  krp;
		ZeroMemory(&krp,sizeof(krp));
		krp.nAdminID=0;
		krp.nAreaID =lpRoomData->roomdata.nAreaID;
		krp.nGameID =lpRoomData->roomdata.nGameID;
		krp.nRoomID =lpRoomData->roomdata.nRoomID;
		krp.nPlayer=nPlayerID;
		lstrcpy(krp.szMsg,_T("系统清除"));
		krp.nMsgLen =lstrlen(krp.szMsg)+1;
		NotifyServer(GR_KICKOFF_ROOM_PLAYER,&krp,sizeof(krp)-MAX_ROOMCHAT_LEN+krp.nMsgLen);
	}
	
	return TRUE;
}

BOOL CBaseServer::GetChatForbid()
{
	REQUEST request;
	memset(&request, 0, sizeof(request));
	REQUEST response;
	memset(&response, 0, sizeof(response));
	
	CONTEXT_HEAD context;
	context.hSocket = g_pSockClient->GetSocket();
	context.lSession = 0;
	context.bNeedEcho = TRUE;
	
	request.head.nRequest = GR_GET_CHATFORBID;
	request.nDataLen = 0;
	request.pDataPtr =0;
	
	BOOL bTimeout = FALSE;
	BOOL bSendOK = g_pSockClient->SendRequest(&context, &request, &response, 
		bTimeout, 30*1000);
	if(!bSendOK || UR_FETCH_SUCCEEDED != response.head.nRequest){
		UwlLogFile(_T("GetChatForbid() failed!"));
		UwlClearRequest(&response);
		return FALSE;
	}
	LPCHAT_FORBID lpChatForbid = LPCHAT_FORBID(response.pDataPtr);
	{ 
		CString sKey;
		CAutoLock lock(&g_csChatForbid);
		g_mapChatForbid.RemoveAll();
		g_mapChatForbidID.RemoveAll();
		
		CHAT_FORBID  cf;
		int nCount=response.nDataLen / sizeof(CHAT_FORBID);
        for(int i = 0; i < nCount; i++)
		{
			sKey.Format(_T("%s"),lpChatForbid->szHardID);
			memcpy(&cf,lpChatForbid,sizeof(CHAT_FORBID));
			g_mapChatForbid.SetAt(sKey,cf);
			g_mapChatForbidID.SetAt(cf.nPlayerID,cf);
			///////////////////////////////////////////////
			lpChatForbid++;
		}
	}
	UwlClearRequest(&response);
	UwlLogFile(_T("GetChatForbid() OK!"));
	return TRUE;
} 
