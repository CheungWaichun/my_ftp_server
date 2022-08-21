# include "../include/Acceptor.h"
#include <iostream>
Acceptor::Acceptor(ACE_INET_Addr &addr){
    this->open(addr);
    std::cout<<"i am a new acceptor."<<std::endl;
}

Acceptor::~Acceptor(){
    std::cout<<"i am an acceptor.i am dying."<<std::endl;
}

int Acceptor::open(ACE_INET_Addr &addr){
    this->acceptor.open(addr);
    return 0;
}

int Acceptor::handle_input(ACE_HANDLE fd){
    Handler *eh = new Handler();
    // ACE_INET_Addr client_addr;
    this->acceptor.accept(eh->get_stream(), 0, 0, 1);

    User *user = new User();
    eh->set_user(user);

    ACE_Reactor::instance()->register_handler(eh,ACE_Event_Handler::READ_MASK);

    eh->get_stream().send("220 Service ready for new user.\n",32);
    // std::cout<<"handle_input"<<std::endl;

    return 0;
}

ACE_HANDLE Acceptor::get_handle() const{
    return this->acceptor.get_handle();
}


int Acceptor::handle_close(ACE_HANDLE fd){

}



// int Acceptor::set_stream(ACE_SOCK_Stream& stream){
//     this->stream = stream;
// }