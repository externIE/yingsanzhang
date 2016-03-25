#include <iostream>
#include <string>

#include "assistprotocol.h"
#include "uwllog.h"
#include "mpserverfactory.h"
#ifdef DEBUG
//#include "vld.h"
#endif
int main(int argc, char* argv[])
{
	asitProtocol *transformer = NULL;
	try
	{
#ifdef DEBUG
		UwlBeginLog("zgjhassitmpsvr", UWL_LOG_LEVEL_TRACE);
#else
		UwlBeginLog("zgjhassitmpsvr", UWL_LOG_LEVEL_ERROR);
#endif
		transformer = new asitProtocol;
#ifdef MP_WIN_SERVICE
		mp::MPServerFactory::startService(argc, argv,"zgjhassitmpsvr", "同城游移动辅助代理服务器", "zgjhassitmpsvr.ini", transformer);
		return 0;
#endif

		UWL_INFO("Starting server....");
		mp::MPServerFactory::startServer("zgjhassitmpsvr.ini", transformer);
	}
	catch (std::exception& e)
	{
		std::cerr << "exception: " << e.what() << "\n";
	}
	if (transformer)
	{
		delete transformer;
	}
	UwlEndLog();
	return 0;
}