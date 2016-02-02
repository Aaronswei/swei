#include <iostream>
#include <string>

#include "XCrawler.h"
#include "WebServer.h"
#include "config.h"
#include "url.h"

void PxCrawler()
{
	try{
		WebServer server;
		server.run();
	
		XCrawler crawler;
		crawler.start(); 
	}
	catch(const std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
}
