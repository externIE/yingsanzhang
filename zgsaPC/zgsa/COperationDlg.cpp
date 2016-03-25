#include "StdAfx.h"
#include "COperationDlg.h"

COperationDlg::COperationDlg()
{
	m_plBoard				= NULL;
	m_plNotes				= NULL;
	m_btnBet				= NULL;
	m_btnBetEight			= NULL;
	m_btnBetTwo				= NULL;
	m_btnBetFour			= NULL;
	m_btnAllin				= NULL;
	m_btnFollow				= NULL;
	m_btnLookCard			= NULL;
	m_btnGiveUp				= NULL;
	m_btnNextAutoRestFocus	= NULL;
	m_btnNextAutoRest		= NULL;
	m_btnAutoFollowFocus	= NULL;
	m_btnAutoFollow			= NULL;
	m_btnWinShow			= NULL;
	m_btnStandBy			= NULL;
	m_btnSitDown			= NULL;
	m_btnChangeTable		= NULL;
	m_pMainGame				= NULL;
	m_btnBetFull			= NULL;
	m_btnAutoSetDeposit		= NULL;
	m_digitBet				= NULL;
	m_digitAllInBet			= NULL;
	m_digitFullBet			= NULL;
}
COperationDlg::~COperationDlg()
{
	
}

void 	COperationDlg::OnIniData()
{
    m_pMainGame=(CMyGame*)GetObject(OBJECTNAME_MAIN_GAME); 	
	m_plBoard=CreatePlane(100,100); 
	m_plBoard->Erasure();
	m_plBoard->SetFloor(FLOOR_BTN);
	m_plBoard->TransGUI();
#ifdef _OFFLINE_
	m_plBoard->ShowBorderLine(RGB_GREEN);
#endif
	SetFrame(m_plBoard); 
	
    m_plNotes=CreatePlane(400,30); 
	m_plNotes->SetFloor(m_plNotes->GetZ()+0.1); 
    m_plNotes->MoveTo(m_plNotes->GetX(),m_plNotes->GetY());
    m_plBoard->AddChild(m_plNotes,OBJECT_LOCK_UPDATA);  
	m_plBoard->Show();
	
	POINT pt; 
	pt.x=m_plBoard->GetLeft()+708;
	pt.y=m_plBoard->GetTop()+35;
	
	m_btnNextAutoRestFocus = FastCreateStandButton("btn_nextAutoRestFocus", CreateMsg(m_pMainGame->GetHandle(),MSG_BUTTON_AUTO_START_FOCUS),pt);
	m_btnNextAutoRest = FastCreateStandButton("btn_nextAutoRest", CreateMsg(m_pMainGame->GetHandle(),MSG_BUTTON_AUTO_START),pt);
	m_btnAutoFollowFocus = FastCreateStandButton("btn_zidonggenzhu_focus", CreateMsg(m_pMainGame->GetHandle(),MSG_BUTTON_AUTO_FOLLOW_FOCUS),pt);
	m_btnAutoFollow = FastCreateStandButton("btn_zidonggenzhu", CreateMsg(m_pMainGame->GetHandle(),MSG_BUTTON_AUTO_FOLLOW),pt);
	
	m_btnSitDown = FastCreateStandButton("btn_zuoxia", CreateMsg(m_pMainGame->GetHandle(),MSG_BUTTON_SIT_ON),pt);
	m_btnStandBy = FastCreateStandButton("btn_pangguan", CreateMsg(m_pMainGame->GetHandle(),MSG_BUTTON_STAND_BY),pt);
	m_btnChangeTable = FastCreateStandButton("btn_huanzhuo", CreateMsg(m_pMainGame->GetHandle(),MSG_BUTTON_CHANGE_TABLE),pt);

	m_btnLookCard = FastCreateStandButton("btn_lookcard", CreateMsg(m_pMainGame->GetHandle(),MSG_BUTTON_LOOK_CARD),pt);
	m_btnFollow	= FastCreateStandButton("btn_follow", CreateMsg(m_pMainGame->GetHandle(),MSG_BUTTON_FOLLOW),pt);
	m_btnGiveUp	= FastCreateStandButton("btn_giveup", CreateMsg(m_pMainGame->GetHandle(),MSG_BUTTON_GIVE_UP),pt);
    m_btnBet =  FastCreateStandButton("btn_bet", CreateMsg(m_pMainGame->GetHandle(),MSG_BUTTON_BET),pt);
	m_btnBetTwo = FastCreateStandButton("btn_bet2", CreateMsg(m_pMainGame->GetHandle(),MSG_BUTTON_BET_TWO),pt);
	m_btnBetFour = FastCreateStandButton("btn_bet4", CreateMsg(m_pMainGame->GetHandle(),MSG_BUTTON_BET_FOUR),pt);
	m_btnBetEight = FastCreateStandButton("btn_bet8", CreateMsg(m_pMainGame->GetHandle(),MSG_BUTTON_BET_EIGHT),pt);
	m_btnAllin = FastCreateStandButton("btn_allin", CreateMsg(m_pMainGame->GetHandle(),MSG_BUTTON_ALLIN),pt); 
	m_btnBetFull = FastCreateStandButton("btn_betfull", CreateMsg(m_pMainGame->GetHandle(),MSG_BUTTON_BET_FULL),pt); 
	m_btnWinShow = FastCreateStandButton("亮牌", CreateMsg(m_pMainGame->GetHandle(),MSG_BUTTON_WIN_SHOW),pt); 
	m_btnAutoSetDeposit= FastCreateStandButton("btn_AutoSetDeposit", CreateMsg(m_pMainGame->GetHandle(),MSG_BUTTON_AUTO_SET_DEPOSIT),pt); 

	m_digitBet = FastCreateDigit("text_bet_numb", DIGIT_FLAGS_MID_JUSITIFY|DIGIT_FLAGS_USE_PIC_SIZE);
	m_digitAllInBet = FastCreateDigit("text_bet_numb", DIGIT_FLAGS_MID_JUSITIFY|DIGIT_FLAGS_USE_PIC_SIZE);
	m_digitFullBet = FastCreateDigit("text_bet_numb", DIGIT_FLAGS_MID_JUSITIFY|DIGIT_FLAGS_USE_PIC_SIZE);
}

CXyStandButton*  COperationDlg::FastCreateStandButton(LPCTSTR picName,SNG_MSG& msg,POINT pt,LPCTSTR szNote)
{
	CXyStandButton* pgl=NULL;
	PGL_MSG_CLASS(pgl,CXyStandButton);
	pgl->CreateGUI(Res_GetPICStruct(picName),&msg);
	pgl->SetButtonSound(BUTTON_STATUS_TOUCH,"SndTouchButton");
	pgl->SetButtonSound(BUTTON_STATUS_LEFT_DOWN,"SndHitButton");
	pgl->SetFloor(m_plBoard->GetZ()+0.2); 
	pgl->MoveTo(pt.x,pt.y);
	PGL_SET_OBJECT_NOTE(pgl,szNote); 
	m_plBoard->AddChild(pgl,OBJECT_LOCK_UPDATA);
	return pgl;
}

CXyDynamicButton*  COperationDlg::FastCreateDynamicButton(LPCTSTR picUntouch,LPCTSTR picTouch,LPCTSTR picDown,LPCTSTR picDisable,SNG_MSG& msg,POINT pt,LPCTSTR szNote)
{
	CXyDynamicButton* pgl = NULL;
	PGL_MSG_CLASS(pgl,CXyDynamicButton);
	pgl->CreateGUI(Res_GetPICStruct(picUntouch),&msg);
	pgl->SetButtonSound(BUTTON_STATUS_TOUCH,"SndTouchButton");
	pgl->SetButtonSound(BUTTON_STATUS_LEFT_DOWN,"SndHitButton");
	pgl->SetFloor(m_plBoard->GetZ()+0.2); 
	pgl->MoveTo(pt.x,pt.y);
	pgl->LoadButtonPic(BUTTON_STATUS_UNTOUCH,Res_GetPICStruct(picUntouch));
	pgl->LoadButtonPic(BUTTON_STATUS_TOUCH,Res_GetPICStruct(picTouch));
	pgl->LoadButtonPic(BUTTON_STATUS_LEFT_DOWN,Res_GetPICStruct(picDown));
	pgl->LoadButtonPic(BUTTON_STATUS_DISABLE,Res_GetPICStruct(picDisable));
	PGL_SET_OBJECT_NOTE(pgl,szNote); 
	m_plBoard->AddChild(pgl,OBJECT_LOCK_UPDATA);
	return pgl;
}

CMyDigit* COperationDlg::FastCreateDigit(LPCTSTR picName, DWORD dwRelation)
{
	CMyDigit* pDigit;
	PGL_MSG_CLASS(pDigit,CMyDigit); 
	pDigit->CreateDigit(picName, dwRelation);
	pDigit->SetFloor(m_btnBetFull->GetZ()+0.1); 
	m_plBoard->AddChild(pDigit, OBJECT_LOCK_UPDATA);

	return pDigit;
}

void COperationDlg::FitScreen()
{
	RECT rc = m_pMainGame->GetGameRect(); 
	int w = rc.right-rc.left;  
	int h = rc.bottom-rc.top; 
	int nMiddle = w/2;
	int nSize = Sys_GetIntStatus(GAME_STATUS_TABLE_SIZE);

	if (0 == nSize)
	{
		m_btnLookCard->MoveTo(nMiddle, h/2+202);
	}
	else
	{
		m_btnLookCard->MoveTo(nMiddle, h/2+252);
	}

	m_btnBetTwo->MoveTo(nMiddle - 102, rc.bottom - 80);	
	m_btnBetFour->MoveTo(nMiddle, rc.bottom - 80);	
	m_btnBetEight->MoveTo(nMiddle + 102, rc.bottom - 80);

	m_btnGiveUp->MoveTo(nMiddle - 102, rc.bottom - 28);
	m_btnBetFull->MoveTo(nMiddle, rc.bottom - 28);
	m_btnAllin->MoveTo(nMiddle + 102, rc.bottom - 28);
	m_btnFollow->MoveTo(nMiddle + 102, rc.bottom - 28);
	m_btnBet->MoveTo(nMiddle + 102, rc.bottom - 28);

	m_digitBet->MoveTo(nMiddle + 102, rc.bottom - 18);
	m_digitAllInBet->MoveTo(nMiddle + 102, rc.bottom - 18);
	m_digitFullBet->MoveTo(nMiddle, rc.bottom - 18);

	m_btnNextAutoRest->MoveTo(70, rc.bottom - 22);	
	m_btnNextAutoRestFocus->MoveTo(70, rc.bottom - 22);
	m_btnAutoFollow->MoveTo(rc.right - 58,rc.bottom - 22);
	m_btnAutoFollowFocus->MoveTo(rc.right - 58,rc.bottom - 22);
	m_btnAutoSetDeposit->MoveTo(rc.right - 55,rc.bottom - 70);

	m_btnChangeTable->MoveTo(rc.right - 60, rc.bottom - 30);
	m_btnSitDown->MoveTo(rc.right - 165, rc.bottom - 30);
	m_btnStandBy->MoveTo(rc.right - 165, rc.bottom - 30);
	
	m_btnAutoSetDeposit->Show();
	m_plBoard->Updata();
}

void  COperationDlg::ShowPlayerCouldOpe(DWORD dwCouldOpe)
{
	if (theApp.IsLookOn())
	{
		HidePlayerOpe(GAME_OPE_GIVE_UP|GAME_OPE_LOOK_CARD|GAME_OPE_AUTO_START|GAME_OPE_AUTO_FOLLOW);
		return;
	}
	
	HWND frontWnd =::GetForegroundWindow();
	HWND mainWnd=theApp.GetMainWnd()->GetSafeHwnd();
	if (frontWnd!=mainWnd )
	{
		::FlashWindow(theApp.GetMainWnd()->GetSafeHwnd(),TRUE);
	}
	
#ifdef  _OFFLINE_
	HidePlayerOpe(GAME_OPE_GIVE_UP|GAME_OPE_LOOK_CARD|GAME_OPE_AUTO_START|GAME_OPE_AUTO_FOLLOW);
	int ope[5]={GAME_OPE_GIVE_UP|GAME_OPE_ALLIN|GAME_OPE_LOOK_CARD|GAME_OPE_BET|GAME_OPE_COMPARE,
		GAME_OPE_GIVE_UP|GAME_OPE_ALLIN|GAME_OPE_FOLLOW|GAME_OPE_RISE|GAME_OPE_COMPARE,
		GAME_OPE_GIVE_UP|GAME_OPE_FOLLOW,
		GAME_OPE_GIVE_UP|GAME_OPE_ALLIN|GAME_OPE_COMPARE,
		GAME_OPE_GIVE_UP|GAME_OPE_LOOK_CARD|GAME_OPE_RISE};
	dwCouldOpe=ope[rand()%5];
#endif

	if (m_pMainGame->GetGameInfoEx()->GetCurrentChairNO() == theApp.GetChairNO())
	{
		m_btnFollow->Enable(FALSE);
		m_btnBet->Enable(FALSE);
		m_btnAllin->Enable(FALSE);
		m_btnBetFull->Enable(FALSE);

		EnableBetBoard(0);
		ShowBetBoard();

		m_btnFollow->Show();
		m_btnAllin->Show();
		m_btnBetFull->Show();

		if (IS_BIT_SET(dwCouldOpe,GAME_OPE_COMPARE))
		{
			for (int i=0; i < TOTAL_CHAIRS; i++)
			{
				if(!m_pMainGame->GetPlayerInfoEx(i)->bInGame || i==theApp.GetChairNO() || m_pMainGame->IsPlayerGiveUp(i))
					continue;
				
				m_pMainGame->m_ptrPlayer[m_pMainGame->RUL_GetDrawIndexByChairNO(i)]->ShowCompareBtn();
			}
		}
		else
		{
			for (int i=0; i < TOTAL_CHAIRS; i++)
			{
				if(!m_pMainGame->GetPlayerInfoEx(i)->bInGame || i==theApp.GetChairNO() || m_pMainGame->IsPlayerGiveUp(i))
					continue;
				
				m_pMainGame->m_ptrPlayer[m_pMainGame->RUL_GetDrawIndexByChairNO(i)]->HideCompareBtn();
			}
		}
		
		if (IS_BIT_SET(dwCouldOpe,GAME_OPE_FOLLOW))
		{
			m_btnAllin->Hide();
			m_btnBet->Hide();
			m_btnFollow->Enable(TRUE);
			SetBetDeposit(m_pMainGame->GetBetDeposit());
			PGL_SHOW_OBJECT(m_digitBet);
			EnableBetBoard(dwCouldOpe);
		}
		
		if (IS_BIT_SET(dwCouldOpe,GAME_OPE_BET))
		{ 
			int nMinBet=m_pMainGame->GetGameInfoEx()->GetRoundMinDeposit();
			int nMaxBet=m_pMainGame->GetGameInfoEx()->GetChairRestDeposit(theApp.GetChairNO());  
			nMaxBet+=m_pMainGame->GetGameInfoEx()->GetChairRoundDeposit(theApp.GetChairNO());
			
			//下注，跟注，全下 显示在同一位置,是互斥事件
			m_btnFollow->Hide();
			m_btnAllin->Hide();
			m_btnBet->Show();
			m_btnBet->Enable(TRUE);
			SetBetDeposit(m_pMainGame->GetBetDeposit());
			PGL_SHOW_OBJECT(m_digitBet);
			EnableBetBoard(dwCouldOpe);
		}	   
		
		if (!IS_BIT_SET(dwCouldOpe,GAME_OPE_RISE) &&!IS_BIT_SET(dwCouldOpe,GAME_OPE_BET) )
		{
			HideBetBoard();
			m_btnBet->Hide();
		}
		
		if (IS_BIT_SET(dwCouldOpe,GAME_OPE_ALLIN))
		{
			m_btnAllin->Enable(TRUE);
			m_pMainGame->SetAutoFollow(FALSE);
			PGL_SHOW_OBJECT(m_btnAllin);
			PGL_HIDE_OBJECT(m_btnFollow);
			PGL_HIDE_OBJECT(m_btnBet);

			int nDeposit = m_pMainGame->GetGameInfoEx()->GetChairRestDeposit(theApp.GetChairNO()); 
			m_digitAllInBet->SetDigit(nDeposit);
			PGL_SHOW_OBJECT(m_digitAllInBet);
			PGL_HIDE_OBJECT(m_digitBet);
		}

		if (IS_BIT_SET(dwCouldOpe, GAME_OPE_BET_FULL))
		{
			m_btnBetFull->Enable(TRUE);
			SetFullBetDeposit(m_pMainGame->GetBetFullDeposit());
			PGL_HIDE_OBJECT(m_btnAllin);
			PGL_SHOW_OBJECT(m_btnFollow);
			PGL_HIDE_OBJECT(m_btnBet);
		}
		else
		{
			m_btnBetFull->Enable(FALSE);
		}

		if(IS_BIT_SET(dwCouldOpe, GAME_OPE_LOOK_CARD))
		{
			PGL_HIDE_OBJECT(m_btnLookCard);
		}
		
		m_btnFollow->Updata();
		m_btnBet->Updata();
		m_btnAllin->Updata();
		m_btnBetFull->Updata();

		if (m_pMainGame->GetGiveUpBtnConfig() == FALSE)
			ShowGiveUpBtn(dwCouldOpe);
	}
	
	ShowPlayerCouldOpeNotCurrentChair(dwCouldOpe);
}

void COperationDlg::ShowPlayerCouldOpeNotCurrentChair(DWORD dwCouldOpe)
{
	if (TRUE==theApp.IsLookOn() || FALSE==m_pMainGame->CheckRoomPlayerStatus(theApp.GetChairNO()))
		return;

	m_btnLookCard->Enable(FALSE); 
	m_btnLookCard->Hide(); 
	if (IS_BIT_SET(dwCouldOpe,GAME_OPE_LOOK_CARD))
	{
		m_btnLookCard->Show();
		m_btnLookCard->Enable(TRUE);
	}
	m_btnLookCard->Updata(); 
	
	if (m_pMainGame->GetGiveUpBtnConfig() == TRUE)
		ShowGiveUpBtn(dwCouldOpe);
	
	ShowNextAutoRestAndFollowBtn();
}

//借用宏GAME_OPE_FOLLOW表示隐藏自动开始及跟注，GAME_OPE_GIVE_UP弃牌，GAME_OPE_LOOK_CARD看牌
void      COperationDlg::HidePlayerOpe(int ndwOpe)
{
	PGL_HIDE_OBJECT(m_btnBet);
	PGL_HIDE_OBJECT(m_btnFollow);
	PGL_HIDE_OBJECT(m_btnAllin);
	PGL_HIDE_OBJECT(m_btnBetFull);
	
	if (IS_BIT_SET(ndwOpe, GAME_OPE_GIVE_UP))
	{
		PGL_HIDE_OBJECT(m_btnGiveUp);
	}

	if(IS_BIT_SET(ndwOpe, GAME_OPE_LOOK_CARD))
	{
		PGL_HIDE_OBJECT(m_btnLookCard);
	}

	if (IS_BIT_SET(ndwOpe, GAME_OPE_AUTO_START))
	{ 
		ShowNextAutoRestBtn(FALSE, FALSE);
	}

	if (IS_BIT_SET(ndwOpe, GAME_OPE_AUTO_FOLLOW))
	{
		ShowAutoFollowBtn(FALSE, FALSE);
	}
	
	HideBetBoard();
} 

void  COperationDlg::ShowShowCardBtn()
{
	m_btnWinShow->Show();
	m_btnWinShow->Updata();
}

void COperationDlg::HideShowCardBtn()
{
	m_btnWinShow->Hide();
	m_btnWinShow->Updata();
}

enum{
	MSG_SCOLLBAR_BEGIN=MSG_CUSTOMER_BEGIN+1,
		MSG_SCOLLBAR_TOUCH_BTN,        //触发滑块
		MSG_SCOLLBAR_UNTOUCH_BTN,      //脱离滑块
		MSG_SCOLLBAR_LEFTDOWN_BTN,
		MSG_SCOLLBAR_LEFTUP_BTN,
		MSG_SCOLLBAR_LHIT_MOVE_BTN,
		MSG_SCOLLBAR_LEFTDOWN_BACKBROUND,  
		MSG_SCOLLBAR_MAX,
		MSG_SCOLLBAR_END
};

POINT  COperationDlg::GetRightBottomBoardPosition()
{
	RECT rc = m_pMainGame->GetGameRect();
	int nWidth = rc.right - rc.left;
	int nHeight = rc.bottom - rc.top;
	POINT pt;
	
	pt.x=rc.right-190;
	pt.y=rc.bottom-30;
	
	return pt;
}

POINT  COperationDlg::GetBetBoardPosition()
{
	RECT rc=m_pMainGame->RUL_GetZgSaTableRect();
	int nTableSize=Sys_GetIntStatus(GAME_STATUS_TABLE_SIZE);
	POINT pt;
	if (nTableSize==0)
	{
		pt.x=rc.right-73;
		pt.y=rc.top+517;
	}
	else
	{
		pt.x=rc.right-80;
		pt.y=rc.top+620;
	}
	
	return pt;
}

void COperationDlg::ShowBetBoard()
{
	PGL_SHOW_OBJECT(m_btnBetTwo);
	PGL_SHOW_OBJECT(m_btnBetFour);
	PGL_SHOW_OBJECT(m_btnBetEight);
}

void COperationDlg::HideBetBoard()
{
	PGL_HIDE_OBJECT(m_btnBetTwo);
	PGL_HIDE_OBJECT(m_btnBetFour);
	PGL_HIDE_OBJECT(m_btnBetEight);
}
//控制倍率可操作显示
void COperationDlg::EnableBetBoard(int nCouldOpe)
{
	if (IS_BIT_SET(nCouldOpe, GAME_OPE_BET_TWO))
		m_btnBetTwo->Enable(TRUE); 
	else
		m_btnBetTwo->Enable(FALSE);
	
	if (IS_BIT_SET(nCouldOpe, GAME_OPE_BET_FOUR))
		m_btnBetFour->Enable(TRUE); 
	else
		m_btnBetFour->Enable(FALSE);
	
	if (IS_BIT_SET(nCouldOpe, GAME_OPE_BET_EIGHT))
		m_btnBetEight->Enable(TRUE); 
	else
		m_btnBetEight->Enable(FALSE);
	
	m_btnBetTwo->Updata();
	m_btnBetFour->Updata();
	m_btnBetEight->Updata();
}

void COperationDlg::ShowNextAutoRestAndFollowBtn()
{
	if (TRUE == theApp.IsLookOn())
		return;

	int nUsedDeposit = m_pMainGame->GetGameInfoEx()->GetChairUsedDeposit(theApp.GetChairNO());
	int nBaseDeposit = m_pMainGame->GetGameInfoEx()->GetBaseDeposit();
	int nBetFullCount = m_pMainGame->GetBetFullCount();
	BOOL bAllIn = IS_BIT_SET(m_pMainGame->GetGameInfoEx()->GetCouldOpe(), GAME_OPE_ALLIN);
	if (nUsedDeposit <= nBaseDeposit || bAllIn || (1 == nBetFullCount))
	{
		EnableAutoFollowBtn(FALSE, FALSE);
		ShowAutoFollowBtn(TRUE, FALSE);
	}
	else
	{
		if (m_pMainGame->IsAutoFollow())
		{
			EnableAutoFollowBtn(FALSE, TRUE);
			ShowAutoFollowBtn(FALSE, TRUE);
		}
		else
		{
			EnableAutoFollowBtn(TRUE, FALSE);
			ShowAutoFollowBtn(TRUE, FALSE);
		}
	}
	
	if (m_pMainGame->IsAutoStart())
	{
		EnableNextAutoRestBtn(TRUE, FALSE);
		ShowNextAutoRestBtn(TRUE, FALSE);
	}
	else
	{
		EnableNextAutoRestBtn(FALSE, TRUE);
		ShowNextAutoRestBtn(FALSE, TRUE);
	}
}

void COperationDlg::ShowNextAutoRestBtn(BOOL bShowAutoStart, BOOL bShowAutoStartFocus)
{
	if (bShowAutoStart)
	{
		PGL_SHOW_OBJECT(m_btnNextAutoRest);
	}
	else
	{
		PGL_HIDE_OBJECT(m_btnNextAutoRest);
	}
	
	if (bShowAutoStartFocus)
	{
		PGL_SHOW_OBJECT(m_btnNextAutoRestFocus);
	}
	else
	{
		PGL_HIDE_OBJECT(m_btnNextAutoRestFocus);
	}
}

void COperationDlg::ShowAutoFollowBtn(BOOL bShowAutoFollow, BOOL bShowAutoFollowFocus)
{
	if (bShowAutoFollow)
	{
		PGL_SHOW_OBJECT(m_btnAutoFollow);
	}
	else
	{
		PGL_HIDE_OBJECT(m_btnAutoFollow);
	}
	
	if (bShowAutoFollowFocus)
	{
		PGL_SHOW_OBJECT(m_btnAutoFollowFocus);
	}
	else
	{
		PGL_HIDE_OBJECT(m_btnAutoFollowFocus);
	}
}

void COperationDlg::EnableNextAutoRestBtn(BOOL bEnableAutoStart, BOOL bEnableAutoStartFocus)
{
	m_btnNextAutoRest->Enable(bEnableAutoStart);
	m_btnNextAutoRestFocus->Enable(bEnableAutoStartFocus);
}

void COperationDlg::EnableAutoFollowBtn(BOOL bEnableAutoFollow, BOOL bEnableAutoFollowFocus)
{
	m_btnAutoFollow->Enable(bEnableAutoFollow);
	m_btnAutoFollowFocus->Enable(bEnableAutoFollowFocus);
}

void COperationDlg::SetFullBetDeposit(int nDeposit)
{
	if (NULL == m_digitFullBet || nDeposit <= 0)
		return;

	m_digitFullBet->SetDigit(nDeposit);
	PGL_SHOW_OBJECT(m_digitFullBet);
}

void COperationDlg::SetBetDeposit(int nDeposit)
{
	if (NULL == m_digitBet || nDeposit <= 0)
		return;
	
	m_digitBet->SetDigit(nDeposit);
}

void COperationDlg::HideDigitNumber()
{
	PGL_HIDE_OBJECT(m_digitBet);
	PGL_HIDE_OBJECT(m_digitAllInBet);
	PGL_HIDE_OBJECT(m_digitFullBet);
}

void COperationDlg::ShowGiveUpBtn(DWORD dwCouldOpe)
{
	m_btnGiveUp->Enable(FALSE);
	m_btnGiveUp->Show(); 
	
	if (IS_BIT_SET(dwCouldOpe,GAME_OPE_GIVE_UP))
	{
		m_btnGiveUp->Enable(TRUE); 
	}
	m_btnGiveUp->Updata();
}