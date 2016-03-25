// GameRequest.cpp: implementation of the CGameRequest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "game.h"
#include "GameRequest.h"
#include "resource.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGameRequest::CGameRequest()
{

}

CGameRequest::~CGameRequest()
{

}


UINT CGameRequest::SendMsgToServer(DWORD dwFlags,DWORD datalen,void* data,BOOL bNeedEcho)
{
	CString sRet;
	sRet.Empty();
	
	GAME_MSG msg;
	ZeroMemory(&msg,sizeof(msg));
	CGameWinApp* pGameApp=(CGameWinApp*)AfxGetApp();
	
	msg.nRoomID=pGameApp->GetRoomID();
	msg.nUserID=pGameApp->GetUserID();
	msg.nMsgID=dwFlags;
	msg.nDatalen=datalen;
	
	int size=datalen+sizeof(GAME_MSG);
	BYTE* pGameMsg=new BYTE[size];
	memset(pGameMsg,0,size);
	memcpy(pGameMsg,&msg,sizeof(GAME_MSG));
	if (datalen)
	{
		BYTE* pGameData=pGameMsg+sizeof(GAME_MSG);
		memcpy(pGameData,data,datalen);
	}
	
	UINT nResponse;
	LPVOID pRet=NULL;
	
	
	BOOL bn=ProcessRequest(sRet,GR_SENDMSG_TO_SERVER, size, pGameMsg, nResponse, pRet,bNeedEcho);
	delete []pGameMsg;
	
	if (bn)
		return nResponse;
	else
		return UR_OPERATE_FAILED;
}

void* CGameRequest::SendMsgToServerEx(DWORD dwFlags,UINT& nResponse,int& datalen,void* data)
{
	CString sRet;
	sRet.Empty();
	
	GAME_MSG msg;
	ZeroMemory(&msg,sizeof(msg));
	CGameWinApp* pGameApp=(CGameWinApp*)AfxGetApp();
	
	msg.nRoomID=pGameApp->GetRoomID();
	msg.nUserID=pGameApp->GetUserID();
	msg.nMsgID=dwFlags;
	msg.nDatalen=datalen;
	
	int size=datalen+sizeof(GAME_MSG);
	BYTE* pGameMsg=new BYTE[size];
	memset(pGameMsg,0,size);
	memcpy(pGameMsg,&msg,sizeof(GAME_MSG));
	if (datalen)
	{
		BYTE* pGameData=pGameMsg+sizeof(GAME_MSG);
		memcpy(pGameData,data,datalen);
	}
	
	LPVOID pRet=NULL;
	
	BOOL bn=ProcessRequest(sRet,GR_SENDMSG_TO_SERVER, size, pGameMsg, nResponse, pRet,TRUE);
	delete []pGameMsg;
	
	if (!bn)
	{
		nResponse=UR_OPERATE_FAILED;
		SAFE_DELETE_ARRAY(pRet);
		datalen=0;
		return NULL;
	}
	else
	{
		datalen=size;
	}
	return pRet;
}


BOOL CGameRequest::ProcessRequest(CString& sRet,UINT nRequest, int& nDataLen, VOID *pDataPtr, UINT &nResponse,
								  LPVOID &pRet,BOOL bNeedEcho)
{
	//ϵͳ��Ϣ������������֮��
	if (nRequest<=GAME_REQ_BASE_EX+20000)
		return CGameConnect::ProcessRequest(sRet,nRequest,nDataLen,pDataPtr,nResponse,pRet,bNeedEcho);
	
	BOOL bn=CGameConnect::ProcessRequest(sRet,nRequest,nDataLen,pDataPtr,nResponse,pRet,bNeedEcho);
	if (bNeedEcho&&!bn)//��Ҫ��Ӧ��ʱ��
	{
	//	if (nResponse==UR_OPERATE_FAILED||!bn)//����UR_OPERATE_FAILED��������ʧ��
	//	{
		UwlLogFile("ProcoessRequest Faild,ReQuestid:%ld,nResponse:%ld",nRequest,nResponse);
		//((CGameApp*)GetGameWinApp())->UpdateTableInfo();
			return FALSE;//����False,�����ظ�ִ��
	//	}
	}
 
	return TRUE;
}

BOOL CGameRequest::GC_LookSafeDeposit(CString& sRet, SAFE_DEPOSIT_EX * pSafeDeposit)
{
	CGameWinApp* pGameApp =  GetGameWinApp();
	
	sRet.Empty();
	LOOK_SAFE_DEPOSIT lsd;
	memset(&lsd,0,sizeof(LOOK_SAFE_DEPOSIT));
	lsd.nUserID = pGameApp->GetUserID();
	lsd.nGameID = pGameApp->GetGameID();
	xyGetHardID(lsd.szHardID);
	
	///////////     Process  Request   ///////////////////////////////////////
	UINT nResponse;
	LPVOID pRet=NULL;
	int nDataLen=sizeof(LOOK_SAFE_DEPOSIT);
	BOOL bResult = ProcessRequest(sRet,GR_LOOK_SAFE_DEPOSIT, nDataLen, &lsd, nResponse, pRet);
	
	if(!bResult)//
		return FALSE;
	
	if(pRet && (nResponse==UR_OPERATE_SUCCEEDED))
	{
		memcpy(pSafeDeposit,pRet,sizeof(SAFE_DEPOSIT_EX));
		SAFE_DELETE_ARRAY(pRet);
		return TRUE;
	}
	SAFE_DELETE_ARRAY(pRet);
	GetSafeBoxReturnString(sRet,nResponse); 
	return FALSE;
	
}

BOOL CGameRequest::GC_TakeSafeDeposit(CString& sRet,int nTakeDeposit,int nKeyResult)
{
	CGameWinApp* pGameApp = GetGameWinApp();
	if(!pGameApp->IsTakeDepositInGame())
		return FALSE;
	
	sRet.Empty();
	///////////     Process  Request   ///////////////////////////////////////
	TAKE_SAFE_DEPOSIT tsd;
	memset(&tsd,0,sizeof(TAKE_SAFE_DEPOSIT));
	tsd.nUserID = pGameApp->GetUserID();		// �û�ID
	tsd.nGameID = pGameApp->GetGameID();		// ��ϷID
	tsd.nRoomID = pGameApp->GetRoomID();
	tsd.nTableNO = pGameApp->GetTableNO();
	tsd.nChairNO = pGameApp->GetChairNO();
	tsd.nDeposit = nTakeDeposit;				// ����
	tsd.nKeyResult = nKeyResult;				// ������ = func(��������, �����)
	//tsd.nPlayingGameID = 0;
	//tsd.nGameVID = 0;
	//tsd.nTransferTotal = 0;					//���������Ϸ���ܺ�(�۳�������)��ע�����Ϊ��ֵ��
	//tsd.nTransferLimit = 0;					//���������Ϸ���޶�����(��,>0)
	//tsd.dwIPAddr = 0;                         //IP��ַ
	if (FALSE == theApp.IsLookOn())
	{
		tsd.nGameDeposit = pGameApp->m_GamePlayer[pGameApp->GetChairNO()].nDeposit;    //��Ϸ����(����)
	}
	else
	{
		GAMEPLAYER* pPlayer = theApp.GetVisitorPlayerByUserID(theApp.GetUserID());

		if (NULL == pPlayer)
		{
			sRet.Format(GMS_OPERATE_FAILD);
			return FALSE;
		}
	
		tsd.nGameDeposit = pPlayer->nDeposit;    //��Ϸ����(����)
	}
	
	tsd.dwFlags = FLAG_SUPPORT_MONTHLY_LIMIT_EX;
	xyGetHardID(tsd.szHardID);					// Ӳ����ʶ�����������кţ�
	
	UINT nResponse;
	LPVOID pRet=NULL;
	int nDataLen=sizeof(TAKE_SAFE_DEPOSIT);
	BOOL bResult = ProcessRequest(sRet,GR_TAKE_SAFE_DEPOSIT, nDataLen, &tsd, nResponse, pRet);
	if(!bResult)//
		return FALSE;

    if(nResponse==UR_OPERATE_SUCCEEDED )///if succeed
	{
		sRet.Format(GMS_MOVESAFEDEPOSIT_SUCCEED);
		return TRUE;
	}
   	if(nResponse==GR_NODEPOSIT_GAME_EX)
	{
		CGameWinApp* pGameApp =  GetGameWinApp();
		sRet.Format(GMS_NODEPOSIT_GAME,pGameApp->GetGameName());
		return FALSE;
	}
	if(nResponse==GR_DEPOSIT_NOTENOUGH_EX)
	{
		sRet.Format(GMS_MOVESAFEDEPOSIT_NOTENOUGH);
		return FALSE;
	}  
	if(nResponse==UR_INVALID_PARAM)
	{
		sRet.Format(GMS_SECUREPWD_KEY_ERROR);
		return FALSE;
	}
	if(nResponse==GR_SAFEBOX_GAME_PLAYING)
	{
		sRet.Format(GMS_PLAYING_GAME_NOTTRANS);
		return FALSE;
	}
	if(nResponse==GR_SAFEBOX_GAME_READY)
	{
		sRet.Format(GMS_GAME_USER_START);
		return FALSE;
	}
	if(nResponse==GR_SAFEBOX_DEPOSIT_MAX)
	{
		int nMaxDeposit = *(int*)pRet;
		sRet.Format(GMS_MAX_DEPOSIT, nMaxDeposit);
		SAFE_DELETE_ARRAY(pRet);
		return FALSE;
	}
	if(nResponse==GR_CONTINUE_PWDWRONG_EX)
	{
		sRet.Format(GMS_CONTINUE_SECUREPWDERROR_TAKEDEPOSIT);
		return FALSE;
	}
	if(nResponse==GR_INPUTLIMIT_DAILY_EX)
	{
		CGameWinApp* pGameApp =  GetGameWinApp();
		int nTransferTotal=*(int*)pRet;
		int nTransferLimit=*(int*)((PBYTE)pRet+sizeof(int));
		int nRemain=nTransferLimit-nTransferTotal;

		if(nRemain>0)
		{
			sRet.Format(GMS_INPUTLIMIT_DAILY,pGameApp->GetGameName(),nTransferLimit ,nRemain);
		}
		else
		{
			sRet.Format(GMS_INPUTLIMIT_TOMORROW,pGameApp->GetGameName(),nTransferLimit);
		}

		SAFE_DELETE_ARRAY(pRet);
		return FALSE;
	}
	else if(nResponse==GR_INPUTLIMIT_MONTHLY_EX)
	{
		LONGLONG llTransfreTotal=*(LONGLONG*)pRet;
		LONGLONG llTransferLimit=*(LONGLONG*)((PBYTE)pRet+sizeof(LONGLONG));
		LONGLONG llRemain=llTransferLimit-llTransfreTotal;
		CString sFormat;
		if(llRemain>0)
		{
			sRet.Format(GMS_INPUTLIMIT_MONTHLY,llTransferLimit,llRemain);
		}
		else
		{
			sRet.Format(GMS_INPUTLIMIT_NEXTMONTH,llTransferLimit);
		}
		
		SAFE_DELETE_ARRAY(pRet);
		return FALSE;
	}
	
    GetSafeBoxReturnString(sRet,nResponse);
	return FALSE;
}

BOOL CGameRequest::GC_SaveDeposit(CString& sRet,int& nSaveDeposit, UINT& nResponse)
{
	CGameWinApp* pGameApp = GetGameWinApp();
	if (!pGameApp->IsTakeDepositInGame())
		return FALSE;
	
	sRet.Empty();
	///////////     Process  Request   ///////////////////////////////////////
	SAVE_SAFE_DEPOSIT ssd;
	memset(&ssd,0,sizeof(SAVE_SAFE_DEPOSIT));
	ssd.nUserID = pGameApp->GetUserID();		// �û�ID
	ssd.nGameID = pGameApp->GetGameID();		// ��ϷID
	ssd.nRoomID = pGameApp->GetRoomID();
	ssd.nTableNO = pGameApp->GetTableNO();
	ssd.nChairNO = pGameApp->GetChairNO();
	ssd.nDeposit = nSaveDeposit;				// ����
	//ssd.nKeyResult = 0;						// ������ = func(��������, �����)
	//ssd.nPlayingGameID = 0;
	//ssd.nGameVID = 0;
	//ssd.nTransferTotal = 0;					// ���������Ϸ���ܺ�(�۳�������)��ע�����Ϊ��ֵ��
	//ssd.nTransferLimit = 0;					// ���������Ϸ���޶�����(��,>0)
	//ssd.dwIPAddr = 0;                         // IP��ַ							
	if (FALSE == theApp.IsLookOn())
	{
		ssd.nGameDeposit = pGameApp->m_GamePlayer[pGameApp->GetChairNO()].nDeposit;    //��Ϸ����(����)
	}
	else
	{
		GAMEPLAYER* pPlayer = theApp.GetVisitorPlayerByUserID(theApp.GetUserID());
		
		if (NULL == pPlayer)
		{
			sRet.Format(GMS_OPERATE_FAILD);
			return FALSE;
		}
		
		ssd.nGameDeposit = pPlayer->nDeposit;    //��Ϸ����(����)
	}
	ssd.dwFlags = FLAG_SUPPORT_KEEPDEPOSIT_EX;
	xyGetHardID(ssd.szHardID);					// Ӳ����ʶ�����������кţ�
	
	//UINT nResponse;
	LPVOID pRet=NULL;
	int nDataLen=sizeof(SAVE_SAFE_DEPOSIT);
	BOOL bResult = ProcessRequest(sRet,GR_SAVE_SAFE_DEPOSIT, nDataLen, &ssd, nResponse, pRet);
	if(!bResult)//
		return FALSE;
	
    if(nResponse==UR_UPDATE_SUCCEEDED )///if succeed
		return TRUE;
   	if(nResponse==GR_NODEPOSIT_GAME_EX)
	{
		CGameWinApp* pGameApp =  GetGameWinApp();
		sRet.Format(GMS_NODEPOSIT_GAME,pGameApp->GetGameName());
		return FALSE;
	}
	if(nResponse==GR_DEPOSIT_NOTENOUGH_EX)
	{
		sRet.Format(GMS_TRANSFER_NOTENOUGH);
		return FALSE;
	}
	if(nResponse==GR_SAFEBOX_GAME_PLAYING)
	{
		sRet.Format(GMS_PLAYING_GAME_NOTTRANS);
		return FALSE;
	}
	if(nResponse==GR_SAFEBOX_GAME_READY)
	{
		sRet.Format(GMS_GAME_USER_START);
		return FALSE;
	}
	if(nResponse==GR_SAFEBOX_DEPOSIT_MIN)
	{
		int nMinDeposit = *(int*)pRet;
		sRet.Format(GMS_MIN_DEPOSIT, nMinDeposit);
		SAFE_DELETE_ARRAY(pRet);
		return FALSE;
	}
	if(nResponse==GR_BOUT_NOTENOUGH_EX)
	{
		int nMinBout=0;
		memcpy(&nMinBout,pRet,sizeof(int));
		SAFE_DELETE_ARRAY(pRet);
		sRet.Format(GMS_OUTPUT_BOUT_NOTENOUGH,nMinBout);
		return FALSE;
	}
	if(nResponse==GR_TIMECOST_NOTENOUGH_EX)
	{
		int nMinMinute=0;
		memcpy(&nMinMinute,pRet,sizeof(int));
		SAFE_DELETE_ARRAY(pRet);
		sRet.Format(GMS_OUTPUT_TIMECOST_NOTENOUGH,nMinMinute);
		return FALSE;
	}
	
	if (nResponse==GR_KEEPDEPOSIT_LIMIT_EX)
	{ 
		int nGameDeposit=*(int*)pRet;
		int nKeepDeposit=*(int*)((PBYTE)pRet+sizeof(int));
		SAFE_DELETE_ARRAY(pRet);
		sRet.Format(GMS_KEEPDEPOSIT_LIMIT, nGameDeposit-nKeepDeposit, nKeepDeposit);
		nSaveDeposit = nGameDeposit-nKeepDeposit;
		return FALSE;
	}
	
    GetSafeBoxReturnString(sRet,nResponse);
	return FALSE;
}

BOOL CGameRequest::GC_AskNewTableChair(CString& sRet)
{
	CGameWinApp* pGameApp =  GetGameWinApp();
	sRet.Empty();
	
	ASK_NEWTABLECHAIR antc;
	memset(&antc,0,sizeof(ASK_NEWTABLECHAIR));
	
	antc.nUserID = pGameApp->GetUserID();
	antc.nRoomID = pGameApp->GetRoomID();
	antc.nTableNO = pGameApp->GetTableNO();
	antc.nChairNO = pGameApp->GetChairNO();
	
	///////////     Process  Request   ///////////////////////////////////////
	UINT nResponse;
	LPVOID pRet=NULL;
	int nDataLen=sizeof(ASK_NEWTABLECHAIR);
	BOOL bResult = ProcessRequest(sRet,GR_ASK_NEW_TABLECHAIR, nDataLen,&antc, nResponse, pRet);
	
	if (!bResult)
		return FALSE;
	
	if (nResponse == UR_OPERATE_SUCCEEDED)
	{
		SAFE_DELETE_ARRAY(pRet);
		return TRUE;
	}
	else if (nResponse == UR_ASK_NEW_TABLE_DEPOSIT_NOT_ENOUGH)
	{
		sRet=_T(" <z=tcϵͳ֪ͨ> <c=255>��Я�����������㣬�޷�������\r\n");
		GetGameWinApp()->ShowMsgInChatView(sRet);
		RemindTakeDeposit();
		SAFE_DELETE_ARRAY(pRet);
		return FALSE;
	}
	else if (nResponse == GR_RESPONE_ASKNEWTABLE_SPANSHORT)
	{
		sRet=_T(" <z=tcϵͳ֪ͨ> <c=255>���Ļ�����������Ƶ�������Ե�Ƭ��!\r\n");
		GetGameWinApp()->ShowMsgInChatView(sRet);
		SAFE_DELETE_ARRAY(pRet);
		return FALSE;
	}
	
	SAFE_DELETE_ARRAY(pRet);
    return FALSE;
}

BOOL  CGameRequest::GC_SyncStartCountdown(int nCountdown)
{
	SYNC_COUNTDOWN  sc;
	memset(&sc,0,sizeof(SYNC_COUNTDOWN));
	sc.nUserID=GetGameWinApp()->GetUserID();
	sc.nRoomID=GetGameWinApp()->GetRoomID();
	sc.nTableNO=GetGameWinApp()->GetTableNO();
	sc.nChairNO=GetGameWinApp()->GetChairNO();
	sc.nCountDown = nCountdown;
	///////////     Process  Request   ///////////////////////////////////////
	CString sRet;
	UINT nResponse;
	LPVOID pRet=NULL;
	int nDataLen=sizeof(SYNC_COUNTDOWN);
	BOOL bResult = ProcessRequest(sRet,GR_SYNC_START_COUNTDOWN, nDataLen, &sc, nResponse, pRet,FALSE);
   	return bResult;
}

BOOL CGameRequest::GC_ChangeSeatedToLookOn(CString& sRet)
{
	CGameWinApp* pGameApp =  GetGameWinApp();
	sRet.Empty();
	
	if (pGameApp->IsLookOn())
	{
		sRet=_T(" <z=tcϵͳ֪ͨ> <c=255>���Ѿ����Թ�״̬�ˡ�\r\n");
		return FALSE;
	}

	LOOK_ON_AND_SEATED_CHANGE  sc;
	memset(&sc,0,sizeof(LOOK_ON_AND_SEATED_CHANGE));
	sc.nUserID	= GetGameWinApp()->GetUserID();
	sc.nRoomID	= GetGameWinApp()->GetRoomID();
	sc.nTableNO	= GetGameWinApp()->GetTableNO();
	sc.nChairNO	= GetGameWinApp()->GetChairNO();
	///////////     Process  Request   ///////////////////////////////////////

	UINT nResponse;
	LPVOID pRet	= NULL;
	int nDataLen = sizeof(LOOK_ON_AND_SEATED_CHANGE);
	BOOL bResult = ProcessRequest(sRet,GR_SEATED_TO_LOOK_ON, nDataLen, &sc, nResponse, pRet);
   	
	if (FALSE == bResult)
	{
		sRet=_T(" <z=tcϵͳ֪ͨ> <c=255>����ʧ�ܣ��޷��Թۣ����Ժ����ԡ�\r\n");
	}
	else
	{
		switch(nResponse)
		{
		case UR_OPERATE_FAILED:
			sRet=_T(" <z=tcϵͳ֪ͨ> <c=255>����ʧ�ܣ��޷��Թۣ����Ժ����ԡ�\r\n");
			bResult = FALSE;
			break;
		case UR_CHANGE_LOOKON_OR_SEATED_SPANSHORT:
			sRet=_T(" <z=tcϵͳ֪ͨ> <c=255>�����Թ۲�������Ƶ�������Ե�Ƭ��!\r\n");
			bResult = FALSE;
			break;
		case UR_CHANGE_LOOKON_OR_SEATED_SAME_STATUS:
			sRet=_T(" <z=tcϵͳ֪ͨ> <c=255>���Ѿ����Թ�״̬�ˡ�\r\n");
			bResult = FALSE;
			break;
		case UR_CHANGE_LOOKON_OR_SEATED_FULL_LOOK_ON:
			sRet=_T(" <z=tcϵͳ֪ͨ> <c=255>�Թ������Ѵ����ޣ��޷��Թۡ�\r\n");
			bResult = FALSE;
			break;
		case UR_OPERATE_SUCCEEDED:
		default:
			break;
		}
	}
	
	SAFE_DELETE_ARRAY(pRet);
    return bResult;
}
			
BOOL CGameRequest::GC_ChangeLookOnToSeated(CString& sRet)
{
	CGameWinApp* pGameApp =  GetGameWinApp();
	sRet.Empty();
	
	if (FALSE == pGameApp->IsLookOn())
	{
		sRet=_T(" <z=tcϵͳ֪ͨ> <c=255>���Ѿ�������״̬�ˡ�\r\n");
		return FALSE;
	}
	
	LOOK_ON_AND_SEATED_CHANGE  sc;
	memset(&sc,0,sizeof(LOOK_ON_AND_SEATED_CHANGE));
	sc.nUserID	= GetGameWinApp()->GetUserID();
	sc.nRoomID	= GetGameWinApp()->GetRoomID();
	sc.nTableNO	= GetGameWinApp()->GetTableNO();
	sc.nChairNO	= GetGameWinApp()->GetChairNO();
	///////////     Process  Request   ///////////////////////////////////////
	
	UINT nResponse;
	LPVOID pRet	= NULL;
	int nDataLen = sizeof(LOOK_ON_AND_SEATED_CHANGE);
	BOOL bResult = ProcessRequest(sRet,GR_LOOK_ON_TO_SEATED, nDataLen, &sc, nResponse, pRet);
   	
	if (FALSE == bResult)
	{
		sRet=_T(" <z=tcϵͳ֪ͨ> <c=255>����ʧ�ܣ��޷����£����Ժ����ԡ�\r\n");
	}
	else
	{
		switch(nResponse)
		{
		case UR_OPERATE_FAILED:
			sRet=_T(" <z=tcϵͳ֪ͨ> <c=255>����ʧ�ܣ��޷����£����Ժ����ԡ�\r\n");
			bResult = FALSE;
			break;
		case UR_CHANGE_LOOKON_OR_SEATED_SPANSHORT:
			sRet=_T(" <z=tcϵͳ֪ͨ> <c=255>�������²�������Ƶ�������Ե�Ƭ��!\r\n");
			bResult = FALSE;
			break;
		case UR_CHANGE_LOOKON_OR_SEATED_SAME_STATUS:
			sRet=_T(" <z=tcϵͳ֪ͨ> <c=255>���Ѿ�������״̬�ˡ�\r\n");
			bResult = FALSE;
			break;
		case UR_CHANGE_LOOKON_OR_SEATED_DEPOSIT_NOT_ENOUGH:
			sRet=_T(" <z=tcϵͳ֪ͨ> <c=255>��Я�����������㣬�޷����¡�\r\n");
			RemindTakeDeposit();
			bResult = FALSE;
			break;
		case UR_CHANGE_LOOKON_OR_SEATED_NO_EMPTY_CHAIR:
			sRet=_T(" <z=tcϵͳ֪ͨ> <c=255>û�п������λ�ˣ����Ժ����ԡ�\r\n");
			bResult = FALSE;
			break;
		case UR_OPERATE_SUCCEEDED:
		default:
			break;
		}
	}
	
	SAFE_DELETE_ARRAY(pRet);
    return bResult;
}

void CGameRequest::RemindTakeDeposit()
{
	DEPOSIT_NOT_ENOUGH dth;
	memset(&dth,0,sizeof(DEPOSIT_NOT_ENOUGH));
	
	int nChangeUserID = GetGameWinApp()->GetUserID();
	GAMEPLAYER* pPlayer = theApp.GetVisitorPlayerByUserID(nChangeUserID);
	dth.nDeposit = pPlayer->nDeposit;
	dth.nMinDeposit = GetGameWinApp()->m_GameRoom.nMinDeposit;
	GetGameWinApp()->OnDepositNotEnough((LPVOID*)&dth,0);
}