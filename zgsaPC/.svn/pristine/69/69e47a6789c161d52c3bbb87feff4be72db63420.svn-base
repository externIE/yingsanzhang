#ifndef __HALL_PROTOCOL_H__
#define __HALL_PROTOCOL_H__
#include "MPProtocol.h"

class ZgJhProtocol:public mp::MPProtocol
{
public:
	virtual int c2sTransformType(int msg_type);
	virtual int s2cTransformType(int msg_type);
	virtual int c2sMsgTransform(int msg_type, unsigned long clientip, const char* in_msg, unsigned int in_length, char*& out_msg, unsigned int &size);
	virtual int s2cMsgTransform(int msg_type, const char* in_msg, unsigned int in_length, char*& out_msg, unsigned int &size);
	virtual int releaseOutMessage(int msg_type, char * out_msg) ;

	virtual bool v1S2cTransformEncrypt(int msg_type){return true;}
private:
	int c2sDefault(unsigned long clientip, const char* in_msg, unsigned int in_length, char*& out_msg, unsigned int &out_len);


	int s2cDefault(const char* in_msg, unsigned int in_length, char*& out_msg, unsigned int &out_len);
};

#endif