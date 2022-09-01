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

        case RETR:
            this->cmd_retr(param);
            break;

        case STOR:
            this->cmd_stor(param);
            break;

        case CDUP:
            this->cmd_cdup();
            break;

        case REIN:
            this->cmd_rein();
            break;

        case MKD:
            this->cmd_mkd(param);
            break;

        case RNFR:
            this->cmd_rnfr(param);
            break;
        
        case RNTO:
            this->cmd_rnto(param);
            break;

        default:
            std::cout<<"default case."<<std::endl;
            user->send_control_msg(500, "can't understand.");
            break;
    }


    return;
}

int Command::cmd_cdup(){
    std::string dir = "../";
    return this->cmd_cwd(dir);
}

int Command::cmd_cwd(std::string& param){
    std::string dir = this->get_formal_path(param);
    if(dir.compare("-1") == 0){
        user->send_control_msg(501, "not Exist.");
        return -1;
    }
    if(dir.back() != '/'){
        user->send_control_msg(553, "not a directory.");
        return -1;
    }
    if(dir.empty()){
        dir = "/home";
    }

    ACE_OS::chdir(dir.c_str());
    this->user->send_control_msg(250, "dir changed to " + dir);
    // erase '/' at tail
    dir.erase(dir.end() - 1);
    this->user->set_currrent_dir(dir);
    return 0;
}

int Command::cmd_dele(std::string& param){
    std::string path = this->get_formal_path(param);
    if(path.empty()){
        this->user->send_control_msg(550, "file not found.");
        return -1;
    }
    if(path.back() == '/'){
        this->user->send_control_msg(550, "not a file.");
        return -1;
    }
    std::string cmd = "rm -f " + path;
    std::cout<<"command is:"<<cmd<<std::endl;
    ACE_OS::system(cmd.c_str());
    this->user->send_control_msg(250, "file deleted.");
    return 0;
}

int Command::cmd_list(std::string param){
    std::string path = get_formal_path(param);
    if(path.compare("-1") == 0){
        std::cout<<"path not exist."<<std::endl;
        this->user->send_control_msg(550, "path not exist.");
        return -1;
    }
    FILE *fp;
    std::string cmd = "ls -alp " + path;
    std::cout<<"command is: "<<cmd<<std::endl;
    std::string data = "";
    if((fp = popen(cmd.c_str(), "r")) == NULL){
        std::cout << "error" << std::endl;
    }else{
        char row[1024];
        while(ACE_OS::fgets(row, sizeof(row), fp)){
            data += row;
        }
    }
    pclose(fp);

    this->user->send_control_msg(125, "ready!");
    this->user->send_data_msg(data);
    std::cout<<"after send data"<<std::endl;
    this->user->send_control_msg(250, "completed!");

    return 0;
}

int Command::cmd_mkd(std::string param){
    std::string dir = "";
    if(param.empty()){
        dir = this->user->get_current_dir();
    }else if(param[0] == '/'){
        dir = param;
    }else{
        dir = this->user->get_current_dir() + '/' + param;
    }

    if(ACE_OS::mkdir(dir.c_str()) == -1){
        user->send_control_msg(550, "mkdir error.");
        return -1;
    }
    user->send_control_msg(257, "mkdir success.");
    return 0;

}

int Command::cmd_nlst(std::string param){
    std::string path = get_formal_path(param);
    if(path.compare("-1") == 0){
        std::cout<<"path not exist."<<std::endl;
        this->user->send_control_msg(550, "path not exist.");
        return -1;
    }
    FILE *fp;
    // std::string cmd = "ls -ap " + path + " | grep -v '/'";
    std::string cmd = "ls -ap " + path;
    std::cout<<"command is: "<<cmd<<std::endl;
    std::string data = "";
    if((fp = popen(cmd.c_str(), "r")) == NULL){
        std::cout << "popen error" << std::endl;
    }else{
        char row[1024];
        while(ACE_OS::fgets(row, sizeof(row), fp)){
            data += row;
        }
    }
    pclose(fp);

    this->user->send_control_msg(125, "ready!");
    this->user->send_data_msg(data);
    std::cout<<"after send data"<<std::endl;
    this->user->send_control_msg(250, "completed!");

    return 0;
}

int Command::cmd_noop(){
    user->send_control_msg(220, "I am here.");
    return 0;
}

int Command::cmd_opts(std::string param){
    this->user->send_control_msg(200, "opts success.");
    return 0;
}

int Command::cmd_pass(std::string param){
    std::cout<<"pass param is: "<< param<<std::endl;
    if(!param.compare(this->user->get_password())){
        this->user->send_control_msg(230, "login success.");
    }else{
        this->user->send_control_msg(530, "wrong password!");
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
    user->send_control_msg(227, msg);
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

    this->user->set_passive(false);
    this->user->send_control_msg(200, "PORT set.");
    return 0;
}

int Command::cmd_pwd(){
    char buf[128] = {0};
    ACE_OS::getcwd(buf, 128);
    std::string dir = buf;
    this->user->send_control_msg(257, "current dir: " + dir);
    this->user->set_currrent_dir(dir);
    return 0;
}

int Command::cmd_quit(){
    this->user->send_control_msg(221, "byebye.");
    ACE_OS::exit(0);
    return 0;
}

int Command::cmd_rein(){
    user->clear();
    user->send_control_msg(220, "rein success.");
    return 0;
}

// read file logic implemented here, not in class User
int Command::cmd_retr(std::string& param){
    ACE_FILE_Connector file_connector;
    ACE_FILE_Addr file_addr;
    ACE_FILE_IO file_io;

    std::string path = get_formal_path(param);
    if(path.empty()){
        std::cout<<"file not found."<<std::endl;
        this->user->send_control_msg(550, "file not found.");
        return -1;
    }
    if(path.back() == '/'){
        this->user->send_control_msg(550, "not a file.");
        return -1;
    }

    file_addr.set(path.c_str());

    int file_mode = O_RDONLY;
    if(this->user->get_trans_type() == 'I'){
        file_mode |= O_BINARY;
    }

    file_connector.connect(file_io, file_addr, 0, ACE_Addr::sap_any, 0, file_mode);

    user->send_control_msg(125, "send begin.");
    user->send_data_msg_file(file_io);
    std::cout<<"file sent."<<std::endl;
    user->send_control_msg(250, "send done.");

    return 0;
}

int Command::cmd_rmd(std::string& param){
    std::string dir = get_formal_path(param);
    if(dir.compare("-1") == 0){
        user->send_control_msg(550, "dir not found.");
        return -1;
    }
    if(dir.empty()){
        user->send_control_msg(501, "dir can't be empty.");
        return -1;
    }
    if(dir.back() != '/'){
        user->send_control_msg(550, "not a dir.");
        return -1;
    }
    std::string cmd = "rm -rf " + dir;
    std::cout<<"command is:"<<cmd<<std::endl;
    ACE_OS::system(cmd.c_str());
    user->send_control_msg(250, "dir removed.");
    return 0;
}

int Command::cmd_rnfr(std::string param){
    // both dir and file can be renamed
    std::string path = get_formal_path(param);
    if(path.compare("-1") == 0){
        user->send_control_msg(550, "file not exist.");
        return -1;
    }
    if(path.empty()){
        user->send_control_msg(501, "file name can't be empty.");
        return -1;
    }
    user->set_dir_to_be_renamed(path);
    user->send_control_msg(350, "waiting for RNTO command.");
    return 0;
}

int Command::cmd_rnto(std::string param){
    if(user->get_dir_to_be_renamed().empty()){
        user->send_control_msg(503, "need RNFR command first.");
        return -1;
    }
    std::string path = "";
    if(param.empty()){
        user->send_control_msg(501, "new file name can't be empty.");
        return -1;
    }else if(param[0] == '/'){
        path = param;
    }else{
        path = this->user->get_current_dir() + '/' + param;
    }

    if(ACE_OS::rename(user->get_dir_to_be_renamed().c_str(), 
                      path.c_str()) == -1)
    {
        user->send_control_msg(501, "rename failed.");
        return -1;
    }

    user->set_dir_to_be_renamed("");
    user->send_control_msg(250, "rename success.");
    return 0;
}

int Command::cmd_stor(std::string& param){
    ACE_FILE_Connector file_connector;
    ACE_FILE_Addr file_addr;
    ACE_FILE_IO file_io;

    std::string path = "";
    // cant use get_formal_path() method
    if(param.empty()){
        path = this->user->get_current_dir();
    }else if(param[0] == '/'){
        path = param;
    }else{
        path = this->user->get_current_dir() + '/' + param;
    }

    if(path.back() == '/'){
        this->user->send_control_msg(550, "not a file. it's a directory.");
        return -1;
    }
    std::cout<<"path is: "<<path<<std::endl;
    file_addr.set(path.c_str());

    int file_mode = O_RDWR | O_TRUNC | O_CREAT;
    if(this->user->get_trans_type() == 'I'){
        file_mode |= O_BINARY;
    }

    if(-1 == file_connector.connect(file_io, file_addr, 0, ACE_Addr::sap_any, 0, file_mode)){
        user->send_control_msg(451, "local file error.");
        return -1;
    }

    user->send_control_msg(125, "recv begin.");
    user->recv_data_msg_file(file_io);
    std::cout<<"file sent."<<std::endl;
    user->send_control_msg(226, "recv done.");

    return 0;
}

int Command::cmd_syst(std::string param){
    this->user->send_control_msg(215, "ubuntu.");
    return 0;
}

int Command::cmd_type(std::string param){
    if(param.compare("A") == 0){
        this->user->set_trans_type('A');
        user->send_control_msg(200, "Type set to ASCII.");
    }else if(param.compare("I") == 0){
        this->user->set_trans_type('I');
        user->send_control_msg(200, "Type set to Image.");
    }else{
        user->send_control_msg(500, "what's this type?");
    }
    return 0;
}

int Command::cmd_user(std::string param){
    std::cout<<"user param is: "<< param<<std::endl;
    std::cout<<"param size :"<<param.size()<<std::endl;
    // std::cout<<"name size:"<<this->user->get_name().size()<<std::endl;
    // std::printf("%d%d", param[5],param[6]);
    if(param.compare(this->user->get_name()) == 0){
        this->user->send_control_msg(331, "need password.");
    }else{
        this->user->send_control_msg(530, "user not found!");
    }
    return 0;
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

    // file of dir not exist, return ""
    if(ACE_OS::access(path.c_str(), 0) == -1){
        return "-1";
    }

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

