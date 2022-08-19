#include "../include/Command.h"

Command::Command(std::string com, User* user){
    this->com = com;
    this->user = user;
}

Command::~Command(){

}

std::pair<int, std::string> Command::handle(){
    int space = com.find(' ');
    std::string head = com.substr(0, space);
    std::string param = space != -1 ? com.substr(space + 1) : "";
    // head.erase(head.find('\r'),2);
    // param.erase(param.find('\r'),2);
    while(head[head.length() - 1] == '\r' || head[head.length() - 1] == '\n'){
        head = head.substr(0, head.length() - 1);
    }
    while(param[param.length() - 1] == '\r' || param[param.length() - 1] == '\n'){
        param = param.substr(0, param.length() - 1);
    }
    std::cout<<"head is:"<<head<<std::endl;
    std::cout<<"param is: "<<param<<std::endl;
    std::cout<<"map result is:"<<com_map[head]<<std::endl;
    switch (com_map[head]){
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

        case QUIT:
            this->cmd_quit();
            break;

            
        default:
            std::cout<<"default case."<<std::endl;
            break;
    }
    return std::pair<int, std::string>(this->stat, this->ret);
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
    this->stat = 1;//providing service
    return 0;
}

int Command::cmd_list(std::string param){
    return 0;
}

int Command::cmd_syst(std::string param){
    ret = construct_ret(215, "ubuntu.");
    return 0;
}

int Command::cmd_quit(){
    ret = construct_ret(221, "byebye.");
    this->stat = -1;//shutdown
    return 0;
}

std::string Command::construct_ret(int statcode, std::string descript){
    return std::to_string(statcode) + " " + descript + "\n";
}