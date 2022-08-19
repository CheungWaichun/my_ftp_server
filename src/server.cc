#include "../include/Acceptor.h"
#include <iostream>
// #include <cstdlib>

int main(int argc, char* argv[]){
    if(argc < 3){
        std::cout<<"enter ip and port !!!"<<std::endl;
        ACE_OS::exit(1);
    }
    ACE_INET_Addr addr(((std::string)argv[1] + ":" + argv[2]).c_str());
    Acceptor *eh = new Acceptor(addr);

    // eh->reactor(ACE_Reactor::instance());

    ACE_Reactor::instance()->register_handler(eh, ACE_Event_Handler::ACCEPT_MASK);
    
    ACE_Reactor::instance()->run_reactor_event_loop();

}