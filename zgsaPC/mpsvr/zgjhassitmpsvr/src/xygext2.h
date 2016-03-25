#ifndef _XYGEXT2_H_
#define _XYGEXT2_H_

/*
#define		WM_USER							0x0400

#define     GAMESVR_CLSNAME_EX              _T("TCYX_GAMESVR_CLSNAME")
#define     GAMESVR_WNDNAME_EX              _T("TCYX_GAMESVR_WNDNAME")
#define     WM_RTG_RECONNECTSVR_EX          (WM_USER+999)
#define     WM_RTG_GETFINISHED_EX           (WM_USER+1000)
#define     WM_RTG_CLEARTABLE_EX            (WM_USER+1001)
#define     WM_RTG_HARDCODE_EX              (WM_USER+1002)
#define     WM_RTG_TOKENID_EX               (WM_USER+1003)
#define		WM_RTG_ROOMTABLECHAIR_EX		(WM_USER+1004)
#define     WM_RTG_EXPERIENCE_EX			(WM_USER+1005)
#define     WM_RTG_LEAVEROOM_EX             (WM_USER+1006)
#define		WM_RTG_ROOMSVRHWND_EX			(WM_USER+1009)
#define     WM_RTG_ROOMOPTIONS_EX           (WM_USER+1010)  
#define     WM_RTG_ROOMCONFIGS_EX           (WM_USER+1011)
#define     WM_RTG_ROOMMANAGES_EX           (WM_USER+1012)  
#define     WM_RTG_GAMEOPTIONS_EX           (WM_USER+1013)  
#define     WM_RTG_GAMEPARAM_EX             (WM_USER+1014)  
#define     WM_RTG_GAMEDATA_EX              (WM_USER+1015)  
#define     WM_RTG_MINPLAYSCORE_EX          (WM_USER+1025)  
#define     WM_RTG_MAXPLAYSCORE_EX          (WM_USER+1026)  
#define     WM_RTG_MINDEPOSIT_EX			(WM_USER+1027)  
#define     WM_RTG_MAXDEPOSIT_EX			(WM_USER+1028) 
#define     WM_RTG_STARTVALID_EX            (WM_USER+1030)  
#define     WM_RTG_STARTINVALID_EX          (WM_USER+1031)

#define     WM_RTG_STARTSOLOTABLE_EX		(WM_USER+1040)  
#define     WM_RTG_KICKOFFPLAYER_EX			(WM_USER+1050)

#define		WM_GTR_ENTERGAMEOK_EX			(WM_USER+2000)
#define		WM_GTR_VERIFYSTART_EX           (WM_USER+2001)
#define     WM_GTR_LEAVEGAMEOK_EX			(WM_USER+2002)
#define		WM_GTR_GAMESTARTUP_EX			(WM_USER+2003)
#define		WM_GTR_GAMEBOUTEND_EX			(WM_USER+2004)
#define     WM_GTR_ASKNEWTABLE_EX 		    (WM_USER+2005)
#define     WM_GTR_CLOSESOLOTABLE           (WM_USER+2010)
*/


#define		FEE_MODE_FREE					0			// 免收
#define		FEE_MODE_FIXED					1			// 固定
#define		FEE_MODE_TEA					6			// 茶水
#define		FEE_MODE_SERVICE				7			// 服务费模式，抽取每位玩家固定数量服务费，每局付费后，再结算输赢
#define		FEE_MODE_SERVICE_MINDEPOSIT		8			// 服务费模式，按最小银两玩家的万分比收取茶水费
#define		FEE_MODE_SERVICE_SELFDEPOSIT	9			// 服务费模式，按各自银两的万分比收取茶水费

#define		MAX_FILTERWORD_LEN		32			// 过滤词最大长度
#define		MAX_LEVELNAME_LEN_EX	16			// 级别名称长度
#define		MAX_HARDID_LEN_EX		32			// 硬件标识ID最大长度
#define		MAX_SERVERIP_LEN_EX		32			// 服务器IP最长长度(含NULL)
#define		CLIENT_TYPE_GAME_EX		4			// 游戏服务器
// 
#define		RF_VERIFY_SCORE			0x00000001			// 从数据库校验积分结果
#define		RF_LOG_TO_DB			0x00000002			// 数据库记录游戏结果
#define		RF_VERIFY_DEPOSIT		0x00000004			// 从数据库校验银子结果
#define		RF_IGNORE_DIFFSUM		0x00000008			// 校验银子结果时忽略差值总和校验(水草,对局奖)-不支持此项FLAG

/////////////////////////////////////////////////////////////////////////////
// req id from 20000 to 30000 (compact for commreq.h of version 6.0)
// 

#define		COMMON_REQ_BASE_EX		20000

#define		GR_RECONNECT_SVR		(COMMON_REQ_BASE_EX + 12) 

/////////////////////////////////////////////////////////////////////////////

#define		GAME_REQ_BASE_EX		200000
#define		GAME_REQ_INDIVIDUAL		400000

#define		ROOM_REQ_BASE			50000         //使用与RoomSvr一样的区间
#define     IR_REQ_BASE             90000		  //与insidereq.h一致

// request (to check server)
#define		GR_GET_PLAYERLEVELS_EX	(GAME_REQ_BASE_EX + 10)		// 获取级别信息
#define		GR_SEND_PULSE_EX		(GAME_REQ_BASE_EX + 20)		// 发送脉搏信号
#define		GR_REFRESH_RESULT_EX	(GAME_REQ_BASE_EX + 30)		// 更新游戏结果

#define		GR_ENTER_GAME_EX		(GAME_REQ_BASE_EX + 110)	// 用户进入游戏
#define		GR_VALIDATE_GAMESVR_EX	(GAME_REQ_BASE_EX + 200)	// 连接帐户服务

#define     GR_SEND_GAMERUNLOG_EX  	(GAME_REQ_BASE_EX + 210)	// 发送服务器运行日志

#define		GR_GET_FILTERWORD_EX	(ROOM_REQ_BASE + 22)		// 获取过滤关键字

// response (from check server)
#define		GR_ENTER_GAME_OK_EX		(GAME_REQ_BASE_EX + 500)	// 进入游戏成功

#define		GR_ASK_NEW_TABLECHAIR	 (GAME_REQ_BASE_EX+1300)		// 玩家请求换座位
#define		GR_LOOK_SAFE_DEPOSIT	(GAME_REQ_BASE_EX + 40)		// 查看保险箱银子
#define		GR_TAKE_SAFE_DEPOSIT    (GAME_REQ_BASE_EX + 41)		// 取保险箱银子(到游戏)
#define		GR_SAVE_SAFE_DEPOSIT	(GAME_REQ_BASE_EX + 42)		// 存到保险箱
#define		GR_TAKE_SAFE_RNDKEY    	(GAME_REQ_BASE_EX + 43)		// 获取随机码
#define		GR_LOOK_BACKDEPOSIT_INGAME	(GAME_REQ_BASE_EX + 44)		// 查看后备箱银子
#define		GR_TAKE_BACKDEPOSIT_INGAME  (GAME_REQ_BASE_EX + 45)		// 取后备箱银子(到游戏)
#define		GR_SAVE_BACKDEPOSIT_INGAME	(GAME_REQ_BASE_EX + 46)		// 存到后备箱
#define     GR_TAKE_DEPOSIT_BONUS	(GAME_REQ_BASE_EX + 50) // 领银子奖励(到游戏)

// error response
#define		GR_SERVICE_BUSY		    (ROOM_REQ_BASE + 998)		// 服务繁忙
#define		GR_DEPOSIT_NOTENOUGH	(ROOM_REQ_BASE + 10005)		// 银子不够
#define		GR_NO_THIS_FUNCTION 	(ROOM_REQ_BASE + 10017)		// 该功能暂不支持
#define		GR_SYSTEM_LOCKED      	(ROOM_REQ_BASE + 10019)		// 系统维护中
#define		GR_TOKENID_MISMATCH    	(ROOM_REQ_BASE + 10023)		// token不匹配
#define		GR_HARDID_MISMATCHEX    (ROOM_REQ_BASE + 10027)		// 硬件认证失败		//与之前使用的名字相同，现在加上EX
#define		GR_CONTINUE_PWDWRONG	(ROOM_REQ_BASE + 10029)	    // 同一台机器不能连续密码错误
#define		GR_ERROR_INFOMATION		(ROOM_REQ_BASE + 10031)	    // 返回错误信息
#define     GR_PWDLEN_INVALID		(ROOM_REQ_BASE + 10038)     // 密码长度无效
#define     GR_NEED_LOGON           (ROOM_REQ_BASE + 10039)	    // 用户需要登陆先
#define     GR_ROOM_NOT_EXIST       (ROOM_REQ_BASE + 10040)	    // 房间不存在
#define     GR_NODEPOSIT_GAME       (ROOM_REQ_BASE + 10042)     // 该游戏不能存放银子
#define     GR_ROOM_CLOSED          (ROOM_REQ_BASE + 10050)		// 房间已关闭
#define     GR_BOUT_NOTENOUGH       (ROOM_REQ_BASE + 10070)     //   
#define     GR_TIMECOST_NOTENOUGH   (ROOM_REQ_BASE + 10071)     //
#define     GR_ROOM_NOT_OPENED      (ROOM_REQ_BASE + 10078)		// 房间未开放
#define     GR_NEED_PLAYING		    (ROOM_REQ_BASE + 10084)     // 需要正在玩才能继续
#define     GR_NO_MOBILEUSER	    (ROOM_REQ_BASE + 10085)     // 不是手机用户
#define		GR_INPUTLIMIT_DAILY		(ROOM_REQ_BASE + 10086)		// 划银每日限制
#define		GR_KEEPDEPOSIT_LIMIT	(ROOM_REQ_BASE + 10092)		// 游戏银子转出限制
#define		GR_INPUTLIMIT_MONTHLY   (ROOM_REQ_BASE + 10095)		// 划入每月限制
#define		GR_USER_FORBIDDEN		(ROOM_REQ_BASE + 10101)		// 用户已被禁用

// notification  (from check server)
#define		GR_ENTER_ROOM_NOTIFY	(GAME_REQ_BASE_EX + 600)
#define		GR_RESULT_UNSAVED	    (GAME_REQ_BASE_EX + 610)

#define     GR_ADD_FILER_WORD_EX        (IR_REQ_BASE + 110)		 //添加过滤关键字
#define     GR_DELETE_FILTER_WORD_EX    (IR_REQ_BASE + 209)	     //删除过滤关键字

/************************************************************************/
/* AsitSvr/ChunkSvr Common                                                                  
/************************************************************************/
#define		GAME_REQ_BASE			50000					    // 此数据值必须固定,应对版本升级
#define		GR_QUERY_PLAYERLOGON	(GAME_REQ_BASE + 31004)		// 查询玩家登录信息

#define		FLAG_SUPPORT_KEEPDEPOSIT_EX		0x00000001	//客户端存银子请求如果带上这个标记，表示客户端支持了GR_KEEPDEPOSIT_LIMIT
#define		FLAG_SUPPORT_MONTHLY_LIMIT_EX	0x00000002	//客户端取银子请求如果带上这个标记，表示客户端支持了GR_INPUTLIMIT_MONTHLY

// compacted to existed check server
typedef struct _tagGET_PLAYERLEVELS_EX{
	int nGameID;								// 游戏ID
	int nLevelID;								// 级别ID
	int nReserved[4];
}GET_PLAYERLEVELS_EX, *LPGET_PLAYERLEVELS_EX;

typedef struct _tagPLAYERLEVELS_EX{
	int nLevelCount;							// 级别个数
	int nGameID;								// 游戏ID
	int nScoreMult;								// 相对倍数
	int nReserved[6];
}PLAYERLEVELS_EX, *LPPLAYERLEVELS_EX;

typedef struct _tagPLAYERLEVEL_EX{
	int nLevelID;								// 级别ID
	char szLevelName[MAX_LEVELNAME_LEN_EX];	// 级别名称
	int nReserved[4];
}PLAYERLEVEL_EX, *LPPLAYERLEVEL_EX;

typedef struct _tagREFRESH_RESULT_EX{
	int nResultCount;							// 结果个数
	int nRoomID;								// 房间ID
	int	nGameID;								// 游戏ID
	int nClientID;								// 发送端ID
	T_DWORD dwFlags;	
	int nTableNO;								// 桌号
	T_DWORD dwResetTime;							// 上次清桌时间
	int nReserved[2];
}REFRESH_RESULT_EX, *LPREFRESH_RESULT_EX;

typedef struct _tagGAME_RESULT_EX{
	int	nUserID;								// 用户ID
	int nRoomID;								// 房间ID
	int nTableNO;								// 桌号
	int nChairNO;								// 位置
	int	nGameID;								// 游戏ID
	int	nBaseScore;								// 基本积分
	int	nBaseDeposit;							// 基本银子
	int	nOldScore;								// 旧积分
	int	nOldDeposit;							// 旧银子
	int	nExperience;							// 经验值(分钟)
	int	nTimeCost;								// 耗时(秒)
	int	nBout;									// 总回合
	int	nBreakOff;								// 断线次数
	int	nWin;									// 赢(次数)
	int	nLoss;									// 输(次数)
	int	nStandOff;								// 和(次数)
	int	nScoreDiff;								// 积分增减	
	int	nDepositDiff;							// 银子输赢
	int	nLevelID;								// 级别ID
	char szLevelName[MAX_LEVELNAME_LEN_EX];	    // 级别名称
	int nFee;									// 手续费
	int nCut;									// 逃跑扣银
	int nExtra;									// 额外银子(水草,对局费等)
	int nReserved[3];
}GAME_RESULT_EX, *LPGAME_RESULT_EX;

typedef struct _tagENTER_GAME_EX{
	int nUserID;								// 用户ID
	int nUserType;								// 用户类型
	int	nGameID;								// 游戏ID
	int nRoomID;								// 房间ID
	int nTableNO;								// 桌号
	int nChairNO;								// 位置
	char szHardID[MAX_HARDID_LEN_EX];			// 硬件标识
	T_BOOL bLookOn;								// 旁观
	int nReserved[3];
	T_DWORD dwUserConfigs;						// 游戏设置
	int nReserved2[3];
	int nRoomTokenID;							// 房间令牌ID
	int nMbNetType;
	int nReserved3[3];
}ENTER_GAME_EX, *LPENTER_GAME_EX;

typedef struct _tagENTER_GAME_OK_EX{
	int nUserID;								// 用户ID
	int nRoomID;								// 房间ID
	int nTableNO;								// 桌号
	int nChairNO;								// 位置
	int nScore;									// 积分
	int	nDeposit;								// 银子
	int nLevelID;								// 级别ID
	char szLevelName[MAX_LEVELNAME_LEN_EX];	    // 级别名称
	int nUserType;								// 用户类型
	int nExperience;							// 经验值
	int nBout;		                            // 玩家游戏总局数
	int nReserved[1];
}ENTER_GAME_OK_EX, *LPENTER_GAME_OK_EX;

typedef struct _tagVALIDATE_GAMESVR{
	int nClientID;
	int nClientType;
	int nGameID;
	int nGamePort;
	T_DWORD dwIPAddr;
	int nReserved[6];
}VALIDATE_GAMESVR, *LPVALIDATE_GAMESVR;

typedef struct _tagENTER_ROOM_NOTIFY{
	int nUserID;								// 用户ID
	int nGameID;								// 游戏ID
	int nRoomID;								// 房间ID
    int nReserved[4];
}ENTER_ROOM_NOTIFY, *LPENTER_ROOM_NOTIFY;

typedef struct _tagRECONNECT_SVR{	
 	int   nAdminID;
	char szHardID[MAX_HARDID_LEN_EX];	 
  	char szReconnectIP[MAX_SERVERIP_LEN_EX];
	int   nReconnectPort;
   	int nReserved[4];
}RECONNECT_SVR, *LPRECONNECT_SVR;

//typedef CMap<int, int, PLAYERLEVEL_EX*, PLAYERLEVEL_EX*> CPlayerLevelExMap;


#define     RUCID_DATABASEERROR      1
#define     RUCID_INVALIDPARAM		 2
#define     RUCID_OBJECTNOTFIND      3
#define     RUCID_SCOREDIFFER       10
#define     RUCID_DEPOSITDIFFER     11
#define     RUCID_FORBIDDENUSER     20
typedef struct _tagNTF_RESULT_UNSAVED{
	int nCauseID; 
	int nGameID;
	int nRoomID;
	int nUserID;
	int nTableNO;
	T_DWORD dwFlags;
	T_DWORD dwResetTime;
	int nReserved[2];
}NTF_RESULT_UNSAVED, *LPNTF_RESULT_UNSAVED;



typedef struct _tagNTF_RESULT_CHECKED{
	int nResultCount;							// 结果个数
	int nPropsCount;							// 道具发送
	int nClientID;								// 发送端ID
	
	int nRoomID;								// 房间ID
	int	nGameID;								// 游戏ID
	int nTableNO;								// 桌号
	T_DWORD dwFlags;	
	T_DWORD dwResetTime;							// 上次清桌时间
	
	int nReserved[7];
}NTF_RESULT_CHECKED, *LPNTF_RESULT_CHECKED;

typedef struct _tagNTF_CHAIR_RESULT{
	int nChairNO;								// 位置
	int	nUserID;								// 用户ID
	int	nTimeCost;								// 耗时(秒)
	int	nOldScore;								// 旧积分
	int	nOldDeposit;							// 旧银子
	int	nScoreDiff;								// 积分增减	
	int	nDepositDiff;							// 银子输赢
	int	nLevelID;								// 级别ID
	int nFee;									// 手续费
	int nCut;									// 逃跑扣银
	int nExtra;									// 额外银子(水草,对局费等)
	int nReserved[3];
}NTF_CHAIR_RESULT, *LPNTF_CHAIR_RESULT;

typedef struct _tagFILTER_WORD_EX{
	int	nWordID;
	int nType;
    char szWord[MAX_FILTERWORD_LEN];
	int nReserved;
}FILTER_WORD_EX, *LPFILTER_WORD_EX;

typedef struct _tagADD_FILTER_WORD_EX{
	int nClientID;
	int nAdminID;
	char szHardID[MAX_HARDID_LEN_EX];				 
    FILTER_WORD_EX  fw; 
   	int nReserved[4];
}ADD_FILTER_WORD_EX, *LPADD_FILTER_WORD_EX;


typedef struct _tagDEL_FILTER_WORD_EX{
	int nClientID;
	int nAdminID;
	char szHardID[MAX_HARDID_LEN_EX];			 
    int nWordID;
   	int nReserved[4];
}DEL_FILTER_WORD_EX, *LPDEL_FILTER_WORD_EX;

typedef struct _tagAsk_NewTableChair{
	int nUserID;								// 用户ID
	int nRoomID;								// 房间ID
	int nTableNO;								// 桌号
	int nChairNO;								// 位置
	int nReserved[4];
}ASK_NEWTABLECHAIR, *LPASK_NEWTABLECHAIR;

typedef struct _tagTAKE_SAFE_DEPOSIT{
	int nUserID;								// 用户ID
	int nGameID;								// 游戏ID
	int nRoomID;
	int nTableNO;
	int nChairNO;	
	int nDeposit;								// 银子
	int nKeyResult;								// 计算结果 = func(保护密码, 随机数)
	int nPlayingGameID;                         
	int nGameVID;
	int nTransferTotal;//划入这个游戏的总和(扣除划出的)，注意可能为负值，
	int nTransferLimit;//划入这个游戏，限定数量(两,>0)。
	T_DWORD dwIPAddr;                             //IP地址
	char szHardID[MAX_HARDID_LEN_EX];			// 硬件标识符（网卡序列号）	
	int nGameDeposit;
	int nReserved[7];
}TAKE_SAFE_DEPOSIT, *LPTAKE_SAFE_DEPOSIT;

typedef TAKE_SAFE_DEPOSIT    SAVE_SAFE_DEPOSIT;
typedef LPTAKE_SAFE_DEPOSIT  LPSAVE_SAFE_DEPOSIT;

typedef struct _tagLOOK_SAFE_DEPOSIT{
	int nUserID;								// 用户ID
	int nGameID;
	T_DWORD dwIPAddr;                             //IP地址
	char szHardID[MAX_HARDID_LEN_EX];			// 硬件标识符（网卡序列号）	
	int nReserved[8];
}LOOK_SAFE_DEPOSIT, *LPLOOK_SAFE_DEPOSIT;

typedef struct _tagSAFE_DEPOSIT_EX{
	int  nUserID;
	int  nDeposit;								// 银子
	long bHaveSecurePwd;                        //是否有保护密码
	int  nRemindDeposit;                        //超过这个银子，就提醒要设置保护密码
	int  nReserved[4];
}SAFE_DEPOSIT_EX, *LPSAFE_DEPOSIT_EX;

typedef struct _tagTAKE_SAFE_DEPOSIT_OK{
	int nUserID;								// 用户ID
	int nGameID;								// 游戏ID
	int nRoomID;
	int nTableNO;
	int nChairNO;
	int nDeposit;								// 银子
	int nGameDeposit;                           //游戏银子(最终)
	int nReserved[7];
}TAKE_SAFE_DEPOSIT_OK, *LPTAKE_SAFE_DEPOSIT_OK;

typedef TAKE_SAFE_DEPOSIT_OK    SAVE_SAFE_DEPOSIT_OK;
typedef LPTAKE_SAFE_DEPOSIT_OK  LPSAVE_SAFE_DEPOSIT_OK;

typedef LOOK_SAFE_DEPOSIT    TAKE_SAFE_RNDKEY;
typedef LPLOOK_SAFE_DEPOSIT  LPTAKE_SAFE_RNDKEY;

typedef struct _tagSAFE_RNDKEY{
	int nUserID;
	int nRndKey;
	int nReserved[4];
}SAFE_RNDKEY, *LPSAFE_RNDKEY;
typedef TAKE_SAFE_DEPOSIT    TAKE_BACKDEPOSIT_INGAME;
typedef LPTAKE_SAFE_DEPOSIT  LPTAKE_BACKDEPOSIT_INGAME;

typedef TAKE_SAFE_DEPOSIT    SAVE_BACKDEPOSIT_INGAME;
typedef LPTAKE_SAFE_DEPOSIT  LPSAVE_BACKDEPOSIT_INGAME;

typedef LOOK_SAFE_DEPOSIT    LOOK_BACKDEPOSIT_INGAME;
typedef LPLOOK_SAFE_DEPOSIT  LPLOOK_BACKDEPOSIT_INGAME;

typedef TAKE_SAFE_DEPOSIT_OK    TAKE_BACKDEPOSIT_INGAME_OK;
typedef LPTAKE_SAFE_DEPOSIT_OK  LPTAKE_BACKDEPOSIT_INGAME_OK;

typedef TAKE_SAFE_DEPOSIT_OK    SAVE_BACKDEPOSIT_INGAME_OK;
typedef LPTAKE_SAFE_DEPOSIT_OK  LPSAVE_BACKDEPOSIT_INGAME_OK;

typedef SAFE_DEPOSIT_EX    BACK_DEPOSIT_EX;
typedef LPSAFE_DEPOSIT_EX  LPBACK_DEPOSIT_EX;

/************************************************************************/
/* AsitSvr / ChunkSvr Example                                            
/************************************************************************/
typedef struct _tagGetPropInfo{ //获取道具信息
	int   nUserID;
	int   nGameID; 
	char  szHardID[MAX_HARDID_LEN_EX];
	T_DWORD dwIP;
	int   nReserved[7];
}GET_PROP_INFO, *LPGET_PROP_INFO;

typedef struct _tagUseLuckCard{ //使用一张手气卡
	int   nUserID;
	int   nGameID;					
	char  szHardID[MAX_HARDID_LEN_EX];
	T_DWORD dwIP;
	int   nReserved[7];
}USE_LUCKCARD, *LPUSE_LUCKCARD;

#endif