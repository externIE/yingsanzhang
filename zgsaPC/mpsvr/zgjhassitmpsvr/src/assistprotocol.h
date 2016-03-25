#ifndef __HALL_PROTOCOL_H__
#define __HALL_PROTOCOL_H__
#include "MPProtocol.h"

#define		GR_SHI_JU_CHOU				(GAME_REQ_BASE_EX+25100)		// 玩家十局抽
#define		GR_MY_WIN_ORDER				(GAME_REQ_BASE_EX+25200)		// 玩家自己赢银排序
#define		GR_WIN_ORDER				(GAME_REQ_BASE_EX+25300)		// 玩家赢银排序
#define		GR_GET_PROPINFO				(GAME_REQ_INDIVIDUAL + 1002)  // 获取道具信息
#define		GR_USE_LUCKCARD				(GAME_REQ_INDIVIDUAL + 1003)  // 使用一次手气卡

class asitProtocol:public mp::MPProtocol
{
public:
	virtual int c2sTransformType(int msg_type);
	virtual int s2cTransformType(int msg_type);
	virtual int c2sMsgTransform(int msg_type, unsigned long clientip, const char* in_msg, unsigned int in_length, char*& out_msg, unsigned int &size);
	virtual int s2cMsgTransform(int msg_type, const char* in_msg, unsigned int in_length, char*& out_msg, unsigned int &size);
	virtual int releaseOutMessage(int msg_type, char * out_msg) ;
	virtual bool v1S2cTransformEncrypt(int ){return false;};
private:
	int c2sDefault(unsigned long clientip, const char* in_msg, unsigned int in_length, char*& out_msg, unsigned int &out_len);
	int c2sGetProp(unsigned long clientip, const char* in_msg, unsigned int in_length, char*& out_msg, unsigned int &out_len);
	int c2sUseLuckCard(unsigned long clientip, const char* in_msg, unsigned int in_length, char*& out_msg, unsigned int &out_len);

	int s2cDefault(const char* in_msg, unsigned int in_length, char*& out_msg, unsigned int &out_len);
};

#endif