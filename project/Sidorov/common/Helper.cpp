#include "Helper.h"

ByteBufferPtr Helper::eraseCodeCommand(ByteBufferPtr bufferPtr)
{
    uint8_t code = bufferPtr->at(0);
    std::string s(bufferPtr->data());
    s.erase(s.begin());
    ByteBufferPtr ptr(new ByteBuffer(s.begin(),s.end()));
    return ptr;
}

void Helper::addCodeCommand(CodeCommand code, ByteBufferPtr bufferPtr)
{
   bufferPtr->emplace(bufferPtr->begin(), static_cast<uint8_t>(code));
}

BuffersVector Helper::addBufferSize(ByteBufferPtr buffer)
{
    BuffersVector result;

    ByteBufferPtr bufferSize_(new ByteBuffer());
    bufferSize_->resize(2);
    int size = buffer->size();

    (*bufferSize_)[0] = ((size & 0xff00) << 8);
    (*bufferSize_)[1] = (size & 0x00ff);
    result.push_back(bufferSize_);
    result.push_back(buffer);

    return result;
}

BufferSequence Helper::toBufferSequence(BuffersVector buffers)
{
    BufferSequence result;    
    for ( ByteBufferPtr buffer : buffers)
    {
        result.push_back(asio::buffer(*buffer));
    }
    return result;
}

std::vector<std::string> Helper::splitCommandAndData(const std::string &s)
{
    std::vector<std::string> tokens;
    tokens.push_back(s.substr(0,s.find(' ')));
    tokens.push_back(s.substr(s.find(' ')+1));
    return tokens;
}

std::pair<CodeCommand, ByteBufferPtr> Helper::getCodeAndData(const std::string &str)
{
    std::istringstream ist(str);

        std::string codeStr;
        ist >> codeStr;

        int code;

        try{
            code = std::stoi(codeStr);
        }
        catch(const std::invalid_argument& exp)
        {
            std::cout << "Wrong number of command!" << std::endl;
            return std::pair<CodeCommand, ByteBufferPtr>();
        }

        if(code < static_cast<int>(CodeCommand::LOGIN) || code > static_cast<int>(CodeCommand::DISCONNECT_FROM_USER))
        {
            std::cout << "Wrong number of command!" << std::endl;
            return std::pair<CodeCommand, ByteBufferPtr>();
        }


        CodeCommand commandCode = static_cast<CodeCommand>(code);

        std::string data;
        ist >> data;

        ByteBufferPtr buff = std::make_shared<ByteBuffer>();

        if(!data.empty())
            buff = std::make_shared<ByteBuffer>(data.begin(), data.end());

        return std::make_pair(commandCode, buff);
}


