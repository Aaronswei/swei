#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <boost/regex.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <cstddef>
#include "ConsoleCrawlerViewer.h"
#include "Crawler.h"

namespace UserInterface
{
	#define MEASURE_OF_TIME		"milliseconds"

	#if PLATFORM == PLATFORM_WINDOWS

	#define CLEAR_SCREEN		"cls"
	
	#else

	#define CLEAR_SCREEN

	#endif

	#define AMOUNT_HANDLERS 	12

	class Menu
	{
		public:
			Menu();

		private:
		
			void printMenu();
			void interrogation();

			//handlers
			void quit();
			void start();
			void clearScreen();
			void setStartAddress();
			void setPauseBetweenReqs();
			void setUserAgent();
			void setMaxDepth();
			void switchInfiniteMode();
			void switchSearchInSE();
			void selectSearchEngine();
			void displayPhrasesForSearch();
			void addPhraseForSearch();

			//============================
			//

			void (Menu::*runHandler[AMOUNT_HANDLERS])()=
			{
				&Menu::quit, 		&Menu::start, 	&Menu::clearScreen,
				&Menu::setStartAddress, &Menu::setPauseBetweenReqs,
				&Menu::serUserAgent, 	&Menu::setMaxDepth,
				&Menu::switchInfiniteMode, &Menu::switchSearchInSE, 
				&Menu::selectSearchEngine, &Menu::displayPhrasesForSearch,
				&Menu::addPhraseForSearch
			};
			
			bool turnOnOffInfCrawl 	 = false;
			bool turnOnOffSearchInSE = false;

			ViewHandler::ConsoleCrawlerViewer ccv;
			CrawlerImpl::Crawler crawl;

			Apx::url startLnk;

			static const boost::regex getNumbers;

			void eatline(){ while(std::cin.get() != '\n'); }
			void explode(const std::string &str, std::vector<std::size_t> &result);
	};

}

#endif 
