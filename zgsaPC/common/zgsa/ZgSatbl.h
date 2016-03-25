#pragma once
extern int  ZGSA_GetMaxValueCard(int nCardID[],int& MaxType,int MaxCard[],int CompareValue[]);
extern GAME_WIN_RESULT ZGSA_CalcGameResult(GAME_TABLE_INFO game_table_info);

extern void InitialGameTableInfo(GAME_TABLE_INFO* table);

class CGameTable : public CTable{
public:
	CGameTable(int roomid = INVALID_OBJECT_ID, int tableno = INVALID_OBJECT_ID, int score_mult = 1, 
			int totalchairs = TOTAL_CHAIRS, DWORD gameflags = GAME_FLAGS,
			DWORD gameflags2 = 0, 
			int max_asks = MAX_ASK_REPLYS, 
			int betWait = BET_WAIT );
 
	virtual ~CGameTable();
public:
	///ZGSA_FUNCTION
	void    AddChairUsedDeposit(int chairno,int nAddDeposit);  //注意增量
	void    SetChairRoundDeposit(int chairno,int nDeposit);
    int     GetChairRestDeposit(int chairno);
	int     GetChairRoundDeposit(int chairno);
	
	void    SetChairStatus(int chairno,int status);
	int     GetChairStatus(int chairno);
	
	int     GetRoundMinDeposit();
	
	void    CalcFirstRound();
	DWORD   CalcPlayerCouldOpe(int nCurrentChair); 
	

	int     GetAliveUserCount();
	BOOL    IsRoundNeedOperation();
    int     GetNextOperatorChair(int nFromChair);
 	BOOL    OnShowCard(LPSHOW_CARD_REQUEST pShowCard, LPSHOW_CARD_RESPONSE pShowCardResponse);
	BOOL	OnGiveUp(LPGIVE_UP_REQUEST pGiveUp, LPGIVE_UP_RESPONSE pGiveUpResponse);
	BOOL	OnBet(LPBET_REQUEST pBet, LPBET_RESPONSE pBetResponse);
	BOOL	OnRise(LPBET_REQUEST pBet, LPBET_RESPONSE pBetResponse);
	BOOL	OnFollowBet(LPFOLLOW_BET_REQUEST pFollowBet, LPFOLLOW_BET_RESPONSE pFollowBetResponse);
	BOOL	OnCompare(LPCOMPARE_CARD_REQUEST pCompare, LPCOMPARE_CARD_RESPONSE pCompareResponse);
	BOOL	OnAllIn(LPALL_IN_REQUEST pAllIn, LPALL_IN_RESPONSE pAllInResponse);
	BOOL	OnBetFull(LPBET_FULL_REQUEST pBetFull, LPBET_FULL_RESPONSE pBetFullResponse);

	int     GetValidChairNO();

//重载区
	virtual	int   SetCurrentChairOnStart();
    virtual	DWORD SetStatusOnStart();
    virtual int   SetCurrentChair(int chairno,int nWaitSecond);
	virtual DWORD SetStatus(DWORD bits);
	virtual int   GetNextChair(int chairno);
	virtual int   GetPrevChair(int chairno);
	virtual int   GetBaseDeposit(int deposit_mult);
	virtual int   PlayerLeave(int nUserid,BOOL bDeletePtr=TRUE); 
		
public:
	//////////////////////////////////////////////////////////////////////////
	//重载区
	virtual void            ResetMembers(BOOL bResetAll = TRUE);
	virtual void			ResetPlayer(int chairno);//清除玩家信息
	//各种fill，需要重载
	virtual int				GetGameTableInfoSize();
	virtual int				GetGameStartSize();
	virtual int				GetGameWinSize();
	virtual int				GetEnterGameInfoSize();
	virtual void			FillupEnterGameInfo(void* pData, int nLen, int chairno, BOOL lookon = FALSE);
	virtual void			FillupGameTableInfo(void* pData, int nLen, int chairno, BOOL lookon = FALSE);
	virtual void			FillupGameStart(void* pData, int nLen, int chairno, BOOL lookon = FALSE);
	virtual int				FillupGameWin(void* pData, int nLen, int chairno);
	virtual void			FillupStartData(void* pData, int nLen);

 
	//选择重载
	virtual int				CalcBankerChairBefore();
	virtual int				CalcBankerChairAfter(void* pData, int nLen);
	virtual int				CalcResultDiffs(void* pData, int nLen, int nScoreDiffs[], int nDepositDiffs[]);
	virtual BOOL			CalcWinPoints(void* pData, int nLen, int chairno, int nWinPoints[]);
	virtual void			FillupNextBoutInfo(void* pData, int nLen, int chairno);

	 
	GAME_TABLE_INFO*	GetGameTableInfo();
	GAME_START_DATA*	GetStartData();
	GAME_PUBLIC_INFO*	GetPublicInfo();
	GAME_PLAYER_INFO*	GetPlayerInfo(int chairno);

			void		ConstructGameData();
			void		CalcZgsaFee(int nFee[TOTAL_CHAIRS]);
	virtual int			CalcBaseDeposit(int nDeposits[], int tableno);
	virtual void		StartDeal(); 
	virtual int			CalcSurplus(int nDepositDiffs[]);
	virtual int			CalcUserBoutEndDiff(int chairno, int &nWinPoint, int& nScoreDiff, int& nDepositDiff);
	virtual int			CompensateDeposits(int nOldDeposits[], int nDepositDiffs[]);
	virtual BOOL		IsNeedCountdown();
	virtual int			CalcDoubleOfScore(int chairno, int breakchair, int defdouble);
	virtual int			BreakDoubleOfDeposit(int defdouble);
	virtual int			CalcBreakDeposit(int breakchair, int breakdouble, int& cut);
	virtual int			ShareDepositOnBreak(int chairno, int breakchair, int dividend);

	//读取配置文件
			BOOL		ReadCardsFromFile();
			CString		GetINIFileName();
			int			RetrieveFields ( TCHAR *buf, TCHAR **fields, int maxfields, TCHAR**buf2 );
	
	//通讯相关
	virtual BOOL		IsGameMsg(UINT resquesID);//查询是不是游戏消息

			void		CalcBeginPlay(int chairno);
			int			CalcLoseChair(int chairno,int destchair);

			//做牌相关
			void		MakeDeal(int* pCards, int nLastDealCardIndex, int nInGamePlayerCount); 
			int			GetPlayerSameCardIndex(int nChairNo, int nCardID);
			BOOL		CheckCards(int* pCards);
			int			CalcMaxSingleCardValue(int nChairCards[CHAIR_CARDS], int nDefaultCardValue);

	DWORD   m_dwLastClockStop;
	int		m_nBetWait;
	int		m_nDiffDeposit;

	GAME_TABLE_INFO  *m_GameTalbeInfo;
};


//按项值从大到小排序
inline void  xyReversalMoreByValue(int array[],int value[],int length)
{
	int i,j,temp; 
	for(i=0;i<length-1;i++) 
		for(j=i+1;j<length;j++) /*注意循环的上下限*/ 
			if(value[i]<value[j]) 
			{ 
				temp=array[i]; 
				array[i]=array[j]; 
				array[j]=temp;
				temp=value[i]; 
				value[i]=value[j]; 
				value[j]=temp;
			}
}

inline void  xyRandomSort(int array[],int length,int seed)
{
	srand(seed);
	int* value=new int[length];
	int s=length*1000;
	for(int i=0;i<length;i++)
		value[i]=rand()%s;
	xyReversalMoreByValue(array,value,length);
	delete []value;
}

//从大到小排序
inline void xyReversalMore(int a[],int n)  
{  
	int i,j,temp; 
	for(i=0;i<n-1;i++) 
		for(j=i+1;j<n;j++) /*注意循环的上下限*/ 
			if(a[i]<a[j]) 
			{ 
				temp=a[i]; 
				a[i]=a[j]; 
				a[j]=temp;
			}
}  


//从大到小排序
inline void xyReversalLess(int a[],int n)  
{  
	int i,j,temp; 
	for(i=0;i<n-1;i++) 
		for(j=i+1;j<n;j++) /*注意循环的上下限*/ 
			if(a[i]>a[j]) 
			{ 
				temp=a[i]; 
				a[i]=a[j]; 
				a[j]=temp;
			}
}  

