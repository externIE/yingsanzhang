#pragma once

#define		INVALID_CHAIR_NO						15				//因为RoomTableChair无法使用-1作为无效chairno
#define		MAX_SEATED_TO_LOOKON_TIME				3600000			//一个小时，单位毫秒

//自定义Windows消息号(理论上应该从3000开始，不过为了统一与其它游戏的消息号)
#define     WM_GTR_CHANGE_SEATED_TO_LOOK_ON			(WM_USER+3002)	//接到用户从客户端主动变为旁观状态通知，去除上桌状态
#define     WM_GTR_CHANGE_LOOK_ON_TO_SEATED			(WM_USER+3003)	//接到用户从客户端主动变为上桌状态通知，去除旁观状态
#define     WM_GTR_CHANGE_SEATED_TO_LOOK_ON_RESULT	(WM_USER+3004)	//推送坐下变为旁观结果给GameSvr
#define     WM_GTR_CHANGE_LOOK_ON_TO_SEATED_RESULT	(WM_USER+3005)	//推送旁观变为坐下结果给GameSvr
