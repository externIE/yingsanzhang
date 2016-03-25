// GameInfo.cpp: implementation of the CGameInfoEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "game.h"
#include "GameInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGameInfoEx::CGameInfoEx()
{
	ZeroMemory(&m_GameTableInfo,sizeof(m_GameTableInfo));
	ZeroMemory(&m_GameWinResult,sizeof(m_GameWinResult));
	InitialGameTableInfo(&m_GameTableInfo);
	m_bnAlowLook=FALSE;
}

CGameInfoEx::~CGameInfoEx()
{

}

GAME_WIN_RESULT*  CGameInfoEx::GetGameWin()
{
	return &m_GameWinResult;
}

GAME_START_DATA*  CGameInfoEx::GetGameStartData()
{
	return &m_GameTableInfo.startData;
}

GAME_PUBLIC_INFO*    CGameInfoEx::GetPubInfo()
{
	return &m_GameTableInfo.m_Public;
}

GAME_TABLE_INFO*  CGameInfoEx::GetGameTableInfo()
{
	return &m_GameTableInfo;
}
 
void  CGameInfoEx::NTF_StartGame(START_GAME* pStartGame)
{
	int chairno=pStartGame->nChairNO;

	m_GameEnterInfo.ei.dwUserStatus[chairno]|=US_GAME_STARTED;
}

void  CGameInfoEx::NTF_PlayerAbort(int chairno)
{
	m_GameEnterInfo.ei.dwUserStatus[chairno]&=~US_GAME_STARTED;
}

void CGameInfoEx::NTF_GameStart(GAME_START_INFO * pStartData)
{
	ResetStatusWhenStart();

	GAME_START_INFO * pGameStartInfo=(GAME_START_INFO*)pStartData;
    memcpy(&m_GameTableInfo,pStartData,sizeof(GAME_START_INFO));
	ZeroMemory(&m_GameWinResult,sizeof(m_GameWinResult)); 
}

void  CGameInfoEx::NTF_GameWin(GAME_WIN_RESULT* pGameWin)
{ 	
	m_GameTableInfo.m_Public.dwStatus=0;
	memcpy(&m_GameWinResult,pGameWin,sizeof(GAME_WIN_RESULT));
}

void  CGameInfoEx::ClearTableInfo()
{
	ZeroMemory(&m_GameTableInfo,sizeof(m_GameTableInfo));
	InitialGameTableInfo(&m_GameTableInfo);
}

void  CGameInfoEx::ResetStatusWhenStart()
{
	for(int i=0;i<MAX_CHAIR_COUNT;i++)
	{
		m_GameEnterInfo.ei.dwUserStatus[i]&=~US_GAME_STARTED;
	}
}

DWORD CGameInfoEx::GetStatus()
{
	return m_GameTableInfo.m_Public.dwStatus;
}

void CGameInfoEx::SetStatus(DWORD dwStatus)
{
    m_GameTableInfo.m_Public.dwStatus=dwStatus;
}

BOOL CGameInfoEx::IsGamePlaying()
{
    return IS_BIT_SET(GetStatus(),TS_PLAYING_GAME);
}

BOOL CGameInfoEx::IsForbidDesert()
{
	return m_GameTableInfo.startData.bForbidDesert;
}

BOOL CGameInfoEx::IsChairReady(int chairno)
{
	return IS_BIT_SET(m_GameEnterInfo.ei.dwUserStatus[chairno],US_GAME_STARTED);
}

void CGameInfoEx::SetCurrentChairNO(int chairno)
{
	GetPubInfo()->nCurrentChair=chairno;
}

int CGameInfoEx::GetCurrentChairNO()
{
	return GetPubInfo()->nCurrentChair;
}

int  CGameInfoEx::GetBoutCount()
{
	return GetGameStartData()->nBoutCount;
}

int CGameInfoEx::GetBaseDeposit()
{
	return GetGameStartData()->nBaseDeposit;
}

int CGameInfoEx::GetBaseScore()
{
    return GetGameStartData()->nBaseScore;
}

int  CGameInfoEx::GetBanker()
{
	return GetPubInfo()->nBanker;
}

int CGameInfoEx::GetBetWait()
{
#ifdef _OFFLINE_
	return BET_WAIT;
#endif
	return GetGameStartData()->nBetWaitTime;
}

int CGameInfoEx::GetOpeRestTime()
{
    int nTime=m_GameTableInfo.nOpeRestTime;

	if (nTime<=0)
	 	nTime=1;

	if (nTime>=GetBetWait())
		nTime=GetBetWait();

	return nTime;
}

void CGameInfoEx::SetOpeRestTime(int nRestTime)
{
	m_GameTableInfo.nOpeRestTime = nRestTime;
}	

int CGameInfoEx::GetChairTotalDeposit(int chairno)
{
	return m_GameTableInfo.m_Player[chairno].nTotalDeposit;
}

int   CGameInfoEx::GetChairPlayerDeposit(int chairno)
{
    return m_GameTableInfo.m_Player[chairno].nPlayerDeposit;
}

void CGameInfoEx::SetChairPlayerDepoist(int chairno,int nDeposit)
{
	m_GameTableInfo.m_Player[chairno].nPlayerDeposit = nDeposit;
}

void  CGameInfoEx::SetChairTotalDeposit(int chairno,int nDeposit)
{
	m_GameTableInfo.m_Player[chairno].nTotalDeposit = nDeposit;
}

void  CGameInfoEx::SetChairUsedDeposit(int chairno,int nDeposit)
{
	m_GameTableInfo.m_Player[chairno].nUsedDeposit = nDeposit;
}

int  CGameInfoEx::GetChairUsedDeposit(int chairno)
{
#ifdef _OFFLINE_
	return 1000;
#endif
	return m_GameTableInfo.m_Player[chairno].nUsedDeposit;
}

int   CGameInfoEx::GetChairRestDeposit(int chairno)
{
    if (m_GameTableInfo.m_Player[chairno].bInGame)
    {  
		int nChairUserID = theApp.m_GamePlayer[chairno].nUserID;
		int nGameUserID = m_GameTableInfo.m_Player[chairno].nUserID;
     	if (nChairUserID==nGameUserID)
		{
			return GetChairPlayerDeposit(chairno)-GetChairUsedDeposit(chairno);
		}
    }
     //放弃玩家或idlePlayer
	return  theApp.m_GamePlayer[chairno].nDeposit;
}

void  CGameInfoEx::SetChairRoundDeposit(int chairno,int nDeposit)
{
	m_GameTableInfo.m_Player[chairno].nRoundDeposit = nDeposit;
}

int CGameInfoEx::GetChairRoundDeposit(int chairno)
{
	return m_GameTableInfo.m_Player[chairno].nRoundDeposit;
}

int   CGameInfoEx::GetChairStatus(int chairno)
{
	return m_GameTableInfo.m_Player[chairno].nPlayerStatus;
}

void  CGameInfoEx::SetChairStatus(int chairno,int status)
{
	m_GameTableInfo.m_Player[chairno].nPlayerStatus=status;
} 
 
DWORD  CGameInfoEx::GetCouldOpe()
{
	return m_GameTableInfo.m_Public.dwCouldOpe;
}
 
void CGameInfoEx::SetCouldOpe(DWORD dwOpe)
{
	m_GameTableInfo.m_Public.dwCouldOpe = dwOpe;
}
 
int  CGameInfoEx::GetRoundMinDeposit()
{
	return m_GameTableInfo.m_Public.nRoundMinDeposit;
}
 
 
int  CGameInfoEx::GetRoundCurDeposit()
{
	return m_GameTableInfo.m_Public.nRoundCurDeposit;
}


void  CGameInfoEx::SetRoundCurDeposit(int nDeposit)
{
	m_GameTableInfo.m_Public.nRoundCurDeposit = nDeposit;
}

int   CGameInfoEx::GetTotalDesposit()
{
	return m_GameTableInfo.m_Public.nTotalDeposit;
}
 
void CGameInfoEx::SetTotalDesposit(int nDesposit)
{
	m_GameTableInfo.m_Public.nTotalDeposit= nDesposit;
}

BOOL   CGameInfoEx::IsNeedAssistOpe()
{
	
	if (theApp.IsLookOn())
	{
		return FALSE;
	}
	int nCurChair=GetCurrentChairNO();
	if (nCurChair==theApp.GetChairNO())
	{
		return FALSE;
	}
	if (!m_GameTableInfo.m_Player[theApp.GetChairNO()].bInGame)
	{
		return FALSE;
	}
	int nChairStatus=GetChairStatus(theApp.GetChairNO());
	if (nChairStatus==PLAYER_STATUS_ALLIN || nChairStatus==PLAYER_STATUS_GIVE_UP)
	{
		return FALSE;
	}
	int nParticipate=0;
	for (int i=0;i<TOTAL_CHAIRS;i++)
	{
		if (m_GameTableInfo.m_Player[i].bInGame)
		{
			nParticipate++;
		}
	}
	
	
	return TRUE;
}
 
BOOL  CGameInfoEx::IsGameEndAhead()
{
	return  m_GameWinResult.bGameEndAhead ;
}
 
//bIngame情况下
BOOL   CGameInfoEx::IsChairFold(int chairno)
{
	if (IS_BIT_SET(GetChairStatus(chairno),PLAYER_STATUS_GIVE_UP))
	{
		return TRUE;
	}
	return FALSE;
}
	
BOOL   CGameInfoEx::IsIdlePlayer(int chairno)
{
	if (m_GameTableInfo.m_Player[chairno].bInGame)
	{  
		int nChairUserID=theApp.m_GamePlayer[chairno].nUserID;
		int nGameUserID= m_GameTableInfo.m_Player[chairno].nUserID;
		if (nChairUserID==nGameUserID)
		{
			return FALSE;
		} 
	}
	return TRUE;
}
	
BOOL    CGameInfoEx::IsUserIDEqual(int chairno)
{
	if (m_GameTableInfo.m_Player[chairno].bInGame)
	{  
		int nChairUserID=theApp.m_GamePlayer[chairno].nUserID;
		int nGameUserID= m_GameTableInfo.m_Player[chairno].nUserID;
		if (nChairUserID==nGameUserID)
		{
			return TRUE;
		} 
	}
	return FALSE;
}
	
double  CGameInfoEx::GetWinRatio(int chairno)
{
	double ratio=1.0;
	return ratio;
	if (m_GameWinResult.gamewin.nDepositDiffs[chairno]<=0)
	{
		return ratio;
	}
	int nWinPoint=m_GameWinResult.gamewin.nWinPoints[chairno];
	int  nDepositDiff=m_GameWinResult.gamewin.nDepositDiffs[chairno];
    ratio=1.0*nDepositDiff/nWinPoint;
	return ratio;	
}	
	
int CGameInfoEx::GetFinalDeposit(int chairno)
{
	return m_GameWinResult.gamewin.nDepositDiffs[chairno] +m_GameWinResult.gamewin.nOldDeposits[chairno]; 
}
