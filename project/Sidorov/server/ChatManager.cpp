#include "ChatManager.h"

ChatManager::ChatManager(Server &server)
{
    server.subscribe(std::bind(
                         &ChatManager::onConnected
                         , this
                         , std::placeholders::_1));
}

void ChatManager::onConnected(ChatSessionPtr session)
{
    LOG_INFO("Connected session.");
    session->setHandleRead(std::bind(&ChatManager::processCommand
                                , this
                                , std::placeholders::_1
                                , std::placeholders::_2));
    sessions_.push_back(session);
}

void ChatManager::processCommand(ChatSessionPtr chatSession, ByteBufferPtr bufferPtr)
{
    CodeCommand code = static_cast<CodeCommand>(bufferPtr->at(0));
    //LOG_INFO("Your code command = " << code);
    ByteBufferPtr data ( Helper::eraseCodeCommand(bufferPtr));
}

