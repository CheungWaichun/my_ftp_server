#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include "ace/SOCK_Acceptor.h"
#include "ace/Event_Handler.h"
#include "ace/SOCK_Stream.h"
#include "ace/INET_Addr.h"
#include "User.h"
#include "Handler.h"

class Acceptor:public ACE_Event_Handler{
public:
    Acceptor(ACE_INET_Addr &addr);
    ~Acceptor();
    //
    int open(ACE_INET_Addr &addr);

    int handle_input(ACE_HANDLE);
    int handle_close(ACE_HANDLE);

    ACE_HANDLE get_handle() const;
    // int set_stream(ACE_SOCK_Stream& stream);

private:
    ACE_SOCK_Acceptor acceptor;
    // ACE_SOCK_Stream stream;
    ACE_INET_Addr addr;


};


#endif