#ifndef SERVER_EVENT_HANDLER_HPP
#define SERVER_EVENT_HANDLER_HPP

class ServerEventHandler {
    public:
        virtual ~ServerEventHandler() {}
        virtual void addWriteEvent(int fd) = 0;
        virtual void removeReadEvent(int fd) = 0;
        virtual void removeWriteEvent(int fd) = 0;
        virtual const std::string &getServerName() = 0;
};

#endif