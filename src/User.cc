
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

    if(trans_type == ASCII){
        msg = trim_to_CRLF((char*)msg.c_str(), msg.length());
    }
    std::cout<<"trim done."<<std::endl;
    this->data_stream.send(msg.c_str(), msg.length());
    close_data_stream();
    return 0;
}

int User::send_data_msg_buf(char* buf, int size){
    init_data_stream();
    if(trans_type == ASCII){
        std::string str = trim_to_CRLF(buf, size);
        size = str.length();
        buf = (char*)str.c_str();
    }
    this->data_stream.send(buf, size);
    close_data_stream();
    return 0;
}

int User::send_data_msg_file(ACE_FILE_IO& file_io){
    init_data_stream();
    char* buf = new char[128];
    int recv_size = 0;
    std::string str = "";
    while((recv_size = file_io.recv(buf, sizeof(buf))) > 0){
        if(trans_type == ASCII){
            str = trim_to_CRLF(buf, recv_size);
            buf = std::strncpy(buf, str.c_str(), str.length());
            recv_size = str.length();
        }
        data_stream.send(buf, recv_size);
    }
    delete[] buf;
    file_io.close();
    close_data_stream();
    return 0;
}


int User::recv_data_msg_buf(char* buf, int size){
    // no trim yet
    init_data_stream();
    int total_size = 0;
    int recv_size = 0;
    // std::printf("buf pointer is: %p\n", buf);
    while((recv_size = data_stream.recv(buf, size)) > 0){
        buf += recv_size;
        size -= recv_size;
        total_size += recv_size;
        std::cout<<"recv_size: "<<recv_size<<std::endl;
        if(size <= 0){
            std::cout<<"buf too small !!"<<std::endl;
            break;
        }
    }

    close_data_stream();
    // return size of recv data in buf
    return total_size;
}

int User::recv_data_msg_file(ACE_FILE_IO& file_io){
    init_data_stream();
    char* buf = new char[128];
    int data_size = 0;
    // used in ASCII case
    std::string str = "";
    while((data_size = data_stream.recv(buf, sizeof(buf))) > 0){
        if(trans_type == ASCII){
            str = trim_to_LF(buf, data_size);
            buf = strncpy(buf, str.c_str(), str.length());
            data_size = str.length();
        }
        std::cout<<"data_size is: "<<data_size<<std::endl;
        file_io.send(buf, data_size);
        std::cout<<"file written."<<std::endl;
    }
    delete[] buf;
    std::cout<<"delete complete."<<std::endl;
    file_io.close();
    std::cout<<"file closed."<<std::endl;
    close_data_stream();
    std::cout<<"data stream closed."<<std::endl;
    return 0;
}


void User::clear(){
    
    login_stat = WAIT_NAME;
    trans_type = ASCII;
    is_passive = false;
    current_dir.clear();
    
}


std::string User::trim_to_CRLF(char* buf, size_t n){
    std::string str(buf, n);
    int index = 0;
    while((index = str.find('\n', index)) != std::string::npos){
        str.replace(index, 1, "\r\n");
        // skip the new "\r\n"
        index += 2;
    }
    return str;
}

std::string User::trim_to_LF(char* buf, size_t n){
    std::string str(buf, n);
    int index = -1;
    while((index = str.find("\r\n")) != std::string::npos){
        str.replace(index, 2, "\n");
    }
    return str;
}