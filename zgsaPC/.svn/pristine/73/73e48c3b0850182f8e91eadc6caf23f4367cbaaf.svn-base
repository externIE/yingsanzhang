#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define	MAX_PROCESSORS_SUPPORT	4

#define LICENSE_FILE		_T("license.dat")
#define PRODUCT_NAME		_T("ZgjhChunksvr")
#define	PRODUCT_VERSION		_T("1.00")

#define DEF_RESPONSE_WAIT	10000
#define DEF_TIMER_INTERVAL	5 // minitues
#define MIN_TIMER_INTERVAL	1 // minitues
#define DEF_ROOMSVRPULSE_INTERVAL    10 //seconds
#define DEF_ROOMSVRPULSE_DEADTIME    30 //seconds
#define DEF_CLIENTPULSE_INTERVAL    10 //seconds


#define DEF_REFRESH_ELAPSE		(DEF_TIMER_INTERVAL * 12) // minitues

#define MAX_FORBIDDEN_IPS		100 // 

#define	DEF_KICKOFF_DEADTIME	360			// minutes
#define	DEF_KICKOFF_STIFFTIME	300			// seconds

CMainServer::CMainServer()
{ 
	ZeroMemory(&g_threadInfo, sizeof(g_threadInfo));
	ZeroMemory(&g_SockServer, sizeof(g_SockServer));
 	ZeroMemory(&g_ChunkDB, sizeof(g_ChunkDB));

 	ZeroMemory(&g_pSockClients ,sizeof(g_pSockClients));	
	ZeroMemory(&g_uiThrdSends,sizeof(g_uiThrdSends));
	ZeroMemory(&g_uiThrdConnects,sizeof(g_uiThrdConnects));

 	g_pSockServer = new CSockServer(KEY_HALL, ENCRYPT_AES, 0);

	g_hExitServer = NULL;
}

CMainServer::~CMainServer()
{
	for(int i = 0; i < MAX_TOTALSVR_COUNT; i++){
		SAFE_DELETE(g_pSockClients[i]);
	}
	SAFE_DELETE(g_pSockServer);
}

BOOL CMainServer::InitStart()
{	
	g_hExitServer = CreateEvent(NULL, TRUE, FALSE, NULL);

 	TCHAR szFullName[MAX_PATH];
	GetModuleFileName(GetModuleHandle(NULL), szFullName, sizeof(szFullName));
 	UwlSplitPath(szFullName, SPLIT_DRIVE_DIR, g_szIniFile);
	lstrcat(g_szIniFile, PRODUCT_NAME);
	lstrcat(g_szIniFile, _T(".ini"));

	UwlSplitPath(szFullName, SPLIT_DRIVE_DIR, g_szDBAFile);
	lstrcat(g_szDBAFile, _T("dbaccount.dba"));
 
	TCHAR szPID[32];
	sprintf(szPID,_T("%d"),GetCurrentProcessId());
	WritePrivateProfileString(_T("listen"),		// section name
							_T("pid"),	// key name
							szPID,		//
							g_szIniFile			// initialization file name
							);


	g_nClientID = GetPrivateProfileInt(_T("listen"), _T("clientid"), 0, g_szIniFile);	
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

	return TRUE;
}

BOOL CMainServer::GetSockServers()
{
	REQUEST request;
	memset(&request, 0, sizeof(request));
	REQUEST response;
	memset(&response, 0, sizeof(response));

	CONTEXT_HEAD context;
	context.hSocket = g_pSockClients[0]->GetSocket();
	context.lSession = 0;
	context.bNeedEcho = TRUE;
	
	request.head.nRequest = CR_GET_SOCKSERVER;

	BOOL bTimeout = FALSE;
	BOOL bSendOK = g_pSockClients[0]->SendRequest(&context, &request, &response, 
									bTimeout, 10000);
	if(!bSendOK || UR_FETCH_SUCCEEDED != response.head.nRequest){
		UwlLogFile(_T("GetSockServers() failed!"));
		UwlClearRequest(&response);
		return FALSE;
	}
	UwlLogFile(_T("GetSockServers() OK!"));
	memset(g_SockServer,0,sizeof(g_SockServer));
	memcpy(g_SockServer, response.pDataPtr, response.nDataLen);
	UwlClearRequest(&response);

	g_nTotalSvrCount=response.nDataLen / sizeof(SOCK_SERVER);
	for(int i=1;i<g_nTotalSvrCount;i++)
	{
		if(g_SockServer[i].nType==TYPE_SUB_SERVER)
			g_nSubSvrCount++;
	}
 	return TRUE;
}

BOOL CMainServer::CreateClientPulseThread()
{
 
	g_hThrdClientPulse = (HANDLE)_beginthreadex(NULL,				// Security
										0,					// Stack size - use default
										ClientPulseThreadFunc,    // Thread fn entry point
										(void*) this,		// Param for thread
										0,					// Init flag
										&g_uiThrdClientPulse);	// Thread address;
	return TRUE;
}

BOOL CMainServer::CreateTimerThread()
{
 
	g_hThrdTimer = (HANDLE)_beginthreadex(NULL,				// Security
										0,					// Stack size - use default
										TimerThreadFunc,    // Thread fn entry point
										(void*) this,		// Param for thread
										0,					// Init flag
										&g_uiThrdTimer);	// Thread address;
	return TRUE;
}

BOOL CMainServer::ReadMainDBData()
{
	/*
 	_ConnectionPtr pConnect = NULL;
    if(!ConnectMainDB(pConnect))
	{
		UwlTrace(_T("connect main db failed."));
 		UwlLogFile(_T("connect main db failed"));
 		return FALSE;
	}
	RefreshGameKubSvrID(pConnect);

	if(!RefreshGameDBInfo(pConnect))
	{
		UwlTrace(_T("refresh gamedb info failed."));
 		UwlLogFile(_T("refresh gamedb info failed"));
 		return FALSE;
	}

	RefreshFilterWord(pConnect);
	RefreshMutexHard(pConnect);
	RefreshProxyIP(pConnect);
	RefreshGameSvrIP(pConnect);

	DB_SafeClose(pConnect);

	UwlTrace(_T("get data from maindb succeeded."));
 	UwlLogFile(_T("get data from maindb succeeded."));

	*/
	return TRUE;

}
BOOL CMainServer::Initialize()
{
	UwlLogFile(_T("server starting..."));

	if(S_FALSE == ::CoInitialize(NULL))
 		return FALSE;

	if(!InitStart())
		return FALSE;


	/*	按需开启

	// 订阅登录在线
	if(!ConnectOnlineSvr())
		return FALSE;

	// 订阅充值、游戏通知
	if(!ConnectTrankGameSvr())
		return FALSE;

	*/

	if(!InitChunkDB()) 
		return FALSE;

	//启动时需要从数据库读取一些数据
    if(!ReadMainDBData())
		return FALSE;

  	CreateTimerThread();
	CreateClientPulseThread();
 	////////////////////////////////////////////////////////
 
 	UwlTrace(_T("Server start up OK."));
	UwlLogFile(_T("Server start up OK."));

	int nCheckPort = GetPrivateProfileInt(_T("listen"),_T("port"),	PORT_CHUNK_SERVER,	g_szIniFile	);
 
	SYSTEM_INFO SystemInfo;
	ZeroMemory(&SystemInfo,sizeof(SystemInfo));
	GetSystemInfo( &SystemInfo );
	UwlTrace(_T("number of processors: %lu"), SystemInfo.dwNumberOfProcessors);
	UwlLogFile(_T("number of processors: %lu"), SystemInfo.dwNumberOfProcessors);

	if(SystemInfo.dwNumberOfProcessors > MAX_PROCESSORS_SUPPORT)
	{
		//监听端口
		//启动MAX_PROCESSORS_SUPPORT * 2个工作线程
		return g_pSockServer->Initialize(_T(""), nCheckPort, 
										GetHelloData(), GetHelloLength() + 1,
         								0, 10,
										MAX_OVERLAPPED_ACCEPTS, 
										MAX_OVERLAPPED_SENDS, 
										DEF_ACCEPT_WAIT,
										DEF_SOCBUF_SIZE,
										MAX_PROCESSORS_SUPPORT * 2, 
										MAX_PROCESSORS_SUPPORT * 2);
	}
	else
	{
		//监听端口
		//启动 CPU核数 * 2个工作线程
		return g_pSockServer->Initialize(_T(""), nCheckPort, 
										GetHelloData(), GetHelloLength() + 1,
         								0, 10);
	}
	
	/*
	return g_pSockServer->Initialize(_T(""), nCheckPort, 
										GetHelloData(), GetHelloLength() + 1,
         								0, 10,
										MAX_OVERLAPPED_ACCEPTS, 
										MAX_OVERLAPPED_SENDS, 
										DEF_ACCEPT_WAIT,
										DEF_SOCBUF_SIZE,
										SystemInfo.dwNumberOfProcessors, 
										SystemInfo.dwNumberOfProcessors);
	*/
}

BOOL CMainServer::ConnectOnlineSvr()
{
	TCHAR szOnlineIP[MAX_SERVERIP_LEN];
	memset(szOnlineIP, 0, sizeof(szOnlineIP));
	GetPrivateProfileString(_T("OnlineServer"), _T("Name"), _T(""), szOnlineIP, sizeof(szOnlineIP), g_szIniFile);
	if(!lstrcmpi(szOnlineIP, _T(""))){
		UwlLogFile(_T("Online server ip is empty!"));
		return FALSE;
	}
	
	int nPort = GetPrivateProfileInt(_T("OnlineServer"),_T("port"),PORT_TRANK_SERVER,g_szIniFile); 
	
	if (ConnectOneServer(SOCKCILENT_ONLINESVR, szOnlineIP, nPort, _T("OnlineServer"), TYPE_TRANK_SERVER))
	{
		// validate client id
		ValidateClientInfo(SOCKCILENT_ONLINESVR, g_nClientID);
		
		//向Subsvr订阅登录消息
		RSSPlayerLogOnOff(SOCKCILENT_ONLINESVR, g_nClientID);
	}

	return TRUE;
}


void CMainServer::Shutdown()
{
	SetEvent(g_hExitServer);

	PostThreadMessage(g_uiThrdTimer, WM_QUIT, 0, 0);
	WaitForSingleObject(g_hThrdTimer, WAITTIME_EXIT);	
	if(g_hThrdTimer){
		CloseHandle(g_hThrdTimer);
		g_hThrdTimer = NULL;
	}

	PostThreadMessage(g_uiThrdClientPulse, WM_QUIT, 0, 0);
	WaitForSingleObject(g_hThrdClientPulse, WAITTIME_EXIT);	
	if(g_hThrdClientPulse){
		CloseHandle(g_hThrdClientPulse);
		g_hThrdClientPulse = NULL;
	}

	for(int i = 0; i < MAX_TOTALSVR_COUNT; i++)
	{
		if (!g_uiThrdSends[i])
			continue;

		PostThreadMessage(g_uiThrdSends[i], WM_QUIT, 0, 0);
		WaitForSingleObject(g_hThrdSends[i], WAITTIME_EXIT);
		if(g_hThrdSends[i]){
			CloseHandle(g_hThrdSends[i]);
			g_hThrdSends[i] = NULL;
		}

		PostThreadMessage(g_uiThrdConnects[i], WM_QUIT, 0, 0);
		WaitForSingleObject(g_hThrdConnects[i], WAITTIME_EXIT);	
		if(g_hThrdConnects[i]){
			CloseHandle(g_hThrdConnects[i]);
			g_hThrdConnects[i] = NULL;
		}
	}
 
 
	for(i = 0; i < MAX_TOTALSVR_COUNT; i++)
	{
		if(g_pSockClients[i])
		{
			g_pSockClients[i]->Destroy();
			UwlLogFile(_T("socket clients terminated. svrindex = %ld"), i);
		}
	}

	g_pSockServer->Shutdown();
	UwlLogFile(_T("socket server terminated."));

	if(g_hExitServer){
		CloseHandle(g_hExitServer);
		g_hExitServer = NULL;
	}

	ClearGameSvrTokenMap();

 	::CoUninitialize();
	UwlLogFile(_T("server exited."));
}

unsigned CMainServer::SendThreadFunc(LPVOID lpVoid)
{
	LPTHREAD_INFO pThreadInfo = (LPTHREAD_INFO)lpVoid;
	CMainServer * pThread = (CMainServer *) (pThreadInfo->lpVoid);

    return pThread->SendThreadProc(pThreadInfo->nIndex);
} 

unsigned CMainServer::SendThreadProc(int svrindex)
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
				BOOL bSendOK = g_pSockClients[svrindex]->SendRequest(&context, pRequest, NULL, bTimeout);

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
				request.head.nRepeated = 2;

				UwlClearRequest(pRequest);
				SAFE_DELETE(pContext);
				SAFE_DELETE(pRequest);

				CONTEXT_HEAD context;
				memset(&context, 0, sizeof(context));
						
				BOOL bTimeout = FALSE;
				BOOL bSendOK = g_pSockClients[svrindex]->SendRequest(&context, &request, NULL, bTimeout);

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

unsigned CMainServer::ConnectThreadFunc(LPVOID lpVoid)
{	
	LPTHREAD_INFO pThreadInfo = (LPTHREAD_INFO)lpVoid;
	CMainServer * pThread = (CMainServer *) (pThreadInfo->lpVoid);

    return pThread->ConnectThreadProc(pThreadInfo->nIndex);
} 

unsigned CMainServer::ConnectThreadProc(int svrindex)
{
	UwlTrace(_T("connect thread started. id = %d"), GetCurrentThreadId());
	
	MSG msg;
	while (GetMessage(&msg, 0, 0, 0))
	{
		if(UM_DATA_TOSEND == msg.message)
		{
			if(UR_SOCKET_CLOSE == msg.wParam)
			{
				while(TRUE)
				{
					int nPort = g_SockServer[svrindex].nPort;
					if(FALSE == g_pSockClients[svrindex]->BeginConnect(g_SockServer[svrindex].szIP, nPort, 1))
					{
						UwlTrace(_T("Can not connect to %s server. index = %ld."), g_SockServer[svrindex].szName,svrindex);
						UwlLogFile(_T("Can not connect to %s server. index = %ld."),g_SockServer[svrindex].szName,svrindex);
						if(!g_hExitServer || 
							WAIT_OBJECT_0 == WaitForSingleObject(g_hExitServer, 0)){
							break;
						}
					}
					else
					{
						UwlTrace(_T("reconnect to %s server OK. index = %ld"), g_SockServer[svrindex].szName,svrindex);
						UwlLogFile(_T("reconnect to %s server OK. index = %ld"),g_SockServer[svrindex].szName, svrindex);
						// validate client id
						ValidateClientInfo(svrindex, g_nClientID);

						//向Subsvr订阅登录消息
						if (TYPE_SUB_SERVER == g_SockServer[svrindex].nType)
						{
							RSSPlayerLogOnOff(svrindex, g_nClientID);
						}

						break;
					}
				}
			}			
		}
		else
		{
			DispatchMessage(&msg);
		}
	}
	UwlTrace(_T("connect thread exiting. id = %d"), GetCurrentThreadId());
 	UwlLogFile(_T("connect thread exiting. id = %d"), GetCurrentThreadId());

   	return 0;
} 

unsigned CMainServer::TimerThreadFunc(LPVOID lpVoid)
{
	CMainServer * pThread = (CMainServer *) lpVoid;

    return pThread->TimerThreadProc();
} 

unsigned CMainServer::TimerThreadProc()
{
	UwlTrace(_T("timer thread started. id = %d"), GetCurrentThreadId());

	int sameHour = 0;
	int lastHour = -1;
	int cycleMin = 0;

	while(TRUE){
		int interval = GetPrivateProfileInt(
					_T("Timer"),			// section name
					_T("Interval"),			// key name
					DEF_TIMER_INTERVAL,		// default int
					g_szIniFile				// initialization file name
					);
		if(interval < MIN_TIMER_INTERVAL){ // 至少1分钟周期
			interval = MIN_TIMER_INTERVAL;
		}
		DWORD dwRet = WaitForSingleObject(g_hExitServer, interval * 60 * 1000);
		if(WAIT_OBJECT_0 == dwRet){ // exit event
			break;
		}
		if(WAIT_TIMEOUT == dwRet){ // timeout
			UwlTrace(_T("timer thread triggered. do something. interval = %ld minutes."), interval);
//			UwlLogFile(_T("timer thread triggered. do something. interval = %ld minutes."), interval);
			
			for(int i=0;i<MAX_TOTALSVR_COUNT;i++)
			{
				if(g_pSockClients[i])
					SendPulseData(i);  				
			}

			/////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////
			{
				int mode = GetPrivateProfileInt(
									_T("Refresh"),		// section name
									_T("Mode"),			// key name
									0,					// default int
									g_szIniFile			// initialization file name
									);
				int timing = GetPrivateProfileInt(
									_T("Refresh"),		// section name
									_T("Timing"),		// key name
									0,					// default int
									g_szIniFile			// initialization file name
									);
				int elapse = GetPrivateProfileInt(
									_T("Refresh"),			// section name
									_T("Elapse"),			// key name
									DEF_REFRESH_ELAPSE,		// default int
									g_szIniFile				// initialization file name
									);
				int timingOK = 0;
				if(1 == mode){
					cycleMin += interval;
					if(cycleMin >= elapse){
						cycleMin = 0;
						timingOK = 1;
					}
				}
				if(2 == mode){
					SYSTEMTIME time;
					GetLocalTime(&time);
					if(time.wHour != lastHour){
						sameHour = 0;
					}else{
						sameHour++;
					}
					lastHour = time.wHour;
					if(time.wHour == timing){
						if(0 == sameHour){
							timingOK = 1;
						}
					}
				}
				if(timingOK)
				{
					//Do something
					
				}
			}
			////////////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////
		}
	}

	UwlTrace(_T("timer thread exiting. id = %d"), GetCurrentThreadId());
	UwlLogFile(_T("timer thread exiting. id = %d"), GetCurrentThreadId());

   	return 0;
} 


unsigned CMainServer::ClientPulseThreadFunc(LPVOID lpVoid)
{
	CMainServer * pThread = (CMainServer *) lpVoid;

    return pThread->ClientPulseThreadProc();
} 


unsigned CMainServer::ClientPulseThreadProc()
{
	UwlTrace(_T("pulse thread started. id = %d"), GetCurrentThreadId());
	
	while(TRUE)
	{
		int interval = GetPrivateProfileInt(
					_T("ClientPulse"),			// section name
					_T("Interval"),			// key name
					DEF_CLIENTPULSE_INTERVAL,		//seconds
					g_szIniFile				// initialization file name
					);
		DWORD dwRet = WaitForSingleObject(g_hExitServer, interval  * 1000);
		if(WAIT_OBJECT_0 == dwRet)// exit event
		{ 
			break;
		}
		if(WAIT_TIMEOUT == dwRet)// timeout
		{ 
 			CONTEXT_HEAD context;
			memset(&context, 0, sizeof(context));
			context.bNeedEcho = FALSE;
			
			REQUEST request;
			memset(&request, 0, sizeof(request));
 			request.head.nRequest = GR_SEND_PULSE;
 	 
			CTokenSockMap mapTokenSock;
			CopyMapAllTokenSock(mapTokenSock);
	
			POSITION pos = mapTokenSock.GetStartPosition();
			while (pos)	
			{	
				SOCKET sock = 0;
				LONG token = 0;
				mapTokenSock.GetNextAssoc(pos, token, sock);
  				if(sock)
				{
					context.hSocket = sock;
					context.lTokenID = token;
					DWORD dwEncrypt = g_pSockServer->m_flagEncrypt;
 					g_pSockServer->SendRequest(context.hSocket, &context, &request,MSG_REQUEST, dwEncrypt, 0);
				}
			}
		}
	 
	}
	UwlTrace(_T("clientpulse thread exiting. id = %d"), GetCurrentThreadId());
  	UwlLogFile(_T("clientpulse thread exiting. id = %d"), GetCurrentThreadId());

	return 0;
}

BOOL CMainServer::InitChunkDB()
{
	ZeroMemory(&g_ChunkDB, sizeof(g_ChunkDB));

	BOOL bRet = FALSE;

	try{
		int nDBCount = ReadChunkDBConfig(g_ChunkDB, MAX_TOTALDB_COUNT);
		
		UwlLogFile(_T("Init %d chunk db success."), nDBCount);

		bRet = TRUE;

	}catch(...)
	{
		UwlLogFile(_T("Init chunk db failed."));

		bRet = FALSE;	
	} 

	return bRet;
}


BOOL CMainServer::ConnectTrankGameSvr()
{
	TCHAR szTrankIP[MAX_SERVERIP_LEN];
	memset(szTrankIP, 0, sizeof(szTrankIP));
	GetPrivateProfileString(_T("TrankGame"), _T("Name"), _T(""), szTrankIP, sizeof(szTrankIP), g_szIniFile);
	if(!lstrcmpi(szTrankIP, _T(""))){
		UwlLogFile(_T("TrankGame server ip is empty!"));
		return FALSE;
	}

	int nPort = GetPrivateProfileInt(_T("TrankGame"),_T("port"),PORT_TRANK_SERVER,g_szIniFile); 

	if (ConnectOneServer(SOCKCILENT_TRANKGAME, szTrankIP, nPort, _T("TrankGame"), TYPE_TRANK_SERVER))
	{
		// validate client id
		ValidateClientInfo(SOCKCILENT_TRANKGAME, g_nClientID);
		
		//订阅充值消息
		SubscribePayEvent(SOCKCILENT_TRANKGAME, g_nClientID);
	}
	
	return TRUE;
}


BOOL CMainServer::ConnectOneServer(int nServerIndex, LPCTSTR lpszIP, int nPort, LPCTSTR lpszServerName, int nServerType)
{
	if((NULL == lpszIP) || !lstrcmpi(lpszIP, _T(""))){
		UwlLogFile(_T("%s server ip is empty!"), lpszServerName);
		return FALSE;
	}

	ASSERT(nServerIndex>=0 && nServerIndex<MAX_TOTALSVR_COUNT);
	
	// 连接OnlineServer 信息有错误
	g_SockServer[nServerIndex].nID		= nServerIndex;
	g_SockServer[nServerIndex].nPort	= nPort;
	g_SockServer[nServerIndex].nType	= nServerType;
	lstrcpyn(g_SockServer[nServerIndex].szIP, lpszIP, MAX_SERVERIP_LEN);
	lstrcpyn(g_SockServer[nServerIndex].szName, lpszServerName, MAX_SERVERNAME_LEN);
	g_pSockClients[nServerIndex]		= new CSockClient(KEY_HALL, ENCRYPT_AES, 0, nServerIndex);
	
	g_threadInfo[nServerIndex].lpVoid	= this;
	g_threadInfo[nServerIndex].nIndex	= nServerIndex;
	g_hThrdSends[nServerIndex]			= (HANDLE)_beginthreadex(NULL,					// Security
		0,												// Stack size - use default
		SendThreadFunc,									// Thread fn entry point
		(void*) &g_threadInfo[nServerIndex],			// Param for thread
		0,												// Init flag
		&g_uiThrdSends[nServerIndex]);					// Thread address;
	g_hThrdConnects[nServerIndex]		 = (HANDLE)_beginthreadex(NULL,					// Security
		0,												// Stack size - use default
		ConnectThreadFunc,								// Thread fn entry point
		(void*) &g_threadInfo[nServerIndex],			// Param for thread
		0,												// Init flag
		&g_uiThrdConnects[nServerIndex]);				// Thread address;
	
	if(FALSE == g_pSockClients[nServerIndex]->Create(g_SockServer[nServerIndex].szIP, g_SockServer[nServerIndex].nPort, 10, 
		TRUE, GetHelloData(), GetHelloLength() + 1,
		10, 10)){
		g_pSockClients[nServerIndex]->Destroy();
		
		UwlLogFile(_T("Can not connect to %s."), lpszServerName);
		return FALSE;
	}

	UwlLogFile(_T("Connect to %s OK."), lpszServerName);
	
	
	return TRUE;
}