#pragma once

extern BOOL RegMessageWnd(int nPort);
extern void UnregMessageWnd(int nPort);
extern HWND CreateMessageWnd(int nPort);

typedef struct _tagSeatedToLookOnPlayer{
	int		nUserID;				//UserID
	int		nRoomID;				//�û����ڷ����
	int		nTableNo;				//���Ӻ�
	int		nChairNo;				//���Ӻ�
	int		nIPConfig;				//IP����
	int		nBreakReq;				//��������
	int		nSpeedReq;				//��������
	int		nMinScore;				//��С��������
	int		nMinDeposit;			//��С��������
	int		nStartLookOnTime;		//��ʼ�Թ۵�ʱ��
	int		nReserved[4];			//�����ֶ�
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
	CCritSec					m_csSeatedToLookOnPlayers;		//�������±��Թ�������ݵ��ٽ���
	CSeatedToLookOnPlayerMap	m_mapSeatedToLookOnPlayers;		//���±��Թ���Ҷ���
};
