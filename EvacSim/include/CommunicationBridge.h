#ifndef COMMUNICATIONBRIDGE_H
#define COMMUNICATIONBRIDGE_H
#include <unordered_map>
#include "Actor.h"

enum messageType{
    EXIT,
    NOEXIT
};

class CommunicationBridge
{
    public:
        CommunicationBridge();
        virtual ~CommunicationBridge();
        void sendMessage(unsigned int id,std::pair<messageType,glm::vec2> message);
        std::pair<messageType,glm::vec2> readMessage(unsigned int id);
        void addThread(int id);
    protected:

    private:
        std::unordered_map<unsigned int,std::vector<std::pair<messageType,glm::vec2>> > dataContainer;
};

#endif // COMMUNICATIONBRIDGE_H
