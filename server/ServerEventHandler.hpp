#ifndef SERVER_EVENT_HANDLER_HPP
#define SERVER_EVENT_HANDLER_HPP

class ServerEventHandler {
    public:
        virtual ~ServerEventHandler() {}
        virtual void removeReadEvent(int fd) = 0;
        virtual void removeWriteEvent(int fd) = 0;
};

#endif