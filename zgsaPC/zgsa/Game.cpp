// Game.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Game.h"

//#include "vld.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// CGameApp
using namespace ShinyNova;

BEGIN_MESSAGE_MAP(CGameApp, CGameWinApp)
	//{{AFX_MSG_MAP(CGameApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGameApp construction

extern int   MsgBoxEx(HWND   hwnd,   TCHAR   *szText,   TCHAR   *szCaption,   UINT   uType)     ;

CGameApp::CGameApp():CGameWinApp(APP_NAME,GAME_PORT,VER_MAJOR,VER_MINOR)
{
   	memset(&m_GameSet,0,sizeof(m_GameSet));
}

CGameApp::~CGameApp()
{

}
/////////////////////////////////////////////////////////////////////////////
// The one and only CGameApp object
CGameTable m_MyGameTable;
CGameApp theApp;
HANDLE g_hMutex = NULL; 
HHOOK   hMsgBoxHook = NULL;   
BOOL bOpen= FALSE;
/////////////////////////////////////////////////////////////////////////////

int  CGameApp::GetTableChairCount()
{
	return TOTAL_CHAIRS;
}

CXyGameFrame* CGameApp::CreateLogicFrame()
{
	return new CMyGameFrame;
}


void CGameApp::UpdateTableInfo()
{
	if (GetOfflineFlag()==OFFLINEFLAG_OFFLINE)
		return;
	
	CGameFrameWnd* pFrame=(CGameFrameWnd*)AfxGetMainWnd();
	CMyGame* pGame=(CMyGame*)GetMainGame();
	
	pGame->OPE_ClearGame();
	pGame->OPE_DXXW();
	
	//提示用户自启动游戏程序
	SetRunSelf(TRUE);
	CString sMsg;
	sMsg.Format(_T("您的网络出现了异常，请重新登录大厅。"));
	ShowQuitTimerBox(sMsg,10);
	
	//	if( AfxGetMainWnd()==NULL)
	//		return ;
	//	DisConnectGameSvr();
	//	pFrame->PostMessage(WM_CLOSE,0,0);//先退出
}


void CGameApp::PlayGameSound(UINT nSoundID,UINT nFlag)
{
	if(IsSilence())
		return ;
	xyPlayWAVByFlag(nSoundID,nFlag);
}


BOOL CGameApp::InitInstance()
{
//	InitAppSet();                   //初始化程序设置
    ReadGameSet();					//获取游戏初始化设置
//	CheckReplayMode();
#ifdef _OFFLINE_
	SetOfflineFlag(OFFLINEFLAG_OFFLINE);
#endif
	if (IsReplayMode())
	{
		
	}
	if (GetOfflineFlag()==OFFLINEFLAG_OFFLINE)
	{
		m_GameInit.nUserID=0;
		m_GameInit.nAreaID=1;
		m_GameInit.nGameID=30;
		m_GameInit.nRoomID=0;
		m_GameInit.nTableNO=0;
		m_GameInit.nChairNO=0;
		m_GameInit.hRoomWnd=0;
		m_GameInit.bLookOn =FALSE;
		m_GameInit.bReEnter=FALSE;
		lstrcpy(m_GameInit.szGameIP,_T("127.0.0.1"));
		lstrcpy(m_GameInit.szGameName,_T("诈金花"));
	}

	if(!CGameWinApp::InitInstance())//进行SDI初始化，创建通讯线程，连接游戏服务器
		return FALSE;    

     if(!CreateGameFrame())          //创建游戏界面框架
		return FALSE;  
 

	if (GetOfflineFlag()==OFFLINEFLAG_OFFLINE && !IsReplayMode())
	{
		m_MatchTable.nChairCount=TOTAL_CHAIRS;
 		m_GameTable.nPlayerCount=TOTAL_CHAIRS;
		lstrcpy(m_GameRoom.szWWW,_T("www.uc108.com"));
 		lstrcpy(m_GameRoom.szWWW2,_T("www2.youxi518.com"));
		lstrcpy(m_GameRoom.szRoomName,_T("古越斋"));
		lstrcpy(m_GameRoom.szAreaName,_T("绍兴电信1站"));
		for(int i=0;i<TOTAL_CHAIRS;i++)
		{
		//	sprintf(m_GamePlayer[i].szUsername,_T("长崎雨_%d今夜认识"),i);
					sprintf(m_GamePlayer[i].szUsername,_T("长崎雨_%d今夜"),i);
			sprintf(m_GamePlayer[i].szNickName,_T("wsk_%d"),i);
			m_GamePlayer[i].nChairNO=i;
			m_GamePlayer[i].nTableNO=0;
			m_GamePlayer[i].nUserID =10+i;
			m_GamePlayer[i].nScore=rand()%999999;

			m_GamePlayer[i].nWin=rand()%1000;
			m_GamePlayer[i].nLoss=rand()%1000;
			m_GamePlayer[i].nStandOff=rand()%1000;
			m_GamePlayer[i].nBout=m_GamePlayer[i].nWin+m_GamePlayer[i].nLoss+m_GamePlayer[i].nStandOff;
			m_GamePlayer[i].nBreakOff=rand()%1000;
			m_GamePlayer[i].nNetSpeed=100+rand()%1000;
			m_GamePlayer[i].nExperience=rand()%1000000;
			
			//////////////////////
			sprintf(m_Visitor[i].szUsername ,_T("超级注射器%i"),i);
			sprintf(m_Visitor[i].szNickName,_T("美工"));
			m_Visitor[i].nChairNO=i;
			m_Visitor[i].nUserID =20+i;
			m_Visitor[i].bLookOn =1;
		}
		
		sprintf(m_GamePlayer[0].szNickName,_T("玩家"));
		sprintf(m_GamePlayer[1].szNickName,_T("策划"));
		sprintf(m_GamePlayer[2].szNickName,_T("策划"));
		sprintf(m_GamePlayer[3].szNickName,_T("程序"));
		
		sprintf(m_GamePlayer[0].szUsername,_T("玩家大人需要八字"));
		sprintf(m_GamePlayer[1].szUsername,_T("Rogerhi"));
		sprintf(m_GamePlayer[2].szUsername,_T("123456789"));
		sprintf(m_GamePlayer[3].szUsername,_T("黑木大人是猪"));

		sprintf(m_GamePlayer[0].szLevelName,"一段");
		sprintf(m_GamePlayer[1].szLevelName,"判官");
		sprintf(m_GamePlayer[2].szLevelName,"美国总统大人");
		sprintf(m_GamePlayer[3].szLevelName,"超级大富翁");
		
		m_GamePlayer[0].nPortrait=0;
		m_GamePlayer[1].nPortrait =10;
		m_GamePlayer[2].nPortrait =9;
		m_GamePlayer[3].nPortrait =10;

		srand(GetTickCount());
		m_GamePlayer[0].nClothingID =rand()%116;
		m_GamePlayer[1].nClothingID =rand()%116;
		m_GamePlayer[2].nClothingID =70;
		m_GamePlayer[3].nClothingID =rand()%116;

		
		m_GamePlayer[0].nDeposit=99999999;
		m_Visitor[0].nPortrait =10;
		m_GamePlayer[0].nPortrait=10;
		m_GamePlayer[1].nPortrait =9;

	
		int result[MAX_CHAIR_COUNT];
		memset(result,0,sizeof(result));
		for(int k=0;k<100;k++)
		{
			for(int i=0;i<TOTAL_CHAIRS;i++)
			{
				result[i]=1000000+(k+1)*100+i;
			}
			SaveResultDiff(result);
		}

		UpdataResultList();

		AfxGetMainWnd()->PostMessage(WM_INIT_DATA,0,0);
	}


	if (theApp.IsReplayMode())
	{
		m_GameTable.nPlayerCount=TOTAL_CHAIRS;
		AfxGetMainWnd()->PostMessage(WM_INIT_DATA,0,0);
	}
	
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CGameApp message handlers
DWORD CGameApp::GetUserConfig()
{
	return 0;
}

CGameInfo* CGameApp::GetGameInfo()
{
  return &m_GameInfoEx;
}

VOID*  CGameApp::GetTableInfo()
{
 	return (VOID*)&m_GameInfoEx.m_GameTableInfo;
}

VOID*  CGameApp::GetEnterInfo()
{
	return (VOID*)&m_GameInfoEx.m_GameEnterInfo;
}

void CGameApp::ReadGameSet()
{
	CString sConfigFile=GetGameConfigFile();
	m_GameSet.nSoundEffect =GetPrivateProfileInt ( _T("GAMESET"), _T("SoundEffect"), 0, sConfigFile) ; 
	m_GameSet.nMusic =GetPrivateProfileInt ( _T("GAMESET"), _T("Music"), 0, sConfigFile) ; 
	m_GameSet.nRejectKanPaiAsk=GetPrivateProfileInt ( _T("GAMESET"), _T("RejectKanPaiAsk"), 0, sConfigFile) ;
	m_GameSet.nRejectPassSound=GetPrivateProfileInt ( _T("GAMESET"), _T("RejectPassSound"), 0, sConfigFile) ;
	m_GameSet.nPokerType=GetPrivateProfileInt ( _T("GAMESET"), _T("PokerType"), 0, sConfigFile) ;
}

void CGameApp::FreshGameSet()
{

}

void CGameApp::SaveGameSet()
{	
	CString sConfigFile=GetGameConfigFile();
	
	TCHAR szText[256];
	memset(szText,0,sizeof(szText));
	
	sprintf(szText,_T("%d"),m_GameSet.nSoundEffect);
	WritePrivateProfileString ( _T("GAMESET"),_T("SoundEffect"),szText,  sConfigFile) ; 
	
	sprintf(szText,_T("%d"),m_GameSet.nMusic);
	WritePrivateProfileString ( _T("GAMESET"),_T("Music"),szText,  sConfigFile); 

	sprintf(szText,_T("%d"),m_GameSet.nRejectKanPaiAsk) ;
	WritePrivateProfileString ( _T("GAMESET"),_T("RejectKanPaiAsk"),szText,sConfigFile) ; 

	sprintf(szText,_T("%d"),m_GameSet.nRejectPassSound) ;
	WritePrivateProfileString ( _T("GAMESET"),_T("RejectPassSound"),szText,sConfigFile) ; 


	sprintf(szText,_T("%d"),m_GameSet.nPokerType) ;
	WritePrivateProfileString ( _T("GAMESET"),_T("PokerType"),szText,sConfigFile) ; 
}

void CGameApp::FreshScores()
{
// 	CMainFrame* pFrame=(CMainFrame*)AfxGetMainWnd();
// 	pFrame->GetGamePlayerView()->NTF_GameWin(&m_gamewin);
// 
}

void CGameApp::SetVisitorPlayerBySoloPlayer(GAMEPLAYER* pgp, SOLO_PLAYER* psp)
{
	pgp->bLookOn = TRUE;
	pgp->nUserID=psp->nUserID;
	pgp->nUserType=psp->nUserType;
	pgp->nStatus=psp->nStatus;
	pgp->nTableNO=psp->nTableNO;
	pgp->nChairNO=psp->nChairNO;
	pgp->nNickSex=psp->nNickSex;
	pgp->nPortrait=psp->nPortrait;
	pgp->nNetSpeed=psp->nNetSpeed;
	pgp->nClothingID=psp->nClothingID;
	
	pgp->nDeposit=psp->nDeposit;
	pgp->nPlayerLevel=psp->nPlayerLevel;
	pgp->nScore=psp->nScore;
	pgp->nBreakOff=psp->nBreakOff;
	pgp->nWin=psp->nWin;
	pgp->nLoss=psp->nLoss;
	pgp->nStandOff=psp->nStandOff;
	pgp->nBout=psp->nBout;
	pgp->nTimeCost=psp->nTimeCost;
	
	lstrcpy(pgp->szUsername,psp->szUsername);
	lstrcpy(pgp->szNickName,psp->szNickName);
	
	lstrcpy(pgp->szLevelName,GetLevelByID(psp->nPlayerLevel));//本地字段
	pgp->nExperience =psp->nTimeCost / 60 ;//本地字段
}

GAMEPLAYER* CGameApp::GetEmptyVisitorPlayer()
{
	for(int i=0; i<TOTAL_VISITOR_COUNT; i++)
	{
		if (0 == m_Visitor[i].nUserID)
			return &m_Visitor[i];
	}

	return NULL;
}

GAMEPLAYER* CGameApp::GetVisitorPlayerByUserID(int nUserID)
{
	GAMEPLAYER* pPlayer = NULL;
	for(int i=0; i<TOTAL_VISITOR_COUNT; i++)
	{
		if(m_Visitor[i].nUserID == nUserID)
		{
			pPlayer = &m_Visitor[i];
			break;
		}
	}

	return pPlayer;
}

BOOL CGameApp::RemoveVisitorPlayerByUserID(int nUserID)
{
	for(int i=0; i<TOTAL_VISITOR_COUNT; i++)
	{
		if(m_Visitor[i].nUserID == nUserID)
		{
			memset(&m_Visitor[i], 0, sizeof(GAMEPLAYER));
			return TRUE;
		}
	}

	return FALSE;
}

void CGameApp::RemoveAllVisitorPlayer()
{
	for(int j=0; j<TOTAL_VISITOR_COUNT; j++)
	{
		if(m_Visitor[j].nUserID != 0)
			memset(&m_Visitor[j], 0, sizeof(GAMEPLAYER));
	}
}

CGameConnect* CGameApp::GetGameConnect()
{
	return &m_GameRequest;
}

// CGameSkin* CGameApp::GetGameSkin()
// {
// 	return &m_GameSkinEx;
// }

CGameNotify* CGameApp::GetGameNotify()
{
	return &m_GameNotifyEx;
}

	
int CGameApp::ExitInstance() 
{
// 	CString fontpath;
// 	fontpath.Format("skins\\%s.tff",GetArtFontType());
// 	CString str=GetGamePath(fontpath);
// 	::RemoveFontResource(str);
//	SendMessage(HWND_BROADCAST,WM_FONTCHANGE,0,0);
  	SaveGameSet();
  	return CGameWinApp::ExitInstance();
}
 

LRESULT   CALLBACK   CBTProc(int   nCode,   
							 WPARAM   wParam,   
							 LPARAM   lParam   
							 )   
{   
	if(nCode   ==   HCBT_ACTIVATE)   
	{   
		if (bOpen==FALSE)
		{
			HWND   hWnd   =   (HWND)wParam;   
			CRect   rect;
			GetWindowRect(hWnd,   &rect);
			CGameFrameWnd* pFrame=(CGameFrameWnd*)AfxGetMainWnd();
			CRect rcTable;
			pFrame->GetWindowRect(&rcTable);
			MoveWindow(hWnd,  rcTable.Width()-271,   36,   rect.Width(),   rect.Height(),   FALSE); 
			bOpen = TRUE;
		}		
	}   
	return   CallNextHookEx(hMsgBoxHook,   nCode,   wParam,   lParam);
}   


int   MsgBoxEx(HWND   hwnd,   TCHAR   *szText,   TCHAR   *szCaption,   UINT   uType)     
{     
	int   retval;     
	bOpen = FALSE;
	//   Install   a   window   hook,   so   we   can   intercept   the   message-box     
	//   creation,   and   customize   it     
	hMsgBoxHook   =   SetWindowsHookEx(     
		WH_CBT,     
		CBTProc,     
		NULL,     
		GetCurrentThreadId()       //   Only   install   for   THIS   thread!!!     
		);     
	//   Display   a   standard   message   box     
	retval   =   MessageBox(hwnd,   szText,   szCaption,   uType);     
	//   remove   the   window   hook     
	UnhookWindowsHookEx(hMsgBoxHook);     
	return   retval;     
}     

void CGameApp::EraseBkgnd()
{
 //  	CMainFrame* fram=(CMainFrame*)AfxGetMainWnd();
//	fram->m_pChatView->OnErase();
}


BOOL CGameApp::IsRejectKanPaiAsk()
{
	return m_GameSet.nRejectKanPaiAsk;
}

BOOL CGameApp::IsRejectPassSound()
{
	return m_GameSet.nRejectPassSound;
}


BOOL	CGameApp::ParseCommond(LPCTSTR str)
{
	//调试用，外部版不做处理
 	return FALSE;


	if (!strcmp(str,"/QuitGame"))
	{
		::PostMessage(AfxGetMainWnd()->m_hWnd,WM_QUIT,0,0);
		return TRUE;
	}
	 
	else if (!strcmp(str,"/chat"))
	{
		CString str;
		str.Format("<c=%d><size=16>===[本局结束]===\r\n",RGB(171,23,23));
		ShowMsgInChatView(str);

		str.Format("\r\n<c=%d><size=16>淘汰赛第%s轮\r\n",RGB_RED,"三");
		ShowMsgInChatView(str);

		str.Format("\r\n<c=%d><size=16><第%d桌,第%d局>\r\n",RGB_BLUE,2,3);
	    ShowMsgInChatView(str);


		str.Format("<c=%d><size=16>===[本局结束]===\r\n",RGB(171,23,23));
		ShowMsgInChatView(str);
		for(int i=0;i<4;i++)
		{

			CString str;
			
			int nScore=10000-rand()%20000;
			if (i==GetChairNO())
			{
				str.Format("<c=%d>[%s]",RGB(171,23,23),GetUserNameByChairNO(i));
				ShowMsgInChatView(str);
				
				for(int s=0;s<=35-str.GetLength();s++)
					ShowMsgInChatView(" ");

				if (nScore>=0)
					str.Format("<c=%d>+%d分\r\n",RGB(171,23,23),nScore);
				else
					str.Format("<c=%d>%d分\r\n",RGB(171,23,23),nScore);

				ShowMsgInChatView(str);
			}
			else
			{
				str.Format("<c=%d>[%s]",RGB(64,64,64),GetUserNameByChairNO(i));
				ShowMsgInChatView(str);
				
				for(int s=0;s<=35-str.GetLength();s++)
					ShowMsgInChatView(" ");

			
				if (nScore>=0)
					str.Format("<c=%d> %d分\r\n",RGB(64,64,64),nScore);
				else
					str.Format("<c=%d>%d分\r\n",RGB(64,64,64),nScore);

				ShowMsgInChatView(str);
			}
		}
		ShowMsgInChatView("\r\n");
		
		CHAT_TO_TABLE ctt;
		memset(&ctt,0,sizeof(CHAT_TO_TABLE));
		int chairno=rand()%3+1;
		ctt.nUserID=GetPlayerIDByChairNO(chairno);

		strcpy(ctt.szChatMsg,"打好牌才是关键，什么都是浮云");

		GetSubSystem()->OnMsg(CreateMsg(0,GAME_MSG_NOTIFY_TABLECHAT,(UINT)&ctt,0));

		return TRUE;
	}
 
	else if (!strcmp(str,"/showgamewin"))
	{
		CMyGame* pGame=(CMyGame*)GetMainGame();
		GAME_WIN_RESULT* pGameWin=((CGameInfoEx*)pGame->GetGameInfo())->GetGameWin();
	 
		pGameWin->gamewin.dwWinFlags=GW_NORMAL;
		pGame->NTF_GameWin();
		return TRUE;
	}
	 
	else if (!strcmp(str,"/DealCard"))
	{
		CMyGame* pGame=(CMyGame*)GetMainGame();
		pGame->OPE_InitData();
		pGame->OPE_DealCard();
		return TRUE;
	}
	else if (!strcmp(str,"/MoveClock"))
	{
		CMyGame* pGame=(CMyGame*)GetMainGame();
		pGame->GUI_MoveClock(rand()%4,30);
		return TRUE;
	}
	else if (!strcmp(str,"/ShowSystem"))
	{CMyGame* pGame=(CMyGame*)GetMainGame();
	pGame->ShowSystemNote(TRUE);
	return TRUE;
	} 
 
	else if (!strcmp(str,"/GameWin"))
	{
        CMyGame* pGame=(CMyGame*)GetMainGame();
		pGame->REQ_SendMsgToServer(SYSMSG_GAME_WIN,0,NULL);
		return TRUE;
	}
	else if (!strcmp(str,"/FreshRankList"))
	{
        CMyGame* pGame=(CMyGame*)GetMainGame();
		FreshRankList();
		return TRUE;
	}


	return FALSE;
}

void CGameApp::CreateArrageTable()//组建新的桌子
{
	CMyGame* pGame=(CMyGame*)GetMainGame();
	pGame->CreateMatchTable();
}

void CGameApp::DisBandTable()
{
	CMyGame* pGame=(CMyGame*)GetMainGame();

}
 
 

int	CGameApp::GetChairCardsCount()
{
	return CHAIR_CARDS;
}

BOOL CGameApp::IsSilence()
{
	return theApp.m_GameSet.nSoundEffect;
}

BOOL    CGameApp::IsAllowCardMaster()
{
	return IS_BIT_SET(GetGameOptions(),GO_USE_CARDMASTER);
//	return TRUE;
}

BOOL CGameApp::IsSaveReplay()
{
//	return TRUE;
	return FALSE;
}

BOOL CGameApp::IsTakeDepositInGame()
{
	if (IsPlayDeposit()
		&& IS_BIT_SET(GetRoomConfigs() ,RC_TAKEDEPOSITINGAME))
		return TRUE;
	else
		return FALSE;
}

void CGameApp::ResetDataByNewPos(USER_POSITION* pUserPos)
{
	InitialGameTableInfo(&((CGameInfoEx*)GetGameInfo())->m_GameTableInfo);
	int nCount = pUserPos->nPlayerCount;
	
	m_GameInit.nChairNO = pUserPos->nChairNO;
	m_GameInit.nTableNO = pUserPos->nTableNO;
	m_GameTable.nTableNO = pUserPos->nTableNO;
	m_GameTable.nPlayerCount = nCount;
	
	memset(&m_GamePlayer,0,sizeof(m_GamePlayer));

	RemoveAllVisitorPlayer();
	
	SOLO_PLAYER * psp=(SOLO_PLAYER*) ((PBYTE)pUserPos +sizeof(USER_POSITION));
	BOOL bSelfSeated = FALSE;
	for(int i=0;i<nCount;i++)
	{
		if (psp)
		{
			if (psp->nStatus != ROOM_PLAYER_STATUS_LOOKON)
			{
				SetGamePlayerBySoloPlayer(&m_GamePlayer[psp->nChairNO],psp);
				if (psp->nUserID == m_GameInit.nUserID)
				bSelfSeated = TRUE;
			}
			else
			{
				GAMEPLAYER* pPlayer = theApp.GetEmptyVisitorPlayer();
				if (NULL != pPlayer)
				{
					theApp.SetVisitorPlayerBySoloPlayer(pPlayer, psp);
				}
			}
		}
		
		psp++;
	}

	//拷贝玩家状态数据
	CMyGame* pGame=(CMyGame*)GetMainGame();
	memcpy(pGame->GetGameInfoEx()->m_GameEnterInfo.ei.dwUserStatus, pUserPos->dwUserStatus, sizeof(DWORD)*MAX_CHAIRS_PER_TABLE);
	if(IS_BIT_SET(pUserPos->dwTableStatus,TS_PLAYING_GAME))
	{
		GameRun();
		pGame->m_dwGameStatus = STATUS_PLAYING_GAME;
		memcpy(&((CGameInfoEx*)GetGameInfo())->m_GameTableInfo,psp,sizeof(GAME_TABLE_INFO));
	}
	else
	{
		GameStop();
		pGame->m_dwGameStatus = STATUS_WAITING_START;
	}

	START_COUNTDOWN  sc;
	memset(&sc,0,sizeof(START_COUNTDOWN));
	sc.nRoomID = pUserPos->nRoomID;
	sc.nTableNO = pUserPos->nTableNO;
	if (pUserPos->nCountdown > 0)
	{
		sc.bStartorStop = TRUE;
		sc.nCountDown = pUserPos->nCountdown;
		pGame->m_dwGameStatus = STATUS_START_COUNTDOWN;
	}
	else
	{
		sc.bStartorStop = FALSE;
	}
	pGame->NTF_StartCountdown(&sc);
}

void  CGameApp::NotifyUserBoutEnd(LPUSER_BOUTEND pUser_BoutEnd)
{
	CGameWinApp::NotifyUserBoutEnd(pUser_BoutEnd);
	CGameBase* pMainGame=GetMainGame(); 
	
	if (pMainGame)
	{
		pMainGame->OnMsg(CreateMsg(0,GAME_MSG_USER_BOUT_END,(UINT)pUser_BoutEnd));
	}
	
}

void CGameApp::NotifyUserDepositEvent(LPUSER_DEPOSITEVENT pUser_Deposit)
{
	if (pUser_Deposit->nEvent != USER_LOOK_SAFE_DEPOSIT)
	{		
		GAMEPLAYER* pPlayer = theApp.GetGamePlayerByUserID(pUser_Deposit->nUserID);
		if (NULL!=pPlayer && FALSE==pPlayer->bLookOn)
		{
			if (IsMatch())
			{
				m_MatchPlayer[pUser_Deposit->nChairNO].useronmatch.nDeposit=pUser_Deposit->nDeposit;
			}
			else
			{
				//更新玩家信息
				m_GamePlayer[pUser_Deposit->nChairNO].nDeposit = pUser_Deposit->nDeposit;
			}
		}
		else
		{
			pPlayer->nDeposit = pUser_Deposit->nDeposit;
		}
		
		UpdataPlayerList();			//刷新玩家列表
		
		CXyGameFrame* pGameFrame=(CXyGameFrame*)GetGameFrame();	
		if (pGameFrame)
		{
			pGameFrame->OPE_UpdataUserDetail();	//刷新展示面板
			pGameFrame->NotifyUserDepositEvent(pUser_Deposit);
		}
	}
}

BOOL CGameApp::CreateGameFrame()
{
	SetRegistryKey(_T(GetGameName()));
	
	int nWindowsWidth=GetSystemMetrics(SM_CXSCREEN);
	int nWindowsHeight=GetSystemMetrics(SM_CYSCREEN);//;
	int nTitleHeight=GetSystemMetrics(SM_CYCAPTION);
	nWindowsHeight-=nTitleHeight;
	
	RECT rc={0,0,nWindowsWidth,nWindowsHeight-nTitleHeight};
	
	m_pGameFrameWnd=(CMainFrame*)(RUNTIME_CLASS(CMainFrame))->CreateObject();
	m_pMainWnd=m_pGameFrameWnd;
 
	m_pGameFrameWnd->CreateWnd(GetGameName(),WS_MINIMIZE|WS_MAXIMIZE|WS_SYSMENU|WS_CLIPCHILDREN,0,rc,NULL);
 
	m_pGameFrameWnd->CreateCore(CreateLogicFrame());
 
	m_pGameFrameWnd->ShowWindow(SW_SHOW);
	m_pGameFrameWnd->SetForegroundWindow();
	m_pGameFrameWnd->UpdateWindow();
	
	////设置窗口透明//////////////////////////////////////////////////
	/*
	SetWindowLong(m_pGameFrameWnd->GetSafeHwnd(),GWL_EXSTYLE, 
	GetWindowLong(m_pGameFrameWnd->GetSafeHwnd(),GWL_EXSTYLE)^0x80000);

	HINSTANCE   hInst   =   LoadLibrary( "User32.DLL");   
	if(hInst)   
	{                         
		typedef   BOOL   (WINAPI *MYFUNC)(HWND,COLORREF,BYTE,DWORD);                     
		MYFUNC   fun   =   NULL; 
		//取得SetLayeredWindowAttributes函数指针           
		fun=(MYFUNC)GetProcAddress(hInst,"SetLayeredWindowAttributes"); 
		if(fun)fun(m_pGameFrameWnd->GetSafeHwnd(),0,0,1);   
        
		FreeLibrary(hInst);   
	}*/
	///////////////////////////////////////////////////////////////////

	/////设置左上角图标///////////////////////////////////////////
	HICON   hicon = LoadIcon(IDR_MAINFRAME);
	AfxGetMainWnd()   -> SendMessage(WM_SETICON,TRUE,(LPARAM)hicon);
	/////////////////////////////////////////////////////////////

	//解决客户端在windows7下无法前置窗口的问题。
	///////////////////////////////////////////////////////////////////////////////
	::SetWindowPos(m_pGameFrameWnd->m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
	::SetWindowPos(m_pGameFrameWnd->m_hWnd,HWND_NOTOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
	///////////////////////////////////////////////////////////////////////////////

 
	NotifyRoomWhenStart(m_pGameFrameWnd->m_hWnd);
 
	return TRUE;
}
