
#ifndef HANDLER_H
#define HANDLER_H

#include "ace/Event_Handler.h"
#include "ace/Reactor.h"
#include "User.h"
#include "ace/SOCK_Stream.h"
#include "ace/Log_Msg.h"
#include "Command.h"

class Handler:public ACE_Event_Handler{
public:
    Handler();
    ~Handler();
    int set_user(User* user);
    int set_stream(ACE_SOCK_Stream stream);
    // int register_read();
    int handle_input(ACE_HANDLE);
    ACE_HANDLE get_handle() const;
    // ACE_SOCK_Stream &stream_i(){
    //     return this->stream_;
    // };
    ACE_SOCK_Stream& get_stream();


private:
    User* user;
    ACE_SOCK_Stream stream;
    char data[128];


};





#endif