#ifndef HELPER_H
#define HELPER_H

#include <Define.h>
#include <Helper.h>
#include <pqxx/pqxx>


typedef std::vector<ByteBufferPtr> BuffersVector;
typedef std::vector<asio::const_buffer> EndBuffer;
class Helper
{
public:
    static BuffersVector addSize(ByteBufferPtr buffer);
    static EndBuffer makeEndBuffer(BuffersVector buffers);
    static uint16_t getSize(uint16_t left, uint16_t right);
    static std::string makeRegisterMessage(std::string message);
    static std::string makeListMessage();
    static std::string makeCreateChatMessage(std::string message);
    static std::string makeYesMessage(int currentRoom);
    static std::string makeHistoryMessage();
    static std::string makeExitMessage();
    static std::string makeRoomListMessage();
    static std::string makeRoomMessage(int currentRoom);
    static std::string makeAddMessage(std::string message);
    static std::string makeSetRoomNameMessage(std::string message);
    static std::string makeChatMessage(std::string message);
    static std::string makeCreateNewUserMessage(std::string message);
    static std::string makeLoginMessage(std::string message);

};

#endif // HELPER_H
