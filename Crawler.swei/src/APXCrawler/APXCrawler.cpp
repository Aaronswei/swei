#include <iostream>
#include <string>

#include "GeneralHelperFunctions.h"
#include "HttpConnection.h"
#include "Crawler.h"
#include "Menu.h"

void ApxCrawler()
{
	try
	{
		UserInterface::Menu menu;
	}
	catch(const std::exception &e)
	{
		std::cout << e.what();
	}
}
