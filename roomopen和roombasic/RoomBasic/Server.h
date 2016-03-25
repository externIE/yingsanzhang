#pragma once

class CBaseServer{
public:
	CBaseServer();
	virtual ~CBaseServer();

	virtual CBaseSockServer* CreateCSockServer();
	virtual CBaseSockClient* CreateCSockClient();
	virtual CBaseRoomServer* CreateCRoomServer();
	
	virtual BOOL Initialize();
	virtual void Shutdown();
	//获得工程名[注:对于定制的roomsvr,你必须返回你的exe的全名,比如:roomsvrxxxx]
	static CString GetProductName();

			BOOL CheckLicense();
			BOOL InitStart();
			BOOL ConnectCheckSvr();
			BOOL CreateTimerThread();
			BOOL CreatePulseThread();
			BOOL CreateMessageThread();
			BOOL CreateMatchThread();

	BOOL    CreateStatThread();
	BOOL	CreateServerPulseThread();

protected:

	virtual unsigned TimerThreadProc();
	virtual unsigned PulseThreadProc();
	virtual unsigned SendThreadProc();
	virtual unsigned ConnectThreadProc();
	virtual unsigned MessageThreadProc();
	virtual unsigned MatchThreadProc();
	virtual unsigned StatThreadProc();
	virtual unsigned ServerPulseThreadProc();

	static unsigned __stdcall TimerThreadFunc(LPVOID lpVoid);
	static unsigned __stdcall PulseThreadFunc(LPVOID lpVoid);
	static unsigned __stdcall SendThreadFunc(LPVOID lpVoid);
 	static unsigned __stdcall ConnectThreadFunc(LPVOID lpVoid);
 	static unsigned __stdcall MessageThreadFunc(LPVOID lpVoid);
 	static unsigned __stdcall MatchThreadFunc(LPVOID lpVoid);
 	static unsigned __stdcall StatThreadFunc(LPVOID lpVoid);
 	static unsigned __stdcall ServerPulseThreadFunc(LPVOID lpVoid);
public:
			void OnMatchTimer();
	virtual	BOOL OnUserEnterGameOK(int nRoomID,int nUserID){ return TRUE;}
	virtual	BOOL OnUserLeaveGameOK(int nRoomID,int nUserID){ return TRUE;}
	virtual	BOOL OnUserEnterGameOKVerified(int nUserID,DWORD dwRoomTableChair){ return TRUE;}
	virtual	BOOL OnUserLeaveGameOKVerified(int nUserID,DWORD dwRoomTableChair){ return TRUE;}
			BOOL OnUserVerifyStart(DWORD dwRoomTableChair,int nUserID);
			BOOL OnGameStartUp(int nRoomID,DWORD dwTableChairStatus);
			BOOL OnGameBoutEnd(int nRoomID,DWORD dwTableChairStatus);
			BOOL OnUserMatchScore(DWORD nRoomScore,int nUserID);
			BOOL OnUserWelfareDeposit(DWORD nRoomDeposit,int nUserID);
			BOOL OnUserAddScore(DWORD nRoomScore,int nUserID);
			BOOL OnUserMinusScore(DWORD nRoomScore,int nUserID);
	virtual	BOOL OnPlayingChangeChair(int nRoomID,int nTableNO){return TRUE;}
			BOOL OnGameSvrPulse(int nGamePort,HWND hWnd);
	virtual BOOL OnChangeSpecifyTable(DWORD dwRoomTableIndex,int nUserID){ return TRUE;}
	virtual BOOL OnAskNewTable(DWORD dwRoomTableChair,int nUserID){ return TRUE;}
	virtual BOOL OnChangeTable(DWORD dwRoomTableChair,int nUserID){ return TRUE;}
			BOOL OnCloseSoloTable(int nRoomID,int nTableNO);
			BOOL OnFindVPN(DWORD dwIPAddr,DWORD dwPort);
			BOOL OnRemoveVPN(DWORD dwIPAddr,DWORD dwPort);
			BOOL OnUserBoutEnd(int nRoomID,int nUserID);

			BOOL OnChangeChair(DWORD dwRoomTableChair,int nUserID);
			BOOL OnHomeUserChanged(int nUserID, DWORD dwRoomTableChair);

	virtual BOOL OnCustomWndMsg(int nMsgId,int wParam ,int lParam){return FALSE;}

			BOOL ValidateClientEx();
			void ClearRoomDataMap();
			BOOL ClearOverdueKickDetailMap(CKickoffDetailMap*  pKickDetailMap, int nNow, int nMaxIsolate, int& nCount);
			void ClearPlayerPool();
			void ClearPlayerKickList();
			void ClearKickoffRecordMap();

			void SendPulseData();
			void SendRoomSvrPulse();
			void ParseRoomTableChair(DWORD dwRoomTableChair,int& nRoomID,int& nTableNO,int& nChairNO);
			void ParseTableChairStatus(DWORD dwTableChairStatus, int &nTableNO, int nChairStatus[], int nChairCount );
			void ClearOverdueKickList(int nNowSeconds, int nMaxIsolate);
			BOOL KickOffDeadAtMorning(int deadtime, BOOL bStatic = TRUE);
			BOOL KickOffTablePlaying(CBaseRoomData* lpRoomData);
			BOOL GetChatForbid();
};
