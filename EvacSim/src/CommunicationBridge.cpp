#include "CommunicationBridge.h"

CommunicationBridge::CommunicationBridge()
{

}

CommunicationBridge::~CommunicationBridge()
{
    //dtor
}

void CommunicationBridge::sendMessage(unsigned int id,std::pair<messageType,std::pair<unsigned int, b2Vec2> > message){
    //std::cout<<"  wysylam do: "<<id<<" wiad:"<<message.first<<", ["<<message.second.second.x<<"]["<<message.second.second.y<<"]"<<std::endl;
    auto it = dataContainer.find(id);
    (*it).second.push_back(message);
    dataContainer[id]=(*it).second;

}

std::pair<messageType,std::pair<unsigned int, b2Vec2> > CommunicationBridge::readMessage(unsigned int id){

    auto it = dataContainer.find(id);
    if(!(*it).second.empty()){
        std::pair<messageType,std::pair<unsigned int, b2Vec2> > newMessage = (*it).second.at(0);
        //std::cout<<"  czytam: "<<" wiad:"<<newMessage.first<<", ["<<newMessage.second.second.x<<"]["<<newMessage.second.second.y<<"]"<<std::endl;
        (*it).second.pop_back();
        return newMessage;
    }else{
        return std::pair<messageType,std::pair<unsigned int, b2Vec2> >();
    }
}

void CommunicationBridge::addThread(int id){
    std::vector<std::pair<messageType,std::pair<unsigned int, b2Vec2> >> messagesList;
    dataContainer.insert(std::make_pair(id,messagesList));
}
