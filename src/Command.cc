#include "../include/Command.h"

Command::Command(std::string com, User* user){
    this->com = com;
    this->user = user;
}

Command::~Command(){

}

std::string Command::handle(){
    int space = com.find(' ');
    std::string head = com.substr(0, space);
    std::cout<<"head is:"<<head<<std::endl;
    std::string param = com.substr(space + 1);
    param.erase(param.find('\r'));
    std::cout<<"param is: "<<param<<std::endl;
    switch (com_map[head])
    {
    case USER:
        this->cmd_user(param);
        break;
    
    case PASS:
        this->cmd_pass(param);
        break;


    case LIST:
        this->cmd_list(param);
        break;

    case SYST:
        this->cmd_syst(param);
        break;
    default:
        std::cout<<"default case."<<std::endl;
        break;
    }
    std::string rt = this->ret;
    this->ret = "-1";
    return rt;
}

int Command::cmd_user(std::string param){
    std::cout<<"user param is: "<< param<<std::endl;
    std::cout<<"param size :"<<param.size()<<std::endl;
    // std::cout<<"name size:"<<this->user->get_name().size()<<std::endl;
    // std::printf("%d%d", param[5],param[6]);
    if(param.compare(this->user->get_name()) == 0){
        this->ret = construct_ret(331, "need password.");
    }else{
        this->ret = construct_ret(530, "user not found!");
    }
    return 0;
}

int Command::cmd_pass(std::string param){
    std::cout<<"pass param is: "<< param<<std::endl;
    if(!param.compare(this->user->get_password())){
        ret = construct_ret(230, "login success.");
    }else{
        ret = construct_ret(530, "wrong password!");
    }
    return 0;
}

int Command::cmd_list(std::string param){
    return 0;
}

int Command::cmd_syst(std::string param){
    ret = construct_ret(215, "ubuntu.");
    return 0;
}

std::string Command::construct_ret(int statcode, std::string descript){
    return std::to_string(statcode) + " " + descript + "\n";
}