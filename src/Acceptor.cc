# include "../include/Acceptor.h"

Acceptor::Acceptor(ACE_INET_Addr &addr){
    this->open(addr);
    // std::cout<<"i am a new control_acceptor."<<std::endl;
}

Acceptor::~Acceptor(){
    // std::cout<<"i am an control_acceptor.i am dying."<<std::endl;
}

int Acceptor::open(ACE_INET_Addr &addr){
    this->control_acceptor.open(addr);
    std::cout<<"waiting for new connection."<<std::endl;
    return 0;
}

int Acceptor::handle_input(ACE_HANDLE fd){
    Handler *eh = new Handler();
    this->control_acceptor.accept(eh->get_control_stream(), 0, 0, 1);

    User *user = new User();
    eh->set_user(user);
    // std::cout<<"tid:"<<syscall(SYS_gettid)<<std::endl;

    ACE_Reactor::instance()->register_handler(eh->get_control_stream().get_handle(),eh,ACE_Event_Handler::READ_MASK);
    eh->get_control_stream().send("220 Service ready for new user.\n",32);

    return 0;//0:keep listening; -1:suicide
}

ACE_HANDLE Acceptor::get_handle() const{
    return this->control_acceptor.get_handle();
}


int Acceptor::handle_close(ACE_HANDLE fd){
    return 0;
}
