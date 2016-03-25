#pragma once

#define  IS_EMPTY_STR(str)   (str[0]==0)

#define DEF_ACTION_INTERVAL		15 // seconds

#define		WM_GTR_CUSTOM_BEGIN			    (WM_USER+5000)//自定义的roomsvr和gamesvr之间的进程消息开始ID 
#define     WM_GTRC_STARTINWAIT             (WM_GTR_CUSTOM_BEGIN+1) // solo 房间一局结束后又点击了开始按钮


#define  RANKMATCH_STATUS_UNSTART      1
#define  RANKMATCH_STATUS_STARTED      2
#define  RANKMATCH_STATUS_FINISHED     3
typedef struct _tagUSER_SOCK{
	SOCKET sock;
	LONG   token;
	int    nUserID;
    DWORD dwEnterFlags;
}USER_SOCK, *LPUSER_SOCK;

typedef CMap<LONG, LONG, USER_SOCK, USER_SOCK> CTokenUserSockMap;
typedef CMap<int, int ,  USER_SOCK, USER_SOCK> CUserSockMap;

typedef CMap<int, int, CIDMap*, CIDMap*>       CUnwelcomesMap;

#define  ROOM_CHAT_INTERVAL   2000  //毫秒
typedef struct _tagROOM_CHAT{
	int nUserID;
	DWORD dwLatestTime;
}ROOM_CHAT, *LPROOM_CHAT;
typedef CMap<int,int&,ROOM_CHAT,ROOM_CHAT&>   CRoomChatMap;

#define ROLE_KICKER		0			//踢人者
#define ROLE_BEENKICKED	1			//被踢
typedef struct _tagKICKOFF_DETAIL{
	int nOther;						//对方
	int nUserID;					//自己
	int nRole;						//自己在踢人关系中的角色
	int nKickTime;					//踢人发生的时间
}KICKOFF_DETAIL, *LPKICKOFF_DETAIL;

typedef struct _tagRANDOM_CONFIG{
	int nFixTable;					//如果为1，凑够人就分桌
	int nMaxPreTables;				//有人坐的随机桌上的最大桌号
	int nMinRandomPlayer;			//随机最低人数要求
	int nRandomDen;					//加入最大桌的几率
	int nReserved[4];
}RANDOM_CONFIG, *LPRANDOM_CONFIG;

typedef struct _tagOPENTIME_SECTION{
	int nRoomID;
	int nMatchStatus;
	int nSectionCount;
	int nBeginDate;
	int nBeginTime;
	int nEndDate;
	int nEndTime;
	int nBeginSection[8];
	int nEndSection[8];
	int nRestTime;
}OPENTIME_SECTION,*LPOPENTIME_SECTION;

typedef struct _tagSERVERPULSE_INFO{
   	int   nCurrentDate; //20131212
	int   nLatestTime;//2038秒
	int   nReconnectCount;//每日统计，次日清0
}SERVERPULSE_INFO, *LPSERVERPULSE_INFO;

typedef struct _tagKICKOFF_RECORD{
	int nUserID;					//自己
	int nKickCount;					//踢人次数
	int nCurrentDate;				//踢人发生的日期
}KICKOFF_RECORD, *LPKICKOFF_RECORD;

typedef CMap<CString,LPCTSTR,KICKOFF_RECORD,KICKOFF_RECORD&>   CKickoffRecordMap;

typedef struct _tagGAMESVR_INFO{
	int nGameID;
	int nGamePort;
	HWND hWnd;
	int nLatestTime;
}GAMESVR_INFO, *LPGAMESVR_INFO;
typedef CMap<int,int&,GAMESVR_INFO,GAMESVR_INFO&>           CGameSvrInfoMap;

typedef CMap<int, int, CBaseRoomData*, CBaseRoomData*> CRoomDataExMap;

typedef CMap<DWORD, DWORD, TCY_IPINFO, TCY_IPINFO&>         CTcyIpInfoMap;

typedef struct _tagROOM_SOCK{
	SOCKET sock;
	LONG   token;
	int    nRoomID;
}ROOM_SOCK, *LPROOM_SOCK;
typedef CMap<LONG, LONG, ROOM_SOCK, ROOM_SOCK> CTokenRoomSockMap;

typedef struct _tagASKER_ACTION{  //包括querytablepwd,addfriend,inviteplayer
	int    nTime;  //UwlGetCurrentSeconds()函数获取
	int    nCount;
}ASKER_ACTION, *LPASKER_ACTION;
typedef CMap<CString,LPCTSTR,ASKER_ACTION,ASKER_ACTION&>    CAskerActionMap;
typedef CMap<int,int&,KICKOFF_DETAIL,KICKOFF_DETAIL&>   CKickoffDetailMap;
typedef CMap<int,int&,CKickoffDetailMap*,CKickoffDetailMap*&>   CPlayerKickoffListMap;
typedef struct _tagCHAT_ACTION{
	int    nTime;  //UwlGetCurrentSeconds()函数获取聊天时间
	int    nPlayer;
}CHAT_ACTION, *LPCHAT_ACTION;
typedef struct _tagCALL_ACTION{
	int    nTime;  //UwlGetCurrentSeconds()函数获取聊天时间
	int    nPlayer;
}CALL_ACTION, *LPCALL_ACTION;
typedef CList<CHAT_ACTION,CHAT_ACTION&>        CChatActionList;
typedef CList<CALL_ACTION,CALL_ACTION&>        CCallActionList;
typedef CMap<int,int&,CChatActionList*,CChatActionList*&>   CChatActionListMap;
typedef CMap<int,int&,CCallActionList*,CCallActionList*&>   CCallActionListMap;

typedef CMap<LONG, LONG, SOCKET, SOCKET>       CTokenSockMap;
typedef CMap<int, int, LONG, LONG>             CClientTokenMap;
typedef CMap<int, int, CIntIntMap*,  CIntIntMap*&>			CIntIntIntMap;
typedef std::map<int,std::vector<int> >		   CIntVecIntMap;
class CClientInfo{
public:
	CClientInfo(){
		ZeroMemory(&cci, sizeof(cci));
		ZeroMemory(tipp,sizeof(tipp));
		ZeroMemory(tras,sizeof(tras));
		
	}
	~CClientInfo();
	
	COMMIT_CLIENTINFO  cci;
	CTcyIpInfoMap     mapIP;
	CTcyIpInfoMap     mapARP;
	TCY_IPPORTINFO    tipp[4];
	TCY_RASINFO       tras[8];
	
	CStringIntMap     mapARPComb;
public:
	void  ZeroClientInfo();
	void  FillClientInfo(COMMIT_CLIENTINFO* pcc);
	int   GetClientInfoSize();
	void  CombClientInfo(BYTE* pData);
};
typedef CMap<int,int&,CClientInfo*,CClientInfo*&>   CClientInfoMap;


extern int					 g_nOverSeeUserId; // 监视玩家id
extern int					 g_bOpenOverSeeUser;// 是否打开监视玩家状态功能

extern CGameSvrInfoMap	     g_mapGameSvrInfo;
extern CCritSec			     g_csGameSvrInfo;
extern CIntIntMap			 g_mapProxyIP;
extern CCritSec				 g_csProxyIP;

extern CFilterWordMap		 g_mapFilterWord;
extern CCritSec				 g_csFilterWord;

extern CChatForbidMap        g_mapChatForbid;
extern CChatForbidIDMap      g_mapChatForbidID;
extern CCritSec				 g_csChatForbid;

extern CRoomDataExMap		 g_mapRoomDataEx;
extern CCritSec				 g_csRoomData;

extern SERVERPULSE_INFO g_ServerPulseInfo;
extern CCritSec		    g_csServerPulseInfo;

extern CTokenRoomSockMap	g_mapTokenRoomSock;
extern CCritSec			g_csTokenRoomSock;

extern CAskerActionMap  g_mapAddFriendAction;
extern CCritSec         g_csAddFriendAction;
extern CKickoffRecordMap		g_mapKickoffRecord;
extern CCritSec					g_csKickoffRecord;
extern CPlayerKickoffListMap	g_mapPlayerKickList;
extern CCritSec					g_csPlayerKickList;

extern CCritSec			g_csPlayerList;
extern CPtrList			g_PoolOfPlayer;

extern CIDMap           g_mapLocalIP;
extern CCritSec			g_csTokenSock;
extern CTokenSockMap	g_mapTokenSock;
extern CCritSec		    g_csGameSvrToken;
extern CClientTokenMap	g_mapGameSvrToken;
extern CBaseSockServer*		g_pSockServer;
extern CBaseSockClient*		g_pSockClient;
extern HWND             g_hMessageWnd;

extern volatile LONG    g_lNoBufferCount; 
extern volatile LONG    g_lNoBufferLatest; //second
extern int				g_nHostID;
extern int				g_nClientID;
extern LONG				g_lClientForbid;
extern int              g_nRoomPort;
extern int             g_nSysFindCD;
extern int				g_nSysFindScale;   
extern int             g_nModeMoneyDiff;   
extern int             g_nModeBountDiff;   
extern int             g_fModeWinsDiff;  
extern CBaseRoomServer*		g_pRoomServer;
extern CCritSec		    g_csMutexHard;

extern CMutexHardSmallMap    g_mapMutexHardSmall;
extern CStringIntMap         g_mapMutexUserID;

extern HANDLE			g_hExitServer;

extern HANDLE			g_hThrdSend;
extern HANDLE			g_hThrdRecv;
extern HANDLE			g_hThrdTimer;
extern HANDLE           g_hThrdPulse;
extern HANDLE			g_hThrdConnect;
extern HANDLE           g_hThrdMessage;
extern HANDLE           g_hThrdMatch;
extern HANDLE           g_hThrdStat;
extern HANDLE			g_hThrdSvrPulse;
extern UINT				g_uiThrdSend;
extern UINT				g_uiThrdRecv;
extern UINT				g_uiThrdTimer;
extern UINT				g_uiThrdPulse;
extern UINT				g_uiThrdMessage;
extern UINT				g_uiThrdMatch;
extern UINT				g_uiThrdStat;
extern UINT				g_uiThrdSvrPulse;
extern UINT				g_uiThrdConnect;

extern TCHAR			g_szIniFile[MAX_PATH];
extern TCHAR			g_szIPManFile[MAX_PATH];
extern TCHAR			g_szKickManFile[MAX_PATH];
extern TCHAR			g_szCheckServer[MAX_PATH];
extern TCHAR			g_szLicFile[MAX_PATH];

extern BOOL NotifyOnePlayer(CBaseRoomData* lpRoomData, int playerid, UINT nRequest, void* pData, int nLen);
extern BOOL NotifyRoomPlayers(CBaseRoomData* lpRoomData, UINT nRequest, void* pData, int nLen,  SOCKET socExcept = 0, LONG tokenExcept = 0);
extern BOOL NotifyDarkLooker(CBaseRoomData* lpRoomData, int playeridExcept, UINT nRequest, void* pData, int nLen);
extern HWND FindLoftSvrWindow();
extern BOOL IsSomeValueInFile(LPCTSTR lpszSection,LPCTSTR lpszKey,CString& sValue,LPCTSTR lpszFileName);

extern BOOL GetSocketTokenIDByUserID(CBaseRoomData* lpRoomData, int nUserID, 
									SOCKET& hSocket, LONG& lTokenID);
extern int  RemoveTokenRoomSock( LONG token);
extern BOOL ExchangeChair(CBaseRoomData* lpRoomData, LPTABLE pTable, int chair1, int chair2, int user1, int user2);
extern void SetTableAfterFinished(CBaseRoomData* lpRoomData, LPPLAYER lpPlayer);

extern void SetChairLeave(int nUserID,TABLE* pTable,int nChairNO);
extern void SetRandomTableLeave(CBaseRoomData* lpRoomData, LPPLAYER lpPlayer);
extern void SetTableAsEmptyOrLeave(CBaseRoomData* lpRoomData, LPPLAYER lpPlayer);
extern void SetCommonTableLeave(CBaseRoomData* lpRoomData, LPPLAYER lpPlayer);
extern void SetTableAsLookonLeave(CBaseRoomData* lpRoomData, LPPLAYER lpPlayer);

extern BOOL NotifyServer(UINT nRequest, void* pData, int nLen);

extern BOOL FilterPlayAryByMinDeposit(CBaseRoomData* lpRoomData, LPTABLE lpTable, int aryPlayer[]);
extern BOOL FilterPlayAryByMinScore(CBaseRoomData* lpRoomData, LPTABLE lpTable, int aryPlayer[]);
extern BOOL SetTableAsCompareScoreDeposit(CBaseRoomData* lpRoomData, LPTABLE lpTable, int aryPlayer[]);

extern int  ClearAddFriendAction();
extern BOOL SaveAddFriendAction( int nSender,int nPlayer);

extern int  ClearCallAction();
extern BOOL SaveCallAction( int nSender,int nPlayer);

extern int  ClearChatAction();
extern BOOL SaveChatAction( int nSender,int nPlayer);

extern int  ClearTablePwdAction();
extern BOOL SaveTablePwdAction( int nSender,int nPlayer);

extern int  ClearInviteAction();
extern BOOL SaveInviteAction( int nSender,int nPlayer);

extern void RemoveOnePlayer(CBaseRoomData* lpRoomData,LPPLAYER lpPlayer, SOCKET socExcept, LONG tokenExcept);
extern BOOL NotifyPlayerLeft(CBaseRoomData* lpRoomData, LPPLAYER lpPlayer, SOCKET socExcept = 0, LONG tokenExcept = 0);

extern BOOL KickOffDeadPlayersInRoom(CBaseRoomData* lpRoomData, int deadtime, BOOL bStatic,BOOL bAtMorning);
extern BOOL KickOffOneWalkAround(CBaseRoomData* lpRoomData,LPPLAYER lpPlayer );
extern BOOL KickOffRoomPlayer(LPCONTEXT_HEAD lpContext, CBaseRoomData* lpRoomData,int nUserID,BOOL bKickAfterFinished);
extern void SetTableAsEmptyOrLeaveEx(CBaseRoomData* lpRoomData, LPTABLE lpTable, int chairno, int userid);
extern void SetTableAsLookonLeaveEx(CBaseRoomData* lpRoomData, LPTABLE lpTable, int chairno, int userid);

extern void SaveVipKickoffRecord(int nKicker, int nRoomID);
extern void SaveVipKickoffDetail(int nKicker, int nKickedPlayer);
extern CString FormatDeposit(int nDeposit);
extern LONG FindGameSvrToken(int clientid);
extern BOOL NotifyOneGameSvr(UINT nRequest, void* pData, int nLen, int clientid, BOOL toCompress = FALSE);
extern BOOL NotifyRoomPlayer(int roomid,int playerid, LPREQUEST lpRequest);
extern BOOL ValidateClientInfo(int nClientID);

extern void CommitGameUsers(int nStatTime);
extern void CommitGroupUsers(int nStatTime);
extern void CommitGroupUsersOnGame(int nStatTime);
extern void CommitIPUsers(int nStatTime);
extern void CommitIPUsersOnGame(int nStatTime);
extern PLAYER* GetPlayerBlock();

extern BOOL GetFilterWord();
extern BOOL GetMutexHard();

extern BOOL KickOffOneNotMember(CBaseRoomData* lpRoomData,int playeridExcept, BOOL bUnKickOffMobile = FALSE);
extern BOOL TransmitRoomUsers(LPCONTEXT_HEAD lpContext, LPROOM_USERS lpRoomUsers);
extern BOOL ReadRangeRandomConfig(CBaseRoomData* lpRoomData,  RANDOM_CONFIG* lpRandomConfig);
extern HWND FindHoldSvrWindow();
extern CString GetYxpdsvrIniFile();
extern void FreePlayerBlock(PLAYER* pPlayer);
extern int  GetIsolateSecond(int nRoomID);
extern DWORD ComboRoomTableChair(int nRoomID,int nTableNO,int nChairNO);
extern BOOL ClearRoomChat(CBaseRoomData* lpRoomData);

extern BOOL OnUserAddDeposit(DWORD nRoomDeposit,int nUserID);
extern BOOL OnUserMinusDeposit(DWORD nRoomDeposit,int nUserID);
extern void OverSeeUserData(int nUserId, LPCTSTR callFunName);
extern BOOL OnUserStatusPlaying(DWORD nRoomStatus,int nUserID);

