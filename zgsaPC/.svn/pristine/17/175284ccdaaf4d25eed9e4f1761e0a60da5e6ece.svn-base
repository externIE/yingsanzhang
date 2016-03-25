#pragma once

#define		PRODUCT_LICENSE	 	 _T("")
#define		PRODUCT_NAME		 _T("ZgSaSvr")
#define		PRODUCT_VERSION		 _T("1.00")
#define     STR_SERVICE_NAME     _T("ZgSaSvr")
#define     STR_DISPLAY_NAME     _T("同城游新赢三张服务")
#define     STR_DISPLAY_NAME_ENU _T("TCY ZgSaSvr Service") 

#define		GAME_ID					311 //需要修改
#define		GAME_PORT				23311 //需要修改

#define		TOTAL_CHAIRS			6//多少玩家
#define		GAME_FLAGS			    GF_LEVERAGE_ALLOWED//GAME_FLAGS_USE_JOKER 

#define     TOTAL_CARDS             52
#define     CHAIR_CARDS             3

#define		MAX_SCORE_CARD		    24   

#define     TS_WAITING_OPERATION    0x00000002 
#define     TS_WAITING_NEW_ROUND    0x00000004

#define		LAYOUT_MOD		        13
#define		BET_WAIT 			    20 

#define     MAX_BET_PER_BOUT        99999999	//一局最多使用99999999个筹码(一个玩家)
#define		MAX_BET_ALL_COUNT		500	//最多出现99999999个筹码(所有玩家)

#define		BET_ONE					1
#define		BET_TWO					2
#define		BET_FOUR				4
#define		BET_EIGHT				8

#define HANDCARD_TYPE_BAOZI         7		          //豹子
#define HANDCARD_TYPE_SHUNJIN       6		          //顺金
#define HANDCARD_TYPE_JINHUA        5                 //金花
#define HANDCARD_TYPE_SHUNZI        4                 //顺子
#define HANDCARD_TYPE_DUIZI         3                 //对子
#define HANDCARD_TYPE_DANZHANG      2                 //单张
#define HANDCARD_TYPE_235           1                 //235,最小牌，但在有豹子的时候可大过豹子

#define     PUBCARD_COUNT           5
#define     POOL_COUNT              TOTAL_CHAIRS
#define     GAME_OPE_BET            0x00000001	//下注
#define     GAME_OPE_RISE           0x00000002	//加注
#define     GAME_OPE_ALLIN          0x00000004	//全下
#define     GAME_OPE_FOLLOW         0x00000008  //跟
#define     GAME_OPE_GIVE_UP        0x00000010  //弃牌
#define     GAME_OPE_LOOK_CARD      0x00000020	//看牌  
#define     GAME_OPE_COMPARE        0x00000040	//比牌  
#define     GAME_OPE_BET_TWO        0x00000080	//2倍
#define     GAME_OPE_BET_FOUR       0x00000100	//4倍
#define     GAME_OPE_BET_EIGHT		0x00000200	//8倍
#define		GAME_OPE_AUTO_START		0x00000400	//自动开始
#define		GAME_OPE_AUTO_FOLLOW	0x00000800	//自动跟注
#define		GAME_OPE_BET_FULL		0x00001000	//押满

#define		DEFAULT_NEED_MAKE_DEAL_CARD_VALUE	8	//需要做牌的默认牌值
#define		INVALID_CHAIR_NO_EX		15			//因为RoomTableChair无法使用-1作为无效chairno
#define		DEFAULT_BASE_DEPOSIT_COEFFICIENT	100	//默认底注系数
#define		DEFAULT_BASE_DEPOSIT_MULTIPLE		1		//默认底注倍数
#define		DEFAULT_MAX_BASE_DEPOSIT			100000000	//默认底注倍数
#define		DEFAULT_MIN_CHANGE_BASE_DEPOSIT		1		//默认最小可变基础银

#undef MAX
#define MAX(a,b)	((a) > (b) ? (a) : (b))
#undef MIN
#define MIN(a,b)	((a) < (b) ? (a) : (b))

enum     PLAYER_STATUS
{ 
		PLAYER_STATUS_WAIT_START, 
    	PLAYER_STATUS_NULL,
	 	PLAYER_STATUS_WAIT_BET,
		PLAYER_STATUS_BET_SMALL,
		PLAYER_STATUS_BET_BIG,
		PLAYER_STATUS_BET,//下注 
		PLAYER_STATUS_RISE,
		PLAYER_STATUS_ALLIN,
		PLAYER_STATUS_BET_FULL,
		PLAYER_STATUS_FOLLOW,
		PLAYER_STATUS_GIVE_UP,
		PLAYER_STATUS_LOOK_CARD,
		PLAYER_STATUS_COMPARE,
		PLAYER_STATUS_READY,
		PLAYER_STATUS_COUNT	
}; 
enum    CARD_TYPE{
     	CARD_TYPE_SINGLE,				//单张
		CARD_TYPE_DUIZI,				//对子
		CARD_TYPE_DOUBLE_DUIZI,			//两对
		CARD_TYPE_THREE,				//三张
		CARD_TYPE_SHUN,					//顺子
		CARD_TYPE_HUA,					//同花
		CARD_TYPE_HULU,					//葫芦
		CARD_TYPE_BOMB,					//炸弹
		CARD_TYPE_HUA_SHUN,				//同花顺  
	 	CARD_TYPE_HUA_SHUN_MAX,			//皇家同花顺  
		CARD_TYPE_MAX
};
