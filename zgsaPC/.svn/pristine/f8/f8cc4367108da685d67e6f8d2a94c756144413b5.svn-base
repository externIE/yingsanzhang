#include "StdAfx.h"
#include "game.h"
#include "MyGame.h"
/*using namespace ShinyNova;*/
CGameTable g_GameTable;

TCHAR* g_szShape[6]={_T("方块"),_T("梅花"),_T("红桃"),_T("黑桃"),_T("小王"),_T("大王")};
TCHAR* g_szValue[14]={_T("2"),_T("3"),_T("4"),_T("5"),_T("6"),_T("7"),_T("8"),_T("9"),_T("10"),_T("J"),_T("Q"),_T("K"),_T("A"),_T("")};

BOOL				m_bThreadStart;
CMyGame::CMyGame()
{
	m_nTotalPlayer = TOTAL_CHAIRS; 
	
	//设置框
	m_GameSetBoard	=	NULL;
				
    m_plGameInfo=NULL;
	
	m_ShowWaitArrage = NULL;
	m_dwGameStatus = STATUS_WAITING_START;
	m_handleNewRound=0;
	m_nGameZeroCount=0;
	m_nKeepGiveUpTimes = 0;
	
    m_bitCountDown=NULL;
	m_hEventTimer=0;
    m_nZeroCountDown=0;
	m_bitVS = NULL;
	m_bitLightning = NULL;
	m_bLightningLeft = FALSE;
	m_nBetCount = 0;
	m_bCardDealing = FALSE;
	m_bAutoStart = TRUE;
	m_bAutoFollow = FALSE;
	m_bOpeDuringCompareCard = FALSE;
	m_nComNextPlayer = -1;
	m_nLeftCompareNum = -1;
	m_bGameWin = FALSE;
	m_pAllInResponse = NULL;
	m_pCompareCardResponse = NULL;
	m_bSitOrStand = TRUE;

	m_dlgAutoSetDeposit = NULL;
	m_pMyKeyResult		= NULL;

	m_betInfoBoard			= NULL;
	m_digitPotsNum			= NULL;
	m_chatSingleBet			= NULL;
	m_chatBetMaxDepositNum  = NULL;
	m_bOpenBetFull			= FALSE;
	m_nBetFullDeposit		= 0;

	m_bShowGiveUpBtn		= TRUE;
}

CMyGame::~CMyGame()
{
	SAFE_DELETE(m_pAllInResponse);
	SAFE_DELETE(m_pCompareCardResponse);
}

void CMyGame::OnGameSize()
{
    RECT rect=GetGameRect();
	int x=rect.left;
	int y=rect.top;
	int w=rect.right-rect.left;
	int h=rect.bottom-rect.top;
	
   	RUL_CalcGameStatus(w,h);
	
	//调整筹码
	for(int i=0; i<m_nBetCount; i++)
	{
		if (NULL != m_bitThrownBet[i])
		{
			StopObject(m_bitThrownBet[i]->GetHandle());
			POINT pt=GetWorldPoint(m_TrownPoint[i]);
			m_bitThrownBet[i]->MoveTo(pt.x,pt.y);
			if (m_dwGameStatus != STATUS_PLAYING_GAME)
				m_bitThrownBet[i]->Hide();
			m_bitThrownBet[i]->Updata();
		}
	}
	
	if (m_GameSetBoard)
	{
		m_GameSetBoard->MoveTo(x+w/2,y+h/2);
		m_GameSetBoard->Updata();
	}
	
	{
		for(int i=0;i<TOTAL_CHAIRS*CHAIR_CARDS;i++)
		{
			if (FALSE == m_bCardDealing)
				StopObject(m_Card[i]->GetHandle());
			m_Card[i]->FitScreen();
		}

		if (NULL != m_bitVS)
		{
			POINT pt = GetVSPosition();
			m_bitVS->MoveTo(pt.x, pt.y);
		}

		if (NULL != m_bitLightning)
		{
			POINT pt = GetLightningPosition(m_bLightningLeft);
			m_bitLightning->MoveTo(pt.x, pt.y);
		}
	}
	
	{
		for(int i=0;i<m_nTotalPlayer;i++)
			m_ptrPlayer[RUL_GetDrawIndexByChairNO(i)]->FitScrren(); 
	}
	
	
	if (m_btnStart)
	{
		int nSize=Sys_GetIntStatus(GAME_STATUS_TABLE_SIZE);

		if (0 == nSize)
			m_btnStart->MoveTo(w/2,h/2+67);
		else
			m_btnStart->MoveTo(w/2,h/2+90);

		m_btnStart->Updata();
	}
	
	{
		if (m_betInfoBoard)
		{
			POINT pt=GetPotsNumPosition();
			m_betInfoBoard->MoveTo(pt.x,pt.y + 10);
			m_betInfoBoard->Updata();
		}
	}
	
	{
		if (m_dwGameStatus==STATUS_PLAYING_GAME && m_Clock && m_Clock->IsShow())
		{
			int nDrawIndex=RUL_GetDrawIndexByChairNO(GetGameInfo()->GetCurrentChairNO());
			POINT pt=m_ptrPlayer[nDrawIndex]->GetClockPosition();
			m_Clock->MoveTo(pt.x,pt.y);  
			m_Clock->Updata();
		}
		
		if (m_dwGameStatus==STATUS_START_COUNTDOWN && m_ClockCountDown)
		{ 	
			POINT pt=GetWaitCountDownDigitPostion();
			
			m_ClockCountDown->MoveTo(pt.x,pt.y);
			m_ClockCountDown->Updata(); 
		}
	}
	
	if (m_ShowWaitArrage)
	{
		BITObject* pbo = (BITObject*)m_ShowWaitArrage;
		pbo->MoveTo(w/2,h/2);
		pbo->Updata();
	}
	
	if (m_bitCountDown)
	{ 
		POINT pt=GetWaitCountDownPostion();
		m_bitCountDown->MoveTo(pt.x,pt.y);
		m_bitCountDown->Updata();
	}
		
	if (NULL != m_dlgAutoSetDeposit)
	{
		m_dlgAutoSetDeposit->MoveTo(GetScreenWidth()/2,GetScreenHeight()/2);
		PGL_HIDE_OBJECT(m_dlgAutoSetDeposit);
	}

	if (m_dlgOperation)
	{
		m_dlgOperation->FitScreen();
	}
	SCR_InvalidateScreen();
}


void CMyGame::OnIniData()
{
	CreateClock();
	CreateRetInfo();
	CreatePlayers(TOTAL_CHAIRS);
	CreateCard();
	CreateBtn(); 
	CreateReplay();
	createAutoSetDepositDlg();
	
	if (!GetGameInfo()->IsGamePlaying())
	{
		OPE_InitData();
	}
	
	m_hCursorHand=LoadCursor(NULL,IDC_HAND);
	m_hCursorArrow=LoadCursor(NULL,IDC_ARROW);
	
	OnFreshGameSet();

	m_bThreadStart = TRUE;
	AfxBeginThread(ThreadShowWinResult,(LPVOID)this,THREAD_PRIORITY_NORMAL,0,0,0);
}


void  CMyGame::GameInstance()
{
	DWORD dwRoomOptions=theApp.GetRoomOptions(); 
	m_ptrPlayer[MYDRAWINDEX]->LoadPlayer(GetGameWinApp()->GetChairNO());
#ifdef  _OFFLINE_
	PGL_SHOW_OBJECT(m_btnStart); 
	for(int i=0;i<TOTAL_CHAIRS;i++)
	{ 
		m_ptrPlayer[RUL_GetDrawIndexByChairNO(i)]->LoadPlayer(i);
	}
#endif
}

void CMyGame::OnEnterMatchOK()
{
	m_ptrPlayer[MYDRAWINDEX]->LoadPlayer(GetGameWinApp()->GetChairNO());
}

void  CMyGame::OnEnterGameOK()
{
	//加载当前已有的玩家
	for(int i=0;i<GetGameWinApp()->GetTableChairCount();i++)
	{
		int nDrawIndex=RUL_GetDrawIndexByChairNO(i);
		//m_ptrPlayer[nDrawIndex]->DrawEmptySeat();//空位

		if (GetGameWinApp()->IsPlayerEnter(i)) 
		{
			m_ptrPlayer[nDrawIndex]->LoadPlayer(i);
			
			if (GetGameInfo()->IsChairReady(i))
			{
				m_ptrPlayer[nDrawIndex]->ShowReady();
			}
		}
	}

	if(GetGameWinApp()->IsRandomRoom()
		&&!GetGameWinApp()->IsReEnter())
	{
		//显示提示:等待分桌
		ShowWaitArrageTable();
	}
	else if (!GetGameWinApp()->IsLookOn())
	{   
		PGL_SHOW_OBJECT(m_btnStart);
	}

	ShowLookOn_Seat_ChangeTableBtn(TRUE, FALSE, TRUE);

	if (FALSE==GetGameInfoEx()->IsGamePlaying() && IsAutoStart())
	{//进入游戏默认自动开始
		OnStart();
	}
}

void  CMyGame::CreatePlayers(int nTotalChairs)
{
	m_nTotalPlayer=nTotalChairs;
	int nDrawIndex = 0;
	for(int i=0;i<nTotalChairs;i++)
	{
		nDrawIndex = RUL_GetDrawIndexByChairNO(i);
		PGL_MSG_CLASS(m_ptrPlayer[nDrawIndex],CMyPlayer);
		m_ptrPlayer[nDrawIndex]->CreateGUI(nDrawIndex);
	}
}


void    CMyGame::OnEnterDXXW()
{
	for(int i=0;i<GetGameWinApp()->GetTableChairCount();i++)
	{
		if (GetGameWinApp()->IsPlayerEnter(i))
		{
			int nDrawIndex=RUL_GetDrawIndexByChairNO(i);
			m_ptrPlayer[nDrawIndex]->LoadPlayer(i);

			if (GetPlayerInfoEx(i)->bInGame && IsPlayerGiveUp(i)
				&& i!=theApp.GetChairNO())
			{
				theApp.m_GamePlayer[i].nStatus = ROOM_PLAYER_STATUS_PLAYING;
			}
		}
	}
	
	OPE_DXXW();
}

void    CMyGame::OnEnterIdle()
{
	for(int i=0;i<GetGameWinApp()->GetTableChairCount();i++)
	{
		if (GetGameWinApp()->IsPlayerEnter(i))
		{
			int nDrawIndex=RUL_GetDrawIndexByChairNO(i);
			m_ptrPlayer[nDrawIndex]->LoadPlayer(i);

			if (GetPlayerInfoEx(i)->bInGame && IsPlayerGiveUp(i)
				&& i!=theApp.GetChairNO())
			{
				theApp.m_GamePlayer[i].nStatus = ROOM_PLAYER_STATUS_PLAYING;
			}
		}
	}
	
	OPE_DXXW();
}

void   CMyGame::OnPlayerEnter(GAMEPLAYER* pGamePlayer)
{
	CGameWinApp* pGameApp=(CGameWinApp*)AfxGetApp();
	int chairno=pGamePlayer->nChairNO;
	
	// 	CString sMsg;
	// 	sMsg.Format(_T(" <c=%d>%s进入游戏。\n"),	RGB(88,88,44), pGameApp->GetPlayerNameByChairNO(chairno));
	// 	pGameApp->ShowMsgInChatView(sMsg);
	
	int nIndex=RUL_GetDrawIndexByChairNO(chairno);
	m_ptrPlayer[nIndex]->LoadPlayer(chairno);
    if (m_dwGameStatus==STATUS_PLAYING_GAME)
    {
		m_ptrPlayer[nIndex]->SetOpeStatus(PLAYER_STATUS_WAIT_START);
    }

	theApp.RemoveVisitorPlayerByUserID(pGamePlayer->nUserID);
}

void   CMyGame::OnPlayerAbort(GAMEPLAYER* pGamePlayer)
{
	CGameWinApp* pGameApp=(CGameWinApp*)AfxGetApp();
	int chairno=pGamePlayer->nChairNO;
	
	// 	CString sMsg;
	// 	sMsg.Format(_T(" <c=%d>%s离开游戏。\n"),	RGB(88,88,44), pGameApp->GetPlayerNameByChairNO(chairno));
	// 	pGameApp->ShowMsgInChatView(sMsg);
	
	if (FALSE == pGamePlayer->bLookOn)
	{
		int nIndex=RUL_GetDrawIndexByChairNO(chairno);
		m_ptrPlayer[nIndex]->UnLoadPlayer();
		
		theApp.m_GamePlayer[chairno].nStatus = ROOM_PLAYER_STATUS_WAITING;
		GetGameInfoEx()->NTF_PlayerAbort(chairno);
		OPE_HideChairCard(pGamePlayer->nChairNO);
	}
}

void   CMyGame::OnVisitorEnter(GAMEPLAYER* pGamePlayer)
{
	
}

void   CMyGame::OnVisitorAbort(GAMEPLAYER* pGamePlayer)
{
	
}

void  CMyGame::OnStartSoloTable()
{
	CGameWinApp* pApp=(CGameWinApp*)AfxGetApp();
	
	for(int i=0;i<pApp->m_GameTable.nPlayerCount;i++)
	{
		m_ptrPlayer[RUL_GetDrawIndexByChairNO(i)]->UnLoadPlayer();
		m_ptrPlayer[RUL_GetDrawIndexByChairNO(i)]->LoadPlayer(i);
		
	}
	
	//reserved 先放这snd_个音乐
	PlaySound("snd_ArrageTable");
}
CXyStandButton*  CMyGame::FastCreateStandButton(LPCTSTR picName,SNG_MSG& msg,LPCTSTR szNote)
{
	CXyStandButton* pgl=NULL;
	PGL_MSG_CLASS(pgl,CXyStandButton);
	pgl->CreateGUI(Res_GetPICStruct(picName),&msg);
	pgl->SetButtonSound(BUTTON_STATUS_TOUCH,"SndTouchButton");
	pgl->SetButtonSound(BUTTON_STATUS_LEFT_DOWN,"SndHitButton");
	pgl->SetFloor(FLOOR_BTN);
	
	PGL_SET_OBJECT_NOTE(pgl,szNote);
	return pgl;
}

void   CMyGame::SetObjectStayNote(PObject* ob,LPCTSTR note)
{
	CXyToolTip* tool;
	PGL_MSG_CLASS(tool,CXyToolTip);
	tool->BindGUI(ob,note);
}

void  CMyGame::CreateRetInfo()
{
	//设置框
	{
		PGL_MSG_CLASS(m_GameSetBoard,CGameSetBoard);
	}

	//总注
    m_betInfoBoard=CreateBitMap(Res_GetPICStruct("bk_bet_info"));
	m_betInfoBoard->SetFloor(FLOOR_BTN+1); 
	PGL_MSG_CLASS(m_digitPotsNum,CMyDigit); 
	m_digitPotsNum->CreateDigit("总注num",DIGIT_FLAGS_LEFT_JUSITIFY);  
	m_digitPotsNum->SetWordSpace(-2);
	m_digitPotsNum->SetFloor(m_betInfoBoard->GetZ()); 
    m_betInfoBoard->AddChild(m_digitPotsNum,OBJECT_LOCK_POSITION|OBJECT_LOCK_SHOW|OBJECT_LOCK_UPDATA);	

	//单注
	PGL_MSG_CLASS(m_chatSingleBet, CXyChatBox);
	m_chatSingleBet->CreateGUI(72, 20, 0);
	m_chatSingleBet->SetBkgndColor(COLOR_TRANSPARENT);
	m_chatSingleBet->SetMaxFontSize(14);
	m_chatSingleBet->SetTextSize(14);
	m_chatSingleBet->SetTextColor(RGB(120, 174, 87));
	m_chatSingleBet->SetRowSpacePixel(5);
	m_chatSingleBet->SetFloor(m_betInfoBoard->GetZ());
	m_betInfoBoard->AddChild(m_chatSingleBet, OBJECT_LOCK_ALL);

	//本局可压
	PGL_MSG_CLASS(m_chatBetMaxDepositNum, CXyChatBox);
	m_chatBetMaxDepositNum->CreateGUI(72, 20, 0);
	m_chatBetMaxDepositNum->SetBkgndColor(COLOR_TRANSPARENT);
	m_chatBetMaxDepositNum->SetMaxFontSize(14);
	m_chatBetMaxDepositNum->SetTextSize(14);
	m_chatBetMaxDepositNum->SetTextColor(RGB(120, 174, 87));
	m_chatBetMaxDepositNum->SetRowSpacePixel(5);
	m_chatBetMaxDepositNum->SetFloor(m_betInfoBoard->GetZ());
	m_betInfoBoard->AddChild(m_chatBetMaxDepositNum, OBJECT_LOCK_ALL);

	//本局可压图标
	m_bitIconMaxDeposit = CreateBitMap(Res_GetPICStruct("icon_maxdeposit"));
	m_bitIconMaxDeposit->SetFloor(m_betInfoBoard->GetZ());
	m_bitIconMaxDeposit->MoveTo(m_betInfoBoard->GetLeft()+82+(295-m_bitIconMaxDeposit->GetWidth())/2,
		m_betInfoBoard->GetY() - 20);
	m_betInfoBoard->AddChild(m_bitIconMaxDeposit,OBJECT_LOCK_POSITION|OBJECT_LOCK_UPDATA);

	//"VS"和闪电资源
	{
		m_bitVS = CreateBitMap(Res_GetPICStruct("VS"));
		m_bitVS->TransGUI();
		m_bitVS->SetFloor(FLOOR_ANIMATION);

		m_bitLightning = CreateBitMap(Res_GetPICStruct("闪电"));
		m_bitLightning->TransGUI();
		m_bitLightning->SetFloor(FLOOR_ANIMATION);
	}
}

void  CMyGame::CreateCard()
{
	for(int i=0;i<TOTAL_CHAIRS*CHAIR_CARDS;i++)
	{
		PGL_MSG_CLASS(m_Card[i],CMyCard);
		m_Card[i]->Hide();
		m_Card[i]->Updata();
	}
}

void  CMyGame::CreateBtn()
{
	SNG_MSG msg = CreateMsg(GetHandle(),GAME_MSG_START_GAME);
	PGL_MSG_CLASS(m_btnStart,CXyDynamicButton);
	m_btnStart->CreateGUI(Res_GetPICStruct("开始1"), &msg);
	m_btnStart->SetButtonSound(BUTTON_STATUS_TOUCH,"SndTouchButton");
	m_btnStart->SetButtonSound(BUTTON_STATUS_LEFT_DOWN,"SndHitButton");
	m_btnStart->MoveTo(0,0);
	m_btnStart->LoadButtonPic(BUTTON_STATUS_UNTOUCH,Res_GetPICStruct("开始1"));
	m_btnStart->LoadButtonPic(BUTTON_STATUS_TOUCH,Res_GetPICStruct("开始2"));
	m_btnStart->LoadButtonPic(BUTTON_STATUS_LEFT_DOWN,Res_GetPICStruct("开始3"));
	m_btnStart->LoadButtonPic(BUTTON_STATUS_DISABLE,Res_GetPICStruct("开始4"));
	
	PGL_MSG_CLASS(m_dlgOperation,COperationDlg);
}

void CMyGame::CreateClock()
{
	PGL_MSG_CLASS(m_Clock,CClockDigit);
	m_Clock->CreateDigit("icon_time_numb",DIGIT_FLAGS_MID_JUSITIFY);
	m_Clock->SetFloor(FLOOR_CLOCK);
	m_Clock->TransGUI();
	m_Clock->Updata();
	
	PGL_MSG_CLASS(m_ClockCountDown,CXyDigit);
	m_ClockCountDown->CreateDigit("text_daojishinum",DIGIT_FLAGS_MID_JUSITIFY);
	m_ClockCountDown->SetFloor(FLOOR_CLOCK+1);
	m_ClockCountDown->TransGUI();
	m_ClockCountDown->Updata();
}

BOOL CMyGame::OnMsg(SNG_MSG& msg)
{
	if (msg.msgID > MSG_BUTTON_BEGIN
		&&msg.msgID < MSG_BUTTON_END)
		return OM_DealBTN(msg);
	
	if (msg.msgID>OPE_CALLBACKMSG_BEGIN
		&&msg.msgID<OPE_CALLBACKMSG_END)
		return OM_DealOPE(msg);
	
	switch(msg.msgID)
	{
	case GAME_MSG_ARRANGE_TABLE:
		CreateMatchTable();
		break;
	case GAME_MSG_REPLAY_TABLE:
		CreateReplayTable();
		break;
	case GAME_MSG_START_GAME:
		OnStart();
		break;
	case GAME_MSG_ENTER_MATCH_OK:
		OnEnterMatchOK();
		break;
	case GAME_MSG_ENTER_GAME_OK:
		OnEnterGameOK();
		break;
	case GAME_MSG_ENTER_GAME_DXXW:
		OnEnterDXXW();
		break;
	case GAME_MSG_ENTER_GAME_IDLE: 
		OnEnterIdle();
		break;
	case GAME_MSG_PLAYER_ENTER:
		OnPlayerEnter((GAMEPLAYER*)msg.param1);
		break;
	case GAME_MSG_PLAYER_ABORT:
		OnPlayerAbort((GAMEPLAYER*)msg.param1);
		break;
	case GAME_MSG_VISITOR_ENTER:
		OnVisitorEnter((GAMEPLAYER*)msg.param1);
		break;
	case GAME_MSG_VISITOR_ABORT:
		OnVisitorAbort((GAMEPLAYER*)msg.param1);
		break;
	case GAME_MSG_START_SOLOTABLE:
		OnStartSoloTable();
		break;
	case GAME_MSG_CLOSETIMER:
		OPE_EndTimer();
		break; 
	case GAME_CALLBACKMSG_DEALCARD:
		MOVIE_DealCard();
		break;
	case GAME_CALLBACKMSG_DEALCARD_END:
		MOVIE_DealCardEnd(msg);
		break;
	case GAME_CALLBACKMSG_OPE_START:
		OPE_GameStart();
		break;
	case GAME_CALLBACKMSG_ONTIMER:
		OnTimer();
		break;
	case GAME_CALLBACKMSG_HITSCREEN:
		break;
	case GAME_CALLBACKMSG_COUNTDOWNZERO:
		{
			if (STATUS_START_COUNTDOWN == m_dwGameStatus)
			{
				CGameRequest* pGameConnect=(CGameRequest* )GetGameWinApp()->GetGameConnect();
				pGameConnect->GC_SyncStartCountdown(m_ClockCountDown->GetDigit());
			}
		}
		break;
	case GAME_MSG_SHOW_GAMESET:
		{
			if (m_GameSetBoard)
			{
				if (!m_GameSetBoard->IsShow())
				{
					m_GameSetBoard->SetFloor(FLOOR_GAMSET);
					m_GameSetBoard->Show();
					m_GameSetBoard->Updata();
				}
				else
				{
					m_GameSetBoard->Hide();
					m_GameSetBoard->Updata();
				}
			}
		}
		break;
	case GAME_MSG_FRESH_GAMESET:
		{
			OnFreshGameSet();
		}
		break;  
	case GAME_MSG_USER_DEPOSIT_EVENT:
		{
			NTF_UserDepositEvent((LPUSER_DEPOSITEVENT)msg.param1);
		}
		break;
	case GAME_MSG_USER_BOUT_END:
		{ 
			NTF_UserBoutEnd((LPUSER_BOUTEND)msg.param1);
		}
		break;
	case GAME_MSG_GET_NEW_POSITION:
		{
			OnGetNewPosition();
		}	
		break;
	case ZGSA_SAFEBOX_SETDEPOSIT_CHECK_SET:
		{
			int nKeyResult = (int)msg.param1;
			int nAutoFixDeposit = GetAutoSetDeposit();
			TakeDepositToGame(nAutoFixDeposit,nKeyResult);
		}
		break;
	default:
		return CMainGame::OnMsg(msg);
		break;
	}
	return TRUE;
}

BOOL   CMyGame::OM_DealBTN(SNG_MSG& msg)
{
	BOOL bResult = TRUE;

	switch(msg.msgID)
	{
	case MSG_BUTTON_BET:
		{//下注 
			HidePlayersCompareBtn();
			if (m_dlgOperation->m_btnBet->IsShow() && m_dlgOperation->m_btnBet->IsEnable())
			{
				int nAddBet = -1;
				bResult = OPE_Bet(nAddBet); 
			}
			m_nKeepGiveUpTimes = 0;
		}
		break;
	case MSG_BUTTON_BET_TWO:
		{//2倍注 
			HidePlayersCompareBtn();
			if (m_dlgOperation->m_btnBetTwo->IsShow() && m_dlgOperation->m_btnBetTwo->IsEnable())
			{
				int nAddBet = 2;
				bResult = OPE_Rise(nAddBet);
			}
			m_nKeepGiveUpTimes =0;
		}
		break;
	case MSG_BUTTON_BET_FOUR:
		{//4倍注
			HidePlayersCompareBtn();
			if (m_dlgOperation->m_btnBetFour->IsShow() && m_dlgOperation->m_btnBetFour->IsEnable())
			{
				int nAddBet = 4;
				bResult = OPE_Rise(nAddBet); 				
			}
			m_nKeepGiveUpTimes = 0;
		}
		break;
	case MSG_BUTTON_BET_EIGHT:
		{//8倍注 	
			HidePlayersCompareBtn();
			if (m_dlgOperation->m_btnBetEight->IsShow() && m_dlgOperation->m_btnBetEight->IsEnable())
			{
				int nAddBet = 8;
				bResult = OPE_Rise(nAddBet); 			
			}
			m_nKeepGiveUpTimes = 0;
		}
		break;
   	case MSG_BUTTON_FOLLOW:
		{//跟注
			HidePlayersCompareBtn();
			if (m_dlgOperation->m_btnFollow->IsShow() && m_dlgOperation->m_btnFollow->IsEnable())
				bResult = OPE_Follow();
			
			m_nKeepGiveUpTimes = 0;
		}
		break;
	case MSG_BUTTON_LOOK_CARD:
		{//看牌
			if (m_dlgOperation->m_btnLookCard->IsShow() && m_dlgOperation->m_btnLookCard->IsEnable())
				bResult = OPE_LookCard(); 
		}
		break;
	case MSG_BUTTON_COMPARE_INDEX1:
	case MSG_BUTTON_COMPARE_INDEX2:
	case MSG_BUTTON_COMPARE_INDEX3:
	case MSG_BUTTON_COMPARE_INDEX4:
	case MSG_BUTTON_COMPARE_INDEX5:
		{//选择比牌玩家
			int nDrawIndex = msg.msgID-MSG_BUTTON_COMPARE_INDEX1+1;
			if (NULL != m_ptrPlayer[nDrawIndex])
			{
				if (m_ptrPlayer[nDrawIndex]->IsShowCompareBtn())
				{
					HidePlayersCompareBtn();
					bResult = OPE_CompareCardByIndex(nDrawIndex);
				}
			}
			m_nKeepGiveUpTimes = 0;
		}
		break;
	case MSG_BUTTON_ALLIN:
		{//全下
			HidePlayersCompareBtn();
			if (m_dlgOperation->m_btnAllin->IsShow() && m_dlgOperation->m_btnAllin->IsEnable())
				bResult = OPE_Allin();

			m_nKeepGiveUpTimes = 0;
		}
		break;
	case MSG_BUTTON_BET_FULL:
		{//压满
			HidePlayersCompareBtn();
			if (m_dlgOperation->m_btnBetFull->IsShow() && m_dlgOperation->m_btnBetFull->IsEnable())
			{
				bResult = OPE_BetFull();	
			}
			m_nKeepGiveUpTimes = 0;
		}
		break;
	case MSG_BUTTON_GIVE_UP:
		{
			HidePlayersCompareBtn();
			if (m_dlgOperation->m_btnGiveUp->IsShow() && m_dlgOperation->m_btnGiveUp->IsEnable())
			{
				bResult = OPE_GiveUp(); 
			}
			m_nKeepGiveUpTimes = 0;
		}
		break;
	case MSG_BUTTON_AUTO_START:
	case MSG_BUTTON_AUTO_START_FOCUS:
		{
			m_bAutoStart = !m_bAutoStart;
			m_dlgOperation->ShowNextAutoRestAndFollowBtn();
		}
		break;
	case MSG_BUTTON_AUTO_FOLLOW:
	case MSG_BUTTON_AUTO_FOLLOW_FOCUS:
		{
			m_nAutoFollowTime = AUTO_FOLLOW_TIME;
			m_bAutoFollow = !m_bAutoFollow;
			m_dlgOperation->ShowNextAutoRestAndFollowBtn();
		}
		break;
	case MSG_BUTTON_WIN_SHOW:  
		{  
			HidePlayersCompareBtn();
			if (m_dlgOperation->m_btnWinShow->IsShow() && m_dlgOperation->m_btnWinShow->IsEnable())
			{
				m_dlgOperation->HideShowCardBtn(); 
				OnShowCard(); 
			}
		}
		break;
	case MSG_BUTTON_SIT_ON:
		{
			if (m_dlgOperation->m_btnSitDown->IsShow() && m_dlgOperation->m_btnSitDown->IsEnable())
			{
				OPE_ChangeLookOnToSeated();
			}
		}
		break;
	case MSG_BUTTON_STAND_BY:
		{
			if (m_dlgOperation->m_btnStandBy->IsShow() && m_dlgOperation->m_btnStandBy->IsEnable())
			{
				OPE_ChangeSeatedToLookOn();
			}
		}
		break;
	case MSG_BUTTON_CHANGE_TABLE:
		if(m_dlgOperation->m_btnChangeTable->IsShow() && m_dlgOperation->m_btnChangeTable->IsEnable())
		{
			OPE_ChangeTable();

			//假如有数字动画残留，则去隐藏
			for (int i = 0; i < m_nTotalPlayer; i++)
				m_ptrPlayer[i]->HideNewDigitAni();
		}
		break;
	case MSG_BUTTON_AUTO_SET_DEPOSIT:
		{
			if (m_dlgAutoSetDeposit->IsShow())
			{
				PGL_HIDE_OBJECT(m_dlgAutoSetDeposit);
			}
			else
			{
				PGL_SHOW_OBJECT(m_dlgAutoSetDeposit);
			}
		}
		break;
	}
	return bResult;
}

BOOL   CMyGame::OM_DealOPE(SNG_MSG& msg)
{
	switch(msg.msgID)
	{	
	case	MSG_SHOW_RESULT_END:
		{
			OPE_AfterShowResultAni();
			//在此根据自动开始标志设置，重新开始
			SetAutoFollow(FALSE);

			if(IsAutoStart())
			{
				OnStart();
			}
			else if(!IsAutoStart() && !theApp.IsLookOn() )
			{//玩家下一局休息,站立旁观
				OPE_ClearGame();
				for(int i=0;i<GetGameWinApp()->GetTableChairCount();i++)
				{
					if (GetGameInfo()->IsChairReady(i))
						m_ptrPlayer[RUL_GetDrawIndexByChairNO(i)]->ShowReady();
				}
				OPE_ChangeSeatedToLookOn();
 			}
			
		}
		break;
	case MSG_COMPARE_RESULT:
		{
			if (m_pCompareCardResponse)
			{
				int nMyChairNO = theApp.GetChairNO();
				if (theApp.IsLookOn())
				{
					ShowLookOn_Seat_ChangeTableBtn(FALSE, TRUE, TRUE);
				}
				else if (theApp.GetChairNO()==m_pCompareCardResponse->nLostChairNo)
				{
					ShowLookOn_Seat_ChangeTableBtn(TRUE, FALSE, TRUE);
					FreshTotalDepositInfo();
				}
				else if (FALSE==IsPlayerGiveUp(nMyChairNO) && m_pCompareCardResponse->bNextChair && nMyChairNO != m_pCompareCardResponse->nNextChair)
				{
					m_dlgOperation->ShowPlayerCouldOpeNotCurrentChair(GetGameInfoEx()->GetCouldOpe());
				}

				if (m_pCompareCardResponse->bNextChair)
					EnableNextChairOpe(m_pCompareCardResponse->nChairNo, m_pCompareCardResponse->nNextChair, m_pCompareCardResponse->dwNextCouldOpe);
				
				SAFE_DELETE(m_pCompareCardResponse);
			}

			if (m_pAllInResponse)
			{				
				m_nComNextPlayer = RUL_GetNextChairNO(m_nComNextPlayer);
				while (1)//find the next comparer
				{
					if(m_pAllInResponse->nChairNo == m_nComNextPlayer)
					{//全下比牌结束，同时对局结束（主动比牌玩家赢了所有玩家或者被其它玩家赢了）
						if(!m_pAllInResponse->bNextChair)
						{
							m_nLeftCompareNum = 0;
						}
						else
						{
							EnableNextChairOpe(m_pAllInResponse->nChairNo, m_pAllInResponse->nNextChair, m_pAllInResponse->dwNextCouldOpe);
							if (FALSE == IsPlayerGiveUp(theApp.GetChairNO()))
								m_dlgOperation->ShowPlayerCouldOpeNotCurrentChair(GetGameInfoEx()->GetCouldOpe());
						}
						
						SAFE_DELETE(m_pAllInResponse);
						m_vecCompareAni.clear();
						return TRUE;
					}

					if (!GetPlayerInfoEx(m_nComNextPlayer)->bInGame
						|| IsPlayerGiveUp(m_nComNextPlayer) || !m_pAllInResponse->bNeedCompare[m_nComNextPlayer])
					{
						m_nComNextPlayer = RUL_GetNextChairNO(m_nComNextPlayer);
					}
					else
					{
						break;
					}
				}

				COMPARE_CARD_RESPONSE mComp;
				mComp.nBet = 0;
				mComp.nChairNo = m_pAllInResponse->nChairNo;
				mComp.nGoalChairNo = m_nComNextPlayer;
				
				if(m_pAllInResponse->bWinOrLose[m_nComNextPlayer])
					mComp.nLostChairNo = m_nComNextPlayer;
				else
					mComp.nLostChairNo = m_pAllInResponse->nChairNo;
				
				if (m_pAllInResponse->bNextChair && m_pAllInResponse->nNextChair == m_nComNextPlayer)
				{//最后一次比牌，主动比牌玩家输了
					mComp.bNextChair = TRUE;
					mComp.dwNextCouldOpe = m_pAllInResponse->dwNextCouldOpe;
					mComp.nNextChair = m_nComNextPlayer;

					m_vecCompareAni.clear();
				} 
				else
				{//继续播放下一玩家的比牌动画
					mComp.bNextChair = FALSE;
					mComp.dwNextCouldOpe = 0;
					mComp.nNextChair = -1;

					m_vecCompareAni.clear();
				}
				NTF_LocalMsg(GAME_MSG_COMPARE, (void*)&mComp, sizeof(COMPARE_CARD_RESPONSE));
			}
		}
		break;
	}
	
	return TRUE;
}

void    CMyGame::OPE_CloseResult()
{
	m_dwGameStatus=STATUS_WAITING_START;
	if (GetGameWinApp()->IsGameRunning())
		return;
	
	
	if (GetGameWinApp()->IsMatch())
	{
		
	}
	else
	{
		//是否允许在关闭结果框继续查看
		OPE_ClearGame();
		if (!GetGameWinApp()->IsLookOn())
			PGL_SHOW_OBJECT(m_btnStart);
	}
}

void    CMyGame::OnStart()
{
#ifdef  _OFFLINE_
	{
		//显示倒计时
		if (!m_bitCountDown)
		{
			m_bitCountDown = CreateBitMap(Res_GetPICStruct("text_daojishi"));
			m_bitCountDown->SetFloor(FLOOR_ANIMATION);
			POINT pt=GetWaitCountDownPostion();
			m_bitCountDown->MoveTo(pt.x,pt.y);
		} 
		//m_bitCountDown->Show();
		m_bitCountDown->Updata();

		//显示明、暗牌
		{
			for (int i = 0; i < m_nTotalPlayer; i++)
			{
				if(GetPlayerInfoEx(i)->bInGame)
					continue;

				m_ptrPlayer[i]->ShowBrightOrDarkBoard(GetGameInfoEx()->GetGameTableInfo()->m_Public.bnLookCard[i]);
				GetGameInfoEx()->GetGameTableInfo()->m_Public.bnLookCard[i] = !GetGameInfoEx()->GetGameTableInfo()->m_Public.bnLookCard[i];
				m_ptrPlayer[i]->HideLostLightningMask();
			}
		}

		{
			for(int i=0;i<TOTAL_CHAIRS;i++)
			{
				OPE_ThrowChips(i,i+1);
				m_ptrPlayer[RUL_GetDrawIndexByChairNO(i)]->HideWinAni();
				m_ptrPlayer[RUL_GetDrawIndexByChairNO(i)]->MOVIE_FreshRestBet(i%2?1235234:-1232532, i, i%2?HANDCARD_TYPE_BAOZI:HANDCARD_TYPE_SHUNJIN);
				m_ptrPlayer[RUL_GetDrawIndexByChairNO(i)]->ShowWinAni(i%2?HANDCARD_TYPE_BAOZI:HANDCARD_TYPE_SHUNJIN);
				m_ptrPlayer[RUL_GetDrawIndexByChairNO(i)]->ShowPlayerFire();
			}
		}

		FreshBetInfo(theApp.GetChairNO());
		FreshTotalDepositInfo();
		static  ss=0;
		ss++;
		m_dlgOperation->ShowPlayerCouldOpe(0);
		
		//m_ptrPlayer[MYDRAWINDEX]->ShowMask(ss%2);

		//填充手牌 
		int cardcount=0;
		for (int chairno=0;chairno<TOTAL_CHAIRS;chairno++)
		{
			for (int j=0;j<CHAIR_CARDS;j++)
			{ 
				
				m_Card[cardcount]->m_bnFace=FALSE;
				if (ss%2)
				{
					m_Card[cardcount]->m_nCardID=GetPlayerInfoEx(chairno)->nChairCards[j];
					m_Card[cardcount]->m_bnFace=TRUE;
				}
				m_Card[cardcount]->m_nChairNO=chairno;
				m_Card[cardcount]->m_nUnitIndex=j;
				m_Card[cardcount]->m_nDrawIndex=RUL_GetDrawIndexByChairNO(chairno); 
				m_Card[cardcount]->FitScreen(); 
				m_Card[cardcount]->Fresh();
				cardcount++;
			}
			int nDrawIndex=RUL_GetDrawIndexByChairNO(chairno); 
			
			m_ptrPlayer[nDrawIndex]->SetOpeStatus(GetPlayerInfoEx(chairno)->nPlayerStatus);
			m_ptrPlayer[nDrawIndex]->FreshPlayerBetInfo();
			
		}
		
		return;
		
		{
			m_Clock->Show();
			m_Clock->Updata();
			int chairno=ss%TOTAL_CHAIRS;
			GUI_MoveClock(chairno,88); 
		}
		
		
		if (ss%3==1)
		{
			OPE_DealCard();
		}
		else if(ss%3==2)
		{
			for (int i=0;i<TOTAL_CHAIRS;i++)
			{
				
// 				static int sss=0;
// 				if (sss%2) 
// 					m_ptrPlayer[i]->ThrowBet(98765432); 
// 				else
// 					m_ptrPlayer[i]->ThrowBet(76543210);
// 				sss++;
				
				for (int j=0;j<CHAIR_CARDS;j++)
				{
					OPE_ShowChairCard(i,j,0,ss%3);
				}
			}
		}
		else
		{
			OPE_InitData();
		}
		
	}
	
	return;
#endif
	PGL_HIDE_OBJECT(m_btnStart); 
	
	CString sRet;
	//游戏已经开始
	if (GetGameWinApp()->IsGameRunning())
		return;
	
	OPE_ClearGame();
	for(int i=0;i<GetGameWinApp()->GetTableChairCount();i++)
	{
		if (GetGameInfo()->IsChairReady(i))
		{
			int nDrawIndex=RUL_GetDrawIndexByChairNO(i);
			m_ptrPlayer[nDrawIndex]->ShowReady();
		}
	}
	
	if (GetGameWinApp()->IsSoloRoom() && GetGameWinApp()->IsLeaveAlone())
	{
		//solo独离房 
		if (!GetGameWinApp()->GetGameConnect()->GC_StartGameEx(sRet)) //等待回应
		{
			OPE_ChangeSeatedToLookOn();
			return;
		}
	}
	else
	{
		if(!GetGameWinApp()->GetGameConnect()->GC_StartGame(sRet))
		{
			if(GetGameWinApp()->GetOfflineFlag()!=OFFLINEFLAG_OFFLINE)//在线
			{
				AfxMessageBox(sRet,MB_ICONINFORMATION);
				CWnd* pWnd=AfxGetMainWnd();
				if(pWnd) pWnd->PostMessage(WM_CLOSE);
				return ;
			}
		}	 
	}
}

void CMyGame::OnFreshGameSet()
{
	if (GetGameWinApp()->IsSilence())
	{
		GetMusicModule()->Silence(FALSE);
	}
	else
	{
		GetMusicModule()->Silence(TRUE);
	}
	
	if (theApp.m_GameSet.nMusic)
	{
		if (GetGameWinApp()->IsSilence())
		{
			if (!GetMusicModule()->IsPlaying(0))
			{
				GetMusicModule()->PlaySound("default",10000);
			}
		}
		else
		{
			if (!GetMusicModule()->IsPlaying(0))
			{
				GetMusicModule()->Silence(FALSE);
				GetMusicModule()->PlaySound("default",10000);
				GetMusicModule()->Silence(TRUE);
			}
		}
	}
	else
	{
		if (GetGameWinApp()->IsSilence())
		{
			GetMusicModule()->FreeSound("default");
		}
		else
		{
			GetMusicModule()->Silence(FALSE);
			GetMusicModule()->FreeSound("default");
			GetMusicModule()->Silence(TRUE);
		}
	}
	
	//刷新牌
	for (int i=0;i<TOTAL_CHAIRS*CHAIR_CARDS;i++)
	{
		if (m_Card[i]&&m_Card[i]->IsShow()&& m_Card[i]->m_bnFace)
		{
			m_Card[i]->Fresh();
		}
	}
}

void CMyGame::OnGetNewPosition()
{
	CGameWinApp* pApp=(CGameWinApp*)AfxGetApp();
	
	OPE_ClearGame();
	int nTotalChair = GetGameWinApp()->GetTableChairCount();
	int nDrawIndex = 0;
	for (int i=0; i<nTotalChair; i++)
	{
		nDrawIndex = RUL_GetDrawIndexByChairNO(i);
		m_ptrPlayer[nDrawIndex]->UnLoadPlayer();
		if (pApp->IsPlayerEnter(i))
			m_ptrPlayer[nDrawIndex]->LoadPlayer(i);

		if (GetPlayerInfoEx(i)->bInGame && i!=theApp.GetChairNO() && theApp.IsPlayerEnter(i))
			theApp.m_GamePlayer[i].nStatus = ROOM_PLAYER_STATUS_PLAYING;
	}
	
	m_bSitOrStand = TRUE;
	m_bCardDealing = FALSE;
	SetAutoFollow(FALSE);
	SetAutoStart(TRUE);

	theApp.m_GamePlayer[theApp.GetChairNO()].nStatus = ROOM_PLAYER_STATUS_WAITING;
	theApp.m_GameInit.bLookOn = FALSE;

	if (FALSE==GetGameInfoEx()->IsGamePlaying() && IsAutoStart())
	{
		PGL_SHOW_OBJECT(m_btnStart);
		ShowLookOn_Seat_ChangeTableBtn(TRUE, FALSE, TRUE);
		OnStart();
	}
	else
	{
		OPE_DXXW();
	}
}

BOOL  CMyGame::OPE_GiveUp()
{
	CGameRequest* pGameConnect=(CGameRequest* )GetGameWinApp()->GetGameConnect();
	GIVE_UP_REQUEST ope_give_up;
	ZeroMemory(&ope_give_up,sizeof(ope_give_up));
	ope_give_up.dwGameOpe = GAME_OPE_GIVE_UP; 
	ope_give_up.nChairNo = theApp.GetChairNO();

	UINT nResponse= REQ_SendMsgToServer(GAME_MSG_GIVE_UP,sizeof(GIVE_UP_REQUEST),&ope_give_up,TRUE);
	if (nResponse!=UR_OPERATE_FAILED)
	{
		return TRUE;
	}
	return FALSE;
	
}
BOOL  CMyGame::OPE_Bet(int nAddBet)
{ 
	BET_REQUEST ope_bet;
	ZeroMemory(&ope_bet,sizeof(ope_bet));

	ope_bet.nBet = nAddBet;
	ope_bet.nChairNo = theApp.GetChairNO();
	if (nAddBet < 0)
	{
		ope_bet.dwGameOpe = GAME_OPE_BET;
		UINT nResponse= REQ_SendMsgToServer(GAME_MSG_BET,sizeof(BET_REQUEST),&ope_bet,TRUE);
		if (nResponse!=UR_OPERATE_FAILED)
		{
			return TRUE;
		}
	} 

	return FALSE;
}

void	 CMyGame::EnableNextChairOpe(int nChairNo,int nNextChair,DWORD dwOpe)
{
	GetGameInfoEx()->SetCurrentChairNO(nNextChair);
	
	int nNextIndex=RUL_GetDrawIndexByChairNO(nNextChair);
	m_ptrPlayer[nNextIndex]->SetOpeStatus(PLAYER_STATUS_NULL);
	GetGameInfoEx()->SetChairStatus(nNextChair,PLAYER_STATUS_NULL);
	GUI_MoveClock(nNextChair,GetGameInfoEx()->GetBetWait()); 

	if (TRUE == theApp.IsLookOn() || FALSE==CheckRoomPlayerStatus(theApp.GetChairNO()))
		return;

	if(nChairNo==theApp.GetChairNO()) //是我自己
	{
		m_dlgOperation->HideDigitNumber();
		if (IsPlayerGiveUp(nChairNo))
		{//比牌输的玩家显示,当前玩家弃牌
			ShowLookOn_Seat_ChangeTableBtn(TRUE, FALSE, TRUE);
			m_dlgOperation->HidePlayerOpe(GAME_MSG_GIVE_UP|GAME_OPE_LOOK_CARD|GAME_OPE_AUTO_START|GAME_OPE_AUTO_FOLLOW); 
			return;
		}

		m_dlgOperation->ShowNextAutoRestAndFollowBtn();
		if (GetGiveUpBtnConfig() == TRUE)
			m_dlgOperation->HidePlayerOpe(0);
		else
			m_dlgOperation->HidePlayerOpe(GAME_MSG_GIVE_UP);
		 
	}
	else if (nNextChair==theApp.GetChairNO() && 0 != dwOpe && FALSE==IsPlayerGiveUp(theApp.GetChairNO()))//我是下家
	{
		GetGameInfoEx()->SetCouldOpe(dwOpe);
		m_dlgOperation->ShowPlayerCouldOpe(dwOpe); 
	}
}

BOOL     CMyGame::OPE_Rise(int nAddBet)
{
	BET_REQUEST ope_rise;
	ZeroMemory(&ope_rise,sizeof(ope_rise));
	ope_rise.nBet = nAddBet;
	ope_rise.nChairNo = theApp.GetChairNO();

	if(2 == nAddBet)
		ope_rise.dwGameOpe = GAME_OPE_BET_TWO;
	else if(4 == nAddBet)
		ope_rise.dwGameOpe = GAME_OPE_BET_FOUR;
	else if(8 == nAddBet)
		ope_rise.dwGameOpe = GAME_OPE_BET_EIGHT;
	else 
		ope_rise.dwGameOpe = GAME_OPE_RISE;

	UINT nResponse= REQ_SendMsgToServer(GAME_MSG_RISE,sizeof(BET_REQUEST),&ope_rise,TRUE);
	if (nResponse!=UR_OPERATE_FAILED)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CMyGame::OPE_Follow()
{
	FOLLOW_BET_REQUEST ope_follow;
	ZeroMemory(&ope_follow, sizeof(ope_follow));
	ope_follow.dwGameOpe = GAME_OPE_FOLLOW;
	ope_follow.nChairNo= theApp.GetChairNO();

	UINT nResponse = REQ_SendMsgToServer(GAME_MSG_FOLLOW, sizeof(FOLLOW_BET_REQUEST), &ope_follow, TRUE);
	if (nResponse != UR_OPERATE_FAILED)
		return TRUE;

	return FALSE;
}

BOOL CMyGame::OPE_BetFull()
{
	BET_FULL_REQUEST ope_full;
	ZeroMemory(&ope_full,sizeof(ope_full));
	ope_full.dwGameOpe = GAME_OPE_BET_FULL;
	ope_full.nChairNo= theApp.GetChairNO();
	
	UINT nResponse = REQ_SendMsgToServer(GAME_MSG_BET_FULL,sizeof(BET_FULL_REQUEST),&ope_full,TRUE);
	if (nResponse != UR_OPERATE_FAILED)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL   CMyGame::OPE_LookCard()
{
	SHOW_CARD_REQUEST  show_request;
	show_request.dwGameOpe = GAME_MSG_LOOK_CARD;
	show_request.nChairNo = theApp.GetChairNO();
	int nlen = sizeof(SHOW_CARD_REQUEST);
	UINT nResponse= REQ_SendMsgToServer(GAME_MSG_LOOK_CARD,nlen,&show_request,TRUE);
	if (nResponse!=UR_OPERATE_FAILED)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL   CMyGame::OPE_CompareCard()
{
#ifdef _OFFLINE_
	{
		for(int i=0;i<TOTAL_CHAIRS;i++)
		{
			if(i==GetGameWinApp()->GetChairNO()) continue;

			m_ptrPlayer[RUL_GetDrawIndexByChairNO(i)]->ShowCompareBtn();
		}
	}

	return TRUE;
#endif

	int nCount = 0;
	int nLast=-1;
	{
		for (int i = 0; i < m_nTotalPlayer; i++)
		{
			if (FALSE == GetPlayerInfoEx(i)->bInGame || i == theApp.GetChairNO() || IsPlayerGiveUp(i))
				continue;
			
			nCount++;
			nLast = i;
		}
	}

	if (1 == nCount)//只剩一个玩家，直接比牌，不需要选择玩家
	{
		COMPARE_CARD_REQUEST ope_compare;
		ZeroMemory(&ope_compare,sizeof(ope_compare));
		ope_compare.dwGameOpe = GAME_OPE_COMPARE; 
		ope_compare.nChairNo = theApp.GetChairNO();
		ope_compare.nGoalChairNo = nLast;
		UINT nResponse= REQ_SendMsgToServer(GAME_MSG_COMPARE,sizeof(COMPARE_CARD_REQUEST),&ope_compare,TRUE);
		if (nResponse!=UR_OPERATE_FAILED)
			return TRUE;
		
		return FALSE;
	}
	
	{
		for(int i=0;i<TOTAL_CHAIRS;i++)
		{
			if (FALSE == GetPlayerInfoEx(i)->bInGame || i == theApp.GetChairNO() || IsPlayerGiveUp(i))
				continue;
			
			m_ptrPlayer[RUL_GetDrawIndexByChairNO(i)]->ShowCompareBtn();
		}
	}

	return TRUE;
}

BOOL   CMyGame::OPE_CompareCardByIndex(int nDrawIndex)
{
	COMPARE_CARD_REQUEST ope_compare;
	ZeroMemory(&ope_compare,sizeof(ope_compare));
	ope_compare.dwGameOpe = GAME_OPE_COMPARE; 
	ope_compare.nChairNo = theApp.GetChairNO();
	ope_compare.nGoalChairNo = RUL_GetChairNOByDrawIndex(nDrawIndex);

#ifdef _OFFLINE_
//	ope.lostchairno = ope.chairno;
//	PlayCompareCardAni(&ope);
	return TRUE;
#endif

	UINT nResponse= REQ_SendMsgToServer(GAME_MSG_COMPARE,sizeof(COMPARE_CARD_REQUEST),&ope_compare,TRUE);
	if (nResponse!=UR_OPERATE_FAILED)
		return TRUE;

	return FALSE;
}

BOOL   CMyGame::OPE_Allin()
{
	ALL_IN_REQUEST ope_allin;
	ZeroMemory(&ope_allin,sizeof(ope_allin));
	ope_allin.dwGameOpe = GAME_OPE_ALLIN;
	ope_allin.nChairNo = theApp.GetChairNO();
	UINT nResponse= REQ_SendMsgToServer(GAME_MSG_ALL_IN,sizeof(ALL_IN_REQUEST),&ope_allin,TRUE);
	if (nResponse!=UR_OPERATE_FAILED)
	{
		return TRUE;
	}
	return FALSE;
}

void   CMyGame::OnShowCard()
{
	if (theApp.IsLookOn())return;
	SHOW_CARD_REQUEST showCard; 
	ZeroMemory(&showCard,sizeof(showCard));
	showCard.dwGameOpe  = GAME_MSG_SHOW_CARD;
	showCard.nChairNo = theApp.GetChairNO();
	REQ_SendMsgToServer(GAME_MSG_SHOW_CARD,sizeof(SHOW_CARD_REQUEST),&showCard,FALSE);
}

void	CMyGame::OPE_ChangeLookOnToSeated()
{
	if (theApp.IsSoloRoom() && IS_BIT_SET(theApp.GetRoomConfigs(), RC_LEAVEALONE))
	{
		if (FALSE == theApp.IsRandomRoom())
		{
			CString sRet;
			if (!theApp.m_GameRequest.GC_ChangeLookOnToSeated(sRet))
				theApp.ShowMsgInChatView(sRet);
		}
	}
}

void	CMyGame::OPE_ChangeSeatedToLookOn()
{
	if (theApp.IsSoloRoom() && IS_BIT_SET(theApp.GetRoomConfigs(), RC_LEAVEALONE))
	{
		if (FALSE == theApp.IsRandomRoom())
		{
			CString sRet;
			if (!theApp.m_GameRequest.GC_ChangeSeatedToLookOn(sRet))
				theApp.ShowMsgInChatView(sRet);
		}
	}
}

void	CMyGame::OPE_ChangeTable()
{
 	CString sRet;
 	theApp.GetGameConnect()->GC_AskNewTableChair(sRet);
}

void    CMyGame::OnTimer()
{
	if (m_ClockCountDown->IsShow() )
	{
		if (m_ClockCountDown->GetDigit()>0)
			m_ClockCountDown->StepDigit();
		else
			m_nZeroCountDown++;
		
		if ( /*m_Clock->IsShow()&& */!theApp.IsGameRunning() && !theApp.IsLookOn() && STATUS_START_COUNTDOWN == m_dwGameStatus)
		{
			if ((m_ClockCountDown->GetDigit()%5==0 && 0 != m_nZeroCountDown ))
			{
				if (m_ClockCountDown->GetDigit()==0)
				{
					APPLY_WaitFewTimes(100*theApp.GetChairNO()+10,&(SNG_MSG)CreateMsg(GetHandle(),GAME_CALLBACKMSG_COUNTDOWNZERO));
				}
				else
				{
					CGameRequest* pGameConnect=(CGameRequest* )GetGameWinApp()->GetGameConnect();
					pGameConnect->GC_SyncStartCountdown(m_ClockCountDown->GetDigit());
				}
				
				return;
			}
		}
		
		return;
	}

	if (!m_Clock->IsShow())
		return;
	
	if (m_Clock->GetDigit()>0)
	{
		m_nGameZeroCount=0;
		m_Clock->StepDigit();
		
		
		int nDigit=m_Clock->GetDigit();
		int nCurChair=GetGameInfoEx()->GetCurrentChairNO();
		if (m_dwGameStatus==STATUS_PLAYING_GAME && (nDigit==3  || nDigit==1)&& theApp.GetChairNO()==nCurChair)
		{
			PlaySound("snd_TimeAlert");
		}
		if (IsAutoFollow() && nCurChair == theApp.GetChairNO())
		{
			m_nAutoFollowTime--;
			if (m_nAutoFollowTime <= 0 && theApp.IsGameRunning())
			{
				m_nAutoFollowTime = AUTO_FOLLOW_TIME;
				HidePlayersCompareBtn();//跟注后隐藏比牌按钮
				OPE_Follow();
			}
		}
	}
	else if (m_Clock->GetDigit()==0)
	{
		HidePlayersCompareBtn();
		if (m_nGameZeroCount==0 && !GetGameWinApp()->IsLookOn())
		{
			if (OPE_GameClockZero())
				return;
		}

		m_nGameZeroCount++;
		if (m_nGameZeroCount>0 && m_nGameZeroCount%10==2)
		{
			//时钟连续为0，检查当前玩家是否掉线
			if (theApp.IsLookOn())return;
			if (m_dwGameStatus==STATUS_PLAYING_GAME && GetGameInfoEx()->GetCurrentChairNO() != theApp.GetChairNO())
			{
				REQ_SendMsgToServer(SYSMSG_GAME_CLOCK_STOP,0,NULL,FALSE);
			}
		}
	}
}

BOOL    CMyGame::OPE_GameClockZero()
{
	if (GetGameWinApp()->IsLookOn()) return FALSE;
	
	if (GetGameInfo()->GetCurrentChairNO()!=GetGameWinApp()->GetChairNO()) return FALSE;
	
	if(m_dlgOperation&&m_dlgOperation->m_btnGiveUp->IsShow()&&m_dlgOperation->m_btnGiveUp->IsEnable())
	{
		m_nKeepGiveUpTimes++;
		return OPE_GiveUp();
	}

	return FALSE;
}

void   CMyGame::NTF_StartGame(int chairno)
{
	CGameApp* pApp=(CGameApp*)GetGameWinApp();
	
	if (!pApp->IsMatch())
	{
		
		if (chairno!=pApp->GetChairNO())
		{
			CString sMsg;
			sMsg.Format(_T("<c=%d> %s 已经开始\r\n"),
				RGB(128,128,128),
				pApp->GetPlayerNameByChairNO(chairno));
			
			//	pApp->ShowMsgInChatView(sMsg);
		}
		int nDrawIndex=RUL_GetDrawIndexByChairNO(chairno);
		int nPlayerEnter=0;
		for(int i=0;i<GetGameWinApp()->GetTableChairCount();i++)
		{ 
			if (GetGameWinApp()->IsPlayerEnter(i))   
				nPlayerEnter++;
		} 
		m_ptrPlayer[nDrawIndex]->ShowReady();
	}
}

void  CMyGame::NTF_GameStart()
{
	PGL_HIDE_OBJECT(m_btnStart); 
	m_dwGameStatus = STATUS_PLAYING_GAME;
	CGameWinApp* pGameApp=GetGameWinApp();
	if (!theApp.IsReplayMode())
	{
		CString str;  
		
		//基础分
		if (theApp.IsPlayDeposit())
			str.Format("\r\n<size=12>【第%d局】基础银：<c=255>%d<>两\r\n",GetGameInfoEx()->GetBoutCount(),GetGameInfoEx()->GetBaseDeposit());
		else
			str.Format("\r\n<size=12>【第%d局】基础分：<c=255>%d<>分\r\n",GetGameInfoEx()->GetBoutCount(),GetGameInfoEx()->GetBaseScore());
		pGameApp->ShowMsgInChatView(str); 
		
		str.Format("<c=%d><size=14>所有玩家拿到手牌\r\n",RGB(113,203,83));		
	}
	//刷新比赛排行榜
	if (strlen(pGameApp->m_szMatchWWW)>0)
	{
		CAdvertBar* bar=(CAdvertBar*)GetObject(GAMEBASE_GAME_ADVERTBAR);
		if (bar)
		{
			bar->ClickHtmlBtn(_T("刷新排名"));
		}
	}	
	
   	OPE_ClearGame();
	OPE_InitData();  
	OPE_ShowGameInfo(); 
	HideWaitArrageTable();
	HideWaitCountDown();
	SetIsOpenBetFull();
	m_nBetFullDeposit = 0;

	if (!theApp.IsLookOn())
		ShowLookOn_Seat_ChangeTableBtn(FALSE, FALSE, FALSE);

	//玩家设置
	for (int i=0;i<TOTAL_CHAIRS;i++)
	{
		if (!GetPlayerInfoEx(i)->bInGame)
		{
			if (theApp.IsPlayerEnter(i))
			{
               	int nDrawindex=RUL_GetDrawIndexByChairNO(i);
				m_ptrPlayer[nDrawindex]->SetOpeStatus(PLAYER_STATUS_WAIT_START);
			}
			continue;
		}
	
		int nDrawindex=RUL_GetDrawIndexByChairNO(i);
		m_ptrPlayer[nDrawindex]->FreshPlayerBetInfo();
		m_ptrPlayer[nDrawindex]->SetOpeStatus(PLAYER_STATUS_WAIT_BET);
		if (i==GetGameInfoEx()->GetBanker())
		{
			m_ptrPlayer[nDrawindex]->ShowBanker(TRUE);
		}

		theApp.m_GamePlayer[i].nStatus = ROOM_PLAYER_STATUS_PLAYING;
	}

	PlaySound("snd_WaitStart");
	OPE_DealCard();
}

void   CMyGame::NTF_GameWin()
{
	SetAutoFollow(FALSE);
	if ( m_nLeftCompareNum >= 0)//最后二家比牌的，需要放动漫后再操作
	{
		m_bGameWin = TRUE;
		return;
	}

	GameWin();
}

void   CMyGame::GameWin()
{
	CGameWinApp* pGameApp=GetGameWinApp();
	if (!pGameApp->IsLookOn())
		xyAppCallAttention();//提醒用户操作
	{
		for (int i = 0; i < TOTAL_CHAIRS; i++)
			m_ptrPlayer[i]->HideCompareBtn();
	}

	GAME_WIN_RESULT* pGameWinResult = GetGameInfoEx()->GetGameWin();

	int nMyChairNo = theApp.GetChairNO();
	int* pKiller =  pGameWinResult->nKiller;

	{
		if (!GetGameWinApp()->IsLookOn() && theApp.m_GamePlayer[nMyChairNo].nStatus==ROOM_PLAYER_STATUS_PLAYING)
		{//显示自己的手牌
			OPE_ShowChairCard(nMyChairNo, 0, pGameWinResult->nChairCards[nMyChairNo][0], 0);
			OPE_ShowChairCard(nMyChairNo, 1, pGameWinResult->nChairCards[nMyChairNo][1], 0);
			OPE_ShowChairCard(nMyChairNo, 2, pGameWinResult->nChairCards[nMyChairNo][2], 0);
			int nDrawIndex = RUL_GetDrawIndexByChairNO(nMyChairNo);
			m_ptrPlayer[nDrawIndex]->HideBrightOrDarkBoard();
			m_ptrPlayer[nDrawIndex]->HideLostLightningMask();
		}

		if (pKiller[nMyChairNo] != -1 &&!GetGameWinApp()->IsLookOn()
			&& theApp.m_GamePlayer[nMyChairNo].nStatus==ROOM_PLAYER_STATUS_PLAYING)
		{//显示比赢我的人的手牌
			OPE_ShowChairCard(pKiller[nMyChairNo], 0, pGameWinResult->nChairCards[pKiller[nMyChairNo]][0], 0);
			OPE_ShowChairCard(pKiller[nMyChairNo], 1, pGameWinResult->nChairCards[pKiller[nMyChairNo]][1], 0);
			OPE_ShowChairCard(pKiller[nMyChairNo], 2, pGameWinResult->nChairCards[pKiller[nMyChairNo]][2], 0);
			int nDrawIndex = RUL_GetDrawIndexByChairNO(pKiller[nMyChairNo]);
			m_ptrPlayer[nDrawIndex]->HideBrightOrDarkBoard();
			m_ptrPlayer[nDrawIndex]->HideLostLightningMask();
		}
		
		for(int i=0;i<TOTAL_CHAIRS;i++)
		{//显示被我比赢的人的手牌
			if (pKiller[i] != nMyChairNo || GetGameWinApp()->IsLookOn() 
				|| (pKiller[i]==nMyChairNo && theApp.m_GamePlayer[i].nStatus!=ROOM_PLAYER_STATUS_PLAYING)) 
				continue;
			
			OPE_ShowChairCard(i, 0, pGameWinResult->nChairCards[i][0], 0);
			OPE_ShowChairCard(i, 1, pGameWinResult->nChairCards[i][1], 0);
			OPE_ShowChairCard(i, 2, pGameWinResult->nChairCards[i][2], 0);
			
			int nDrawIndex = RUL_GetDrawIndexByChairNO(i);
			m_ptrPlayer[nDrawIndex]->HideBrightOrDarkBoard();
			m_ptrPlayer[nDrawIndex]->HideLostLightningMask();
		}
	}

	int nWinChair = pGameWinResult->nWinChair;
	int nWinDrawIndex = RUL_GetDrawIndexByChairNO(nWinChair);
	int nWinType = pGameWinResult->nWinType;
	if(theApp.GetChairNO() == nWinChair || (pKiller[nMyChairNo] != -1&&!GetGameWinApp()->IsLookOn()))
	{
		m_ptrPlayer[nWinDrawIndex]->ShowWinAni(nWinType);
	}

	{ //聊天框显示自己的输赢信息
		if (FALSE==theApp.IsLookOn() && GetPlayerInfoEx(nMyChairNo)->bInGame 
			&& theApp.m_GamePlayer[nMyChairNo].nStatus==ROOM_PLAYER_STATUS_PLAYING)
		{
			int nWinPoint=pGameWinResult->gamewin.nWinPoints[theApp.GetChairNO()];
			int nBoutCount=pGameWinResult->gamewin.nBoutCount;
			CString str;
			if (nWinPoint>0)
				str.Format(_T("【第%d局】<c=255>恭喜！您赢了%d两银子！<>\r\n"),nBoutCount,nWinPoint);	
			else if (nWinPoint<0)
				str.Format(_T("【第%d局】很遗憾！本局您输了%d两银子！\r\n"),nBoutCount,	-nWinPoint);
			else
				str.Format(_T("【第%d局】真可惜！本局您未有斩获！\r\n"),nBoutCount); 

			theApp.ShowMsgInChatView(str);
		}
	}

	OPE_TakeBet();//赢家收取所有筹码
	for (int i = 0; i < TOTAL_CHAIRS; i++)
	{
		if (FALSE == GetPlayerInfoEx(i)->bInGame)
			continue;

		if (theApp.m_GamePlayer[i].nStatus != ROOM_PLAYER_STATUS_PLAYING)
			continue;

		//显示各个玩家银两输赢情况
		int nDrawIndex = RUL_GetDrawIndexByChairNO(i);
		m_ptrPlayer[nDrawIndex]->MOVIE_FreshRestBet(pGameWinResult->gamewin.nWinPoints[i], nWinChair, nWinType);
		if (theApp.IsPlayerEnter(i))
			theApp.m_GamePlayer[i].nStatus = ROOM_PLAYER_STATUS_WAITING;
	}

	m_dlgOperation->HidePlayerOpe(GAME_OPE_GIVE_UP|GAME_OPE_LOOK_CARD|GAME_OPE_AUTO_START|GAME_OPE_AUTO_FOLLOW);
	m_dlgOperation->HideDigitNumber();

    OPE_EndTimer();
    OPE_ShowResult();
	
	//聊天框结果信息
	//	theApp.ShowMsgInChatView("\r\n");
	// 	CString str;
	// 	str.Format(" <c=%d><size=14>===本局结束===\r\n",RGB(0,0,0));
	// 	pGameApp->ShowMsgInChatView(str);  
	
	PlaySound("snd_GameWin");
	
	//刷新比赛排行榜
	if (strlen(pGameApp->m_szMatchWWW)>0)
	{
		CAdvertBar* bar=(CAdvertBar*)GetObject(GAMEBASE_GAME_ADVERTBAR);
		if (bar)
		{
			bar->ClickHtmlBtn(_T("刷新排名"));
		}
	}

	if (m_dlgAutoSetDeposit)
	{
		if (m_dlgAutoSetDeposit->IsAutoSetDeposit())
		{
			AutoSetDeposit();
		}
	}
	
	if (pGameApp->IsLookOn())
		return;

	if (m_nKeepGiveUpTimes >= 2)//踢出玩家
	{
		SetAutoStart(FALSE);
		m_nKeepGiveUpTimes = 0;
	}
	
	if (pGameApp->IsReplayMode())
		return;
}

void CMyGame::NTF_StartCountdown(START_COUNTDOWN* pCountdown)
{
	if (pCountdown->bStartorStop)
	{
		OPE_StartIdleTimer();
		GUI_MoveClockIdle(theApp.GetChairNO(), pCountdown->nCountDown);
		if (m_dwGameStatus==STATUS_PLAYING_GAME)
		{

		}
		else
		{
			ShowWaitCountDown();  
		}
		
		m_dwGameStatus = STATUS_START_COUNTDOWN;
		
	}
	else
	{
		//	theApp.ShowMsgInChatView(_T("人数不足，停止倒计时。\r\n"));
		HideWaitCountDown();
		m_nZeroCountDown=0;
		OPE_EndTimer();
		m_dwGameStatus = STATUS_WAITING_START;
	}
}

void CMyGame::NTF_SyncStartCountdown(SYNC_COUNTDOWN* pSyncCountdown)
{
	if (theApp.IsLookOn())
		return;
	
	if (m_ClockCountDown->IsShow() && STATUS_START_COUNTDOWN == m_dwGameStatus)///?
	{
		// 		CString str;
		// 		str.Format(_T("同步时间 %d  -》　%d\r\n"), m_ClockCountDown->GetDigit(),pSyncCountdown->nCountDown);
		// 		theApp.ShowMsgInChatView(str);
		m_ClockCountDown->SetDigit(pSyncCountdown->nCountDown);
		m_ClockCountDown->Updata();
	}
}

void  CMyGame::NTF_UserDepositEvent(LPUSER_DEPOSITEVENT pUser_Deposit)
{
	if (NULL != theApp.GetVisitorPlayerByUserID(pUser_Deposit->nUserID))
		return;

	CGameWinApp* pGameApp=(CGameWinApp*)AfxGetApp();
	
	int chairno=pUser_Deposit->nChairNO;
    int nFinalDeposit=pUser_Deposit->nDeposit;
	GetGameInfoEx()->SetChairTotalDeposit(chairno,nFinalDeposit);
	GetGameInfoEx()->SetChairPlayerDepoist(chairno, nFinalDeposit);
	GetGameInfoEx()->SetChairUsedDeposit(chairno,0);
	int nDrawIndex=RUL_GetDrawIndexByChairNO(chairno);
	if (nDrawIndex!=-1)
	{ 
		StopObject(m_ptrPlayer[nDrawIndex]->m_digitRestBet->GetHandle()); ///add 0818
		m_ptrPlayer[nDrawIndex]->FreshPlayerBetInfo();
		m_ptrPlayer[nDrawIndex]->SetTakeDeposit(TRUE);
	}
}

void      CMyGame::NTF_UserBoutEnd(LPUSER_BOUTEND pUser_BoutEnd)
{
	int chairno=pUser_BoutEnd->nChairNO;
	int nFinalDeposit=theApp.m_GamePlayer[chairno].nDeposit ;
	GetGameInfoEx()->SetChairTotalDeposit(chairno,nFinalDeposit);
	GetGameInfoEx()->SetChairPlayerDepoist(chairno, nFinalDeposit);
	GetGameInfoEx()->SetChairUsedDeposit(chairno,0);
	int nDrawIndex=RUL_GetDrawIndexByChairNO(chairno);
	if (nDrawIndex!=-1)
	{ 
		m_ptrPlayer[nDrawIndex]->FreshPlayerBetInfo();
	}
}

void    CMyGame::OPE_InitData()
{
	m_nTotalPlayer=TOTAL_CHAIRS; 

	{
		//隐藏所有筹码
		m_nBetCount = 0;
		for(int i=0;i<MAX_BET_ALL_COUNT;i++)
		{
			m_bitThrownBet[i] = NULL;
			m_TrownPoint[i].x = 0;
			m_TrownPoint[i].y = 0;
		}
	}
	
	for(int i=0;i<TOTAL_CHAIRS*CHAIR_CARDS;i++)
	{ 
		m_Card[i]->m_nChairNO=-1;
		m_Card[i]->m_nUnitIndex=i;  
        m_Card[i]->m_nStatus=0;
		m_Card[i]->Hide();
		m_Card[i]->Updata();
		
		
	}
    
	for(i=0;i<m_nTotalPlayer;i++)
	{
		m_ptrPlayer[RUL_GetDrawIndexByChairNO(i)]->Restart();
	}
	m_Clock->Hide();
	m_Clock->Updata();
	//结束后重置
    if (m_hEventTimer)
    {
		StopApply(m_hEventTimer);
    }
}

void      CMyGame::OPE_ClearGame()
{
	ResetAllCards();
		
	PGL_HIDE_OBJECT(m_betInfoBoard);
	PGL_HIDE_OBJECT(m_bitIconMaxDeposit);
	m_dlgOperation->HidePlayerOpe(GAME_OPE_GIVE_UP|GAME_OPE_LOOK_CARD|GAME_OPE_AUTO_START|GAME_OPE_AUTO_FOLLOW);
	m_dlgOperation->HideDigitNumber();
	m_dlgOperation->HideShowCardBtn();

	for(int i=0;i<m_nTotalPlayer;i++)
	{
		int nDrawIndex=RUL_GetDrawIndexByChairNO(i);
		m_ptrPlayer[nDrawIndex]->Restart();
		m_ptrPlayer[nDrawIndex]->HideNewDigitAni();
	}

	for(i=0;i<m_nBetCount;i++)
		PGL_HIDE_OBJECT(m_bitThrownBet[i]);
	
	//隐藏结果框
	PGL_HIDE_OBJECT(m_plGameInfo); 
	
	GetGameFrame()->HideBoard(); 
	GetGameFrame()->OPE_HideMatchResult();

	m_Clock->Hide();
	m_Clock->Updata(); 

	StopCompareCardAni();
}

void    CMyGame::OPE_ShowGameInfo()
{
	return;
	if (!m_plGameInfo)
	{
		SNG_PIC& pic=Res_GetPICStruct("游戏信息框");
		m_plGameInfo=CreatePlane(pic.width,pic.height);
		m_plGameInfo->TransGUI();
		m_plGameInfo->DrawPic(pic);
		m_plGameInfo->Updata();
		
		PGL_MSG_CLASS(m_digitBase,CXyDigit);
		m_digitBase->CreateDigit("digit_basescore",DIGIT_FLAGS_MID_JUSITIFY);
		m_digitBase->LoadWord("分",Res_GetPICStruct("str_分"));
		m_digitBase->Updata();
	}
	
	m_plGameInfo->Erasure();
	m_plGameInfo->DrawPic(Res_GetPICStruct("游戏信息框"));
	/////////////////////////////////////////////////////////////
	SNG_PIC picBasescore=Res_GetPICStruct("str_基础分");
	m_plGameInfo->DrawPic(picBasescore,10,10,picBasescore.width,picBasescore.height);
	
	CString strBaseScore;
	strBaseScore.Format("%d分",GetGameInfo()->GetBaseScore());
	m_digitBase->WriteString(m_plGameInfo,80,10,strBaseScore,DIGIT_FLAGS_LEFT_JUSITIFY);
	
	m_plGameInfo->Show();
	m_plGameInfo->SetAlpha(0);
	m_plGameInfo->Updata();
	APPLY_ShowObject(m_plGameInfo,0.1,1);
	
}

void   CMyGame::OPE_DealCard()
{
	m_bCardDealing = TRUE;
	m_nDealCount=0; 
	APPLY_WaitFewTimes(200,&(SNG_MSG)CreateMsg(GetHandle(),GAME_CALLBACKMSG_DEALCARD));
}

void CMyGame::MOVIE_DealCard()
{
	int nBanker=GetPublicInfoEx()->nBanker;
	int cardcount=m_nDealCount;
	int chairno=(m_nDealCount+nBanker)%TOTAL_CHAIRS;
	int nUnitIndex=m_nDealCount/TOTAL_CHAIRS;
	m_nDealCount++; 
	//根据m_nDealCount得到chairno nUnitIndex
	int n=0;
	int tIdx=0;
	while (n!=m_nDealCount)
	{
		if (FALSE == m_bCardDealing)
		{
			OnGetNewPosition();
			return;
		}
		
		for (int i=nBanker;i>=0;i--)
		{
			if(GetPlayerInfoEx(i)->bInGame)
				n++; 
			if (n==m_nDealCount)
			{
				chairno=i;
				nUnitIndex=tIdx;
				break;
			}
		}
		if (n==m_nDealCount)
		{
			break;
		}
		for (i=TOTAL_CHAIRS;i>nBanker;i--)
		{
			if(GetPlayerInfoEx(i)->bInGame)
				n++;
			if (n==m_nDealCount)
			{
				chairno=i;
				nUnitIndex=tIdx;
				break;
			}
		}
		tIdx++;
	}
	
	BOOL bnFace=(chairno==theApp.GetChairNO()&&!theApp.IsLookOn());
	POINT ptStart=GetPokerDealPosition();
	m_Card[cardcount]->MoveTo(ptStart.x,ptStart.y); 
	m_Card[cardcount]->m_nChairNO=chairno;
	m_Card[cardcount]->m_nDrawIndex=RUL_GetDrawIndexByChairNO(chairno); 
	m_Card[cardcount]->m_nUnitIndex=nUnitIndex;
	m_Card[cardcount]->m_bnFace=FALSE;
	m_Card[cardcount]->Fresh();
	
	int nDrawIndex=RUL_GetDrawIndexByChairNO(chairno);
	POINT ptDest= m_ptrPlayer[nDrawIndex]->GetCardPositon(bnFace,nUnitIndex); 
	//	   m_nDealCount++; 
	double speed=90; 
	PlaySound("snd_MoveCard");
	APPLY_MoveObject(m_Card[cardcount],ptDest,speed,6,speed+30,&(SNG_MSG)CreateMsg(GetHandle(),GAME_CALLBACKMSG_DEALCARD_END,m_nDealCount,(DWORD)m_Card[cardcount]));  
	
}

void CMyGame::MOVIE_DealCardEnd(SNG_MSG& msg)
{
	
	int nDealCount=(int)msg.param1;
	CMyCard*pCard=(CMyCard*)msg.param2;
	int chairno=pCard->m_nChairNO;
	int nUnitIdex=pCard->m_nUnitIndex;

	pCard->Fresh();
	int nTotalDeal=0;
	for(int i=0;i<TOTAL_CHAIRS;i++)
	{
		if(GetPlayerInfoEx(i)->bInGame)
			nTotalDeal++;
	}
	nTotalDeal*=3;
	
	if (nDealCount==nTotalDeal )
	{
		APPLY_WaitFewTimes(500,&(SNG_MSG)CreateMsg(GetHandle(),GAME_CALLBACKMSG_OPE_START));
	}
	else
	{
		APPLY_WaitFewTimes(1,&(SNG_MSG)CreateMsg(GetHandle(),GAME_CALLBACKMSG_DEALCARD));
	}
}

void  CMyGame::OPE_GameStart()
{
	if (FALSE == theApp.IsGameRunning())
		return;

	m_bCardDealing = FALSE;
	//显示明、暗牌
	{
		for (int i = 0; i < m_nTotalPlayer; i++)
		{
			if(FALSE == GetPlayerInfoEx(i)->bInGame 
				|| (RUL_GetDrawIndexByChairNO(i) == MYDRAWINDEX && !theApp.IsLookOn()))
				continue;

 			m_ptrPlayer[RUL_GetDrawIndexByChairNO(i)]->ShowBrightOrDarkBoard(GetGameInfoEx()->GetGameTableInfo()->m_Public.bnLookCard[i]);
		}
	}
	
	OPE_StartTimer();
#ifdef _OFFLINE_
	GUI_MoveClock(0,99);
#else
	GUI_MoveClock(GetGameInfo()->GetCurrentChairNO(),GetGameInfoEx()->GetBetWait());
#endif
	

	PlaySound("snd_GameStart"); 
	FreshBetInfo(theApp.GetChairNO());
	FreshTotalDepositInfo();

	//玩家设置
	for (int i=0;i<TOTAL_CHAIRS;i++)
	{
		if (!GetPlayerInfoEx(i)->bInGame)continue;
		
		int nDrawindex=RUL_GetDrawIndexByChairNO(i);
		
		OPE_ThrowChips(i,1);//丢基础银
	}
    int nCurrentChair=GetGameInfoEx()->GetCurrentChairNO();
	if (FALSE == theApp.IsLookOn() && CheckRoomPlayerStatus(theApp.GetChairNO()))
		m_dlgOperation->ShowPlayerCouldOpe(GetGameInfoEx()->GetCouldOpe()); 

//////////////////////////////////////////////////////////////////////////
	OnGameSize();
}

void  CMyGame::OPE_DXXW()
{
	int nMyChairNo = theApp.GetChairNO();
	if (!theApp.IsLookOn()&&GetGameInfoEx()->IsUserIDEqual(nMyChairNo))
		REQ_SendMsgToServer(SYSMSG_PLAYER_ONLINE,0,NULL,FALSE);
	
	m_dwGameStatus = STATUS_PLAYING_GAME;
	m_nBetCount = 0;
	int nDrawIndex = -1;

	{
		for(int i=0;i<GetGameWinApp()->GetTableChairCount();i++)
		{
			nDrawIndex=RUL_GetDrawIndexByChairNO(i);

			if (FALSE == GetPlayerInfoEx(i)->bInGame)
				continue;

			//设置筹码
			OPE_ThrowOneChips(i, 1, TRUE);
			int restbet=GetPublicInfoEx()->nCurrentBet[i];
			int bet4,bet2,bet1;
			FitBet(restbet, bet4, bet2, bet1);
			for(int j=0;j<bet4;j++)
				OPE_ThrowOneChips(i,4,TRUE);
			
			for(j=0;j<bet2;j++)
				OPE_ThrowOneChips(i,2,TRUE);
			
			for(j=0;j<bet1;j++)
				OPE_ThrowOneChips(i,1,TRUE);
		}
	}

	{
		//更新玩家状态
		int cardcount=0;
		for (int i=0; i<TOTAL_CHAIRS; i++)
		{
			nDrawIndex = RUL_GetDrawIndexByChairNO(i); 
			if (TRUE == CheckRoomPlayerStatus(i))
			{//在游戏中的玩家处理
				int nDrawIndex=RUL_GetDrawIndexByChairNO(i); 
				m_ptrPlayer[nDrawIndex]->ShowBanker(i==GetGameInfoEx()->GetBanker());
				m_ptrPlayer[nDrawIndex]->SetOpeStatus(GetPlayerInfoEx(i)->nPlayerStatus);
				m_ptrPlayer[nDrawIndex]->FreshPlayerBetInfo();
				
				//设置明、暗牌
				if (FALSE == IsPlayerGiveUp(i))
					m_ptrPlayer[nDrawIndex]->ShowBrightOrDarkBoard(GetPublicInfoEx()->bnLookCard[i]);
				else
					m_ptrPlayer[nDrawIndex]->ShowBrightOrDarkBoard(GetPublicInfoEx()->bnLookCard[i], TRUE);
			}
			else
			{//不在游戏中的玩家处理
				m_ptrPlayer[RUL_GetDrawIndexByChairNO(i)]->SetOpeStatus(PLAYER_STATUS_WAIT_START);
			}
		}
	}
	
	UpdateAllCards();
	FreshBetInfo(theApp.GetChairNO());
	FreshTotalDepositInfo();
	
	//显示当前操作
	OPE_StartTimer();
	m_nGameZeroCount=0;
    DWORD dwStatus=GetGameInfoEx()->GetStatus();
	if (theApp.IsGameRunning())
    { 
		int nCurrentChair=GetGameInfoEx()->GetCurrentChairNO();
		nDrawIndex=RUL_GetDrawIndexByChairNO(nCurrentChair);
		m_ptrPlayer[nDrawIndex]->SetOpeStatus(PLAYER_STATUS_NULL);
		GUI_MoveClock(nCurrentChair,GetGameInfoEx()->GetOpeRestTime()); 
		m_ptrPlayer[nDrawIndex]->SetCurrentChair(TRUE,GetGameInfoEx()->GetOpeRestTime());	
    }
	else
	{
		m_Clock->Hide();
		m_Clock->SetDigit(0);
		m_Clock->Updata();
	}
	
	if (CheckRoomPlayerStatus(nMyChairNo))
		m_dlgOperation->ShowPlayerCouldOpe(GetGameInfoEx()->GetCouldOpe()); 
	else
		ShowLookOn_Seat_ChangeTableBtn(TRUE, FALSE, TRUE);
}

void  CMyGame::GUI_MoveClock(int chairno,int second)
{
	if (second>=0)//当time<0的时候，时间不变化
	{
		m_Clock->SetDigit(second);
	}

	int nDrawIndex=RUL_GetDrawIndexByChairNO(chairno);
	
	POINT pt=m_ptrPlayer[nDrawIndex]->GetClockPosition();
//	m_Clock->Show(); 
	m_Clock->MoveTo(pt.x,pt.y);
	if (theApp.GetChairNO() == chairno)
	{
		PGL_SHOW_OBJECT(m_Clock);
	}
	else
	{
		PGL_HIDE_OBJECT(m_Clock);
	}

	GetGameInfo()->SetCurrentChairNO(chairno); 
	
	for(int i=0;i<TOTAL_CHAIRS;i++)
	{
		m_ptrPlayer[RUL_GetDrawIndexByChairNO(i)]->SetCurrentChair(i == chairno);
	}
}

void  CMyGame::GUI_MoveClockIdle(int chairno,int second)
{
	if (second>=0)//当time<0的时候，时间不变化
	{
		m_ClockCountDown->SetDigit(second);
	}
	
	RECT rect=GetGameRect();
	int x=rect.left;
	int y=rect.top;
	int w=rect.right-rect.left;
	int h=rect.bottom-rect.top;
	
	//	POINT pt=m_ptrPlayer[nDrawIndex]->GetClockPosition(1);
	POINT pt=GetWaitCountDownDigitPostion(); 
	if(	m_dwGameStatus==STATUS_PLAYING_GAME)
    {
		m_ClockCountDown->Hide();
		
	}
	else
	{
		m_ClockCountDown->Show(); 
	}
	
	m_ClockCountDown->MoveTo(pt.x,pt.y);
	m_ClockCountDown->Updata(); 
	
	for(int i=0;i<TOTAL_CHAIRS;i++)
	{
		m_ptrPlayer[RUL_GetDrawIndexByChairNO(i)]->SetCurrentChair(FALSE);
	}
}

GAME_START_DATA* CMyGame::GetStartData()
{
	return (GetGameInfoEx()->GetGameStartData());
}


GAME_PUBLIC_INFO* CMyGame::GetPublicInfoEx()
{
	return &(GetGameInfoEx()->GetGameTableInfo()->m_Public);
}


GAME_PLAYER_INFO* CMyGame::GetPlayerInfoEx(int chairno)
{
	return &(GetGameInfoEx()->GetGameTableInfo()->m_Player[chairno]);
}

CGameInfoEx* CMyGame::GetGameInfoEx()
{
	CGameApp* pApp=(CGameApp*)GetGameWinApp();
	return (CGameInfoEx*)pApp->GetGameInfo();
}

void  CMyGame::RUL_CalcGameStatus(int w,int h)
{
	if (w>980&&h>690) 
		Sys_SetIntStatus(GAME_STATUS_TABLE_SIZE,1); 
   	else 
		Sys_SetIntStatus(GAME_STATUS_TABLE_SIZE,0);
	// 	int nTableSize=Sys_GetIntStatus(GAME_STATUS_TABLE_SIZE);
	// 	CString str;///-
	// 	str.Format("nTableSize=%d",nTableSize);
	// 	theApp.ShowMsgInChatView(str);
	Sys_SetIntStatus(GAME_STATUS_RGB_ME,RGB(0,128,64));
	Sys_SetIntStatus(GAME_STATUS_RGB_PLAYER,RGB(48,113,246));
	Sys_SetIntStatus(GAME_STATUS_RGB_SYSTEM,RGB(64,64,64));
}

RECT   CMyGame::RUL_GetZgSaTableRect()
{
	RECT rc=GetMainGameRect(); 
    int nTableStatus=Sys_GetIntStatus(GAME_STATUS_TABLE_SIZE);
	
	
	if (nTableStatus==0) //小	785 *571
	{
		int wTable=785;
		int hTable=571;
		rc.left=rc.left+(rc.right-rc.left-wTable)/2;
		rc.top=rc.top+(rc.bottom-rc.top-hTable)/2;
		rc.right=rc.left+wTable;
		rc.bottom=rc.top+hTable;
	}
	else
	{
		int wTable=1000;
		int hTable=700;
		rc.left=(rc.right-rc.left-wTable)/2+rc.left;
		rc.top=(rc.bottom-rc.top-hTable)/2+rc.top;
		rc.right=rc.left+wTable;
		rc.bottom=rc.top+hTable;
	}  

	return rc;
}

int  CMyGame::RUL_GetDrawIndexByChairNO(int nChairNO)
{
	int index=MYDRAWINDEX;
	int chairno=GetGameWinApp()->GetChairNO();
	
	while(1)
	{
		if (chairno==nChairNO)
		{
			return index;
		}
		index = RUL_GetNextIndex(index);
		chairno=RUL_GetNextChairNO(chairno);  
	} 
	return -1;
}

int  CMyGame::RUL_GetChairNOByDrawIndex(int nDrawIndex)
{ 	
	int index=MYDRAWINDEX;
	int chairno=GetGameWinApp()->GetChairNO();
	
	while(1)
	{
		if (index==nDrawIndex)
		{
			return chairno;
		}
		index = RUL_GetNextIndex(index);
		chairno=RUL_GetNextChairNO(chairno);  
	} 
	return -1;
}

int  CMyGame::RUL_GetPrevChairNO(int nChairNO)
{
	int nTotalChair = GetGameWinApp()->GetTableChairCount();
	return (nChairNO+1)% nTotalChair;
}

int  CMyGame::RUL_GetNextChairNO(int nChairNO)
{
	int nTotalChair = GetGameWinApp()->GetTableChairCount();
	return (nChairNO-1+m_nTotalPlayer)% nTotalChair;
}

int  CMyGame::RUL_GetPrevIndex(int nDrawIndex)
{
	return  (nDrawIndex+1)% TOTAL_CHAIRS;
}

int CMyGame::RUL_GetNextIndex(int nDrawIndex)
{
	
	return (nDrawIndex-1+TOTAL_CHAIRS)% TOTAL_CHAIRS;
}

UINT CMyGame::REQ_SendMsgToServer(UINT msg,int datalen,void* data,BOOL bnNeedEcho)
{
	CGameRequest* pGameConnect=(CGameRequest* )GetGameWinApp()->GetGameConnect();
	if (!bnNeedEcho)
		return pGameConnect->SendMsgToServer(msg,datalen,data,FALSE);
	
	//需要等待回应
	UINT nResponse=0;
	void* EchoDataPtr=pGameConnect->SendMsgToServerEx(msg,nResponse,datalen,data);
	if (EchoDataPtr)
	{
		GetGameWinApp()->GetGameNotify()->OnNotifyReceived(GR_SENDMSG_TO_PLAYER,EchoDataPtr,datalen);
		SAFE_DELETE_ARRAY(EchoDataPtr);
		//NTF_GameMsg((GAME_MSG*)EchoDataPtr);
		//SAFE_DELETE_ARRAY(EchoDataPtr);
	}
	return nResponse;
}

void  CMyGame::NTF_PlayerOffline(int nUserID)
{
	if (!GetGameWinApp()->IsTablePlayer(nUserID))
		return;
	int chairno=GetGameWinApp()->GetChairNOByUserID(nUserID);
	if(!GetGameWinApp()->IsLookOn()
		&&chairno==GetGameWinApp()->GetChairNO())
	{
		//判断我自己断线了，错误，申报
		REQ_SendMsgToServer(SYSMSG_PLAYER_ONLINE,0,NULL,FALSE);
	}
	else
	{
		CString str;
		str.Format(" <z=tc系统通知> <c=%d>%s掉线了!\r\n",RGB(125,150,75),GetGameWinApp()->GetUserNameByChairNO(chairno));
		GetGameWinApp()->ShowMsgInChatView(str);
		int nDrawIndex=RUL_GetDrawIndexByChairNO(chairno);
		//	m_ptrPlayer[nDrawIndex]->Offline(TRUE);
	}	
}

void CMyGame::NTF_GameMsg(GAME_MSG* pGameMsg)
{
	//解密工作
	DWORD key=pGameMsg->nMsgID;
	void* data=((PBYTE)pGameMsg)+sizeof(GAME_MSG);//返回数据位置
	DWORD len=pGameMsg->nDatalen;
	
    if (key>SYSMSG_BEGIN
		&&key<SYSMSG_END)
		NTF_SystemMsg(key,data,len);
	else if (key>LOCAL_GAME_MSG_BEGIN
		&&key<LOCAL_GAME_MSG_END)
		NTF_LocalMsg(key,data,len);
}

void	CMyGame::NTF_SystemMsg(int nMsgID,void* data,DWORD datalen)
{
	switch(nMsgID)
	{
		
	case SYSMSG_RETURN_GAME:
		{
			int chairno=*(int*)data;
			int nDrawIndex=RUL_GetDrawIndexByChairNO(chairno);
			//	if (m_ptrPlayer[nDrawIndex]->IsOffline())
			{
				CString str;
				str.Format(" <z=tc系统通知> <c=%d>%s重新回到了游戏!\r\n",Sys_GetIntStatus(GAME_STATUS_RGB_PLAYER),GetGameWinApp()->GetUserNameByChairNO(chairno));
				GetGameWinApp()->ShowMsgInChatView(str);
				
				//GetMusic()->PlaySound("Snd_ReturnGame");
				//m_Player[*chairno]->HideOffline();
				
				//	m_ptrPlayer[nDrawIndex]->Offline(FALSE);
				//	m_Clock->Show();
				//有人断线回来，大家重调计时器等他一下。  ///?
				// 				if (m_Clock->IsShow()
				// 					&&chairno==GetGameInfo()->GetCurrentChairNO())
				// 					m_Clock->SetDigit(GetGameInfoEx()->GetBetWait());
			}
		}
		break;
	}
	
}

void	CMyGame::NTF_LocalMsg(int nMsgID,void* data,DWORD datalen)
{
	//nUsedDeposit:玩家已经使用银两(总下注银两) nRoundCurDeposit:本次下注消耗银两 nTotalDeposit:游戏总下注银两
	int chairno = -1;
	int nChairStatus = PLAYER_STATUS_NULL;
	int nBet = 0;
	int nNextChair = -1;
	DWORD dwCouldOpe = 0;
	int nUsedDeposit = 0;
	int nRoundCurDeposit = 0;
	int nTotalDeposit = 0;
	COMPARE_CARD_RESPONSE mRes;
	memset(&mRes, 0, sizeof(COMPARE_CARD_RESPONSE));
	switch(nMsgID)
	{
	case GAME_MSG_BET:
		{
			if (NULL != data)
			{
				BET_RESPONSE* pRes_bet = (BET_RESPONSE*)data;
				chairno = pRes_bet->nChairNo;
				nChairStatus = PLAYER_STATUS_BET;

				GetGameInfoEx()->GetGameTableInfo()->m_Public.nCurrentDouble = BET_ONE;

				nBet = pRes_bet->nBet;
				nRoundCurDeposit = nBet * GetGameInfoEx()->GetGameStartData()->nBaseDeposit;
				nUsedDeposit = GetGameInfoEx()->GetChairUsedDeposit(chairno) + nRoundCurDeposit;
				nTotalDeposit =  GetGameInfoEx()->GetTotalDesposit() + nRoundCurDeposit;
				nNextChair =pRes_bet->nNextChair;
				dwCouldOpe= pRes_bet->dwNextCouldOpe;
			}
		}
		break;
	case GAME_MSG_RISE:
		{
			if (NULL != data)
			{
				BET_RESPONSE* pRes_rise = (BET_RESPONSE*)data;
				chairno = pRes_rise->nChairNo;
				nChairStatus = PLAYER_STATUS_RISE;

				nBet = pRes_rise->nBet;
				nRoundCurDeposit = nBet * GetGameInfoEx()->GetGameStartData()->nBaseDeposit;
				nUsedDeposit = GetGameInfoEx()->GetChairUsedDeposit(chairno) + nRoundCurDeposit;
				nTotalDeposit =  GetGameInfoEx()->GetTotalDesposit() + nRoundCurDeposit;

				if (GetGameInfoEx()->GetGameTableInfo()->m_Public.bnLookCard[chairno])
				{
					GetGameInfoEx()->GetGameTableInfo()->m_Public.nCurrentDouble = nBet/2;
				} 
				else
				{
					GetGameInfoEx()->GetGameTableInfo()->m_Public.nCurrentDouble = nBet;
				}
				
				nNextChair = pRes_rise->nNextChair;
				dwCouldOpe = pRes_rise->dwNextCouldOpe;
			}
		}
		break;
	case GAME_MSG_GIVE_UP:
		{			
			if (NULL != data)
			{
				GIVE_UP_RESPONSE* pRes_giveup = (GIVE_UP_RESPONSE*)data;
				chairno = pRes_giveup->nChairNo;
				nRoundCurDeposit = nBet;//本次下注消耗银两
				nUsedDeposit = GetGameInfoEx()->GetChairUsedDeposit(chairno);
				nChairStatus = PLAYER_STATUS_GIVE_UP;
				nTotalDeposit =  GetGameInfoEx()->GetTotalDesposit(); 
				
				nNextChair = pRes_giveup->nNextChair;
				dwCouldOpe = pRes_giveup->dwNextCouldOpe;
			}
			PlaySound("放弃");
		}
		break;
	case GAME_MSG_COMPARE:
		{
			if(NULL != data)
			{
				COMPARE_CARD_RESPONSE* pRes_Comp = (COMPARE_CARD_RESPONSE*)data;
				chairno = pRes_Comp->nChairNo;
				nChairStatus = PLAYER_STATUS_NULL;
				
				nNextChair =pRes_Comp->nNextChair;
				dwCouldOpe= pRes_Comp->dwNextCouldOpe;
				
				if(!pRes_Comp->bNextChair && pRes_Comp->nBet > 0)
				{
					if(m_nLeftCompareNum < 0)
						m_nLeftCompareNum = 0;
				}
				nBet = pRes_Comp->nBet;
				nRoundCurDeposit = nBet * GetGameInfoEx()->GetGameStartData()->nBaseDeposit;
				nUsedDeposit = GetGameInfoEx()->GetChairUsedDeposit(chairno) + nRoundCurDeposit;
				nTotalDeposit =  GetGameInfoEx()->GetTotalDesposit() + nRoundCurDeposit;

				memcpy(&mRes, pRes_Comp, sizeof(COMPARE_CARD_RESPONSE));
			}
		}
		break;
	case GAME_MSG_FOLLOW:
		{
			if (NULL != data)
			{
				FOLLOW_BET_RESPONSE* pRes_follow = (FOLLOW_BET_RESPONSE*)data;
				chairno = pRes_follow->nChairNo;
				nChairStatus = PLAYER_STATUS_FOLLOW;

				nBet = pRes_follow->nBet;
				nRoundCurDeposit = nBet * GetGameInfoEx()->GetGameStartData()->nBaseDeposit;
				nUsedDeposit = GetGameInfoEx()->GetChairUsedDeposit(chairno) + nRoundCurDeposit;
				nTotalDeposit =  GetGameInfoEx()->GetTotalDesposit() + nRoundCurDeposit;
				nNextChair = pRes_follow->nNextChair;
				dwCouldOpe= pRes_follow->dwNextCouldOpe;

				if (FALSE==theApp.IsLookOn() && theApp.GetChairNO()==chairno && IsAutoFollow())
					m_nAutoFollowTime = AUTO_FOLLOW_TIME;

				if (m_pAllInResponse)
					m_bOpeDuringCompareCard = TRUE;
			}
		}	
		break;
	case GAME_MSG_BET_FULL:
		{
			if (NULL != data)
			{
				BET_FULL_RESPONSE* pRes_betFull = (BET_FULL_RESPONSE*)data;
				chairno = pRes_betFull->nChairNo;
				nChairStatus = PLAYER_STATUS_BET_FULL;

				if(!pRes_betFull->bNextChair && pRes_betFull->nDeposit > 0)
				{
					if(m_nLeftCompareNum < 0)
						m_nLeftCompareNum = 0;
				}

				nRoundCurDeposit = pRes_betFull->nDeposit;
				nBet = nRoundCurDeposit / GetGameInfoEx()->GetGameStartData()->nBaseDeposit;
				nUsedDeposit = GetGameInfoEx()->GetChairUsedDeposit(chairno) + nRoundCurDeposit;
				nTotalDeposit = GetGameInfoEx()->GetTotalDesposit() + nRoundCurDeposit;

				nNextChair = pRes_betFull->nNextChair;
				dwCouldOpe = pRes_betFull->dwNextCouldOpe;
			}
		}
		break;
	case GAME_MSG_ALL_IN:
		{
			if (NULL!=data)
			{
				ALL_IN_RESPONSE* pRes_all = (ALL_IN_RESPONSE*)data;
				chairno = pRes_all->nChairNo;
				nChairStatus = PLAYER_STATUS_ALLIN;

				nRoundCurDeposit = pRes_all->nDeposit;
				nBet = nRoundCurDeposit / GetGameInfoEx()->GetGameStartData()->nBaseDeposit;
				nUsedDeposit = GetGameInfoEx()->GetChairUsedDeposit(chairno) + nRoundCurDeposit;
				nTotalDeposit = GetGameInfoEx()->GetTotalDesposit() + nRoundCurDeposit;
				nNextChair = pRes_all->bNextChair;
				dwCouldOpe = pRes_all->dwNextCouldOpe;
			}
		}
		break;
	case GAME_MSG_SHOW_CARD:
		{
			SHOW_CARD_RESPONSE *pShowCard=(SHOW_CARD_RESPONSE*)data;
			if (datalen!=sizeof(SHOW_CARD_RESPONSE))
			{
				theApp.ShowMsgInChatView("SHOW_CARD_RESPONSE 消息长度有误\r\n"); 
				return ;
			} 
			NTF_ZgsaShowCard(pShowCard); 
		}
		return;	
	case GAME_MSG_LOOK_CARD:
		{
			SHOW_CARD_RESPONSE *pLookCard=(SHOW_CARD_RESPONSE*)data;
			if (datalen!=sizeof(SHOW_CARD_RESPONSE))
			{
				theApp.ShowMsgInChatView("SHOW_CARD_RESPONSE 消息长度有误\r\n");
				return ;
			}
					
			NTF_LookCard(pLookCard);
		}
		return;
	default:
		return;
	}

	if(NULL == data)
		return;

	if (GAME_MSG_ALL_IN == nMsgID)
	{	
		ALL_IN_RESPONSE* pAllin = (ALL_IN_RESPONSE*)data;
		m_nComNextPlayer = (chairno + TOTAL_CHAIRS - 1) % TOTAL_CHAIRS;
		while (1)//find the next comparer
		{
			if(chairno == m_nComNextPlayer)
				return;
			if (!GetPlayerInfoEx(m_nComNextPlayer)->bInGame || PLAYER_STATUS_WAIT_START == GetPlayerInfoEx(m_nComNextPlayer)->nPlayerStatus
				|| IsPlayerGiveUp(m_nComNextPlayer) || 0 >= pAllin->bNeedCompare[m_nComNextPlayer])
			{
				m_nComNextPlayer = (m_nComNextPlayer + TOTAL_CHAIRS - 1) % TOTAL_CHAIRS;
			}
			else
			{
				break;
			}
		}
		//变成比牌操作
		mRes.nBet = nBet;
		mRes.nChairNo = chairno;
		mRes.nGoalChairNo = m_nComNextPlayer;
		if(pAllin->bNeedCompare[m_nComNextPlayer])
		{
			if(pAllin->bWinOrLose[m_nComNextPlayer])
				mRes.nLostChairNo = m_nComNextPlayer;
			else
				mRes.nLostChairNo = chairno;
		}
		nMsgID = GAME_MSG_COMPARE;//全下消息至少有一次比牌消息，消息变换

		mRes.bNextChair = FALSE;
		mRes.dwNextCouldOpe = 0;
		mRes.nNextChair = -1;
		if (pAllin->bNextChair)//存在下一玩家
		{	
			if(pAllin->nNextChair != m_nComNextPlayer)
				nNextChair = -1;//第一个被比牌的玩家比输
		} 
		else
		{//比完所有玩家
			int nCompNum = 0;
			for (int k=0;k < TOTAL_CHAIRS;k++)
			{
				if(pAllin->bNeedCompare[k] > 0)
					nCompNum++;
			}
			m_nLeftCompareNum = nCompNum;
			nNextChair = -1;
		}

		m_pAllInResponse = new ALL_IN_RESPONSE;
		memcpy(m_pAllInResponse, pAllin, sizeof(ALL_IN_RESPONSE));
	}

	if(GAME_MSG_BET_FULL == nMsgID)
	{
		BET_FULL_RESPONSE* pBetFull = (BET_FULL_RESPONSE*)data;
		m_nBetFullDeposit = pBetFull->nDeposit;
		if(FALSE == pBetFull->bNextChair)
		{
			mRes.nBet = nBet;
			mRes.nChairNo = chairno;
			mRes.bNextChair = pBetFull->bNextChair;
			mRes.nNextChair = pBetFull->nChairNo;
			mRes.dwNextCouldOpe = pBetFull->dwNextCouldOpe;
			mRes.nGoalChairNo = pBetFull->nGoalCompareChair;
			mRes.dwNextCouldOpe = 0;
			mRes.nNextChair = -1;

			if(pBetFull->bWinOrLose)
				mRes.nLostChairNo = mRes.nGoalChairNo;
			else
				mRes.nLostChairNo = chairno;

			nMsgID = GAME_MSG_COMPARE;
		}
	}

	GetGameInfoEx()->SetChairUsedDeposit(chairno,nUsedDeposit);  
	int nIndex=RUL_GetDrawIndexByChairNO(chairno);
	m_ptrPlayer[nIndex]->SetOpeStatus(nChairStatus);

	if (nMsgID==GAME_MSG_GIVE_UP)
	{
		m_ptrPlayer[nIndex]->ShowBrightOrDarkBoard(FALSE, TRUE);
		if (theApp.GetChairNO() != chairno)
		{
			m_ptrPlayer[RUL_GetDrawIndexByChairNO(chairno)]->HideCompareBtn();
		}
	}
	
	if (nMsgID==GAME_MSG_GIVE_UP && chairno==theApp.GetChairNO())
	{
		if (theApp.IsLookOn())
		{
			m_ptrPlayer[nIndex]->ShowBrightOrDarkBoard(FALSE, TRUE);
		}
	}

	m_ptrPlayer[nIndex]->FreshPlayerBetInfo();  
	OPE_ThrowChips(chairno, nBet);
	GetGameInfoEx()->SetChairRoundDeposit(chairno,GetGameInfoEx()->GetChairRoundDeposit(chairno) + nRoundCurDeposit); 
	if(GAME_MSG_COMPARE != nMsgID)
		GetGameInfoEx()->SetChairStatus(chairno,nChairStatus);
	if(nRoundCurDeposit > 0)
		GetGameInfoEx()->SetRoundCurDeposit(nRoundCurDeposit); 
	GetGameInfoEx()->SetTotalDesposit(nTotalDeposit);
	FreshBetInfo(theApp.GetChairNO());

	if (GAME_MSG_COMPARE == nMsgID)
	{
		PlayCompareCardAni(&mRes);
		m_Clock->Hide();
		m_Clock->Updata();
		m_dlgOperation->HidePlayerOpe(GAME_OPE_GIVE_UP|GAME_OPE_LOOK_CARD|GAME_OPE_AUTO_START|GAME_OPE_AUTO_FOLLOW);
		m_dlgOperation->HideDigitNumber();

		for (int i=0;i<TOTAL_CHAIRS;i++)
			m_ptrPlayer[i]->SetCurrentChair(FALSE);
	
		GetGameInfoEx()->SetChairStatus(mRes.nLostChairNo, PLAYER_STATUS_GIVE_UP);
		
		if(m_nLeftCompareNum > 0)
			m_nLeftCompareNum--;

		m_pCompareCardResponse = new COMPARE_CARD_RESPONSE;
		memcpy(m_pCompareCardResponse, &mRes, sizeof(COMPARE_CARD_RESPONSE));
	} 
    else if(nNextChair >= 0)
	{
		EnableNextChairOpe(chairno, nNextChair, dwCouldOpe);
	}

	if(nMsgID == GAME_MSG_GIVE_UP && chairno==theApp.GetChairNO() && FALSE==theApp.IsLookOn())//玩家自己弃牌
	{
		m_dlgOperation->HidePlayerOpe(GAME_OPE_GIVE_UP|GAME_OPE_LOOK_CARD|GAME_OPE_AUTO_START|GAME_OPE_AUTO_FOLLOW);
		m_dlgOperation->HideDigitNumber();
		HidePlayersCompareBtn();
		ShowLookOn_Seat_ChangeTableBtn(TRUE, FALSE, TRUE);
		FreshTotalDepositInfo();
	}
}

void CMyGame::NTF_ZgsaShowCard(SHOW_CARD_RESPONSE *pShowCard)
{ 
	int chairno=pShowCard->nChairNo;
	
	for (int i = 0; i < m_nTotalPlayer; i++)
	{
		if (FALSE == GetPlayerInfoEx(i)->bInGame)
			continue;

		m_ptrPlayer[RUL_GetDrawIndexByChairNO(chairno)]->HideBrightOrDarkBoard();
	}

// 		CString str;	
// 		str.Format("<c=%d>%s<c=%d> ",RGB(255,255,0),theApp.m_GamePlayer[pShowCard->chairno].szUsername,RGB(255,255,255));
// 		str.Format(str+"<c=%d>亮出手牌   %s %s %s \r\n",RGB(113,203,83), 
// 			GetCardDescStr(pShowCard->nChairCards[0])
// 			,GetCardDescStr(pShowCard->nChairCards[1])
// 			,GetCardDescStr(pShowCard->nChairCards[2]));
	
	OPE_ShowChairCard(chairno,0,pShowCard->nChairCards[0],0);
	OPE_ShowChairCard(chairno,1,pShowCard->nChairCards[1],0);
	OPE_ShowChairCard(chairno,2,pShowCard->nChairCards[2],0);

	int nDrawIndex=RUL_GetDrawIndexByChairNO(chairno);
	m_ptrPlayer[nDrawIndex]->HideBrightOrDarkBoard();
	m_ptrPlayer[nDrawIndex]->HideLostLightningMask();

	//在聊天框显示
	CString str;
	str.Format("<c=0>%s<c=%d> 亮出底牌:",theApp.m_GamePlayer[chairno].szUsername,RGB_BLUE);
	theApp.ShowMsgInChatView(str);
	
	for(i=0;i<CHAIR_CARDS;i++)
	{
		int cardid = pShowCard->nChairCards[i];
		if (cardid/13==0)
		{
			str.Format("<z=tc方块> %s",g_szValue[cardid%13]);
		}
		else if (cardid/13==1)
		{
			str.Format("<z=tc梅花> %s",g_szValue[cardid%13]);
		}
		else if (cardid/13==2)
		{
			str.Format("<z=tc红桃> %s",g_szValue[cardid%13]);
		}
		else
		{
			str.Format("<z=tc黑桃> %s",g_szValue[cardid%13]);
		}
		
		theApp.ShowMsgInChatView(str);
		if (i!=CHAIR_CARDS-1)
			theApp.ShowMsgInChatView("、");
	}
	theApp.ShowMsgInChatView("\r\n");
}

void    CMyGame::OPE_StartTimer()
{
	if (m_hEventTimer)
	{
		StopApply(m_hEventTimer);
	}
	m_hEventTimer=APPLY_CallBackPerTime(1000,-1,&(SNG_MSG)CreateMsg(GetHandle(),GAME_CALLBACKMSG_ONTIMER));
	m_Clock->Show();
	m_Clock->Updata();  
	m_ClockCountDown->Hide();
	m_ClockCountDown->Updata();
}
void    CMyGame::OPE_StartIdleTimer()
{
	if (m_hEventTimer)
	{
		StopApply(m_hEventTimer);
	}
	m_hEventTimer=APPLY_CallBackPerTime(1000,-1,&(SNG_MSG)CreateMsg(GetHandle(),GAME_CALLBACKMSG_ONTIMER));
	m_Clock->Hide();
	m_Clock->Updata();  
	m_ClockCountDown->Show();
	m_ClockCountDown->Updata();
}

void    CMyGame::OPE_EndTimer()
{
	StopApply(m_hEventTimer);
	m_Clock->Hide();
	m_Clock->Updata(); 
	
	m_ClockCountDown->Hide();
    m_ClockCountDown->Updata();
}

void      CMyGame::MOVIE_ShowWinCards(PLAYER_WIN_INFO*pPlayerWinInfo,int chairno)
{	
	int nDrawindex=RUL_GetDrawIndexByChairNO(chairno);
	for (int i=0;i<CHAIR_CARDS;i++)
	{
		int nChairCardID=pPlayerWinInfo->nChairCards[i];
		OPE_ShowChairCard(chairno,i,nChairCardID,1);
	}
}

POINT  CMyGame::GetWaitCountDownPostion()
{
	RECT rc=GetMainGameRect();
	int nWidth = rc.right - rc.left;
	int nHeight = rc.bottom - rc.top;
	POINT pt;
	
	pt.x = nWidth/2;
	pt.y = nHeight/2-50;
	
	return pt;
}

POINT CMyGame::GetWaitCountDownDigitPostion()
{
	RECT rc=GetMainGameRect();
	int nWidth = rc.right - rc.left;
	int nHeight = rc.bottom - rc.top;
	POINT pt;
	
	pt.x = nWidth/2;
	pt.y = nHeight/2+15;

	return pt;
}

void CMyGame::ShowWaitCountDown()
{
	//游戏已经开始
	if (GetGameWinApp()->IsGameRunning())
		return;
	
	m_nZeroCountDown=0;
	BITObject* pbo=NULL;
	if (!m_bitCountDown)
	{
		m_bitCountDown = CreateBitMap(Res_GetPICStruct("text_daojishi"));
		m_bitCountDown->SetFloor(FLOOR_ANIMATION);
		POINT pt=GetWaitCountDownPostion();
		m_bitCountDown->MoveTo(pt.x,pt.y);
		
	} 
	m_bitCountDown->Show();
	m_bitCountDown->Updata();
}

void CMyGame::ShowWaitArrageTable()
{
	//游戏已经开始
	if (GetGameWinApp()->IsGameRunning()||GetGameWinApp()->IsLookOn())
		return;
	
	RECT rect=GetGameRect();
	int x=rect.left;
	int y=rect.top;
	int w=rect.right-rect.left;
	int h=rect.bottom-rect.top;
	
	BITObject* pbo=NULL;
	if (!m_ShowWaitArrage)
	{
		pbo = CreateBitMap(Res_GetPICStruct("waitsolo"));
		pbo->SetFloor(FLOOR_ANIMATION);
		pbo->MoveTo(x+w/2,y+h/2);
		m_ShowWaitArrage = pbo;
	}
	else
	{
		pbo = (BITObject*)m_ShowWaitArrage;
	}
	APPLY_PlayBitObject(pbo);
	pbo->Show();
	pbo->Updata();
}

void CMyGame::HideWaitArrageTable()
{
	if (m_ShowWaitArrage)
	{
		BITObject* pl = (BITObject*)m_ShowWaitArrage;
		StopObject(pl->GetHandle());
		pl->Hide();
		pl->Updata();
	}
	if (m_Clock)
	{
		m_Clock->Hide();
		m_Clock->Updata();
	} 
}

void CMyGame::HideWaitCountDown()
{
	if (m_bitCountDown)
	{ 
		m_bitCountDown->Hide();
		m_bitCountDown->Updata();
	}
	if (m_ClockCountDown)
	{
		m_ClockCountDown->Hide();
		m_ClockCountDown->Updata();
	}
}

void    CMyGame::OPE_ShowResultAni()
{
	OPE_ShowResultAni2();
	return;
}


void    CMyGame::OPE_ShowResultAni2()
{
	GAME_WIN_RESULT *pGameWin= GetGameInfoEx()->GetGameWin();
	PLAYER_WIN_INFO *pPlayerWinInfo=pGameWin->playerWinInfo; 
	
	{
		//清状态
		if (!pGameWin->bGameEndAhead)
		{ 
			for (int i=0;i<TOTAL_CHAIRS;i++)
			{
				if (GetPlayerInfoEx(i)->bInGame)
				{
					int nDrawIndex=RUL_GetDrawIndexByChairNO(i); 
					m_ptrPlayer[nDrawIndex]->SetOpeStatus(PLAYER_STATUS_NULL);
				}
			}
		}
	}
	
	APPLY_WaitFewTimes(3000, &(SNG_MSG)CreateMsg(GetHandle(),MSG_SHOW_RESULT_END,0,0));
}

void   CMyGame::OPE_AfterShowResultAni()
{
	///+
	if (m_dwGameStatus==STATUS_START_COUNTDOWN)
	{
		m_ClockCountDown->Show();
		m_ClockCountDown->Updata();
		ShowWaitCountDown();  
	}
	
	if (!GetGameWinApp()->IsLookOn())
		PGL_SHOW_OBJECT(m_btnStart);

    OPE_UnMaskAllShowCard();

	if (FALSE == theApp.IsLookOn())
	{
		ShowLookOn_Seat_ChangeTableBtn(TRUE, FALSE, TRUE);
	}
	else
	{
		ShowLookOn_Seat_ChangeTableBtn(FALSE, TRUE, TRUE);	
	}
}

CString  CMyGame::GetCardDescStr(int nCardID)
{ 
	//	<z=tc系统通知>
	static TCHAR szCardsShape[4][20]={"<z=方块> ","<z=梅花> ","<z=红桃> ","<z=黑桃> "};
	static TCHAR szCardsValue[LAYOUT_MOD][20]={"<size=14>2","<size=14>3","<size=14>4","<size=14>5","<size=14>6","<size=14>7","<size=14>8","<size=14>9",
		"<size=14>10","<size=14>J","<size=14>Q","<size=14>K","<size=14>A"};
	
	CString str;
	
	int nCardShape=nCardID/LAYOUT_MOD; 
	int nCardValue=nCardID%LAYOUT_MOD;
	str.Format( "<c=%d>%s%s ",RGB_WHITE,szCardsShape[nCardShape],szCardsValue[nCardValue]);
	
	
	return str;
}

void     CMyGame::OPE_ShowResult()
{
    m_dwGameStatus=STATUS_SHOW_RESULT;
	if (m_handleNewRound)
	{
		StopApply(m_handleNewRound);
	} 

	OPE_ShowResultAni();

	{
		for (int i=0;i<TOTAL_CHAIRS;i++)  //玩家状态清空 哪些信息需清空
		{
			if (!GetPlayerInfoEx(i)->bInGame)continue;
			
			GetGameInfoEx()->SetChairRoundDeposit(i,0); //清掉后为了显示正确的池筹码
			int chairStatus=GetGameInfoEx()->GetChairStatus(i);
			int nIndex=RUL_GetDrawIndexByChairNO(i);
			m_ptrPlayer[nIndex]->SetCurrentChair(FALSE);
		}
	}
}

POINT  CMyGame::GetPokerDealPosition()
{
	RECT rc=GetMainGameRect();
	int nWidth = rc.right - rc.left;
	int nHeight = rc.bottom - rc.top;
	POINT pt;

	pt.x=nWidth/2;
	pt.y=nHeight/2;
	
	return pt;
}

POINT  CMyGame::GetBaseDepositBoardPosition()
{
	RECT rc=GetMainGameRect();
	POINT pt;

	pt.x=rc.left+105;
	pt.y=rc.bottom-60;
	
	return pt;
}

POINT  CMyGame::GetPotsNumPosition()
{
	RECT rc=GetMainGameRect();
	int nWidth = rc.right - rc.left;
	int nHeight = rc.bottom - rc.top;
	POINT pt;

	pt.x=nWidth/2;
	pt.y=nHeight/2 - 110;
	
	return pt;
}

POINT  CMyGame::GetComparePosition()
{
	RECT rc=GetMainGameRect();
	int nWidth = rc.right - rc.left;
	int nHeight = rc.bottom - rc.top;
	POINT pt;

	pt.x=nWidth/2 - 125;
	pt.y=nHeight/2 - 20;
	
	return pt;
}

POINT  CMyGame::GetVSPosition()
{
	RECT rc=GetMainGameRect();
	int nWidth = rc.right - rc.left;
	int nHeight = rc.bottom - rc.top;
	POINT pt;

	pt.x=nWidth/2;
	pt.y=nHeight/2-20;
	
	return pt;
}

POINT  CMyGame::GetLightningPosition(BOOL bLeft)
{
	RECT rc=GetMainGameRect();
	int nWidth = rc.right - rc.left;
	int nHeight = rc.bottom - rc.top;
	POINT pt;

	pt.x=nWidth/2;
	pt.y=nHeight/2;

	if (bLeft)
		pt.x -= 80;
	else
		pt.x += 120;
	
	return pt;
}
 
void CMyGame::FreshBetInfo(int nChairNo)
{
	int nPotsTotalBet = GetGameInfoEx()->GetTotalDesposit();
	int nBaseDeposit = GetGameInfoEx()->GetBaseDeposit();
	int nCurrentBetNum = GetGameInfoEx()->GetGameTableInfo()->m_Public.nCurrentDouble;

	if(nCurrentBetNum <= 0)
		nCurrentBetNum = 1;

	if (nChairNo == theApp.GetChairNO() && GetPublicInfoEx()->bnLookCard[nChairNo])
		nCurrentBetNum *= 2;	

	if (nPotsTotalBet == 0 || nBaseDeposit == 0)
	{
		PGL_HIDE_OBJECT(m_betInfoBoard);
	}
	else
	{
		m_digitPotsNum->SetDigit(nPotsTotalBet);
		m_digitPotsNum->MoveTo(m_betInfoBoard->GetLeft()+82+(185-m_digitPotsNum->GetWidth())/2,m_betInfoBoard->GetY() + 15);

		int nSingleRet = nBaseDeposit * nCurrentBetNum;
		m_dlgOperation->SetBetDeposit(nSingleRet);

		char singleRet[4]= {0};
		sprintf(singleRet, "%d两", nSingleRet);

		m_chatSingleBet->ClearChat();
		m_chatSingleBet->InSertString(singleRet);
		m_chatSingleBet->MoveTo(m_betInfoBoard->GetLeft()+82+(92-m_chatSingleBet->GetWidth())/2,m_betInfoBoard->GetY() - 17);
		
		if (TRUE == GetIsOpenBetFull())
		{
			int nTotalDeposit = GetGameInfoEx()->GetChairTotalDeposit(theApp.GetChairNO());
			char canRetMax[256]= {0};
			sprintf(canRetMax, "%d两", nTotalDeposit);
			
			//m_chatBetMaxDepositNum->ClearChat();
			//m_chatBetMaxDepositNum->InSertString(canRetMax);
			m_chatBetMaxDepositNum->MoveTo(m_bitIconMaxDeposit->GetX() + 80, m_bitIconMaxDeposit->GetY() + 3);
			
			PGL_SHOW_OBJECT(m_bitIconMaxDeposit);
		}
		else
		{
			PGL_HIDE_OBJECT(m_bitIconMaxDeposit);
			PGL_HIDE_OBJECT(m_chatBetMaxDepositNum);	
		}
		
		m_betInfoBoard->Show();
		m_betInfoBoard->Updata();
	} 
}

void CMyGame::FreshTotalDepositInfo()
{
	if (TRUE == GetIsOpenBetFull())
	{
		BOOL bShow = FALSE;
		int nChairNO = theApp.GetChairNO();
		int nTotalDeposit = GetGameInfoEx()->GetChairTotalDeposit(nChairNO);
		char canBetMax[256]= {0};

		if(!GetPlayerInfoEx(nChairNO)->bInGame || 0 == nTotalDeposit || IsPlayerGiveUp(nChairNO))
			bShow = FALSE;	
		else
			bShow = TRUE;

		if (FALSE == bShow)
			sprintf(canBetMax, "- -两");
		else
			sprintf(canBetMax, "%d两", nTotalDeposit);

		m_chatBetMaxDepositNum->ClearChat();
		m_chatBetMaxDepositNum->InSertString(canBetMax);
		m_chatBetMaxDepositNum->MoveTo(m_bitIconMaxDeposit->GetX() + 80, m_bitIconMaxDeposit->GetY() + 3);
		
		PGL_SHOW_OBJECT(m_chatBetMaxDepositNum);
		PGL_SHOW_OBJECT(m_bitIconMaxDeposit);
	}
	else
	{
		PGL_HIDE_OBJECT(m_bitIconMaxDeposit);
		PGL_HIDE_OBJECT(m_chatBetMaxDepositNum);	
	}
}

void CMyGame::DrawPlaneChip(PlaneObject*pl,int maxCluster,int nBet)
 {
	 DrawPlaneChip2(pl,maxCluster,nBet);
	 return;
 }
 
 void     CMyGame::DrawPlaneChip2(PlaneObject*pl,int maxCluster,int nBet)
 {
	 //	 nBet=13271;
	 pl->Erasure();
	 if (nBet==0)
	 {
		 return;
	 }
	 CArray<CHIP_POS,CHIP_POS>chips;
	 int nChipsCount=0;
	 int bHaveKind[CHIPS_COUNT];
	 ZeroMemory(bHaveKind,sizeof(bHaveKind));
	 int nKindsCount=0;
	 int nTotalBet=nBet;
	 int nHeight=90;
	 int nWidth=80;
	 int nDrawChipX=0;
	 //int nChipWidth= Res_GetPICStruct(g_chipsDesc[0].szResName).width;
	 int nChipWidth= Res_GetPICStruct("icon_chip").width;
	 //得到此池的筹码分配 
	 for (int k=CHIPS_COUNT-1;k>=0;k--)
	 {
		 if (nTotalBet>=g_chipsDesc[k].nUnit)
		 {
			 if (!bHaveKind[k]) 
				 nKindsCount++;  
			 bHaveKind[k]++;
			 
			 nChipsCount++;
			 nTotalBet-=g_chipsDesc[k].nUnit;
			 k++;
			 if (nTotalBet==0) 
				 break; 
		 }
	 } 
	 
	 
	 nTotalBet=nBet;
	 if (nKindsCount<=maxCluster)
	 {
		 int _w=nKindsCount*33;
		 if (_w<nWidth)
		 {
			 _w=nWidth;
		 }
		 nDrawChipX=(_w-(nKindsCount-1)*3-nKindsCount*nChipWidth)/2;
		 pl->ScalePlane(_w,nHeight);
		 pl->ScaleTo(_w,nHeight);
		 int nClusterPos=0;
		 int nClusterBetIndex=0;
		 for (int k=CHIPS_COUNT-1;k>=0;k--)
		 {
			 if (nTotalBet>=g_chipsDesc[k].nUnit)
			 {
				 
				 nChipsCount++;
				 nTotalBet-=g_chipsDesc[k].nUnit;
				 CHIP_POS chipPos;
				 chipPos.nChipDescIndex=k;
				 chipPos.nClusterPos=nClusterPos;
				 chipPos.nBetIndex=nClusterBetIndex;
				 chips.Add(chipPos);
				 if (nTotalBet==0) 
					 break; 
				 if (nTotalBet>=g_chipsDesc[k].nUnit)
				 {
					 k++;
					 nClusterBetIndex++;
				 }
				 else
				 {
					 nClusterPos++;
					 nClusterBetIndex=0;
				 } 
			 }
		 }
	 }
	 else
	 {
		 int nClusterBetCount=nChipsCount/maxCluster+1;
		 int nClusterPos=0;
		 int nClusterBetIndex=0;
		 
		 
		 for (int k=CHIPS_COUNT-1;k>=0;k--)
		 {
			 if (nTotalBet>=g_chipsDesc[k].nUnit)
			 {
				 
				 nChipsCount++;
				 nTotalBet-=g_chipsDesc[k].nUnit;
				 CHIP_POS chipPos;
				 chipPos.nChipDescIndex=k;
				 chipPos.nClusterPos=nClusterPos;
				 chipPos.nBetIndex=nClusterBetIndex;
				 chips.Add(chipPos);
				 
				 if (nTotalBet==0) 
					 break; 
				 if (nTotalBet>=g_chipsDesc[k].nUnit)
				 { 
					 nClusterBetIndex++;
				 }
				 else
				 {
					 nClusterBetIndex++;
					 if (nClusterBetIndex>=nClusterBetCount)
					 {
						 nClusterPos++;
						 nClusterBetIndex=0;
					 } 
				 } 
				 
				 k++;
			 }
		 }
		 int _w=(nClusterPos+1)*33;
		 if (_w<nWidth)
		 {
			 _w=nWidth;
		 }
		 nDrawChipX=(_w-(nClusterPos)*3-(nClusterPos+1)*nChipWidth)/2;
		 pl->ScalePlane(_w,nHeight);
		 pl->ScaleTo(_w, nHeight);
		 
	 }
	 
	 //居中画筹码
	 for (int x=0;x<chips.GetSize();x++)
	 {
		 CHIP_POS chip=chips.GetAt(x);
		 int nCluster=chip.nClusterPos;
		 int nBetIndex=chip.nBetIndex;
		 int nChipsDescIndex=chip.nChipDescIndex;
		 //	 SNG_PIC pic=Res_GetPICStruct(g_chipsDesc[nChipsDescIndex].szResName);
		 SNG_PIC pic=Res_GetPICStruct("icon_chip",nChipsDescIndex);
		 int x,y;
		 x=nCluster*(pic.width+3)+nDrawChipX;
		 y=pl->GetHeight()-pic.height-3*nBetIndex-25;
		 if (nBetIndex>=10)
		 {
			 
		 }
		 else
		 {
			 pl->DrawPic(pic,x,y,pic.width,pic.height); 
		 }
	 } 
	 int nFontSize=14; 
	 int nX=pl->GetWidth()/2;
	 int nY=pl->GetHeight()-20;
	 pl->SetTextSize(nFontSize,400);
	 pl->SetTextColor(RGB_WHITE);
	 CString szTotalBet;
	 szTotalBet.Format("%d",nBet);
	 
	 m_ptrPlayer[MYDRAWINDEX]->m_digitRestBet->WriteString(pl,nX,nY,szTotalBet,DIGIT_FLAGS_MID_JUSITIFY);
	 pl->Updata();
 }
 
 
 void  CMyGame::OPE_HideChairCard(int chairno)
 {
	 for(int i=0;i<CHAIR_CARDS*TOTAL_CHAIRS;i++)
	 {
		 if (m_Card[i]->m_nChairNO==chairno)
		 {
			 m_Card[i]->Hide();
			 m_Card[i]->Updata();
		 }
	 }
 }
 
 void   CMyGame::OPE_ShowChairCard(int chairno,int nUnitIndex,int nCardID,int nCardStatus)
 {
	 BOOL bFace=TRUE;
#ifdef  _OFFLINE_
	 nCardID=rand()%TOTAL_CARDS;
	 if (chairno==theApp.GetChairNO())
		 bFace=FALSE;
	 
#endif
	 for (int i=0;i<TOTAL_CHAIRS*CHAIR_CARDS;i++)
	 { 
		 if (m_Card[i]->m_nChairNO==chairno && m_Card[i]->m_nUnitIndex==nUnitIndex)
		 { 
			 m_Card[i]->m_nCardID=nCardID;
			 
			 m_Card[i]->m_nStatus=nCardStatus;
			 m_Card[i]->m_bnFace=bFace;  
			 int nIndex=RUL_GetDrawIndexByChairNO(chairno);
			 POINT pt=m_ptrPlayer[nIndex]->GetCardPositon(bFace,nUnitIndex,nCardStatus);
			 m_Card[i]->MoveTo(pt.x,pt.y);
			 m_Card[i]->Fresh(); 
			 break;
		 }
	 }
 }
 
 void     CMyGame::OPE_MaskAllShowCard()
 {
	 //玩家离开牌不见了
	 
	 for (int i=0;i<TOTAL_CHAIRS*CHAIR_CARDS;i++)
	 {  
		 if (m_Card[i]->m_nChairNO!=-1 &&m_Card[i]->m_bnFace && m_Card[i]->IsShow())
		 { 
			 CMyCard*card=m_Card[i]; 
			 m_Card[i]->m_nStatus=2; 
			 int nIndex=RUL_GetDrawIndexByChairNO(card->m_nChairNO);
			 POINT pt=m_ptrPlayer[nIndex]->GetCardPositon(card->m_bnFace,card->m_nUnitIndex,card->m_nStatus);
			 m_Card[i]->MoveTo(pt.x,pt.y);
			 m_Card[i]->Fresh(); 
			 // break;
		 }
	 }
 }
 
 void   CMyGame::OPE_UnMaskAllShowCard()
 {
	 
	 //玩家离开牌不见了
	 
	 for (int i=0;i<TOTAL_CHAIRS*CHAIR_CARDS;i++)
	 {  
		 if (m_Card[i]->m_nChairNO!=-1 &&m_Card[i]->m_bnFace&& m_Card[i]->IsShow())
		 { 
			 CMyCard*card=m_Card[i]; 
			 m_Card[i]->m_nStatus=0; 
			 int nIndex=RUL_GetDrawIndexByChairNO(card->m_nChairNO);
			 POINT pt=m_ptrPlayer[nIndex]->GetCardPositon(card->m_bnFace,card->m_nUnitIndex,card->m_nStatus);
			 m_Card[i]->MoveTo(pt.x,pt.y);
			 m_Card[i]->Fresh(); 
			 //	 break;
		 }
	 }
 }
 
 struct APPLYINFO_TURN
 {
	 int         speed;
	 int			nStep;
	 int          nBit1Width;
	 BITObject    *bit1;
 };
 BOOL FP_TURN(PObject* ob, BYTE* data, GameCore* m_core, DWORD time)
 {  
	 
	 APPLYINFO_TURN* applyinfo=(APPLYINFO_TURN*)data;
	 
	 int speed=applyinfo->speed;
	 int& currentStep=applyinfo->nStep;
	 
	 BITObject *bit=(BITObject*)ob;
	 BITObject *bit1=applyinfo->bit1;
	 if (currentStep==0)
	 {
		 int w=	ob->GetWidth();
		 
		 if (ob->GetWidth()>1)
		 {
			 if (ob->GetWidth()-speed<1)
				 ob->ScaleTo(1,ob->GetHeight());
			 else
				 ob->ScaleTo(w-speed,ob->GetHeight());
			 
			 ob->Updata();
		 }
		 else if (ob->GetWidth()==1)
		 {
			 
			 
			 //	memcpy(&pic,&info.common[2],sizeof(PIC));
			 ob->Hide(); 
			 ob->Updata();
			 
			 currentStep=1;
			 bit1->Show();
			 bit1->Updata();
		 }
	 }
	 else
	 {
		 int nBit1Width=applyinfo->nBit1Width;
		 int w=bit1->GetWidth();
		 
		 if (bit1->GetWidth()< nBit1Width)
		 {
			 if (bit1->GetWidth()+speed>nBit1Width)
				 bit1->ScaleTo(nBit1Width,bit1->GetHeight());
			 else
				 bit1->ScaleTo(bit1->GetWidth()+speed,bit1->GetHeight());
			 
			 bit1->Updata();
			 
		 }
		 else
		 {
			 return TRUE;
		 }
	 }
	 
	 return FALSE;
 }
 
 EVENTHANDLE     CMyGame::APPLY_Turn(PObject* ob,
	 int speed,  
	 BITObject * bit1,
	 SNG_MSG* CallBackMsg
	 )
 {
	 APPLY* apply=GetObjectModule()->CreateApply();
	 if (!apply) return 0;
	 apply->ObjectPtr=ob;
	 if (CallBackMsg)
		 memcpy(&apply->CallBackMsg,CallBackMsg,sizeof(SNG_MSG));
	 
	 apply->fp_applyfuction=FP_TURN;
	 apply->dwSpaceCount=GetMinApplySpace();
	 APPLYINFO_TURN temp;
	 ZeroMemory(&temp,sizeof(temp));
	 temp.bit1=bit1;
	 temp.speed=speed;
	 temp.nStep=0;
	 temp.nBit1Width=bit1->GetWidth();
	 bit1->ScaleTo(1,bit1->GetHeight()); 
	 bit1->Hide();
	 bit1->Updata();
	 
	 memcpy(apply->param,&temp,sizeof(APPLYINFO_TURN));   
	 GetCore()->PostQuest(OBJECT_MODULE,SNG_QUEST_CRAETE_APPLY,apply,NULL);
	 return apply->EventHandle;
 }
 
void CMyGame::NTF_LookCard(SHOW_CARD_RESPONSE* pLook_card)
{
	int chairno=pLook_card->nChairNo;
	GetGameInfoEx()->GetGameTableInfo()->m_Public.bnLookCard[pLook_card->nChairNo]=TRUE;
	
	int nDrawIndex = RUL_GetDrawIndexByChairNO(chairno);
 	if (chairno!=GetGameWinApp()->GetChairNO()
 		|| GetGameWinApp()->IsLookOn())
 	{
		m_ptrPlayer[nDrawIndex]->ShowBrightOrDarkBoard();
 	}
 	else
 	{
		OPE_ShowChairCard(chairno,0,pLook_card->nChairCards[0],0);
		OPE_ShowChairCard(chairno,1,pLook_card->nChairCards[1],0);
		OPE_ShowChairCard(chairno,2,pLook_card->nChairCards[2],0);

		m_dlgOperation->m_btnLookCard->Hide();
		m_dlgOperation->m_btnLookCard->Updata();

		if (FALSE==theApp.IsLookOn() && pLook_card->nChairNo==theApp.GetChairNO() && GetGameInfoEx()->GetCurrentChairNO()==theApp.GetChairNO())
			m_dlgOperation->ShowPlayerCouldOpe(pLook_card->dwNextCouldOpe); 

		FreshBetInfo(chairno);
 	}
}

void CMyGame::NTF_ChangeSeatedToLookOn(LOOK_ON_AND_SEATED_CHANGE_RESULT* ploascr)
{
	if (NULL == ploascr)
		return;

	if (TRUE == ploascr->bSucceed)
	{
		int nChangeUserID = ploascr->nUserID;
		if (theApp.GetUserID() == nChangeUserID)
		{//UserID相同才是切换状态的玩家
			PGL_HIDE_OBJECT(m_btnStart);
			
			m_bSitOrStand = FALSE;
			SetAutoStart(TRUE);
			SetAutoFollow(FALSE);
			ShowLookOn_Seat_ChangeTableBtn(FALSE, TRUE, TRUE);
			
			theApp.m_GameInit.bLookOn = TRUE;
			theApp.m_GameInit.nChairNO = ploascr->nChairNO;
		}
		
		int nChairNo = ploascr->nChairNO;
		GAMEPLAYER* pPlayer = &theApp.m_GamePlayer[nChairNo];
		
		if (NULL == pPlayer)
		{
			UwlLogFile(_T("OPE_StandBy failed. pPlayer is NULL."));
			return;
		}
		
		pPlayer->nStatus = ROOM_PLAYER_STATUS_LOOKON;

		//加入旁观队列
		for(int i=0; i<TOTAL_VISITOR_COUNT; i++)
		{
			if (0 == theApp.m_Visitor[i].nUserID)
			{
				pPlayer->bLookOn = TRUE;
				memcpy(&theApp.m_Visitor[i], pPlayer, sizeof(GAMEPLAYER));
				break;
			}
		}
		
		//从玩家删除
		memset(&theApp.m_GamePlayer[nChairNo], 0, sizeof(GAMEPLAYER));
		m_ptrPlayer[RUL_GetDrawIndexByChairNO(nChairNo)]->UnLoadPlayer();
		GetGameInfoEx()->m_GameEnterInfo.ei.dwUserStatus[nChairNo] &= ~US_GAME_STARTED;
		
		for(i=0;i<TOTAL_CHAIRS*CHAIR_CARDS;i++)
		{
			if (m_Card[i]->m_nChairNO == ploascr->nChairNO)
			{
				m_Card[i]->m_nStatus=0;
				m_Card[i]->Hide();
				m_Card[i]->Updata();
			}
		}

		((CMyGameFrame*)theApp.GetGameFrame())->NTF_LookOnAndSeatedChange();
	}
	else
	{
		theApp.ShowMsgInChatView(_T(" <z=tc系统通知> <c=255>操作失败，无法旁观，请稍后再试。\r\n"));
	}
}

void CMyGame::NTF_ChangeLookOnToSeated(LOOK_ON_AND_SEATED_CHANGE_RESULT* ploascr)
{
	if (NULL == ploascr)
		return;
	
	if (TRUE == ploascr->bSucceed)
	{
		int nChangeUserID = ploascr->nUserID;
		if (theApp.GetUserID() == nChangeUserID)
		{//UserID相同才是切换状态的玩家
			PGL_SHOW_OBJECT(m_btnStart);
			
			m_bSitOrStand = TRUE;
			SetAutoFollow(FALSE);
			SetAutoStart(TRUE);
			ShowLookOn_Seat_ChangeTableBtn(TRUE, FALSE, TRUE);
			
			theApp.m_GameInit.bLookOn = FALSE;
			theApp.m_GameInit.nChairNO = ploascr->nChairNO;

			if (FALSE==GetGameInfoEx()->IsGamePlaying() && IsAutoStart())
			{
				UpdateAllInPlayersStatus(m_pAllInResponse);
				OnStart();
				m_dwGameStatus = STATUS_WAITING_START;
			}
			else if (TRUE == GetGameInfoEx()->IsGamePlaying())
			{
				if (m_bCardDealing)
				{//发牌
					GetGameInfoEx()->SetOpeRestTime(GetGameInfoEx()->GetBetWait());
					m_bCardDealing = FALSE;//停止发牌，同时调用OnGetNewPosition
				}
				else if (NULL!=m_pCompareCardResponse && NULL==m_pAllInResponse)
				{//比牌
					UpdateAllCards();
					GetGameInfoEx()->SetChairStatus(m_pCompareCardResponse->nNextChair, PLAYER_STATUS_NULL);
					if (FALSE==m_Clock->IsShow())
						GUI_MoveClock(m_pCompareCardResponse->nNextChair, GetGameInfoEx()->GetBetWait());
					
					StopCompareCardAni();
				}
				else if (NULL != m_pAllInResponse)
				{//全下比牌
					UpdateAllCards();
					UpdateAllInPlayersStatus(m_pAllInResponse);
					GetGameInfoEx()->SetChairStatus(m_pCompareCardResponse->nNextChair, PLAYER_STATUS_NULL);
					if (FALSE==m_Clock->IsShow() && FALSE==m_bOpeDuringCompareCard)
						GUI_MoveClock(m_pAllInResponse->nNextChair, GetGameInfoEx()->GetBetWait()); 

					StopCompareCardAni();
				}
				else
				{
					UpdateAllCards();
					GUI_MoveClock(GetGameInfoEx()->GetCurrentChairNO(),m_Clock->GetDigit()); 
				}
				
				PGL_HIDE_OBJECT(m_btnStart);
			}
		}

		GAMEPLAYER* pPlayer = theApp.GetVisitorPlayerByUserID(nChangeUserID);
		
		if (NULL == pPlayer)
		{
			UwlLogFile(_T("NTF_ChangeLookOnToSeated failed. pPlayer is NULL."));
			return;
		}
		
		pPlayer->nStatus = ROOM_PLAYER_STATUS_WAITING;

		//加入玩家队列
		pPlayer->bLookOn = FALSE;
		pPlayer->nChairNO = ploascr->nChairNO;
		memcpy(&theApp.m_GamePlayer[pPlayer->nChairNO], pPlayer, sizeof(GAMEPLAYER));

		{
			int nDrawIndex = -1;
			for(int i=0; i<TOTAL_CHAIRS; i++)
			{
				nDrawIndex = RUL_GetDrawIndexByChairNO(i);
				if (theApp.GetUserID()==ploascr->nUserID || theApp.m_GamePlayer[i].nUserID==ploascr->nUserID)
				{
					m_ptrPlayer[nDrawIndex]->UnLoadPlayer();
					if (theApp.IsPlayerEnter(i))
					{
						if (i == ploascr->nChairNO)
						{
							GetGameInfoEx()->SetChairTotalDeposit(i, theApp.m_GamePlayer[i].nDeposit);
							GetGameInfoEx()->SetChairPlayerDepoist(i, theApp.m_GamePlayer[i].nDeposit);
							GetGameInfoEx()->SetChairUsedDeposit(i, 0);
						}
						m_ptrPlayer[nDrawIndex]->LoadPlayer(i);
					}
					
					if (theApp.m_GamePlayer[i].nStatus != ROOM_PLAYER_STATUS_PLAYING)
					{
						if (TRUE == GetGameInfoEx()->IsGamePlaying())
							m_ptrPlayer[nDrawIndex]->SetOpeStatus(PLAYER_STATUS_WAIT_START);
						else
							m_ptrPlayer[nDrawIndex]->SetOpeStatus(PLAYER_STATUS_NULL);
						
						continue;
					}
					
					if (FALSE==IsPlayerGiveUp(i) && i!=theApp.GetChairNO())
						m_ptrPlayer[nDrawIndex]->ShowBrightOrDarkBoard(GetPublicInfoEx()->bnLookCard[i]);
					else if (i!=theApp.GetChairNO())
						m_ptrPlayer[nDrawIndex]->ShowBrightOrDarkBoard(GetPublicInfoEx()->bnLookCard[i], TRUE);
					
					if (i == GetGameInfoEx()->GetBanker())
						m_ptrPlayer[nDrawIndex]->ShowBanker(TRUE);
	
					m_ptrPlayer[nDrawIndex]->SetOpeStatus(GetPlayerInfoEx(i)->nPlayerStatus);
				}

				if (FALSE == GetGameInfoEx()->IsGamePlaying() && CheckRoomPlayerStatus(i))
				{
					GetGameInfoEx()->SetChairTotalDeposit(i, theApp.m_GamePlayer[i].nDeposit);
					GetGameInfoEx()->SetChairPlayerDepoist(i, theApp.m_GamePlayer[i].nDeposit);
					GetGameInfoEx()->SetChairUsedDeposit(i, 0);
					m_ptrPlayer[nDrawIndex]->FreshPlayerBetInfo();
				}
			}
		}
		
		//从旁观删除
		memset(pPlayer, 0, sizeof(GAMEPLAYER));

		((CMyGameFrame*)theApp.GetGameFrame())->NTF_LookOnAndSeatedChange();
	}
	else
	{
		if (INVALID_CHAIR_NO_EX == ploascr->nChairNO)
			theApp.ShowMsgInChatView(_T(" <z=tc系统通知> <c=255>没有空余的座位了，请稍后再试。\r\n"));
		else
			theApp.ShowMsgInChatView(_T(" <z=tc系统通知> <c=255>操作失败，无法坐下，请稍后再试。\r\n"));
	}
}

void CMyGame::PlayCompareCardAni(COMPARE_CARD_RESPONSE* pCompare)
{
	if (NULL == pCompare)
		return;

	int nIndexValue[TOTAL_CHAIRS]={3,4,5,2,1,0};
	int fighter_drawIndex=RUL_GetDrawIndexByChairNO(pCompare->nChairNo);
	int dest_drawIndex=RUL_GetDrawIndexByChairNO(pCompare->nGoalChairNo);
	CMyCard* pCardIDs_1[CHAIR_CARDS] = {NULL, NULL, NULL};
	CMyCard* pCardIDs_2[CHAIR_CARDS] = {NULL, NULL, NULL};

	//计算并获取比牌玩家手牌及其左右位置
	if (nIndexValue[fighter_drawIndex] > nIndexValue[dest_drawIndex])
	{
		if (pCompare->nLostChairNo == pCompare->nChairNo)
			m_bLightningLeft = TRUE;
		else
			m_bLightningLeft = FALSE;

		for(int i=0; i<TOTAL_CHAIRS*CHAIR_CARDS; i++)
		{
			if (m_Card[i]->m_nChairNO != pCompare->nChairNo
				&& m_Card[i]->m_nChairNO != pCompare->nGoalChairNo)
				continue;
			else if (m_Card[i]->m_nChairNO == pCompare->nChairNo)
				pCardIDs_1[m_Card[i]->m_nUnitIndex] = m_Card[i];
			else if (m_Card[i]->m_nChairNO == pCompare->nGoalChairNo)
				pCardIDs_2[m_Card[i]->m_nUnitIndex] = m_Card[i];
		}
	}
	else if (nIndexValue[fighter_drawIndex] < nIndexValue[dest_drawIndex])
	{
		if (pCompare->nLostChairNo == pCompare->nChairNo)
			m_bLightningLeft = FALSE;
		else
			m_bLightningLeft = TRUE;

		for(int i=0; i<TOTAL_CHAIRS*CHAIR_CARDS; i++)
		{
			if (m_Card[i]->m_nChairNO != pCompare->nChairNo
				&& m_Card[i]->m_nChairNO != pCompare->nGoalChairNo)
				continue;
			else if (m_Card[i]->m_nChairNO == pCompare->nGoalChairNo)
				pCardIDs_1[m_Card[i]->m_nUnitIndex] = m_Card[i];
			else if (m_Card[i]->m_nChairNO == pCompare->nChairNo)
				pCardIDs_2[m_Card[i]->m_nUnitIndex] = m_Card[i];
		}
	}
	else
	{
		UwlLogFile(_T("比牌错误：DrawIndex相同,line=%d"), __LINE__);
		return;
	}

	if (NULL == pCardIDs_1 || NULL == pCardIDs_2)
	{
		UwlLogFile(_T("比牌错误：比牌玩家的手牌无法获取,line=%d"), __LINE__);
		return;
	}

	m_vecCompareAni.push_back(BeginGroup());
	//移动玩家手牌到比牌位置
	{
		for (int i = 0; i < CHAIR_CARDS; i++)
		{
			POINT ptBase = GetComparePosition();
			POINT pt = ptBase;
			pt.x += pCardIDs_1[i]->m_nUnitIndex*25;
			APPLY_MoveObject(pCardIDs_1[i], pt, COMPARE_MOVE_SPEED);
			
			pt = ptBase;
			pt.x += 200+pCardIDs_2[i]->m_nUnitIndex*25;
			APPLY_MoveObject(pCardIDs_2[i], pt, COMPARE_MOVE_SPEED);
		}
		
		//移动比牌玩家的明暗牌资源
		POINT ptBase = GetComparePosition();
		ptBase.x += CARD_WIDTH_SMALL/2;
		if (MYDRAWINDEX != pCardIDs_1[0]->m_nDrawIndex || theApp.IsLookOn())
		{
			APPLY_MoveObject(m_ptrPlayer[pCardIDs_1[0]->m_nDrawIndex]->m_bitBrightOrDark, ptBase, COMPARE_MOVE_SPEED);
			APPLY_MoveObject(m_ptrPlayer[pCardIDs_1[0]->m_nDrawIndex]->m_bitLost, ptBase, COMPARE_MOVE_SPEED);
		}
		else
		{
			m_ptrPlayer[pCardIDs_1[0]->m_nDrawIndex]->m_bitLost->MoveTo(ptBase.x, ptBase.y);
		}
		
		ptBase.x += 200;
		if (MYDRAWINDEX != pCardIDs_2[0]->m_nDrawIndex || theApp.IsLookOn())
		{
			APPLY_MoveObject(m_ptrPlayer[pCardIDs_2[0]->m_nDrawIndex]->m_bitBrightOrDark, ptBase, COMPARE_MOVE_SPEED);
			APPLY_MoveObject(m_ptrPlayer[pCardIDs_2[0]->m_nDrawIndex]->m_bitLost, ptBase, COMPARE_MOVE_SPEED);
		}
		else
		{
			m_ptrPlayer[pCardIDs_2[0]->m_nDrawIndex]->m_bitLost->MoveTo(ptBase.x, ptBase.y);
		}
		

		m_bitVS->GoToFrame(0);
		m_bitLightning->GoToFrame(0);
		ptBase = GetLightningPosition(m_bLightningLeft);
		m_bitLightning->MoveTo(ptBase.x, ptBase.y);
	}
	m_vecCompareAni.push_back(NextGroup());
	//播放vs动画
	APPLY_DirectShow(m_bitVS, TRUE);
	APPLY_PlayBitObject(m_bitVS,MODE_PLAYBIT_BY_CIRCLE,1);
	APPLY_WaitFewTimes(1200);//1500
	m_vecCompareAni.push_back(NextGroup());
	//播放闪电动画
	PlaySound("比牌");
	APPLY_DirectShow(m_bitLightning, TRUE);
	APPLY_PlayBitObject(m_bitLightning,MODE_PLAYBIT_BY_CIRCLE,1);
	APPLY_WaitFewTimes(400);
	m_vecCompareAni.push_back(NextGroup());
	//隐藏vs和闪电
	APPLY_DirectShow(m_bitVS, FALSE);
	APPLY_DirectShow(m_bitLightning, FALSE);
	m_vecCompareAni.push_back(NextGroup());
	//播放失败玩家被劈动画
	APPLY_DirectShow(m_ptrPlayer[RUL_GetDrawIndexByChairNO(pCompare->nLostChairNo)]->m_bitLost, TRUE);
	APPLY_PlayBitObject(m_ptrPlayer[RUL_GetDrawIndexByChairNO(pCompare->nLostChairNo)]->m_bitLost,MODE_PLAYBIT_BY_CIRCLE,1);
	m_ptrPlayer[RUL_GetDrawIndexByChairNO(pCompare->nLostChairNo)]->m_bLost = TRUE;
	APPLY_WaitFewTimes(700);//1200
	m_vecCompareAni.push_back(NextGroup());
	//将玩家手牌移回原来位置
	{
		for (int i = 0; i < CHAIR_CARDS; i++)
		{
			POINT pt = m_ptrPlayer[pCardIDs_1[i]->m_nDrawIndex]->GetCardPositon(FALSE, i, 0);
			APPLY_MoveObject(pCardIDs_1[i], pt, COMPARE_MOVE_SPEED);
			
			pt = m_ptrPlayer[pCardIDs_2[i]->m_nDrawIndex]->GetCardPositon(FALSE, i, 0);
			APPLY_MoveObject(pCardIDs_2[i], pt, COMPARE_MOVE_SPEED);
		}
		
		POINT pt = m_ptrPlayer[pCardIDs_1[0]->m_nDrawIndex]->GetCardPositon(FALSE, 0, 0);
		pt.x += CARD_WIDTH_SMALL/2;
		if (MYDRAWINDEX != pCardIDs_1[0]->m_nDrawIndex || theApp.IsLookOn())
		{
			APPLY_MoveObject(m_ptrPlayer[pCardIDs_1[0]->m_nDrawIndex]->m_bitBrightOrDark, pt, COMPARE_MOVE_SPEED);
			APPLY_MoveObject(m_ptrPlayer[pCardIDs_1[0]->m_nDrawIndex]->m_bitLost, pt, COMPARE_MOVE_SPEED);
		}
		else if (MYDRAWINDEX == pCardIDs_1[0]->m_nDrawIndex && pCardIDs_1[0]->m_nChairNO == pCompare->nLostChairNo)
		{
			APPLY_MoveObject(m_ptrPlayer[pCardIDs_1[0]->m_nDrawIndex]->m_bitLost, pt, COMPARE_MOVE_SPEED);
		}
 		pt = m_ptrPlayer[pCardIDs_2[0]->m_nDrawIndex]->GetCardPositon(FALSE, 0, 0);
 		pt.x += CARD_WIDTH_SMALL/2;
		if (MYDRAWINDEX != pCardIDs_2[0]->m_nDrawIndex || theApp.IsLookOn())
		{
			APPLY_MoveObject(m_ptrPlayer[pCardIDs_2[0]->m_nDrawIndex]->m_bitBrightOrDark, pt, COMPARE_MOVE_SPEED);
			APPLY_MoveObject(m_ptrPlayer[pCardIDs_2[0]->m_nDrawIndex]->m_bitLost, pt, COMPARE_MOVE_SPEED);
		}
		else if (MYDRAWINDEX == pCardIDs_2[0]->m_nDrawIndex && pCardIDs_2[0]->m_nChairNO == pCompare->nLostChairNo)
		{
			APPLY_MoveObject(m_ptrPlayer[pCardIDs_2[0]->m_nDrawIndex]->m_bitLost, pt, COMPARE_MOVE_SPEED);
		}
	}
	m_vecCompareAni.push_back(NextGroup());
	APPLY_WaitFewTimes(1,&(SNG_MSG)CreateMsg(GetHandle(),MSG_COMPARE_RESULT));
	EndGroup();
}

void CMyGame::StopCompareCardAni()
{
	std::vector<ShinyNova::GROUP*>::iterator itor;
	for (itor=m_vecCompareAni.begin(); itor!=m_vecCompareAni.end(); itor++)
	{
		ShinyNova::GROUP* pGroup = *itor;
		if (NULL != pGroup)
			StopGroup(pGroup);
	}
	m_vecCompareAni.clear();

	m_nComNextPlayer = -1;
	m_nLeftCompareNum = -1;
	m_bGameWin = FALSE;
	m_bOpeDuringCompareCard = FALSE;

	APPLY_DirectShow(m_bitVS, FALSE);
	APPLY_DirectShow(m_bitLightning, FALSE);
	SAFE_DELETE(m_pAllInResponse);
	SAFE_DELETE(m_pCompareCardResponse);
}

void CMyGame::HidePlayersCompareBtn()
{
	for (int i = 0; i < TOTAL_CHAIRS; i++)
	{
		if (NULL != m_ptrPlayer[i])
			m_ptrPlayer[i]->HideCompareBtn();
	}
}
//动画扔的注数
void  CMyGame::OPE_ThrowChips(int chairno,int bet)
{
#ifdef _OFFLINE_
	if (m_nBetCount >= 50)
	{
		OPE_TakeBet();
		return;
	}
#endif
	if (bet == 0)
	{
		return;
	}
	else if (bet < 0)
	{
		UwlLogFile(_T("筹码注数错误:%ld,line=%d"), bet, __LINE__);
		return;
	}

	int throw_bet4 = 0;
	int throw_bet2 = 0;
	int throw_bet1 = 0;
	
	//根据要扔的注数，计算各种筹码数量
	FitBet(bet,throw_bet4,throw_bet2,throw_bet1);
	PlaySound("snd_ThrowBet");
	for(int i=0;i<throw_bet4;i++)
		OPE_ThrowOneChips(chairno,4);
	
	for(i=0;i<throw_bet2;i++)
		OPE_ThrowOneChips(chairno,2);
	
	for(i=0;i<throw_bet1;i++)
		OPE_ThrowOneChips(chairno,1);
}

void  CMyGame::OPE_ThrowOneChips(int chairno,int times,BOOL bnFast)
{
	if (m_nBetCount >= MAX_BET_ALL_COUNT)
	{
#ifdef _OFFLINE_
		OPE_TakeBet();
#endif
		return;
	}
	
	m_bitThrownBet[m_nBetCount] = CreateBitMap(Res_GetPICStruct("coin10_1"));
	m_bitThrownBet[m_nBetCount]->SetFloor(CARD_FLOOR_MASK);
	m_bitThrownBet[m_nBetCount]->SetAlpha(1);

	int index=rand()%8;
	if (times == 4)
		m_bitThrownBet[m_nBetCount]->LoadPic(Res_GetPICStruct("coin40_1",index));
	else if (times == 2)
		m_bitThrownBet[m_nBetCount]->LoadPic(Res_GetPICStruct("coin20_1",index));
	else
		m_bitThrownBet[m_nBetCount]->LoadPic(Res_GetPICStruct("coin10_1",index));

 	POINT pt_start=m_ptrPlayer[RUL_GetDrawIndexByChairNO(chairno)]->GetReadyPosition();
 	m_bitThrownBet[m_nBetCount]->MoveTo(pt_start.x,pt_start.y);
 	m_bitThrownBet[m_nBetCount]->Show();
 	m_bitThrownBet[m_nBetCount]->Updata();
 	
	RECT rc = GetGameRect();
 	POINT pt={0,0};
 	double angle=rand()%360;
 	
	pt.x = (rc.right - rc.left)/2;
	pt.y = (rc.bottom - rc.top)/2;

 	m_TrownPoint[m_nBetCount].x=cos(angle)*(rand()%120);
 	m_TrownPoint[m_nBetCount].y=sin(angle)*(rand()%60);
 	
 	pt = GetWorldPoint(m_TrownPoint[m_nBetCount]);
	if (bnFast)
	{
 		m_bitThrownBet[m_nBetCount]->MoveTo(pt.x,pt.y);
 		m_bitThrownBet[m_nBetCount]->Updata();
	}
	else
	{
		APPLY_MoveObject(m_bitThrownBet[m_nBetCount], pt, COMPARE_MOVE_SPEED);
		//APPLY_ThrowBet(m_ThrownBet[m_nBetCount],pt.x,pt.y,30,0);
	}
	
	m_nBetCount++;
}

void CMyGame::FitBet(int total, int& nBet4, int& nBet2, int& nBet1)
{
	if (total>16)
	{
		nBet4=(total-16)/7+((total-16)%7)/4+3;
		int temp=(total-16)%7;
		int temp2=temp/4;
		nBet2=(total-16)/7+(temp-temp2*4)/2+2;
	}
	else if (total>12)
	{
		nBet4=2;
		nBet2=2;
	}
	else if (total>8)
	{
		nBet4=1;
		nBet2=2;
	}
	else if (total>6)
	{
		nBet4=1;
		nBet2=1;
	}
	else if (total==6)
	{
		nBet4=1;
		nBet2=1;
	}
	else if (total==5)
	{
		nBet4=1;
		nBet2=0;
	}
	else if (total==4)
	{
		nBet4=1;
		nBet2=0;
	}
	else if (total==3)
	{
		nBet4=0;
		nBet2=1;
	}
	else if (total==2)
	{
		nBet4=0;
		nBet2=1;
	}
	else 
	{
		nBet4=0;
		nBet2=0;
	}
	
	nBet1=total-nBet4*4-nBet2*2;
}

void  CMyGame::OPE_TakeBet()
{
	int nWinChair=GetGameInfoEx()->GetGameWin()->nWinChair;
	int nWinDrawIndex=RUL_GetDrawIndexByChairNO(nWinChair);
//重新设置赢家总银两
	GAME_WIN_RESULT* pGameWinResult = GetGameInfoEx()->GetGameWin();
	if(pGameWinResult)
	{
		int nTotalDeposit = pGameWinResult->gamewin.nOldDeposits[nWinChair] + pGameWinResult->gamewin.nDepositDiffs[nWinChair];
		GetGameInfoEx()->SetChairTotalDeposit(nWinChair, nTotalDeposit);
		GetGameInfoEx()->SetChairPlayerDepoist(nWinChair, nTotalDeposit);
		GetGameInfoEx()->SetChairUsedDeposit(nWinChair, 0);
		m_ptrPlayer[nWinDrawIndex]->FreshPlayerBetInfo();
	}

#ifdef _OFFLINE_
	nWinDrawIndex = 0;
#endif
	
	POINT pt = m_ptrPlayer[nWinDrawIndex]->GetReadyPosition();

	for(int i=0;i<m_nBetCount;i++)
	{
		APPLY_MoveObject(m_bitThrownBet[i], pt, TAKE_BET_SPEED);
		APPLY_ShowObject(m_bitThrownBet[i],-0.04,0);
		m_TrownPoint[i] = pt;
	}
}

POINT CMyGame::GetWorldPoint(POINT pt)
{
	RECT rc = GetGameRect();
	int nWidth = rc.right - rc.left;
	int nHeight = rc.bottom - rc.top;

	pt.x=nWidth/2 + pt.x;
	pt.y=nHeight/2 + pt.y;
	return pt;
}

BOOL CMyGame::IsAutoFollow()
{
	return m_bAutoFollow;
}

void CMyGame::SetAutoFollow(BOOL bAutoFollow)
{
	m_bAutoFollow = bAutoFollow;
}

void CMyGame::SetAutoStart(BOOL bAutoStart)
{
	m_bAutoStart = bAutoStart;
}

BOOL CMyGame::IsAutoStart()
{
	return m_bAutoStart;
}

void CMyGame::ShowLookOn_Seat_ChangeTableBtn(BOOL bShowLookOn, BOOL bShowSeatBtn, BOOL bShowChangeTableBtn)
{
	//旁观按钮
	m_dlgOperation->m_btnStandBy->Enable(TRUE);
	if (TRUE == bShowLookOn)
	{
		PGL_SHOW_OBJECT(m_dlgOperation->m_btnStandBy);
	}
	else
	{
		PGL_HIDE_OBJECT(m_dlgOperation->m_btnStandBy);
	}

	//坐下按钮
	m_dlgOperation->m_btnSitDown->Enable(TRUE);
	if (TRUE == bShowSeatBtn)
	{
		PGL_SHOW_OBJECT(m_dlgOperation->m_btnSitDown);
	}
	else
	{
		PGL_HIDE_OBJECT(m_dlgOperation->m_btnSitDown);
	}

	//换桌按钮
	m_dlgOperation->m_btnChangeTable->Enable(TRUE);
	if (TRUE == bShowChangeTableBtn)
	{
		PGL_SHOW_OBJECT(m_dlgOperation->m_btnChangeTable);
	}
	else
	{
		PGL_HIDE_OBJECT(m_dlgOperation->m_btnChangeTable);
	}
}

void CMyGame::ResetAllCards()
{
	for(int i=0;i<TOTAL_CHAIRS*CHAIR_CARDS;i++)
	{
		m_Card[i]->m_nCardID	= -1;
		m_Card[i]->m_nChairNO	= -1;
		m_Card[i]->m_nDrawIndex	= -1;
		m_Card[i]->m_bnFace		= FALSE;
		m_Card[i]->m_nStatus	= 0;
		m_Card[i]->Hide();
		m_Card[i]->Updata();
	}
}

void CMyGame::UpdateAllCards()
{
	ResetAllCards();
	int cardcount=0;
	for(int i=0; i<TOTAL_CHAIRS; i++)
	{
		if (FALSE == CheckRoomPlayerStatus(i))
			continue;
		
		BOOL bLookCard = GetPublicInfoEx()->bnLookCard[i];
		for (int j=0;j<CHAIR_CARDS;j++)
		{ 
			m_Card[cardcount]->m_bnFace = bLookCard;
			if (i==theApp.GetChairNO() && !GetGameWinApp()->IsLookOn())
			{
				m_Card[cardcount]->m_nCardID = GetPlayerInfoEx(i)->nChairCards[j];
			} 
			m_Card[cardcount]->m_nChairNO = i;
			m_Card[cardcount]->m_nUnitIndex = j;
			m_Card[cardcount]->m_nDrawIndex = RUL_GetDrawIndexByChairNO(i); 
			
			m_Card[cardcount]->FitScreen(); 
			m_Card[cardcount]->Fresh();
			
			cardcount++;
		}
	}
}

void CMyGame::UpdateAllInPlayersStatus(ALL_IN_RESPONSE* allInReponse)
{
	if (NULL == allInReponse)
		return;

	int nChairNO = allInReponse->nChairNo;
	int nGoalChairNO = nChairNO;

	for (int i=0; i<TOTAL_CHAIRS; i++)
	{
		nGoalChairNO = RUL_GetNextChairNO(nGoalChairNO);

		if (nChairNO == nGoalChairNO)
			break;

		if (GetPlayerInfoEx(nGoalChairNO)->bInGame && allInReponse->bNeedCompare[nGoalChairNO])
		{
			if (allInReponse->bWinOrLose[nGoalChairNO])
			{
				GetGameInfoEx()->SetChairStatus(nGoalChairNO, PLAYER_STATUS_GIVE_UP);
			}
			else
			{
				GetGameInfoEx()->SetChairStatus(nChairNO, PLAYER_STATUS_GIVE_UP);
				break;
			}
		}
	}
}

BOOL CMyGame::IsPlayerGiveUp(int nChairNO)
{
	if (GetPlayerInfoEx(nChairNO)->nPlayerStatus == PLAYER_STATUS_GIVE_UP)
		return TRUE;

	return FALSE;
}

BOOL CMyGame::CheckRoomPlayerStatus(int nChairNO)
{
	if (theApp.m_GamePlayer[nChairNO].nUserID != GetPlayerInfoEx(nChairNO)->nUserID)
		return FALSE;

	if (theApp.m_GamePlayer[nChairNO].nStatus != ROOM_PLAYER_STATUS_PLAYING)
		return FALSE;

	return TRUE;
}

UINT CMyGame::ThreadShowWinResult(LPVOID lpParameter)
{
	CMyGame* pMyGame = (CMyGame*)lpParameter;
	if(!pMyGame)
		return 0;

	while(m_bThreadStart)
	{
		if (pMyGame->m_bGameWin && 0 == pMyGame->m_nLeftCompareNum)
		{
			Sleep(3500);
			if (pMyGame->m_bGameWin && 0 == pMyGame->m_nLeftCompareNum)
			{
				pMyGame->m_nLeftCompareNum = -1;
				pMyGame->m_bGameWin = FALSE;
				pMyGame->GameWin();
			}
		}
		Sleep(200);
	}
	return 0;
}

//auto set deposit begin
void CMyGame::createAutoSetDepositDlg()
{
	PGL_MSG_CLASS(m_dlgAutoSetDeposit,CAutoSetDepositDlg);
	m_dlgAutoSetDeposit->CreateSetDepositDlg();
	m_dlgAutoSetDeposit->SetFloor(FLOOR_SYSTEMWARNING);
	m_dlgAutoSetDeposit->SetMoveAble(TRUE);
	PGL_HIDE_OBJECT(m_dlgAutoSetDeposit);
}

void CMyGame::CheckKeyResult()
{
	if (m_dlgAutoSetDeposit)
	{
		if (m_dlgAutoSetDeposit->IsAutoSetDeposit())
		{
			CGameWinApp* pGameApp = (CGameWinApp*)AfxGetApp();
			CString sRet;
			UINT nResponse = 0;
			SAFE_RNDKEY sr;
			memset(&sr,0,sizeof(SAFE_RNDKEY));
			sr.nUserID = pGameApp->GetUserID();
			BOOL bRet = pGameApp->GetGameConnect()->GC_TakeRndKey(sRet,&sr,nResponse);
			if(bRet)
			{//有密码且没输入过密码	
				ShowKeyResult(TRUE, sr.nRndKey, 1);
			}
		}
	}
}

void CMyGame::AutoSetDeposit()
{
	if (m_dlgAutoSetDeposit)
	{
		if (m_dlgAutoSetDeposit->IsAutoSetDeposit())
		{
			int nAutoFixDeposit = GetAutoSetDeposit();
			int nPlayerDeposit = GetPlayerDeposit();
			if (nAutoFixDeposit > nPlayerDeposit)
			{
				CGameWinApp* pGameApp = (CGameWinApp*)AfxGetApp();
				CString sRet;
				UINT nResponse = 0;
				SAFE_RNDKEY sr;
				memset(&sr,0,sizeof(SAFE_RNDKEY));
				sr.nUserID = pGameApp->GetUserID();
				BOOL bRet = pGameApp->GetGameConnect()->GC_TakeRndKey(sRet,&sr,nResponse);
				if(bRet)
				{//有密码且没输入过密码	
					ShowKeyResult(TRUE, sr.nRndKey, 1);
				}
				else
				{//包括保护密码为空和其他情况
					TakeDepositToGame(nAutoFixDeposit,0);		
				}
			}
		}
	}
}

int CMyGame::GetAutoSetDeposit()
{
	CGameWinApp* pGameApp=(CGameWinApp*)AfxGetApp();
	TCHAR szValue[32];	
	memset(szValue,0,32);
	CString strKeyName;
	strKeyName.Format(_T("AutoFix"));
	GetPrivateProfileString ( _T("FIXDEPOSIT"),strKeyName,_T(""),szValue,32,pGameApp->GetDepositSetFile());
	return atoi(szValue);
}

void CMyGame::ShowKeyResult(BOOL bShow,int nRandKey, int nFlag)
{
	if (bShow)
	{
		if (NULL == m_pMyKeyResult)
		{
			PGL_MSG_CLASS(m_pMyKeyResult,CMyKeyResult);
			m_pMyKeyResult->CreateKeyResultDlg(nRandKey);
			m_pMyKeyResult->SetMoveAble(TRUE);			
			m_pMyKeyResult->SetFloor(FLOOR_SYSTEMWARNING-20);
		}
		
		if (m_pMyKeyResult)
		{
			m_pMyKeyResult->SetRandKey(nRandKey);
			m_pMyKeyResult->SpiritContext();
			m_pMyKeyResult->ClearEditResult();
			m_pMyKeyResult->SetFlag(nFlag);
			
			m_pMyKeyResult->MoveTo(GetScreenWidth()/2,GetScreenHeight()/2);
			m_pMyKeyResult->Show();
			m_pMyKeyResult->Updata();
		}
	}
	else
	{
		if (m_pMyKeyResult)
		{
			m_pMyKeyResult->Hide();
			m_pMyKeyResult->Updata();
		}
	}
}

void CMyGame::TakeDepositToGame(int nDestDeposit,int nKeyResult) 
{	
	CGameWinApp* pGameApp=(CGameWinApp*)AfxGetApp();			
	int nGameDeposit = GetPlayerDeposit();
	
	if (nDestDeposit>nGameDeposit)
	{
		CString sRet;
		CGameWinApp* pGameApp=(CGameWinApp*)AfxGetApp();
		BOOL bRet = pGameApp->GetGameConnect()->GC_TakeSafeDeposit(sRet,nDestDeposit-nGameDeposit,nKeyResult);	//取银子
		
		if (!bRet)
		{
			if (!theApp.IsGameRunning())
			{
				if (m_dlgAutoSetDeposit->IsAutoSetDeposit())
				{
					m_dlgAutoSetDeposit->OnCheck();
				}
			}

			pGameApp->ShowTimerMsgBox(sRet,NULL,NULL,10,TIMERDLG_FLAGS_ICON|TIMERDLG_FLAGS_DANYMIC,"xyf打牌");
		}
	}
}

int CMyGame::GetPlayerDeposit()
{
	int nDeposit = 0;
	if (theApp.IsLookOn())
	{//旁观
		int nChangeUserID = GetGameWinApp()->GetUserID();
		GAMEPLAYER* pPlayer = theApp.GetVisitorPlayerByUserID(nChangeUserID);
		nDeposit = pPlayer->nDeposit;
	}
	else
	{//坐下
		CGameWinApp* pGameApp=(CGameWinApp*)AfxGetApp();
		nDeposit = pGameApp->m_GamePlayer[pGameApp->GetChairNO()].nDeposit;
	}

	return nDeposit;
}

void CMyKeyResult::OnOK()
{
	if(!m_editResult)
		return;
	
	LPCTSTR szText= m_editResult->GetTextContext();
	int nDeposit=atoi(szText);
	m_nKeyResult=nDeposit;
	
	if(nDeposit<=0)
	{
		CGameWinApp* pGameApp=(CGameWinApp*)AfxGetApp();
		pGameApp->ShowTimerMsgBox(GMS_SECUREPWD_KEY_ERROR,NULL,NULL,10,TIMERDLG_FLAGS_ICON|TIMERDLG_FLAGS_DANYMIC,"xyf打牌");		
		return;
	}

//	CMyGame* pGame = (CMyGame*)GetObject(OBJECTNAME_MAIN_GAME);
//	pGame->OnMsg(CreateMsg(0,ZGSA_SAFEBOX_SETDEPOSIT_CHECK_SET,m_nKeyResult));

	OnCancel();
}
//auto set deposit end

void CMyGame::SetIsOpenBetFull()
{
	m_bOpenBetFull = GetPublicInfoEx()->bOpenBetFull;
}

BOOL CMyGame::GetIsOpenBetFull()
{
	return m_bOpenBetFull;
}

int CMyGame::GetBetDeposit()
{
	int nBaseDeposit = GetGameInfoEx()->GetBaseDeposit();
	int nCurrentBetNum = GetGameInfoEx()->GetGameTableInfo()->m_Public.nCurrentDouble;
	if(nCurrentBetNum <= 0)
		nCurrentBetNum = 1;
	
	if(GetPublicInfoEx()->bnLookCard[theApp.GetChairNO()])
		nCurrentBetNum *= 2;
	
	return nCurrentBetNum * nBaseDeposit;
}

int CMyGame::GetBetFullDeposit()
{
	int nChairNO = theApp.GetChairNO();
	int nDeposit = 0;
	int nAliveCount = 0;
	int nBetFullCount = 0;
	int nRestPlayerChair1No = -1;
	int nRestPlayerChair2No = -1;
	
	for (int i = 0; i < TOTAL_CHAIRS; i++)
	{
		if (GetPlayerInfoEx(i)->bInGame && PLAYER_STATUS_GIVE_UP != GetPlayerInfoEx(i)->nPlayerStatus)
		{
			nAliveCount++;
			if ((1 == nAliveCount && i == nChairNO)
				|| (2 == nAliveCount && i == nChairNO))
				nRestPlayerChair1No = i;
			else
				nRestPlayerChair2No = i;

			if (PLAYER_STATUS_BET_FULL == GetPlayerInfoEx(i)->nPlayerStatus)
				nBetFullCount++;
		}
	}
	
	if (2 != nAliveCount)
		return nDeposit;

	if (0 == nBetFullCount)
	{
		int nRestDeposit1 = GetGameInfoEx()->GetChairTotalDeposit(nRestPlayerChair1No) -
			GetGameInfoEx()->GetChairUsedDeposit(nRestPlayerChair1No);
		int nRestDeposit2 = GetGameInfoEx()->GetChairTotalDeposit(nRestPlayerChair2No) -
			GetGameInfoEx()->GetChairUsedDeposit(nRestPlayerChair2No);
		
		if (TRUE == GetPublicInfoEx()->bnLookCard[nRestPlayerChair1No])
			nDeposit = MIN(nRestDeposit1, nRestDeposit2);//明牌
		else
			nDeposit = MIN(nRestDeposit1, nRestDeposit2/2);//暗牌，向下取整
	}
	else if (1 == nBetFullCount)
	{
		int nRestDeposit2 = m_nBetFullDeposit;
		if (TRUE == GetPublicInfoEx()->bnLookCard[nRestPlayerChair2No])
		{//明牌
			if (TRUE == GetPublicInfoEx()->bnLookCard[nRestPlayerChair1No])
				nDeposit = nRestDeposit2;
			else
				nDeposit = nRestDeposit2 / 2;//向下取整
		}
		else
		{//暗牌
			if (TRUE == GetPublicInfoEx()->bnLookCard[nRestPlayerChair1No])
				nDeposit = nRestDeposit2 * 2;
			else
				nDeposit = nRestDeposit2;
		}
	}

	if (0 == nDeposit)
			nDeposit = 1;//如果算出来的押满银两为0，则取1

	return nDeposit;
}

int CMyGame::GetBetFullCount()
{
	int nBetFullCount = 0;
	for (int i = 0; i < TOTAL_CHAIRS; i++)
	{
		if (GetPlayerInfoEx(i)->bInGame && PLAYER_STATUS_GIVE_UP != GetPlayerInfoEx(i)->nPlayerStatus)
		{	
			if (PLAYER_STATUS_BET_FULL == GetPlayerInfoEx(i)->nPlayerStatus)
				nBetFullCount++;
		}
	}

	return nBetFullCount;
}

void CMyGame::NTF_GetGiveUpBtnConfig(int* bOpen)
{
	int a = *((int*) bOpen);
	if (*((int*) bOpen) >= 1)
		m_bShowGiveUpBtn = FALSE;
	else
		m_bShowGiveUpBtn = TRUE;
}

BOOL CMyGame::GetGiveUpBtnConfig()
{
	return m_bShowGiveUpBtn;
}