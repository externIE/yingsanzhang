#include <iostream>
#include <string>

#include "ZgJhprotocol.h"
#include "uwllog.h"
#include "mpserverfactory.h"
#ifdef DEBUG
//#include "vld.h"
#endif
int main(int argc, char* argv[])
{
	ZgJhProtocol *transformer = NULL;
	try
	{
#ifdef DEBUG
		UwlBeginLog("ZgJhmpsvr", UWL_LOG_LEVEL_TRACE);
#else
		UwlBeginLog("ZgJhmpsvr", UWL_LOG_LEVEL_ERROR);
#endif
		transformer = new ZgJhProtocol;
#ifdef MP_WIN_SERVICE
		mp::MPServerFactory::startService(argc, argv,"ZgJhmpsvr", "同城游赢三张移动代理服务器", "ZgSampsvr.ini", transformer);
		return 0;
#endif

		UWL_INFO("Starting server....");
		mp::MPServerFactory::startServer("ZgJhmpsvr.ini", transformer);
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