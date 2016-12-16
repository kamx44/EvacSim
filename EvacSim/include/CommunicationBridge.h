#ifndef COMMUNICATIONBRIDGE_H
#define COMMUNICATIONBRIDGE_H
#include <unordered_map>
#include "Agent.h"

enum messageType{
    NOEXIT,
    MAIN_EXIT,
    EXIT
};

class CommunicationBridge
{
    public:
        CommunicationBridge();
        virtual ~CommunicationBridge();
        void sendMessage(unsigned int id,std::pair<messageType,std::pair<unsigned int, b2Vec2> > message);
        std::vector<std::pair<messageType,std::pair<unsigned int, b2Vec2>>> readMessage(unsigned int id);
        void addThread(int id);
        void clearAgentMessages(unsigned int id);
    protected:

    private:
        std::unordered_map<unsigned int,std::vector<std::pair<messageType,std::pair<unsigned int, b2Vec2>>> > dataContainer;
};

#endif // COMMUNICATIONBRIDGE_H
