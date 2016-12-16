#include "CommunicationBridge.h"

CommunicationBridge::CommunicationBridge()
{

}

CommunicationBridge::~CommunicationBridge()
{
    //dtor
}

void CommunicationBridge::sendMessage(unsigned int id,std::pair<messageType,std::pair<unsigned int, b2Vec2> > message){
    auto it = dataContainer.find(id);
    (*it).second.push_back(message);
    dataContainer[id]=(*it).second;
}

std::vector<std::pair<messageType,std::pair<unsigned int, b2Vec2>>> CommunicationBridge::readMessage(unsigned int id){
    int i = dataContainer.count(id);
    if(i != 0){
        auto it = dataContainer.find(id);
        if(!(*it).second.empty()){
            std::vector<std::pair<messageType,std::pair<unsigned int, b2Vec2>>> newMessages = (*it).second;
            (*it).second.clear();
            return newMessages;
        }else{
            return std::vector<std::pair<messageType,std::pair<unsigned int, b2Vec2>>>();
        }
    }
}

void CommunicationBridge::addThread(int id){
    std::vector<std::pair<messageType,std::pair<unsigned int, b2Vec2> >> messagesList;
    dataContainer.insert(std::make_pair(id,messagesList));
}

void CommunicationBridge::clearAgentMessages(unsigned int id){
    if(dataContainer.count(id) > 0){
        std::vector<std::pair<messageType,std::pair<unsigned int, b2Vec2> >> messagesList;
        dataContainer[id] = messagesList;
    }
}
