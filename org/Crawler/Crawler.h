#ifndef CRAWLER_H
#define CRAWLER_H

#include <cstddef>
#include <cassert>
#include <string>
#include <deque>
#include <fstream>
#include <vector>
#include <exception>
#include "HttpLib/HttpResponse.h"
#include "HttpLib/HttpRequest.h"
#include "HttpLib/HttpConnection.h"
#include "ViewHandler/CrawlerViewer.h"
#include "Apx/Apx.h"
#include "Apx/Url.h"

#if PLATFORM == PLATFORM_WINDOWS

#define SLEEP(ms) Sleep(ms)

#else

#define SLEEP(ms) usleep(ms * 1000)

#endif


namespace CrawlerImpl
{

    /**************************************************************************************************************
     *
     *   #1. Object of type Crawler saves in queue of internal links, only relative part of link.
     *   Likewise saves in indexed queue. The relative path URL must start with character slash "/".
     *
     **************************************************************************************************************
     *
     *   #2. Conversion functions relative addresses returns a new address, starting with the character slash "/".
     *
     **************************************************************************************************************
     *
     *   #3. Single slash - "/" in internal and indexed queues denotes the root of the site.
     *
     **************************************************************************************************************
     */


    class Crawler
    {
    public:

        enum class SEARCH_ENGINE
        {
              GOOGLE    // HttpLib don't support HTTPS
            , YANDEX    // http://yandex.ru/yandsearch?text=request
        };

    private:

        // for determine internal/external/indexed queues
        // 0, 2, 4 for support transmission of packets
        // like this call as: function(FIRST_CONST | SECOND_CONST | THIRD_CONST);
        enum { INTERNAL, EXTERNAL = 2, INDEXED_IN = 4, INDEXED_EX = 8 };

        // for addToQueue. This constants specifies which urls
        // should get from html page
        enum class URLS { INTERNAL, EXTERNAL, DEFAULT };

        std::deque<std::string> indexedInternal_;
        std::deque<std::string> indexedExternal_;
        std::deque<std::string> internalLinks_;
        std::deque<std::string> externalLinks_;

        // pointer because objects of type HttpResponse
        // may create and edit only objects of type HttpConnection
        const HttpLib::HttpResponse     *   response_;
        HttpLib::HttpRequest                request_;
        HttpLib::HttpConnection             controller_;	// managing the communication with the HTTP server

        Apx::Url host_;					    // main website URL to crawl
        std::size_t timePause_;				// between requests
        std::string signature_;				// full signature in User-agent
        std::string sign_;					// original signature of crawler
        SEARCH_ENGINE whereSearch_;         // where search if ended queue of external links
        std::size_t maxDepth_;

        bool readyForStart_;
        bool infiniteCrawl_;
        bool searchInSE_;

        const ViewHandler::CrawlerViewer *viewerDevice_ = nullptr;

        // =================================================================================

        // main cycle of index website
        void crawlStart();
        void crawlResource();

        // the second argument is expected
        // relative address indicating the file or without it
        void addToQueue(const Apx::TagParser &parser,
                        const Apx::Url &relativePath,
                        URLS urls = URLS::DEFAULT);

        // finds link in indexed and internal queues
        //bool existsInQueues(const std::string &lnk, int whereToSearch = INTERNAL);
        bool existsInQueues(const Apx::Url &url, int whereToSearch = INTERNAL);

        // add link into specified queue with check on uniqueness
        // if addition into internal queue then adds only relative path.
        // if addition into external queue then adds subdomain + host
        void addLinkToQueue(const Apx::Url &url, int whereToSearch);

        // the flags you can pass through |. Old of C method.
        void clearQueue(int queueMark);

        // reset configurations with clear internal and indexedInternal queues
        void configByDefault();

        // this interrupt need for console interface
        // by pressing on key ENTER, execute is interrupts
        // working only on *nix systems.
        void interruptForEnter(long ms);

        // requests search engine in order to
        // get new external URLs
        bool requestSearchEngine();

        /** API for conversion true relative addresses **/

        // converts the relative path in accordance with the address where the received link
        // expects that second parameter is set by the website root
        std::string convertRelativeAddr(const Apx::Url &relAddr, const Apx::Url &where);

        // divides path by folders
        // expects path following [/]aaa/[bbb/[page.php[?var1=val&var2=val]]]
        // the last slash should be, otherwise will not correct the results
        std::vector<std::string> dividePath(const std::string &path);

    public:

        Crawler();
        Crawler(const std::string &startAddr);
        Crawler(const std::string &startAddr, std::size_t timePause);

        /** configuration - setters **/
        void setStartAddress(const std::string &startAddr);

        /** inline setters **/

        void setSignature(const std::string &signature)
        {
            signature_ = sign_ + signature;
            request_.setUserAgent(signature_);
        }

        void setViewer(const ViewHandler::CrawlerViewer *viewer) { viewerDevice_ = viewer;   }
        void setPause(std::size_t timePause)                     { timePause_ = timePause;   }
        void setMaxDepth(std::size_t depth = 0)                  { maxDepth_ = depth;        }
        void setModeInfiniteCrawl(bool val)                      { infiniteCrawl_ = val;     }
        void setModeSearchInSE(bool val)                         { searchInSE_ = val;        }
        void setSearchEngine(const SEARCH_ENGINE &se)            { whereSearch_ = se;        }

        /** ******************* **/

        static const std::string storage_of_phrases;    // stores name of file which consists from words for generate phrases
        static const std::string report_url;            // name of file that stores errors generated by the class Apx::Url
        static const std::string storage_urls;          // name of file that stores the addresses of sites passed

        bool readyForStart();
        void start();

        void clearInternalQueue();
        void clearExternalQueue();
        void clearIndexedInQueue();
        void clearIndexedExQueue();

        /** configuration - inline getters **/
        std::size_t getTimePause()              const { return timePause_;              }
        std::size_t getMaxDepth()               const { return maxDepth_;               }

        bool getModeInfiniteCrawl()             const { return infiniteCrawl_;          }
        bool getModeSearchInSE()                const { return searchInSE_;             }

        const SEARCH_ENGINE &getSelectedSE()    const { return whereSearch_;            }
        const std::string &getSignature()       const { return signature_;              }
        const std::string &getStartAddress()    const { return host_.getHost();         }

    };

}

#endif
