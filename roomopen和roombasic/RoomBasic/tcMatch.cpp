//#include "xygame3.h"
#include "roombasic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CTcMatchPlayer::CTcMatchPlayer(LPMATCH_PLAYER lmp,LPUSERONMGAME lug,LPUSERONMATCH lum,LPMATCH_HINT lmh)
{
	m_nStatus=TCMATCH_STATUS_NOT_READY;
	m_nRoomID=-1;
	m_nTableNO=-1;
	m_nChairNO=-1;
	m_nTotalBout=0;
	m_nLastPlaySecond=0;
	m_nLastSendRankSecond=0;
	m_nMode=-1;
	m_bReady=FALSE;
	m_bActive=FALSE;
	m_bOffline=FALSE;
	m_bQuit=FALSE;
	m_nTotalWait=0;
	m_nParseBout=0;

	memcpy(&match_player,lmp,sizeof(MATCH_PLAYER));
	memcpy(&userongame,lug,sizeof(USERONMGAME));
	memcpy(&useronmatch,lum,sizeof(USERONMATCH));
	memcpy(&match_hint,lmh,sizeof(MATCH_HINT));

	useronmatch.nRank=-1;//初始化名次为-1;
}

CTcMatchPlayer::~CTcMatchPlayer()
{
	

}

unsigned long CTcMatchPlayer::GetWaitSecond()
{
	return UwlGetCurrentSeconds()-m_nLastPlaySecond+m_nTotalWait;
}

BOOL CTcMatchPlayer::IsPlaying()
{
	return m_nStatus==TCMATCH_STATUS_PLAYING;
}

BOOL CTcMatchPlayer::IsQuit()
{
	return m_bQuit;
}

BOOL CTcMatchPlayer::IsWaitNextPhase()
{
	return m_nStatus==TCMATCH_STATUS_WAIT_NEXT_PHASE;
}

BOOL CTcMatchPlayer::IsOffline()
{
	return m_nStatus==TCMATCH_STATUS_OFFLINE;
}

BOOL CTcMatchPlayer::IsReadyMatch()
{
	return m_bReady;
}

void CTcMatchPlayer::SetRank(int nRank)
{
	useronmatch.nRank=nRank;
}

int  CTcMatchPlayer::GetRank()
{
	return useronmatch.nRank;
}

void CTcMatchPlayer::SetScore(int nScore)
{
	useronmatch.nScore=nScore;
}

int  CTcMatchPlayer::GetScore()
{
	return useronmatch.nScore;
}

void CTcMatchPlayer::SetDeposit(int nDeposit)
{
	useronmatch.nDeposit=nDeposit;
}

int  CTcMatchPlayer::GetDeposit()
{
	return useronmatch.nDeposit;
}

int  CTcMatchPlayer::GetUserID()
{
	return match_player.nUserID;
}

CTcMatchTable::CTcMatchTable(CTcMatchBase* pTcMatchBase,int nTableNO)
{
	m_pTcMatchBase=pTcMatchBase;

	m_nTableNO=nTableNO;
	m_nPlayerCount=0;
 	m_nPlanBout=0;
	m_nBout=0;
 	m_nLastPlayStartTime=0;
	m_nLastPlayEndTime=0;
	m_nMaxPlaySecond=600;
	m_nStatus=TCMATCH_TABLE_EMPTY;

    m_nBaseScore=0;
	m_nWashScore=0;
	m_nBanker=0;

	memset(m_playerPtr,0,sizeof(m_playerPtr));
}

CTcMatchTable::CTcMatchTable(int nRoomID,int nTableNO, DWORD dwMatchOptions)
{ 
	m_nTableNO=nTableNO;
	m_nPlayerCount=0;
	m_nPlanBout=0;
	m_nBout=0;
 	m_nLastPlayStartTime=0;
	m_nLastPlayEndTime=0;
	m_nMaxPlaySecond=600;
	m_nStatus=TCMATCH_TABLE_EMPTY;

	m_nBaseScore=0;
	m_nWashScore=0;
	m_nBanker=0;

	memset(m_playerPtr,0,sizeof(m_playerPtr));
}

CTcMatchTable::~CTcMatchTable()
{

}

BOOL	CTcMatchTable::PlayerEnter(CTcMatchPlayer* player)
{
	if (player->m_nStatus!=TCMATCH_STATUS_WAIT)
		return FALSE;

	player->m_nTableNO=m_nTableNO;
	player->m_nRoomID=m_pTcMatchBase->GetRoomID();
	player->m_nStatus=TCMATCH_STATUS_READY_NEXT_BOUT;
	player->m_nTotalWait+=UwlGetCurrentSeconds()-player->m_nLastPlaySecond;
	m_playerPtr[m_nPlayerCount]=player;
	m_nPlayerCount++;

	return TRUE;
}


void    CTcMatchTable::ResetTable(int nStatus)
{
	for(int i=0;i<m_nPlayerCount;i++)
	{
		if (m_playerPtr[i])
		{
			m_playerPtr[i]->m_nMode=i;
			m_playerPtr[i]->m_nTableNO=-1;
			m_playerPtr[i]->m_nChairNO=-1;
			if (m_playerPtr[i]->m_nStatus>=0)//还没被淘汰或者放弃
				m_playerPtr[i]->m_nStatus=nStatus;
		}
	}
	memset(m_playerPtr,0,sizeof(m_playerPtr));

	m_nPlayerCount=0;
	m_nBout=0;
	m_nPlanBout=0;
	m_nStatus=TCMATCH_TABLE_EMPTY;
	m_nBanker=0;
}
DWORD   CTcMatchTable::GetPlaySecond()//游戏进行的秒数
{
	if (m_nStatus!=TCMATCH_TABLE_PLAYING)
		return 0;

	return UwlGetCurrentSeconds()-m_nLastPlayStartTime;
}

DWORD   CTcMatchTable::GetWaitSecond()//获取桌子等待开始的秒数
{
	if (m_nStatus!=TCMATCH_TABLE_WAIT_NEXT_BOUT)
		return 0;

	return UwlGetCurrentSeconds()-m_nLastPlayEndTime;
}

BOOL    CTcMatchTable::IsOverTime()
{
	if (m_nStatus!=TCMATCH_TABLE_PLAYING)
		return FALSE;

	return UwlGetCurrentSeconds()-m_nLastPlayStartTime>m_nMaxPlaySecond;
}

BOOL    CTcMatchTable::GetNextBout(LPARRANGE_TABLE pArrangeTable,int nBaseScore,int nLevelScore,int nMaxSecond)
{
 	int n=0;
	m_nBaseScore=nBaseScore;
	m_nWashScore=nLevelScore;
	m_nMaxPlaySecond=nMaxSecond;

	m_nLastPlayStartTime=UwlGetCurrentSeconds();
	
	DWORD dwConfig=m_pTcMatchBase->GetMatchConfig();

	for(int i=0;i<m_nPlayerCount;i++)
	{
		if (m_playerPtr[i]->m_nStatus==TCMATCH_STATUS_READY_NEXT_BOUT)
			n++;
	}

	if (n!=m_nPlayerCount)
	{
		UwlLogFile("比赛启动游戏桌失败,桌内人数:%ld,需要人数:%d,nTableNO:%ld",n,m_nPlayerCount,m_nTableNO);
		ResetTable(TCMATCH_STATUS_WAIT);//重新回到分桌状态
		return FALSE;
	}

	if (m_nBout==0)
	{
		RandomSortPlayer(m_playerPtr,m_nPlayerCount);
		for(int i=0;i<m_nPlayerCount;i++)
		{
			m_playerPtr[i]->m_nMode=i;//切换模组
		}
	}
	else
	{
		if (IS_BIT_SET(dwConfig,MATCH_CFG_POSITION_TURN) && m_nPlayerCount>2) //每桌人数>2
		{
 			LPCTcMatchPlayer temp=m_playerPtr[1];

			for(int i=1;i<m_nPlayerCount-1;i++)
			{
				m_playerPtr[i]=m_playerPtr[i+1];
			}
			m_playerPtr[m_nPlayerCount-1]=temp;
		}
	}
	
	//决定庄家
	if (IS_BIT_SET(dwConfig,MATCH_CFG_TURN_BANKER))
	{
		if (m_nBout==0)
		{
			m_nBanker=0;
		}
		else
		{
			m_nBanker=GetNextChair(m_nBanker);
		}
	}
	else if (IS_BIT_SET(dwConfig,MATCH_CFG_RANDOM_BANKER))
	{
		srand(GetTickCount());
		m_nBanker=rand()%m_nPlayerCount;
	}
	else if (IS_BIT_SET(dwConfig,MATCH_CFG_DECIDE_BY_GAMESVR))
	{
		
	}
	pArrangeTable->nRoomID=m_pTcMatchBase->GetRoomID();
	pArrangeTable->nTableNO=m_nTableNO;
	pArrangeTable->nChairCount=m_nPlayerCount;
	pArrangeTable->nMatchBaseScore=m_nBaseScore;
	pArrangeTable->nMatchLevelScore=m_nWashScore;
//	XygInitChairCards(pArrangeTable->nUserIDs,MAX_MATCHCHAIR_COUNT);
	for(i=0;i<m_nPlayerCount;i++)
	{
		pArrangeTable->nUserIDs[i]=m_playerPtr[i]->GetUserID();
		pArrangeTable->nMatchScore[i]=m_playerPtr[i]->GetScore();
		pArrangeTable->nMatchDeposite[i]=m_playerPtr[i]->GetDeposit();
		m_playerPtr[i]->m_nParseBout++;
		m_playerPtr[i]->m_nTotalBout++;
		m_playerPtr[i]->m_nStatus=TCMATCH_STATUS_PLAYING;
		m_playerPtr[i]->m_nChairNO=i;
	}
	pArrangeTable->nBout=m_nBout;
	pArrangeTable->nBanker=m_playerPtr[m_nBanker]->GetUserID();
	
	m_nBout++;
	m_nStatus=TCMATCH_TABLE_PLAYING;
	return TRUE;
}

int		CTcMatchTable::GetNextChair(int chairno)
{
	return (chairno + m_nPlayerCount - 1) % m_nPlayerCount;
}

int		CTcMatchTable::GetPrevChair(int chairno)
{
	return (chairno + 1) % m_nPlayerCount;
}


void ReversalMoreByValue(LPCTcMatchPlayer array[],int value[],int length)
{
	
	int i,j,t;
	LPCTcMatchPlayer temp;
	for(i=0;i<length-1;i++) 
	{
		for(j=i+1;j<length;j++) /*注意循环的上下限*/ 
			if(value[i]<value[j]) 
			{ 
				temp=array[i]; 
				array[i]=array[j]; 
				array[j]=temp;
				t=value[i]; 
				value[i]=value[j]; 
				value[j]=t;
			}
	}
}

void  CTcMatchTable::RandomSortPlayer(LPCTcMatchPlayer array[],int length)
{
	srand(GetTickCount());
	int* value=new int[length];
	int s=length*1000;
	for(int i=0;i<length;i++)
	{
		value[i]=rand()%s;
	}
	ReversalMoreByValue(array,value,length);
	delete []value;
}

CTcMatch::CTcMatch(CBaseRoomData* pRoomData,LPMATCH lpMatch):CTcMatchBase(pRoomData,lpMatch)
{
	m_CurrentPhase=NULL;
	m_FirstPhase=NULL;
}



CTcMatch::~CTcMatch()
{
	ClearMatch();
}


void    CTcMatch::BeginMatch()
{
	if (!m_CurrentPhase)
		return;

	if (m_nMatchStatus>=MATCH_STATUS_STARTED)
		return;

	m_nMaxPlaySecond=m_pRoomData->roomdata.nMaxBoutSecond;
	if (m_nMaxPlaySecond==0)
	{
		m_nMaxPlaySecond=TCMATCH_MAX_GAME_PLAY_SECOND;
	}
	int nCurrentSecond=UwlGetCurrentSeconds();


	m_nMatchStatus=MATCH_STATUS_STARTED;

	//清理所有的桌子
	{
		POSITION pos=0;
		int nTableNO=0;
		CTcMatchTable* pTable=NULL;
		while(pos)
		{
			m_mapTable.GetNextAssoc(pos,nTableNO,pTable);
			if (pTable)
			{
				SAFE_DELETE(pTable);
			}
		}
		m_mapTable.RemoveAll();
	}

	POSITION pos = m_mapPlayer.GetStartPosition();
	int userid=0;
	CTcMatchPlayer* pPlayer=NULL;
	while (pos)	{
		userid=0;
		pPlayer=NULL;
		m_mapPlayer.GetNextAssoc(pos, userid, pPlayer);
		if (pPlayer&&pPlayer->IsReadyMatch())
		{
			pPlayer->m_nStatus=TCMATCH_STATUS_WAIT;
			pPlayer->m_bActive=TRUE;
			pPlayer->m_nTotalWait=0;
			pPlayer->m_nLastSendRankSecond=nCurrentSecond;
			pPlayer->m_nLastPlaySecond=nCurrentSecond;
		}
	}

	m_nTableSortCount=0;
	m_nMatchStartSecond=nCurrentSecond;
	m_nWashOutIndex=GetTotalUserCount();

	m_CurrentPhase->BeginPhase();
	while(m_CurrentPhase->m_NextPhase
		&&m_CurrentPhase->m_NextPhase->IsReadyEnterPhase())
	{
		m_CurrentPhase=m_CurrentPhase->m_NextPhase;
		m_CurrentPhase->TransPhase();
		m_CurrentPhase->BeginPhase();
	}

	CalcBaseScore();
    CalcLevelScore();
}

void    CTcMatch::CreatePlayer(LPMATCH_PLAYER lmp,LPUSERONMGAME lug,LPUSERONMATCH lum,LPMATCH_HINT lmh)
{
	if (m_CurrentPhase && !m_CurrentPhase->ValidatePlayerEnter(lmp->nUserID))
		return;//已经开始比赛

	LPCTcMatchPlayer player=new CTcMatchPlayer(lmp,lug,lum,lmh);

	if (m_CurrentPhase)
		m_CurrentPhase->InitialPlayerWhenEnter(player);

	m_mapPlayer.SetAt(lmp->nUserID,player);

	UwlLogFile("CurrentChairEntered:user:%s,TotalUserCount %d",lmp->szUsername,GetTotalUserCount());
}

BOOL    CTcMatch::FreshMatch()
{
	if (m_nMatchStatus==MATCH_STATUS_STARTED
		&&m_bGameSvrConnnet)//计算比赛是否已经结束
	{
		CheckTableStatus();	 
		CheckPlayerStatus();

		if (IsMatchEnd())
			OnMatchEnd();

	
		//判断赛制转换
		TransMatchPhase();

		
		//刷新排名
		//CalcPlayersRank();
		CalcBaseScore();
		CalcLevelScore();

		if (UwlGetCurrentSeconds()-m_nMatchStartSecond>5)
			ArrageTable();
	}

	return TRUE;
}




void SwapInt(int& x,int&y)
{
	int t=x;
	x=y;
	y=t;
}

void  CTcMatch::CreateMatch(LPMATCH_STARTTOROOM pStartMatch)
{
	POSITION pos = m_mapPlayer.GetStartPosition();
	int userid=0;
	CTcMatchPlayer* pPlayer=NULL;
	while (pos)	
	{
		userid=0;
		pPlayer=NULL;
		m_mapPlayer.GetNextAssoc(pos, userid, pPlayer);
		if (pPlayer&&!pPlayer->IsReadyMatch())
		{
			m_mapPlayer.RemoveKey(userid);
		}
	}

	int nPlayerCount=GetTotalUserCount();

	//刷新数据
	m_MatchData.match.nMaxRunTime=pStartMatch->nMaxRunTime;

	if (pStartMatch->nMatchType==MATCH_TYPE_STAND)
	{
   		//打立出局
		PushBackPhase(new CTCMatchPhase(pStartMatch->nBoutPerTurn[0]));
		
		PushBackPhase(new CTCMatchPhase2(pStartMatch->nBoutPerTurn[1],m_nChairPerTable,nPlayerCount));

	//	PushBackPhase(new CTCMatchPhase2(1,m_nChairPerTable,nPlayerCount));
	}
	else
	{
		PushBackPhase(new CTCMatchPhase(pStartMatch->nBoutPerTurn[0]));
	}
}

void  CTcMatch::PushBackPhase(CTCMatchPhase* phase)
{
	phase->SetMatchPtr(this);
	if (!m_FirstPhase)
	{
		m_FirstPhase=phase;
		m_CurrentPhase=m_FirstPhase;
		return;
	}

	CTCMatchPhase* next=m_FirstPhase;
	while(next->m_NextPhase)
	{
		next=next->m_NextPhase;
	}

	next->m_NextPhase=phase;
}

void CTcMatch::CalcBaseScore()
{
	if (m_CurrentPhase)
		m_CurrentPhase->CalcBaseScore();
}

void  CTcMatch::CalcLevelScore()
{
	if (m_CurrentPhase)
		m_CurrentPhase->CalcLevelScore();
}

void  CTcMatch::CheckPlayerStatus()
{
	int nPlayeingTalbeCount=GetPlayingTableCount();
	DWORD nMaxWait=120;//最多等待120秒
	if (nPlayeingTalbeCount==0)//已经无人可以继续
		nMaxWait=10;//只等待10秒

	BOOL bnTransPhase=FALSE;
	if (m_CurrentPhase->m_NextPhase
		&&m_CurrentPhase->m_NextPhase->IsPrepareEnterPhase())
		bnTransPhase=TRUE;

	int nCurrentSecond=UwlGetCurrentSeconds();
	m_nCurrentUserWaitArrage=0;
	POSITION pos = m_mapPlayer.GetStartPosition();
	int userid=0;
	CTcMatchPlayer* pPlayer=NULL;
	while (pos)	{
		userid=0;
		pPlayer=NULL;

		m_mapPlayer.GetNextAssoc(pos, userid, pPlayer);
		if (pPlayer)
		{
			//首先检查该Player是否还在房间内
			LPPLAYER lpRoomPlayer=NULL;
			if (!m_pRoomData->m_mapPlayer.Lookup(pPlayer->GetUserID(),lpRoomPlayer))
			{
				ForceKickPlayer(pPlayer->GetUserID());
				continue;
			}
			
			if (  pPlayer->m_bActive
				&&pPlayer->m_nStatus==TCMATCH_STATUS_OFFLINE
				&&pPlayer->m_bOffline==TRUE
				&&nCurrentSecond-pPlayer->m_nLastPlaySecond>nMaxWait
				&&m_CurrentPhase->AutoKickOffliner())
			{
				pPlayer->SetRank(GetWashOutRank());
				pPlayer->m_nStatus=TCMATCH_STATUS_GIVEUP;
				NotifyMatchFinish(pPlayer);
			}
			else if (pPlayer->m_bActive
				&&pPlayer->m_nStatus==TCMATCH_STATUS_WAIT
				&&bnTransPhase)//转阶段的时候，从等待分桌换成等待进入下一个阶段
			{
				pPlayer->m_nStatus=TCMATCH_STATUS_WAIT_NEXT_PHASE;
				NotifyUserStatus(pPlayer->GetUserID(),TCMATCH_ANIMATCION_WAIT_FINAL,TRUE,GetPlayingTableCount());
			}
			
			if (pPlayer->m_bActive
				&&(pPlayer->m_nStatus==TCMATCH_STATUS_WAIT
				||pPlayer->m_nStatus==TCMATCH_STATUS_WAIT_NEXT_PHASE))
			{
				if (nCurrentSecond-pPlayer->m_nLastSendRankSecond>TCMATCH_FRESHRANKLIST_SECOND)
					NotifyUserRankList(pPlayer->GetUserID());
				else if (pPlayer->m_nLastSendRankSecond==m_nMatchStartSecond
					&&nPlayeingTalbeCount>0)//未能加入第一次分桌
					NotifyUserRankList(pPlayer->GetUserID());

				if (pPlayer->m_nStatus==TCMATCH_STATUS_WAIT)
				{
					if (m_nCurrentUserWaitArrage<nCurrentSecond-pPlayer->m_nLastPlaySecond)
					{
						m_nCurrentUserWaitArrage=nCurrentSecond-pPlayer->m_nLastPlaySecond;
					}
				}
			}
		}
	}
}

void  CTcMatch::CheckTableStatus()
{

	POSITION pos=m_mapTable.GetStartPosition();
	int nTableNO=0;
	CTcMatchTable* pTable=NULL;
	while(pos)
	{
		m_mapTable.GetNextAssoc(pos,nTableNO,pTable);
		if (pTable
			&&pTable->m_nPlayerCount>=m_nChairPerTable)
		{
			if (pTable->m_nStatus==TCMATCH_TABLE_PLAYING)
			{
				//已经开始的桌子检查是否游戏超时
				if (pTable->IsOverTime())
				{
					MATCH_OVERTIME* over=new MATCH_OVERTIME;
					over->nRoomID=GetRoomID();
					over->nMatchID=GetMatchID();
					over->nTableNO=pTable->m_nTableNO;
					PushStep(MATCH_STEP_OVERTIME,over );
				}
			}
			else
			{
				//未开始的桌子检查是否超时未启动
				if (pTable->GetWaitSecond()>=m_nWaitNextBoutSecond)
				{
					//重新激活该桌
					if (pTable&&pTable->m_nStatus!=TCMATCH_TABLE_PLAYING)
					{
						ARRANGE_TABLE* pArrangeTable=new ARRANGE_TABLE;
						memset(pArrangeTable,0,sizeof(ARRANGE_TABLE));
						
						CalcBaseScore();
						CalcLevelScore();
						
						if (pTable->GetNextBout(pArrangeTable,GetBaseScore(),GetWashScore(),m_nMaxPlaySecond))
						{
							pArrangeTable->nMatchMaxScore=m_nUserMaxScore;
							//一局中，所有条件不变
							pArrangeTable->nTableSort=CalcTableSort();
							m_CurrentPhase->FillMatchDetail(pArrangeTable);
							PushStep(MATCH_STEP_ARREAGETABLE,pArrangeTable );
							CreateTableRankList(pTable->m_nTableNO);
						}
						else
						{
							delete pArrangeTable;
						}
					}
				}
			}
		}
	
	}
 
}

void  CTcMatch::ArrageTable()    //分桌
{
	if (m_CurrentPhase)
		m_CurrentPhase->ArrageTable();
}

BOOL   CTcMatch::IsMatchStart()
{
	return (m_nMatchStatus>=MATCH_STATUS_STARTED);
}


BOOL  CTcMatch::IsMatchEnd()
{
	if (GetPlayingTableCount()==0
		&&m_CurrentPhase->m_NextPhase==NULL
		&&m_CurrentPhase->IsPhaseEnd())
		return TRUE;

	if (UwlGetCurrentSeconds()-m_nMatchStartSecond>=m_MatchData.match.nMaxRunTime)
		return TRUE;

	return FALSE;
}

void  CTcMatch::OnMatchEnd()
{
	if (m_nMatchStatus>=MATCH_STATUS_FINISHED)
		return;
	
	m_nMatchStatus=MATCH_STATUS_FINISHED;
	CalcPlayersRank();

	//拆掉左右桌子

	POSITION pos = m_mapPlayer.GetStartPosition();
	int userid=0;
	CTcMatchPlayer* pPlayer=NULL;
	while (pos)	
	{
		userid=0;
		pPlayer=NULL;
		m_mapPlayer.GetNextAssoc(pos, userid, pPlayer);
// 		if (pPlayer
// 			&&pPlayer->m_nStatus!=TCMATCH_STATUS_GIVEUP
// 			&&pPlayer->m_nStatus!=TCMATCH_STATUS_WASHOUT
// 			&&pPlayer->m_nStatus!=TCMATCH_STATUS_WIN)
		if (pPlayer&&!pPlayer->IsQuit())
		{
			pPlayer->m_nStatus=TCMATCH_STATUS_WIN;
			NotifyMatchFinish(pPlayer);
		}
	}

	LogMatchInfo();

	MATCH_CLOSE* match_close=new MATCH_CLOSE;
	memset(match_close,0,sizeof(MATCH_CLOSE));
	match_close->nMatchID=GetMatchID();
	match_close->nRoomID=GetRoomID();

	PushStep(MATCH_STEP_MATCHEND,match_close);
}

void   CTcMatch::ClearMatch()
{
	CTcMatchBase::ClearMatch();
	//清理阶段
	if (m_FirstPhase)
	{
		while(m_FirstPhase)
		{
			CTCMatchPhase* phase=m_FirstPhase;
			m_FirstPhase=m_FirstPhase->m_NextPhase;
			delete phase;
		}
	}
	m_CurrentPhase=NULL;
	m_FirstPhase=NULL;	
}

void   CTcMatch::OnPlayerReady(LPMATCHUSER_READY pPlayerReady)
{
	int userid=pPlayerReady->nUserID;
	CTcMatchPlayer* pPlayer=GetPlayer(userid);
	if (!pPlayer) return;
	if (pPlayer->IsReadyMatch()) return;

	pPlayer->m_bReady=TRUE;
}

void   CTcMatch::OnPlayerOK(LPMATCH_PLAYER_OK pPlayerOk)
{
	int userid=pPlayerOk->nUserID;
	CTcMatchPlayer* pPlayer=GetPlayer(userid);

	if (!pPlayer) return;
	if (!pPlayer->m_bActive) return;//失去活力的玩家不再重新处理
	if (pPlayer->m_nStatus==TCMATCH_STATUS_PLAYING) return;
	if (pPlayer->m_nStatus==TCMATCH_STATUS_READY_NEXT_BOUT) return;
// 
	if (  pPlayer->IsQuit())
	{
		return;
	}

	if (pPlayer->m_nStatus==TCMATCH_STATUS_OFFLINE)//掉线状态，重新请求检索，那么恢复到等待分桌
		pPlayer->m_nStatus=TCMATCH_STATUS_WAIT;

	pPlayer->m_bOffline=FALSE;//移除掉线状态

	if (pPlayerOk->bAskRankList)
	{
		//发送积分列表
		NotifyUserRankList(userid);
	}

    if (pPlayer->m_nStatus==TCMATCH_STATUS_WAIT)
	{
		NotifyUserStatus(pPlayer->GetUserID(),TCMATCH_ANIMATCION_ARRAGETABLE,TRUE);
		return;
	}
	else if(pPlayer->m_nStatus==TCMATCH_STATUS_WAIT_NEXT_PHASE)
	{
		CTCMatchPhase* phase=m_CurrentPhase;
		CTCMatchPhase* nextphase=m_CurrentPhase->m_NextPhase;
		if (nextphase)
			NotifyUserStatus(pPlayer->GetUserID(),TCMATCH_ANIMATCION_WAIT_FINAL,TRUE,GetPlayingTableCount(),nextphase->GetStartUserCount());
		else
			NotifyUserStatus(pPlayer->GetUserID(),TCMATCH_ANIMATCION_WAIT_FINAL,TRUE,GetPlayingTableCount());

		return;
	}
	else if(pPlayer->m_nStatus==TCMATCH_STATUS_WAIT_RANK)
	{
		NotifyUserStatus(pPlayer->GetUserID(),TCMATCH_ANIMATCION_WATI_RANK,TRUE);
		return;
	}
}

BOOL  CTcMatch::TransMatchPhase()
{
	if (GetPlayingTableCount()>0)//还有桌在玩
		return FALSE;

	m_nCurrentUserWaitArrage=0;

	while(m_CurrentPhase->m_NextPhase
		&&m_CurrentPhase->m_NextPhase->IsReadyEnterPhase())
	{
		m_CurrentPhase=m_CurrentPhase->m_NextPhase;
		m_CurrentPhase->TransPhase();
		m_CurrentPhase->BeginPhase();

		CalcPlayersRank();
		CalcBaseScore();
		CalcLevelScore();
	}

	return TRUE;
}

void  CTcMatch::OnPlayerLeave(LPMATCHUSER_LEAVE pPlayerLeave)
{
	int userid=pPlayerLeave->nUserID;
	CTcMatchPlayer* player=GetPlayer(userid);
	if (!player) return;
	DWORD dwStatus=player->m_nStatus;
	if (player->IsQuit())
		return;
	

	if (m_nMatchStatus>=MATCH_STATUS_STARTED)//已经正式开始
	{
		if (player->IsPlaying())
		{
			player->m_nTotalBout--;//游戏中退出此局不算
		}

		player->SetRank(GetWashOutRank());
		m_CurrentPhase->SetPlayerStatusWhenLeave(player);

		NotifyMatchFinish(player);

		CalcPlayersRank();//有人退出就重新排列
	}
	else
	{
		CTcMatchPlayer* pPlayer=NULL;
		if (m_mapPlayer.Lookup(userid,pPlayer))
		{
			m_mapPlayer.RemoveKey(userid);
			SAFE_DELETE(pPlayer);
		}
		
		return;
	}


}
/*
void   CTcMatch::OnContinueMatch(LPCONTINUE_MATCH pContinueMatch)
{
	

}
*/

void     CTcMatch::DisBandTable(LPCOMMIT_DIFF pDisBandTable)
{
	int tableno=pDisBandTable->nTableNO;
	if (!m_CurrentPhase)
		return;

	CTcMatchTable* pTable=NULL;
	if (!m_mapTable.Lookup(tableno,pTable))
		return;

	if (pTable->m_nStatus!=TCMATCH_TABLE_PLAYING
		&&!pDisBandTable->bnReset)
		return;

	int nUserID[MAX_MATCHCHAIR_COUNT];
	memset(nUserID,0,sizeof(nUserID));
	//拷贝同桌的人
	for(int i=0;i<pTable->m_nPlayerCount;i++)
	{
		nUserID[i]=pTable->m_playerPtr[i]->GetUserID();
	}
	int nCount=pTable->m_nPlayerCount;

	//记录最大一局时间
	int t=UwlGetCurrentSeconds();
	if (m_nMaxGamePlaySeconds<t-pTable->m_nLastPlayStartTime)
		m_nMaxGamePlaySeconds=t-pTable->m_nLastPlayStartTime;
	//记录总游戏时间
	m_nTotalPlaySeconds+=t-pTable->m_nLastPlayStartTime;



	//准备进入决赛阶段直接拆桌
	BOOL bReset=m_CurrentPhase->DisBandTable(pDisBandTable,pTable);
	pDisBandTable->bnReset=bReset;//记录是否拆桌

    if(bReset)
	{
	 	DISBAND_TABLE * pdt=new DISBAND_TABLE;
		memset(pdt,0,sizeof(DISBAND_TABLE));
		pdt->nTableNO=tableno;
		pdt->nPlayerCount=pTable->m_nPlayerCount;
		memcpy(pdt->nPlayerIDs,nUserID,sizeof(nUserID));
	 	PushStep(MATCH_STEP_DISBANDTABLE,pdt );
	}
	CalcPlayersRank();
	CreateTableRankList(nUserID,nCount);
}



void   CTcMatch::FreshUserResult(LPUSER_DIFF pResult)
{
	int userid=pResult->nUserID;
	int tableno=pResult->nTableNO;
	
	CTcMatchPlayer* pPlayer=GetPlayer(userid);
	if (!pPlayer)
		return;
	//校正
	if(pPlayer->IsQuit())
	{
		//玩家已经退出，不再计算局数
		pResult->nWinDiff=0;
		pResult->nStandOffDiff=0;
		pResult->nLossDiff=0;
		pResult->nBoutDiff=0;
	}
	
	//更新到数据
	pPlayer->userongame.nWin+=pResult->nWinDiff;
	pPlayer->userongame.nLoss+=pResult->nLossDiff;
	pPlayer->userongame.nStandOff+=pResult->nStandOffDiff;
	pPlayer->userongame.nBout+=pResult->nBoutDiff;
	pPlayer->userongame.nBreakOff+=pResult->nBreakOffDiff;
	pPlayer->userongame.nTimeCost+=pResult->nTimeCostDiff;

	pPlayer->useronmatch.nWin+=pResult->nWinDiff;
	pPlayer->useronmatch.nLoss+=pResult->nLossDiff;
	pPlayer->useronmatch.nStandOff+=pResult->nStandOffDiff;
	pPlayer->useronmatch.nBout+=pResult->nBoutDiff;
	pPlayer->useronmatch.nBreakOff+=pResult->nBreakOffDiff;
	pPlayer->useronmatch.nTimeCost+=pResult->nTimeCostDiff;
	pPlayer->useronmatch.nScore+=pResult->nScoreDiff;
	pPlayer->useronmatch.nDeposit+=pResult->nDepositDiff;
}

BOOL   CTcMatch::CommitResult(LPCOMMIT_DIFF pResult)
{
	BOOL bnInvalid=m_CurrentPhase->IsGameInvalid();

	int n=pResult->nDiffCount;
	BYTE* ptr=(BYTE*)pResult;
	ptr+=sizeof(COMMIT_DIFF);

	if (bnInvalid)
	{
		for(int i=0;i<n;i++)
		{
			FreshUserResult((USER_DIFF*)ptr);
			
			ptr+=sizeof(USER_DIFF);
		}
	}
	//拆桌
	DisBandTable(pResult);

	//将当前最新的名次填充到USER_DIFF中
	if (bnInvalid)
	{
		ptr=(PBYTE(pResult)+sizeof(COMMIT_DIFF));
		for(int i=0;i<n;i++)
		{
			USER_DIFF* pMatchResult=	(USER_DIFF*) ptr;
			CTcMatchPlayer* pPlayer=GetPlayer(pMatchResult->nUserID);
			pMatchResult->nRank=pPlayer->GetRank();
		}
	}
	return bnInvalid;
}


CTcMatchBase::CTcMatchBase(CBaseRoomData* pRoomData,LPMATCH lpMatch)
{
	m_PlayerRank=NULL;
	m_pRoomData=pRoomData;
	ZeroMemory(&m_MatchData,sizeof(m_MatchData));
	memcpy(&(m_MatchData.match),lpMatch,sizeof(MATCH));

	m_nChairPerTable=pRoomData->GetChairCount();
	ResetParameter();
}

// CTcMatchBase::CTcMatchBase(int nMatchID,int nRoomID,int chairPerTable,DWORD dwOptions)
// {
// 	m_PlayerRank=NULL;
// 	ResetParameter();
// // 	m_MatchData.nRoomID=nRoomID;
// // 	m_MatchData.nMatchID=nMatchID;
// // 	m_MatchData.dwOptions =dwOptions;
// // 	m_nChairPerTable=chairPerTable;
// // 
// //  	m_nMatchStatus=MATCH_NONE;
// // 	m_nFinalBout=0;
// //  	m_nRankListCount=0;
// // 	m_nTableSortCount=0;
// // 	m_nWaitNextBoutSecond=TCMATCH_WAIT_NEXT_BOUT_SECOND;
// // 	m_nMaxPlaySecond=TCMATCH_MAX_GAME_PLAY_SECOND;
// // 	m_nWashOutIndex=0;
// // 
// // 
// // 	m_nTotalPlaySeconds=0;
// // 	m_nMaxGamePlaySeconds=0;
// // 	m_nTotalUserWaitSeconds=0;
// // 	m_nMaxUserWaitSeconds=0;
// // 	m_nMaxUserWaitArrageTable=0;
// // 	m_nMatchStartSecond=0;
// // 	m_nMaxUserPlayBout=0;
// // 	m_nMaxUserPlayBout=0;
// // 
// // 	memset(&m_PlayerRank,0,sizeof(m_PlayerRank));
// }

void CTcMatchBase::ResetParameter()
{
	m_nMatchStatus=MATCH_STATUS_NONE;
	m_nFinalBout=0;
	m_nRankListCount=0;
	m_nTableSortCount=0;
	m_nWaitNextBoutSecond=TCMATCH_WAIT_NEXT_BOUT_SECOND;
	m_nMaxPlaySecond=TCMATCH_MAX_GAME_PLAY_SECOND;
	m_nWashOutIndex=0;
	
	
	m_nTotalPlaySeconds=0;
	m_nMaxGamePlaySeconds=0;
	m_nTotalUserWaitSeconds=0;
	m_nMaxUserWaitSeconds=0;
	m_nMaxUserWaitArrageTable=0;
	m_nMatchStartSecond=0;
	m_nMaxUserPlayBout=0;
	m_nMaxUserPlayBout=0;
	m_bGameSvrConnnet=TRUE;
	
	SAFE_DELETE_ARRAY(m_PlayerRank);
}

CTcMatchBase::~CTcMatchBase()
{
	ClearMatch();
}

void  CTcMatchBase::NotifyUserStatus(int nUserID,LPCTSTR szStatus,BOOL bnAnimation,int nTablePlaying,int nPromotionCount)
{
	MATCH_USER_STATUS* pStatus=new MATCH_USER_STATUS;
	memset(pStatus,0,sizeof(MATCH_USER_STATUS));
	pStatus->nRoomID=GetRoomID();
	pStatus->nMatchID=GetMatchID();
	pStatus->nUserID=nUserID;
	pStatus->bnAnimation=bnAnimation;
	pStatus->nTablePlaying=nTablePlaying;
	pStatus->nPromotionCount=nPromotionCount;
	strcpy(pStatus->szStatus,szStatus);
	PushStep(MATCH_STEP_USERSTATUS,pStatus);
}

void  CTcMatchBase::NotifyWaitUserTableCount(int nTablePlayingCount,int nPromotionCount)
{
	int nIndex=0;
	POSITION pos = m_mapPlayer.GetStartPosition();
	int userid=0;
	CTcMatchPlayer* pPlayer=NULL;
	while (pos)
	{
		userid=0;
		pPlayer=NULL;
		m_mapPlayer.GetNextAssoc(pos, userid, pPlayer);
		if (!pPlayer->IsQuit()
			&&pPlayer->m_nStatus==TCMATCH_STATUS_WAIT_NEXT_PHASE)
		{
			NotifyUserStatus(pPlayer->GetUserID(),"",FALSE,nTablePlayingCount,nPromotionCount);
		}
	}
}

void  CTcMatchBase::NotifyUserWaitTalbe()
{
	int nIndex=0;
	POSITION pos = m_mapPlayer.GetStartPosition();
	int userid=0;
	CTcMatchPlayer* pPlayer=NULL;
	while (pos)
	{
		userid=0;
		pPlayer=NULL;
		m_mapPlayer.GetNextAssoc(pos, userid, pPlayer);
		if (!pPlayer->IsQuit()
			&&pPlayer->m_nStatus==TCMATCH_STATUS_WAIT)
		{
			NotifyUserStatus(pPlayer->GetUserID(),TCMATCH_ANIMATCION_WAITTALBE,TRUE);
		}
	}
}

int   CTcMatchBase::GetWashOutRank()
{
	int nIndex=0;
	POSITION pos = m_mapPlayer.GetStartPosition();
	int userid=0;
	CTcMatchPlayer* pPlayer=NULL;
	while (pos)
	{
		userid=0;
		pPlayer=NULL;
		m_mapPlayer.GetNextAssoc(pos, userid, pPlayer);
// 		if (pPlayer->m_nStatus!=TCMATCH_STATUS_WASHOUT
// 			&&pPlayer->m_nStatus!=TCMATCH_STATUS_GIVEUP
// 			&&pPlayer->m_nStatus!=TCMATCH_STATUS_WIN)
		if (!pPlayer->IsQuit())
			nIndex++;
	}
	return nIndex;
}

BOOL   CTcMatchBase::IsPlayerSeated(int nUserID)
{
	CTcMatchPlayer* pPlayer=GetPlayer(nUserID);
	if (!pPlayer) 
		return FALSE;

	if (pPlayer->m_nStatus==TCMATCH_STATUS_READY_NEXT_BOUT)
		return TRUE;

	return FALSE;
}

int  CTcMatchBase::GetTotalUserCount()
{
	return m_mapPlayer.GetCount();
}

void  CTcMatchBase::CopyUserIDs(int* pUserIDs)
{
	POSITION pos = m_mapPlayer.GetStartPosition();
	int userid=0;
	CTcMatchPlayer* pPlayer=NULL;
	int nCount=0;
	while (pos)	
	{
		userid=0;
		pPlayer=NULL;
		m_mapPlayer.GetNextAssoc(pos, userid, pPlayer);
		if (pPlayer)
		{
			pUserIDs[nCount++]=userid;
		}
	}

}

int  CTcMatchBase::GetPlayingTableCount()
{
	POSITION pos=m_mapTable.GetStartPosition();
	int nTableNO=0;
	CTcMatchTable* pTable=NULL;
	int nCount=0;
	while(pos)
	{
		nTableNO=0;
		pTable=NULL;
		m_mapTable.GetNextAssoc(pos,nTableNO,pTable);
		if (pTable
			&&pTable->m_nStatus!=TCMATCH_TABLE_EMPTY)
		{
			nCount++;
		}
	}
	
	return nCount;
}

void    CTcMatchBase::ClearMatch()
{
	int nCurrentSecond=UwlGetCurrentSeconds();
	{
		POSITION pos = m_mapPlayer.GetStartPosition();
		int userid=0;
		CTcMatchPlayer* pPlayer=NULL;
		while (pos)	
		{
			userid=0;
			pPlayer=NULL;
			m_mapPlayer.GetNextAssoc(pos, userid, pPlayer);
			SAFE_DELETE(pPlayer);
		}
		m_mapPlayer.RemoveAll();
	}

	
	{
		POSITION pos= m_mapTable.GetStartPosition();
		int nTableNO=0;
		CTcMatchTable* pTable=NULL;
		while(pos)
		{
			m_mapTable.GetNextAssoc(pos,nTableNO,pTable);
			if (pTable)
			{
				SAFE_DELETE(pTable);
			}
		}
		m_mapTable.RemoveAll();
	}

	ResetParameter();
}


LPCTcTable   CTcMatchBase::GetFreeTable()
{
	POSITION pos=m_mapTable.GetStartPosition();
	int nTableNO=0;
	CTcMatchTable* pTable=NULL;
	while(pos)
	{
		m_mapTable.GetNextAssoc(pos,nTableNO,pTable);
		if (pTable
			&&pTable->m_nStatus==TCMATCH_TABLE_EMPTY
			&&pTable->m_nPlayerCount==0)
			return pTable;
	}

	nTableNO=m_mapTable.GetCount()+1;
	pTable=new CTcMatchTable(this,nTableNO); 
	//	pTable=new CTcMatchTable(m_MatchData.nRoomID,nTableNO,m_MatchData.dwOptions);
	m_mapTable.SetAt(nTableNO,pTable);

	return pTable;
}


void    CTcMatchBase::PushStep(int nStepID,void* pStepData)
{
	UINT nReqID=0;
	int nDataLen=0;

	switch(nStepID)
	{
	case MATCH_STEP_ARREAGETABLE:
		nReqID=R2G_ARRANGE_TABLE;
		nDataLen=sizeof(ARRANGE_TABLE);
		break;
    case MATCH_STEP_RANKLIST:
		nReqID=R2G_FRESH_RANK_LIST;
		nDataLen=sizeof(MATCH_RANK_LIST);
		break;
	case MATCH_STEP_MATCHEND:
		nReqID=R2G_MATCH_CLOSE;
		nDataLen=sizeof(MATCH_CLOSE);
		break;
	case MATCH_STEP_USERSTATUS:
		nReqID=R2G_MATCH_USER_STATUS;
		nDataLen=sizeof(MATCH_USER_STATUS);
		break;
	case MATCH_STEP_OVERTIME:
		nReqID=R2G_MATCH_OVER_TIME;
		nDataLen=sizeof(MATCH_OVERTIME);
		break;
  	case MATCH_STEP_WAITFINAL:
		nReqID=R2G_MATCH_WAIT_FINAL;
		nDataLen=sizeof(MATCH_WAIT_FINAL);
		break;
	case MATCH_STEP_FINIEDMATCH:
		break;
	}

	MATCH_STEP* step=new MATCH_STEP;
	memset(step,0,sizeof(MATCH_STEP));
	step->nStepID=nStepID;
	step->pStepData=pStepData;
	step->nReqID =nReqID;
	step->nDataLen=nDataLen;
	m_listMatch.AddTail(step);

}

MATCH_STEP*   CTcMatchBase::PopStep()
{
	if (!m_listMatch.GetCount())
		return NULL;
	
	MATCH_STEP* step=m_listMatch.GetHead();

	return step;
}

void         CTcMatchBase::ClearStep(MATCH_STEP* step)
{
	if (step)
	{
   	    delete []step->pStepData;
		delete step;
		if (m_listMatch.GetCount())
			m_listMatch.RemoveHead();
	}
}

void    CTcMatchBase::NotifyMatchFinish(CTcMatchPlayer* pPlayer)
{
	if (pPlayer->IsQuit())
		return;

	m_nTotalUserWaitSeconds+=pPlayer->m_nTotalWait;
    if (pPlayer->m_nTotalWait>m_nMaxUserWaitSeconds)
		m_nMaxUserWaitSeconds=pPlayer->m_nTotalWait;

	pPlayer->m_bActive=FALSE;
	pPlayer->m_bQuit=TRUE;

	MATCH_USERGAIN* gain=new MATCH_USERGAIN;
	memset(gain,0,sizeof(MATCH_USERGAIN));
	gain->nMatchID=GetMatchID();
	gain->nRoomID=GetRoomID();
	gain->nUserID=pPlayer->GetUserID();
	gain->nStatus=pPlayer->m_nStatus;
	gain->nRank=pPlayer->GetRank();
	
	PushStep(MATCH_STEP_FINIEDMATCH,gain );
}

void  CTcMatchBase::NotifyUserRankList(int nUserID)
{
	CreateTableRankList(&nUserID,1);
}

CTcMatchPlayer*    CTcMatchBase::GetPlayer(int nUserID)
{
	CTcMatchPlayer* pPlayer=NULL;
	m_mapPlayer.Lookup(nUserID,pPlayer);
	return pPlayer;
}

CTcMatchTable*   CTcMatchBase::GetTable(int nTableNO)
{
	CTcMatchTable* pTable=NULL;
	m_mapTable.Lookup(nTableNO,pTable);
	return pTable;
}


void  CTcMatchBase::PushWaitRankUser(int nUserID)//预备发送一个玩家的排名信息
{
	if (nUserID<=0)return;

	m_WaitWaitRankList.AddTail(nUserID);
}

void  CTcMatchBase::SendWaitRankUser()//发送列表中所有排名信息
{
	if (m_WaitWaitRankList.GetCount()==0)
		return;
	while(m_WaitWaitRankList.GetCount())
	{
		int nUserID=m_WaitWaitRankList.GetHead();
		NotifyUserRankList(nUserID);
		m_WaitWaitRankList.RemoveHead();
	}
}

void   CTcMatchBase::LogMatchInfo()
{
	//记录比赛的各种信息
	POSITION pos = m_mapPlayer.GetStartPosition();
	int userid=0;
	CTcMatchPlayer* pPlayer=NULL;
	while (pos)	{
		userid=0;
		pPlayer=NULL;
		m_mapPlayer.GetNextAssoc(pos, userid, pPlayer);

		if (pPlayer->m_nTotalBout>m_nMaxUserPlayBout)
			m_nMaxUserPlayBout=pPlayer->m_nTotalBout;
	}
	//记录比赛时长
    UwlLogFile("MatchInfo:MatchCostSeconds:%ld",UwlGetCurrentSeconds()-m_nMatchStartSecond);
	//记录所有游戏耗费的总时间
	UwlLogFile("MatchInfo:TotalGameTakeSeconds:%ld",m_nTotalPlaySeconds);
	//记录平均游戏耗时
	if (m_nTableSortCount>0)
		UwlLogFile("MatchInfo:AverageGameSeconds:%ld",m_nTotalPlaySeconds/m_nTableSortCount);
	//记录单局游戏最长耗时
	UwlLogFile("MatchInfo:MaxGameSeconds:%ld",m_nMaxGamePlaySeconds);
	//记录用户等待的总时长
	UwlLogFile("MatchInfo:MaxUserWaitSeconds:%ld",m_nMaxUserWaitSeconds);
	//记录单次用户等待分桌的最大时间长度:
	UwlLogFile("MatchInfo:MaxUserWaitArrageTableSeconds:%ld",m_nMaxUserWaitArrageTable);
	//记录用户参与的最大局数
	UwlLogFile("MatchInfo:MaxUserBout:%ld",m_nMaxUserPlayBout);
	//记录用户最大相关局数

#ifdef _DEBUG
	int nMaxSameTable=0;
	std::map<CString,int>::iterator itorEx=m_mapSametable.begin();
	while(itorEx!=m_mapSametable.end())
	{
		if (nMaxSameTable<itorEx->second)
			nMaxSameTable=itorEx->second;

		itorEx++;
	}
	UwlLogFile("MatchInfo:MaxUserSameTableBout:%ld",nMaxSameTable);
#endif
}

void   CTcMatchBase::CalcPlayersRank()
{
	int nTotalUserCount=GetTotalUserCount();
	if (m_nRankListCount!=nTotalUserCount)
	{
		delete []m_PlayerRank;
		m_PlayerRank=NULL;
		m_nRankListCount=nTotalUserCount;
		if (m_nRankListCount>0)
		{
			m_PlayerRank=new MATCH_RANK[m_nRankListCount];
		}
	}

	if (m_nRankListCount>0)
	{
		memset(m_PlayerRank,0,sizeof(MATCH_RANK)*m_nRankListCount);
	}
	
	int index=0;
	int nMinTurn=m_nPhaseTurn;
	POSITION pos = m_mapPlayer.GetStartPosition();
	int userid=0;
	CTcMatchPlayer* pPlayer=NULL;
	while (pos)	{
		userid=0;
		pPlayer=NULL;
		m_mapPlayer.GetNextAssoc(pos, userid, pPlayer);
		m_PlayerRank[index].nMatchScore=pPlayer->GetScore();
		m_PlayerRank[index].nMatchDeposit=pPlayer->GetDeposit();
		m_PlayerRank[index].nUserID=pPlayer->GetUserID();
		m_PlayerRank[index].dwStatus=pPlayer->m_nStatus;
		m_PlayerRank[index].nMatchRank=pPlayer->GetRank();
		m_PlayerRank[index].bEndMatch=pPlayer->IsQuit();
		strcpy(m_PlayerRank[index].szUsername,pPlayer->match_player.szUsername);
		
// 		if (pPlayer->m_nStatus!=TCMATCH_STATUS_GIVEUP
// 			&&pPlayer->m_nStatus!=TCMATCH_STATUS_WASHOUT
// 			&&pPlayer->m_nStatus!=TCMATCH_STATUS_WIN)
// 			m_nRestUserCount++;

		if (pPlayer->m_bActive
			&&pPlayer->m_nParseBout>=m_nPhaseTurn)
		{
			if (nMinTurn==m_nPhaseTurn)
				nMinTurn=pPlayer->m_nParseBout;
			else if (pPlayer->m_nParseBout<nMinTurn)
				nMinTurn=pPlayer->m_nParseBout;
		}
		
		index++;
	}
	
	if (nMinTurn>m_nPhaseTurn)
		m_nPhaseTurn=nMinTurn;
	
	SortPlayerRank(m_PlayerRank,m_nRankListCount);
	
	for(int i=0;i<m_nRankListCount;i++)
	{
		m_PlayerRank[i].nMatchRank=i+1;
		CTcMatchPlayer* pPlayer=GetPlayer(m_PlayerRank[i].nUserID);
		if (pPlayer)
		{
			pPlayer->SetRank(m_PlayerRank[i].nMatchRank);
		}
	}
	m_nRankListCount=index;
	if(m_PlayerRank)
		m_nUserMaxScore=m_PlayerRank[0].nMatchScore;
}

void    CTcMatchBase::SortPlayerRank(MATCH_RANK rank[],int nCount)
{
	if (!nCount)return;
	MATCH_RANK temp;
	memset(&temp,0,sizeof(temp));
	int* nValue=new int[nCount];
	memset(nValue,0,sizeof(int)*nCount);
	
	for(int k=0;k<nCount;k++)
	{
		nValue[k]=rank[k].nMatchScore;
// 		if (rank[k].dwStatus==TCMATCH_STATUS_GIVEUP
// 			||rank[k].dwStatus==TCMATCH_STATUS_WASHOUT
// 			||rank[k].dwStatus==TCMATCH_STATUS_WIN)
		if (rank[k].bEndMatch)
			nValue[k]=TCMATCH_MIN_SCORE-rank[k].nMatchRank;
	}
	
	int t=0;
	for(int i=0;i<nCount-1;i++)
	{
		for(int j=i+1;j<nCount;j++)
		{
			if (nValue[i]<nValue[j])
			{
				temp=rank[i];
				rank[i]=rank[j];
				rank[j]=temp;
				
				t=nValue[j];
				nValue[j]=nValue[i];
				nValue[i]=t;
			}
		}
	}
		
		delete []nValue;
}

void   CTcMatchBase::CreateTableRankList(int nTalbeNO)
{
	
	LPCTcTable pTable=GetTable(nTalbeNO);
	if (!pTable) return;
	
	int nUserID[MAX_MATCHCHAIR_COUNT];
	memset(nUserID,0,sizeof(nUserID));
	//拷贝同桌的人
	for(int i=0;i<pTable->m_nPlayerCount;i++)
	{
		nUserID[i]=pTable->m_playerPtr[i]->GetUserID();
	}
	CreateTableRankList(nUserID,pTable->m_nPlayerCount);
}

void   CTcMatchBase::CreateTableRankList(int nUserIDs[],int nUserCount)
{
	MATCH_RANK_LIST* rank_list=new MATCH_RANK_LIST;
	memset(rank_list,0,sizeof(MATCH_RANK_LIST));
	rank_list->nRoomID=GetRoomID();
	rank_list->nCount=m_nRankListCount>MAX_RANKPLAYER_COUNT?MAX_RANKPLAYER_COUNT:m_nRankListCount;
	rank_list->nSendCount=nUserCount;
	rank_list->nRestUserCount=CalcRestUserCount();
	rank_list->bnInSameTable=TRUE;
	rank_list->nMaxScore=m_nUserMaxScore;
	int nTableNO=-1;
	for(int i=0;i<nUserCount;i++)
	{
		CTcMatchPlayer* pPlayer=GetPlayer(nUserIDs[i]);
		if (pPlayer)
		{
			rank_list->PlayerRank[i].nMatchScore=pPlayer->GetScore();
			rank_list->PlayerRank[i].nMatchDeposit=pPlayer->GetDeposit();
			rank_list->PlayerRank[i].nUserID=pPlayer->GetUserID();
			rank_list->PlayerRank[i].nMatchRank=pPlayer->GetRank();
			rank_list->PlayerRank[i].dwStatus=pPlayer->m_nStatus;
			rank_list->PlayerRank[i].bEndMatch=pPlayer->IsQuit();
			strcpy(rank_list->PlayerRank[i].szUsername,pPlayer->match_player.szUsername);
			
			if (i==0)
				nTableNO=pPlayer->m_nTableNO;
			else if (nTableNO!=pPlayer->m_nTableNO)
				rank_list->bnInSameTable=FALSE;
			else if (pPlayer->m_nTableNO==-1)
				rank_list->bnInSameTable=FALSE;
			
			pPlayer->m_nLastSendRankSecond=UwlGetCurrentSeconds();
		}
		else
		{
			UwlLogFile("CreateTableRankList Faild,user:%ld is not exist!",nUserIDs[i]);
		}
	}
	
	
	
    int index=nUserCount;
	
	for(i=0;i<m_nRankListCount;i++)
	{
		BOOL bnSame=FALSE;
		for(int j=0;j<nUserCount;j++)
		{
			if (m_PlayerRank[i].nUserID==nUserIDs[j])
			{
				bnSame=TRUE;
				break;
			}
		}
		if (bnSame) continue;
		
		rank_list->PlayerRank[index].nMatchScore=m_PlayerRank[i].nMatchScore;
		rank_list->PlayerRank[index].nMatchDeposit=m_PlayerRank[i].nMatchDeposit;
		rank_list->PlayerRank[index].nUserID=m_PlayerRank[i].nUserID;
		rank_list->PlayerRank[index].nMatchRank=m_PlayerRank[i].nMatchRank;
		rank_list->PlayerRank[index].dwStatus=m_PlayerRank[i].dwStatus;
		rank_list->PlayerRank[index].bEndMatch=m_PlayerRank[i].bEndMatch;
		strcpy(rank_list->PlayerRank[index].szUsername,m_PlayerRank[i].szUsername);
		index++;
		if (index>=rank_list->nCount)
			break;
	}
	
	PushStep(MATCH_STEP_RANKLIST,rank_list );
}

void  CTcMatchBase::CreateUserRankList(int userid)
{
	CTcMatchPlayer* pPlayer=GetPlayer(userid);
	if (!pPlayer) return;
	if (pPlayer->m_nTableNO==-1)
	{
		CreateTableRankList(&userid,1);
		return;
	}
	LPCTcTable pTable=GetTable(pPlayer->m_nTableNO);
	if (!pTable) return;

	MATCH_RANK_LIST* rank_list=new MATCH_RANK_LIST;
	memset(rank_list,0,sizeof(MATCH_RANK_LIST));
	rank_list->nRoomID=GetRoomID();
	rank_list->nCount=m_nRankListCount>MAX_RANKPLAYER_COUNT?MAX_RANKPLAYER_COUNT:m_nRankListCount;
	rank_list->nSendCount=1;
	rank_list->nRestUserCount=CalcRestUserCount();
	rank_list->bnInSameTable=TRUE;
	rank_list->nMaxScore=m_nUserMaxScore;
	int nTableNO=-1;

	rank_list->PlayerRank[0].nMatchScore=pPlayer->GetScore();
	rank_list->PlayerRank[0].nMatchDeposit=pPlayer->GetDeposit();
	rank_list->PlayerRank[0].nUserID=pPlayer->GetUserID();
	rank_list->PlayerRank[0].nMatchRank=pPlayer->GetRank();
	rank_list->PlayerRank[0].dwStatus=pPlayer->m_nStatus;
	rank_list->PlayerRank[0].bEndMatch=pPlayer->IsQuit();
	strcpy(rank_list->PlayerRank[0].szUsername,pPlayer->match_player.szUsername);

	int count=1;
	for(int i=0;i<pTable->m_nPlayerCount;i++)
	{
		if (pTable->m_playerPtr[i]->GetUserID()==userid)
			continue;

		CTcMatchPlayer* pPlayer=GetPlayer(pTable->m_playerPtr[i]->GetUserID());
		if (pPlayer)
		{
			rank_list->PlayerRank[count].nMatchScore=pPlayer->GetScore();
			rank_list->PlayerRank[count].nMatchDeposit=pPlayer->GetDeposit();
			rank_list->PlayerRank[count].nUserID=pPlayer->GetUserID();
			rank_list->PlayerRank[count].nMatchRank=pPlayer->GetRank();
			rank_list->PlayerRank[count].dwStatus=pPlayer->m_nStatus;
			rank_list->PlayerRank[count].bEndMatch=pPlayer->IsQuit();
			strcpy(rank_list->PlayerRank[count].szUsername,pPlayer->match_player.szUsername);
			count++;
		}
		else
		{
			delete rank_list;
			return;
		}
	}
	
    int index=count;
	
	for(i=0;i<m_nRankListCount;i++)
	{
		BOOL bnSame=FALSE;
		for(int j=0;j<pTable->m_nPlayerCount;j++)
		{
			if (m_PlayerRank[i].nUserID==pTable->m_playerPtr[j]->GetUserID())
			{
				bnSame=TRUE;
				break;
			}
		}
		if (bnSame) continue;
		
		rank_list->PlayerRank[index].nMatchScore=m_PlayerRank[i].nMatchScore;
		rank_list->PlayerRank[index].nMatchDeposit=m_PlayerRank[i].nMatchDeposit;
		rank_list->PlayerRank[index].nUserID=m_PlayerRank[i].nUserID;
		rank_list->PlayerRank[index].nMatchRank=m_PlayerRank[i].nMatchRank;
		rank_list->PlayerRank[index].dwStatus=m_PlayerRank[i].dwStatus;
		rank_list->PlayerRank[index].bEndMatch=m_PlayerRank[i].bEndMatch;
		strcpy(rank_list->PlayerRank[index].szUsername,m_PlayerRank[i].szUsername);
		index++;
		if (index>=rank_list->nCount)
			break;
	}
	
	PushStep(MATCH_STEP_RANKLIST,rank_list );
}

int   CTcMatchBase::CalcRestUserCount()
{
	int nRestCount=0;
	POSITION pos = m_mapPlayer.GetStartPosition();
	int userid=0;
	CTcMatchPlayer* pPlayer=NULL;
	while (pos)	
	{
		userid=0;
		pPlayer=NULL;
		m_mapPlayer.GetNextAssoc(pos, userid, pPlayer);
		if (  pPlayer->m_nStatus!=TCMATCH_STATUS_WASHOUT
			&&pPlayer->m_nStatus!=TCMATCH_STATUS_GIVEUP
			&&pPlayer->m_nStatus!=TCMATCH_STATUS_WIN)
		{
			nRestCount++;
		}
	}
	return nRestCount;
}

void   CTcMatchBase::ReFreshUserScore()
{
   int nCount=CalcRestUserCount();
   int nSize=sizeof(MATCH_REFRESH_USERSCORE)+nCount*sizeof(MATCH_USERSCORE);
   BYTE* pData=new BYTE[nSize];
   memset(pData,0,sizeof(BYTE)*nSize);
   LPMATCH_REFRESH_USERSCORE pFreshScore=(LPMATCH_REFRESH_USERSCORE)pData;
   pFreshScore->nGameID=GetGameID();
   pFreshScore->nMatchID=GetMatchID();
   pFreshScore->nRoomID=GetRoomID();
   pFreshScore->nCount=nCount;

   BYTE* pTemp=pData+sizeof(MATCH_REFRESH_USERSCORE);
   for(int i=0;i<nCount;i++)
   {
	   LPMATCH_USERSCORE pUserData=(LPMATCH_USERSCORE)pTemp;
	   pUserData->nUserID=m_PlayerRank[i].nUserID;
	   pUserData->nScore=m_PlayerRank[i].nMatchScore;
   	   pUserData->nDeposit=m_PlayerRank[i].nMatchDeposit;
	   pUserData->nRank=m_PlayerRank[i].nMatchRank;
	   pTemp+=sizeof(MATCH_USERSCORE);
   }

   PushStep(MATCH_STEP_RF_USERSCORE,pData);
}

void  CTcMatchBase::ContinueMatch()
{
	m_bGameSvrConnnet=TRUE;

	POSITION pos = m_mapPlayer.GetStartPosition();
	int userid=0;
	CTcMatchPlayer* pPlayer=NULL;
	while (pos)	{
		userid=0;
		pPlayer=NULL;
		m_mapPlayer.GetNextAssoc(pos, userid, pPlayer);
		if (pPlayer)
		{
			pPlayer->m_nLastPlaySecond=UwlGetCurrentSeconds();
		}
	}
}

void  CTcMatchBase::BreakOffMatch()
{
	m_bGameSvrConnnet=FALSE;
	POSITION pos=m_mapTable.GetStartPosition();
	int nTableNO=0;
	CTcMatchTable* pTable=NULL;
	while(pos)
	{
		m_mapTable.GetNextAssoc(pos,nTableNO,pTable);
		if (pTable
			&&pTable->m_nStatus==TCMATCH_TABLE_PLAYING)
		{
			for(int i=0;i<pTable->m_nPlayerCount;i++)
			{
				pTable->m_playerPtr[i]->m_bOffline=TRUE;
				pTable->m_playerPtr[i]->m_nLastPlaySecond=UwlGetCurrentSeconds();
			}
			pTable->ResetTable(TCMATCH_STATUS_OFFLINE);
		}
	}
}

int nRankWinPoints[11]={0,120,100,80,70,60,50,40,30,20,10};
int   CTcMatchBase::CalcWinPointByRank(int nUserID,int nRank)
{
	int nTotal=m_mapPlayer.GetCount();
	int nWinRank=nTotal/3;

	CTcMatchPlayer* pPlayer=NULL;
	if (!m_mapPlayer.Lookup(nUserID,pPlayer))
		return 0;

		
	if (nRank<=nWinRank)
	{//排名1/3有奖
		if (nRank>10)
			return nRankWinPoints[10]+pPlayer->m_nTotalBout;
		else
			return nRankWinPoints[nRank]+pPlayer->m_nTotalBout;
	}
	else
	{
		return pPlayer->m_nTotalBout;
	}

}

int   CTcMatchBase::CalcBaseDuanScore()
{
	return m_MatchData.match.nMatchLevel;
}



ARRANGE_TABLE*  CTcMatchBase::ArrageOneTable(CTcMatchTable* pTable,int nUserIDs[])
{
	int nCurrentSecond=UwlGetCurrentSeconds();
 	for(int j=0;j<m_nChairPerTable;j++)
	{
		CTcMatchPlayer* pPlayer=GetPlayer(nUserIDs[j]);
		if (!pPlayer) return NULL;
		//记录单局最大等待时间
		if (m_nMaxUserWaitArrageTable<nCurrentSecond-pPlayer->m_nLastPlaySecond)
		{
			m_nMaxUserWaitArrageTable=nCurrentSecond-pPlayer->m_nLastPlaySecond;
		}
		pTable->PlayerEnter(pPlayer);
	}

#ifdef _DEBUG
	for(int i=0;i<m_nChairPerTable-1;i++)
	{
		for(int j=i+1;j<m_nChairPerTable;j++)
		{
			CString pairuser;
			if (pTable->m_playerPtr[i]->GetUserID()>pTable->m_playerPtr[j]->GetUserID())
				pairuser.Format("%d_%d",pTable->m_playerPtr[j]->GetUserID(),pTable->m_playerPtr[i]->GetUserID());
			else
				pairuser.Format("%d_%d",pTable->m_playerPtr[i]->GetUserID(),pTable->m_playerPtr[j]->GetUserID());
             
			std::map<CString,int>::iterator itor=m_mapSametable.find(pairuser);
			if (itor==m_mapSametable.end())
			{
				m_mapSametable.insert(std::make_pair(pairuser,1));
			}
			else
			{
				itor->second++;
			}
		}
	}
#endif
	
	ARRANGE_TABLE* table=new ARRANGE_TABLE;
	memset(table,0,sizeof(ARRANGE_TABLE));
	
	if (!pTable->GetNextBout(table,GetBaseScore(),GetWashScore(),m_nMaxPlaySecond))
	{
	    delete table;
		return NULL;
	}

	table->nMatchMaxScore=m_nUserMaxScore;
	table->nTableSort=CalcTableSort();
	
	UwlLogFile("ArrageTable sort:%d,nUser:%d,%d,%d,%d",table->nTableSort,nUserIDs[0],nUserIDs[1],nUserIDs[2],nUserIDs[3]);
	return table;

}


int  CTcMatchBase::CalcTableSort()
{
	m_nTableSortCount++;
	return m_nTableSortCount;
}

void CTcMatchBase::SetOptions(DWORD dwOptions)
{
	m_MatchData.match.dwOptions=dwOptions;
}

CTCMatchPhase::CTCMatchPhase(int nBoutPerTurn)
{
	if (nBoutPerTurn==0)
		nBoutPerTurn=1;

	m_MatchPtr=NULL;
	m_NextPhase=NULL;
	m_nBoutPerTurn=nBoutPerTurn;
	m_nArrageTableTimes=0;
}

CTCMatchPhase::~CTCMatchPhase()
{
 
}

BOOL  CTCMatchPhase::AutoKickOffliner()
{
	if (!m_MatchPtr->m_bGameSvrConnnet)
		 return FALSE;

	return TRUE;
}


int      CTCMatchPhase::CalcBaseScore()
{
	m_MatchPtr->SetBaseScore(1000);
 	return m_MatchPtr->GetBaseScore();
}

int       CTCMatchPhase::CalcLevelScore()
{
	m_MatchPtr->SetWashScore( (m_MatchPtr->m_nPhaseTurn+1)*m_MatchPtr->m_MatchData.match.nInitOutScore );
	return m_MatchPtr->GetWashScore();
}


BOOL       CTCMatchPhase::IsReadyEnterPhase()//是否条件允许进入阶段
{
	//对于打立出局来说，随时可以进入
	return TRUE;
}

BOOL      CTCMatchPhase::IsPrepareEnterPhase()
{

	return TRUE;
}

void      CTCMatchPhase::ArrageTable()
{
	int nRestUserCount=m_MatchPtr->CalcRestUserCount();
	int nPlayers=m_MatchPtr->GetPlayingTableCount()*m_MatchPtr->m_nChairPerTable;
 	if(nRestUserCount-nPlayers <  m_MatchPtr->m_nChairPerTable)
	{
		return;
	}

	int n=m_MatchPtr->GetTotalUserCount();
	int*	nUserIDs=new int[n];
	memset(nUserIDs,0,sizeof(int)*n);
	int*  nWaitSecond=new int[n];
	memset(nWaitSecond,0,sizeof(int)*n);
	int*    nValue=new int[n];
	memset(nValue,0,sizeof(int)*n);

	int nCurrentSecond=UwlGetCurrentSeconds();
	srand(nCurrentSecond);

	int nMaxWaitSecond=0;
	int nIndex=0;
	POSITION pos = m_MatchPtr->m_mapPlayer.GetStartPosition();
	int userid=0;
	CTcMatchPlayer* pPlayer=NULL;
	while (pos)	
	{
		userid=0;
		pPlayer=NULL;
		m_MatchPtr->m_mapPlayer.GetNextAssoc(pos, userid, pPlayer);
		if (pPlayer->m_nStatus==TCMATCH_STATUS_WAIT
			&&pPlayer->m_nTableNO==-1
			&&nCurrentSecond-pPlayer->m_nLastPlaySecond>m_MatchPtr->m_nWaitNextBoutSecond)//等待到足够时间
		{
			nUserIDs[nIndex]=pPlayer->GetUserID();
			nWaitSecond[nIndex]=pPlayer->GetWaitSecond();//转换为等待的秒数

			if (m_nArrageTableTimes==0)//第一次打乱顺序随便排序
				nValue[nIndex]=rand()%n;
			else
				nValue[nIndex]=nWaitSecond[nIndex]+pPlayer->m_nMode*86400;


         	if (nCurrentSecond-pPlayer->m_nLastPlaySecond>nMaxWaitSecond)
				nMaxWaitSecond=nCurrentSecond-pPlayer->m_nLastPlaySecond;

			nIndex++;
		}
	}

	if (nIndex<m_MatchPtr->m_nChairPerTable)
	{
		//等待玩家凑不够一桌
		delete []nUserIDs;
		delete []nWaitSecond;
		delete []nValue;
		return;
	}

	if (nRestUserCount<2*m_MatchPtr->m_nChairPerTable)
	{
		//两桌人数都不够的时候，按等待时间来
		int i,j;
		for(i=0;i<nIndex-1;i++) 
		{
			for(j=i+1;j<nIndex;j++) /*注意循环的上下限*/ 
				if(nWaitSecond[i]<nWaitSecond[j]) 
			{ 
				SwapInt(nUserIDs[i],nUserIDs[j]);
				SwapInt(nValue[i],nValue[j]);
				SwapInt(nWaitSecond[i],nWaitSecond[j]);
			}
		}
	}
	else
	{
		int nWaitChair=2*m_MatchPtr->m_nChairPerTable;
		if (nMaxWaitSecond>20)
			nWaitChair=m_MatchPtr->m_nChairPerTable;

		if (nIndex<nWaitChair)
		{
			delete []nUserIDs;
			delete []nWaitSecond;
			delete []nValue;
			return;
		}

		int i,j;
		for(i=0;i<nIndex-1;i++) 
		{
			for(j=i+1;j<nIndex;j++) /*注意循环的上下限*/ 
				if(nValue[i]<nValue[j]) 
			{ 
				SwapInt(nUserIDs[i],nUserIDs[j]);
				SwapInt(nValue[i],nValue[j]);
				SwapInt(nWaitSecond[i],nWaitSecond[j]);
			}
		}
	}

	m_nArrageTableTimes++;
	int nPair=nIndex/m_MatchPtr->m_nChairPerTable;
	for(int i=0;i<nIndex-m_MatchPtr->m_nChairPerTable+1;i+=m_MatchPtr->m_nChairPerTable)
	{
		ArrageOneTable(&nUserIDs[i]);
	}

	if (m_nArrageTableTimes==1)
	{
		//本阶段第一次分桌，将没有分配的玩家通知为轮空
		m_MatchPtr->NotifyUserWaitTalbe();
	}

	delete []nUserIDs;
	delete []nWaitSecond;
	delete []nValue;
}

BOOL       CTCMatchPhase::ValidatePlayerEnter(int nUserID)
{
	if (m_MatchPtr->m_nMatchStatus>=MATCH_STATUS_STARTED)
		return FALSE;

	//已进入游戏不能重复进入
	CTcMatchPlayer* pPlayer=NULL;
	if (m_MatchPtr->m_mapPlayer.Lookup(nUserID,pPlayer))
		return FALSE;

	return TRUE;
}

void      CTCMatchPhase::InitialPlayerWhenEnter(CTcMatchPlayer* pPlayer)
{

}

void      CTCMatchPhase::ArrageOneTable(int nUserIDs[])
{
	CTcMatchTable* pTable=m_MatchPtr->GetFreeTable();
	InitialMatchTable(pTable);
	ARRANGE_TABLE* pArrageTable=m_MatchPtr->ArrageOneTable(pTable,nUserIDs);

	pArrageTable->nWaitAutoStart=3;//3秒钟自动开始

	if (pArrageTable)
	{
		FillMatchDetail(pArrageTable);
		
		m_MatchPtr->PushStep(MATCH_STEP_ARREAGETABLE,pArrageTable );
		m_MatchPtr->CreateTableRankList(pArrageTable->nTableNO);
	}

}

void      CTCMatchPhase::FillMatchDetail(LPARRANGE_TABLE pArrageTable)
{
	strcpy(pArrageTable->szMatchRule,"打立出局");
	sprintf(pArrageTable->szMatchDetail,"本局结束后，积分低于%d的玩家将被淘汰!",m_MatchPtr->GetWashScore());
}

int  CTCMatchPhase::GetStartUserCount()
{
   return -1;
}

void   CTCMatchPhase::SetPlayerStatusWhenLeave(CTcMatchPlayer* pPlayer)
{
	pPlayer->m_nStatus=TCMATCH_STATUS_GIVEUP;
}

BOOL  CTCMatchPhase::IsFirstPhase()
{
	return ((CTcMatch*)m_MatchPtr)->m_FirstPhase==this;
}


void      CTCMatchPhase::InitialMatchTable(CTcMatchTable* pTable)
{
	pTable->m_nPlanBout=m_nBoutPerTurn;//预计打3局
}

BOOL      CTCMatchPhase::DisBandTable(LPCOMMIT_DIFF pDisBandTable,CTcMatchTable* pTable)
{
	BOOL bnResetTable=pDisBandTable->bnReset;

	int nCurrentSecond=UwlGetCurrentSeconds();
	pTable->m_nLastPlayEndTime=nCurrentSecond;
	pTable->m_nStatus=TCMATCH_TABLE_WAIT_NEXT_BOUT;

	BOOL bnTransPhase=FALSE;
	int nStatus=TCMATCH_STATUS_WAIT;
	//前判断
	if (m_NextPhase
		&&m_NextPhase->IsPrepareEnterPhase())
	{
		bnResetTable=TRUE;
		bnTransPhase=TRUE;
		nStatus=TCMATCH_STATUS_WAIT_NEXT_PHASE;
	}

	for(int i=0;i<pTable->m_nPlayerCount;i++)
	{
		if (pTable->m_playerPtr[i])
		{
			pTable->m_playerPtr[i]->m_nLastPlaySecond=nCurrentSecond;
			pTable->m_playerPtr[i]->m_bOffline=pDisBandTable->bnOffline[i];
			
			if (pTable->m_playerPtr[i]->m_nStatus==TCMATCH_STATUS_PLAYING)
				if (pTable->m_nWashScore>=0
					&&pTable->m_playerPtr[i]->GetScore()< pTable->m_nWashScore
					&&!bnTransPhase)//进入转阶段不再淘汰玩家
				{
					pTable->m_playerPtr[i]->SetRank(m_MatchPtr->GetWashOutRank());
					pTable->m_playerPtr[i]->m_nStatus=TCMATCH_STATUS_WASHOUT;
					m_MatchPtr->NotifyMatchFinish(pTable->m_playerPtr[i]);
				}
				else
				{
					if (!pDisBandTable->bnOffline[i])
						pTable->m_playerPtr[i]->m_nStatus=TCMATCH_STATUS_READY_NEXT_BOUT;
					else
						pTable->m_playerPtr[i]->m_nStatus=TCMATCH_STATUS_OFFLINE;
				}
				
				if (pTable->m_playerPtr[i]->m_nStatus==TCMATCH_STATUS_GIVEUP
					||pTable->m_playerPtr[i]->m_nStatus==TCMATCH_STATUS_WASHOUT
					||pTable->m_playerPtr[i]->m_nStatus==TCMATCH_STATUS_OFFLINE
					||pTable->m_playerPtr[i]->m_nStatus==TCMATCH_STATUS_WIN)
				{
					bnResetTable=TRUE;
				}
		}
	}
	
	if (pTable->m_nBout>=pTable->m_nPlanBout)
		bnResetTable=TRUE;

	if (m_MatchPtr->m_nCurrentUserWaitArrage>60)//有玩家等待分桌超过60秒
	{
		m_MatchPtr->m_nCurrentUserWaitArrage=0;
		bnResetTable=TRUE;
	}
	

	//后判断
    m_MatchPtr->CalcRestUserCount();
	if (!bnTransPhase//原本未进入转阶段
		&&m_NextPhase//现在转阶段了
		&&m_NextPhase->IsPrepareEnterPhase())
	{
		bnResetTable=TRUE;
		nStatus=TCMATCH_STATUS_WAIT_NEXT_PHASE;

		//发送消息给所有的客户端
		MATCH_WAIT_FINAL* wait_final=new MATCH_WAIT_FINAL;
		memset(wait_final,0,sizeof(MATCH_WAIT_FINAL));
		wait_final->nMatchID=m_MatchPtr->GetMatchID();
		wait_final->nRoomID=m_MatchPtr->GetRoomID();
		wait_final->nNextPhaseUserCount=m_NextPhase->GetStartUserCount();
		m_MatchPtr->PushStep(MATCH_STEP_WAITFINAL,wait_final );
	}

	if (bnResetTable)
	{
		//通知其他等待中的玩家当前还剩余多少桌
		if (m_NextPhase//现在转阶段了
		&&m_NextPhase->IsPrepareEnterPhase())
		{
			int nTablePlaying=m_MatchPtr->GetPlayingTableCount()-1;
			if (nTablePlaying>0)
			{
				int nPromotionCount=0;
				if (m_NextPhase)
					nPromotionCount=m_NextPhase->GetStartUserCount();
				m_MatchPtr->NotifyWaitUserTableCount(nTablePlaying,nPromotionCount);
			}
		}
		
		//先刷新其他玩家的，再拆桌，避免重复发送
		pTable->ResetTable(nStatus);
	}

	return bnResetTable;
}

BOOL      CTcMatch::TerminateTable(int nTableNO)
{
	UwlLogFile("ForceDisBandTable :%ld",nTableNO);

	CTcMatchTable* pTable=GetTable(nTableNO);
	if (!pTable) return FALSE;

	int nCurrentSecond=UwlGetCurrentSeconds();
	pTable->m_nLastPlayEndTime=nCurrentSecond;
	pTable->m_nStatus=TCMATCH_TABLE_WAIT_NEXT_BOUT;


	{
		//通知大厅和管理员大厅
		DISBAND_TABLE * pdt=new DISBAND_TABLE;
		memset(pdt,0,sizeof(DISBAND_TABLE));
		pdt->nTableNO=pTable->m_nTableNO;
		pdt->nPlayerCount=pTable->m_nPlayerCount;
		for(int i=0;i<pTable->m_nPlayerCount;i++)
		{
			pdt->nPlayerIDs[i]=pTable->m_playerPtr[i]->GetUserID();
		}
		PushStep(MATCH_STEP_DISBANDTABLE,pdt);
	}


	int nStatus=TCMATCH_STATUS_WAIT;
	pTable->ResetTable(nStatus);

	TERMINATE_TABLE* pTT=new TERMINATE_TABLE;
	memset(pTT,0,sizeof(TERMINATE_TABLE));
	pTT->nMatchID=m_MatchData.match.nMatchID;
	pTT->nRoomID=m_pRoomData->roomdata.nRoomID;
	pTT->nTableNO=nTableNO;
	PushStep(MATCH_STEP_TERMINATE_TABLE,pTT);
	return TRUE;
}

BOOL      CTcMatch::ForceKickPlayer(int nUserID)
{
	CTcMatchPlayer* pPlayer=GetPlayer(nUserID);
	if (!pPlayer) return FALSE;
	if (pPlayer->IsQuit()) return FALSE;

	UwlLogFile("ForceKickPlayer :%ld",nUserID);
	pPlayer->SetRank(GetWashOutRank());
	m_CurrentPhase->SetPlayerStatusWhenLeave(pPlayer);
	NotifyMatchFinish(pPlayer);

	return TRUE;
}

void      CTCMatchPhase::SetMatchPtr(CTcMatchBase* Ptr)
{
	m_MatchPtr=Ptr;
}

void      CTCMatchPhase::TransPhase() //清理前一个阶段
{

}

BOOL      CTCMatchPhase::IsPhaseEnd()
{
	if (m_MatchPtr->CalcRestUserCount()<m_MatchPtr->m_nChairPerTable)
		return TRUE;

	return FALSE;
}

void      CTCMatchPhase::BeginPhase() //开始阶段
{
	int nCurrentSecond=UwlGetCurrentSeconds();
	int n=0;
	POSITION pos = m_MatchPtr->m_mapPlayer.GetStartPosition();
	int userid=0;
	CTcMatchPlayer* pPlayer=NULL;
	while (pos)	
	{
		userid=0;
		pPlayer=NULL;
		m_MatchPtr->m_mapPlayer.GetNextAssoc(pos, userid, pPlayer);
		if 	(pPlayer->m_nStatus==TCMATCH_STATUS_WAIT)
		{
// 			pPlayer->m_nScore=m_MatchPtr->GetInitScore();//初始分
// 			pPlayer->m_nDeposit =m_MatchPtr->GetInitDeposit();

			pPlayer->m_nMode=n % m_MatchPtr->m_nChairPerTable;
			pPlayer->m_nLastPlaySecond=nCurrentSecond;
			pPlayer->m_nTotalWait=0;
			n++;
		}
	}

	m_MatchPtr->m_nPhaseTurn=0;
	m_MatchPtr->CalcPlayersRank();
}



CTCMatchPhase2::CTCMatchPhase2(int nBoutPerTurn,int nChairPerTable,int nTotalPlayerCount):CTCMatchPhase(nBoutPerTurn)
{
	m_nTurnIndex=0;
	m_nWashTurn=1;
	m_nTransUserCount=nChairPerTable*1.5;

	int nFinalCount=nChairPerTable;
	int nNextTransCount=nFinalCount*0.5;

	while(nFinalCount*2+nNextTransCount<nTotalPlayerCount*4/5
		&&m_nWashTurn<TCMATCH_MAX_WASHTURN)
	{	
		m_nWashTurn++;
		nFinalCount*=2;
		if (m_nWashTurn<5)
		{
			m_nTransUserCount=nFinalCount*1.5;
			nNextTransCount=nFinalCount*2*0.5;
		}
		else
		{
			m_nTransUserCount=nFinalCount*1.25;
			nNextTransCount=nFinalCount*2*0.25;
		}
	}
}

CTCMatchPhase2::~CTCMatchPhase2()
{

}

void  CTCMatchPhase2::TransPhase()//清理前一个阶段
{
	int nLastPlayer=pow(2,m_nWashTurn-1)*m_MatchPtr->m_nChairPerTable;//最终参与比赛的人数

	//首先淘汰掉断线玩家
	{
		POSITION pos = m_MatchPtr->m_mapPlayer.GetStartPosition();
		int userid=0;
		CTcMatchPlayer* pPlayer=NULL;
		int nRestCount=0;
		while (pos)	
		{
			userid=0;
			pPlayer=NULL;
			m_MatchPtr->m_mapPlayer.GetNextAssoc(pos, userid, pPlayer);
			if (pPlayer
				&&pPlayer->m_nStatus==TCMATCH_STATUS_OFFLINE)
			{
				pPlayer->SetRank(m_MatchPtr->GetWashOutRank());
				pPlayer->m_nStatus=TCMATCH_STATUS_WASHOUT;
				m_MatchPtr->NotifyMatchFinish(pPlayer);
			}
		}

		//当淘汰人数过多的时候，减少人数
		int nRest=m_MatchPtr->CalcRestUserCount();
		while(nRest<nLastPlayer)
		{
			m_nWashTurn--;
			nLastPlayer=pow(2,m_nWashTurn-1)*m_MatchPtr->m_nChairPerTable;//最终参与比赛的人数
		}
	}



	
	//淘汰
	int nTime=UwlGetCurrentSeconds();
	m_MatchPtr->CalcPlayersRank();
	POSITION pos = m_MatchPtr->m_mapPlayer.GetStartPosition();
	int userid=0;
	CTcMatchPlayer* pPlayer=NULL;
	int nRestCount=0;
	while (pos)	
	{
		userid=0;
		pPlayer=NULL;
		m_MatchPtr->m_mapPlayer.GetNextAssoc(pos, userid, pPlayer);
		if (pPlayer
			&&pPlayer->m_nStatus==TCMATCH_STATUS_WAIT_NEXT_PHASE
			||pPlayer->m_nStatus==TCMATCH_STATUS_WAIT)
		{
			if (pPlayer->GetRank()>nLastPlayer)
			{
				char szMsg[96];
				sprintf(szMsg,"很遗憾，只有前%d名玩家进入下一轮比赛，您的最终名次是第%d名。\r\n",nLastPlayer,pPlayer->GetRank());
				m_MatchPtr->NotifyUserStatus(pPlayer->GetUserID(),szMsg,FALSE);

				pPlayer->m_nStatus=TCMATCH_STATUS_WASHOUT;
				m_MatchPtr->PushWaitRankUser(pPlayer->GetUserID());
				m_MatchPtr->NotifyMatchFinish(pPlayer);
			}
			else
			{
				nRestCount++;
				pPlayer->m_nStatus=TCMATCH_STATUS_WAIT;
				pPlayer->m_nParseBout=0;//重置局数
				pPlayer->m_nLastPlaySecond=nTime;
			}
		}
	}

	if (nRestCount!=nLastPlayer)
	{
		UwlLogFile("TransPhase2:nRestCount!=nLastPlayer");
	}

	m_MatchPtr->CalcPlayersRank();
	m_MatchPtr->SendWaitRankUser();
}

void   CTCMatchPhase2::BeginPhase()//开始阶段
{
	UwlLogFile("MatchPhase2 Begin!");

    int nCurrentSecond=UwlGetCurrentSeconds();
	int n=0;
	POSITION pos = m_MatchPtr->m_mapPlayer.GetStartPosition();
	int userid=0;
	CTcMatchPlayer* pPlayer=NULL;
	while (pos)	
	{
		userid=0;
		pPlayer=NULL;
		m_MatchPtr->m_mapPlayer.GetNextAssoc(pos, userid, pPlayer);
		if 	(pPlayer->m_nStatus==TCMATCH_STATUS_WAIT
			||pPlayer->m_nStatus==TCMATCH_STATUS_WAIT_NEXT_PHASE)
		{
			pPlayer->m_nMode=n%m_MatchPtr->m_nChairPerTable;
			pPlayer->m_nLastPlaySecond=nCurrentSecond;
			n++;
		}
	}
	
	m_MatchPtr->m_nPhaseTurn=0;
	m_MatchPtr->CalcPlayersRank();
}

void   CTCMatchPhase2::SetPlayerStatusWhenLeave(CTcMatchPlayer* pPlayer)
{
	pPlayer->m_bOffline=TRUE;

}

int    CTCMatchPhase2::CalcBaseScore()
{
	m_MatchPtr->SetBaseScore(1000);
  	return m_MatchPtr->GetBaseScore();
}

int    CTCMatchPhase2::CalcLevelScore()
{
	m_MatchPtr->SetWashScore( -100000000 );
	return m_MatchPtr->GetWashScore();
}

BOOL  CTCMatchPhase2::IsPrepareEnterPhase()
{
	int nFinalCount=pow(2,m_nWashTurn-1)*m_MatchPtr->m_nChairPerTable;
	if (m_nTransUserCount<nFinalCount)
	{
		if (m_nWashTurn<5)
		{
			m_nTransUserCount=nFinalCount*1.5;
		}
		else
		{
			m_nTransUserCount=nFinalCount*1.25;
		}
	}

	if (m_MatchPtr->CalcRestUserCount()<=m_nTransUserCount)
       	return TRUE;

	return FALSE;
}

BOOL   CTCMatchPhase2::IsReadyEnterPhase()//是否条件允许进入阶段
{
	if (m_MatchPtr->CalcRestUserCount()<=m_nTransUserCount
		&&m_MatchPtr->GetPlayingTableCount()==0)//没有桌子在玩
		return TRUE;

	return FALSE;
}

void   CTCMatchPhase2::InitialMatchTable(CTcMatchTable* pTable)
{
	pTable->m_nPlanBout=m_nBoutPerTurn;//预计打3局
// 	for(int i=0;i<pTable->m_nPlayerCount;i++)
// 	{
// 		if (pTable->m_playerPtr[i])
// 		{
// 			pTable->m_playerPtr[i]->m_nScore=m_MatchPtr->GetInitScore();
// 
// 		}
// 	}
}

void    CTCMatchPhase2::FillMatchDetail(LPARRANGE_TABLE pArrageTable)
{
	strcpy(pArrageTable->szMatchRule,"定局淘汰");
	sprintf(pArrageTable->szMatchDetail,"每轮打%d局,本桌积分最低的%d名玩家淘汰",m_nBoutPerTurn,m_MatchPtr->m_nChairPerTable/2);
	
	if (pArrageTable->nBout==0)//每轮的第一局显示动画
		pArrageTable->nWashTurn=m_nTurnIndex;
}


void   CTCMatchPhase2::ArrageTable()
{
	if (m_MatchPtr->GetPlayingTableCount()!=0)
		return;//一定要全部桌都结束
	
	m_nTurnIndex++;
	
	int nRestCount=m_MatchPtr->CalcRestUserCount();
	int nPlayerCount=pow(2,m_nWashTurn-1)*m_MatchPtr->m_nChairPerTable;
	int nTurn=m_MatchPtr->m_nPhaseTurn/m_nBoutPerTurn;
	while(nTurn>0)
	{
		nPlayerCount=nPlayerCount/2;
		nTurn--;
	}

	if (nPlayerCount>nRestCount)
	{
		nPlayerCount=m_MatchPtr->m_nChairPerTable;
		while(nPlayerCount*2<=nRestCount)
		{
			nPlayerCount*=2;
		}
	}
	if (nRestCount<nPlayerCount)
		return;
	
	int n=0;//继续比赛的人数
	int m=0;//淘汰人数
	int nWashCount=nRestCount-nPlayerCount;
	int* nWashUserID=new int[nWashCount];
    int* nUserIDs=new int[nPlayerCount];
	int* nValue=new int[nPlayerCount];
	memset(nWashUserID,0,sizeof(int)*nWashCount);
	memset(nUserIDs,0,sizeof(int)*nPlayerCount);
	memset(nValue,0,sizeof(int)*nPlayerCount);
	
	//淘汰
	POSITION pos = m_MatchPtr->m_mapPlayer.GetStartPosition();
	int userid=0;
	CTcMatchPlayer* pPlayer=NULL;
	while (pos)	
	{
		userid=0;
		pPlayer=NULL;
		m_MatchPtr->m_mapPlayer.GetNextAssoc(pos, userid, pPlayer);
		if (pPlayer->m_nStatus==TCMATCH_STATUS_WAIT_NEXT_PHASE
			||pPlayer->m_nStatus==TCMATCH_STATUS_WAIT
			||pPlayer->m_nStatus==TCMATCH_STATUS_OFFLINE)//在定局淘汰中，即便断线也必须加入分组
		{
			if (pPlayer->GetRank()>nPlayerCount)
			{
				pPlayer->m_nStatus=TCMATCH_STATUS_WASHOUT;
				nWashUserID[m++]=pPlayer->GetUserID();
			}
			else
			{
				pPlayer->SetScore(m_MatchPtr->GetInitScore()+pPlayer->GetScore()/10);//重置分数
				pPlayer->m_nStatus=TCMATCH_STATUS_WAIT;//强制转化为等待分桌
				nUserIDs[n]=pPlayer->GetUserID();
				nValue[n]=pPlayer->m_nMode;
				n++;
			}
		}
	}
	
	m_MatchPtr->CalcPlayersRank();
	m_MatchPtr->ReFreshUserScore();//刷新新的数值到数据库

	for(int j=0;j<m;j++)
	{
		m_MatchPtr->NotifyUserRankList(nWashUserID[j]);
		CTcMatchPlayer* Player=m_MatchPtr->GetPlayer(nWashUserID[j]);
		if (Player)
			m_MatchPtr->NotifyMatchFinish(Player);
	}

	if (n<m_MatchPtr->m_nChairPerTable)
	{
		delete []nWashUserID;
		delete []nUserIDs;
     	delete []nValue;
		return; //玩家已经不足
	}
	
	//排序
	{
		for(int i=0;i<nPlayerCount-1;i++) 
		{
			for(int j=i+1;j<nPlayerCount;j++) 
				if(nValue[i]<nValue[j]) 
				{ 
					SwapInt(nUserIDs[i],nUserIDs[j]);
					SwapInt(nValue[i],nValue[j]);
				}
		}
	}
	
	
	//分桌
	m_nArrageTableTimes++;
	for(int i=0;i<n;i+=m_MatchPtr->m_nChairPerTable)
	{
		ArrageOneTable(&nUserIDs[i]);
	}
	
	delete []nWashUserID;
	delete []nUserIDs;
	delete []nValue;
}



BOOL   CTCMatchPhase2::DisBandTable(LPCOMMIT_DIFF pDisBandTable,CTcMatchTable* pTable)
{
	BOOL bnResetTable=pDisBandTable->bnReset;
	int nCurrentSecond=UwlGetCurrentSeconds();

	pTable->m_nLastPlayEndTime=nCurrentSecond;
	pTable->m_nStatus=TCMATCH_TABLE_WAIT_NEXT_BOUT;

	int nScore[MAX_MATCHCHAIR_COUNT];
	int nChairno[MAX_MATCHCHAIR_COUNT];
	memset(nScore,0,sizeof(nScore));
	memset(nChairno,0,sizeof(nChairno));


	for(int i=0;i<pTable->m_nPlayerCount;i++)
	{
		if (pTable->m_playerPtr[i])
		{
			pTable->m_playerPtr[i]->m_nLastPlaySecond=nCurrentSecond;
			pTable->m_playerPtr[i]->m_bOffline=pDisBandTable->bnOffline[i];
			pTable->m_playerPtr[i]->m_nStatus=TCMATCH_STATUS_READY_NEXT_BOUT;

			nScore[i]=pTable->m_playerPtr[i]->GetScore();
			nChairno[i]=i;
		}
	}

	for( i=0;i<pTable->m_nPlayerCount-1;i++)
	{
		for(int j=i+1;j<pTable->m_nPlayerCount;j++)
			if (nScore[i]>nScore[j])
			{
				SwapInt(nScore[i],nScore[j]);
				SwapInt(nChairno[i],nChairno[j]);
			}
	}

	
 	if (pTable->m_nBout>=pTable->m_nPlanBout)
	{
		bnResetTable=TRUE;
		for(int i=0;i<=pTable->m_nPlayerCount/2-1;i++)
		{
			int chairno=nChairno[i];
			pTable->m_playerPtr[chairno]->SetRank(m_MatchPtr->GetWashOutRank());

			if (pTable->m_playerPtr[chairno]->GetRank()<=m_MatchPtr->m_nChairPerTable)
				pTable->m_playerPtr[chairno]->m_nStatus=TCMATCH_STATUS_WIN;
			else
				pTable->m_playerPtr[chairno]->m_nStatus=TCMATCH_STATUS_WASHOUT;

            m_MatchPtr->NotifyMatchFinish(pTable->m_playerPtr[chairno]);
		}
	}
	
	if (bnResetTable)
	{
		//通知其他等待中的玩家当前还剩余多少桌
		int nTablePlaying=m_MatchPtr->GetPlayingTableCount()-1;
		if (nTablePlaying>0)
		{
			m_MatchPtr->NotifyWaitUserTableCount(nTablePlaying,0);
		}

		pTable->ResetTable(TCMATCH_STATUS_WAIT_NEXT_PHASE);
	}
	return bnResetTable;
}

int   CTCMatchPhase2::GetStartUserCount()
{
	return pow(2,m_nWashTurn-1)*m_MatchPtr->m_nChairPerTable;
}

BOOL  CTCMatchPhase2::AutoKickOffliner()
{
	return FALSE;
}


BOOL  CTCMatchPhase3::ValidatePlayerEnter(int nUserID)
{
	CTcMatchPlayer* pPlayer=NULL;
	if (m_MatchPtr->m_mapPlayer.Lookup(nUserID,pPlayer))
		return FALSE;
	
	return TRUE;
}


CTCMatchPhase3::CTCMatchPhase3(int nBoutPerTurn,int nMatchSeconds,int nWinBout)
:CTCMatchPhase(nBoutPerTurn)
{
	m_nMatchSeconds=nMatchSeconds;
	m_nWinBout=nWinBout;
}

CTCMatchPhase3::~CTCMatchPhase3()
{

}

void CTCMatchPhase3::InitialMatchTable(CTcMatchTable* pTable)
{
	pTable->m_nPlanBout=m_nBoutPerTurn;//预计打3局
}

void  CTCMatchPhase3::InitialPlayerWhenEnter(CTcMatchPlayer* pPlayer)
{
	pPlayer->SetScore(0);
	pPlayer->m_nStatus=TCMATCH_STATUS_NOT_READY;//进入就等待分桌
}

BOOL  CTCMatchPhase3::IsGameInvalid()
{
	if (m_MatchPtr->m_nMatchStatus<MATCH_STATUS_STARTED
		||m_MatchPtr->m_nMatchStatus>=MATCH_STATUS_FINISHED)
		return FALSE;

	if (UwlGetCurrentSeconds()-m_MatchPtr->m_nMatchStartSecond>m_nMatchSeconds)
		return FALSE;

	return TRUE;
}

BOOL  CTCMatchPhase3::DisBandTable(LPCOMMIT_DIFF pDisBandTable,CTcMatchTable* pTable)
{
	BOOL bnResetTable=pDisBandTable->bnReset;
	int nCurrentSecond=UwlGetCurrentSeconds();
	pTable->m_nLastPlayEndTime=nCurrentSecond;
	pTable->m_nStatus=TCMATCH_TABLE_WAIT_NEXT_BOUT;
	int nStatus=TCMATCH_STATUS_NOT_READY;

	
	for(int i=0;i<pTable->m_nPlayerCount;i++)
	{
		if (pTable->m_playerPtr[i])
		{
			pTable->m_playerPtr[i]->m_nLastPlaySecond=nCurrentSecond;
			pTable->m_playerPtr[i]->m_bOffline=pDisBandTable->bnOffline[i];
			
			if (pTable->m_playerPtr[i]->m_nStatus==TCMATCH_STATUS_PLAYING)
			{
				if (!pDisBandTable->bnOffline[i])
					pTable->m_playerPtr[i]->m_nStatus=TCMATCH_STATUS_READY_NEXT_BOUT;
				else
					pTable->m_playerPtr[i]->m_nStatus=TCMATCH_STATUS_OFFLINE;
			}
			
			//有玩家离开或者断线或者退出的时候
			if (pTable->m_playerPtr[i]->m_nStatus==TCMATCH_STATUS_GIVEUP
				||pTable->m_playerPtr[i]->m_nStatus==TCMATCH_STATUS_WASHOUT
				||pTable->m_playerPtr[i]->m_nStatus==TCMATCH_STATUS_OFFLINE
				||pTable->m_playerPtr[i]->m_nStatus==TCMATCH_STATUS_WIN)
			{
				bnResetTable=TRUE;
			}
		}
	}
	
	if (pTable->m_nBout>=pTable->m_nPlanBout)
		bnResetTable=TRUE;
	
	if (bnResetTable)
	{
		pDisBandTable->bnNeedTrigger=TRUE;

		pTable->ResetTable(nStatus);
	}
	
	return bnResetTable;
}

void      CTCMatchPhase3::TransPhase()//清理前一个阶段
{

}

void      CTCMatchPhase3::BeginPhase()//开始阶段
{
    int nCurrentSecond=UwlGetCurrentSeconds();
	int n=0;
	POSITION pos = m_MatchPtr->m_mapPlayer.GetStartPosition();
	int userid=0;
	CTcMatchPlayer* pPlayer=NULL;
	while (pos)	
	{
		userid=0;
		pPlayer=NULL;
		m_MatchPtr->m_mapPlayer.GetNextAssoc(pos, userid, pPlayer);
		if 	(pPlayer->m_nStatus==TCMATCH_STATUS_NOT_READY)
		{
			pPlayer->m_nStatus=TCMATCH_STATUS_WAIT;
			pPlayer->SetScore(0);//初始分
			pPlayer->m_nMode=n%m_MatchPtr->m_nChairPerTable;
			pPlayer->m_nLastPlaySecond=nCurrentSecond;
			pPlayer->m_nTotalWait=0;
			n++;
		}
	}
	
	m_MatchPtr->m_nPhaseTurn=0;
	m_MatchPtr->CalcPlayersRank();
}

int       CTCMatchPhase3::CalcBaseScore()
{
	m_MatchPtr->SetBaseScore(1000);
  	return m_MatchPtr->GetBaseScore();
}

int       CTCMatchPhase3::CalcLevelScore()
{
	m_MatchPtr->SetWashScore(-100000000);
	return m_MatchPtr->GetWashScore();
}


void      CTCMatchPhase3::ArrageTable()
{
	int nRestUserCount=m_MatchPtr->CalcRestUserCount();
	if (m_MatchPtr->GetPlayingTableCount()*m_MatchPtr->m_nChairPerTable
		>nRestUserCount-m_MatchPtr->m_nChairPerTable)
	{
		return;
	}
	int n=m_MatchPtr->GetTotalUserCount();//增加10，防止越界
	int*	nUserIDs=new int[n];
	memset(nUserIDs,0,sizeof(int)*n);
	int*  nWaitSecond=new int[n];
	memset(nWaitSecond,0,sizeof(int)*n);
	int*    nValue=new int[n];
	memset(nValue,0,sizeof(int)*n);

	int nCurrentSecond=UwlGetCurrentSeconds();
	srand(nCurrentSecond);

	int nMaxWaitSecond=0;
	int nIndex=0;
	POSITION pos = m_MatchPtr->m_mapPlayer.GetStartPosition();
	int userid=0;
	CTcMatchPlayer* pPlayer=NULL;
	while (pos)	
	{
		userid=0;
		pPlayer=NULL;
		m_MatchPtr->m_mapPlayer.GetNextAssoc(pos, userid, pPlayer);
		if (pPlayer->m_nStatus==TCMATCH_STATUS_WAIT
			&&pPlayer->m_nTableNO==-1
			&&nCurrentSecond-pPlayer->m_nLastPlaySecond>m_MatchPtr->m_nWaitNextBoutSecond)//等待到足够时间
		{
			nUserIDs[nIndex]=pPlayer->GetUserID();
			nWaitSecond[nIndex]=pPlayer->GetWaitSecond();//转换为等待的秒数

			if (m_nArrageTableTimes==0)//第一次打乱顺序随便排序
				nValue[nIndex]=rand()%n;
			else
				nValue[nIndex]=nWaitSecond[nIndex]+pPlayer->m_nMode*86400;


         	if (nCurrentSecond-pPlayer->m_nLastPlaySecond>nMaxWaitSecond)
				nMaxWaitSecond=nCurrentSecond-pPlayer->m_nLastPlaySecond;
		}
	}

	if (nIndex<m_MatchPtr->m_nChairPerTable)
	{
		//等待玩家凑不够一桌
		delete []nUserIDs;
		delete []nWaitSecond;
		delete []nValue;
		return;
	}

	if (nRestUserCount<2*m_MatchPtr->m_nChairPerTable)
	{
		//两桌人数都不够的时候，按等待时间来
		int i,j;
		for(i=0;i<nIndex-1;i++) 
		{
			for(j=i+1;j<nIndex;j++) /*注意循环的上下限*/ 
				if(nWaitSecond[i]<nWaitSecond[j]) 
			{ 
				SwapInt(nUserIDs[i],nUserIDs[j]);
				SwapInt(nValue[i],nValue[j]);
				SwapInt(nWaitSecond[i],nWaitSecond[j]);
			}
		}
	}
	else
	{
		int nWaitChair=2*m_MatchPtr->m_nChairPerTable;
		if (nMaxWaitSecond>20)
			nWaitChair=m_MatchPtr->m_nChairPerTable;

		if (nIndex<nWaitChair)
		{
			delete []nUserIDs;
			delete []nWaitSecond;
			delete []nValue;
			return;
		}

		int i,j;
		for(i=0;i<nIndex-1;i++) 
		{
			for(j=i+1;j<nIndex;j++) /*注意循环的上下限*/ 
				if(nValue[i]<nValue[j]) 
			{ 
				SwapInt(nUserIDs[i],nUserIDs[j]);
				SwapInt(nValue[i],nValue[j]);
				SwapInt(nWaitSecond[i],nWaitSecond[j]);
			}
		}
	}


	m_nArrageTableTimes++;
	int nPair=nIndex/m_MatchPtr->m_nChairPerTable;
	for(int i=0;i<nIndex-m_MatchPtr->m_nChairPerTable+1;i+=m_MatchPtr->m_nChairPerTable)
	{
		ArrageOneTable(&nUserIDs[i]);
	}

	delete []nUserIDs;
	delete []nWaitSecond;
	delete []nValue;
}


void   CTCMatchPhase3::FillMatchDetail(LPARRANGE_TABLE pArrageTable)
{
	strcpy(pArrageTable->szMatchRule,"循环积分赛");
	sprintf(pArrageTable->szMatchDetail,"%d分钟内打满%d局的玩家按积分排名。",m_nMatchSeconds/60,m_nWinBout);
}

void    CTcMatchBase::RefreshDataFromStartToRoom(LPMATCH_STARTTOROOM lpMS)
{
	m_MatchData.match.nMatchID       =lpMS->nMatchID ;
	m_MatchData.match.nMatchType     =lpMS->nMatchType;
 	m_MatchData.match.nSubType       =lpMS->nSubType;
	m_MatchData.match.nStatus        =lpMS->nStatus;
  	m_MatchData.match.dwOptions      =lpMS->dwOptions;
    m_MatchData.match.dwConfigs      =lpMS->dwConfigs;

 	m_MatchData.match.nGameID        =lpMS->nGameID;
	m_MatchData.match.nAreaID        =lpMS->nAreaID;
	m_MatchData.match.nRoomID        =lpMS->nRoomID;

	m_MatchData.match.nBigMatchID	   =lpMS->nBigMatchID;
	m_MatchData.match.nPreviousID    =lpMS->nPreviousID;
	m_MatchData.match.nLaunchDate    =lpMS->nLaunchDate;
	m_MatchData.match.nLaunchTime    =lpMS->nLaunchTime;
 	m_MatchData.match.nMaxRunTime    =lpMS->nMaxRunTime;
	m_MatchData.match.nMatchLevel    =lpMS->nMatchLevel;

 	m_MatchData.match.nInitScore      =lpMS->nInitScore;
	m_MatchData.match.nInitDeposit    =lpMS->nInitDeposit;
	m_MatchData.match.nInitBaseScore  =lpMS->nInitBaseScore;
	m_MatchData.match.nInitBaseDeposit=lpMS->nInitBaseDeposit;
 	m_MatchData.match.nInitOutScore   =lpMS->nInitOutScore;
	m_MatchData.match.nInitOutDeposit =lpMS->nInitOutDeposit;
	m_MatchData.match.nFixRings       =lpMS->nFixRings;
	m_MatchData.match.nFixBouts       =lpMS->nFixBouts;

 	m_MatchData.match.nSignUpCount    =lpMS->nSignUpCount;
	m_MatchData.match.nMinRunners     =lpMS->nMinRunners;
	m_MatchData.match.nMaxRunners     =lpMS->nMaxRunners;
	m_MatchData.match.nEndRunners     =lpMS->nEndRunners;



	memcpy(&m_MatchData.dns,&lpMS->dns,sizeof(DUAN_NEEDSCORE));
}

void	CTcMatchBase::CopyMatchDetail(MATCH_DETAIL* lpMatchDetail,MATCH_DYNA* lpMD2)
{
	LPMATCH lpMatch=(LPMATCH)&(m_MatchData.match);
	lpMatchDetail->nMatchID =lpMatch->nMatchID ;
	lpMatchDetail->nMatchType=lpMatch->nMatchType;
	lpMatchDetail->nSubType=lpMatch->nSubType;
	lpMatchDetail->nLayOrder=lpMatch->nLayOrder;

   	lpMatchDetail->dwOptions=lpMatch->dwOptions;
 	lpMatchDetail->dwConfigs=lpMatch->dwConfigs;
 	lpMatchDetail->dwManages=lpMatch->dwConfigs;

  	lpMatchDetail->nIconID=lpMatch->nIconID;
	lpMatchDetail->nFontColor=lpMatch->nFontColor;
	lpMatchDetail->nFolderID=lpMatch->nFolderID;
	lpMatchDetail->nGameID=lpMatch->nGameID;
	lpMatchDetail->nAreaID=lpMatch->nAreaID;
	lpMatchDetail->nRoomID=lpMatch->nRoomID;
	
	lpMatchDetail->nBigMatchID=lpMatch->nBigMatchID;
	lpMatchDetail->nPreviousID=lpMatch->nPreviousID;
	lpMatchDetail->nLaunchDate=lpMatch->nLaunchDate;
	lpMatchDetail->nLaunchTime=lpMatch->nLaunchTime;
	lpMatchDetail->nJoinEndTime=lpMatch->nJoinEndTime;
	lpMatchDetail->nMinRunners=lpMatch->nMinRunners;
	lpMatchDetail->nMaxRunners=lpMatch->nMaxRunners;
	lpMatchDetail->nJoinFeeKind=lpMatch->nJoinFeeKind;
	lpMatchDetail->nJoinFeeMuch=lpMatch->nJoinFeeMuch;


	lpMatchDetail->nInitScore=lpMatch->nInitScore;
	lpMatchDetail->nInitDeposit=lpMatch->nInitDeposit;
	
	lpMatchDetail->nEnterRoomPS=lpMatch->nEnterRoomPS;  
	lpMatchDetail->nEnterGamePS=lpMatch->nEnterGamePS;

	//动态数据
	lpMatchDetail->nEndRunners          =       lpMD2->nEndRunners;
	lpMatchDetail->nSignUpCount			=		lpMD2->nSignUpCount;
	lpMatchDetail->nStatus				=		lpMD2->nStatus;
	lpMatchDetail->nUserMatchStatus		=		lpMD2->nUserMatchStatus;
	lpMatchDetail->nEnterGameTI			=		lpMD2 ->nEnterGameTI;
	lpMatchDetail->nLaunchRS			=		lpMD2->nLaunchRS;
	
	lstrcpy(lpMatchDetail->szMatchName,lpMatch->szMatchName);
	lstrcpy(lpMatchDetail->szMatchWWW,lpMatch->szMatchWWW);
	lstrcpy(lpMatchDetail->szSignUpWWW,lpMatch->szSignUpWWW);
	lstrcpy(lpMatchDetail->szRoomWWW1,lpMatch->szRoomWWW1);
	lstrcpy(lpMatchDetail->szRoomWWW2,lpMatch->szRoomWWW2);
	lstrcpy(lpMatchDetail->szGameWWW1,lpMatch->szGameWWW1);
	lstrcpy(lpMatchDetail->szGameWWW2,lpMatch->szGameWWW2);
	lstrcpy(lpMatchDetail->szRoomTXT,lpMatch->szRoomTXT);
	lstrcpy(lpMatchDetail->szGameTXT,lpMatch->szGameTXT);
}


void    CTcMatchBase::RefreshDataFromMatchDynamic(LPMATCH_DYNA lpmatch_dynamic)
{
	m_MatchData.match.nMatchID       =lpmatch_dynamic->nMatchID ;
	m_MatchData.match.nStatus        =lpmatch_dynamic->nStatus;

 	m_MatchData.match.nSignUpCount    =lpmatch_dynamic->nSignUpCount;
	m_MatchData.match.nEndRunners     =lpmatch_dynamic->nEndRunners;
}

int   CTcMatchBase::GetRoomID()
{
	return m_MatchData.match.nRoomID;
}
int   CTcMatchBase::GetMatchID()
{
	return m_MatchData.match.nMatchID;
}

int	CTcMatchBase::GetGameID()
{
	return m_MatchData.match.nGameID;
}

DWORD   CTcMatchBase::GetMatchConfig()
{
	return m_MatchData.match.dwConfigs;

}

int   CTcMatchBase::GetInitScore()
{
	return m_MatchData.match.nInitScore;

}

int   CTcMatchBase::GetInitDeposit()
{
	return m_MatchData.match.nInitDeposit;

}
int   CTcMatchBase::GetBaseScore()
{
 	return m_MatchData.nBaseScore;

}
int  CTcMatchBase::GetWashScore()
{
	return m_MatchData.nWashScore;
}

void   CTcMatchBase::SetBaseScore(int nBaseScore)
{
	m_MatchData.nBaseScore=nBaseScore;
}

void  CTcMatchBase::SetWashScore(int nWashScore)
{
	m_MatchData.nWashScore=nWashScore;
}
