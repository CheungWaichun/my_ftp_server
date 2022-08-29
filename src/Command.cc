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

        case RMD:
            this->cmd_rmd(param);
            break;

        case PASV:
            this->cmd_pasv();
            break;


        default:
            std::cout<<"default case."<<std::endl;
            break;
    }


    return;
}

int Command::cmd_cwd(std::string& param){
    std::string dir = this->get_formal_path(param);
    if(dir.empty() || dir.back() != '/'){
        this->user->send_control_msg(construct_ret(501, "not Exist."));
        return -1;
    }

    ACE_OS::chdir(dir.c_str());
    this->user->send_control_msg(construct_ret(250, "dir changed to " + dir));
    // erase '/' at tail
    dir.erase(dir.end() - 1);
    this->user->set_currrent_dir(dir);
    return 0;
}

int Command::cmd_dele(std::string& param){
    std::string path = this->get_formal_path(param);
    if(path.empty()){
        this->user->send_control_msg(construct_ret(550, "file not found."));
        return -1;
    }
    if(path.back() == '/'){
        this->user->send_control_msg(construct_ret(550, "not a file."));
        return -1;
    }
    std::string cmd = "rm -f " + path;
    std::cout<<"command is:"<<cmd<<std::endl;
    ACE_OS::system(cmd.c_str());
    this->user->send_control_msg(construct_ret(250, "file deleted."));
    return 0;
}

int Command::cmd_list(std::string param){
    std::string path = get_formal_path(param);
    if(!param.empty() && path.empty()){
        std::cout<<"path not exist."<<std::endl;
        this->user->send_control_msg(construct_ret(550, "path not exist."));
        return -1;
    }
    FILE *fp;
    std::string cmd = "ls -a " + path;
    std::cout<<"command is: "<<cmd<<std::endl;
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

int Command::cmd_pasv(){
    this->user->set_passive(true);

    // choose a random port to listen to data connection
    ACE_OS::srand(time(NULL));
    int data_port = (ACE_OS::rand() % 10000) + 10000;
    
    // get host ip
    std::string cmd = "hostname -I";
    FILE* fp;
    std::string output = "";
    if((fp = popen(cmd.c_str(), "r")) == NULL){
        std::cout << "command line error" << std::endl;
    }else{
        char row[1024];
        while(ACE_OS::fgets(row, sizeof(row), fp)){
            output += row;
        }
    }
    pclose(fp);
    
    // erase '\n' and '\r'
    while(!isdigit(output.back())){
        output.erase(output.end() - 1);
    }
    std::cout<<"output is:"<<output<<std::endl;

    std::string local_ip = output;
    std::string full_addr = local_ip + ":" + std::to_string(data_port);
    std::cout<<"full_addr is: "<<full_addr<<std::endl;
    ACE_INET_Addr addr = (ACE_INET_Addr)full_addr.c_str();
    this->user->set_local_data_addr(addr);
    this->user->open_data_acceptor(this->user->get_local_data_addr());

    // replace '.' to ','
    int index = -1;
    while((index = local_ip.find('.')) != -1){
        local_ip[index] = ',';
    }
    std::string port1 = std::to_string(data_port / 256);
    std::string port2 = std::to_string(data_port % 256);
    std::string msg = "Entering Passive Mode (" + local_ip + "," + port1 + "," + port2 + ").";
    std::cout<<"msg is:"<<msg<<std::endl;
    user->send_control_msg(construct_ret(227, msg));
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

int Command::cmd_rmd(std::string& param){
    std::string dir = get_formal_path(param);
    if(dir.empty()){
        user->send_control_msg(construct_ret(550, "dir not found."));
        return -1;
    }
    if(dir.back() != '/'){
        user->send_control_msg(construct_ret(550, "not a dir."));
        return -1;
    }
    std::string cmd = "rm -rf " + dir;
    std::cout<<"command is:"<<cmd<<std::endl;
    ACE_OS::system(cmd.c_str());
    user->send_control_msg(construct_ret(250, "dir removed."));
    return 0;
}

int Command::cmd_syst(std::string param){
    this->user->send_control_msg(construct_ret(215, "ubuntu."));
    return 0;
}

int Command::cmd_type(std::string param){
    if(param.compare("I") == 0){
        user->send_control_msg(construct_ret(220, "Type set to binary."));
    }else if(param.compare("A") == 0){
        user->send_control_msg(construct_ret(220, "Type set to binary."));
    }else{
        user->send_control_msg(construct_ret(500, "what's this type?"));
    }
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


std::string Command::get_formal_path(std::string& raw_path){
    std::string path = "";
    if(raw_path.empty()){
        return "";
    }
    if(raw_path[0] == '/'){
        path = raw_path;
    }else{
        path = this->user->get_current_dir() + "/" + raw_path;
    }
    std::cout<<"full path:"<<path<<std::endl;
    // erase '/' at the tail
    // while(path.back() == '/'){
    //     path.erase(path.end() - 1);
    // }
    std::cout<<"access"<<std::endl;
    // file of dir not exist, return ""
    if(ACE_OS::access(path.c_str(), 0) == -1){
        return "";
    }
    std::cout<<"opendir"<<std::endl;
    // file path (not simplified)
    if(ACE_OS::opendir(path.c_str()) == NULL){
        return path;
    }
    //simplify dir
    std::string usr_dir = this->user->get_current_dir();
    ACE_OS::chdir(path.c_str());
    char buf[128] = {0};
    ACE_OS::getcwd(buf, 128);
    path = buf;
    path.push_back('/');
    // restore 
    ACE_OS::chdir(usr_dir.c_str());
    std::cout<<"formal path is: "<<path<<std::endl;
    return path;
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