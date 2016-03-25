#pragma once
#define     GR_SENDMSG_TO_PLAYER   (GAME_REQ_BASE_EX + 29500)		//系统通知，转发其他玩家
#define     GR_SENDMSG_TO_SERVER   (GAME_REQ_BASE_EX + 29510)		//系统通知, 发送给系统
#define     GR_INITIALLIZE_REPLAY  (GAME_REQ_BASE_EX + 29520)		//初始化replay

/////////////////////////////////////////与RoomSvr的通讯自定义Windows消息(理论上应该从3000开始，不过为了统一与其它游戏的消息号)
#define     WM_GTR_CHANGE_SEATED_TO_LOOK_ON_EX			(WM_USER+3002)	//当用户从客户端主动变为旁观状态时，通知房间服务器去除上桌状态
#define     WM_GTR_CHANGE_LOOK_ON_TO_SEATED_EX			(WM_USER+3003)	//当用户从客户端主动变为上桌状态时，通知房间服务器去除旁观状态
#define     WM_GTR_CHANGE_SEATED_TO_LOOK_ON_RESULT_EX	(WM_USER+3004)	//接受RoomSvr推送的坐下变为旁观结果
#define     WM_GTR_CHANGE_LOOK_ON_TO_SEATED_RESULT_EX	(WM_USER+3005)	//接受RoomSvr推送的旁观变为坐下结果		
/////////////////////////////////////////		

//一共256+40BYTE
#define  GAME_MSG_DATA_LENGTH                      256
#define  GAME_MSG_SEND_EVERYONE                     -1 //包括自己,包括旁观
#define  GAME_MSG_SEND_OTHER                        -2 //除了自己,包括旁观
#define  GAME_MSG_SEND_EVERY_PLAYER                 -3 //发送给包括自己的其他玩家
#define  GAME_MSG_SEND_OTHER_PLAYER                 -4 //发送给包括自己的其他玩家
#define  GAME_MSG_SEND_VISITOR                      -5 //发送给所有旁观者

#define	 GR_SEATED_TO_LOOK_ON							(GAME_REQ_INDIVIDUAL + 1)	//坐下变为旁观
#define	 GR_LOOK_ON_TO_SEATED							(GAME_REQ_INDIVIDUAL + 2)	//旁观变为坐下
#define  UR_CHANGE_LOOKON_OR_SEATED_SPANSHORT			(GAME_REQ_INDIVIDUAL + 3)	//旁观/坐下请求频繁
#define  UR_CHANGE_LOOKON_OR_SEATED_SAME_STATUS			(GAME_REQ_INDIVIDUAL + 4)	//旁观/坐下相同状态
#define  UR_CHANGE_LOOKON_OR_SEATED_DEPOSIT_NOT_ENOUGH	(GAME_REQ_INDIVIDUAL + 5)	//坐下银两不足
#define  UR_CHANGE_LOOKON_OR_SEATED_FULL_LOOK_ON		(GAME_REQ_INDIVIDUAL + 6)	//旁观人数已达上限
#define  UR_CHANGE_LOOKON_OR_SEATED_NO_EMPTY_CHAIR		(GAME_REQ_INDIVIDUAL + 7)	//没有空余座位
#define  GR_CHANGE_SEATED_TO_LOOK_ON_RESULT				(GAME_REQ_INDIVIDUAL + 8)	//通知客户端坐下变为旁观的结果
#define  GR_CHANGE_LOOK_ON_TO_SEATED_RESULT				(GAME_REQ_INDIVIDUAL + 9)	//通知客户端旁观变为坐下的结果
#define  UR_ASK_NEW_TABLE_DEPOSIT_NOT_ENOUGH			(GAME_REQ_INDIVIDUAL + 10)	//换桌银两不足
#define  GR_HIDE_GIVE_UP_BUTTON							(GAME_REQ_INDIVIDUAL + 11)  //配置是否显示棋牌按钮

enum GAMEMSG{ ///+
    	SYSMSG_BEGIN=19840323,
		SYSMSG_RETURN_GAME,            //
		SYSMSG_PLAYER_ONLINE,          //玩家在线
		SYSMSG_PLAYER_OFFLINE,         //有人掉线了
		SYSMSG_GAME_CLOCK_STOP,        //游戏时钟停止。停止5秒时发送该请求,
		SYSMSG_GAME_DATA_ERROR,        //服务器端通知客户端数据消息有异常
		SYSMSG_GAME_ON_AUTOPLAY,       //客户端托管
		SYSMSG_GAME_CANCEL_AUTOPLAY,   //托管中止
		SYSMSG_GAME_WIN,               //游戏结束
		SYSMSG_GAME_TEST,
		SYSMSG_END,
		//游戏消息，注与客户端共通,必须是流程消息，会保存到replay
		LOCAL_GAME_MSG_BEGIN, 
		GAME_MSG_OPERATION, 
	 	GAME_MSG_SHOW_CARD, //亮牌 
		GAME_MSG_LOOK_CARD, //看牌 
		GAME_MSG_GIVE_UP,	//弃牌
		GAME_MSG_BET,		//下注
		GAME_MSG_RISE,		//加注
		GAME_MSG_FOLLOW,	//跟注
		GAME_MSG_COMPARE,	//比牌
		GAME_MSG_ALL_IN,	//全下
		GAME_MSG_BET_FULL,	//押满
		LOCAL_GAME_MSG_END,
};


typedef struct _tagGAME_MSG///+
{
	int   nRoomID;
	int   nUserID;					  // 用户ID            4
	int   nMsgID;				      // 消息号            4
	int   nVerifyKey;                 // 验证码            4
	int   nDatalen;					  // 数据长度          4
}GAME_MSG,*LPGAME_MSG;

typedef struct _tagGAME_ENTER_INFO{
	ENTER_INFO     ei;
	int            nResultDiff[MAX_CHAIR_COUNT][MAX_RESULT_COUNT];
	int			   nTotalResult[MAX_CHAIR_COUNT];
	int            nReserve[4];
}GAME_ENTER_INFO,*LPGAME_ENTER_INFO;

typedef struct _tagGAME_START_DATA{  
	TCHAR	szSerialNO[MAX_SERIALNO_LEN];//本局序列号
	int		nBoutCount;				// 第几局
	int		nBaseDeposit;			// 基本银子
	int		nBaseScore;				// 基本积分
	BOOL	bNeedDeposit;			// 是否需要银子
	BOOL	bForbidDesert;			// 禁止强退 
    int     nBetWaitTime; 
}GAME_START_DATA, *LPGAME_START_DATA;

//游戏公共信息,所有玩家都可见的信息
typedef struct _tagGAME_PUBLIC_INFO
{
	int			nBanker;					// 庄家，初始玩家
	DWORD		dwStatus;					// 当前状态 
	int			nCurrentChair;				// 当前活动椅子号
    DWORD		dwCouldOpe;					// 当前可能进行操作  加注 跟 让 放弃 
	int			nRoundMinDeposit;			// 最小下注 
    int			nTotalDeposit;				// 所有玩家下注总和
	BOOL		bOpenBetFull;				// 是否支持押满
    int			nRoundStartChair;			// 本轮操作开始椅子号 本轮操作到开始椅子号前一位截止 动态变化  
	int			nRoundCurDeposit;			// 本轮当前最大注 
	int			nTableNO;

	int         nCurrentDouble;				// 当前倍数
    int			nKiller[TOTAL_CHAIRS];		// 小于0，是自己放弃，否则为拼牌胜利者ID
	BOOL        bnLookCard[TOTAL_CHAIRS];	// 是否看过牌
	int			nFee;						// 本局茶水费
	int			nReserved[5];				// 保留字段
	int			nCurrentBet[TOTAL_CHAIRS];	// 当前下了多少注，注数×基础银+当前剩余银子=实际拥有的银子
}GAME_PUBLIC_INFO,*LPGAME_PUBLIC_INFO;

//玩家私人信息，只有自己可见
typedef struct _tagGAME_PLAYER_INFO
{
	int		nUserID;
	int		nChairCards[CHAIR_CARDS];   // 自己手里的牌 
	int		nCardType;					//手牌类型
	int		nCardsWeight;				//手牌权值
	int		nPlayerStatus;				// 状态 ：0  下注 跟1 让2 加3 全下4 盖牌5   
	int		nUsedDeposit;  
	int		nRoundDeposit;				//在本轮中的注 
	int		nTotalDeposit;				//持有的总可用银两，会和携带银两不一样
	BOOL	bInGame; 
	int		nPlayerDeposit;				//携带银两（减去茶水费）
    int		nReserved[3];
}GAME_PLAYER_INFO,*LPGAME_PLAYER_INFO;


typedef struct _tagGAME_START_INFO{  
    GAME_START_DATA  startData; 
 	//公共信息
	GAME_PUBLIC_INFO    m_Public;
	//玩家信息
	GAME_PLAYER_INFO    m_Player[TOTAL_CHAIRS];
	
}GAME_START_INFO, *LPGAME_START_INFO;

typedef struct _tagGAME_TABLE_INFO{
	GAME_START_DATA		startData;
	//公共信息
	GAME_PUBLIC_INFO    m_Public;
	//玩家私密信息
	GAME_PLAYER_INFO    m_Player[TOTAL_CHAIRS];


	int              nOpeRestTime;//余下操作时间
	//保留字
	int					nReserved[4];
}GAME_TABLE_INFO, *LPGAME_TABLE_INFO;

typedef struct _tagPLAYER_WIN_INFO
{
	//游戏结果 
	int          nChairCards[3];
	int          nTotalWinDeposit;
	int          nMaxCards[5];
	int          nMaxCardType;
}PLAYER_WIN_INFO;

typedef struct _tagGAME_WIN_RESULT{
	GAME_WIN	gamewin;
	PLAYER_WIN_INFO playerWinInfo[TOTAL_CHAIRS];
	int         nWinChair;   //赢家
	DWORD       nWinType;  //赢家牌型
	int			nChairCards[TOTAL_CHAIRS][CHAIR_CARDS];
	int			nKiller[TOTAL_CHAIRS];
	
	BOOL		bGameEndAhead; //游戏提前结束
	int			nTableNO;
}GAME_WIN_RESULT, *LPGAME_WIN_RESULT;

typedef struct _tagZGSA_OPERATION
{
	DWORD gameOpe;
	int chairno;
	int destchairno;
	int lostchairno;
	int nAddBet; //加 
	
	//返回
	int   nChairUsedBet; 
    int   nChairRoundBet; 
	int   nChairRestBet;  //余下注
	int   chairStatus;
	//	BOOL  bReRaise; //是否重加注
	
	int   nTotalDeposit; 
	int   nCurrentDouble;
	int	  nCurrentBet[TOTAL_CHAIRS];
	BOOL  bRoundEnd;
	int   nRoundCurDeposit;
	int   nNextChair;
	DWORD dwNextCouldOpe;  
}ZGSA_OPERATION;

typedef struct _tagZGSA_NEW_ROUND
{
	BOOL  bRoundEnd;
	
	int   nRound;
	int   nNextChair;
	DWORD dwNextCouldOpe;  
	int   nPubCard[PUBCARD_COUNT];

	int   nChairCards[TOTAL_CHAIRS][CHAIR_CARDS];
}ZGSA_NEW_ROUND;


typedef struct _tagZGSA_SHOWCARD
{
	int		chairno;
	BOOL	bShow;
	int		nChairCards[CHAIR_CARDS];
	int		dwOPE;              //该玩家可以进行的操作
}ZGSA_SHOWCARD,*LPZGSA_SHOWCARD;


//看牌、亮牌请求
typedef struct _tagSHOW_CARD_REQUEST
{
	DWORD	dwGameOpe;
	int		nChairNo;
}SHOW_CARD_REQUEST,*LPSHOW_CARD_REQUEST;

//看牌、亮牌应答
typedef struct _tagSHOW_CARD_RESPONSE
{
	int		nChairNo;
	int		nChairCards[CHAIR_CARDS];
	DWORD	dwNextCouldOpe; 
}SHOW_CARD_RESPONSE,*LPSHOW_CARD_RESPONSE;

//下注、加注请求
typedef struct _tagBET_REQUEST
{
	DWORD	dwGameOpe;
	int		nChairNo;
	int		nBet;
}BET_REQUEST, *LPBET_REQUEST;

//下注、加注应答
typedef struct _tagBET_RESPONSE
{
	int		nChairNo;
	int		nBet;
	int		nNextChair;
	DWORD	dwNextCouldOpe;  
}BET_RESPONSE, *LPBET_RESPONSE;

//跟注请求
typedef struct _tagFOLLOW_BET_REQUEST
{
	DWORD	dwGameOpe;
	int		nChairNo;
}FOLLOW_BET_REQUEST, *LPFOLLOW_BET_REQUEST;

//跟注应答
typedef struct _tagFOLLOW_BET_RESPONSE
{
	int		nChairNo;
	int		nBet;
	int		nNextChair;
	DWORD	dwNextCouldOpe;  
}FOLLOW_BET_RESPONSE, *LPFOLLOW_BET_RESPONSE;

//弃牌请求
typedef struct _tagGIVE_UP_REQUEST
{
	DWORD	dwGameOpe;
	int		nChairNo;
}GIVE_UP_REQUEST, *LPGIVE_UP_REQUEST;

//弃牌应答
typedef struct _tagGIVE_UP_RESPONSE
{
	int		nChairNo;
	BOOL	bNextChair;
	int		nNextChair;
	DWORD	dwNextCouldOpe;  
}GIVE_UP_RESPONSE, *LPGIVE_UP_RESPONSE;

//比牌请求
typedef struct _tagCOMPARE_CARD_REQUEST
{
	DWORD	dwGameOpe;
	int		nChairNo;
	int		nGoalChairNo;
}COMPARE_CARD_REQUEST, *LPCOMPARE_CARD_REQUEST;

//比牌应答
typedef struct _tagCOMPARE_CARD_RESPONSE
{
	int		nChairNo;
	int		nGoalChairNo;
	int		nLostChairNo;
	int		nBet;
	BOOL	bNextChair;
	int		nNextChair;
	DWORD	dwNextCouldOpe;  
}COMPARE_CARD_RESPONSE, *LPCOMPARE_CARD_RESPONSE;

//全下请求
typedef struct _tagALL_IN_REQUEST
{
	DWORD	dwGameOpe;
	int		nChairNo;
}ALL_IN_REQUEST, *LPALL_IN_REQUEST;

//全下应答
typedef struct _tagALL_IN_RESPONSE
{
	int		nChairNo;
	int		nDeposit;
	BOOL	bNeedCompare[TOTAL_CHAIRS];
	BOOL	bWinOrLose[TOTAL_CHAIRS];
	BOOL	bNextChair;
	int		nNextChair;
	DWORD	dwNextCouldOpe;  
}ALL_IN_RESPONSE, *LPALL_IN_RESPONSE;

//押满请求
typedef struct _tagBET_FULL_REQUEST
{
	DWORD	dwGameOpe;
	int		nChairNo;
}BET_FULL_REQUEST, *LPBET_FULL_REQUEST;

//押满应答
typedef struct _tagBET_FULL_RESPONSE
{
	int		nChairNo;
	int		nDeposit;
	BOOL	bNextChair;
	int		nNextChair;
	int		nGoalCompareChair;
	BOOL	bWinOrLose;
	DWORD	dwNextCouldOpe;  
}BET_FULL_RESPONSE, *LPBET_FULL_RESPONSE;

//旁观/坐下状态切换
typedef struct _tagLOOK_ON_AND_SEATED_CHANGE
{
	int nUserID;								// 用户ID
	int nRoomID;								// 房间ID
	int nTableNO;								// 桌号
	int nChairNO;								// 位置
	int nReserved[4];
}LOOK_ON_AND_SEATED_CHANGE, *LPLOOK_ON_AND_SEATED_CHANGE;

//旁观/坐下状态切换结果
typedef struct _tagLOOK_ON_AND_SEATED_CHANGE_RESULT
{
	int		nUserID;							// 用户ID
	int		nChairNO;							// 位置
	BOOL	bSucceed;
	int		nTableNO;
	int		nReserved[3];
}LOOK_ON_AND_SEATED_CHANGE_RESULT, *LPLOOK_ON_AND_SEATED_CHANGE_RESULT;


typedef struct _tagPlayerRecord
{
    int 	nPlayerID;										//用户ID
    DWORD	dwPlayerWin;									//玩家赢得银两
    DWORD	dwPlayerBout;									//今日玩家局数
    DWORD	dwPlayerFee;									//今日玩家贡献茶水费
    DWORD   dwPlayerDeposit;                                //这一局结束玩家身上银子
    bool operator<(const struct _tagPlayerRecord& ti) const 
    {  
        return dwPlayerWin < ti.dwPlayerWin;
    }
}PLAYER_RECORD_ST,* PPLAYER_RECORD_ST;						//每个玩家信息记录

typedef std::list<PLAYER_RECORD_ST> PLAYERRECORDLIST;		//每个玩家信息记录列表


typedef struct _tagRoomRecord
{
    int		nRoomID;										//房间ID
    DWORD	dwRoomFee;										//今日房间茶水费总额
}ROOM_RECORD_ST,*PROOM_RECORD_ST;							//每个房间信息记录

typedef std::map<int,ROOM_RECORD_ST> ROOMRECORDMAP;			//每个房间信息记录列表
