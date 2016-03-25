#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define	SERVER_INITIAL_RECVS	1			// recvs

#define	SERVER_INITIAL_ACCEPTS	300			// accepts

#define	DEF_KICKOFF_DEADTIME	360			// minutes

#define	DEF_KICKOFF_STIFFTIME	300			// seconds

#define	DEF_KICKOFF_STALETIME	(24*60*60)	// seconds

#define	PORT_OF_ROOMSVR			30640		// port

#define CONNECT_ROOMSVR_WAIT	5			// waittime(seconds)

#define	CONNECTS_TO_ROOMSVR		1			// connects

#define	LOCAL_ROOMSVR_IP		_T("127.0.0.1")

#define DEF_TIMER_INTERVAL_KICK		5		// cycle interval(seconds)

#define MIN_TIMER_INTERVAL_KICK		1		// seconds

#define DEF_KICK_ENTERBKG_WAIT		10		// kick_bkg wait(seconds)

#define MIN_KICK_ENTERBKG_WAIT		5		// seconds

#define DEF_KICK_OVER_WAIT			60		// kick_over wait(seconds)

#define MIN_KICK_OVER_WAIT			10		// seconds

CString  GetYxpdSvrINIFileName()
{
	CString sRet;
	TCHAR szSystemFile[MAX_PATH];
	GetSystemDirectory(szSystemFile,MAX_PATH);
	sRet.Format(_T("%s\\yxpdsvr.ini"),szSystemFile);
	return sRet;
}

BOOL IsFileExistEx(LPCTSTR szDir)
{
	LPWIN32_FIND_DATA   lpFindFileData   =   new   WIN32_FIND_DATA;   
	HANDLE   hfile   =  ::FindFirstFile(szDir,lpFindFileData);   
	delete   lpFindFileData;   
	BOOL bn;
	if(hfile==INVALID_HANDLE_VALUE)   
		bn= FALSE;
	else   
		bn= TRUE;
	::FindClose(hfile);   
	
	return bn;
}

BOOL BuildDirectory(CString   strPath,BOOL bnBulid)   
{   
	CString   strSubPath;   
	CString   strInfo;   
	int   nCount   =   0;     
	int   nIndex   =   0;   
    
	//查找字符"\\"的个数   
	do   
	{   
		nIndex   =   strPath.Find("\\",nIndex)   +   1;   
		if (nIndex!=strPath.GetLength())//省略最后一个
			nCount++;   
	}while(   (nIndex-1)   !=   -1);   
	nIndex   =   0;   
	
	//跳过盘符
	nIndex   =   strPath.Find("\\",nIndex)   +   1; 
	nCount--;
	//检查，并创建目录   
	while(   (nCount-1)   >=   0)   
	{   
		nIndex   =   strPath.Find("\\",nIndex)   +   1;   
		if(   (nIndex   -   1)   ==   -1)   
		{   
			strSubPath   =   strPath;   
		}   
		else   
			strSubPath   =   strPath.Left(nIndex-1);  
		
		if(!IsFileExistEx(strSubPath))   
		{   
			if (bnBulid)
			{
				if(!::CreateDirectory(strSubPath,NULL))   
				{    
					return   FALSE;   
				}  
			}
			else
			{
				return FALSE;
			}
		}   
		nCount--;   
	};   
	return   TRUE;   
} 

BOOL xygRegMessageWnd(int nGamePort, LPCTSTR pszClassName, WNDPROC pWndProc)
{
	TCHAR szGamePort[MAX_PATH];
	memset(szGamePort, 0, sizeof(szGamePort));
	TCHAR szClsName[MAX_PATH];
	memset(szClsName, 0, sizeof(szClsName));
	
	_stprintf(szGamePort, _T("%ld"), nGamePort);
	
	lstrcat(szClsName, pszClassName);
	lstrcat(szClsName, szGamePort);
	
	WNDCLASS clsSockWnd;
	
	ZeroMemory(&clsSockWnd, sizeof(clsSockWnd));
	
	clsSockWnd.hCursor = LoadCursor(NULL,IDC_ARROW);
	clsSockWnd.hIcon = NULL;
	clsSockWnd.lpszMenuName=NULL; // Don't want any menu
	clsSockWnd.lpszClassName = szClsName;
	clsSockWnd.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	clsSockWnd.hInstance = GetModuleHandle(NULL);
	clsSockWnd.style = 0;
	clsSockWnd.lpfnWndProc = (WNDPROC)pWndProc;
	clsSockWnd.cbWndExtra = 0;
	clsSockWnd.cbClsExtra = 0;
	
	if (!RegisterClass(&clsSockWnd)){
		UwlTrace(_T("RegisterClass() %s error. errcode: %ld, errstr: %s."), 
			szClsName, GetLastError(), UwlGetAPIErrStr(GetLastError()));
		UwlLogFile(_T("RegisterClass() %s error. errcode: %ld, errstr: %s."), 
			szClsName, GetLastError(), UwlGetAPIErrStr(GetLastError()));
		return FALSE;
	}else{
		//UwlLogFile(_T("RegisterClass() %s OK."), szClsName);
	}
	return TRUE;
}

void xygUnregMessageWnd(int nGamePort, LPCTSTR pszClassName)
{
	TCHAR szGamePort[MAX_PATH];
	memset(szGamePort, 0, sizeof(szGamePort));
	TCHAR szClsName[MAX_PATH];
	memset(szClsName, 0, sizeof(szClsName));
	
	_stprintf(szGamePort, _T("%ld"), nGamePort);
	
	lstrcat(szClsName, pszClassName);
	lstrcat(szClsName, szGamePort);
	
	UnregisterClass(szClsName, GetModuleHandle(NULL));
}

HWND xygCreateMessageWnd(int nGamePort, LPCTSTR pszClassName, LPCTSTR pszWindowName, LPCTSTR pszIniFile)
{
	TCHAR szGamePort[MAX_PATH];
	memset(szGamePort, 0, sizeof(szGamePort));
	TCHAR szClsName[MAX_PATH];
	memset(szClsName, 0, sizeof(szClsName));
	TCHAR szWndName[MAX_PATH];
	memset(szWndName, 0, sizeof(szWndName));
	
	_stprintf(szGamePort, _T("%ld"), nGamePort);
	
	lstrcat(szClsName, pszClassName);
	lstrcat(szClsName, szGamePort);
	lstrcat(szWndName, pszWindowName);
	lstrcat(szWndName, szGamePort);
	
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
		UwlTrace(_T("CreateWindowEx() classname: %s windowname: %s error. errcode: %ld, errstr: %s"), 
			szClsName, szWndName, GetLastError(), UwlGetAPIErrStr(GetLastError()));
		UwlLogFile(_T("CreateWindowEx() classname: %s windowname: %s error. errcode: %ld, errstr: %s"), 
			szClsName, szWndName, GetLastError(), UwlGetAPIErrStr(GetLastError()));
		return NULL;
	}else{
		//UwlLogFile(_T("CreateWindowEx() classname: %s windowname: %s OK."), szClsName, szWndName);
		if(pszIniFile){
			TCHAR szWndHandle[MAX_PATH];
			memset(szWndHandle, 0, sizeof(szWndHandle));
			_stprintf(szWndHandle, _T("%ld"), hWnd);
			WritePrivateProfileString(_T("listen"), _T("hwnd"), szWndHandle, pszIniFile);
		}
	}
	return hWnd;
}

static unsigned long g_seedsMakeCode[] = {
	0x9967769e, 0xbf4b5b1a, 0x786d70c8, 0xf7401577
};

int xygMakeHardID2Code(LPCTSTR szStr, int nLen, DWORD& dwCode)
{
	return UwlMakeOnewayCodeDWord(szStr, nLen, dwCode, g_seedsMakeCode);
}

CAssitServer::CAssitServer(const TCHAR* szLicenseFile,
				   const TCHAR* szProductName,
				   const TCHAR* szProductVer,
				   const int nListenPort, const int nGameID,
				   DWORD flagEncrypt, DWORD flagCompress)
			: CSockServer(KEY_GAMESVR_2_0, flagEncrypt, flagCompress)
{
	m_pSockClient  = new CSockClient(this, KEY_HALL, flagEncrypt, flagCompress);
	
	m_hExitServer  = NULL;	
	m_hThrdSend    = NULL;
	m_hThrdTimer   = NULL; 
	m_hThrdConnect = NULL;
	m_hThrdMessage = NULL;
	m_hThrdSvrPulse= NULL;
	
	lstrcpy(m_szChunkServer, _T("")); 
	lstrcpy(m_szConnect, _T(""));
	lstrcpy(m_szLicFile, szLicenseFile);
	lstrcpy(m_szIniFile, _T(""));
	lstrcpy(m_szProductName, szProductName);
	lstrcpy(m_szProductVer, szProductVer);
	m_nClientID = 0;
	
	ZeroMemory(&m_lic, sizeof(m_lic));
	
	m_nListenPort = nListenPort;
	m_nGameID     = nGameID;
	m_nScoreMult  = 1;
	
	m_uiThrdSend    = 0;
	m_uiThrdTimer   = 0;
	m_uiThrdConnect = 0;
	m_uiThrdMessage = 0;  
	m_uiThrdSvrPulse= 0;
	
	m_hGameSvrWnd = NULL;

	ZeroMemory(&m_SvrPulseInfo,sizeof(m_SvrPulseInfo));
	int nDate=0,nTime=0;
	UwlGetCurrentDateTime(nDate,nTime);
    m_SvrPulseInfo.nCurrentDate=nDate;
}

BOOL CAssitServer::CheckLicense()
{
	int nError = 0;	
	
	//正式ReleaseS版发布需要申请license
#ifndef _DEBUG
	if(lstrlen(m_szLicFile)){ // 
		if(!UwlCheckLicense(m_szLicFile, m_szProductName, m_szProductVer, nError, &m_lic)){
			CString strMsg;
			UwlGetLicErrMsg(nError, strMsg);
			UwlTrace(_T("license chunk error! code = %d, msg = %s."), nError, (LPCTSTR)strMsg);
			UwlLogFile(_T("license chunk error! code = %d, msg = %s."), nError, (LPCTSTR)strMsg);
			
			CString strErr;
			UwlGetErrMsg(ERR_SERVER_ABORTED, strErr);
			strErr += strMsg;
			UwlLogFile(_T("%s"), strErr);
			UwlShowServiceMsg(strErr, NULL, MB_OK);
			
			return FALSE;
		}
	}
#endif

	return TRUE;
}
BOOL CAssitServer::InitClientID()
{
	int nError=0;

	CString strClientID;
	if(!UwlGetLicFeature(m_szLicFile, _T("clientid"), strClientID, nError)){
		CString strMsg;
		UwlGetLicErrMsg(nError, strMsg);
		UwlTrace(_T("license clientid error! code = %d, msg = %s."), nError, (LPCTSTR)strMsg);
		UwlLogFile(_T("license clientid error! code = %d, msg = %s."), nError, (LPCTSTR)strMsg);
		
		CString strErr;
		UwlGetErrMsg(ERR_SERVER_ABORTED, strErr);
		strErr += strMsg;
		UwlLogFile(_T("%s"), strErr);
		//		UwlShowServiceMsg(strErr, NULL, MB_OK);
		return FALSE;
	}
	m_nClientID = atoi(LPCTSTR(strClientID));
	if(0 == m_nClientID)
	{
		UwlTrace(_T("invalid client id!"));
		UwlLogFile(_T("invalid client id!"));
		return FALSE;
	}
	else
	{
		UwlTrace(_T("client id=%d!"),m_nClientID);
		UwlLogFile(_T("client id=%d!"),m_nClientID);
	}
	
	return TRUE;
}
BOOL CAssitServer::InitIniFile()
{
	TCHAR szFullName[MAX_PATH];
	GetModuleFileName(GetModuleHandle(NULL), szFullName, sizeof(szFullName));
	
	UwlSplitPath(szFullName, SPLIT_DRIVE_DIR, m_szIniFile);
	lstrcat(m_szIniFile, m_szProductName);
	lstrcat(m_szIniFile, _T(".ini"));

	m_nClientID = GetPrivateProfileInt(_T("listen"), _T("clientid"), 0, m_szIniFile);	
	if(0 == m_nClientID)
	{
		UwlTrace(_T("invalid client id!"));
		UwlLogFile(_T("invalid client id!"));
		return FALSE;
	}
	else
	{
		UwlTrace(_T("client id=%d!"),m_nClientID);
		UwlLogFile(_T("client id=%d!"),m_nClientID);
	}
	
	return TRUE;
}
BOOL CAssitServer::GetChunkSvrIP()
{
	lstrcpy(m_szChunkServer, (""));
	GetPrivateProfileString(_T("ChunkServer"), _T("Name"), _T(""), m_szChunkServer, sizeof(m_szChunkServer), m_szIniFile);

	if(!lstrcmpi(m_szChunkServer, _T(""))){
		CString strMsg;
		UwlGetErrMsg(ERR_SERVER_NOTFOUND, strMsg);
		
		CString strErr;
		UwlGetErrMsg(ERR_SERVER_ABORTED, strErr);
		strErr += strMsg;
		UwlLogFile(_T("%s"), strErr);
		UwlShowServiceMsg(strErr, NULL, MB_OK);
		return FALSE;
	}

	UwlTrace(_T("chunk server name: %s"), m_szChunkServer);
	return TRUE;
}
BOOL CAssitServer::Initialize()
{	
	UwlLogFile(_T("server starting..."));

	// initialize ole environment
	if(S_FALSE == ::CoInitialize(NULL)) {
		return FALSE;
	}


	InitIniFile();

	if (!GetChunkSvrIP())
		return FALSE;
	
	m_hExitServer = CreateEvent(NULL, TRUE, FALSE, NULL);

			m_hThrdTimer = (HANDLE)_beginthreadex(NULL,			// Security
											0,						// Stack size - use default
											TimerThreadFunc,     	// Thread fn entry point
											(void*) this,			// Param for thread
											0,						// Init flag
											&m_uiThrdTimer);		// Thread address;	
			m_hThrdMessage = (HANDLE)_beginthreadex(NULL,		// Security
											0,						// Stack size - use default
											MessageThreadFunc,     	// Thread fn entry point
											(void*) this,			// Param for thread
											0,						// Init flag
											&m_uiThrdMessage);		// Thread address;

	if (m_pSockClient)
	{
		// create send and recv thread for client
		m_hThrdSend = (HANDLE)_beginthreadex(NULL,				// Security
											0,						// Stack size - use default
											SendThreadFunc,     	// Thread fn entry point
											(void*) this,			// Param for thread
											0,						// Init flag
											&m_uiThrdSend);			// Thread address;

		m_hThrdConnect = (HANDLE)_beginthreadex(NULL,			// Security
											0,						// Stack size - use default
											ConnectThreadFunc,     	// Thread fn entry point
											(void*) this,			// Param for thread
											0,						// Init flag
											&m_uiThrdConnect);			// Thread address;
		m_hThrdSvrPulse = (HANDLE)_beginthreadex(NULL,			// Security
											0,						// Stack size - use default
											ServerPulseThreadFunc,  // Thread fn entry point
											(void*) this,			// Param for thread
											0,						// Init flag
											&m_uiThrdSvrPulse);		// Thread address;

		if(FALSE == m_pSockClient->Create(m_szChunkServer, PORT_OF_CHUNKSVR, CONNECT_CHUNKSVR_WAIT, 
									TRUE, GetHelloData(), GetHelloLength()+1,
									CONNECTS_TO_CHUNKSVR, CLIENT_INITIAL_RECVS)){
			m_pSockClient->Destroy();

			UwlTrace(_T("Can not connect to chunk server."));
			UwlLogFile(_T("Can not connect to chunk server."));

			CString strMsg;
			UwlGetErrMsg(ERR_CONNECT_FAILED, strMsg);

			CString strErr;
			UwlGetErrMsg(ERR_SERVER_ABORTED, strErr);
			strErr += strMsg;
			UwlLogFile(_T("%s"), strErr);
			UwlShowServiceMsg(strErr, NULL, MB_OK);
			return FALSE;
		}
		UwlLogFile(_T("Connect to ChunkServer sucess. ip[%s],port[%d]"), m_szChunkServer,PORT_OF_CHUNKSVR);
		//validate assit server is authorized
		if(!ValidateClientEx())
		{
			UwlTrace(_T("Can not pass Client verify."));
			UwlLogFile(_T("Can not pass Client verify."));
			return FALSE;
		}
		ValidateClientInfo(m_nClientID);
	} 

	// get my ip that can listen
// 	int nError=0;
// 	CString strIP;
// 	UwlGetLicFeature(m_szLicFile, _T("IP"), strIP, nError);

	// Listen Game Connect
	if(CSockServer::Initialize("", m_nListenPort, GetHelloData(), GetHelloLength()+1, 
							   /*SERVER_INITIAL_ACCEPTS*/600, 1, /*MAX_OVERLAPPED_ACCEPTS*/1000)){
		UwlTrace(_T("Server startup OK."));
		UwlLogFile(_T("Server startup OK."));
		return TRUE;
    }else{
		return FALSE;
	}
}

void CAssitServer::Shutdown()
{
	SetEvent(m_hExitServer);
	
	PostThreadMessage(m_uiThrdSend, WM_QUIT, 0, 0);
	WaitForSingleObject(m_hThrdSend, WAITTIME_EXIT);
	if(m_hThrdSend){
		CloseHandle(m_hThrdSend);
		m_hThrdSend = NULL;
	}
	PostThreadMessage(m_uiThrdTimer, WM_QUIT, 0, 0);
	WaitForSingleObject(m_hThrdTimer, WAITTIME_EXIT);	
	if(m_hThrdTimer){
		CloseHandle(m_hThrdTimer);
		m_hThrdTimer = NULL;
	}
	
	PostThreadMessage(m_uiThrdConnect, WM_QUIT, 0, 0);
	WaitForSingleObject(m_hThrdConnect, WAITTIME_EXIT);
	if(m_hThrdConnect){
		CloseHandle(m_hThrdConnect);
		m_hThrdConnect = NULL;
	}
	PostThreadMessage(m_uiThrdMessage, WM_QUIT, 0, 0);
	WaitForSingleObject(m_hThrdMessage, WAITTIME_EXIT);
	if(m_hThrdMessage){
		CloseHandle(m_hThrdMessage);
		m_hThrdMessage = NULL;
	}
	PostThreadMessage(m_uiThrdSvrPulse, WM_QUIT, 0, 0);
	WaitForSingleObject(m_hThrdSvrPulse, WAITTIME_EXIT);	
	if(m_hThrdSvrPulse){
		CloseHandle(m_hThrdSvrPulse);
		m_hThrdSvrPulse = NULL;
	}
	
	if (m_pSockClient){ m_pSockClient->Destroy();} 
	
	CSockServer::Shutdown();
	
	if(m_hExitServer){
		CloseHandle(m_hExitServer);
		m_hExitServer = NULL;
	}
	::CoUninitialize();
	
	UwlLogFile(_T("server exited."));
} 

BOOL CAssitServer::BaseVerify(int userid, int gameid)
{
	if (userid <= 0 || gameid <= 0 || gameid >= MAX_GAME_NUM)
		return FALSE;

	return TRUE;
}

void CAssitServer::SendPulseData()
{
	if (!m_pSockClient) return;
	
	REQUEST request;
	memset(&request, 0, sizeof(request));
	REQUEST response;
	memset(&response, 0, sizeof(response));
	
	CONTEXT_HEAD context;
	memset(&context, 0, sizeof(context));
	context.hSocket = m_pSockClient->GetSocket();
	
	request.head.nRequest = GR_SEND_PULSE_EX;
	
	BOOL bSendOK = m_pSockClient->SendCast(&context, &request, &response);
}


int CAssitServer::ClearStaleUserData(DWORD dwSeconds)
{
	return 0;
// 	CDWordArray aryKeyToDel;
// 	{
// 		CAutoLock lock(&m_csUserData);
// 		
// 		int userid = 0;
// 		USER_DATA user_data;
// 		memset(&user_data, 0, sizeof(user_data));
// 		
// 		POSITION pos = m_mapUserData.GetStartPosition();
// 		while(pos){
// 			m_mapUserData.GetNextAssoc(pos, userid, user_data);
// 			if(user_data.nLastUpdate && dwSeconds){
// 				if(UwlGetCurrentSeconds() - user_data.nLastUpdate > dwSeconds){
// 					aryKeyToDel.Add(userid);
// 				}
// 			}
// 		}
// 		for(int i = 0; i < aryKeyToDel.GetSize(); i++){
// 			if(aryKeyToDel[i] > 0){
// 				m_mapUserData.RemoveKey(aryKeyToDel[i]);
// 				//RemoveSoloPlayer(aryKeyToDel[i]); //预防死锁
// 			}
// 		}
// 	}
// 	
// 	for(int i = 0; i < aryKeyToDel.GetSize(); i++){
// 		if(aryKeyToDel[i] > 0){
// 			//m_mapUserData.RemoveKey(aryKeyToDel[i]);
// 			RemoveSoloPlayer(aryKeyToDel[i]);
// 		}
// 	}
// 	
// 	return aryKeyToDel.GetSize();
}

BOOL  CAssitServer::SimulateReqFromUser(SOCKET hSocket, LONG lToken, UINT nRequest, int nDataLen, VOID* pData)
{
	LPCONTEXT_HEAD pContext = new CONTEXT_HEAD;
	ZeroMemory(pContext, sizeof(CONTEXT_HEAD));
	pContext->hSocket  =  hSocket;
	pContext->lTokenID =  lToken;
	pContext->dwFlags |= CH_FLAG_SYSTEM_EJECT;
	
	LPREQUEST pRequest = new REQUEST;
	ZeroMemory(pRequest, sizeof(REQUEST));
	
	PBYTE pNewData = new BYTE[nDataLen];
	memset(pNewData,0,nDataLen);
	memcpy(pNewData, pData, nDataLen);

	pRequest->head.nRequest = nRequest;
	pRequest->pDataPtr      = pNewData;
   	pRequest->nDataLen      = nDataLen;
	
	return PutRequestToWorker(pRequest->nDataLen, DWORD(pContext->hSocket), pContext, pRequest, pRequest->pDataPtr);
}

BOOL CAssitServer::OnConnectSocket(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{		
	{
		CAutoLock lock(&m_csTokenSock);
		m_mapTokenSock.SetAt(lpContext->lTokenID, lpContext->hSocket);
	}

	return TRUE;
}

unsigned CAssitServer::SendThreadFunc(LPVOID lpVoid)
{
	CAssitServer * pThread = (CAssitServer *) lpVoid;
	
    return pThread->SendThreadProc();
} 

unsigned CAssitServer::SendThreadProc()
{
	UwlTrace(_T("send thread started. id = %d"), GetCurrentThreadId());
	
	MSG msg;
	while (GetMessage(&msg, 0, 0, 0))
	{
		if(UM_DATA_TOSEND == msg.message)
		{
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
			
			UwlClearRequest(pRequest);
			SAFE_DELETE(pContext);
			SAFE_DELETE(pRequest);
			
			CONTEXT_HEAD context;
			memset(&context, 0, sizeof(context));
			
			BOOL bTimeout = FALSE;
			BOOL bSendOK = m_pSockClient->SendRequest(&context, &request, NULL, bTimeout);
			
			UwlClearRequest(&request);
		}else{
			DispatchMessage(&msg);
		}
	}
	UwlTrace(_T("send thread exiting. id = %d"), GetCurrentThreadId());
   	return 0;
} 

unsigned CAssitServer::ConnectThreadFunc(LPVOID lpVoid)
{
	CAssitServer * pThread = (CAssitServer *) lpVoid;

    return pThread->ConnectThreadProc();
} 

unsigned CAssitServer::ConnectThreadProc()
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
					m_pSockClient->CloseSockets();					
					GetPrivateProfileString(_T("ChunkServer"),_T("Name"),_T(""),m_szChunkServer,sizeof(m_szChunkServer),m_szIniFile);

					if(FALSE == m_pSockClient->BeginConnect(m_szChunkServer, PORT_OF_CHUNKSVR, 1))
					{
						UwlTrace(_T("Can not connect to chunk server."));
						UwlLogFile(_T("Can not connect to chunk server."));
						if(!m_hExitServer || 
							WAIT_OBJECT_0 == WaitForSingleObject(m_hExitServer, 0)){
							break;
						}
					}else{
						UwlTrace(_T("reconnect to chunk server OK."));
						UwlLogFile(_T("reconnect to chunk server OK."));
						ValidateClientInfo(m_nClientID);
						break;
					}
				}
			}
			else if(UR_RECONNECT_SVR == msg.wParam)
			{
				m_pSockClient->CloseSockets();	
				GetPrivateProfileString(_T("ChunkServer"),_T("Name"),_T(""),m_szChunkServer,sizeof(m_szChunkServer) ,m_szIniFile);	

				if(FALSE == m_pSockClient->BeginConnect(m_szChunkServer, PORT_OF_CHUNKSVR, 1))
				{
					UwlTrace(_T("Can not reconnect to chunk server as received cmd."));
					UwlLogFile(_T("Can not reconnect to chunk server as received cmd."));
				}else{
					UwlTrace(_T("reconnect to chunk server OK as received cmd."));
					UwlLogFile(_T("reconnect to chunk server OK as received cmd."));
					
					// validate client id
					ValidateClientInfo(m_nClientID);
				}
			}					
		}
		else
		{
			DispatchMessage(&msg);
		}
	}
	UwlTrace(_T("connect thread exiting. id = %d"), GetCurrentThreadId());
   	return 0;
}

unsigned CAssitServer::TimerThreadFunc(LPVOID lpVoid)
{
	CAssitServer * pThread = (CAssitServer *) lpVoid;

    return pThread->TimerThreadProc();
} 

unsigned CAssitServer::TimerThreadProc()
{
	UwlTrace(_T("timer thread started. id = %d"), GetCurrentThreadId());
	
	int sameHour = 0;
	int lastHour = -1;
	int cycleMin = 0;
	int dayhour = -1;

	while(TRUE){
		int interval = GetPrivateProfileInt(_T("Timer"), _T("Interval"), DEF_TIMER_INTERVAL, m_szIniFile);
		if(interval < MIN_TIMER_INTERVAL){ // 至少1分钟周期
			interval = MIN_TIMER_INTERVAL;
		}
		DWORD dwRet = WaitForSingleObject(m_hExitServer, interval * 60 * 1000);
		if(WAIT_OBJECT_0 == dwRet){ // exit event
			break;
		}
		if(WAIT_TIMEOUT == dwRet){ // timeout
			UwlTrace(_T("timer thread triggered. do something. interval = %ld minutes."), interval);
			
			SendPulseData();
			
			OnTimerTriggered(interval, sameHour, lastHour, cycleMin);

			OnHourTriggered(dayhour);
		}
	}
	UwlTrace(_T("timer thread exiting. id = %d"), GetCurrentThreadId());
   	return 0;
}

static LRESULT CALLBACK MessageWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
							{return DefWindowProc(hwnd, uMsg, wParam, lParam);}

unsigned CAssitServer::MessageThreadFunc(LPVOID lpVoid)
{
	CAssitServer * pThread = (CAssitServer *) lpVoid;

    return pThread->MessageThreadProc();
} 

unsigned CAssitServer::MessageThreadProc()
{
	UwlTrace(_T("message thread started. id = %d"), GetCurrentThreadId());
	
	xygRegMessageWnd(m_nListenPort, ASSITSVR_CLSNAME_EX, MessageWndProc);
	/*HWND hMsgWnd*/ m_hGameSvrWnd = xygCreateMessageWnd(m_nListenPort, ASSITSVR_CLSNAME_EX, ASSITSVR_WNDNAME_EX, m_szIniFile);
	if(!m_hGameSvrWnd){
		UwlLogFile(_T("can not create message windows. we exit."));
		return 0;
	}
	MSG msg;
	while (GetMessage(&msg, 0, 0, 0))
	{
		if(m_hGameSvrWnd != msg.hwnd) continue; //
		if (WM_RTG_RECONNECTSVR_EX==msg.message)
		{
			int adminid=int(msg.wParam);
			OnReconnectSvr(adminid);
		}else if(WM_RTG_HARDCODE_EX == msg.message){
			DWORD hardcode = DWORD(msg.wParam);
			int userid = int(msg.lParam);
			OnHardCode(userid, hardcode);
		}else if(WM_RTG_TOKENID_EX == msg.message){
			int tokenid = int(msg.wParam);
			int userid = int(msg.lParam);
			OnRoomTokenID(userid, tokenid);
		}else{
			DispatchMessage(&msg);
		}
	}
	if(m_hGameSvrWnd){
		DestroyWindow(m_hGameSvrWnd);
		m_hGameSvrWnd = NULL;
	}
	xygUnregMessageWnd(m_nListenPort, ASSITSVR_CLSNAME_EX);
	UwlTrace(_T("message thread exiting. id = %d"), GetCurrentThreadId());
   	return 0;
}

unsigned CAssitServer::ServerPulseThreadFunc(LPVOID lpVoid)
{
	CAssitServer * pThread = (CAssitServer *) lpVoid;
	
    return pThread->ServerPulseThreadProc();
} 

unsigned CAssitServer::ServerPulseThreadProc()
{
	UwlTrace(_T("serverpulse thread started. id = %d"), GetCurrentThreadId());
	
	//重连失败大概耗时31秒左右，这样定时间隔60秒，即可实现重连失败之后定时重连
	while(TRUE)
	{
		int interval = GetPrivateProfileInt(_T("ServerPulse"), _T("Interval"), DEF_SERVERPULSE_INTERVAL, m_szIniFile);
		DWORD dwRet = WaitForSingleObject(m_hExitServer, interval  * 1000);
		if(WAIT_OBJECT_0 == dwRet)// exit event
			break;

		if(WAIT_TIMEOUT == dwRet)// timeout
		{ 	
			int nCheck = GetPrivateProfileInt(_T("ServerPulse"), _T("Check"),1, m_szIniFile);//默认=1表示要检查
			if(nCheck==0)
				continue;
			
			CAutoLock lock(&m_csSvrPulseInfo);
			int nLatest=m_SvrPulseInfo.nLatestTime;
			
			if(nLatest!=0)
			{
				int nNow=UwlGetCurrentSeconds();
				if(nNow-nLatest > interval )
				{
					int nDate,nTime;
					UwlGetCurrentDateTime(nDate,nTime);
					
					if(m_SvrPulseInfo.nCurrentDate == nDate )
					{
						m_SvrPulseInfo.nReconnectCount ++;
					}
					else
					{
						m_SvrPulseInfo.nCurrentDate=nDate;
						m_SvrPulseInfo.nReconnectCount=0;
					}
					
					int nNowTick=GetTickCount();
					UwlLogFile(_T("[%u] Begin reconnect as server pulse lost [%d] 秒."),nNowTick,nNow-nLatest);
					
					PostThreadMessage(m_uiThrdConnect, UM_DATA_TOSEND, UR_RECONNECT_SVR, nNowTick);					
				}
			}
		}
	}
	
	UwlTrace(_T("serverpulse thread exiting. id = %d"), GetCurrentThreadId());
	UwlLogFile(_T("serverpulse thread exiting. id = %d"), GetCurrentThreadId());
	
   	return 0;
} 

BOOL CAssitServer::ValidateClientEx()
{
	REQUEST request;
	memset(&request, 0, sizeof(request));
	REQUEST response;
	memset(&response, 0, sizeof(response));
	
	CONTEXT_HEAD context;
	context.hSocket = m_pSockClient->GetSocket();
	context.lSession = 0;
	context.bNeedEcho = TRUE;
	
	VALIDATE_CLIENT_EX vce;
	ZeroMemory(&vce,sizeof(vce));
	vce.nClientID   = m_nGameID;
	vce.nClientType = CLIENT_TYPE_ASSIT; // 
	xyGetHardID(vce.szHardID);
	xyGetVolumeID(vce.szVolumeID);
	xyGetMachineID(vce.szMachineID);
	request.head.nRequest = GR_VALIDATE_CLIENT_EX;
	request.nDataLen = sizeof(vce);
	request.pDataPtr =&vce;
	
	BOOL bTimeout = FALSE;
	BOOL bSendOK = m_pSockClient->SendRequest(&context, &request, &response, bTimeout, 10000);
	if(!bSendOK || UR_FETCH_SUCCEEDED != response.head.nRequest)
	{
		UwlLogFile(_T("ValidateClientEx() failed!"));
		UwlClearRequest(&response);
		return FALSE;
	}
	
	UwlClearRequest(&response);
	UwlLogFile(_T("ValidateClientEx() OK!"));
	return TRUE;
}

BOOL CAssitServer::ValidateClientInfo(int nClientID)
{
	REQUEST request;
	memset(&request, 0, sizeof(request));
	REQUEST response;
	memset(&response, 0, sizeof(response));
	
	CONTEXT_HEAD context;
	memset(&context, 0, sizeof(context));
	context.hSocket = m_pSockClient->GetSocket();
	
	VALIDATE_CLIENT vc;
	ZeroMemory(&vc,sizeof(vc));
	vc.nClientID = nClientID; 
	vc.nClientType = CLIENT_TYPE_ASSIT; // 
	
	request.head.nRequest = GR_VALIDATE_CLIENT;
	request.nDataLen = sizeof(vc);
	request.pDataPtr = &vc;
	
	BOOL bTimeout = FALSE;
	BOOL bSendOK = m_pSockClient->SendRequest(&context, &request, &response,bTimeout);
	return bSendOK;
}  

BOOL CAssitServer::IsTimingOK(DWORD interval, int& sameHour, int& lastHour, int& cycleMin)
{
	int mode = GetPrivateProfileInt(
					_T("KickOff"),			// section name
					_T("Mode"),				// key name
					DEF_KICKOFF_MODE,		// default int
					m_szIniFile				// initialization file name
					);
	int timing = GetPrivateProfileInt(
					_T("KickOff"),			// section name
					_T("Timing"),			// key name
					DEF_KICKOFF_TIMING,		// default int
					m_szIniFile				// initialization file name
					);
	int elapse = GetPrivateProfileInt(
					_T("KickOff"),			// section name
					_T("Elapse"),			// key name
					DEF_KICKOFF_ELAPSE,		// default int
					m_szIniFile				// initialization file name
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
	return timingOK;
}

int CAssitServer::DoTimingWork()
{
	int count = ReleaseSockBufPool();
	UwlLogFile(_T("release server buffer pool done. count = %ld."), count);

	if (m_pSockClient)
	{
		count = m_pSockClient->ReleaseSockBufPool();
		UwlLogFile(_T("release client buffer pool done. count = %ld."), count);	
	}

	int deadtime = GetPrivateProfileInt(
						_T("KickOff"),			// section name
						_T("DeadTime"),			// key name
						DEF_KICKOFF_DEADTIME,	// default int
						m_szIniFile				// initialization file name
						);

	
	int stifftime = GetPrivateProfileInt(
						_T("KickOff"),			// section name
						_T("StiffTime"),		// key name
						DEF_KICKOFF_STIFFTIME,	// default int
						m_szIniFile				// initialization file name
						);
	count = CloseStiffs(stifftime);
	UwlLogFile(_T("CloseStiffs() called. stifftime = %ld seconds, stiffs = %ld."), stifftime, count);
	
	int staletime = GetPrivateProfileInt(
						_T("UserData"),			// section name
						_T("StaleTime"),		// key name
						DEF_KICKOFF_STALETIME,	// default int
						m_szIniFile				// initialization file name
						);
	count = ClearStaleUserData(staletime);
	UwlLogFile(_T("ClearStaleUserData() called. staletime = %ld seconds, count = %ld."), staletime, count);
	return 1;
}

int CAssitServer::OnTimerTriggered(DWORD interval, int& sameHour, int& lastHour, int& cycleMin)
{
	if(IsTimingOK(interval, sameHour, lastHour, cycleMin)){
		return DoTimingWork();
	}
	return 0;
}

void CAssitServer::OnHourTriggered(int& lastHour)
{
	SYSTEMTIME time;
	GetLocalTime(&time);
	
	if (lastHour < 0 
		|| time.wHour != lastHour)
	{
		//每小时记录一次..
		//添加在这里..
	}
	
	lastHour = time.wHour;
}


BOOL CAssitServer::OnRequest(void* lpParam1, void* lpParam2)
{
	try
	{
		LPCONTEXT_HEAD	lpContext = LPCONTEXT_HEAD	(lpParam1);
		LPREQUEST		lpRequest = LPREQUEST		(lpParam2);
		
		UwlTrace(_T("----------------------start of request process-------------------"));
		
#if defined(_UWL_TRACE) | defined(UWL_TRACE)
		DWORD dwTimeStart = GetTickCount();
#else
		DWORD dwTimeStart = 0;
#endif
		CWorkerContext* pThreadCxt = reinterpret_cast<CWorkerContext*>(GetWorkerContext());
		
		assert(lpContext && lpRequest);
		UwlTrace(_T("socket = %ld requesting..."), lpContext->hSocket);
		switch(lpRequest->head.nRequest){
			// admin reqs
		case UR_SOCKET_CONNECT:
			UwlTrace(_T("UR_SOCKET_CONNECT requesting..."));
			OnConnectSocket(lpContext, lpRequest, pThreadCxt);
			break;
		case GR_RECONNECT_SVR:
			UwlTrace(_T("GR_RECONNECT_SVR requesting..."));
			OnReconnectSvr(lpContext, lpRequest, pThreadCxt);
			break;	
			
			// client's request
		case GR_GAME_PULSE:
			UwlTrace(_T("GR_GAME_PULSE requesting..."));
			OnGamePulse(lpContext, lpRequest, pThreadCxt);
			break;
		case UR_SOCKET_CLOSE:
			UwlTrace(_T("UR_SOCKET_CLOSE requesting..."));
			OnCloseSocket(lpContext, lpRequest, pThreadCxt);
			break;
		// example
		case GR_GET_PROPINFO:
			UwlTrace(_T("GR_GET_PROPINFO requesting..."));
			OnGetPropInfo(lpContext, lpRequest, pThreadCxt);
			break; 
		case GR_USE_LUCKCARD:
			UwlTrace(_T("GR_USE_LUCKCARD requesting..."));
			OnUseLuckCard(lpContext, lpRequest, pThreadCxt);
			break;
		// example end

		case GR_QUERY_PLAYERLOGON:
			UwlTrace(_T("GR_QUERY_PLAYERLOGON requesting..."));
			OnQuerytUserLogon(lpContext, lpRequest, pThreadCxt);
			break; 
			// end
			
		default:
			UwlTrace(_T("goto default proceeding..."));
			CSockServer::OnRequest(lpParam1, lpParam2);
			break;
		}
		UwlClearRequest(lpRequest);
		
#if defined(_UWL_TRACE) | defined(UWL_TRACE)
		DWORD dwTimeEnd = GetTickCount();
#else
		DWORD dwTimeEnd = 0;
#endif
		UwlTrace(_T("request process time costs: %d ms"), dwTimeEnd - dwTimeStart);
		UwlTrace(_T("----------------------end of request process---------------------\r\n"));
	}
	catch (...)
	{
		LPREQUEST lpReq  = LPREQUEST(lpParam2);
   		UwlLogFile("catch error!!! reqid=%ld,subreqid=%ld,DataLen=%ld",lpReq->head.nRequest,lpReq->head.nSubReq,lpReq->nDataLen );
	}

	return TRUE;
}

BOOL CAssitServer::OnReconnectSvr(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
 	int nResult = UR_OPERATE_FAILED;
 
	LPRECONNECT_SVR lpReconnectSvr = LPRECONNECT_SVR(PBYTE(lpRequest->pDataPtr));
	
	DWORD dwCode = 0;
 	xygMakeHardID2Code(lpReconnectSvr->szHardID, lstrlen(lpReconnectSvr->szHardID), dwCode); 

	TCHAR szAdminID[32];
	memset(szAdminID, 0, sizeof(szAdminID));
	_stprintf(szAdminID, _T("%ld"), lpReconnectSvr->nAdminID);

	int nCode = GetPrivateProfileInt(_T("SysAdminCode"), szAdminID, 0, m_szIniFile);
    //if(dwCode == nCode){
		nResult = UR_OPERATE_SUCCEEDED;
	//}else{
	//	UwlLogFile(_T("reconnect cmd received from illeagal admin."));
	//}
  	REQUEST response;
	memset(&response, 0, sizeof(response));

	response.head.nRequest = nResult;
	response.pDataPtr =0;
	response.nDataLen =0;

	BOOL bSendOK = FALSE;

	CONTEXT_HEAD context;
	memcpy(&context, lpContext, sizeof(context));
	context.bNeedEcho = FALSE;
	
	bSendOK = SendResponse(lpContext->hSocket, &context, &response);
		

	if(nResult == UR_OPERATE_SUCCEEDED){
		if(lstrlen(lpReconnectSvr->szReconnectIP)){
			WritePrivateProfileString(_T("ChunkServer"), _T("Name"), lpReconnectSvr->szReconnectIP, m_szIniFile);
		}
		PostThreadMessage(m_uiThrdConnect, UM_DATA_TOSEND, UR_RECONNECT_SVR, 0);
	}

	return TRUE;
}

int CAssitServer::OnReconnectSvr(int adminid)
{
	UwlTrace(_T("AdminID %ld Require Reconnect to ChunkSvr!"),adminid);
	UwlLogFile(_T("AdminID %ld Require Reconnect to ChunkSvr!"),adminid);
	PostThreadMessage(m_uiThrdConnect, UM_DATA_TOSEND, UR_RECONNECT_SVR, 0);

	return 1;
}

int CAssitServer::OnHardCode(int userid, DWORD hardcode)
{
	return 0;
}

int CAssitServer::OnRoomTokenID(int userid, int roomtokenid)
{
	return 0;
}

int CAssitServer::OnExperience(int userid, int experience)
{
	return 0;
}

BOOL CAssitServer::OnChunkSendPulse(LPREQUEST lpRequest)
{
	CAutoLock lock(&m_csSvrPulseInfo);
	m_SvrPulseInfo.nLatestTime = UwlGetCurrentSeconds();

	return TRUE;
}

BOOL CAssitServer::TransmitRequest(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest)
{
	LPREQUEST pRequest = new REQUEST;
	memcpy(pRequest, lpRequest, sizeof(REQUEST));
	
	int nDataLen = lpRequest->nDataLen;
	pRequest->pDataPtr = new BYTE[nDataLen];
	pRequest->nDataLen = nDataLen;
	memcpy(pRequest->pDataPtr, lpRequest->pDataPtr, nDataLen);
	
	LPCONTEXT_HEAD pContext = new CONTEXT_HEAD;
	memcpy(pContext, lpContext, sizeof(CONTEXT_HEAD));
	
	if(!PostThreadMessage(m_uiThrdSend, UM_DATA_TOSEND, (WPARAM)pContext, (LPARAM)pRequest)){
		UwlClearRequest(pRequest);
		SAFE_DELETE(pRequest);
		SAFE_DELETE(pContext);
		return FALSE;
	}else{
		return TRUE;
	}
}

void CAssitServer::NotifyOneUserErrorInfo(LPCONTEXT_HEAD lpContext, LPCTSTR lpErroMsg)
{
	if (!lpContext || !lpErroMsg || lstrlen(lpErroMsg) <= 0)
		return;
	
	ERROR_INFO ei={0};	
	
	int len=lstrlen(lpErroMsg);
	int szSize = sizeof(ei.szMsg);
	
	if (len >= szSize){
		lstrcpyn(ei.szMsg, lpErroMsg, szSize);
	}else{
		lstrcpy(ei.szMsg, lpErroMsg);
	}	
	
	NotifyOneUser(lpContext->hSocket, lpContext->lTokenID, GR_ERROR_INFOMATION_EX, &ei, sizeof(ei));
}

BOOL CAssitServer::OnUserLogon(LPNTF_PLAYERLOGON lpPlayerLogon)
{
	NTF_PLAYERLOGON logon;
	ZeroMemory(&logon, sizeof(logon));
	memcpy(&logon, lpPlayerLogon, sizeof(logon));

#ifdef _DEBUG
// 	if (4526 == logon.nUserID) //for test
// 	{
// 		int x = 5;
// 	}
#endif
	
	InsertUserLogonMap(logon.nUserID, logon);

	return TRUE;
}

BOOL CAssitServer::OnUserLogoff(LPNTF_PLAYERLOGOFF lpPlayerLogoff)
{
	NTF_PLAYERLOGOFF logoff;
	ZeroMemory(&logoff, sizeof(logoff));
	memcpy(&logoff, lpPlayerLogoff, sizeof(logoff));
	
	EraseUserLogonMap(logoff.nUserID);

	return TRUE;
}

void CAssitServer::QueryUserLogon(LPCONTEXT_HEAD lpContext, int nUserID)
{
	QUERY_PLAYERLOGON query;
	ZeroMemory(&query, sizeof(query));

	query.nClientID=m_nClientID;
	query.nClientType=CLIENT_TYPE_ASSIT;
	query.nUserID=nUserID;
	SimulateReqFromUser(lpContext->hSocket,lpContext->lTokenID,GR_QUERY_PLAYERLOGON,sizeof(query),&query);
}

int CAssitServer::VerifyUserLogon(LPCONTEXT_HEAD lpContext ,int nUserID,LPCTSTR lpszHardID)
{
	CAutoLock lock(&m_csUserLogon);
	NTF_PLAYERLOGON npl;
	if (m_mapUserLogon.Lookup(nUserID, npl))
	{
		if (!lstrcmp(lpszHardID, npl.szHardID))
		{
			return TRUE;
		}
	}

	UwlTrace("Verify Failed! There's no logon info, userid=%d", nUserID);
	UwlLogFile("Verify Failed! There's no logon info, userid=%d", nUserID);

	return LOGON_NOUSERINFO;
}

BOOL CAssitServer::OnGamePulse(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	LPGAME_PULSE pGamePulse = LPGAME_PULSE(PBYTE(lpRequest->pDataPtr));
	int nUserID=pGamePulse->nUserID;
	
	UwlTrace(_T("nUser:%ld,Send Game Pulse"),nUserID);
	
// 	USER_DATA user_data;
// 	memset(&user_data,0,sizeof(USER_DATA));
// 	if (!LookupUserData(nUserID,user_data))
// 		return FALSE;
// 	
// 	SetUserData(nUserID,user_data,(*pGamePulse));
	
// 	CheckGameWin();
// 	CheckRoomInOpenTime(user_data.nRoomID);
	
	return TRUE;
}

BOOL CAssitServer::OnQuerytUserLogon(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{	
	LPQUERY_PLAYERLOGON pQuery = (LPQUERY_PLAYERLOGON)(lpRequest->pDataPtr);
	int clientid   = pQuery->nClientID;
	int clienttype = pQuery->nClientType;
	int userid     = pQuery->nUserID;
	SOCKET sock    = lpContext->hSocket;
	LONG token     = lpContext->lTokenID;
	
	TransmitRequest(lpContext, lpRequest);

	return TRUE;
}

BOOL CAssitServer::OnGetPropInfo(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	LPGET_PROP_INFO pGPI = (LPGET_PROP_INFO)(lpRequest->pDataPtr);
	int userid  = pGPI->nUserID;
	int gameid  = pGPI->nGameID;
	SOCKET sock = lpContext->hSocket;
	LONG token  = lpContext->lTokenID;

	if (!BaseVerify(userid, gameid) || gameid != m_nGameID)
	{
		UwlLogFile("GetPropInfo Failed! userid=%d gameid=%d ip=%s", 
			userid, gameid, xyConvertIPToStr(pGPI->dwIP));

		NotifyOneUserErrorInfo(lpContext, ASS_PARAM_NOTVERIFY);
		return FALSE;
	}

	int dwRet=VerifyUserLogon(lpContext,userid,pGPI->szHardID);
	if (dwRet<0)
	{
		UwlLogFile("GetPropInfo, verify logon failed! userid=%d ip=%s", 
			userid, xyConvertIPToStr(pGPI->dwIP));

		NotifyOneUserErrorInfo(lpContext, ASS_USER_NOTLOGON);
		if (LOGON_NOUSERINFO == dwRet)
		{
			QueryUserLogon(lpContext,userid);
		}
		return FALSE;
	}

	return TransmitRequest(lpContext, lpRequest);
}

BOOL CAssitServer::OnGetPropInfoRet(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClit, LPREQUEST lpReqFromSvr)
{
	LPGET_PROPINFO_OK lpGPI = (LPGET_PROPINFO_OK)( PBYTE(lpReqFromSvr->pDataPtr) + sizeof(CONTEXT_HEAD));
	int nLen     = lpReqFromSvr->nDataLen - sizeof(CONTEXT_HEAD);
	SOCKET sock  = lpContext->hSocket;
	LONG   token = lpContext->lTokenID;

	if (UR_FETCH_SUCCEEDED != lpReqFromSvr->head.nSubReq)
	{
		UwlTrace(_T("OnGetPropInfo failed! ERRCODE=%d"), lpReqFromSvr->head.nSubReq);
		UwlLogFile(_T("OnGetPropInfo failed! ERRCODE=%d"), lpReqFromSvr->head.nSubReq);
		NotifyOneUserErrorInfo(lpContext, ASS_GETPROP_FAIL);
		return FALSE;
	}

	UwlTrace(_T("OnGetPropInfo OK!"));

	GET_PROPINFO_OK gpi;
	memset(&gpi,0,sizeof(gpi));
	memcpy(&gpi, lpGPI, nLen);
	NotifyOneUser(sock, token, GR_GET_PROPINFO, &gpi, sizeof(gpi));
	
	return TRUE;	
}

BOOL CAssitServer::OnUseLuckCard(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{
	LPUSE_LUCKCARD pUL = (LPUSE_LUCKCARD)(lpRequest->pDataPtr);
	int userid  = pUL->nUserID;
	int gameid  = pUL->nGameID;
	SOCKET sock = lpContext->hSocket;
	LONG token  = lpContext->lTokenID;
	
	if (!BaseVerify(userid, gameid) || gameid != m_nGameID)
	{
		UwlLogFile("UseLuckCard Failed! userid=%d gameid=%d ip=%s", 
			userid, gameid, xyConvertIPToStr(pUL->dwIP)); 

		NotifyOneUserErrorInfo(lpContext, ASS_PARAM_NOTVERIFY);
		return FALSE;
	}
	
	int dwRet=VerifyUserLogon(lpContext,userid,pUL->szHardID);
	if (dwRet<0)
	{
		UwlLogFile("UseLuckCard, verify logon failed! userid=%d ip=%s", 
			userid, xyConvertIPToStr(pUL->dwIP));

		NotifyOneUserErrorInfo(lpContext, ASS_USER_NOTLOGON);
		if (LOGON_NOUSERINFO == dwRet)
		{
			QueryUserLogon(lpContext,userid);
		}
		return FALSE;
	}
	
	return TransmitRequest(lpContext, lpRequest);
}

BOOL CAssitServer::OnUseLuckCardRet(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClit, LPREQUEST lpReqFromSvr)
{
	LPUSE_LUCKCARD_OK pULC = (LPUSE_LUCKCARD_OK)( PBYTE(lpReqFromSvr->pDataPtr) + sizeof(CONTEXT_HEAD));
	int nLen     = lpReqFromSvr->nDataLen - sizeof(CONTEXT_HEAD);
	SOCKET sock  = lpContext->hSocket;
	LONG   token = lpContext->lTokenID;
	
	if (UR_OPERATE_SUCCEEDED != lpReqFromSvr->head.nSubReq)
	{
		UwlTrace(_T("OnUseLuckCard failed! ERRCODE=%d"), lpReqFromSvr->head.nSubReq);
		UwlLogFile(_T("OnUseLuckCard failed! ERRCODE=%d"), lpReqFromSvr->head.nSubReq);

		if (UR_LUCKCARD_ZERO == lpReqFromSvr->head.nSubReq)
		{
			NotifyOneUserErrorInfo(lpContext, ASS_LUCKYCARD_ZERO);
		}
		else
		{
			NotifyOneUserErrorInfo(lpContext, ASS_USECARD_FAIL);			
		}
		return FALSE;
	}

	UwlTrace(_T("OnUseLuckCard OK!"));

	USE_LUCKCARD_OK ulc;
	memset(&ulc,0,sizeof(ulc));
	memcpy(&ulc,pULC,nLen);
	NotifyOneUser(sock,token,GR_USE_LUCKCARD,&ulc,sizeof(ulc));
	
	return TRUE;
}

BOOL CAssitServer::OnCloseSocket(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt)
{	
	SOCKET sock = lpContext->hSocket;
	LONG token  = lpContext->lTokenID;
	int userid  = 0;
	
	CloseClient(sock, token);

	{
		CAutoLock lock(&m_csTokenSock);
		SOCKET sock = 0;
		if(m_mapTokenSock.Lookup(lpContext->lTokenID, sock)){
			if(sock == lpContext->hSocket){
				m_mapTokenSock.RemoveKey(lpContext->lTokenID);
			}
		}	
	}

	return TRUE;
}

void CAssitServer::InsertUserLogonMap(int userid, NTF_PLAYERLOGON& ntfLogon)
{
	CAutoLock lock(&m_csUserLogon);
	m_mapUserLogon.SetAt(userid, ntfLogon);
}

void CAssitServer::EraseUserLogonMap(int userid)
{
	CAutoLock lock(&m_csUserLogon);
	m_mapUserLogon.RemoveKey(userid);
}


SOCKET CAssitServer::FindSocket(LONG token)
{
	SOCKET sock = 0;
	
	CAutoLock lock(&m_csTokenSock);
	
	m_mapTokenSock.Lookup(token, sock);
	
	return sock;
}