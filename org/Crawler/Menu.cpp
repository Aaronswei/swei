#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <string>
#include "GeneralHelperFunctions.h"
#include "Menu.h"

const boost::regex UserInterface::Menu::getNumbers("^\\s*\\d+(?:\\s*,\\s*\\d+)*$");

UserInterface::Menu::Menu()
{
    crawl.setViewer(&ccv);
    printMenu();
    interrogation();
}

void UserInterface::Menu::interrogation()
{
    using namespace std;
    int command = -1;

    while(true)
    {
        bool validCmd = false;
        cout << "\nInput the command: ";

        while(!validCmd)
        {
            while(!(cin >> command))
            {
                cin.clear();
                string line;
                getline(cin, line);

                if(command == 0)
                {
                    cout << "\nSorry, but '" << line << "' is not a number.\n";
                    cout << "Input the valid command: ";
                }
                else
                {
                    break;
                }
            }

            if(command < 0 || command >= AMOUNT_HANDLERS)
            {
                cout << "\nSorry, but command '" << command << "' is undefined.\n";
                cout << "Input the valid command: ";
            }
            else
            {
                validCmd = true;
            }
        }

        (this->*runHandler[command])();
    }
}

void UserInterface::Menu::printMenu()
{
    using namespace std;
    string border = "****************************************************************************";

    cout << border;

    cout.width(border.size() - 1); cout.fill(' ');
    cout << left << endl <<  "|" << right << "|";
    cout.width(border.size() - 1); cout.fill(' ');
    cout << left << endl <<  "|" << right << "|";
    cout.width(border.size() - 1); cout.fill('_');
    cout << left << endl << "| 0. For exit from program." << right << "|";


    cout.width(border.size() - 1); cout.fill(' ');
    cout << left << endl <<  "|" << right << "|";
    cout.width(border.size() - 1); cout.fill('_');
    cout << left << endl <<  "| 1. Start." << right << "|";

    cout.width(border.size() - 1); cout.fill(' ');
    cout << left << endl <<  "|" << right << "|";
    cout.width(border.size() - 1); cout.fill('_');
    cout << left << endl <<  "| 2. Clear console window." << right << "|";

    cout.width(border.size() - 1); cout.fill(' ');
    cout << left << endl <<  "|" << right << "|";
    cout.width(border.size() - 1); cout.fill('_');
    cout << left << endl << "| 3. Set start address." << right << "|";

    cout.width(border.size() - 1); cout.fill(' ');
    cout << left << endl <<  "|" << right << "|";
    cout.width(border.size() - 1); cout.fill('_');
    cout << left << endl <<  "| 4. Set pause between HTTP requests(3000 milliseconds by default)." << right << "|";

    cout.width(border.size() - 1); cout.fill(' ');
    cout << left << endl <<  "|" << right << "|";
    cout.width(border.size() - 1); cout.fill('_');
    cout << left << endl <<  "| 5. Set signature which will be displayed in User-Agent." << right << "|";

    cout.width(border.size() - 1); cout.fill(' ');
    cout << left << endl <<  "|" << right << "|";
    cout.width(border.size() - 1); cout.fill('_');
    cout << left << endl <<  "| 6. Set max depth(0 is unlimited)." << right << "|";

    cout.width(border.size() - 1); cout.fill(' ');
    cout << left << endl <<  "|" << right << "|";
    cout.width(border.size() - 1); cout.fill('_');
    cout << left << endl <<  "| 7. Turn on/off infinite crawl." << right << "|";

    cout.width(border.size() - 1); cout.fill(' ');
    cout << left << endl <<  "|" << right << "|";
    cout.width(border.size() - 1); cout.fill('_');
    cout << left << endl <<  "| 8. Turn on/off search in search engine." << right << "|";

    cout.width(border.size() - 1); cout.fill(' ');
    cout << left << endl <<  "|" << right << "|";
    cout.width(border.size() - 1); cout.fill('_');
    cout << left << endl << "| 9. Select search engine(Yandex by default)." << right << "|";

    cout.width(border.size() - 1); cout.fill(' ');
    cout << left << endl <<  "|" << right << "|";
    cout.width(border.size() - 1); cout.fill('_');
    cout << left << endl << "| 10. Display phrases for search in search engine(phrases.txt)." << right << "|";

    cout.width(border.size() - 1); cout.fill(' ');
    cout << left << endl <<  "|" << right << "|";
    cout.width(border.size() - 1); cout.fill('_');
    cout << left << endl << "| 11. Add phrases for search." << right << "|";

    cout.width(border.size() - 1); cout.fill(' ');
    cout << left << endl <<  "|" << right << "|";
    cout.width(border.size() - 1); cout.fill(' ');
    cout << left << endl <<  "|" << right << "|";

    cout << endl << border << endl << endl;
}

// handlers
void UserInterface::Menu::quit()
{
    exit(EXIT_SUCCESS);
}

void UserInterface::Menu::start()
{
    using namespace std;

    eatline();
    string ready;

    cout << "\nCrawler will be started with configurations:\n\n"
            "Start address: " << crawl.getStartAddress() << endl <<
            "User-Agent: " << crawl.getSignature() << endl <<
            "Pause time: " << crawl.getTimePause() << MEASURE_OF_TIME << endl <<
            "Max Depth: " << crawl.getMaxDepth() << " links" << endl;

    if(crawl.getModeInfiniteCrawl())
    {
        cout << "Infinite crawl mode turned on.\n";
    }
    else
    {
        cout << "Infinite crawl mode turned off.\n";
    }

    cout << "Selected search system: " <<
            (crawl.getSelectedSE() == CrawlerImpl::Crawler::SEARCH_ENGINE::GOOGLE ? "Google\n" : "Yandex\n");
    cout << "Search in search system mode turned " << (crawl.getModeSearchInSE() ? "on\n\n" : "off\n\n");

    cout << "\nCase-insensitive 'q', 'quit' - exit from program\n"
            "Case-insensitive 'r', 'ret', 'return' - return to menu\n";

    if(crawl.readyForStart())
    {
        cout << "After starting will not be able to stop the program until it is finished.\n\nRun?(y/n): ";

        while(true)
        {
            getline(cin, ready);

            if(Apx::strToLower(ready) == "q" || Apx::strToLower(ready) == "quit")
            {
                exit(0);
            }

            if(Apx::strToLower(ready) == "r" || Apx::strToLower(ready) == "ret"
                || Apx::strToLower(ready) == "return" || Apx::strToLower(ready) == "n")
            {
                break;
            }

            if(Apx::strToLower(ready) != "y")
            {
                cout << "Invalid value. Please re-enter: ";
                continue;
            }

            if(Apx::strToLower(ready) == "y")
            {
                crawl.start();
                break;
            }
        }
    }
    else
    {
        cout << "Crawler don't ready for start.\n";
        cin.get();
    }
}

void UserInterface::Menu::clearScreen()
{
    system(CLEAR_SCREEN);
    printMenu();
}

void UserInterface::Menu::setStartAddress()
{
    using namespace std;

    eatline();
    bool successful = false;
    string address;

    while(!successful)
    {
        cout << "\nCase-insensitive 'q', 'quit' - exit from program\n"
                "Case-insensitive 'r', 'ret', 'return' - return to menu\n"
                "Please input valid address: ";

        while(!getline(cin, address))
        {
            cin.clear();
        }

        if(Apx::strToLower(address) == "q" || Apx::strToLower(address) == "quit")
        {
            exit(0);
        }

        if(Apx::strToLower(address) == "r" || Apx::strToLower(address) == "ret"
            || Apx::strToLower(address) == "return")
        {
            return;
        }

        startLnk = address;
        successful = startLnk.isAbsoluteAddr();
        if(!successful)
        {
            cout << "Invalid address!\n";
        }
    }

    if(Apx::strToLower(address) == "r" || Apx::strToLower(address) == "ret" || Apx::strToLower(address) == "return")
    {
        return;
    }

    cout << "Start address was set. \n";
    crawl.setStartAddress(address);
}

void UserInterface::Menu::setPauseBetweenReqs()
{
    using namespace std;

    cout << "\nCase-insensitive 'q', 'quit' - exit from program\n"
            "Case-insensitive 'r', 'ret', 'return' - return to menu\n"
            "Input pause time between HTTP requests in" MEASURE_OF_TIME ": ";

    unsigned timePause = -1; // big number
    string line;

    while(!(cin >> timePause))
    {
        cin.clear();
        getline(cin, line);

        if(!timePause)
        {
            if(Apx::strToLower(line) == "q" || Apx::strToLower(line) == "quit")
            {
                exit(0);
            }

            if(Apx::strToLower(line) == "r" || Apx::strToLower(line) == "ret"
                || Apx::strToLower(line) == "return")
            {
                return;
            }

            cout << "\nSorry, but '" << line << "' is not a number.\n";
            cout << "Input the valid command: ";
        }
        else
        {
            return;
        }
    }

    if(Apx::strToLower(line) == "r" || Apx::strToLower(line) == "ret"
            || Apx::strToLower(line) == "return")
    {
        return;
    }

    cout << "Pause time equal " << timePause << MEASURE_OF_TIME " was set.\n";
    crawl.setPause(timePause);
}

void UserInterface::Menu::setUserAgent()
{
    using namespace std;

    eatline();
    string signature;

    cout << "\nCase-insensitive 'q', 'quit' - exit from program\n"
            "Case-insensitive 'r', 'ret', 'return' - return to menu\n"
            "Please input signature: ";

    while(!getline(cin, signature))
    {
        cin.clear();
    }

    if(Apx::strToLower(signature) == "q" || Apx::strToLower(signature) == "quit")
    {
        exit(0);
    }

    if(Apx::strToLower(signature) == "r" || Apx::strToLower(signature) == "ret"
        || Apx::strToLower(signature) == "return")
    {
        return;
    }

    cout << "Signature: '" << signature << "' was set.\n";
    crawl.setSignature(signature);
}

void UserInterface::Menu::setMaxDepth()
{
    using namespace std;

    cout << "\nCase-insensitive 'q', 'quit' - exit from program\n"
            "Case-insensitive 'r', 'ret', 'return' - return to menu\n"
            "Input max depth: ";

    size_t depth = -1; // big number
    string line;

    while(!(cin >> depth))
    {
        cin.clear();
        getline(cin, line);

        if(!depth)
        {
            if(Apx::strToLower(line) == "q" || Apx::strToLower(line) == "quit")
            {
                exit(0);
            }

            if(Apx::strToLower(line) == "r" || Apx::strToLower(line) == "ret"
                || Apx::strToLower(line) == "return")
            {
                return;
            }

            cout << "\nSorry, but '" << line << "' is not a number.\n";
            cout << "Input the valid command: ";
        }
        else
        {
            return;
        }
    }

    cout << "Max depth equal " << depth << " links was set.\n";
    crawl.setMaxDepth(depth);
}

void UserInterface::Menu::switchInfiniteMode()
{
    using namespace std;

    turnOnOffInfCrawl = !turnOnOffInfCrawl;
    if(turnOnOffInfCrawl)
    {
        cout << "Infinite crawl mode turned on.\n";
    }
    else
    {
        cout << "Infinite crawl mode turned off.\n";
    }

    crawl.setModeInfiniteCrawl(turnOnOffInfCrawl);
}

void UserInterface::Menu::switchSearchInSE()
{
    using namespace std;

    turnOnOffSearchInSE = !turnOnOffSearchInSE;

    if(turnOnOffSearchInSE)
    {
        cout << "You should to make sure that " + CrawlerImpl::Crawler::storage_of_phrases + " contains phrases.\n";
        cout << "Search in search system turned on.\n";
    }
    else
    {
        cout << "Search in search system turned off.\n";
    }

    crawl.setModeSearchInSE(turnOnOffSearchInSE);
}

void UserInterface::Menu::selectSearchEngine()
{
    using namespace std;

    cout << "\nCase-insensitive 'q', 'quit' - exit from program\n"
            "Case-insensitive 'r', 'ret', 'return' - return to menu\n";

    cout << "\nInput 'G' or 'Google' if you want to search in Google\n"
            "Input 'Y' or 'Yandex' if you want to search in Yandex\n\n";

    cout << "Input name of search system: ";

    string se;
    eatline();

    while(true)
    {
        getline(cin, se);
        Apx::cutSpaceChars(se);
        se = Apx::strToLower(se);

        if(se == "q" || se == "quit")
        {
            exit(0);
        }

        if(se == "r" || se == "ret"
            || se == "return" || se == "n")
        {
            break;
        }


        if(se == "g" || se == "google")
        {
            crawl.setSearchEngine(CrawlerImpl::Crawler::SEARCH_ENGINE::GOOGLE);
            break;
        }
        else if(se == "y" || se == "yandex")
        {
            crawl.setSearchEngine(CrawlerImpl::Crawler::SEARCH_ENGINE::YANDEX);
            break;
        }
        else
        {
            cout << "Invalid value. Please re-enter: ";
            continue;
        }
    }
}

void UserInterface::Menu::displayPhrasesForSearch()
{
    using namespace std;

    ifstream in(CrawlerImpl::Crawler::storage_of_phrases, ios_base::in);
    if(in.is_open())
    {
        vector<string> lines;
        string line;

        while(getline(in, line))
        {
            Apx::cutSpaceChars(line);

            if(!line.empty())
            {
                lines.push_back(line);
            }
        }

        if(lines.empty())
        {
            cout << "\n" << CrawlerImpl::Crawler::storage_of_phrases << " is empty. ";
            cout << "You can to add phrases.\n";
        }
        else
        {
            cout << endl;

            auto size = lines.size();
            for(decltype(size) i = 0; i < size; ++i)
            {
                cout << setw(4) << (i + 1) << ". " + lines[i] << endl;
            }

            // handler of possibility delete phrases
            string ready, numberOfStrings;
            boost::smatch sm;

            cout << "\nCase-insensitive 'q', 'quit' - exit from program\n"
            "Case-insensitive 'r', 'ret', 'return' - return to menu\n";

            cout << "\nDelete phrase?(y/n): ";

            eatline();

            while(true)
            {
                getline(cin, ready);
                Apx::cutSpaceChars(ready);
                ready = Apx::strToLower(ready);

                if(ready == "q" || ready == "quit")
                {
                    exit(0);
                }

                if(ready == "r" || ready == "ret"
                    || ready == "return" || ready == "n")
                {
                    break;
                }


                if(ready == "y")
                {
                    cout << "\nExpects numbers as follow string: num1 [,num2,num3].\n";
                    cout << "Numbers of strings which need to delete: ";
                    // main handler

                    while(true)
                    {
                        getline(cin, numberOfStrings);
                        Apx::cutSpaceChars(numberOfStrings);
                        numberOfStrings = Apx::strToLower(numberOfStrings);

                        if(numberOfStrings == "q" || numberOfStrings == "quit")
                        {
                            exit(0);
                        }

                        if(numberOfStrings == "r" || numberOfStrings == "ret"
                            || numberOfStrings == "return" || numberOfStrings == "n")
                        {
                            break;
                        }

                        if(boost::regex_search(numberOfStrings, sm, getNumbers))
                        {
                            vector<size_t> numbers;
                            explode(numberOfStrings, numbers);
                            for(const auto &elem : numbers)
                            {
                                if(elem > lines.size())
                                {
                                    cout << "\nWarning: number " << elem << " not exists.\n"
                                            << "This number will be ignored.\n";
                                }

                                in.clear();
                                in.close();

                                General::writeBesides(CrawlerImpl::Crawler::storage_of_phrases, numbers, lines);

                                cout << "\nDone!\n";
                            }
                            break;
                        }
                        else
                        {
                            cout << "\nInvalid value. Please re-enter: ";
                            continue;
                        }
                    }

                    break;
                }
                else
                {
                    cout << "Invalid value. Please re-enter: ";
                    continue;
                }
            }
        }
    }
    else
    {
        cout << "Cannot open file for read. Possibly file does not exists.\n";
        cout << "You can to attempt add one or more phrases in order to create a file.\n";
    }
}

void UserInterface::Menu::addPhraseForSearch()
{
    using namespace std;
    cout << "\nAdd your phrase: ";

    string phrase;
    eatline();
    getline(cin, phrase);

    ofstream out(CrawlerImpl::Crawler::storage_of_phrases, ios_base::app);
    out << phrase << endl;

    cout << "\nDone!\n";
}

void UserInterface::Menu::explode(const std::string &str, std::vector<std::size_t> &result)
{
    auto end = str.find(",");
    decltype(end) beg = 0;

    // store num
    std::string tmpNum;

    while(end != std::string::npos)
    {
        tmpNum.assign(str, beg, end);
        Apx::cutSpaceChars(tmpNum);

        // ignore negative numbers
        long num = -1;
        if( (num = std::strtol(tmpNum.c_str(), 0, 10)) >= 0)
        {
            result.push_back(static_cast<std::size_t>(num));
        }

        beg = ++end;
        end = str.find(",", beg);
    }

    // read last number
    tmpNum.assign(str, beg, std::string::npos);
    Apx::cutSpaceChars(tmpNum);

    result.push_back(std::strtol(tmpNum.c_str(), 0, 10));
}
