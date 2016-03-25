#include "ZgJhprotocol.h"
#include "uwllog.h"
#include <algorithm>
#include "uwltypes.h"
#include "xygext2.h"
#include "xygrdefine.h"
/************************************************************************/
/* >0 the size of struct
   =0 no content, only msg_type
   =-1 not send to mobile client
   =-2 the same as the in buffer
*/
/************************************************************************/

int ZgJhProtocol::c2sTransformType(int msg_type)
{
	switch (msg_type)
	{

	default:
		return mp::TRANSFORMER_CONVERT;
	}
}
int ZgJhProtocol::s2cTransformType(int msg_type)
{
	switch (msg_type)
	{
	case GR_ASK_EXIT:
	case GR_ALLOW_EXIT:
//	case GR_CHAT_TO_TABLE:
	case GR_CALL_PLAYER:
	case GR_LOOKON_ENTER:
	case GR_LOOKON_ABORT:
//	case GR_CHAT_FROM_TALBE:
		return mp::TRANSFORMER_REJECT;




	default:
		return mp::TRANSFORMER_CONVERT;
	}
}

/*
int SzgqProtocol::transformLogonUser(const char* in_msg, unsigned int in_length, char* out_msg)
{
	LOGON_USER user;
	memset(&user, 0x00, sizeof(user));
	hall::logon_user msg;
	msg.ParseFromArray(in_msg, in_length);
	user.nBlockSvrID  = msg.blocksvrid();
	user.nUserID = msg.userid();
	user.nHallSvrID = msg.hallsvrid();
	user.nAgentGroupID = msg.agentgroupid();
	user.nGroupType = msg.grouptype();
	user.dwIPAddr = msg.ipaddr();
	user.dwSoapFlags = msg.soapflags();
	user.dwLogonFlag = msg.logonflag();	 
	user.lTokenID = msg.tokenid();
	user.nResponse = msg.response();
	memcpy(user.szUsername, msg.username().c_str(), std::min(MAX_USERNAME_LEN, (int)msg.username().length())); 
	memcpy(user.szPassword, msg.password().c_str(), std::min(MAX_PASSWORD_LEN, (int)msg.password().length()));	 
	memcpy(user.szHardID, msg.hardid().c_str(), std::min(MAX_HARDID_LEN, (int)msg.hardid().length()));	 
	memcpy(user.szVolumeID, msg.volumeid().c_str(), std::min(MAX_HARDID_LEN, (int)msg.volumeid().length()));	 
	memcpy(user.szMachineID, msg.machineid().c_str(), std::min(MAX_HARDID_LEN, (int)msg.machineid().length()));  
	memcpy(user.szHashPwd, msg.hashpwd().c_str(), std::min(DEF_HASHPWD_LEN+2, (int)msg.hashpwd().length()));	 
	memcpy(user.szRndKey, msg.rndkey().c_str(), std::min(MAX_RNDKEY_LEN_EX, (int)msg.rndkey().length()));

	user.dwSysVer = msg.sysver();     
	user.nLogonSvrID = msg.logonsvrid(); 
	user.nHallBuildNO = msg.hallbuildno();
	user.nHallNetDelay = msg.hallnetdelay(); 
	user.nHallRunCount = msg.hallruncount();

	memcpy(out_msg, &user,sizeof(user));
	memcpy(out_msg, in_msg, in_length);
	return 0;
}
*/

int ZgJhProtocol::c2sDefault(unsigned long clientip, const char* in_msg, unsigned int in_length, char*& out_msg, unsigned int &out_len)
{
	out_msg = new char[in_length];
	out_len = in_length;
	memcpy(out_msg, in_msg, in_length);
	return 0;
}

int ZgJhProtocol::c2sMsgTransform(int msg_type, 
									 unsigned long ip,
										   const char* in_msg, 
										   unsigned int in_length,
										   char*& out_msg,
										   unsigned int & out_len)
{
	UWL_INFO("c2sMsgTransform:%d",msg_type);
	switch (msg_type)
	{
	default:
		{
			c2sDefault(ip, in_msg, in_length, out_msg, out_len);
		}
		break;
	}
	
	return 0;
}

int ZgJhProtocol::s2cDefault(const char* in_msg, unsigned int in_length, char*& out_msg, unsigned int &out_len)
{
	out_msg = new char[in_length];
	out_len = in_length;
	memcpy(out_msg, in_msg, in_length);
	return 0;
}
int ZgJhProtocol::s2cMsgTransform(int msg_type,
										   const char* in_msg, 
										   unsigned int in_length,
										   char*& out_msg,
										   unsigned int & out_len)
{
	/*switch (msg_type)
	{
		case GR_LOGON_USER:
			transformLogonUser(in_msg, in_length, out_msg);
			break;
		case GR_LOGON_SUCCEEDED:
			transformLogonUser(in_msg, in_length, out_msg);
			break;
	default:
		break;
	}*/
	UWL_INFO("s2cMsgTransform:%d",msg_type);
	s2cDefault(in_msg, in_length, out_msg, out_len);
	return 0;
}

int ZgJhProtocol::releaseOutMessage(int msg_type, char * out_msg)
{
	if (out_msg)
	{
		delete[] out_msg;
	}
	
	return 0;
}