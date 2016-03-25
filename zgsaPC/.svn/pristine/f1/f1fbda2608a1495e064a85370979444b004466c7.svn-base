// Main.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

int					g_nClientID = 0;

SOCK_SERVER			g_SockServer[MAX_TOTALSVR_COUNT];//服务器列表




TCHAR				g_szLicFile[MAX_PATH];//许可证文件
TCHAR				g_szIniFile[MAX_PATH];//配置文件
TCHAR				g_szDBAFile[MAX_PATH];//数据库帐密文件
 //


CStringLONGMap      g_mapGameSvrToken;
CGameSvrTokenMap	g_mapClientTokenG;
CCritSec			g_csClientTokenG;

CClientTokenMap		g_mapRoomSvrToken;
CCritSec			g_csRoomSvrToken;

CClientTokenMap		g_mapAssitSvrToken;
CCritSec			g_csAssitSvrToken;

CTokenSockMap		g_mapTokenSock;
CCritSec			g_csTokenSock;

CClientTokenMap		g_mapChunkLogToken;
CCritSec			g_csChunkLogToken;

UINT				g_uiThrdTimer = 0;
UINT				g_uiThrdClientPulse=0;

UINT				g_uiThrdSends[MAX_TOTALSVR_COUNT];
UINT				g_uiThrdConnects[MAX_TOTALSVR_COUNT];

CSockServer*		g_pSockServer = NULL;
CSockClient*		g_pSockClients[MAX_TOTALSVR_COUNT];
CSockClient*		g_pSockClientOnline;
int					g_nTotalSvrCount = 0;
int					g_nSubSvrCount=0;

HANDLE				g_hExitServer = NULL;
HANDLE				g_hThrdTimer = NULL;
HANDLE				g_hThrdClientPulse = NULL;

HANDLE				g_hThrdSends[MAX_TOTALSVR_COUNT];
HANDLE				g_hThrdConnects[MAX_TOTALSVR_COUNT];


THREAD_INFO			g_threadInfo[MAX_TOTALSVR_COUNT];

CHUNK_DB			g_ChunkDB[MAX_TOTALDB_COUNT];


// 唯一的应用程序对象
CWinApp				theApp;

HINSTANCE			g_hResDll = NULL;

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// 初始化 MFC 并在失败时显示错误
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0)){
		// TODO: 更改错误代码以符合您的需要
		MessageBox(NULL, _T("Fatal error: MFC initialization failed!\n"), APPLICATION_TITLE, MB_ICONSTOP);
		nRetCode = -1;
		return nRetCode;
	}

	if (!UwlInit()){
		// TODO: 更改错误代码以符合您的需要
		MessageBox(NULL, _T("Fatal error: UWL initialization failed!\n"), APPLICATION_TITLE, MB_ICONSTOP);
		nRetCode = -1;
		return nRetCode;
	}

	DWORD dwTraceMode = UWL_TRACE_DATETIME | UWL_TRACE_FILELINE | UWL_TRACE_NOTFULLPATH
						| UWL_TRACE_FORCERETURN | UWL_TRACE_CONSOLE;
						//| UWL_TRACE_FORCERETURN | UWL_TRACE_DUMPFILE | UWL_TRACE_CONSOLE;

	UwlBeginTrace((TCHAR*)AfxGetAppName(), dwTraceMode);
	UwlBeginLog((TCHAR*)AfxGetAppName());

	if(!AfxSocketInit()){
		MessageBox(NULL, _T("Fatal error: Failed to initialize sockets!\n"), APPLICATION_TITLE, MB_ICONSTOP);
		nRetCode = -1;
		return nRetCode;
	}


#ifdef UWL_SERVICE

	CMainService MainService(STR_SERVICE_NAME, STR_DISPLAY_NAME, 0, 0);
    
    if (!MainService.ParseStandardArgs(argc, argv)) {
        // Didn't find any standard args so start the service
        // Uncomment the DebugBreak line below to enter the debugger when the service is started.
        //DebugBreak();
        MainService.StartService();
    }
	// When we get here, the service has been stopped
    nRetCode = MainService.m_Status.dwWin32ExitCode;
#else
	
	CMainServer MainServer;
		
	if(FALSE == MainServer.Initialize()){
		UwlTrace(_T("server initialize failed!"));
	}

	UwlTrace( "Type 'q' when you want to exit. " );
	TCHAR ch;
	do{
	  ch = _getch();
	  ch = toupper( ch );

#if 1 
	  //For test 
	  //模拟请求
		if ('A' == ch)
		{
			CONTEXT_HEAD* lpContext = new CONTEXT_HEAD;

			USE_LUCKCARD* pl = new USE_LUCKCARD;
			ZeroMemory(pl, sizeof(USE_LUCKCARD));
			pl->nGameID = GAME_ID;
			pl->nUserID = 4526;
			g_pSockServer->PutToServer(lpContext, GR_USE_LUCKCARD, pl, sizeof(USE_LUCKCARD));
		}
#endif

	} while( ch != 'Q' );

	MainServer.Shutdown();

	nRetCode = 1;
#endif
	
	if(g_hResDll){
		AfxFreeLibrary(g_hResDll);
	}
	UwlEndLog();
	UwlEndTrace();
	UwlTerm();
	return nRetCode;
}
 

SOCKET FindSocket(LONG token)
{
	SOCKET sock = 0;

	CAutoLock lock(&g_csTokenSock);

	g_mapTokenSock.Lookup(token, sock);

	return sock;
}

void CopyMapGameSvrToken(CStringLONGMap& mapGameSvrToken)
{
	CAutoLock lock(&g_csClientTokenG);

	CString sKey;
	LONG token = 0;
	POSITION pos = g_mapGameSvrToken.GetStartPosition();
	while (pos)	{
		g_mapGameSvrToken.GetNextAssoc(pos, sKey, token);
		mapGameSvrToken.SetAt(sKey, token);
	}

}

void CopyMapClientTokenG(CClientTokenMap& mapClientToken, int gameid)
{
	CAutoLock lock(&g_csClientTokenG);

	CClientTokenMap* pClientTokenMap = NULL;
	if(g_mapClientTokenG.Lookup(gameid, pClientTokenMap) && pClientTokenMap){
		int client = 0;
		LONG token = 0;
		POSITION pos = pClientTokenMap->GetStartPosition();
		while (pos)	{
			pClientTokenMap->GetNextAssoc(pos, client, token);
			mapClientToken.SetAt(client, token);
		}
	}
}

void CopyMapAllTokenSock(CTokenSockMap& mapTokenSock)
{
	CAutoLock lock(&g_csTokenSock);
	SOCKET sock = 0;
	LONG token = 0;
	POSITION pos = g_mapTokenSock.GetStartPosition();
	while (pos)	
	{	
		g_mapTokenSock.GetNextAssoc(pos, token, sock);
		mapTokenSock.SetAt(token,sock);
	}
}

void SendPulseData(int svrindex)
{
	REQUEST request;
	memset(&request, 0, sizeof(request));
	REQUEST response;
	memset(&response, 0, sizeof(response));

	CONTEXT_HEAD context;
	memset(&context, 0, sizeof(context));
	context.hSocket = g_pSockClients[svrindex]->GetSocket();
	
	request.head.nRequest = GR_SEND_PULSE;

	BOOL bSendOK = g_pSockClients[svrindex]->SendCast(&context, &request, &response);
}

BOOL ValidateClientInfo(int svrindex, int nClientID)
{
	REQUEST request;
	memset(&request, 0, sizeof(request));
	REQUEST response;
	memset(&response, 0, sizeof(response));

	CONTEXT_HEAD context;
	context.hSocket = g_pSockClients[svrindex]->GetSocket();
	context.lSession = 0;
	context.bNeedEcho = 0;
	
	VALIDATE_CLIENT validateclient;
	ZeroMemory(&validateclient,sizeof(validateclient));

	validateclient.nClientID = nClientID; // 
	validateclient.nClientType = CLIENT_TYPE_CHUNK; // 
	
	request.head.nRequest = GR_VALIDATE_CLIENT;
	request.nDataLen = sizeof(validateclient);
	request.pDataPtr = &validateclient;

	BOOL bTimeout = FALSE;
	BOOL bSendOK = g_pSockClients[svrindex]->SendRequest(&context, &request, &response, 
									bTimeout);
	return bSendOK;
}
 
BOOL NotifyClientsG(UINT nRequest, void* pData, int nLen, int gameid, BOOL toCompress)
{
	CClientTokenMap mapClientToken;
	CopyMapClientTokenG(mapClientToken, gameid);
	
	BOOL bSendOK = FALSE;

	CONTEXT_HEAD context;
	memset(&context, 0, sizeof(context));
	context.bNeedEcho = FALSE;
	
	REQUEST request;
	memset(&request, 0, sizeof(request));

	request.head.nRequest = nRequest;

	request.pDataPtr = pData;
	request.nDataLen = nLen;
	
	int client = 0;
	SOCKET sock = 0;
	LONG token = 0;
	POSITION pos = mapClientToken.GetStartPosition();
	while (pos)	{
		mapClientToken.GetNextAssoc(pos, client, token);
		sock = FindSocket(token);
		if(sock){
			context.hSocket = sock;
			context.lTokenID = token;
			DWORD dwEncrypt = g_pSockServer->m_flagEncrypt;
			DWORD dwCompress = toCompress ? COMPRESS_ZIP : 0;
			bSendOK = g_pSockServer->SendRequest(context.hSocket, &context, &request,
								MSG_REQUEST, dwEncrypt, dwCompress);
		}
	}
	return TRUE;
}

BOOL NotifyAllGameSvr(UINT nRequest, void* pData, int nLen ,BOOL toCompress)
{
	CStringLONGMap mapGameSvrToken;
	CopyMapGameSvrToken(mapGameSvrToken);

	BOOL bSendOK = FALSE;

	CONTEXT_HEAD context;
	memset(&context, 0, sizeof(context));
	context.bNeedEcho = FALSE;
	
	REQUEST request;
	memset(&request, 0, sizeof(request));
 	request.head.nRequest = nRequest;
 	request.pDataPtr = pData;
	request.nDataLen = nLen;
	
	CString sKey;
	SOCKET sock = 0;
	LONG token = 0;
	POSITION pos = mapGameSvrToken.GetStartPosition();
	while (pos)	
	{
		mapGameSvrToken.GetNextAssoc(pos, sKey, token);
		sock = FindSocket(token);
		if(sock)
		{
			context.hSocket = sock;
			context.lTokenID = token;
			DWORD dwEncrypt = g_pSockServer->m_flagEncrypt;
			DWORD dwCompress = toCompress ? COMPRESS_ZIP : 0;
			bSendOK = g_pSockServer->SendRequest(context.hSocket, &context, &request,MSG_REQUEST, dwEncrypt, dwCompress);
		}
	}
	return TRUE;
}

BOOL NotifyOneGameSvr(UINT nRequest, void* pData, int nLen, LONG token)
{
 	BOOL bSendOK = FALSE;

	CONTEXT_HEAD context;
	memset(&context, 0, sizeof(context));
	context.bNeedEcho = FALSE;
	
	REQUEST request;
	memset(&request, 0, sizeof(request));
 	request.head.nRequest = nRequest;
 	request.pDataPtr = pData;
	request.nDataLen = nLen;
	
	SOCKET sock =  FindSocket(token);
 	if(sock)
	{
		context.hSocket = sock;
		context.lTokenID = token;
		DWORD dwEncrypt = g_pSockServer->m_flagEncrypt;
 		bSendOK = g_pSockServer->SendRequest(context.hSocket, &context, &request,MSG_REQUEST, dwEncrypt, 0);
	}
	return bSendOK;
}


BOOL NotifyHubSvr(UINT nRequest, void* pData, int nLen)
{
	return NotifyServer(nRequest,pData,nLen,0);
}

BOOL NotifyServer(UINT nRequest, void* pData, int nLen, int svrindex)
{
	LPREQUEST pRequest = new REQUEST;
	memset(pRequest, 0, sizeof(REQUEST));

	pRequest->head.nRequest = nRequest;

	pRequest->pDataPtr = new BYTE[nLen];
	pRequest->nDataLen = nLen;
	memcpy(pRequest->pDataPtr, pData, nLen);

	if(!PostThreadMessage(g_uiThrdSends[svrindex], UM_DATA_TOSEND, 0, (LPARAM)pRequest)){
		UwlClearRequest(pRequest);
		SAFE_DELETE(pRequest);
		return FALSE;
	}else{
		return TRUE;
	}
}

BOOL NotifyServerEx(UINT nRequest,int nSubReq, void* pData, int nLen, int svrindex)
{
	LPREQUEST pRequest = new REQUEST;
	memset(pRequest, 0, sizeof(REQUEST));

	pRequest->head.nRequest = nRequest;
	pRequest->head.nSubReq = nSubReq;

	pRequest->pDataPtr = new BYTE[nLen];
	pRequest->nDataLen = nLen;
	memcpy(pRequest->pDataPtr, pData, nLen);

	if(!PostThreadMessage(g_uiThrdSends[svrindex], UM_DATA_TOSEND, 0, (LPARAM)pRequest)){
		UwlClearRequest(pRequest);
		SAFE_DELETE(pRequest);
		return FALSE;
	}else{
		return TRUE;
	}
}

void ClearGameSvrTokenMap()
{
	CAutoLock lock(&g_csClientTokenG);

	int gameid = 0;
	CClientTokenMap* pMap = NULL;
	POSITION pos = g_mapClientTokenG.GetStartPosition();
	while (pos)	{
		g_mapClientTokenG.GetNextAssoc(pos, gameid, pMap);
		if(pMap){
		}
		SAFE_DELETE(pMap);
	}
	g_mapClientTokenG.RemoveAll();

	return;
}

BOOL IsInPeriodOfProfile(LPCTSTR lpSection, int nBegin, int nEnd)
{
	int begin = GetPrivateProfileInt(
					lpSection,			// section name
					_T("begin"),		// key name
					nBegin,				// default int
					g_szIniFile			// initialization file name
					);
	int end = GetPrivateProfileInt(
					lpSection,			// section name
					_T("end"),			// key name
					nEnd,				// default int
					g_szIniFile			// initialization file name
					);
	if(0 == begin && 0 == end) return FALSE;
	int curDate = 0;
	int curTime = 0;
	int result = UwlGetCurrentDateTime(curDate, curTime);
	curTime /= 100;
	if(curTime > begin && curTime < end){
		return TRUE;
	}else{
		return FALSE;
	}
}

int CalcSubIndexByUserID(int userid)
{
	return 0;// 连接OnlineSvr了已经

	int nCount=0;
	int nRemain=userid % g_nSubSvrCount;
 
	for(int i=1;i<g_nTotalSvrCount;i++)
	{
		if(g_SockServer[i].nType==TYPE_SUB_SERVER)
 			nCount++;
		if(nCount==nRemain+1)
			return i;
	}
	return 0;
}
 
void ReportDBErrorLog(_com_error &e, LPCTSTR lpszSql)
{
	/*
	{
		CAutoLock lock(&g_csDBMonitor);
		g_dbMonitor.nDBErrCount += 1;
	}
	
	int nNoLog = GetPrivateProfileInt(_T("LogDBError"), _T("disable"),0, g_szIniFile);
	if (nNoLog>0)
		return;
	
	if (!lpszSql)
		return;
	
	DB_ERROR_LOG del;
	ZeroMemory(&del, sizeof(del));
	
	del.nClientID = g_nClientID;
	del.nClientType = CLIENT_TYPE_CHUNK;
	sprintf(del.szErrCode, _T("%08lx"), e.Error());
	del.nSqlLen = sprintf(del.szSQLString, _T("%s"), lpszSql) + 1;
	
	NotifyHubSvr(GR_DB_ERROR_LOG, &del, sizeof(del)-(MAX_SQL_LENGTH-del.nSqlLen));
	*/
}

LONG FindRoomToken(int clientid)
{
	LONG token = 0;
	
	CAutoLock lock(&g_csRoomSvrToken);
	
	g_mapRoomSvrToken.Lookup(clientid, token);
	
	return token;
}

BOOL NotifyOneRoomSvr(UINT nRequest, void* pData, int nLen, int clientid, BOOL toCompress)
{	
	BOOL bSendOK = FALSE;
	
	CONTEXT_HEAD context;
	memset(&context, 0, sizeof(context));
	context.bNeedEcho = FALSE;
	
	REQUEST request;
	memset(&request, 0, sizeof(request));
	
	request.head.nRequest = nRequest;
	
	request.pDataPtr = pData;
	request.nDataLen = nLen;
	
	LONG token = FindRoomToken(clientid);
	if(token){
		SOCKET sock = FindSocket(token);
		if(sock){
			context.hSocket = sock;
			context.lTokenID = token;
			DWORD dwEncrypt = g_pSockServer->m_flagEncrypt;
			DWORD dwCompress = toCompress ? COMPRESS_ZIP : 0;
			bSendOK = g_pSockServer->SendRequest(context.hSocket, &context, &request,
				MSG_REQUEST, dwEncrypt, dwCompress);
		}
	}
	return TRUE;
}

BOOL NotifyRoomPlayer(UINT nRequest, void* pData, int nLen, int clientid,int roomid, int playerid)
{	
	DWORD flagCompress = 0;
	//	if(nRequest==GR_MATCH_DETAIL)//暂定为需要压缩
	//		flagCompress=COMPRESS_ZIP;
	
	BOOL bSendOK = FALSE;
	
	CONTEXT_HEAD context;
	memset(&context, 0, sizeof(context));
	context.bNeedEcho = FALSE;
	
	REQUEST request;
	memset(&request, 0, sizeof(request));
	
	request.head.nRequest = nRequest;
	
	request.head.nValue = playerid;
	request.head.wParam = roomid;
	
	request.pDataPtr = pData;
	request.nDataLen = nLen;
	
	LONG token = FindRoomToken(clientid);
	if(token){
		SOCKET sock = FindSocket(token);
		if(sock){
			context.hSocket = sock;
			context.lTokenID = token;
			bSendOK = g_pSockServer->SendRequest(context.hSocket, &context, &request,MSG_REQUEST,0,flagCompress);
			return bSendOK;
		}
	}
	return FALSE;
}

void CopyMapRoomSvrToken(CClientTokenMap& mapClientToken)
{
	CAutoLock lock(&g_csRoomSvrToken);
	
	int client = 0;
	LONG token = 0;
	POSITION pos = g_mapRoomSvrToken.GetStartPosition();
	while (pos)	{
		g_mapRoomSvrToken.GetNextAssoc(pos, client, token);
		mapClientToken.SetAt(client, token);
	}
}


BOOL NotifyAllRoomSvr(UINT nRequest, void* pData, int nLen, int exceptClient, BOOL toCompress)
{
	CClientTokenMap mapClientToken;
	CopyMapRoomSvrToken(mapClientToken);
	
	BOOL bSendOK = FALSE;
	
	CONTEXT_HEAD context;
	memset(&context, 0, sizeof(context));
	context.bNeedEcho = FALSE;
	
	REQUEST request;
	memset(&request, 0, sizeof(request));
	
	request.head.nRequest = nRequest;
	
	request.pDataPtr = pData;
	request.nDataLen = nLen;
	
	int client = 0;
	SOCKET sock = 0;
	LONG token = 0;
	POSITION pos = mapClientToken.GetStartPosition();
	while (pos)	{
		mapClientToken.GetNextAssoc(pos, client, token);
		if(client == exceptClient) continue;
		sock = FindSocket(token);
		if(sock){
			context.hSocket = sock;
			context.lTokenID = token;
			DWORD dwEncrypt = g_pSockServer->m_flagEncrypt;
			DWORD dwCompress = toCompress ? COMPRESS_ZIP : 0;
			bSendOK = g_pSockServer->SendRequest(context.hSocket, &context, &request,
				MSG_REQUEST, dwEncrypt, dwCompress);
		}
	}
	return TRUE;
}

LONG FindAssitToken(int clientid)
{
	LONG token = 0;
	
	CAutoLock lock(&g_csAssitSvrToken);
	
	g_mapAssitSvrToken.Lookup(clientid, token);
	
	return token;
}

BOOL NotifyOneAssitSvr(UINT nRequest, void* pData, int nLen, int clientid, BOOL toCompress)
{	
	BOOL bSendOK = FALSE;
	
	CONTEXT_HEAD context;
	memset(&context, 0, sizeof(context));
	context.bNeedEcho = FALSE;
	
	REQUEST request;
	memset(&request, 0, sizeof(request));
	request.head.nRequest = nRequest;
	request.pDataPtr = pData;
	request.nDataLen = nLen;
	
	LONG token = FindAssitToken(clientid);
	if(token){
		SOCKET sock = FindSocket(token);
		if(sock){
			context.hSocket = sock;
			context.lTokenID = token;
			DWORD dwEncrypt = g_pSockServer->m_flagEncrypt;
			DWORD dwCompress = toCompress ? COMPRESS_ZIP : 0;
			bSendOK = g_pSockServer->SendRequest(context.hSocket, &context, &request,
				MSG_REQUEST, dwEncrypt, dwCompress);
		}
	}
	return TRUE;
}

void CopyMapAssitSvrToken(CClientTokenMap& mapClientToken)
{
	CAutoLock lock(&g_csAssitSvrToken);
	
	int client = 0;
	LONG token = 0;
	POSITION pos = g_mapAssitSvrToken.GetStartPosition();
	while (pos)	{
		g_mapAssitSvrToken.GetNextAssoc(pos, client, token);
		mapClientToken.SetAt(client, token);
	}
}

BOOL NotifyAllAssitSvr(UINT nRequest, void* pData, int nLen, int exceptClient, BOOL toCompress)
{
	CClientTokenMap mapClientToken;
	CopyMapAssitSvrToken(mapClientToken);
	
	BOOL bSendOK = FALSE;
	
	CONTEXT_HEAD context;
	memset(&context, 0, sizeof(context));
	context.bNeedEcho = FALSE;
	
	REQUEST request;
	memset(&request, 0, sizeof(request));
	
	request.head.nRequest = nRequest;
	
	request.pDataPtr = pData;
	request.nDataLen = nLen;
	
	int client = 0;
	SOCKET sock = 0;
	LONG token = 0;
	POSITION pos = mapClientToken.GetStartPosition();
	while (pos)	{
		mapClientToken.GetNextAssoc(pos, client, token);
		if(client == exceptClient) continue;
		sock = FindSocket(token);
		if(sock){
			context.hSocket = sock;
			context.lTokenID = token;
			DWORD dwEncrypt = g_pSockServer->m_flagEncrypt;
			DWORD dwCompress = toCompress ? COMPRESS_ZIP : 0;
			bSendOK = g_pSockServer->SendRequest(context.hSocket, &context, &request,
				MSG_REQUEST, dwEncrypt, dwCompress);
		}
	}
	return TRUE;
}

BOOL RSSPlayerLogOnOff(int svrindex, int nClientID)
{
	REQUEST request;
	memset(&request, 0, sizeof(request));
	REQUEST response;
	memset(&response, 0, sizeof(response));
	
	CONTEXT_HEAD context;
	context.hSocket = g_pSockClients[svrindex]->GetSocket();
	context.lSession = 0;
	context.bNeedEcho = 0;
	
	RSS_PLAYERLOGONOFF rpl;
	ZeroMemory(&rpl, sizeof(rpl));
	rpl.nClientID = nClientID;
	rpl.nClientType = CLIENT_TYPE_CHUNK;
	rpl.nGameID = GAME_ID;
	rpl.dwFlags = FLAG_PUSHLOGON_MOBILE|FLAG_PUSHLOGON_BYGAMEID;	//只订阅指定移动端游戏的用户登录信息
//	rpl.dwFlags = FLAG_PUSHLOGON_PC|FLAG_PUSHLOGON_MOBILE|FLAG_PUSHLOGON_ALLAPP; //For test 可订阅所有用户登录信息
	
	request.head.nRequest = GR_RSS_PLAYERLOGONOFF;
	request.nDataLen = sizeof(rpl);
	request.pDataPtr = &rpl;
	
	BOOL bTimeout = FALSE;
	BOOL bSendOK = g_pSockClients[svrindex]->SendRequest(&context, &request, &response, bTimeout);
	return bSendOK;
}


int ReadChunkDBConfig(CHUNK_DB chunkDB[], int nCount)
{
	TCHAR szKey[32];
	TCHAR szValue[256];
	TCHAR *p1, *p2;
	TCHAR *fields[32];
	memset(fields, 0, sizeof(fields));
	memset(szKey, 0, sizeof(szKey));
	memset(szValue, 0, sizeof(szValue));
	
	int nDBCount = GetPrivateProfileInt(_T("chunkdb"),_T("count"),	1,g_szIniFile);	
	for(int i = 0; i < nDBCount && i < nCount; i++)
	{
		sprintf(szKey, _T("CD%d"), i);
		GetPrivateProfileString(_T("chunkdb"),szKey,_T(""),szValue, sizeof(szValue),g_szIniFile);
		
		p1=szValue;
		xyRetrieveFields( p1, fields, 8, &p2 ) ;
		chunkDB[i].nID=atoi(fields[0]);
		lstrcpy(chunkDB[i].szName ,fields[1]);
		lstrcpy(chunkDB[i].szSource ,fields[2]);
		lstrcpy(chunkDB[i].szCatalog ,fields[3]);

		//调试时可直接读写配置文件中的账号密码
#ifdef _DEBUG
       	lstrcpy(chunkDB[i].szUserName ,fields[4]);
		lstrcpy(chunkDB[i].szPassword ,fields[5]);
#endif

		chunkDB[i].nSecurityMode =atoi(fields[6]);
		
		if(!lstrcmpi(chunkDB[i].szName,NAME_CHUNKDB_MAIN))
			chunkDB[i].nType = TYPE_CHUNKDB_MAIN;
		else if(!lstrcmpi(chunkDB[i].szName,NAME_CHUNKDB_GAME))
			chunkDB[i].nType = TYPE_CHUNKDB_GAME;
		else if(!lstrcmpi(chunkDB[i].szName,NAME_CHUNKDB_LOG))
			chunkDB[i].nType = TYPE_CHUNKDB_LOG;
		
	}

	//正式发布时ReleaseS版本，需要读写加密dba文件中的账号密码
#ifndef _DEBUG
	FillDBAccount(chunkDB, nCount);
#endif
	
	return nDBCount;
}

LONG GetChunkLogToken()
{	
	int client = 0;
	LONG token = 0;

	CAutoLock lock(&g_csChunkLogToken);
	POSITION pos = g_mapChunkLogToken.GetStartPosition();
	if (pos) 
		g_mapChunkLogToken.GetNextAssoc(pos, client, token);

	return token;
}


BOOL NotifyLogServer(UINT nRequest, void* pData, int nLen)
{
	BOOL bSendOK = FALSE;
	
	CONTEXT_HEAD context;
	memset(&context, 0, sizeof(context));
	context.bNeedEcho = FALSE;
	
	REQUEST request;
	memset(&request, 0, sizeof(request));
	request.head.nRequest = nRequest;
	request.pDataPtr = pData;
	request.nDataLen = nLen;

	LONG token = GetChunkLogToken();
	if(token){
		SOCKET sock = FindSocket(token);
		if(sock){
			context.hSocket = sock;
			context.lTokenID = token;
			DWORD dwEncrypt = g_pSockServer->m_flagEncrypt;
			bSendOK = g_pSockServer->SendRequest(context.hSocket, &context, &request,
				MSG_REQUEST, dwEncrypt, 0);
		}
	}
	return TRUE;
}

//填入加密文件中的数据库账号密码
void FillDBAccount(CHUNK_DB DBServer[], int nCount)
{
	if(!UwlPathExists(g_szDBAFile,FALSE))//文件没有找到
		return;
	
	HANDLE hFile;
	hFile=CreateFile(g_szDBAFile,
		GENERIC_READ,
		FILE_SHARE_READ,NULL,
		OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile==INVALID_HANDLE_VALUE)
		return;
	
	DWORD dwFileSize=GetFileSize(hFile,NULL);
	if (dwFileSize < sizeof(DB_ACCOUNT_HEADER))
		return;
	
	SetFilePointer(hFile,0,NULL,FILE_BEGIN);
	
	int nLen=dwFileSize;
	BYTE * pData=new BYTE[nLen];
	memset(pData,0,nLen);
	DWORD dwRead ;
	ReadFile(hFile,(LPVOID)pData, dwFileSize ,&dwRead,NULL);
	CloseHandle(hFile);
	
	BYTE * pDecryptData=new BYTE[nLen];
	TcyDecryptData(nLen, pDecryptData, pData );
	SAFE_DELETE_ARRAY(pData);
	
	BYTE* pByte = pDecryptData;
	DB_ACCOUNT_HEADER*  pDBAHeader = (DB_ACCOUNT_HEADER*)pByte;
	pByte+=sizeof(DB_ACCOUNT_HEADER);
	
	if (dwRead >= (sizeof(DB_ACCOUNT_HEADER)+pDBAHeader->nCount*sizeof(DB_ACCOUNT)))
	{
		DB_ACCOUNT* pDA = (DB_ACCOUNT*)pByte;
		for (int i=0; i<pDBAHeader->nCount; i++)
		{
			if (pDA->nDBIndex<nCount&&pDA->nDBIndex>=0)
			{
				lstrcpyn(DBServer[pDA->nDBIndex].szUserName, pDA->szUserName, MAX_USERNAME_LEN);
				lstrcpyn(DBServer[pDA->nDBIndex].szPassword, pDA->szPassword, MAX_PASSWORD_LEN);
			}
			
			pDA++;
		}
	}
	
	SAFE_DELETE_ARRAY(pDecryptData);
}


BOOL SubscribePayEvent(int svrindex, int nClientID)
{
	REQUEST request;
	memset(&request, 0, sizeof(request));
	REQUEST response;
	memset(&response, 0, sizeof(response));
	
	CONTEXT_HEAD context;
	context.hSocket = g_pSockClients[svrindex]->GetSocket();
	context.lSession = 0;
	context.bNeedEcho = 0;
	
	SS_PAYEVENT sp;
	ZeroMemory(&sp, sizeof(sp));
	sp.nClientID = nClientID;
	sp.nClientType = CLIENT_TYPE_CHUNK;
	sp.nGameID = GAME_ID;
	sp.dwFlags = FLAG_PUSHNOTIFY_BYGAMEID|FLAG_PUSHNOTIFY_PAY;	
	
	request.head.nRequest = GR_SS_PAYEVENT;
	request.nDataLen = sizeof(sp);
	request.pDataPtr = &sp;
	
	BOOL bTimeout = FALSE;
	BOOL bSendOK = g_pSockClients[svrindex]->SendRequest(&context, &request, &response, bTimeout);
	return bSendOK;
}