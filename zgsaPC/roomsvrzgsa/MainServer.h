#pragma once

extern BOOL RegMessageWnd(int nPort);
extern void UnregMessageWnd(int nPort);
extern HWND CreateMessageWnd(int nPort);

typedef struct _tagSeatedToLookOnPlayer{
	int		nUserID;				//UserID
	int		nRoomID;				//用户所在房间号
	int		nTableNo;				//桌子号
	int		nChairNo;				//椅子号
	int		nIPConfig;				//IP配置
	int		nBreakReq;				//断线限制
	int		nSpeedReq;				//网速限制
	int		nMinScore;				//最小分数限制
	int		nMinDeposit;			//最小银两限制
	int		nStartLookOnTime;		//开始旁观的时间
	int		nReserved[4];			//备用字段
}SEATEDTOLOOKONPLAYER, *LPSEATEDTOLOOKONPLAYER;

typedef CMap<int, int, LPSEATEDTOLOOKONPLAYER, LPSEATEDTOLOOKONPLAYER> CSeatedToLookOnPlayerMap;

class CMainServer : public CMainOpenServer{
public:
	CMainServer();
	virtual ~CMainServer();

	virtual CBaseSockServer* CreateCSockServer();
	virtual CBaseSockClient* CreateCSockClient();
	virtual CBaseRoomServer* CreateCRoomServer();


	virtual BOOL Initialize();
	virtual void Shutdown();
	virtual CString GetProductName();
	virtual BOOL OnChangeTable(DWORD dwRoomTableChair,int nUserID);
	virtual	BOOL OnUserLeaveGameOKVerified(int nUserID,DWORD dwRoomTableChair);

			void OnChangeSeatedToLookOnFromGameSvr(DWORD dwRoomTableChair,int nUserID);
			void OnChangeLookOnToSeatedFromGameSvr(DWORD dwRoomTableChair,int nUserID);

			BOOL InsertToSeatedToLookOnPlayerMap(int nUserID, int nRoomID, int nTableNo, int nChairNo, int nIPConfig, int nBreakReq, int nSpeedReq, int nMinScore, int nMinDeposit);
			void DestorySeatedToLookOnPlayer(LPSEATEDTOLOOKONPLAYER pSeatedToLookOnPlayer);
			void RemoveSeatedToLookOnPlayer(int nUserID);
			void RemoveTimeOutSeatedToLookOnPlayers();
			void ClearSeatedToLookOnPlayerMap();

			static unsigned __stdcall Timer2ThreadFunc(LPVOID lpVoid);
			BOOL			CreateTimer2Thread();
			unsigned		Timer2ThreadProc();
protected:
	virtual unsigned MessageThreadProc();

private:
	CCritSec					m_csSeatedToLookOnPlayers;		//控制坐下变旁观玩家数据的临界区
	CSeatedToLookOnPlayerMap	m_mapSeatedToLookOnPlayers;		//坐下变旁观玩家队列
};
