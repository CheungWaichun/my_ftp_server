#include "../include/Acceptor.h"
#include <iostream>
#include <cstdlib>
// using namespace std;
int main(int argc, char* argv[]){
    std::cout<<"hi"<<std::endl;
    // std::printf("hi");
    ACE_INET_Addr addr(8888);
    Acceptor *eh = new Acceptor(addr);
    // printf("hi");
    std::cout<<"hi"<<std::endl;

    ACE_Reactor::instance()->register_handler(eh, ACE_Event_Handler::ACCEPT_MASK);
    printf("hi");
    std::cout<<"hi"<<std::endl;

    while(1)
        ACE_Reactor::instance()->handle_events();
}