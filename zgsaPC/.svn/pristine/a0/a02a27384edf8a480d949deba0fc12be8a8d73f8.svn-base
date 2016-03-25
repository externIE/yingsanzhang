#pragma once

#include "resource.h"

#define APPLICATION_TITLE	 _T("ZgjhChunkSvr")
#define STR_SERVICE_NAME     _T("ZgjhChunkSvr")
#define STR_DISPLAY_NAME     _T("同城游赢三张数据服务")
#define STR_DISPLAY_NAME_ENU _T("TCY ZgjhChunkSvr Service") 

extern int				g_nClientID;
extern SOCK_SERVER		g_SockServer[MAX_TOTALSVR_COUNT];

extern TCHAR			g_szLicFile[MAX_PATH];
extern TCHAR			g_szIniFile[MAX_PATH];
extern TCHAR			g_szDBAFile[MAX_PATH];


extern CStringLONGMap   g_mapGameSvrToken;
extern CGameSvrTokenMap	g_mapClientTokenG;
extern CCritSec			g_csClientTokenG;

extern CClientTokenMap	g_mapRoomSvrToken;
extern CCritSec			g_csRoomSvrToken;

extern CClientTokenMap	g_mapAssitSvrToken;
extern CCritSec			g_csAssitSvrToken;

extern CTokenSockMap	g_mapTokenSock;
extern CCritSec			g_csTokenSock;

extern CClientTokenMap	g_mapChunkLogToken;
extern CCritSec			g_csChunkLogToken;

extern UINT				g_uiThrdTimer;
extern UINT				g_uiThrdClientPulse;

extern UINT				g_uiThrdSends[MAX_TOTALSVR_COUNT];
extern UINT				g_uiThrdConnects[MAX_TOTALSVR_COUNT];

extern CSockServer*		g_pSockServer;
extern CSockClient*		g_pSockClients[MAX_TOTALSVR_COUNT];
extern CSockClient*		g_pSockClientOnline;

extern int				g_nTotalSvrCount;
extern int              g_nSubSvrCount;

extern HANDLE			g_hExitServer;
extern HANDLE			g_hThrdTimer;
extern HANDLE			g_hThrdClientPulse;

extern HANDLE			g_hThrdSends[MAX_TOTALSVR_COUNT];
extern HANDLE			g_hThrdConnects[MAX_TOTALSVR_COUNT];

extern THREAD_INFO      g_threadInfo[MAX_TOTALSVR_COUNT];

extern CHUNK_DB			g_ChunkDB[MAX_TOTALDB_COUNT];


extern SOCKET			FindSocket(LONG token);

extern void				CopyMapGameSvrToken(CStringLONGMap& mapGameSvrToken);
extern void				CopyMapClientTokenG(CClientTokenMap& mapClientToken, int gameid);
extern void				CopyMapAllTokenSock(CTokenSockMap& mapTokenSock);
extern LONG				GetChunkLogToken();

extern void				SendPulseData(int svrindex);
extern BOOL				ValidateClientInfo(int svrindex, int nClientID);


extern BOOL				NotifyClientsG(UINT nRequest, void* pData, int nLen, int gameid, BOOL toCompress = FALSE);
extern BOOL				NotifyAllGameSvr(UINT nRequest, void* pData, int nLen ,BOOL toCompress = FALSE);
extern BOOL				NotifyOneGameSvr(UINT nRequest, void* pData, int nLen, LONG token);

extern BOOL				NotifyServer(UINT nRequest, void* pData, int nLen, int svrindex);
extern BOOL				NotifyHubSvr(UINT nRequest, void* pData, int nLen);

extern BOOL				NotifyServerEx(UINT nRequest,int nSubReq, void* pData, int nLen, int svrindex);

extern BOOL				NotifyLogServer(UINT nRequest, void* pData, int nLen);

extern void				ClearGameSvrTokenMap();
  

extern BOOL				IsInPeriodOfProfile(LPCTSTR lpSection, int nBegin, int nEnd);

extern int				CalcSubIndexByUserID(int userid);

extern void				ReportDBErrorLog(_com_error &e, LPCTSTR lpszSql);


extern LONG				FindRoomToken(int clientid);
extern BOOL				NotifyOneRoomSvr(UINT nRequest, void* pData, int nLen, int clientid, BOOL toCompress = FALSE);
extern BOOL				NotifyAllRoomSvr(UINT nRequest, void* pData, int nLen, int exceptClient = 0, BOOL toCompress = FALSE);
extern BOOL				NotifyRoomPlayer(UINT nRequest, void* pData, int nLen, int clientid, int roomid,int playerid);
extern void				CopyMapRoomSvrToken(CClientTokenMap& mapClientToken);

extern LONG				FindAssitToken(int clientid);
extern BOOL				NotifyOneAssitSvr(UINT nRequest, void* pData, int nLen, int clientid, BOOL toCompress = FALSE);
extern BOOL				NotifyAllAssitSvr(UINT nRequest, void* pData, int nLen, int exceptClient = 0, BOOL toCompress = FALSE);
extern void				CopyMapAssitSvrToken(CClientTokenMap& mapClientToken);
extern BOOL				RSSPlayerLogOnOff(int svrindex, int nClientID);

extern int				ReadChunkDBConfig(CHUNK_DB chunkDB[], int nCount);
extern void				FillDBAccount(CHUNK_DB DBServer[], int nCount);

extern BOOL				SubscribePayEvent(int svrindex, int nClientID);