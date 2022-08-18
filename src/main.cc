#include "../include/Acceptor.h"

int main(int argc, char* argv[]){
    ACE_INET_Addr addr(8888);
    Acceptor *eh = new Acceptor(addr);
    ACE_Reactor::instance()->register_handler(eh, ACE_Event_Handler::ACCEPT_MASK);
    while(1)
        ACE_Reactor::instance()->handle_events();
}