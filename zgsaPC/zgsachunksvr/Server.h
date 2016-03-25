#pragma once

typedef struct _tagTHREAD_INFO{
	void* lpVoid;
	int	  nIndex;
}THREAD_INFO, *LPTHREAD_INFO;

class CMainServer{
public:
	CMainServer();
	virtual ~CMainServer();
	virtual BOOL Initialize();
	virtual void Shutdown();

	BOOL InitStart();
	BOOL ConnectOnlineSvr();
	BOOL GetSockServers();


	BOOL CreateTimerThread();
    BOOL CreateClientPulseThread();

 	BOOL ReadMainDBData();
	BOOL InitChunkDB();
	BOOL ConnectTrankGameSvr();
	BOOL ConnectOneServer(int nServerIndex, LPCTSTR lpszIP, int nPort, LPCTSTR lpszServerName, int nServerType);
protected:
	virtual unsigned  TimerThreadProc();
	virtual unsigned  ClientPulseThreadProc();
	virtual unsigned  SendThreadProc(int svrindex);
	virtual unsigned  ConnectThreadProc(int svrindex);

	static unsigned __stdcall  TimerThreadFunc(LPVOID lpVoid);
	static unsigned __stdcall  ClientPulseThreadFunc(LPVOID lpVoid);
	static unsigned __stdcall  SendThreadFunc(LPVOID lpVoid);
	static unsigned __stdcall  ConnectThreadFunc(LPVOID lpVoid);
};

