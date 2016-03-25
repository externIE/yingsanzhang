// GameNotify.cpp: implementation of the CGameNotify class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "game.h"
#include "GameNotifyEx.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CGameNotifyEx::CGameNotifyEx()
{
 
}

CGameNotifyEx::~CGameNotifyEx()
{

}

void CGameNotifyEx::OnNotifyReceived( int nRequestID, VOID* pDataPtr,int nSize)
{
	switch(nRequestID)
	{
	case GR_START_GAME:
		NTF_StartGame((START_GAME*)pDataPtr);
		break;
	case GR_GAME_START:
		NTF_GameStart((GAME_START_INFO*)pDataPtr);
		break;
	case GR_SENDMSG_TO_PLAYER:
 		NTF_GameMsg((GAME_MSG*)pDataPtr);
 		break;
	case GR_GAME_WIN:
		NTF_GameWin((GAME_WIN_RESULT*)pDataPtr);
		break;
	case GR_TOCLIENT_TABLEINFO:
		NTF_GameTableInfo((GAME_TABLE_INFO*)pDataPtr);
		break;
	case GR_TOCLIENT_OFFLINE:
		NTF_PlayerOffline((LPPLAYER_OFFLINE)pDataPtr);
		break;
	case GR_WAIT_NEWTABLE:
		NTF_WaitNewTable();
		break;
	case GR_START_COUNTDOWN:
		NTF_StartCountdown((START_COUNTDOWN*)pDataPtr);
		break;
	case GR_SYNC_START_COUNTDOWN:
		NTF_SyncStartCountdown((SYNC_COUNTDOWN*)pDataPtr);
		break;
	case GR_CHANGE_SEATED_TO_LOOK_ON_RESULT:
		NTF_ChangeSeatedToLookOn((LOOK_ON_AND_SEATED_CHANGE_RESULT*)pDataPtr);
		break;
	case GR_CHANGE_LOOK_ON_TO_SEATED_RESULT:
		NTF_ChangeLookOnToSeated((LOOK_ON_AND_SEATED_CHANGE_RESULT*)pDataPtr);
		break;
	case GR_HIDE_GIVE_UP_BUTTON:
		NTF_GetGiveUpBtnConfig((int*) pDataPtr);
	default:
		CGameNotify::OnNotifyReceived(nRequestID,pDataPtr,nSize);	//这里需要return，防止重复存消息
		return;
	}

    CGameApp* pApp=(CGameApp*)AfxGetApp();
	if (pApp->IsReplayMode())
		return;

	//始终记录存盘，除非是半路进来的旁观玩家，或者断线续完，这种步骤残缺的
	if (!pApp->IsSaveReplay())
		return;
}

void CGameNotifyEx::NTF_GameTableInfo(GAME_TABLE_INFO* pGameTableInfo)
{
	CGameApp* pApp=(CGameApp*)AfxGetApp();
	((CMyGame*)pApp->GetMainGame())->OPE_InitData();
	
	memcpy(pApp->GetTableInfo(),pGameTableInfo,sizeof(GAME_TABLE_INFO));

	if (pApp->IsMatch())
	{
		((CSubSystem*)pApp->GetSubSystem())->OnEnterMatchDXXW();
		((CMyGame*)pApp->GetMainGame())->OnEnterDXXW();
	}
	else
	{
		((CSubSystem*)pApp->GetSubSystem())->OnEnterGameDXXW();
		((CMyGame*)pApp->GetMainGame())->OnEnterDXXW();
	}

}

void CGameNotifyEx::NTF_GameAbort(GAME_ABORT* pGameAbort)
{	
	CGameApp* pApp=(CGameApp*)AfxGetApp();
	CGameFrameWnd * pFrame=(CGameFrameWnd*)AfxGetMainWnd();

	CGameNotify::NTF_GameAbort(pGameAbort);//结束游戏并且断开与服务器连接

	pFrame->SetForegroundWindow();
	
	CString sMsg;
	if(pGameAbort->bForce )
	{
		sMsg.Format(_T("%s 已经强行退出，游戏结束。(扣除%d两,%d分)"),
			pApp->GetPlayerNameByChairNO(pGameAbort->nChairNO),
			-pGameAbort->nDepositDfif,
			-pGameAbort->nScoreDiff);
	}
	else
	{
		sMsg.Format(_T("%s 已经离开，游戏结束。"),
			pApp->GetPlayerNameByChairNO(pGameAbort->nChairNO),pGameAbort->nChairNO);
	}	 

	pApp->ShowQuitTimerBox(sMsg,10,TIMERDLG_FLAGS_ICON|TIMERDLG_FLAGS_DANYMIC,"xyf抱歉");
}

void CGameNotifyEx::NTF_PlayerAbort(GAME_ABORT * pGameAbort)
{	
	CGameWinApp* pGameApp=(CGameWinApp*)AfxGetApp();
	
	//////////////////////////////////////////////////////////////////////////
	if(pGameApp->IsGameRunning()&& !(IS_BIT_SET(pGameApp->GetRoomConfigs(), RC_VARIABLE_CHAIR)))
		return ;
	//////////////////////////////////////////////////////////////////////////
	
	//modify by pengsy at 20130821，增加userid，chairno，tableno验证
	GAMEPLAYER * pGamePlayer=pGameApp->GetGamePlayerByUserID(pGameAbort->nUserID);//
	if (pGamePlayer)
	{
		if(pGamePlayer->nTableNO!=pGameAbort->nTableNO )
			return;
		if(pGamePlayer->nChairNO!=pGameAbort->nChairNO )
			return;
		
		int chairno=pGameAbort->nChairNO;
		BOOL bLookOn = pGamePlayer->bLookOn;
		CString abortPlayerName = pGamePlayer->szUsername;
		
		if (pGameAbort->nChairNO == pGameApp->GetChairNO())
		{
// 			if (pGameApp->IsLookOn())
// 			{ //旁观的对象已经退出
// 				CString sMsg;
// 				sMsg.Format(_T("%s 已经离开，旁观结束。"),abortPlayerName);
// 				int nLifeTime=10;
// 				
// 				pGameApp->ShowQuitTimerBox(sMsg,nLifeTime,TIMERDLG_FLAGS_ICON|TIMERDLG_FLAGS_DANYMIC,"xyf抱歉");
// 			}
// 			else 
			if (pGameAbort->nAbortFlag)	//pengsy
			{
				int nLifeTime=5;
				
				pGameApp->ShowQuitTimerBox(_T("其他玩家同意您退出游戏！"),10,TIMERDLG_FLAGS_ICON|TIMERDLG_FLAGS_DANYMIC,"xyf胜利");
			}
		}
		
		pGameApp->GetGameFrame()->NTF_PlayerAbort(pGamePlayer);

		if (FALSE == bLookOn)
		{
			memset(&pGameApp->m_GamePlayer[chairno],0,sizeof(GAMEPLAYER));
			
			pGameApp->m_GameTable.nPlayerCount--;
			
			if (pGameApp->m_GameTable.nPlayerCount<0)
			{
				pGameApp->m_GameTable.nPlayerCount = 0;
			}
			
			memset(pGamePlayer,0,sizeof(GAMEPLAYER));
		}
		else
		{
			theApp.RemoveVisitorPlayerByUserID(pGameAbort->nUserID);
		}
	}
}

void CGameNotifyEx::NTF_StartGame(START_GAME* pStartGame)
{ 
	CGameApp* pApp=(CGameApp*)AfxGetApp();
	CGameFrameWnd* pFrame=(CGameFrameWnd*)AfxGetMainWnd();

	((CGameInfoEx*)pApp->GetGameInfo())->NTF_StartGame(pStartGame);
	((CMyGame*)pApp->GetMainGame())->NTF_StartGame(pStartGame->nChairNO);
}

// 
void CGameNotifyEx::NTF_GameStart(GAME_START_INFO * pStartData)
{
	CGameApp* pApp=(CGameApp*)AfxGetApp();
	CGameFrameWnd* pFrame=(CGameFrameWnd*)AfxGetMainWnd();
	
	//游戏开始，必须执行此函数，否则会有严重错误
	////////////////////////////////////////////
	pApp->GameRun();
	///////////////////////////////////////////

	((CGameInfoEx*)pApp->GetGameInfo())->NTF_GameStart(pStartData);
	((CMyGame*)pApp->GetMainGame())->NTF_GameStart();
}


void CGameNotifyEx::NTF_GameMsg(GAME_MSG* pGameMsg)
{
	CGameApp* pApp=(CGameApp*)AfxGetApp();
	((CMyGame*)pApp->GetMainGame())->NTF_GameMsg(pGameMsg);
}

void CGameNotifyEx::NTF_GameWin(GAME_WIN_RESULT* pGameWin)
{
	CGameApp* pApp=(CGameApp*)AfxGetApp();
	//因最后二家比牌会同时返回Game_win信息，需要暂存，待闪电等动漫放完再显示结果

	//游戏结束，必须执行此函数，否则会有严重错误
	/////////////////////////////////////////////
	pApp->GameStop();
	/////////////////////////////////////////////

	((CGameInfoEx*)pApp->GetGameInfo())->NTF_GameWin(pGameWin);

	pApp->NotifyGameWin((GAME_WIN*)pGameWin);

	((CMyGame*)pApp->GetMainGame())->NTF_GameWin();
}


void CGameNotifyEx::NTF_PlayerOffline(LPPLAYER_OFFLINE lpOffline)
{
	CGameApp* pApp=(CGameApp*)AfxGetApp();
	((CMyGame*)pApp->GetMainGame())->NTF_PlayerOffline(lpOffline->nUserID);
}

void CGameNotifyEx::NTF_ChatFromTable(CHAT_FROM_TABLE* pChatFromTable)
{
	CString str;
	str.Format(_T("<c=%d>\r\n"),RGB(64,128,128));
	const TCHAR* tstr = (LPSTR)(LPCSTR)str;
	//过滤掉玩家消息中的空消息
	if (0 == _tcscmp(pChatFromTable->szChatMsg, (LPSTR)(LPCSTR)str))
		return;
	
	CGameNotify::NTF_ChatFromTable(pChatFromTable);
}

void CGameNotifyEx::NTF_GameStartSolo(LPVOID pStartInfo)
{
	NTF_GameStart((GAME_START_INFO*)pStartInfo);
}

void CGameNotifyEx::NTF_WaitNewTable()
{
	if (theApp.IsLookOn())return;  

	CGameWinApp* pApp=(CGameWinApp*)AfxGetApp();
    if(!pApp->IsGameRunning())
		((CMyGame*)pApp->GetMainGame())->ShowWaitArrageTable();
}
 
void CGameNotifyEx::NTF_StartCountdown(START_COUNTDOWN* pCountdown)
{
	CGameApp* pApp=(CGameApp*)AfxGetApp();
	((CMyGame*)pApp->GetMainGame())->NTF_StartCountdown(pCountdown);
}

void CGameNotifyEx::NTF_SyncStartCountdown(SYNC_COUNTDOWN* pSyncCountdown)
{
	CGameApp* pApp=(CGameApp*)AfxGetApp();
	((CMyGame*)pApp->GetMainGame())->NTF_SyncStartCountdown(pSyncCountdown);
}

void CGameNotifyEx::NTF_DepositNotEnough(DEPOSIT_NOT_ENOUGH*  pDepositNotEnough)
{//银两不足变为旁观状态
	CGameApp* pApp=(CGameApp*)AfxGetApp();
	SNG_MSG msg;
	memset(&msg,0,sizeof(SNG_MSG));
	msg.msgID = MSG_BUTTON_STAND_BY;
	((CMyGame*)pApp->GetMainGame())->OM_DealBTN(msg);
}

void CGameNotifyEx::SaveLastNotify(int nRequestID,VOID* pDataPtr,int nSize)
{
	m_nLastRequestID=nRequestID;

	if (nRequestID==GR_SENDMSG_TO_PLAYER)
		m_nLastSubRequestID=((GAME_MSG*)pDataPtr)->nMsgID;
	else
		m_nLastSubRequestID=0;

	m_nLastDataSize=nSize;
    m_dwLastRequestTick=GetTickCount();
}

void CGameNotifyEx::NTF_ChangeSeatedToLookOn(LOOK_ON_AND_SEATED_CHANGE_RESULT* ploascr)
{
	CGameWinApp* pGameApp=(CGameWinApp*)AfxGetApp();
	((CMyGame*)pGameApp->GetMainGame())->NTF_ChangeSeatedToLookOn(ploascr);
}

void CGameNotifyEx::NTF_ChangeLookOnToSeated(LOOK_ON_AND_SEATED_CHANGE_RESULT* ploascr)
{
	CGameWinApp* pGameApp=(CGameWinApp*)AfxGetApp();
	((CMyGame*)pGameApp->GetMainGame())->NTF_ChangeLookOnToSeated(ploascr);
}

void CGameNotifyEx::NTF_GetGiveUpBtnConfig(int* nOpen)
{
	CGameApp* pGameApp = (CGameApp*)AfxGetApp();
	CMyGame* pMyGame = (CMyGame*)pGameApp->GetMainGame();
    if (pGameApp)
    {
        pMyGame->NTF_GetGiveUpBtnConfig(nOpen);
    }
}