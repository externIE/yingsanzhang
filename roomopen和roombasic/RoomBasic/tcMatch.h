#pragma once


#include         <map>
#include         <list>
#include		 <algorithm> 
#include		 <functional>

#define TCMATCH_WASHOUT_COEFFICIENT      1.5  //淘汰系数，打立出局的时候当剩余人数低于决赛人数的1.5倍时进入转阶段
#define TCMATCH_MAX_WASHTURN             7     //最多打7轮定局淘汰

#define TCMATCH_WAIT_NEXT_BOUT_SECOND    5     //5秒后重新分桌
#define TCMATCH_MAX_GAME_PLAY_SECOND     900   //一局游戏最多15分钟
#define TCMATCH_FRESHRANKLIST_SECOND     60    //最长60秒刷新一次积分列表

//玩家状态
#define TCMATCH_STATUS_WIN				 -4       //已晋级
#define TCMATCH_STATUS_OFFLINE           -3       //断线，注明此状态必然在等待分桌中,超过一定时间会被转换为GIVEUP  
#define TCMATCH_STATUS_GIVEUP			 -2       //放弃
#define TCMATCH_STATUS_WASHOUT			 -1       //淘汰
#define TCMATCH_STATUS_NOT_READY		 0       //尚未就绪
#define TCMATCH_STATUS_WAIT				 1       //等待确认
#define TCMATCH_STATUS_WAIT_NEXT_PHASE   2       //等待下一阶段
#define TCMATCH_STATUS_READY_NEXT_BOUT   3       //准备好进行下一局
#define TCMATCH_STATUS_PLAYING			 4       //比赛中
#define TCMATCH_STATUS_WAIT_RANK         6       //比赛已结束，等待其他玩家计算名次
//桌子状态
#define TCMATCH_TABLE_EMPTY              0      //未使用
#define TCMATCH_TABLE_PLAYING            1      //在打
#define TCMATCH_TABLE_WAIT_NEXT_BOUT     2      //等待下一局中



#define  TCMATCH_ANIMATCION_WAIT_FINAL   "animation_WaitFinal"
#define  TCMATCH_ANIMATCION_ARRAGETABLE  "animation_WaitArrangeTable"
#define  TCMATCH_ANIMATCION_WATI_RANK    "animation_WaitCaclRank"
#define  TCMATCH_ANIMATCION_WAITTALBE    "animation_WaitTable"   //轮空状态

#define  TCMATCH_MIN_SCORE              -100000000

#define	 ONE_GROUP_TABLE_NUM			5		//最大5个 一次探索包房
enum SYSTEMFINDPLAYERS_MODE{
	eSFP_MODE_MONEY,
	ESFP_MODE_BOUNT,
	ESFP_MODE_WINS,
	ESFP_MODE_MAX,//系统找人算法模式数量
};
typedef struct _tagTABLEAVGVALUE{
	int nAvgBounts;
	int nAvgWins;
	int nAvgScore;
	int nAvgDeposit;
}TABLEAVGVALUE, *LPTABLEAVGVALUE;
// typedef struct _tagMATCHDATA{      //此结构存放比赛相关配置数据
// 	int nMatchID;
//  	int nMatchType;
// 	int nSubType;
// 	int nStatus;
//    	DWORD dwOptions;
// 	DWORD dwConfigs;
//  
// 	int nGameID;
// 	int nAreaID;
// 	int nRoomID;
// 
// 	int nBigMatchID;
// 	int nPreviousID;
// 	int nLaunchDate;
// 	int nLaunchTime;
// 	int nMaxRunTime;
//  	int nMatchLevel;
// 
// 	int nInitScore;
// 	int nInitBaseScore; 
// 	int nInitOutScore;
// 	int nInitDeposit;
// 	int nInitBaseDeposit;
// 	int nInitOutDeposit;
// 	int nFixRings;
// 	int nFixBouts;
// 
//   	int nSignUpCount; //已报名人数
// 	int nMinRunners; //参赛人数下限
// 	int nMaxRunners; //参赛人数上限
// 	int nEndRunners;//比赛结束人数
// 
// 	int nBaseScore; // 基础分,可变
// 	int nWashScore; // 淘汰分,可变
// 
// 	DUAN_NEEDSCORE dns;
//     int nReserved[16];
// }MATCHDATA, *LPMATCHDATA;

typedef struct _tagMATCHDATA{      //此结构存放比赛相关配置数据
	MATCH					match;

	int						nBaseScore;					// 基础分,可变
	int						nWashScore;					// 淘汰分,可变

	DUAN_NEEDSCORE			dns;

	int                     nSignUpCount;               // 报名人数

    int						nReserved[16];
}MATCHDATA, *LPMATCHDATA;

typedef class CTcMatchPlayer
{
public:
	CTcMatchPlayer(LPMATCH_PLAYER lmp,LPUSERONMGAME lug,LPUSERONMATCH lum,LPMATCH_HINT lmh);
	~CTcMatchPlayer();
	
	unsigned long GetWaitSecond();    
			 BOOL IsPlaying();
			 BOOL IsQuit();
			 BOOL IsWaitNextPhase();   //等待下一阶段
			 BOOL IsOffline();         //掉线状态
			 BOOL IsReadyMatch();
			 void SetRank(int nRank);
			 int  GetRank();
			 void SetScore(int nScore);
			 int  GetScore();
			 void SetDeposit(int nDeposit);
			 int  GetDeposit();
			 int  GetUserID();
public:
	int			  m_nStatus;
	int           m_nRoomID;
	int           m_nTableNO;
	int           m_nChairNO;
	int           m_nMode;              //模组
	int           m_nParseBout;			//在本阶段的局数
	int			  m_nTotalBout;         //总局数,整个比赛
	BOOL          m_bReady;             //是否准备好比赛
	BOOL          m_bActive;            //活跃用户
	BOOL          m_bOffline;           //是否断线
	BOOL          m_bQuit;              //用户自主QuitGame
	int			  m_nLastPlaySecond;    //最后一次进入游戏的时间，用以计算等待时间
	int           m_nLastSendRankSecond;//最后一次发送排名列表的时间
	int			  m_nTotalWait;         //总等待时间
public:
//数据区
	MATCH_PLAYER  match_player;
	USERONMGAME   userongame;
	USERONMATCH   useronmatch;
	MATCH_HINT    match_hint;
}*LPCTcMatchPlayer;

class  CTcMatchBase;

typedef class CTcMatchTable
{
public: 
	CTcMatchTable(CTcMatchBase* pTcMatchBase,int nTableNO);
	CTcMatchTable(int nRoomID,int nTableNO,DWORD dwMatchOptions=MATCH_CFG_TURN_BANKER);
	~CTcMatchTable();
	
	BOOL		  PlayerEnter(CTcMatchPlayer* player);
	void          ResetTable(int nStatus=TCMATCH_STATUS_WAIT);
	BOOL          GetNextBout(LPARRANGE_TABLE pArrangeTable,int nBaseScore,int nLevelScore,int nMaxPlaySecond=60);
	int			  GetNextChair(int chairno);
	int			  GetPrevChair(int chairno);
	void		  RandomSortPlayer(LPCTcMatchPlayer array[],int length);
	DWORD         GetPlaySecond();//游戏进行的秒数
	DWORD         GetWaitSecond();//获取桌子等待开始的秒数
	BOOL          IsOverTime();//游戏超时
public://固定
 	int                 m_nTableNO;
	int					m_nPlayerCount;
public://可变
    int                 m_nStatus;
  	int                 m_nLastPlayStartTime;//
	int                 m_nLastPlayEndTime;

	int                 m_nBaseScore;
	int                 m_nWashScore;

 	int                 m_nBanker;              //庄家,userID
	int					m_nBout;				//玩了几局
	int					m_nPlanBout;			//预计玩几局
	int                 m_nMaxPlaySecond;
	LPCTcMatchPlayer    m_playerPtr[MAX_MATCHCHAIR_COUNT];
public:
	CTcMatchBase*       m_pTcMatchBase;

}*LPCTcTable;


#define  MATCH_STEP_WASHPLAYER    1
#define  MATCH_STEP_ARREAGETABLE  2
#define  MATCH_STEP_RANKLIST      3
#define  MATCH_STEP_FINIEDMATCH   4       //比赛已经结束等待消息
#define  MATCH_STEP_MATCHEND      5       //比赛结束
#define  MATCH_STEP_WAITFINAL     6       //等待晋级
#define  MATCH_STEP_USERSTATUS    7       //返回用户状态
#define  MATCH_STEP_OVERTIME      8       //游戏超时
#define  MATCH_STEP_DISBANDTABLE  9       //正常拆桌
#define  MATCH_STEP_RF_USERSCORE  10	  //刷新用户数据
#define  MATCH_STEP_TERMINATE_TABLE  11   //强制拆桌
#define  MATCH_STEP_BCWARSHTURN      12   //广播当前第几轮

typedef struct _tagMATCH_STEP
{
	int		nStepID;
	void*   pStepData;
	int     nDataLen;
	UINT    nReqID;
}MATCH_STEP,*LPMATCH_STEP;

typedef CMap<int, int, CTcMatchTable*, CTcMatchTable*> CTcMatchTableMap;
typedef CMap<int, int, CTcMatchPlayer*, CTcMatchPlayer*> CTcMatchPlayerMap;
typedef CList<MATCH_STEP*,MATCH_STEP*&> CTcStepList;

class CBaseRoomData;
class CTcMatchBase
{
public:
 	CTcMatchBase(CBaseRoomData* pRoomData,LPMATCH lpMatch);
	virtual ~CTcMatchBase();

	void					         ResetParameter();
	void                             SetOptions(DWORD dwOptions);
	LPCTcTable						 GetFreeTable();
	CTcMatchPlayer*                  GetPlayer(int nUserID);
	CTcMatchTable*                   GetTable(int nTableNO);
	
	void                             PushStep(int nStepID,void* pStepData);
	MATCH_STEP*                      PopStep();
	void                             ClearStep(MATCH_STEP* step);
	virtual  void					 NotifyMatchFinish(CTcMatchPlayer* pPlayer);
	virtual  void                    NotifyUserRankList(int nUserID);
	virtual  void                    NotifyUserStatus(int nUserID,LPCTSTR szStatus,BOOL bnAnimation,int nTablePlaying=0,int nPromotionCount=0);
	virtual  void                    NotifyWaitUserTableCount(int nTablePlayingCount,int nPromotionCount);//刷新当前等待下一阶段的用户，当前还有多少桌在玩
	virtual  void                    NotifyUserWaitTalbe();
	virtual  void                    PushWaitRankUser(int nUserID);//预备发送一个玩家的排名信息
	virtual  void                    SendWaitRankUser();//发送列表中所有排名信息

	virtual  void                    SortPlayerRank(MATCH_RANK rank[],int nCount);
	virtual  void                    CalcPlayersRank();
	virtual  int                     CalcRestUserCount();		//计算剩余可参赛玩家数量
	virtual  void                    CreateTableRankList(int nTalbeNO);
	virtual  void                    CreateTableRankList(int nUserIDs[],int nUserCount);
	virtual  void                    CreateUserRankList(int userid);
	virtual  void                    ReFreshUserScore();//刷新所有用户数据
	virtual  void                    BreakOffMatch();//重置比赛，在GameSvr中断的情况
	virtual  void                    ContinueMatch();//继续比赛，在GameSvr继续链接的情况
	virtual  int                     CalcWinPointByRank(int nUserID,int nRank);//根据名次计算奖励基础点
	virtual  int                     CalcBaseDuanScore();//根据比赛组织的情况返回段位奖励基础分

	virtual  ARRANGE_TABLE*          ArrageOneTable(CTcMatchTable* pTable,int nUserID[]);
	virtual  int                     GetWashOutRank();//注明，必须先调用此函数获得名次，再修改将要淘汰玩家的状态
	virtual  int                     GetTotalUserCount();//计算还有多少玩家存活
	virtual  int                     GetPlayingTableCount();//获取还在游戏中的桌子数
			 void                    CopyUserIDs(int* pUserIDs);
			 BOOL                    IsPlayerSeated(int nUserID);
protected:
	int						         CalcTableSort();
	void                             ClearMatch();
#ifdef _DEBUG
//记录两两用户被分到同桌的次数
 	std::map<CString,int>              m_mapSametable;
#endif
	virtual  void                    LogMatchInfo();//记录游戏信息
public:
   	int                              m_nChairPerTable;  //每桌多少玩家
 	MATCHDATA                        m_MatchData;
public: //可变
 
	int                              m_nUserMaxScore;     //当前玩家最大分数，可变
	int                              m_nPhaseTurn; //比赛各个阶段当前的轮数(CurrentMinUserTurn)，每个阶段新开始置0。
//	int                              m_nRestUserCount;//剩余未被淘汰的玩家
	int                              m_nMatchStatus;//比赛状态

	CList<int,int>                   m_WaitWaitRankList;
	MATCH_RANK*                      m_PlayerRank;
 	int                              m_nRankListCount;//记录指针实际长度，当该长度与比赛内总人数不同时，指针将删除重新申请空间
public:
 	CTcStepList			   		     m_listMatch;
	CTcMatchPlayerMap				 m_mapPlayer;
	CTcMatchTableMap                 m_mapTable;
	int                              m_nTableSortCount;//桌序号，记录一共分桌多少次
public://统计
	int                              m_nTotalPlaySeconds;//所有游戏耗费的总游戏时长
	int                              m_nMaxGamePlaySeconds;//单局游戏最长耗费时间
 	int                              m_nTotalUserWaitSeconds; //用户用于等待的总时间长
	int                              m_nMaxUserWaitSeconds; //用户等待的最大时间长度
	int                              m_nMaxUserWaitArrageTable;//用户单次等待分桌的最大时间长度
	int                              m_nMaxUserPlayBout;//用户参与最大局数
	int                              m_nMatchStartSecond; //比赛开始时间

	int                              m_nFinalBout;//剩余的定局淘汰场次
	int                              m_nWaitNextBoutSecond;//等待蚁一局开始的秒数
	int                              m_nMaxPlaySecond;//一局最长游戏时间
	int                              m_nWashOutIndex;//淘汰顺序

	int                              m_nCurrentUserWaitArrage;//当前用户等待分桌的time
	BOOL                             m_bGameSvrConnnet;//GameSvr是否连接状态
public:
	CBaseRoomData *                      m_pRoomData;
	void							 CopyMatchDetail(MATCH_DETAIL* lpMD,MATCH_DYNA* lpMD2);
    void                             RefreshDataFromStartToRoom(LPMATCH_STARTTOROOM lpMS);
	void                             RefreshDataFromMatchDynamic(LPMATCH_DYNA lpmatch_dynamic);
	int                              GetRoomID();
	int                              GetMatchID();
	int								 GetGameID();
	DWORD                            GetMatchConfig();
	int                              GetInitScore();
	int                              GetInitDeposit();

	int                              GetBaseScore();
	int                              GetWashScore();
	void                             SetBaseScore(int nBaseScore);
 	void                             SetWashScore(int nWashScore);
};



//比赛阶段
class CTCMatchPhase
{
public:
	 CTCMatchPhase(int nBoutPerTurn);
	 ~CTCMatchPhase();
public:
	virtual    void      TransPhase(); //清理前一个阶段
	virtual    void      BeginPhase(); //开始阶段
	virtual    int       CalcBaseScore();
	virtual    int       CalcLevelScore();
	virtual    BOOL      ValidatePlayerEnter(int nUserID);//判断玩家是否可以加入比赛
	virtual    void      InitialPlayerWhenEnter(CTcMatchPlayer* pPlayer);
	virtual    BOOL      IsPrepareEnterPhase();//是否满足条件进入转阶段
	virtual    BOOL      IsReadyEnterPhase();//是否条件允许进入阶段
	virtual    void      FillMatchDetail(LPARRANGE_TABLE pArrageTable);
	virtual    void      ArrageTable();
	virtual    void      ArrageOneTable(int nUserIDs[]);
	virtual    void      InitialMatchTable(CTcMatchTable* pTable);
	virtual    BOOL      DisBandTable(LPCOMMIT_DIFF pDisBandTable,CTcMatchTable* pTable);
	virtual    void      SetMatchPtr(CTcMatchBase* Ptr);
	virtual	   BOOL      IsPhaseEnd();
	virtual    BOOL      AutoKickOffliner();        //是否自动踢出掉线玩家
	virtual    int       GetStartUserCount();//返回本阶段启动时人数，无限制返回-1
	virtual    BOOL      IsGameInvalid(){return TRUE;}//本局是否有效
	virtual    void      SetPlayerStatusWhenLeave(CTcMatchPlayer* pPlayer);
			   BOOL      IsFirstPhase();
	int        m_nBoutPerTurn; //每轮打多少局
	int        m_nArrageTableTimes;//分桌次数
	class      CTcMatchBase*  m_MatchPtr;
	class      CTCMatchPhase* m_NextPhase;
};

//基础比赛类型：打立出局
//玩家参与的最小局数提高基础分，直到淘汰到剩下指定人数结束
class CTcMatch:public CTcMatchBase
{
public:
	CTcMatch(CBaseRoomData* pRoomData,LPMATCH lpMatch);
	virtual ~CTcMatch();
	void                             CreateMatch(LPMATCH_STARTTOROOM pStartMatch);
	void                             CreatePlayer(LPMATCH_PLAYER lmp,LPUSERONMGAME lug,LPUSERONMATCH lum,LPMATCH_HINT lmh);//玩家进入
	//消息处理
	void                             PushBackPhase(CTCMatchPhase* phase);
	void                             BeginMatch();//比赛游戏开始
	BOOL                             CommitResult(LPCOMMIT_DIFF pResult);//刷新结果
	void                             FreshUserResult(LPUSER_DIFF pResult);
	void                             DisBandTable(LPCOMMIT_DIFF pDisBandTable);//拆桌
	virtual  BOOL				     TerminateTable(int nTableNO);
	virtual  BOOL					 ForceKickPlayer(int nUserID);
	virtual  void                    OnPlayerReady(LPMATCHUSER_READY pPlayerReady);
	virtual  void                    OnPlayerOK(LPMATCH_PLAYER_OK pPlayerOk); //玩家获取自身状态
	virtual  void                    OnPlayerLeave(LPMATCHUSER_LEAVE pPlayerLeave);//玩家离开
//	virtual  void                    OnContinueMatch(LPCONTINUE_MATCH pContinueMatch);
	BOOL                             FreshMatch(); //定时刷新比赛状态

	virtual  BOOL                    IsMatchStart();            //计算比赛是否已经开始
	virtual  BOOL                    IsMatchEnd();				//计算比赛是否已经结束
	virtual  void                    OnMatchEnd();				//比赛结束处理
	virtual  void                    ClearMatch();              //数据清理

	virtual  void                    ArrageTable();				//分桌

	virtual  BOOL                    TransMatchPhase();         //转换比赛阶段
	virtual  void                    CheckTableStatus();        //检查每桌的游戏状况是否正常
	virtual  void                    CheckPlayerStatus();       //检查每个玩家的游戏状况

	virtual  void                    CalcBaseScore();
	virtual  void                    CalcLevelScore();

	CTCMatchPhase*                   m_CurrentPhase;
	CTCMatchPhase*                   m_FirstPhase;
};

//定局淘汰
class CTCMatchPhase2:public CTCMatchPhase
{
public:
	  CTCMatchPhase2(int nBoutPerTurn,int nChairPerTable,int nTotalPlayerCount);
	 ~CTCMatchPhase2();
public:
	virtual    void      TransPhase(); //清理前一个阶段
	virtual    void      BeginPhase(); //开始阶段

	virtual    int       CalcBaseScore();
	virtual    int       CalcLevelScore();
    virtual    BOOL      IsPrepareEnterPhase();
	virtual    BOOL      IsReadyEnterPhase();//是否条件允许进入阶段
	virtual    void      FillMatchDetail(LPARRANGE_TABLE pArrageTable);
	virtual    void      ArrageTable();
	virtual    void      InitialMatchTable(CTcMatchTable* pTable);
	virtual    BOOL      DisBandTable(LPCOMMIT_DIFF pDisBandTable,CTcMatchTable* pTable);
	virtual    int       GetStartUserCount();//返回本阶段启动时人数，无限制返回-1
	virtual    BOOL      AutoKickOffliner(); //是否自动踢出掉线玩家
	virtual    void      SetPlayerStatusWhenLeave(CTcMatchPlayer* pPlayer);

	int m_nWashTurn;       //淘汰轮次
	int m_nTransUserCount; //转阶段截止人数，剩余玩家少于这个人数后不再直接淘汰，等待最终名次决定晋级名单
	int m_nTurnIndex;      //淘汰第n轮
};

//循环积分赛
class CTCMatchPhase3:public CTCMatchPhase
{
public:
	CTCMatchPhase3(int nBoutPerTurn,int nMatchSeconds,int nWinBout);
	~CTCMatchPhase3();
public:
	virtual    void      TransPhase(); //清理前一个阶段
	virtual    void      BeginPhase(); //开始阶段
	virtual    int       CalcBaseScore();
	virtual    int       CalcLevelScore();
	virtual    BOOL      ValidatePlayerEnter(int nUserID);//判断玩家是否可以加入比赛
	virtual    void      InitialPlayerWhenEnter(CTcMatchPlayer* pPlayer);
	virtual    void      ArrageTable();
	virtual    void      InitialMatchTable(CTcMatchTable* pTable);
	virtual    BOOL      DisBandTable(LPCOMMIT_DIFF pDisBandTable,CTcMatchTable* pTable);
	virtual    BOOL      IsGameInvalid();//本局是否有效
	virtual    void      FillMatchDetail(LPARRANGE_TABLE pArrageTable);
public:
			   int       m_nMatchSeconds;
			   int       m_nWinBout;
};
