#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "StdAfx.h"
#include <algorithm>

int IsShun(int value[3])
{
	if (value[0]==value[1]+1
		&&value[1]==value[2]+1)
		return value[0];
	
	if (value[0]==12
		&&value[1]==1
		&&value[2]==0)
		return value[1];
	
	return -1;
}

int IsDuiZi(int value[3])
{
	if (value[0]==value[1]
		&&value[0]==value[2])
		return -1;
	
	if (value[0]==value[1])
		return value[0];
	
	if (value[1]==value[2])
		return value[1];
	
	if (value[2]==value[0])
		return value[2];
	
	return -1;
}

BOOL Is235(int value[],int shape[])
{
	if (value[0]==3//5
		&&value[1]==1//3
		&&value[2]==0)
		return TRUE;
	
	return FALSE;
}

CGameTable::CGameTable(int roomid, int tableno, int score_mult, 
			   int totalchairs, DWORD gameflags, DWORD gameflags2, int max_asks,  
			   int betWait  )
			   :CTable(roomid,tableno,score_mult,totalchairs,gameflags,gameflags2,max_asks )
{
	// 固定信息，跟局数无关
	m_GameTalbeInfo=NULL;
 
	m_nBetWait=betWait;
	ResetMembers();
}

CGameTable::~CGameTable()
{
 	ResetMembers();
	
	if (m_GameTalbeInfo)
		delete m_GameTalbeInfo;
}

void InitialGameTableInfo(GAME_TABLE_INFO* table)
{
	if (!table) 
		return;
   ZeroMemory(&(table->m_Public),sizeof(GAME_PUBLIC_INFO));
   table->m_Public.nTableNO = -1;
   for (int i = 0; i < TOTAL_CHAIRS; i++)
		table->m_Public.nKiller[i] = -1;

   ZeroMemory(table->m_Player,sizeof(GAME_PLAYER_INFO)*TOTAL_CHAIRS);
}


GAME_TABLE_INFO* CGameTable::GetGameTableInfo()
{
	if (!m_GameTalbeInfo)
		ConstructGameData();

	return m_GameTalbeInfo;
}

GAME_START_DATA*  CGameTable::GetStartData()
{
	if (!m_GameTalbeInfo)
		ConstructGameData();
	
	return &m_GameTalbeInfo->startData;
}

GAME_PUBLIC_INFO* CGameTable::GetPublicInfo()
{
	if (!m_GameTalbeInfo)
		ConstructGameData();

	return &m_GameTalbeInfo->m_Public;
}

GAME_PLAYER_INFO* CGameTable::GetPlayerInfo(int chairno)
{
	if (!m_GameTalbeInfo)
		ConstructGameData();

	return &m_GameTalbeInfo->m_Player[chairno];
}

void CGameTable::ResetMembers(BOOL bResetAll)
{
	CTable::ResetMembers(bResetAll);

	 
//	InitialGameTableInfo(m_GameTalbeInfo);
	if(bResetAll){
		// 动态信息，跟上局相关
	}
	m_dwLastClockStop=0;

	CString szIniFile=GetINIFileName();
	TCHAR szRoom[20];
	sprintf(szRoom,"%d",m_nRoomID);  
	m_nBetWait = GetPrivateProfileInt(_T("BetTime"), szRoom, BET_WAIT, szIniFile);
	SetStatus(0);
}

void CGameTable::ResetPlayer(int chairno)
{
	m_ptrPlayers[chairno]=NULL;
	//m_mapVisitors[chairno].RemoveAll();
	
	m_dwUserConfig[chairno]=0;
	m_dwRoomOption[chairno]=0;
	m_dwRoomConfig[chairno]=0;
	
	m_nPartnerGroup[chairno]=INVALID_OBJECT_ID;
	m_dwFirstEnter[chairno]=0;
	
	m_dwLatestAction[chairno]=0;
	m_dwCostTime[chairno]=0;
	m_dwBreakTime[chairno]=0;
	m_nAutoCount[chairno]=0;
	m_nBreakCount[chairno]=0;
	m_dwUserStatus[chairno]=0;
	m_dwCheckBreakTime[chairno]=0;
	m_nIdleGameCount[chairno]=0;
}

int CGameTable::GetGameTableInfoSize()
{
	return sizeof(GAME_TABLE_INFO);
}

void CGameTable::FillupGameTableInfo(void* pData, int nLen, int chairno, BOOL lookon)
{
	ZeroMemory(pData, nLen);
	FillupStartData(pData,nLen);
	//跳过GAME_START_INFO,拷贝公共信息
	GAME_TABLE_INFO* pGameInfo=(GAME_TABLE_INFO*) pData;
	//拷贝公共信息
	GAME_TABLE_INFO* pGameData=GetGameTableInfo(); 
	memcpy(&pGameInfo->m_Public,&(pGameData->m_Public),sizeof(GAME_PUBLIC_INFO)); 
	memcpy(&pGameInfo->m_Player,&(pGameData->m_Player),sizeof(GAME_PLAYER_INFO)*TOTAL_CHAIRS);

	pGameInfo->m_Public.nTableNO = m_nTableNO;
	for (int i=0;i<TOTAL_CHAIRS;i++)
	{
		if (chairno == i && FALSE == lookon && m_GameTalbeInfo->m_Public.bnLookCard[i])
			continue;
		
		pGameInfo->m_Player[i].nChairCards[0]=-1;
		pGameInfo->m_Player[i].nChairCards[1]=-1;
		pGameInfo->m_Player[i].nChairCards[2]=-1;										
	}
    if (IS_BIT_SET(m_dwStatus,TS_WAITING_OPERATION))
    {
		int nElpaseTime=(GetTickCount()-m_dwActionStart)/1000;
		pGameInfo->nOpeRestTime=pGameData->startData.nBetWaitTime-nElpaseTime;
    }
}

int CGameTable::GetEnterGameInfoSize()
{
	return sizeof(GAME_ENTER_INFO);
}

void CGameTable::FillupEnterGameInfo(void* pData, int nLen, int chairno, BOOL lookon)
{
	CTable::FillupEnterGameInfo(pData,nLen,chairno,lookon);

	GAME_ENTER_INFO* pEnterGame=(GAME_ENTER_INFO*)pData;
}

int CGameTable::GetGameStartSize()
{
	return sizeof(GAME_START_INFO);
}

void CGameTable::FillupGameStart(void* pData, int nLen, int chairno, BOOL lookon)
{
	ZeroMemory(pData, nLen);

	GAME_START_INFO* pStartData=(GAME_START_INFO*)pData;
	memcpy(pStartData,m_GameTalbeInfo,GetGameStartSize());
 	FillupStartData(pData,nLen); 
	//拷贝公共信息
	GAME_TABLE_INFO* pGameData=GetGameTableInfo(); 
	memcpy(&pStartData->m_Public,&(pGameData->m_Public),sizeof(GAME_PUBLIC_INFO)); 
	memcpy(&pStartData->m_Player,&(pGameData->m_Player),sizeof(GAME_PLAYER_INFO)*TOTAL_CHAIRS);

	pStartData->m_Public.nTableNO = m_nTableNO;
   	for (int i=0;i<TOTAL_CHAIRS;i++)
	{
		pStartData->m_Player[i].nChairCards[0]=-1;
		pStartData->m_Player[i].nChairCards[1]=-1;
		pStartData->m_Player[i].nChairCards[2]=-1;
	}
}

int CGameTable::GetGameWinSize()
{
    return sizeof(GAME_WIN_RESULT);
}

int CGameTable::FillupGameWin(void* pData, int nLen, int chairno)
{
 
	ZeroMemory(pData,GetGameWinSize());
	GAME_WIN_RESULT* gamewin=(GAME_WIN_RESULT*)pData;

	GAME_TABLE_INFO tblInfo;
	memcpy(&tblInfo,m_GameTalbeInfo,sizeof(tblInfo));
	GAME_WIN_RESULT GameResult;
	ZeroMemory(&GameResult,sizeof(GameResult));

	int nTotalWinDeposit = 0;
	int nPlayerCount = 0;
	for (int i = 0; i < TOTAL_CHAIRS; i++)
	{
		if (FALSE == m_GameTalbeInfo->m_Player[i].bInGame)
			continue;

		nTotalWinDeposit += m_GameTalbeInfo->m_Player[i].nUsedDeposit;

		if (m_GameTalbeInfo->m_Player[i].nPlayerStatus != PLAYER_STATUS_GIVE_UP)
		{
			gamewin->nWinChair = i;
			gamewin->nWinType = GetPlayerInfo(i)->nCardType;
		}

		nPlayerCount++;
	}

	memcpy(gamewin->nKiller, GetPublicInfo()->nKiller, sizeof(int)*TOTAL_CHAIRS);
	for (i=0;i<TOTAL_CHAIRS;i++)
	{
		XygInitChairCards(gamewin->nChairCards[i], CHAIR_CARDS);

		if (!m_GameTalbeInfo->m_Player[i].bInGame)continue;

		memcpy(gamewin->nChairCards[i], m_GameTalbeInfo->m_Player[i].nChairCards, sizeof(int)*CHAIR_CARDS);

		if (m_GameTalbeInfo->m_Player[i].nPlayerStatus != PLAYER_STATUS_GIVE_UP)
			gamewin->playerWinInfo[i].nTotalWinDeposit = nTotalWinDeposit;
		else
			gamewin->playerWinInfo[i].nTotalWinDeposit = 0;
	}
	m_nDiffDeposit=0;

	if (GetAliveUserCount()==1)
	{
		gamewin->bGameEndAhead=TRUE;
	}
	gamewin->nTableNO = m_nTableNO;

	return CTable::FillupGameWin(pData, nLen, chairno);
}

void CGameTable::FillupStartData(void* pData, int nLen)
{
	GAME_START_DATA* pStartData=(GAME_START_DATA*)pData;
	memcpy(pStartData->szSerialNO, m_szSerialNO, sizeof(m_szSerialNO)); 
	pStartData->nBoutCount=m_nBoutCount;
	pStartData->nBaseScore=m_nBaseScore;
	pStartData->nBaseDeposit=m_nBaseDeposit;
	
	
	pStartData->bForbidDesert=m_bForbidDesert;  //禁止强退,提醒凯子发布的时候打开
	pStartData->bNeedDeposit=m_bNeedDeposit;
	pStartData->nBetWaitTime = m_nBetWait;
}	

BOOL CGameTable::IsGameMsg(UINT resquesID)
{
	if (resquesID>SYSMSG_BEGIN&&resquesID<SYSMSG_END)
		return TRUE;
	if (resquesID> LOCAL_GAME_MSG_BEGIN&&resquesID< LOCAL_GAME_MSG_END)
		return TRUE;
	
	return FALSE;
}

int CGameTable::CalcBankerChairBefore()
{
    int result = 0;

	int nChair[TOTAL_CHAIRS];
	int nChairCount=0;;
	for ( int i=0;i<TOTAL_CHAIRS;i++)
	{
		if (GetPlayerInfo(i)->bInGame)
		{
		  nChair[nChairCount++]=i;
		}
	} 
 	result = XygGetRandomBetween(nChairCount);
 	return nChair[result]; 
  
}


int CGameTable::CalcBankerChairAfter(void* pData, int nLen)
{
    return m_nBanker;
}

BOOL CGameTable::CalcWinPoints(void* pData, int nLen, int chairno, int nWinPoints[])
{
	GAME_WIN_RESULT* pGameWin = (GAME_WIN_RESULT*)pData;
	m_nBanker = pGameWin->nWinChair;
	 
	for (int i=0;i<TOTAL_CHAIRS;i++)
	{
		int nTotalWinDeposit=pGameWin->playerWinInfo[i].nTotalWinDeposit;
		int nUsedDeposit =GetPlayerInfo(i)->nUsedDeposit;
		nWinPoints[i]=nTotalWinDeposit-nUsedDeposit;
	}
	/////////////////////////////////////////////////////
	int total=0;
	for(  i = 0; i < m_nTotalChairs; i++)
		total+=nWinPoints[i];
    total+=m_nDiffDeposit;
 
	if (total!=0)
	{
		UwlLogFile("错误的记分错误，记分总和不为0! total=%d,nDiff=%d",total,m_nDiffDeposit);
		for(i = 0; i < m_nTotalChairs; i++)
			nWinPoints[i]=0;
	}
	////////////////////////////////////////////////////*/
	return TRUE;
}

int CGameTable::CalcResultDiffs(void* pData, int nLen, int nScoreDiffs[], int nDepositDiffs[])
{
	LPGAME_WIN pGameWin = (LPGAME_WIN)pData;

	int nLose=0;
	for(int i = 0; i < m_nTotalChairs; i++)
	{
		if (m_GameTalbeInfo->m_Player[i].bInGame)
		{
			if (pGameWin->nWinPoints[i]<0)
				nLose++;
		}
	}

	// 计算积分结果
	for(i = 0; i < m_nTotalChairs; i++)
	{		
		if (pGameWin->nWinPoints[i]>0)
			nScoreDiffs[i]=m_nBaseScore*nLose; 
		else if (pGameWin->nWinPoints[i]<0)
			nScoreDiffs[i]=-m_nBaseScore;
		else
			nScoreDiffs[i]=0;
	}

	if(m_nBaseDeposit)
	{ // 玩银子，计算银子结果
		for(int i = 0; i < m_nTotalChairs; i++)
		{		
			nDepositDiffs[i] = pGameWin->nWinPoints[i];
		}
	}
	return 0;
}

void CGameTable::ConstructGameData()
{
	if (!m_GameTalbeInfo)
	{
		m_GameTalbeInfo=new GAME_TABLE_INFO;
		ZeroMemory(m_GameTalbeInfo,sizeof(GAME_TABLE_INFO));
		FillupStartData(m_GameTalbeInfo,sizeof(GAME_START_DATA));
		InitialGameTableInfo(m_GameTalbeInfo);
	}
}

void CGameTable::CalcZgsaFee(int nFee[TOTAL_CHAIRS])
{
	ZeroMemory(nFee,sizeof(int)*TOTAL_CHAIRS); 
	if (m_nFeeMode==FEE_MODE_SERVICE_FIXED)
	{
		for(int i = 0; i < TOTAL_CHAIRS; i++)
		{
			if (m_ptrPlayers[i] && !m_ptrPlayers[i]->m_bIdlePlayer)
			{
				nFee[i]=m_nFeeValue;
			}
		}
	} 
	else if (FEE_MODE_SERVICE_MINDEPOSIT == m_nFeeMode)
	{
		int fee=0;
		int depositMin = 0;
		for(int i = 0; i < TOTAL_CHAIRS; i++)
		{
			if (m_ptrPlayers[i] && !m_ptrPlayers[i]->m_bIdlePlayer)
			{
				int deposit=m_ptrPlayers[i]->m_nDeposit;
				if ((0 == depositMin)
					|| (deposit < depositMin))
					depositMin = deposit;
			}
		} 
		
		if (depositMin>=m_nFeeMinimum)
		{
			 
			fee = ceil((double)depositMin*m_nFeeTenThousandth/10000);  
		}

		for( i = 0; i < TOTAL_CHAIRS; i++)
		{
			if (m_ptrPlayers[i] && !m_ptrPlayers[i]->m_bIdlePlayer)
			{
				nFee[i]=fee;
			}
		}
		
	} 
	else if (FEE_MODE_SERVICE_SELFDEPOSIT == m_nFeeMode)
	{
		for(int i = 0; i < TOTAL_CHAIRS; i++)
		{
			if (m_ptrPlayers[i] && !m_ptrPlayers[i]->m_bIdlePlayer)
			{
				int deposit=m_ptrPlayers[i]->m_nDeposit;
				if (deposit>=m_nFeeMinimum)
				{
					int fee = ceil((double)deposit*m_nFeeTenThousandth/10000);
					nFee[i]=fee;
				}
			}
		}
	}
}

int   CGameTable::CalcBaseDeposit(int nDeposits[], int tableno)
{
	assert(m_nTotalChairs <= MAX_CHAIRS_PER_TABLE);
	
	CString strIniFile = GetINIFileName();

	//获取底注系数
	int nCoefficient = GetPrivateProfileInt(
		_T("ChangeableBaseDeposit"),	// section name
		_T("Coefficient"),				// key name
		DEFAULT_BASE_DEPOSIT_COEFFICIENT,	// default int
		strIniFile						// initialization file name
			);

	if (0 >= nCoefficient)
		nCoefficient = DEFAULT_BASE_DEPOSIT_COEFFICIENT;

	//获取底注倍数
	int nMultiple = GetPrivateProfileInt(
		_T("ChangeableBaseDeposit"),	// section name
		_T("Multiple"),					// key name
		DEFAULT_BASE_DEPOSIT_MULTIPLE,	// default int
		strIniFile						// initialization file name
		);
	
	if (0 >= nMultiple)
		nMultiple = DEFAULT_BASE_DEPOSIT_MULTIPLE;
	
	//获取获取最大可变基础银
	int nMaxBaseDeposit = GetPrivateProfileInt(
		_T("ChangeableBaseDeposit"),	// section name
		_T("MaxBaseDeposit"),			// key name
		DEFAULT_MAX_BASE_DEPOSIT,		// default int
		strIniFile						// initialization file name
		);
	
	//获取房间最小可变基础银
	TCHAR szRoomID[16];
	memset(szRoomID, 0, sizeof(szRoomID));
	_stprintf(szRoomID, _T("%ld"), m_nRoomID);
	int nMinDeposit = GetPrivateProfileInt(
		_T("MinChangeableBaseDeposit"),	// section name
		szRoomID,						// key name
		DEFAULT_MIN_CHANGE_BASE_DEPOSIT,// default int
		strIniFile						// initialization file name
		);
	
	int nBase = 0;
	int mindeposit = 0;
	for(int i = 0; i < m_nTotalChairs; i++){
		if(nDeposits[i]>0 && (0==mindeposit || nDeposits[i]<mindeposit)){
			mindeposit = nDeposits[i];
		}
	}
	int nTemp = mindeposit;
	if(nTemp <= 100){
		nTemp = 100;
	}
	nTemp /= nCoefficient;
	if (1 > nTemp)
		nTemp = 1;
	
	nTemp = UwlLog2(nTemp);
	
	nBase = UwlPow2(nTemp) * nMultiple;
	
	if (nBase < nMinDeposit)
		nBase = nMinDeposit;
	
	if (nBase > nMaxBaseDeposit)
		nBase = nMaxBaseDeposit;
	
	return nBase;
}

void CGameTable::StartDeal()
{
	InitialGameTableInfo(m_GameTalbeInfo);
	m_nBaseScore=1;
	int nFee[TOTAL_CHAIRS];
    CalcZgsaFee(nFee);
	
	{
	
		int nMinDeposit = 0;
		for (int i=0;i<TOTAL_CHAIRS;i++)
		{
			if (m_ptrPlayers[i] && !m_ptrPlayers[i]->m_bIdlePlayer)
			{
				GetPlayerInfo(i)->nUserID=m_ptrPlayers[i]->m_nUserID;

				GetPlayerInfo(i)->bInGame = TRUE;
				GetPlayerInfo(i)->nTotalDeposit = m_ptrPlayers[i]->m_nDeposit-nFee[i];
				GetPlayerInfo(i)->nPlayerDeposit = GetPlayerInfo(i)->nTotalDeposit;

				if (0==nMinDeposit || nMinDeposit>GetPlayerInfo(i)->nTotalDeposit)
					nMinDeposit = GetPlayerInfo(i)->nTotalDeposit;
			}
			else
			{
				GetPlayerInfo(i)->bInGame=FALSE;
			} 

			if (nFee[i]>0 && 0==GetPublicInfo()->nFee)
			{
				GetPublicInfo()->nFee = nFee[i];
			}
		}
 
		//读取是否开启押满功能
		BOOL bOpenbetFull = GetPrivateProfileInt(
								_T("BetFull"),		// section name
								_T("Open"),			// key name
								0,					// default int
								GetINIFileName()	// initialization file name
								);

		if (TRUE == bOpenbetFull)
		{
			for (int i=0;i<TOTAL_CHAIRS;i++)
			{
				if (GetPlayerInfo(i)->bInGame && GetPlayerInfo(i)->nTotalDeposit>nMinDeposit*2)
					GetPlayerInfo(i)->nTotalDeposit = nMinDeposit*2;
			}

			GetPublicInfo()->bOpenBetFull = bOpenbetFull;
		}

		GetPublicInfo()->nRoundMinDeposit = m_nBaseDeposit;
	}

	if (1 == m_nBoutCount)//第一局随机定庄家
		m_nBanker=CalcBankerChairBefore();
	
#ifdef _DEBUG
//	m_nBanker=0;
	DWORD dwRead = GetPrivateProfileInt(_T("StartDeal"), _T("ReadCardsFromFile"), 0, GetINIFileName());
	if (dwRead)
	{
		ReadCardsFromFile();
		m_nBanker = 0;
	}
	else
#endif
	{
		//排序,生成随机牌序
		int card[TOTAL_CARDS];
		for(int i=0;i<TOTAL_CARDS;i++)
			card[i]=i;

		CPlayer* ptrP =NULL;
		for (i=0;i<m_nTotalChairs;i++)
		{
			ptrP=m_ptrPlayers[i];
			if (ptrP)
			{
				break;
			}
		}
	 
		xyRandomSort(card,TOTAL_CARDS,GetTickCount()+ ptrP->m_lTokenID * 10 + ptrP->m_hSocket);

//3个玩家
// 		card[0] = 0;
// 		card[1] = 2;
// 		card[2] = 4;
// 		card[3] = 1;
// 		card[4] = 3;
// 		card[5] = 5;
// 		card[6] = 18;
// 		card[7] = 19;
// 		card[8] = 20;
// 		card[18] = 6;
// 		card[19] = 7;
// 		card[20] = 8;

//4个玩家
// 		card[0] = 0;
// 		card[1] = 2;
// 		card[2] = 4;
// 		card[3] = 6;
// 		card[4] = 1;
// 		card[5] = 3;
// 		card[6] = 5;
// 		card[7] = 7;
// 		card[8] = 18;
// 		card[9] = 19;
// 		card[10] = 20;
// 		card[11] = 21;
// 		card[18] = 8;
// 		card[19] = 9;
// 		card[20] = 10;
// 		card[21] = 11;

//5个玩家
// 		card[0] = 0;
// 		card[1] = 2;
// 		card[2] = 4;
// 		card[3] = 6;
// 		card[4] = 8;
// 		card[5] = 1;
// 		card[6] = 3;
// 		card[7] = 5;
// 		card[8] = 7;
// 		card[9] = 9;
// 		card[10] = 21;
// 		card[11] = 22;
// 		card[12] = 23;
// 		card[13] = 24;
// 		card[14] = 25;
// 		card[21] = 10;
// 		card[22] = 11;
// 		card[23] = 12;
// 		card[24] = 13;
// 		card[25] = 14;

		//发牌
		int index=0;
		for(int j=0;j<CHAIR_CARDS;j++)//每人三张
		{
			for(i=0;i<m_nTotalChairs;i++) //几个人，轮着摸
			{
// #ifdef _DEBUG
// 				if (0 == i)
// 				{
// 					switch(j)
// 					{
// 					case 0:
// 						card[index] = 0;
// 						break;
// 					case 1:
// 						card[index] = 13;
// 						break;
// 					case 2:
// 						card[index] = 26;
// 						break;
// 					}
// 				}
// #endif
				if (m_ptrPlayers[i])
				{
					GetPlayerInfo(i)->nChairCards[j]=card[index];
			    	index++;
				}
			}
		}

		//根据手牌计算牌型及权值
		int nInGamePlayerCount = 0;
		{
			for(int i=0;i<m_nTotalChairs;i++)
			{
				CalcBeginPlay(i);

				if (m_ptrPlayers[i] && GetPlayerInfo(i)->bInGame)
					nInGamePlayerCount++;
			}
		}

		MakeDeal(card, index, nInGamePlayerCount);
		ASSERT(CheckCards(card));
	} 
 
 	CalcFirstRound(); 
}

void CGameTable::MakeDeal(int* pCards, int nLastDealCardIndex, int nInGamePlayerCount)
{
	TCHAR szRoomID[16];
	memset(szRoomID, 0, sizeof(szRoomID));
	_stprintf(szRoomID, _T("%ld"), m_nRoomID);
	
	CString strIniFile = GetINIFileName();
	BOOL bMakeDeal = GetPrivateProfileInt(
		_T("MakeDeal"),			// section name
		szRoomID,				// key name
		0,						// default int
		strIniFile				// initialization file name
		);
	
	if (TRUE == bMakeDeal)
	{
		//获取需要做牌的牌值
		int nNeedMakeDealCardValue = GetPrivateProfileInt(
			_T("NeedMakeDealCardValue"),		// section name
			szRoomID,							// key name
			DEFAULT_NEED_MAKE_DEAL_CARD_VALUE,	// default int
			strIniFile							// initialization file name
			);
		
		if (nNeedMakeDealCardValue<0 || nNeedMakeDealCardValue>12)
			nNeedMakeDealCardValue = DEFAULT_NEED_MAKE_DEAL_CARD_VALUE;

		int nPlayerOrder = 0;//发牌顺序从chairno=0开始
		for (int i=0; i<TOTAL_CHAIRS; i++)
		{
			if (NULL!=m_ptrPlayers[i] && GetPlayerInfo(i)->bInGame)
			{
				if (HANDCARD_TYPE_DANZHANG==GetPlayerInfo(i)->nCardType
					&& CalcMaxSingleCardValue(GetPlayerInfo(i)->nChairCards, nNeedMakeDealCardValue)<=nNeedMakeDealCardValue)
				{
					BOOL bMakeDealSucceed = FALSE;
					int nStartIndex = nLastDealCardIndex;
					int nThirdCardIndex = nInGamePlayerCount*(CHAIR_CARDS-1)+nPlayerOrder;
					int nCurrentCard = GetPlayerInfo(i)->nChairCards[GetPlayerSameCardIndex(i, pCards[nThirdCardIndex])];
					do 
					{
						GetPlayerInfo(i)->nChairCards[GetPlayerSameCardIndex(i, pCards[nThirdCardIndex])] = pCards[nStartIndex];
						CalcBeginPlay(i);
						if ((HANDCARD_TYPE_DANZHANG==GetPlayerInfo(i)->nCardType && CalcMaxSingleCardValue(GetPlayerInfo(i)->nChairCards, nNeedMakeDealCardValue)>nNeedMakeDealCardValue)
							|| (HANDCARD_TYPE_DANZHANG!=GetPlayerInfo(i)->nCardType))
						{//找到合适的牌后，交换牌的位置
							int nSameCardIndex = GetPlayerSameCardIndex(i, pCards[nStartIndex]);
							pCards[nThirdCardIndex] = GetPlayerInfo(i)->nChairCards[GetPlayerSameCardIndex(i, pCards[nStartIndex])];
							pCards[nStartIndex] = nCurrentCard;
							bMakeDealSucceed = TRUE;
						}
						else
						{//不合适的牌需要继续往下找
							GetPlayerInfo(i)->nChairCards[GetPlayerSameCardIndex(i, pCards[nStartIndex])] = nCurrentCard;
							nStartIndex++;
						}

						if (nStartIndex >= TOTAL_CARDS)
							break;//剩余的牌都不合适的时候，不做牌
					} while (FALSE == bMakeDealSucceed);
				}
				
				nPlayerOrder++;
			}
		}
	}
}

int	CGameTable::GetPlayerSameCardIndex(int nChairNo, int nCardID)
{
	for (int i=0; i<CHAIR_CARDS; i++)
	{
		if (GetPlayerInfo(nChairNo)->nChairCards[i] == nCardID)
			return i;//找出跟参数nCardID一样的牌再player中的index
	}

	//UwlLogFile(_T("GetSameCardIndex failed."));
	return -1;
}

BOOL CGameTable::CheckCards(int* pCards)
{
	int	nCardsLay[MAX_CARDS_LAYOUT_NUM];	// 
	ZeroMemory(nCardsLay, sizeof(nCardsLay));
	
	GAME_PUBLIC_INFO* pPublicInfo=GetPublicInfo();
	for(int i=0;i<TOTAL_CARDS;i++)
	{
		nCardsLay[pCards[i]%TOTAL_CARDS]++;
	}
	
	for (i=0;i<TOTAL_CARDS;i++)
	{
		if(nCardsLay[i] != 1){
			return FALSE;
		}
	}
	return TRUE;
}

void CGameTable::CalcBeginPlay(int chairno)
{
	GAME_PLAYER_INFO* player=&m_GameTalbeInfo->m_Player[chairno];
	int* cardids=player->nChairCards;
	
	int shape[CHAIR_CARDS]={cardids[0]/13,cardids[1]/13,cardids[2]/13};
	int value[CHAIR_CARDS]={cardids[0]%13,cardids[1]%13,cardids[2]%13};
	
	xyReversalMoreByValue(cardids,value,CHAIR_CARDS);
	xyReversalMoreByValue(shape,value,CHAIR_CARDS);
	xyReversalMore(value,CHAIR_CARDS);

	int nShun=IsShun(value);
	int nDuizi=IsDuiZi(value);

	//判断是不是豹子
	if(value[0]==value[1]&&value[0]==value[2])
	{
		player->nCardType=HANDCARD_TYPE_BAOZI;
		player->nCardsWeight=HANDCARD_TYPE_BAOZI*1000000+value[0]*10000;
	}
	else if(shape[0]==shape[1]&&shape[0]==shape[2])//同花
	{
		if (nShun>=0)
		{
			player->nCardType=HANDCARD_TYPE_SHUNJIN;
			player->nCardsWeight=HANDCARD_TYPE_SHUNJIN*1000000+nShun*10000;
		}
		else 
		{
			player->nCardType=HANDCARD_TYPE_JINHUA;
			player->nCardsWeight=HANDCARD_TYPE_JINHUA*1000000+value[0]*10000+value[1]*100+value[2];
		}
		
	}
	else if(nShun>=0)//顺子
	{
        player->nCardType=HANDCARD_TYPE_SHUNZI;
		player->nCardsWeight=HANDCARD_TYPE_SHUNZI*1000000+nShun*10000;
	}
	else if (nDuizi>=0)
	{
		//找出单张
		int s=value[0];
		if (value[0]==value[1])
			s=value[2];
		else if (value[0]==value[2])
			s=value[1];
		else
			s=value[0];
		player->nCardType=HANDCARD_TYPE_DUIZI;
		player->nCardsWeight=HANDCARD_TYPE_DUIZI*1000000+nDuizi*10000+s;
	}
	else if (Is235(value,shape))
	{
		player->nCardType=HANDCARD_TYPE_235;
		player->nCardsWeight=HANDCARD_TYPE_235*1000000;
	}
	else 
	{
		player->nCardType=HANDCARD_TYPE_DANZHANG;
		player->nCardsWeight=HANDCARD_TYPE_DANZHANG*1000000+value[0]*10000+value[1]*100+value[2];
	}
}

BOOL CGameTable::ReadCardsFromFile()
{
	BOOL bRead=TRUE;
	TCHAR szKey[32];
	TCHAR szCards[TOTAL_CHAIRS+1][256];
	memset(szKey,0,sizeof(szKey));
	memset(szCards,0,sizeof(szCards));
	///////////////////////////////////////////////
	for(int i=0; i<TOTAL_CHAIRS; i++)
	{
		sprintf(szKey,_T("Chair%d"),i);
		bRead = GetPrivateProfileString ( _T("StartDeal"),szKey, _T(""),szCards[i], 256, GetINIFileName()) ; 
		
		if (bRead)
		{
			TCHAR *fields[128];
			TCHAR *p1, *p2;
			p1=szCards[i];
			int nCount=RetrieveFields ( p1, fields, 60, &p2 ) ;
			
			for(int x=0; x<nCount; x++)
			{
				GetPlayerInfo(i)->nChairCards[x]=atoi(fields[x]);
			}
		}
	}

	return bRead;
}

int CGameTable::RetrieveFields ( TCHAR *buf, TCHAR **fields, int maxfields, TCHAR**buf2 )
{
	if ( buf == NULL )
		return 0;
	
	TCHAR *p;
	p = buf;
	int count = 0;
	
	try{
		while ( 1 ) {
			fields [ count ++ ] = p;
			while ( *p != '|' && *p != '\0' ) p++;
			if ( *p == '\0' || count >= maxfields ) 
				break;
			*p = '\0';
			p++;
		}
	}catch(...)
	{
		buf2=NULL;
		return 0;
	}
	
	if ( *p == '\0' ) 
		*buf2 = NULL;
	else
		*buf2 = p+1;
	*p = '\0';
	
	return count;
}

CString CGameTable::GetINIFileName()
{
	CString sRet;
	TCHAR szFilePath[MAX_PATH];
	GetModuleFileName(NULL,szFilePath,MAX_PATH);
	
	*strrchr(szFilePath,'\\')=0;	
	strcat(szFilePath, "\\");
	strcat(szFilePath,PRODUCT_NAME);
	strcat(szFilePath,".ini");
	sRet.Format(_T("%s"),szFilePath);
	return sRet;
}

void CGameTable::FillupNextBoutInfo(void* pData, int nLen, int chairno)
{
	CTable::FillupNextBoutInfo(pData,nLen,chairno);
}	

void CGameTable::AddChairUsedDeposit(int chairno,int nAddDeposit)
{
   GetPlayerInfo(chairno)->nUsedDeposit += nAddDeposit;
   GetPublicInfo()->nTotalDeposit += nAddDeposit;
}

void CGameTable::SetChairRoundDeposit(int chairno,int nDeposit)
{
 	 GetPlayerInfo(chairno)->nRoundDeposit = nDeposit; 
}

int CGameTable::GetChairRestDeposit(int chairno)
{
	return  GetPlayerInfo(chairno)->nTotalDeposit- GetPlayerInfo(chairno)->nUsedDeposit;
}

int CGameTable::GetChairRoundDeposit(int chairno)
{
	return  GetPlayerInfo(chairno)->nRoundDeposit;
}  

void CGameTable::SetChairStatus(int chairno,int status)
{
    GetPlayerInfo(chairno)->nPlayerStatus=status;
}

int CGameTable::GetChairStatus(int chairno)
{
	if (!GetPlayerInfo(chairno)->bInGame)
	{
		return PLAYER_STATUS_GIVE_UP;
	} 
    return  GetPlayerInfo(chairno)->nPlayerStatus;
} 

int CGameTable::GetRoundMinDeposit()
{
	return GetPublicInfo()->nRoundMinDeposit;
}
 
void CGameTable::CalcFirstRound()
{
    GAME_PUBLIC_INFO *pPubInfo= GetPublicInfo();  
	pPubInfo->nBanker=m_nBanker;

	AddStatus(TS_WAITING_OPERATION);   
	
	for (int i=0;i<TOTAL_CHAIRS;i++)
	{
		if (GetPlayerInfo(i)->bInGame)
		{
			AddChairUsedDeposit(i,pPubInfo->nRoundMinDeposit);
			SetChairRoundDeposit(i,pPubInfo->nRoundMinDeposit);
			SetChairStatus(i,PLAYER_STATUS_WAIT_BET);
		}
	}

	int nCurChair=GetNextChair(m_nBanker);

	SetCurrentChair(nCurChair,GetStartData()->nBetWaitTime);
    CalcPlayerCouldOpe(m_nCurrentChair);
}

DWORD CGameTable::CalcPlayerCouldOpe(int nCurrentChair)
{
	DWORD dwCouldOpe = 0; 
    int chairStatus = GetChairStatus(nCurrentChair);  
	int nRestDeposit = GetChairRestDeposit(nCurrentChair); 
	if (nRestDeposit==0 || chairStatus==PLAYER_STATUS_GIVE_UP) //什么也不能做
	{
		ASSERT(FALSE);
		return dwCouldOpe;
	}

  	dwCouldOpe |= GAME_OPE_GIVE_UP;

	//看牌
	if (FALSE == GetPublicInfo()->bnLookCard[nCurrentChair])
		dwCouldOpe |= GAME_OPE_LOOK_CARD;

	int nLookCardTime = GetPublicInfo()->bnLookCard[nCurrentChair]?2:1;//明暗牌倍数
	int nRestSingleBet = MAX_BET_PER_BOUT - GetPublicInfo()->nCurrentBet[nCurrentChair];
	int nCurSigleBet = GetPublicInfo()->nCurrentDouble * nLookCardTime;

	//统计剩余未弃牌和押满的玩家数
	int nAliveCount = 0;
	int nBetFullCount = 0;
	int nRestPlayerChair1No = -1;
	int nRestPlayerChair2No = -1;
	for (int i = 0; i < TOTAL_CHAIRS; i++)
	{
		if (GetPlayerInfo(i)->bInGame && GetPlayerInfo(i)->nPlayerStatus != PLAYER_STATUS_GIVE_UP)
		{
			nAliveCount++;
			if ((1 == nAliveCount && i == nCurrentChair)
				|| (2 == nAliveCount && i == nCurrentChair))
				nRestPlayerChair1No = i;
			else
				nRestPlayerChair2No = i;
			
			if (PLAYER_STATUS_BET_FULL == GetPlayerInfo(i)->nPlayerStatus)
				nBetFullCount++;
		}
	}

	//比牌
	if (GetPlayerInfo(nCurrentChair)->nUsedDeposit > m_nBaseDeposit)
	{
		if (nRestDeposit >= (nCurSigleBet*m_nBaseDeposit*2))
			dwCouldOpe |= GAME_OPE_COMPARE;
	}

	//跟注
	if (GetPublicInfo()->nCurrentDouble>0 && nRestSingleBet>nCurSigleBet 
		&& nRestDeposit>(nCurSigleBet*m_nBaseDeposit))
	{
		dwCouldOpe |= GAME_OPE_FOLLOW;
	}

	BOOL bRise = FALSE;
	if (0 == GetPublicInfo()->nCurrentDouble)
	{//下注
		bRise = TRUE;
		dwCouldOpe |= GAME_OPE_BET;
	}
	else if (GetPublicInfo()->nCurrentDouble < BET_EIGHT && GetPublicInfo()->nCurrentDouble >= BET_ONE)
	{//加注
		bRise = TRUE;
		dwCouldOpe |= GAME_OPE_RISE;
	}

	if (TRUE == bRise)
	{
		if (nRestDeposit >= (nLookCardTime*m_nBaseDeposit*BET_EIGHT))
		{
			dwCouldOpe |= (GetPublicInfo()->nCurrentDouble<BET_TWO)?GAME_OPE_BET_TWO:0;
			dwCouldOpe |= (GetPublicInfo()->nCurrentDouble<BET_FOUR)?GAME_OPE_BET_FOUR:0;
			dwCouldOpe |= (GetPublicInfo()->nCurrentDouble<BET_EIGHT)?GAME_OPE_BET_EIGHT:0;
		}
		else if (nRestDeposit < (nLookCardTime*m_nBaseDeposit*BET_EIGHT) && nRestDeposit >= (nLookCardTime*m_nBaseDeposit*BET_FOUR))
		{
			dwCouldOpe |= (GetPublicInfo()->nCurrentDouble<BET_TWO)?GAME_OPE_BET_TWO:0;
			dwCouldOpe |= (GetPublicInfo()->nCurrentDouble<BET_FOUR)?GAME_OPE_BET_FOUR:0;
		}
		else if (nRestDeposit < (nLookCardTime*m_nBaseDeposit*BET_FOUR) && nRestDeposit >= (nLookCardTime*m_nBaseDeposit*BET_TWO))
		{
			dwCouldOpe |= (GetPublicInfo()->nCurrentDouble<BET_TWO)?GAME_OPE_BET_TWO:0;
		}
	}

	if (nRestDeposit <= (nCurSigleBet*m_nBaseDeposit))
	{
		dwCouldOpe |= GAME_OPE_ALLIN;
	}

	if (TRUE==GetPublicInfo()->bOpenBetFull && nRestDeposit>(nCurSigleBet*m_nBaseDeposit) 
		&& 2==nAliveCount && (0==nBetFullCount || 1==nBetFullCount) && GetPlayerInfo(nCurrentChair)->nUsedDeposit>m_nBaseDeposit)
	{
		dwCouldOpe |= GAME_OPE_BET_FULL;
		if (1 == nBetFullCount)
		{//已经有人押满，则第二个只能进行看牌、弃牌和押满操作
			dwCouldOpe &= ~(GAME_OPE_COMPARE|GAME_OPE_BET|GAME_OPE_RISE|GAME_OPE_BET_TWO|GAME_OPE_BET_FOUR|GAME_OPE_BET_EIGHT|GAME_OPE_FOLLOW|GAME_OPE_ALLIN);
		}
	}

	if (1 == nAliveCount)
		dwCouldOpe = 0;
   
	GetPublicInfo()->dwCouldOpe=dwCouldOpe;
	return dwCouldOpe;	
}

BOOL  CGameTable::IsRoundNeedOperation()
{
	int nAliveCount=0;
	for (int i=0;i<TOTAL_CHAIRS;i++)
	{ 
		int playerStatus=GetChairStatus(i);
		if (PLAYER_STATUS_ALLIN==playerStatus || PLAYER_STATUS_GIVE_UP==playerStatus)continue;
		nAliveCount++;
	}
	if (nAliveCount<=1)
	{
		return FALSE;
	}
	return TRUE	;
}

int CGameTable::GetAliveUserCount()
{
	int nAliveCount = 0;
	for (int i=0; i<TOTAL_CHAIRS; i++)
	{ 
		int playerStatus=GetChairStatus(i);
		if (PLAYER_STATUS_GIVE_UP==playerStatus)continue;
		nAliveCount++;
	}
	return nAliveCount;
}

int CGameTable::GetNextOperatorChair(int nFromChair)
{
	int nCurChair=GetNextChair(nFromChair);
	while (1)
	{
		int playerStatus=GetChairStatus(nCurChair); 
		
		if (PLAYER_STATUS_BET_FULL==playerStatus || PLAYER_STATUS_GIVE_UP==playerStatus) 
		{
			nCurChair=GetNextChair(nCurChair); 
			if(nCurChair==nFromChair)
			{
				return nFromChair;
			}
			continue;
		}
		return nCurChair;
		
	}
	   return nCurChair;
}

BOOL CGameTable::OnShowCard(LPSHOW_CARD_REQUEST pShowCard, LPSHOW_CARD_RESPONSE pShowCardResponse)
{
	if (IS_BIT_SET(m_dwStatus,TS_PLAYING_GAME))
		return FALSE  ;

	int nChairNo = pShowCard->nChairNo;
	if (!ValidateChair(nChairNo))
		return FALSE;

	if (!m_GameTalbeInfo->m_Player[nChairNo].bInGame)
		return FALSE;

	memcpy(pShowCardResponse->nChairCards, m_GameTalbeInfo->m_Player[nChairNo].nChairCards, sizeof(int)*CHAIR_CARDS);
	return TRUE;
}

int CGameTable::GetValidChairNO()
{
	for (int i=0;i<TOTAL_CHAIRS;i++)
	{
		if(	!GetPlayerInfo(i)->bInGame)continue;
		if(GetChairStatus(i)==PLAYER_STATUS_GIVE_UP)continue;
		return i;
	}  
	return 0;
}

int CGameTable::SetCurrentChairOnStart()
{
	SetCurrentChair(m_nCurrentChair,GetStartData()->nBetWaitTime);
    return GetCurrentChair();
}

DWORD CGameTable::SetStatusOnStart()
{
	return SetStatus(TS_PLAYING_GAME|TS_WAITING_OPERATION);
	return AddStatus(TS_PLAYING_GAME|TS_WAITING_OPERATION);
}

int CGameTable::SetCurrentChair(int chairno,int nWaitSecond)
{
    CTable::SetCurrentChair(chairno,nWaitSecond); 
 	GetPublicInfo()->nCurrentChair=m_nCurrentChair;
	return chairno;
}

DWORD CGameTable::SetStatus(DWORD bits)
{
	m_dwStatusBegin = GetTickCount();
	m_dwStatus = bits;
	GetPublicInfo()->dwStatus= m_dwStatus;
	return m_dwStatus;
}

int CGameTable::GetNextChair(int chairno)
{
	while (1)
	{
		chairno=(chairno + m_nTotalChairs - 1) % m_nTotalChairs;
		CPlayer*pPlayer=m_ptrPlayers[chairno];
		if (pPlayer&&!pPlayer->m_bIdlePlayer)//空闲玩家判断
		{
			return chairno;
		}
	}
}

int CGameTable::GetPrevChair(int chairno)
{
	while (1)
	{
		chairno=(chairno + 1) % m_nTotalChairs;
		CPlayer*pPlayer=m_ptrPlayers[chairno];
		if (pPlayer&&!pPlayer->m_bIdlePlayer)//空闲玩家判断
		{
			return chairno;
		}
	}
}

int CGameTable::GetBaseDeposit(int deposit_mult)
{

	int 	result = CTable::GetBaseDeposit(deposit_mult);
	// 指定基础银
// 	if(m_nBaseSilver){
// 		result = m_nBaseSilver;
// 	} 
// 	if(FEE_MODE_TEA == m_nFeeMode )
// 	{  
// 		// 指定基础银
// 		if(m_nBaseSilver){
// 			result = m_nBaseSilver;
// 		} 
// 	}
   if (result<2)
   {
	   result =1;
   }
	return result;
}

int CGameTable::PlayerLeave(int nUserid,BOOL bDeletePtr)
{
	CPlayer* playerPtr=NULL;
	if (m_mapUser.Lookup(nUserid,playerPtr)
		&&playerPtr)
	{
		int chairno=playerPtr->m_nChairNO;
		if (FALSE == playerPtr->m_bLookOn)
			m_ptrPlayers[chairno]=NULL;
		else
			RemoveVisitor(nUserid, FALSE);

		m_mapUser.RemoveKey(nUserid);
		
		//该处修改必须与CTable::ResetMembers一致
		if (FALSE == playerPtr->m_bLookOn)
			ResetPlayer(chairno);

		if (bDeletePtr)
			SAFE_DELETE(playerPtr);
		
		return chairno;
	}
	
	return -1;
}

//reserved: 
int CGameTable::CalcSurplus(int nDepositDiffs[])
{	


//	return 0;
	int surplus = 0;
	
	for(int i = 0; i < m_nTotalChairs; i++){
		surplus += nDepositDiffs[i];
	}

surplus+=m_nDiffDeposit;
	return surplus;
}


int CGameTable::CalcUserBoutEndDiff(int chairno, int &nWinPoint, int& nScoreDiff, int& nDepositDiff)
{
	nWinPoint=-GetPlayerInfo(chairno)->nUsedDeposit;
    nScoreDiff=-1;
    nDepositDiff=nWinPoint;
	return 0;
}


int CGameTable::CompensateDeposits(int nOldDeposits[], int nDepositDiffs[])
{
	return CTable::CompensateDeposits(nOldDeposits,nDepositDiffs);
	return 0;
}

BOOL CGameTable::IsNeedCountdown()
{
	return TRUE;
}

 
int CGameTable::CalcDoubleOfScore(int chairno, int breakchair, int defdouble)
{
	if(chairno == breakchair){  // 逃跑者扣分倍数
		return -defdouble;
	}else{						// 其他人加分倍数
	 
			return 0; // 和逃跑者相同组
	 
	}
}

int CGameTable::BreakDoubleOfDeposit(int defdouble)
{
	return -defdouble;
}

int CGameTable::CalcBreakDeposit(int breakchair, int breakdouble, int& cut)
{	
	int deposit_diff = 0;
	CPlayer* pPlayer = m_ptrPlayers[breakchair];
	
	if (NULL==pPlayer)
		return deposit_diff;
	int  nAllBet=m_GameTalbeInfo->m_Public.nTotalDeposit;
	int nChairAllBet=m_GameTalbeInfo->m_Player[breakchair].nTotalDeposit;
	if (m_nFeeMode==FEE_MODE_SERVICE_FIXED)
	{
		nChairAllBet+=m_nFeeValue;
	}
	int nMaxBet=nAllBet;
	if (nAllBet>nChairAllBet)
	{
		nMaxBet=nChairAllBet;
	}
	cut = nMaxBet * m_nCutRatio / 100;
	return -nMaxBet;
}

int CGameTable::ShareDepositOnBreak(int chairno, int breakchair, int dividend)
{
	if(chairno == breakchair) return 0;
	return 0;
	int chairs = GetTotalChairsNotGroup(m_nPartnerGroup[breakchair]);
	int surplus = dividend % chairs;
	if(0 == surplus){
		return dividend / chairs;
	}else{
		if(chairno == GetNextChairNotGroup(breakchair, m_nPartnerGroup[breakchair])){
			return dividend / chairs + surplus;
		}else{
			return dividend / chairs;
		}
	}
}

int CGameTable::CalcLoseChair(int chairno,int destchair)
{
	//235大于豹子
	if (m_GameTalbeInfo->m_Player[chairno].nCardType==HANDCARD_TYPE_235
		&&m_GameTalbeInfo->m_Player[destchair].nCardType==HANDCARD_TYPE_BAOZI)
	{
		m_GameTalbeInfo->m_Public.nKiller[destchair]=chairno;
		return destchair;
	}
	
	if (m_GameTalbeInfo->m_Player[destchair].nCardType==HANDCARD_TYPE_235
		&&m_GameTalbeInfo->m_Player[chairno].nCardType==HANDCARD_TYPE_BAOZI)
	{
		m_GameTalbeInfo->m_Public.nKiller[chairno]=destchair;
		return chairno;
	}
	
	if (m_GameTalbeInfo->m_Player[chairno].nCardsWeight
		>m_GameTalbeInfo->m_Player[destchair].nCardsWeight)
	{
		m_GameTalbeInfo->m_Public.nKiller[destchair]=chairno;
		return destchair;
	}
	else
	{
		m_GameTalbeInfo->m_Public.nKiller[chairno]=destchair;
		return chairno;
	}
}

int	CGameTable::CalcMaxSingleCardValue(int nChairCards[CHAIR_CARDS], int nDefaultCardValue)
{
	if (NULL == nChairCards)
	{
		UwlLogFile(_T("CalcMaxSingleCardValue failed. nChairCards is NULL."));
		return nDefaultCardValue+1;//出错就返回最小的不需要做牌的牌值
	}

	int nTmpValue = 0;
	int nMaxCardValue = 0;
	for (int i=0; i<CHAIR_CARDS; i++)
	{
		nTmpValue = 0;
		nTmpValue = nChairCards[i]%13;
		if (nTmpValue > nMaxCardValue)
			nMaxCardValue = nTmpValue;
	}

	return nMaxCardValue;
}

BOOL CGameTable::OnGiveUp(LPGIVE_UP_REQUEST pGiveUp, LPGIVE_UP_RESPONSE pGiveUpResponse)
{
	if (GAME_OPE_GIVE_UP!=pGiveUp->dwGameOpe || FALSE==IS_BIT_SET(GetPublicInfo()->dwCouldOpe, pGiveUp->dwGameOpe))
	{
		UwlLogFile("OnGiveUp failed, pGiveUp->dwGameOpe is wrong, line=%d", __LINE__);
		return FALSE;
	}

	int nChairNo = pGiveUp->nChairNo;
	if (FALSE == GetPlayerInfo(nChairNo)->bInGame)
	{
		UwlLogFile("OnGiveUp failed, player is not Playing game, line=%d", __LINE__);
		return FALSE;
	}

	if (PLAYER_STATUS_GIVE_UP == GetChairStatus(nChairNo))
	{
		UwlLogFile("OnGiveUp failed, player has Given up, line=%d", __LINE__);
		return FALSE;
	}

	SetChairStatus(nChairNo, PLAYER_STATUS_GIVE_UP); 

	int nNextChair = GetNextOperatorChair(nChairNo);
	if (GetCurrentChair()==nChairNo && GetAliveUserCount()>=2)
	{//轮到自己操作回合弃牌
		pGiveUpResponse->bNextChair = TRUE;
		pGiveUpResponse->nNextChair = nNextChair;
		
		SetCurrentChair(nNextChair,GetStartData()->nBetWaitTime); 
		pGiveUpResponse->dwNextCouldOpe = CalcPlayerCouldOpe(nNextChair); 
	}
	else
	{//其它玩家操作回合弃牌
		pGiveUpResponse->bNextChair = FALSE;
		pGiveUpResponse->nNextChair = -1;
		pGiveUpResponse->dwNextCouldOpe = 0;
	}

	SetStatus(TS_PLAYING_GAME|TS_WAITING_OPERATION);

	return TRUE;
}

BOOL CGameTable::OnBet(LPBET_REQUEST pBet, LPBET_RESPONSE pBetResponse)
{
	if (GAME_OPE_BET!=pBet->dwGameOpe || FALSE==IS_BIT_SET(GetPublicInfo()->dwCouldOpe, pBet->dwGameOpe))
	{
		UwlLogFile("OnBet failed, pBet->dwGameOpe is wrong, line=%d", __LINE__);
		return FALSE;
	}
	
	int nChairNo = pBet->nChairNo;
	if (GetCurrentChair() != nChairNo)
	{
		UwlLogFile("OnBet failed, nChairNo is not current chair, line=%d", __LINE__);
		return FALSE;
	}

	if (FALSE == GetPlayerInfo(nChairNo)->bInGame)
	{
		UwlLogFile("OnBet failed, player is not Playing game, line=%d", __LINE__);
		return FALSE;
	}
	
	if (PLAYER_STATUS_GIVE_UP == GetChairStatus(nChairNo))
	{
		UwlLogFile("OnBet failed, player has Given up, line=%d", __LINE__);
		return FALSE;
	}

	if (PLAYER_STATUS_BET_FULL == GetChairStatus(nChairNo))
	{
		UwlLogFile("OnBet failed, player has beted full, line=%d", __LINE__);
		return FALSE;
	}

	if (GetPublicInfo()->nCurrentDouble > 0)
	{
		UwlLogFile("下注错误：单注大于0时无法下注,line=%d ",__LINE__);
		return FALSE;
	}

	if (-1 != pBet->nBet)
	{//下注传上来的pBet->nBet是-1
		UwlLogFile("OnBet failed, pBet->nBet is wrong, line=%d",__LINE__);
		return FALSE;
	}
	
	int nLookCardTime = GetPublicInfo()->bnLookCard[nChairNo]?2:1;//明、暗牌倍数
	int nAddDeposit = BET_ONE * m_nBaseDeposit * nLookCardTime;
	int nRestDeposit = GetChairRestDeposit(nChairNo); 
	if (nAddDeposit>nRestDeposit || nAddDeposit<GetPublicInfo()->nRoundMinDeposit)
	{
		UwlLogFile("OnBet failed, nAddBet is wrong. line=%d ",__LINE__);
		return FALSE;
	}
	
	//设置当前单注和可用银两
	GetPublicInfo()->nCurrentDouble = BET_ONE;
	GetPublicInfo()->nCurrentBet[nChairNo] += BET_ONE * nLookCardTime;
	pBetResponse->nBet = BET_ONE * nLookCardTime;
	
	AddChairUsedDeposit(nChairNo,nAddDeposit);  
	SetChairRoundDeposit(nChairNo,nAddDeposit);
	SetChairStatus(nChairNo,PLAYER_STATUS_BET);  

	int nNextChair=GetNextOperatorChair(nChairNo);
	SetCurrentChair(nNextChair,GetStartData()->nBetWaitTime); 
	pBetResponse->nNextChair = nNextChair;
	pBetResponse->dwNextCouldOpe = CalcPlayerCouldOpe(nNextChair); 

	SetStatus(TS_PLAYING_GAME|TS_WAITING_OPERATION);

	return TRUE;
}

BOOL CGameTable::OnRise(LPBET_REQUEST pBet, LPBET_RESPONSE pBetResponse)
{
	if (FALSE==IS_BIT_SET(GetPublicInfo()->dwCouldOpe, pBet->dwGameOpe))
	{
		UwlLogFile("OnRise failed, pBet->dwGameOpe is wrong, line=%d", __LINE__);
		return FALSE;
	}
	
	int nChairNo = pBet->nChairNo;
	if (GetCurrentChair() != nChairNo)
	{
		UwlLogFile("OnRise failed, nChairNo is not current chair, line=%d", __LINE__);
		return FALSE;
	}

	if (FALSE == GetPlayerInfo(nChairNo)->bInGame)
	{
		UwlLogFile("OnRise failed, player is not Playing game, line=%d", __LINE__);
		return FALSE;
	}
	
	if (PLAYER_STATUS_GIVE_UP == GetChairStatus(nChairNo))
	{
		UwlLogFile("OnRise failed, player has Given up, line=%d", __LINE__);
		return FALSE;
	}

	if (PLAYER_STATUS_BET_FULL == GetChairStatus(nChairNo))
	{
		UwlLogFile("OnRise failed, player has beted full, line=%d", __LINE__);
		return FALSE;
	}

	if (-1 == pBet->nBet)
	{
		UwlLogFile("加注错误：pBet->nBet is -1,line=%d ",__LINE__);
		return FALSE;
	}
	
	if (GetPublicInfo()->nCurrentDouble == BET_EIGHT)
	{
		UwlLogFile("加注错误：单注已满，无法加注,line=%d ",__LINE__);
		return FALSE;
	}
	
	if (GetPublicInfo()->nCurrentDouble == pBet->nBet)
	{
		UwlLogFile("加注错误：相同倍数，无法加注,line=%d ",__LINE__);
		return FALSE;
	}
	
	int nLookCardTime = GetPublicInfo()->bnLookCard[nChairNo]?2:1;//明、暗牌倍数
	int nAddDeposit = pBet->nBet * m_nBaseDeposit * nLookCardTime;
	int nRestDeposit = GetChairRestDeposit(nChairNo); 
	int nRestBet = nRestDeposit/m_nBaseDeposit;
	int nChairRoundDeposit = GetChairRoundDeposit(nChairNo);
	if (nAddDeposit > nRestDeposit)
	{
		UwlLogFile("OnRise failed, nAddBet > nRestDeposit, line=%d ",__LINE__);
		return FALSE;
	}
	
	//设置当前单注和可用银两
	GetPublicInfo()->nCurrentDouble = pBet->nBet;
	GetPublicInfo()->nCurrentBet[nChairNo] += pBet->nBet * nLookCardTime;

	pBetResponse->nBet = pBet->nBet * nLookCardTime;
	
	AddChairUsedDeposit(nChairNo,nAddDeposit);
	SetChairRoundDeposit(nChairNo,nAddDeposit);
	SetChairStatus(nChairNo,PLAYER_STATUS_RISE); 
	
	int nNextChair=GetNextOperatorChair(nChairNo);
	SetCurrentChair(nNextChair,GetStartData()->nBetWaitTime); 
	pBetResponse->nNextChair = nNextChair;
	pBetResponse->dwNextCouldOpe = CalcPlayerCouldOpe(nNextChair); 
	
	SetStatus(TS_PLAYING_GAME|TS_WAITING_OPERATION);
	
	return TRUE;
}

BOOL CGameTable::OnFollowBet(LPFOLLOW_BET_REQUEST pFollowBet, LPFOLLOW_BET_RESPONSE pFollowBetResponse)
{
	if (GAME_OPE_FOLLOW!=pFollowBet->dwGameOpe || FALSE==IS_BIT_SET(GetPublicInfo()->dwCouldOpe, pFollowBet->dwGameOpe))
	{
		UwlLogFile("OnFollowBet failed, pBet->dwGameOpe is wrong, line=%d", __LINE__);
		return FALSE;
	}
	
	int nChairNo = pFollowBet->nChairNo;
	if (GetCurrentChair() != nChairNo)
	{
		UwlLogFile("OnFollowBet failed, nChairNo is not current chair, line=%d", __LINE__);
		return FALSE;
	}

	if (FALSE == GetPlayerInfo(nChairNo)->bInGame)
	{
		UwlLogFile("OnFollowBet failed, player is not Playing game, line=%d", __LINE__);
		return FALSE;
	}
	
	if (PLAYER_STATUS_GIVE_UP == GetChairStatus(nChairNo))
	{
		UwlLogFile("OnFollowBet failed, player has Given up, line=%d", __LINE__);
		return FALSE;
	}

	if (PLAYER_STATUS_BET_FULL == GetChairStatus(nChairNo))
	{
		UwlLogFile("OnFollowBet failed, player has beted full, line=%d", __LINE__);
		return FALSE;
	}

	int nLookCardTime = GetPublicInfo()->bnLookCard[nChairNo]?2:1;//明、暗牌倍数
	int nAddDeoposit= GetPublicInfo()->nCurrentDouble * nLookCardTime * m_nBaseDeposit;
	int nRestDeposit = GetChairRestDeposit(nChairNo); 
	int nRestBet = nRestDeposit/m_nBaseDeposit;
	if (nAddDeoposit > nRestDeposit)
	{
		UwlLogFile("OnFollowBet failed, nAddDeoposit > nRestDeposit, line=%d ",__LINE__);
		return FALSE;
	}
	
	//设置可用银两
	GetPublicInfo()->nCurrentBet[nChairNo] += GetPublicInfo()->nCurrentDouble * nLookCardTime;
	pFollowBetResponse->nBet = GetPublicInfo()->nCurrentDouble * nLookCardTime;
	
	AddChairUsedDeposit(nChairNo,nAddDeoposit);
	SetChairRoundDeposit(nChairNo,nAddDeoposit);
	SetChairStatus(nChairNo,PLAYER_STATUS_FOLLOW); 

	int nNextChair=GetNextOperatorChair(nChairNo);
	SetCurrentChair(nNextChair,GetStartData()->nBetWaitTime); 
	pFollowBetResponse->nNextChair = nNextChair;
	pFollowBetResponse->dwNextCouldOpe = CalcPlayerCouldOpe(nNextChair); 
	
	SetStatus(TS_PLAYING_GAME|TS_WAITING_OPERATION);

	return TRUE;
}

BOOL CGameTable::OnCompare(LPCOMPARE_CARD_REQUEST pCompare, LPCOMPARE_CARD_RESPONSE pCompareResponse)
{
	if (GAME_OPE_COMPARE!=pCompare->dwGameOpe || FALSE==IS_BIT_SET(GetPublicInfo()->dwCouldOpe, pCompare->dwGameOpe))
	{
		UwlLogFile("OnCompare failed, pBet->dwGameOpe is wrong, line=%d", __LINE__);
		return FALSE;
	}
	
	int nChairNo = pCompare->nChairNo;
	if (GetCurrentChair() != nChairNo)
	{
		UwlLogFile("OnCompare failed, nChairNo is not current chair, line=%d", __LINE__);
		return FALSE;
	}

	if (GetPlayerInfo(nChairNo)->nUsedDeposit <= m_nBaseDeposit)
	{
		UwlLogFile("OnCompare failed, nChairNo has not beted, can not compare card, line=%d", __LINE__);
		return FALSE;
	}
	
	if (FALSE==GetPlayerInfo(nChairNo)->bInGame || FALSE==GetPlayerInfo(pCompare->nGoalChairNo)->bInGame)
	{
		UwlLogFile("OnCompare failed, player is not Playing game, line=%d", __LINE__);
		return FALSE;
	}
	
	if (PLAYER_STATUS_GIVE_UP==GetChairStatus(nChairNo) || PLAYER_STATUS_GIVE_UP==GetChairStatus(pCompare->nGoalChairNo))
	{
		UwlLogFile("OnCompare failed, player has Given up, line=%d", __LINE__);
		return FALSE;
	}

	if (PLAYER_STATUS_BET_FULL == GetChairStatus(nChairNo))
	{
		UwlLogFile("OnCompare failed, player has beted full, line=%d", __LINE__);
		return FALSE;
	}

	//比牌过程
	pCompareResponse->nLostChairNo = CalcLoseChair(nChairNo, pCompare->nGoalChairNo);
	
	int nLookCardTime = GetPublicInfo()->bnLookCard[nChairNo]?2:1;//明、暗牌倍数
	int nCompareTime = GetPublicInfo()->nCurrentDouble * nLookCardTime * 2;//比牌2倍于跟注
	int nAddDeposit = nCompareTime * m_nBaseDeposit;
	int nRestDeposit = GetChairRestDeposit(nChairNo); 
	int nRestBet = nRestDeposit/m_nBaseDeposit;

	if (nAddDeposit > nRestDeposit)
	{//剩余可用银两<2倍单注，不够比牌
		UwlLogFile("OnCompare failed, nAddDeoposit < nRestDeposit, line=%d ",__LINE__);
		return FALSE;
	}
	
	if (nCompareTime <= MAX_BET_PER_BOUT - GetPublicInfo()->nCurrentBet[nChairNo])
	{
		GetPublicInfo()->nCurrentBet[nChairNo] += nCompareTime;
		pCompareResponse->nBet = nCompareTime;
		
		AddChairUsedDeposit(nChairNo,nAddDeposit);
		SetChairRoundDeposit(nChairNo,GetPlayerInfo(nChairNo)->nUsedDeposit);
	}
	else
	{
		GetPublicInfo()->nCurrentBet[nChairNo] = MAX_BET_PER_BOUT;
		nCompareTime = MAX_BET_PER_BOUT - GetPublicInfo()->nCurrentBet[nChairNo];
		nAddDeposit = nCompareTime * m_nBaseDeposit;
		pCompareResponse->nBet = nCompareTime;
		
		AddChairUsedDeposit(nChairNo,nAddDeposit);
		SetChairRoundDeposit(nChairNo,nAddDeposit);
	}
	
	SetChairStatus(pCompareResponse->nLostChairNo, PLAYER_STATUS_GIVE_UP); 
	
	int nAliveCount = 0;
	for (int i = 0; i < TOTAL_CHAIRS; i++)
	{
		if (GetPlayerInfo(i)->bInGame && GetPlayerInfo(i)->nPlayerStatus != PLAYER_STATUS_GIVE_UP)
			nAliveCount++;
	}
	
	if (1 == nAliveCount)
	{//只剩下一个玩家，无后续操作回合
		pCompareResponse->bNextChair = FALSE;
		pCompareResponse->nNextChair = -1;
		pCompareResponse->dwNextCouldOpe = 0;
	}
	else
	{
		int nNextChair = GetNextOperatorChair(nChairNo);
		SetCurrentChair(nNextChair,GetStartData()->nBetWaitTime); 
		DWORD dwCouldOpe = CalcPlayerCouldOpe(nNextChair); 
		pCompareResponse->bNextChair=TRUE;
		pCompareResponse->nNextChair=nNextChair;
		pCompareResponse->dwNextCouldOpe=dwCouldOpe; 
	}
	
	SetStatus(TS_PLAYING_GAME|TS_WAITING_OPERATION);

	return TRUE;
}

BOOL CGameTable::OnAllIn(LPALL_IN_REQUEST pAllIn, LPALL_IN_RESPONSE pAllInResponse)
{
	if (GAME_OPE_ALLIN!=pAllIn->dwGameOpe || FALSE==IS_BIT_SET(GetPublicInfo()->dwCouldOpe, pAllIn->dwGameOpe))
	{
		UwlLogFile("OnAllIn failed, pBet->dwGameOpe is wrong, line=%d", __LINE__);
		return FALSE;
	}
	
	int nChairNo = pAllIn->nChairNo;
	if (GetCurrentChair() != nChairNo)
	{
		UwlLogFile("OnAllIn failed, nChairNo is not current chair, line=%d", __LINE__);
		return FALSE;
	}
	
	if (FALSE == GetPlayerInfo(nChairNo)->bInGame)
	{
		UwlLogFile("OnAllIn failed, player is not Playing game, line=%d", __LINE__);
		return FALSE;
	}
	
	if (PLAYER_STATUS_GIVE_UP == GetChairStatus(nChairNo))
	{
		UwlLogFile("OnAllIn failed, player has Given up, line=%d", __LINE__);
		return FALSE;
	}

	if (PLAYER_STATUS_BET_FULL == GetChairStatus(nChairNo))
	{
		UwlLogFile("OnAllIn failed, player has beted full, line=%d", __LINE__);
		return FALSE;
	}

	int nLookCardTime = GetPublicInfo()->bnLookCard[nChairNo]?2:1;//明、暗牌倍数
	int nAddDeposit = GetPublicInfo()->nCurrentDouble * nLookCardTime * m_nBaseDeposit;
	int nRestDeposit = GetChairRestDeposit(nChairNo); 
	int nRestBet = nRestDeposit/m_nBaseDeposit;
	if (nAddDeposit < nRestDeposit)
	{//剩余可用银两比1倍的单注银两还多，不允许全下
		UwlLogFile("OnAllIn failed, nAddDeoposit < nRestDeposit, line=%d ",__LINE__);
		return FALSE;
	}
	
	//比牌过程
	int nNextChair = nChairNo;
	for (int i=0; i<TOTAL_CHAIRS; i++)
	{
		nNextChair = GetNextChair(nNextChair);
		if (GetPlayerInfo(nNextChair)->bInGame && PLAYER_STATUS_GIVE_UP!=GetPlayerInfo(nNextChair)->nPlayerStatus)
		{
			if(nChairNo != nNextChair)
			{
				if (nChairNo == CalcLoseChair(nChairNo, nNextChair))
				{//比输了
					pAllInResponse->bNeedCompare[nNextChair] = TRUE;
					pAllInResponse->bWinOrLose[nNextChair] = FALSE;
					SetChairStatus(nChairNo, PLAYER_STATUS_GIVE_UP); 
					break;
				}
				else
				{//比赢了
					pAllInResponse->bNeedCompare[nNextChair] = TRUE;
					pAllInResponse->bWinOrLose[nNextChair] = TRUE;
					SetChairStatus(nNextChair, PLAYER_STATUS_GIVE_UP); 
				}
			}
			else
			{
				break;
			}
		}
		else
		{//无需比牌
			pAllInResponse->bNeedCompare[nNextChair] = FALSE;
			pAllInResponse->bWinOrLose[nNextChair] = FALSE;
		}
	}
	
	GetPublicInfo()->nCurrentBet[nChairNo] += nRestBet;
	pAllInResponse->nDeposit = nRestDeposit;
	
	AddChairUsedDeposit(nChairNo, nRestDeposit);
	SetChairRoundDeposit(nChairNo, nRestDeposit);

	int nAliveCount = 0;
	{
		for (int i = 0; i < TOTAL_CHAIRS; i++)
		{
			if (GetPlayerInfo(i)->bInGame && GetPlayerInfo(i)->nPlayerStatus != PLAYER_STATUS_GIVE_UP)
				nAliveCount++;
		}
	}
	
	if (1 == nAliveCount)
	{//只剩下一个玩家，无后续操作回合
		pAllInResponse->bNextChair = FALSE;
		pAllInResponse->nNextChair = -1;
		pAllInResponse->dwNextCouldOpe = 0;
	}
	else if (nAliveCount > 1)
	{
		int nNextChair = GetNextOperatorChair(nChairNo);
		SetCurrentChair(nNextChair,GetStartData()->nBetWaitTime); 
		DWORD dwCouldOpe = CalcPlayerCouldOpe(nNextChair); 
		pAllInResponse->bNextChair = TRUE;
		pAllInResponse->nNextChair = nNextChair;
		pAllInResponse->dwNextCouldOpe = dwCouldOpe; 
	}
	else
	{
		UwlLogFile("OnAllIn failed, nAliveCount(%ld) is wrong", nAliveCount);
		return FALSE;
	}
	
	SetStatus(TS_PLAYING_GAME|TS_WAITING_OPERATION);
	
	return TRUE;
}

BOOL CGameTable::OnBetFull(LPBET_FULL_REQUEST pBetFull, LPBET_FULL_RESPONSE pBetFullResponse)
{
	if (GAME_OPE_BET_FULL!=pBetFull->dwGameOpe || FALSE==IS_BIT_SET(GetPublicInfo()->dwCouldOpe, pBetFull->dwGameOpe))
	{
		UwlLogFile("OnBetFull failed, pBet->dwGameOpe is wrong, line=%d", __LINE__);
		return FALSE;
	}
	
	int nChairNo = pBetFull->nChairNo;

	if (FALSE == GetPlayerInfo(nChairNo)->bInGame)
	{
		UwlLogFile("OnBetFull failed, player is not Playing game, line=%d", __LINE__);
		return FALSE;
	}

	if (GetCurrentChair() != nChairNo)
	{
		UwlLogFile("OnBetFull failed, nChairNo is not current chair, line=%d", __LINE__);
		return FALSE;
	}
	
	if (PLAYER_STATUS_GIVE_UP == GetChairStatus(nChairNo))
	{
		UwlLogFile("OnBetFull failed, player has Given up, line=%d", __LINE__);
		return FALSE;
	}

	if (PLAYER_STATUS_BET_FULL == GetChairStatus(nChairNo))
	{
		UwlLogFile("OnBetFull failed, player has beted full, line=%d", __LINE__);
		return FALSE;
	}

	//统计剩余未弃牌的玩家数
	int nAliveCount = 0;
	int nBetFullCount = 0;
	int nRestPlayerChair1No = -1;
	int nRestPlayerChair2No = -1;
	for (int i = 0; i < TOTAL_CHAIRS; i++)
	{
		if (GetPlayerInfo(i)->bInGame && GetPlayerInfo(i)->nPlayerStatus != PLAYER_STATUS_GIVE_UP)
		{
			nAliveCount++;
			if ((1 == nAliveCount && i == nChairNo)
				|| (2 == nAliveCount && i == nChairNo))
				nRestPlayerChair1No = i;
			else
				nRestPlayerChair2No = i;

			if (PLAYER_STATUS_BET_FULL == GetPlayerInfo(i)->nPlayerStatus)
				nBetFullCount++;
		}
	}

	if (2 != nAliveCount)
	{
		UwlLogFile("OnBetFull failed, AliveUserCount(%ld) is wrong, line=%d", nAliveCount, __LINE__);
		return FALSE;
	}

	if (0 == nBetFullCount)
	{
		int nRestDeposit1 = GetPlayerInfo(nRestPlayerChair1No)->nTotalDeposit - GetPlayerInfo(nRestPlayerChair1No)->nUsedDeposit;
		int nRestDeposit2 = GetPlayerInfo(nRestPlayerChair2No)->nTotalDeposit - GetPlayerInfo(nRestPlayerChair2No)->nUsedDeposit;

		if (TRUE == GetPublicInfo()->bnLookCard[nRestPlayerChair1No])
			pBetFullResponse->nDeposit = MIN(nRestDeposit1, nRestDeposit2);//明牌
		else
			pBetFullResponse->nDeposit = MIN(nRestDeposit1, nRestDeposit2/2);//暗牌，向下取整

		if (0 == pBetFullResponse->nDeposit)
			pBetFullResponse->nDeposit = 1;//如果算出来的押满银两为0，则取1

		SetChairStatus(nRestPlayerChair1No, PLAYER_STATUS_BET_FULL);
		SetCurrentChair(nRestPlayerChair2No,GetStartData()->nBetWaitTime); 

		DWORD dwCouldOpe = CalcPlayerCouldOpe(nRestPlayerChair2No); 
		pBetFullResponse->bNextChair = TRUE;
		pBetFullResponse->nNextChair = nRestPlayerChair2No;
		pBetFullResponse->dwNextCouldOpe = dwCouldOpe; 
	}
	else if (1 == nBetFullCount)
	{
		if (TRUE == GetPublicInfo()->bnLookCard[nRestPlayerChair2No])
		{//明牌
			if (TRUE == GetPublicInfo()->bnLookCard[nRestPlayerChair1No])
				pBetFullResponse->nDeposit = GetChairRoundDeposit(nRestPlayerChair2No);
			else
				pBetFullResponse->nDeposit = GetChairRoundDeposit(nRestPlayerChair2No)/2;//向下取整

			if (0 == pBetFullResponse->nDeposit)
				pBetFullResponse->nDeposit = 1;//如果算出来的押满银两为0，则取1
		}
		else
		{//暗牌
			if (TRUE == GetPublicInfo()->bnLookCard[nRestPlayerChair1No])
				pBetFullResponse->nDeposit = GetChairRoundDeposit(nRestPlayerChair2No)*2;
			else
				pBetFullResponse->nDeposit = GetChairRoundDeposit(nRestPlayerChair2No);
		}

		//将2个押满玩家进行比牌
		if (nRestPlayerChair1No == CalcLoseChair(nRestPlayerChair1No, nRestPlayerChair2No))
		{//比输了
			pBetFullResponse->bWinOrLose = FALSE;
			SetChairStatus(nRestPlayerChair1No, PLAYER_STATUS_GIVE_UP); 
		}
		else
		{//比赢了
			pBetFullResponse->bWinOrLose = TRUE;
			SetChairStatus(nRestPlayerChair2No, PLAYER_STATUS_GIVE_UP); 
		}

		pBetFullResponse->bNextChair = FALSE;
		pBetFullResponse->nGoalCompareChair = nRestPlayerChair2No;
	}
	else
	{
		UwlLogFile("OnBetFull failed, BetFullUserCount(%ld) is wrong, line=%d", nBetFullCount, __LINE__);
		return FALSE;
	}

	GetPublicInfo()->nCurrentBet[nRestPlayerChair1No] += pBetFullResponse->nDeposit/m_nBaseDeposit;
	
	AddChairUsedDeposit(nRestPlayerChair1No, pBetFullResponse->nDeposit);
	SetChairRoundDeposit(nRestPlayerChair1No, pBetFullResponse->nDeposit);

	SetStatus(TS_PLAYING_GAME|TS_WAITING_OPERATION);
	
	return TRUE;
}