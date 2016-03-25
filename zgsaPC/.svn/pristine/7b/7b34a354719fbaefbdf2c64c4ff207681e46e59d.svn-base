#ifndef _XYGRDEFINE_H_
#define _XYGRDEFINE_H_

#include "xygext2.h"

// request (from game clients)
#define		GR_GET_VERSION			(GAME_REQ_BASE_EX + 1001)			// 获取版本信息
#define		GR_CHECK_NETWORK		(GAME_REQ_BASE_EX + 1002)			// 检查网络状况
#define		GR_USER_ACTION			(GAME_REQ_BASE_EX + 1010)			// 玩家发送请求
#define     GR_GAME_PULSE          	(GAME_REQ_BASE_EX + 1020)           // 玩家客户端脉搏信号


// request (from game clients)
#define		GR_LEAVE_GAME			(GAME_REQ_BASE_EX + 1120)		// 用户退出游戏
#define		GR_LEAVE_MATCH			(GAME_REQ_BASE_EX + 1122)		// 用户退出比赛
#define		GR_LEAVE_GAME_EX		(GAME_REQ_BASE_EX + 1125)		// Solo模式用户退出游戏
#define		GR_QUERY_BREAK			(GAME_REQ_BASE_EX + 1130)		// 查询断线扣分
#define		GR_GET_TABLE_INFO		(GAME_REQ_BASE_EX + 1140)		// 获取桌面信息
#define		GR_START_GAME			(GAME_REQ_BASE_EX + 1160)		// 玩家开始游戏
#define     GR_AUTO_START_GAME      (GAME_REQ_BASE_EX + 1161)       // 自动开始
#define     GR_CLIENT_CRASH         (GAME_REQ_BASE_EX + 1162)       // 客户端崩溃       
//#define     GR_AUTO_HELP_PLAYER     (GAME_REQ_BASE_EX + 1162)     // 服务器自动帮用户出牌
#define		GR_GIVE_UP_GAME			(GAME_REQ_BASE_EX + 1180)		// 玩家认输不玩
#define		GR_CALL_PLAYER			(GAME_REQ_BASE_EX + 1200)		// 呼叫游戏玩家
#define		GR_CHAT_TO_TABLE		(GAME_REQ_BASE_EX + 1210)		// 在桌面上聊天
#define     GR_CHAT_TO_TABLE_EX		(GAME_REQ_BASE_EX + 1211)		// 在桌面上聊天,服务器端发给所有人，有关键字过滤

#define     GR_ASK_EXIT				(GAME_REQ_BASE_EX + 1212)		// 玩家请求强退
#define     GR_ALLOW_EXIT			(GAME_REQ_BASE_EX + 1213)		// 玩家同意协商强退
#define		GR_ASK_STANDOFF			(GAME_REQ_BASE_EX + 1215)		// 玩家请求和局
#define		GR_AGREE_STANDOFF		(GAME_REQ_BASE_EX + 1218)		// 玩家同意和局



#define     GR_REPORT_ENTER_MATCH    (GAME_REQ_BASE_EX+1220)     	// 用户进入比赛
#define     GR_REPORT_MATCHPLAYER_OK (GAME_REQ_BASE_EX+1221)        // 用户准备好进行下一场比赛
#define     GR_REPORT_GETUSERDETAIL  (GAME_REQ_BASE_EX+1230)        // 请求查看其它用户的数据
#define     GR_REPORT_SIGNUP         (GAME_REQ_BASE_EX+1231)        // 请求查看其它用户的数据
#define     GR_REPORT_GIVEUP         (GAME_REQ_BASE_EX+1232)        // 请求查看其它用户的数据
  

// response (to game clients)
#define		GR_TABLE_INFO						(GAME_REQ_BASE_EX + 10005)		// 返回桌面信息
#define		GR_INVALID_IDENTITY					(GAME_REQ_BASE_EX + 10008)		// 验证身份失败
#define		GR_GAME_NOT_READY					(GAME_REQ_BASE_EX + 10010)		// 游戏没有开始
#define		GR_HAVE_NO_CHAIR					(GAME_REQ_BASE_EX + 10020)		// 没有可用座位
#define		GR_BREAK_CUT_YES					(GAME_REQ_BASE_EX + 10030)		// 逃跑扣银成立
#define		GR_BREAK_CUT_NOT					(GAME_REQ_BASE_EX + 10040)		// 逃跑不会扣银
#define		GR_WAIT_FEW_SECONDS					(GAME_REQ_BASE_EX + 10050)		// 需要等待几秒
#define		GR_HARDID_MISMATCH					(GAME_REQ_BASE_EX + 10060)		// 硬件验证失败
#define		GR_ROOM_TOKENID_MISMATCH			(GAME_REQ_BASE_EX + 10070)		// 令牌验证失败
#define		GR_ROOMTABLECHAIR_MISMATCH			(GAME_REQ_BASE_EX + 10078)		// 位置验证失败
#define     GR_LEAVEGAME_TOOFAST				(GAME_REQ_BASE_EX + 10085)		// 太快退出游戏，不能退出
#define     GR_LEAVEGAME_PLAYING				(GAME_REQ_BASE_EX + 10086)		// 游戏正在进行中，不能退出


#define     GR_RESPONE_ENTER_GAME_OK			 (GAME_REQ_BASE_EX+10200)		//用户进入游戏成功，不在游戏中,带ENTER_GAME_INFO结构
#define     GR_RESPONE_ENTER_GAME_DXXW			 (GAME_REQ_BASE_EX+10210)		//用户进入游戏成功，断线续完，带GAME_TABLE_INFO结构


#define     GR_RESPONE_ENTER_MATCH_OK			 (GAME_REQ_BASE_EX+10500)		 // 用户进入比赛成功,比赛未开始
#define     GR_RESPONE_ENTER_MATCH_DXXW			 (GAME_REQ_BASE_EX+10505)		 // 用户进入比赛成功,断线续完
#define     GR_RESPONE_ENTER_MATCH_LOOKON   	 (GAME_REQ_BASE_EX+10510)		 // 用户进入比赛成功，旁观
#define     GR_RESPONE_ENTER_MATCH_INQUEUE  	 (GAME_REQ_BASE_EX+10520)		 // 用户进入比赛成功，等待分桌中
#define     GR_RESPONE_ENTER_MATCH_NOSIGNUP 	 (GAME_REQ_BASE_EX+10521)        // 用户未报名
#define     GR_RESPONE_ENTER_MATCH_ERROR    	 (GAME_REQ_BASE_EX+10530)	     // 用户进入比赛失败
#define     GR_RESPONE_ENTER_MATCH_WASHOUT  	 (GAME_REQ_BASE_EX+10531)		 // 用户已经被淘汰
#define     GR_RESPONE_ENTER_MATCH_WAIT     	 (GAME_REQ_BASE_EX+10532)        // 用户等待分桌
#define     GR_RESPONE_ENTER_MATCH_STARTED  	 (GAME_REQ_BASE_EX+10533)        // 比赛已经开始，登入失败
#define     GR_RESPONE_ENTER_MATCH_FINISHED      (GAME_REQ_BASE_EX+10534)        // 比赛已经结束
#define     GR_RESPONE_ENTER_MATCH_MISMATCH   	 (GAME_REQ_BASE_EX+10535)        // 身份验证失败
#define     GR_RESPONE_ENTER_MATCH_SYSTEMCANCEL  (GAME_REQ_BASE_EX+10536)        // 身份验证失败
#define     GR_RESPONE_ENTER_MATCH_SELFCANCEL    (GAME_REQ_BASE_EX+10537)        // 玩家已退赛
#define     GR_RESPONE_ENTER_MATCH_SIGNUPUNCHECK (GAME_REQ_BASE_EX+10538)        // 玩家报名未审核 
#define     GR_RESPONE_ENTER_MATCH_REJECTED      (GAME_REQ_BASE_EX+10539)        // 系统拒绝用户参赛
#define     GR_RESPONE_ENTER_MATCH_LATE			 (GAME_REQ_BASE_EX+10540)        // 报名用户迟到

// nofication (to game clients)
#define		GR_GAME_ABORT						 (GAME_REQ_BASE_EX + 11000)		// 玩家强行退出
#define		GR_PLAYER_ABORT					     (GAME_REQ_BASE_EX + 11010)		// 用户退出Solo模式
#define		GR_PLAYER_ENTER						 (GAME_REQ_BASE_EX + 11011)		// 用户进来Solo模式
#define		GR_LOOKON_ABORT						 (GAME_REQ_BASE_EX + 11020)		// 旁观用户退出
#define		GR_LOOKON_ENTER						 (GAME_REQ_BASE_EX + 11025)		// 旁观用户进来
#define     GR_START_SOLOTABLE					 (GAME_REQ_BASE_EX + 11028)		// 组建桌子Solo模式
#define		GR_ALL_STANDBY						 (GAME_REQ_BASE_EX + 11030)		// 通知玩家到位
#define		GR_GAME_START					     (GAME_REQ_BASE_EX + 11040)		// 通知游戏开始
#define		GR_GAME_WIN							 (GAME_REQ_BASE_EX + 11080)		// 玩家出牌获胜
#define		GR_GAINS_BONUS						 (GAME_REQ_BASE_EX + 11090)		// 得分奖励通知
#define		GR_DEPOSIT_NOT_ENOUGH			     (GAME_REQ_BASE_EX + 11110)		// 银子数量不够
#define		GR_PLAYER_NOT_SEATED				 (GAME_REQ_BASE_EX + 11120)		// 玩家已经离开
#define		GR_SCORE_NOT_ENOUGH		             (GAME_REQ_BASE_EX + 11130)		// 积分数量不够
#define		GR_SCORE_TOO_HIGH		             (GAME_REQ_BASE_EX + 11132)		// 积分数量超出
#define		GR_USER_BOUT_TOO_HIGH				 (GAME_REQ_BASE_EX + 11134)		// 玩家局数超出
#define		GR_TABLE_BOUT_TOO_HIGH				 (GAME_REQ_BASE_EX + 11136)		// 一桌局数超出
#define     GR_OUT_OF_OPENTIME					 (GAME_REQ_BASE_EX + 11137)     // 游戏开放时间已经结束
#define		GR_GAME_UNABLE_TO_CONTINUE			 (GAME_REQ_BASE_EX + 11138)		// 游戏结束，无法继续
#define     GR_CHAT_FROM_TALBE    				 (GAME_REQ_BASE_EX + 11200)	    // 通知用户消息
#define		GR_WAIT_NEWTABLE					 (GAME_REQ_BASE_EX + 11300)		// 随机房间等待重新分桌
#define     GR_MATCH_WWW						 (GAME_REQ_BASE_EX + 11400)		// 比赛排行榜地址,显示在广告区
#define     GR_MATCH_UNABLE_TO_CONTINUE			 (GAME_REQ_BASE_EX + 11410)		// 比赛结束，无法继续
// 


#define     GR_TOCLIENT_ARRAGE_TABLE			 (GAME_REQ_BASE_EX+11150)      // 分桌信息
#define     GR_TOCLIENT_TABLEINFO				 (GAME_REQ_BASE_EX+11160)      // 游戏信息
#define     GR_TOCLIENT_FRESHRANK      		     (GAME_REQ_BASE_EX+11170)      // 刷新积分信息
#define     GR_TOCLIENT_FINISH_MATCH			 (GAME_REQ_BASE_EX+11190)      // 用户比赛结束
#define     GR_TOCLIENT_MATCH_GAIN	   			 (GAME_REQ_BASE_EX+11191)      // 比赛结果
#define     GR_TOCLIENT_MATCH_START    			 (GAME_REQ_BASE_EX+11199)      // 比赛开始
#define     GR_TOCLIENT_MATCH_CANCELED 			 (GAME_REQ_BASE_EX+11201)      // 比赛取消
#define     GR_TOCLIENT_MATCH_SIGNUPED 			 (GAME_REQ_BASE_EX+11202)      // 比赛报名已满
#define     GR_TOCLIENT_MATCH_SIGNUPOK 			 (GAME_REQ_BASE_EX+11203)      // 用户报名成功
#define     GR_TOCLIENT_MATCH_GIVEUPOK 			 (GAME_REQ_BASE_EX+11204)      // 用户退赛成功
#define     GR_TOCLIENT_MATCH_SIGNUPFAIL 		 (GAME_REQ_BASE_EX+11205)      // 用户报名失败
#define     GR_TOCLIENT_MATCH_GIVEUPFAIL 		 (GAME_REQ_BASE_EX+11206)      // 用户退赛失败
#define     GR_TOCLIENT_WAIT_FINAL				 (GAME_REQ_BASE_EX+11210)      // 等待其他桌游戏结束，产生决赛名单
#define     GR_TOCLIENT_USER_STATUS			     (GAME_REQ_BASE_EX+11220)      // 通知用户状态
#define     GR_TOCLIENT_OVERTIME                 (GAME_REQ_BASE_EX+11230)      // 游戏超时
#define     GR_TOCLIENT_OFFLINE                  (GAME_REQ_BASE_EX+11240)      // 用户掉线
#define     GR_TOCLIENT_USERDETAIL               (GAME_REQ_BASE_EX+11241)      // 用户详细信息
#define     GR_TOCLIENT_BC_SIGNCOUNT             (GAME_REQ_BASE_EX+11242)      // 广播用户报名人数
#define     GR_TOCLIENT_KICKOFF                  (GAME_REQ_BASE_EX+11243)      // 踢出玩家
#define		GR_USER_DEPOSIT_EVENT				 (GAME_REQ_BASE_EX + 11600)		// 玩家存取银子变化
#define		GR_USER_POSITION					 (GAME_REQ_BASE_EX + 11700)		// 玩家换到新位置
//////////////////////////////////////////////////////////////////////////
//Add on 20130115
//Look card request
#define		GR_ASK_CARD_VIEW					 (GAME_REQ_BASE_EX + 29550)		// 请求旁观看牌
#define		GR_ALLOW_CARD_VIEW					 (GAME_REQ_BASE_EX + 29560)		// 允许旁观看牌
#define		GR_CARD_VIEW_ALLOWED				 (GAME_REQ_BASE_EX + 29570)		// 允许旁观看牌
#define		GR_CARDS_INFO						 (GAME_REQ_BASE_EX + 29580)		// 旁观看牌内容

#define		GR_SAFEBOX_GAME_READY				 (GAME_REQ_BASE_EX + 29600)	
#define		GR_SAFEBOX_GAME_PLAYING				 (GAME_REQ_BASE_EX + 29601)
#define		GR_SAFEBOX_DEPOSIT_DIFFER			 (GAME_REQ_BASE_EX + 29602)
#define		GR_SAFEBOX_DEPOSIT_MIN				 (GAME_REQ_BASE_EX + 29603)	
#define		GR_SAFEBOX_DEPOSIT_MAX				 (GAME_REQ_BASE_EX + 29604)	

#define		FLAG_CHAT_RESERVED   	           0x00000001  //保留
#define		FLAG_CHAT_SYSNOTIFY				   0x00000002  //系统通知
#define		FLAG_CHAT_ADMINMSG				   0x00000004  //网管消息或者发给网管的消息
#define		FLAG_CHAT_PLAYERMSG				   0x00000008  //玩家消息

#define  RANKMATCH_STATUS_UNSTART      1
#define  RANKMATCH_STATUS_STARTED      2
#define  RANKMATCH_STATUS_FINISHED     3

#endif