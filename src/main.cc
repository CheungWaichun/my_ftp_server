#include "../include/Acceptor.h"
#include <iostream>
// #include <cstdlib>
// using namespace std;
int main(int argc, char* argv[]){
    if(argc < 3){
        ACE_OS::exit(1);
    }
    ACE_INET_Addr addr(((std::string)argv[1] + ":" + argv[2]).c_str());
    Acceptor *eh = new Acceptor(addr);

    ACE_Reactor::instance()->register_handler(eh, ACE_Event_Handler::ACCEPT_MASK);
    while(1)
        ACE_Reactor::instance()->handle_events();
}