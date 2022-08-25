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

    //erase CR and LF 
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

        case CWD:
            this->cmd_cwd(param);
            break;

        case PWD:
            this->cmd_pwd();
            break;
        
        case OPTS:
            this->cmd_opts(param);
            break;

        case NLST:
            this->cmd_nlst(param);
            break;

        case DELE:
            this->cmd_dele(param);
            break;

        default:
            std::cout<<"default case."<<std::endl;
            break;
    }


    return;
}

int Command::cmd_cwd(std::string& param){
    std::string dir = this->get_formal_dir(param);
    if(dir.empty()){
        this->user->send_control_msg(construct_ret(501, "not Exist."));
        return -1;
    }

    this->user->send_control_msg(construct_ret(250, "dir changed to " + dir));
    this->user->set_currrent_dir(dir);
    return 0;
}

int Command::cmd_dele(std::string& param){
    std::string dir = this->get_file_dir(param);
    if(dir.empty()){
        this->user->send_control_msg(construct_ret(550, "file not found."));
        return -1;
    }
    std::string cmd = "rm -f " + dir;
    ACE_OS::system(cmd.c_str());
    this->user->send_control_msg(construct_ret(250, "file deleted."));
    return 0;
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
            // std::printf("the last two:%d,%d",data[data.length() - 2],data[data.length() - 1]);
            // data = data.substr(0,data.length() - 1);
            // data += "\r\n";


            // this->user->send_data_msg_buf(row, 1024);
            // ACE_OS::memset(row, 0, sizeof(row));
        }
    }
    pclose(fp);

    // data += '\n';
    // data += '\r';

    this->user->send_control_msg(construct_ret(125, "ready!"));
    this->user->send_data_msg(data);
    std::cout<<"after send data"<<std::endl;
    this->user->close_data_stream();
    this->user->send_control_msg(construct_ret(250, "completed!"));

    return 0;
}

int Command::cmd_nlst(std::string param){
    return this->cmd_list(param);
}

int Command::cmd_opts(std::string param){
    this->user->send_control_msg(construct_ret(200, "opts success."));
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
    char buf[1024] = {0};
    addr.addr_to_string(buf, 1024);
    std::cout<<"addr is:" <<buf<<std::endl;
    this->user->set_client_data_addr(addr);
    this->user->init_data_stream();
    this->user->send_control_msg(construct_ret(200, "PORT set."));
    return 0;
}

int Command::cmd_pwd(){
    char buf[128] = {0};
    ACE_OS::getcwd(buf, 128);
    std::string dir = buf;
    this->user->send_control_msg(construct_ret(257, "current dir: " + dir));
    this->user->set_currrent_dir(dir);
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
    this->user->send_control_msg(construct_ret(200, "Type set to xxx."));
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

std::string Command::get_file_dir(std::string& fd){

}

std::string Command::get_formal_dir(std::string& raw_dir){
    std::string dir = "";

    if(raw_dir.empty()){
        dir = "/home";
    }
    else if(raw_dir[0] == '/'){
        dir = raw_dir;
    }
    else{
        dir = this->user->get_current_dir() + "/" + raw_dir;
    }

    std::string usr_dir = this->user->get_current_dir();
    if(ACE_OS::chdir(dir.c_str()) != 0){
        return "";
    }
    //simplify dir
    char buf[128] = {0};
    ACE_OS::getcwd(buf, 128);
    dir = buf;
    // restore 
    ACE_OS::chdir(usr_dir.c_str());
    return dir;
}

ACE_INET_Addr Command::port_string_to_INET(std::string port_addr){
    int last_comma = port_addr.find_last_of(',');
    int second_port = std::stoi(port_addr.substr(last_comma + 1));
    // std::cout<<"second port:"<<second_port<<std::endl;
    port_addr = port_addr.substr(0, last_comma);
    int port_comma = port_addr.find_last_of(',');
    int first_port = std::stoi(port_addr.substr(port_comma + 1));
    // std::cout<<"first port:"<<first_port<<std::endl;
    int port = first_port * 256 + second_port;
    // std::cout<<"port:"<<port<<std::endl;
    std::string ip = port_addr.substr(0, port_comma);
    // std::cout<<"ip:"<<ip<<std::endl;
    // get format ip address
    int pos = -1;
    while((pos = ip.find(',')) != -1){
        ip.replace(pos, 1, ".");
    }
    // std::cout<<"format ip:"<<ip<<std::endl;
    std::string full_addr = ip + ":" + std::to_string(port);

    // std::cout<<"ip is:"<<addr.get_ip_address()<<std::endl;
    // std::cout<<"port is:"<<addr.get_port_number()<<std::endl;
    return (ACE_INET_Addr)full_addr.c_str();
}