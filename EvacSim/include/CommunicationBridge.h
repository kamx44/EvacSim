#ifndef COMMUNICATIONBRIDGE_H
#define COMMUNICATIONBRIDGE_H
#include <unordered_map>
#include "Actor.h"

enum messageType{
    NOEXIT,
    EXIT
};

class CommunicationBridge
{
    public:
        CommunicationBridge();
        virtual ~CommunicationBridge();
        void sendMessage(unsigned int id,std::pair<messageType,std::pair<unsigned int, b2Vec2> > message);
        std::pair<messageType,std::pair<unsigned int, b2Vec2> > readMessage(unsigned int id);
        void addThread(int id);
    protected:

    private:
        std::unordered_map<unsigned int,std::vector<std::pair<messageType,std::pair<unsigned int, b2Vec2>>> > dataContainer;
};

#endif // COMMUNICATIONBRIDGE_H