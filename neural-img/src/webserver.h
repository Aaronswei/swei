/*
 * webserver.h  -- this is the embedded web server for the neural2d program
 */
#ifndef NEURAL_WEBSERVER
#define NEURAL_WEBSERVER

#include <string>

//For web server:
#include <sys/socket.h>
#include <netinet/in.h>

#include "messagequeue.h"

namespace NNet{
    
    class WebServer
    {
        public:
            WebServer(void);
            ~WebServer(void);
            void start(int portNumber, MessageQueue &messages);
            void stopServer(void);
            void sendHttpResponse(std::string parameterBlock, int httpResponseFileDes);
            void webServerThread(int portNumber, MessageQueue &messageQueue);
            int portNumber;
            int socketFd;

        private:
            void initializeHttpResponse(void);
            void extractAndQueueMessage(std::string s, int httpConnectionFd, MessageQueue &messages);
            void replyToUnknownRequest(int httpConnectionFd);

            bool firstAccess; 
            std::string firstPart; //First part of the HTTP respone
            std::string secondPart; //Last part of the HTTP respone
    };
}

#endif
