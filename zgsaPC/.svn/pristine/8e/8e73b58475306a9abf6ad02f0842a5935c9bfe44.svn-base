#include "StdAfx.h"

CMyCard::CMyCard()
{
	m_nCardID			= -1;
	m_nChairNO			= -1;
	m_nDrawIndex		= -1;
	m_bnFace			= FALSE;
    m_nStatus			= 0;
}

CMyCard::~CMyCard()
{
	
	
}
int CMyCard::GetCardPic(int nCardID)
{
	return nCardID%54;
}

void CMyCard::OnIniData()
{
	SNG_PIC pic_card=Res_GetPICStruct("poker_small5068_bk");
	
	m_bitCard=CreateBitMap(pic_card);
	SetFrame(m_bitCard,OBJECT_LOCK_ALL);
    m_bitCard->TransGUI();
	CMyGame* pMainGame=(CMyGame*)GetObject(OBJECTNAME_MAIN_GAME);
}

void CMyCard::Fresh()
{
	int nTableSize=Sys_GetIntStatus(GAME_STATUS_TABLE_SIZE);
	{
		if (!m_bnFace)
		{
			SetAlpha(1.0);
			m_bitCard->LoadPic(Res_GetPICStruct("poker_small5068_bk"));
			SetFloor(CARD_FLOOR_INHAND+m_nUnitIndex); 
		}
		else 
		{
			SetFloor(CARD_FLOOR_INHAND-m_nUnitIndex); 
			
			if (m_nCardID>=0&&m_nCardID<TOTAL_CARDS)
			{ 
				int nPokerType=theApp.m_GameSet.nPokerType;
				if (nPokerType==0)
					m_bitCard->LoadPic(Res_GetPICStruct("poker_small",GetCardPic(m_nCardID)));
				else
					m_bitCard->LoadPic(Res_GetPICStruct("poker_small2",GetCardPic(m_nCardID)));
				
				SetFloor(CARD_FLOOR_INHAND+m_nUnitIndex); 
			}
			else
			{ 
				m_bitCard->LoadPic(Res_GetPICStruct("poker_small5068_bk"));
			} 

			if (m_nStatus==2)
				m_bitCard->SetAlpha(0.5);
			else
				m_bitCard->SetAlpha(1);
		} 
	}
	Show();
	Updata();
}

void CMyCard::FitScreen()
{
	if (m_nChairNO==-1) 
	{   
		return; 
	}
	else
	{
		CString szName;
		szName.Format("Íæ¼ÒÐÕÃû°å%d",m_nDrawIndex);
		CMyPlayer *pPlayer=(CMyPlayer*)GetObject(szName);
		POINT pt=pPlayer->GetCardPositon(m_bnFace,m_nUnitIndex,m_nStatus);
		MoveTo(pt.x,pt.y);
		Updata();
	}
}