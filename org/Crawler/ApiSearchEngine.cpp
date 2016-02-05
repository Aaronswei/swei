#include "Crawler.h"
#include "GeneralHelperFunctions.h"

// requests search engine in order to
// get new external URLs
bool CrawlerImpl::Crawler::requestSearchEngine()
{
    request_.clear();

    std::string host = getSelectedSE() == SEARCH_ENGINE::GOOGLE ? "www.google.ru" : "www.yandex.ru";

    request_.setHost(host);
    request_.setConnectionType(HttpLib::HttpRequest::CONNECTION_TYPE::CLOSE);

    std::ifstream in(storage_of_phrases);
    if(in.is_open())
    {
        std::vector<std::string> lines;
        std::string line;

        while(getline(in, line))
        {
            Apx::cutSpaceChars(line);
            if(!line.empty())
            {
                lines.push_back(line);
            }
        }

        if(!lines.empty())
        {
            auto size = lines.size();
            decltype(size) i;
            Apx::TagParser parser;

            for(i = 0; i < size; ++i)
            {
                line = lines[i];
                std::string relPath = getSelectedSE() == SEARCH_ENGINE::GOOGLE ? "" : "/yandsearch?text=";
                relPath += General::toWindows1251(line);
                request_.setRelativePath(relPath + "&p=0");
                request_.build();

                try
                {
                    response_ = controller_.openUrl(request_);
                    if(response_->isValid())
                    {
                        parser.parseTags(response_->getEntityBody(), "a");

                        // set for true work method addToQueue
                        // which compare all urls with "host_"
                        host_ = getSelectedSE() == SEARCH_ENGINE::GOOGLE ? "www.google.ru" : "www.yandex.ru";

                        // add external URLs which not exists in EXTERNAL and INDEXED_EX queues
                        addToQueue(parser, request_.getHost() + request_.getRelativePath(), URLS::EXTERNAL);

                        if(externalLinks_.size())
                        {
                            for(auto beg = externalLinks_.begin(), end = externalLinks_.end(); beg != end; ++beg)
                            {
                                if(beg->find("yandex.ru") != std::string::npos)
                                {
                                    std::cout << *beg << std::endl;
                                    externalLinks_.erase(beg);
                                }
                            }

                            for(const auto &url : externalLinks_)
                            {
                                std::cout << url << std::endl;
                            }

                            return true;
                        }
                    }
                    else
                    {
                        // invalid response from server
                        return false;
                    }
                }
                catch(const HttpLib::exFailHttp &ex)
                {
                    continue;
                }
            }

            //std::ofstream out(storage_of_phrases);
            // write data in phrases.txt
            // within delete used phrases
        }
        else
        {
            viewerDevice_->viewWarning(storage_of_phrases + " is empty.\n");
            return false;
        }
    }
    else
    {
        viewerDevice_->viewWarning("Cannot open " + storage_of_phrases + "\n");
        return false;
    }

    return false;
}
