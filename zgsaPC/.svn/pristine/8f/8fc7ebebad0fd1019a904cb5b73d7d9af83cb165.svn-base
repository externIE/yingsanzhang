#pragma once

/************************************************************************/
/* commom req and struct, used by assitsvr/gamesvr/game                  
/************************************************************************/
// 1 ~ 1000
#define  GR_ASVR_VERIFY_USER	(GAME_REQ_INDIVIDUAL + 1)	  // 初始化用户token

// 1000 ~
#define  GR_GET_PROPINFO		(GAME_REQ_INDIVIDUAL + 1002)  // 获取道具信息
#define  GR_USE_LUCKCARD		(GAME_REQ_INDIVIDUAL + 1003)  // 使用一次手气卡


/************************************************************************/
/* chunksvr response                                                   
/************************************************************************/
#define	UR_LUCKCARD_ZERO		(UR_OPERATE_FAILED + 4)


typedef struct _tagSERVERPULSE_INFO{
   	int   nCurrentDate;    // 20150303
	int   nLatestTime;     // 2038秒
	int   nReconnectCount; // 每日统计，次日清0
}SERVERPULSE_INFO, *LPSERVERPULSE_INFO;

typedef struct _tagTableNotify{
	int   nUserID;
	int   nRoomID;
	int   nTableNo;
	int   nChairNO;
	int   nDeposit;
	int   nScore;
	TCHAR szHardID[MAX_HARDID_LEN];
	TCHAR szMsg[MAX_PATH];
	int   nReserved[8];
}TABLENOTIFY, *LPTABLENOTIFY;

typedef struct _tagGetPropInfo{ // 获取道具信息
	int   nUserID;
	int   nGameID; 
	TCHAR szHardID[MAX_HARDID_LEN];
	DWORD dwIP;					// 需游戏代理服务 赋值
	int   nReserved[7];
}GET_PROP_INFO, *LPGET_PROP_INFO;

typedef struct _tagGetPropInfoOK{ 
	int nUserID;
	int nLuckyCard;
	int nScoreMult;
	int nScoreMultStartDate;
	int nScoreMultEndDate;
	int nScoreSafe;
	int nScoreSafeStartDate;
	int nScoreSafeEndDate;
	int	nUpdateDate;
	int nReserved[8];
}GET_PROPINFO_OK, *LPGET_PROPINFO_OK;

typedef struct _tagUseLuckCard{ // 使用一张手气卡
	int   nUserID;
	int   nGameID;					
	TCHAR szHardID[MAX_HARDID_LEN];
	DWORD dwIP;					// 需游戏代理服务 赋值
	int   nReserved[7];
}USE_LUCKCARD, *LPUSE_LUCKCARD;

typedef struct _tagUseLuckCardOK{ 
	int   nUserID;
	int   nUpdateTime;					//最后更新时间
	int   nLuckyCardLeft;				//卡剩余张数
	int   nReserved[8];
}USE_LUCKCARD_OK, *LPUSE_LUCKCARD_OK;


// AssitSvr End
//////////////////////////////////////////////////////////////////////////

