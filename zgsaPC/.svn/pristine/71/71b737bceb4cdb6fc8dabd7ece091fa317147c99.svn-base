/********************************************************************
	created:	2015/02/11
	created:	11:2:2015   14:37
	author:		taohg
	
	purpose:	AssitSvr, 连接客户端与ChunkSvr, 可做扩展数据库操作
*********************************************************************/
 
#pragma once

#include "stdafx.h"

typedef CMap<int, int, NTF_PLAYERLOGON, NTF_PLAYERLOGON&>       CUserLogonMap;
typedef CMap<LONG, LONG, SOCKET, SOCKET>						CTokenSockMap;

class CAssitServer : public CSockServer
{
public:
	CAssitServer(const TCHAR* szLicenseFile,
			   const TCHAR* szProductName,
			   const TCHAR* szProductVer,
			   const int nListenPort, const int nGameID,
			   DWORD flagEncrypt = 0, DWORD flagCompress = 0);
	~CAssitServer(){}

 	virtual BOOL Initialize(); 
	virtual void Shutdown();
			BOOL InitClientID();
			BOOL CheckLicense();
			BOOL InitIniFile();
			BOOL GetChunkSvrIP();

	virtual BOOL BaseVerify(int userid, int gameid);
	virtual void SendPulseData();
	virtual int  ClearStaleUserData(DWORD dwSeconds);

	virtual BOOL  SimulateReqFromUser(SOCKET hSocket,LONG lTokenID,UINT nRequest,int nDataLen,VOID*	pDataPtr);

	//连接相关
	virtual BOOL OnConnectSocket(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);

	//send req to chunksvr
	virtual BOOL TransmitRequest(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest);
	//ntf from chunksvr
	virtual BOOL OnChunkSendPulse(LPREQUEST lpRequest);
	virtual BOOL OnUserLogon(LPNTF_PLAYERLOGON lpPlayerLogon);
	virtual BOOL OnUserLogoff(LPNTF_PLAYERLOGOFF lpPlayerLogoff);
	// chunksvr ret
	virtual BOOL OnGetPropInfoRet(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClit, LPREQUEST lpReqFromSvr);
	virtual BOOL OnUseLuckCardRet(LPCONTEXT_HEAD lpContext, LPREQUEST lpReqToClit, LPREQUEST lpReqFromSvr);

	// ntf to client
	virtual void NotifyOneUserErrorInfo(LPCONTEXT_HEAD lpContext, LPCTSTR lpErroMsg);

// 
protected:
	//
	virtual unsigned SendThreadProc();
	virtual unsigned ConnectThreadProc();
	virtual unsigned TimerThreadProc();
	virtual unsigned MessageThreadProc();
	virtual unsigned ServerPulseThreadProc();

	static unsigned __stdcall SendThreadFunc(LPVOID lpVoid);
	static unsigned __stdcall ConnectThreadFunc(LPVOID lpVoid); //负责重连chunksvr
	static unsigned __stdcall TimerThreadFunc(LPVOID lpVoid);
	static unsigned __stdcall MessageThreadFunc(LPVOID lpVoid);	
 	static unsigned __stdcall ServerPulseThreadFunc(LPVOID lpVoid); //

	// functions to chunk server
	virtual BOOL ValidateClientEx();			    //验证IP是否授权
	virtual BOOL ValidateClientInfo(int nClientID); //验证ClientID是否授权

	// 
	virtual BOOL IsTimingOK(DWORD interval, int& sameHour, int& lastHour, int& cycleMin);
	virtual int  DoTimingWork();
	virtual int  OnTimerTriggered(DWORD interval, int& sameHour, int& lastHour, int& cycleMin);
	virtual void OnHourTriggered(int& lastHour);		//每小时触发一次
	
	// admin reqs
	virtual BOOL OnReconnectSvr(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
	virtual int  OnReconnectSvr(int adminid);
	virtual int  OnHardCode(int userid, DWORD hardcode);
	virtual int  OnRoomTokenID(int userid, int roomtokenid);
	virtual int  OnExperience(int userid, int experience);
	
	// for client's request
	virtual BOOL OnRequest(void* lpParam1, void* lpParam2);
	virtual BOOL OnGamePulse(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
	virtual BOOL OnCloseSocket(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
	virtual	BOOL OnQuerytUserLogon(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt); //重新获取登录信息
	virtual BOOL OnGetPropInfo(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);
	virtual BOOL OnUseLuckCard(LPCONTEXT_HEAD lpContext, LPREQUEST lpRequest, CWorkerContext* pThreadCxt);

public:
			int   VerifyUserLogon(LPCONTEXT_HEAD lpContext ,int nUserID,LPCTSTR lpszHardID);
			void  QueryUserLogon(LPCONTEXT_HEAD lpContext, int nUserID); //查询登录信息
			void  InsertUserLogonMap(int userid, NTF_PLAYERLOGON& ntfLogon);
			void  EraseUserLogonMap(int userid);
			SOCKET FindSocket(LONG token);

public:
	UINT			m_uiThrdSend;
	UINT			m_uiThrdTimer; 
	UINT			m_uiThrdConnect;
	UINT			m_uiThrdMessage; 
	UINT			m_uiThrdSvrPulse;
	
protected:
	CSockClient*	m_pSockClient;
	
	HANDLE			m_hExitServer;
	HANDLE			m_hThrdSend;
	HANDLE			m_hThrdTimer; 
	HANDLE			m_hThrdConnect;
	HANDLE			m_hThrdMessage;
	HANDLE			m_hThrdSvrPulse;
	
	int				m_nListenPort;
	int				m_nGameID;
	int				m_nScoreMult;
	HWND			m_hGameSvrWnd;
	
	TCHAR			m_szChunkServer[MAX_PATH];
	TCHAR			m_szConnect[MAX_PATH];
	TCHAR			m_szLicFile[MAX_PATH];//许可证文件
	int			    m_nClientID;
	TCHAR			m_szIniFile[MAX_PATH];
	TCHAR			m_szProductName[MAX_PRODUCT_LEN];
	TCHAR			m_szProductVer[MAX_VERSION_LEN];
	
	LICSETTINGS		m_lic;

	CCritSec		m_csUserLogon;
	CUserLogonMap	m_mapUserLogon;

	CTokenSockMap	m_mapTokenSock;
	CCritSec		m_csTokenSock;

	CCritSec		 m_csSvrPulseInfo;
	SERVERPULSE_INFO m_SvrPulseInfo;
};
