#include "CommunicationBridge.h"

CommunicationBridge::CommunicationBridge()
{

}

CommunicationBridge::~CommunicationBridge()
{
    //dtor
}

void CommunicationBridge::sendMessage(unsigned int id,std::pair<messageType,glm::vec2> message){
    //std::cout<<"  wysylam do: "<<id<<" wiad:"<<message.first<<", ["<<message.second.x<<"]["<<message.second.y<<"]"<<std::endl;
    auto it = dataContainer.find(id);
    (*it).second.push_back(message);
    dataContainer[id]=(*it).second;

}

std::pair<messageType,glm::vec2> CommunicationBridge::readMessage(unsigned int id){
    auto it = dataContainer.find(id);
    if(!(*it).second.empty()){
        std::pair<messageType,glm::vec2> newMessage = (*it).second.at(0);
       // std::cout<<"  czytam: "<<" wiad:"<<newMessage.first<<", ["<<newMessage.second.x<<"]["<<newMessage.second.y<<"]"<<std::endl;
        (*it).second.pop_back();
        return newMessage;
    }else{
        return std::pair<messageType,glm::vec2>();
    }
}

void CommunicationBridge::addThread(int id){
    std::vector<std::pair<messageType,glm::vec2>> messagesList;
    dataContainer.insert(std::make_pair(id,messagesList));
}
