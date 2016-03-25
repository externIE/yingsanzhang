#pragma once

#define		PRODUCT_LICENSE	 		_T("license.dat")
#define		PRODUCT_NAME			_T("ZgjhAssitSvr")
#define		PRODUCT_VERSION			_T("1.00")
#define     STR_SERVICE_NAME		_T("ZgjhAssitSvr")
#define     STR_DISPLAY_NAME		_T("同城游赢三张辅助服务")
#define     STR_DISPLAY_NAME_ENU	_T("TCY Zgjh Assit Service") 

#define     ASSITSVR_CLSNAME_EX     _T("TCY_ASSITSVR_CLSNAME")
#define     ASSITSVR_WNDNAME_EX     _T("TCY_ASSITSVR_WNDNAME")

#define		GAME_ID				    221	//需要修改 

/************************************************************************/
/*                                                                       
/************************************************************************/
enum{
	LOGON_NOUSERINFO=-1,      //没有此用户
	LOGON_NOTOKEN=-2,	      //没有此token
	LOGON_USERID_MISMATCH=-3, //userid不匹配
	LOGON_HARDID_MISMATCH=-4, //hardid不匹配
};		