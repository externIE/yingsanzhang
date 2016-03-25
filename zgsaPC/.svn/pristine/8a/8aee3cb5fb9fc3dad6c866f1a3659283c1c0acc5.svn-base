// GameRequest.h: interface for the CGameRequest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEREQUEST_H__93409324_E26B_4A4D_B8C4_9331792292B0__INCLUDED_)
#define AFX_GAMEREQUEST_H__93409324_E26B_4A4D_B8C4_9331792292B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGameRequest : public CGameConnect  
{
public:
	CGameRequest();
	virtual ~CGameRequest();
public: //µÈ´ý»ØÓ¦
	UINT   SendMsgToServer(DWORD dwFlags,DWORD datalen=0,void* data=NULL,BOOL bNeedEcho=FALSE);
 	void*  SendMsgToServerEx(DWORD dwFlags,UINT& response,int& datalen,void* data=NULL);
public:

	virtual BOOL	ProcessRequest(CString& sRet,UINT nRequest, int& nDataLen, VOID *pDataPtr, UINT &nResponse,
		LPVOID &pRet,BOOL bNeedEcho=TRUE);
	virtual	BOOL	GC_LookSafeDeposit(CString& sRet, SAFE_DEPOSIT_EX * pSafeDeposit);
	virtual	BOOL	GC_TakeSafeDeposit(CString& sRet,int nTakeDeposit,int nKeyResult);
	virtual	BOOL	GC_SaveDeposit(CString& sRet,int& nSaveDeposit, UINT& nResponse);
	virtual	BOOL	GC_AskNewTableChair(CString& sRet);	
public:

			BOOL	GC_SyncStartCountdown(int nCountdown);
			BOOL	GC_ChangeSeatedToLookOn(CString& sRet);
			BOOL	GC_ChangeLookOnToSeated(CString& sRet);

			void	RemindTakeDeposit();
};

#endif // !defined(AFX_GAMEREQUEST_H__93409324_E26B_4A4D_B8C4_9331792292B0__INCLUDED_)
