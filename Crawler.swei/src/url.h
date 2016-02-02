#ifndef WEBLINK_H_INCLUDED
#define WEBLINK_H_INCLUDED

#include <boost/regex.hpp>
#include <stdexcept>
#include <utility>
#include <vector>

namespace Apx
{
	/**********/
	class ErrorUrl: public std::runtime_error
	{
		std::string badUrl_;
	public:
		ErrorUrl(const std::string &details, const std::string &badUrl)
			: std::runtime_error(details), badUrl_(badUrl){}

		ErrorUrl(const char *details, const char *badUrl)
			: std::runtime_error(details), badUrl_(badUrl){}

		virtual ~ErrorUrl(){}
		
		virtual const char *what() const noexcept{
			return ("Crash regex algorithm when parsing URL: " + badUrl_ + "\n"
				"==================================================\n"
				"Details: "+std::runtime_error::what() + "\n"
				"==================================================\n\n").c_str();
		}
	};

	class Url{
	public:
		
		enum class TYPE_OF_FILE
		{
			UNDEFINED,
			IMAGE,
			ARCHIVE,
			DOCUMENT,
			TEXT,
			AUDIO,
			VIDEO,
			EXECUTE_WEB_FILE
		};

		Url(bool parseCatFile = true);
			Url(const char *lnk, bool parseCatFile = true);
			Url(const std::string &lnk, bool parseCatFile = true);

			bool operator==(const Url &cmpLnk) const;
			bool operator!=(const Url &cmpLnk) const;

		operator bool() const {
			return (anchor_ || absoluteAddr_ || relativeAddr_);
			}

		Url &operator=(const char *lnk);
		Url &operator=(const std::string &lnk);

		void clear();

		void parse(const std::string &testLnk);

		bool compareHost(const Url &cmpLnk) const;

		int isSetVariable(const std::string &nameOfVar);

		void determineContentType(bool dContentType);

		std::string getValOfVar(const std::string &nameOfVar);

		std::string getValOfVar(std::size_t num);

		bool isAnchor()  		const {	return anchor_; 		}
		bool isAbsoluteAddr()		const { return absoluteAddr_;		}
		bool isRelativeAddr()		const { return relativeAddr_;		}

		const TYPE_OF_FILE &getTypeOfFile()	  const { return type_;		    }
		const std::string  &getExtenstionOfFile() const { return extensiontOfFile_; }
		const std::string  &getProtocol()	  const { return protocol_;	    }
		const std::string  &getHost()		  const { return host_;		    }
		const std::string  &getRelativePath()     const { return path_;		    }
		const std::string  &getRelativeDir()	  const { return relativeDir_;	    }
		const std::string  &getFile()		  const { return file_;		    }
		const std::string  &getAnchor()		  const { return nameAnchor_;	    }
		const std::string  &getTopLevelDomain()	  const { return topLevelDomain_;   }

		const std::string  &getVariablesString()  const { return variablesString_;  }

	//=================================================================
	
	private:
		
		bool anchor_;
		bool absoluteAddr_;
		bool relativeAddr_;
		bool determinContentType_;

		std::string protocol_;
		std::string host_;
		std::string path_;
		std::string relativeDir_;
		std::string file_;
		std::string nameAnchor_;
		std::string extensionOfFile_;
		std::string variablesString_;
		std::string topLevelDomain_;
		TYPE_OF_FILE type_=TYPE_OF_FILE::UNDEFINED;

		std::vector<std::pair<std::string . std::String>>variables_;

		void ampToSymb();

		void parseRelativePath();
		void parseGetVars(const std::string &vars);

		void removeAnchor();

		static boost::regex statPtnAbsoluteLink;
		static boost::regex statPtnRelativeLink;
		static const std::string extensionsOfExecuteWebFiles[];

		bool isExtMatchWebPage(const std::string &extension);

	};
		
}

#endif //WEBLINK_H_INCLUDED
