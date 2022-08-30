
#include "../include/User.h"

User::User(){
    name = "admin";
    password = "123456";
    login_stat = WAIT_NAME;
    trans_type = ASCII;
    current_dir = get_current_dir_name();
    // std::cout<<current_dir<<std::endl;
    is_passive = false;
}

User::~User(){

}

std::string User::get_name(){
    return this->name;
}

std::string User::get_password(){
    return this->password;
}

int User::init_data_stream(){
    if(!is_passive){
        // positive
        this->connector.connect(data_stream, client_data_conn_addr);
        std::cout<<"data_stream built in positive mode."<<std::endl;
        return 0;

    }else{
        // passive
        data_acceptor.accept(data_stream);
        std::cout<<"data_stream built in passive mode."<<std::endl;
        data_stream.get_remote_addr(client_data_conn_addr);
        // data_acceptor.close();
        return 0;
    }
}

int User::send_data_msg(std::string msg){
    init_data_stream();
    if(trans_type == Image){
        // binary mode
        this->data_stream.send(msg.c_str(), msg.length());

    }else if(trans_type == ASCII){
        // ascii mode
        // all lines should end with <CRLF>

        std::string trans_msg = "";
        // replace '\n' with "\r\n"
        for(auto i = msg.begin(); i != msg.end(); i++){
            if(*i == '\n'){
                trans_msg += "\r\n";
            }else{
                trans_msg += *i;
            }
        }
        this->data_stream.send(trans_msg.c_str(), trans_msg.length());
    }
    return 0;
}

int User::send_data_msg_buf(char* buf, int size){
    init_data_stream();
    if(trans_type == Image){
        // binary mode
        this->data_stream.send(buf, size);

    }else if(trans_type == ASCII){
        // ascii mode
        // all lines should end with <CRLF>

        char trans_buf[size * 2] = {0};
        int j = 0;
        // replace '\n' with "\r\n"
        for(int i = 0; i < size; i++){
            if(buf[i] == '\n'){
                trans_buf[j++] = '\r';
                trans_buf[j++] = '\n';
            }else{
                trans_buf[j++] = buf[i];
            }
        }
        this->data_stream.send(trans_buf, j);
    }
    return 0;
}

int User::recv_data_msg_buf(char* buf, int size){
    int recv_size = 0;
    char* head = buf;
    int total = 0;
    while((recv_size = data_stream.recv(buf, size)) > 0){
        buf += recv_size;
        size -= recv_size;
        total += recv_size;
        if(size <= 0){
            std::cout<<"buf too small !!"<<std::endl;
            break;
        }
    }
    return total;

}