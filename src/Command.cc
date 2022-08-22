#include "../include/Command.h"

Command::Command(std::string com, User* user){
    this->com = com;
    this->user = user;
    std::cout<<"i am a new Command."<<std::endl;
}

Command::~Command(){
    std::cout<<"i am a Command, i am dying."<<std::endl;
}

void Command::handle(){
    int space = com.find(' ');
    std::string head = com.substr(0, space);
    std::string param = space != -1 ? com.substr(space + 1) : "";

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

        case PORT:
            this->cmd_port(param);
            break;
            
        case TYPE:
            this->cmd_type(param);
            break;

        
        default:
            std::cout<<"default case."<<std::endl;
            break;
    }


    return;
}



int Command::cmd_list(std::string param){
    FILE *fp;
    std::string cmd = "ls -a " + this->user->get_current_dir();
    std::string data = "";
    if((fp = popen(cmd.c_str(), "r")) == NULL){
        std::cout << "error" << std::endl;
    }else{
        char row[1024];
        // this->user->send_control_msg(construct_ret(125, "ready!"));

        while(ACE_OS::fgets(row, sizeof(row), fp)){
            data += row;
            // this->user->send_data_msg_buf(row, 1024);
            // ACE_OS::memset(row, 0, sizeof(row));
        }
    }
    pclose(fp);

    // data += '\n';

    this->user->send_control_msg(construct_ret(125, "ready!"));
    this->user->send_data_msg(data);
    std::cout<<"after send data"<<std::endl;
    this->user->close_data_stream();
    this->user->send_control_msg(construct_ret(250, "completed!"));

    return 0;
}

int Command::cmd_pass(std::string param){
    std::cout<<"pass param is: "<< param<<std::endl;
    if(!param.compare(this->user->get_password())){
        this->user->send_control_msg(construct_ret(230, "login success."));
    }else{
        this->user->send_control_msg(construct_ret(530, "wrong password!"));
    }

    return 0;
}

int Command::cmd_port(std::string raw_addr){
    //
    std::cout<<"raw_addr is:"<<raw_addr<<std::endl;
    ACE_INET_Addr addr = port_string_to_INET(raw_addr);
    this->user->set_client_data_addr(addr);
    this->user->init_data_stream();
    this->user->send_control_msg(construct_ret(200, "PORT set."));
    return 0;
}

int Command::cmd_quit(){
    this->user->send_control_msg(construct_ret(221, "byebye."));

    return 0;
}

int Command::cmd_syst(std::string param){
    this->user->send_control_msg(construct_ret(215, "ubuntu."));
    return 0;
}

int Command::cmd_type(std::string param){
    this->user->send_control_msg(construct_ret(220, "Type set to xxx."));
    return 0;
}

int Command::cmd_user(std::string param){
    std::cout<<"user param is: "<< param<<std::endl;
    std::cout<<"param size :"<<param.size()<<std::endl;
    // std::cout<<"name size:"<<this->user->get_name().size()<<std::endl;
    // std::printf("%d%d", param[5],param[6]);
    if(param.compare(this->user->get_name()) == 0){
        this->user->send_control_msg(construct_ret(331, "need password."));
    }else{
        this->user->send_control_msg(construct_ret(530, "user not found!"));
    }
    return 0;
}

std::string Command::construct_ret(int statcode, std::string descript){
    return std::to_string(statcode) + " " + descript + "\n";
}

ACE_INET_Addr Command::port_string_to_INET(std::string port_addr){
    int last_comma = port_addr.find_last_of(',');
    int second_port = std::stoi(port_addr.substr(last_comma + 1));
    std::cout<<"second port:"<<second_port<<std::endl;
    port_addr = port_addr.substr(0, last_comma);
    int port_comma = port_addr.find_last_of(',');
    int first_port = std::stoi(port_addr.substr(port_comma + 1));
    std::cout<<"first port:"<<first_port<<std::endl;
    int port = first_port * 256 + second_port;
    std::cout<<"port:"<<port<<std::endl;
    std::string ip = port_addr.substr(0, port_comma);
    std::cout<<"ip:"<<ip<<std::endl;
    return ACE_INET_Addr(port);
}