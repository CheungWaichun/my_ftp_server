
#ifndef USER_H
#define USER_H

#include <string>
#include "ace/INET_Addr.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Acceptor.h"
#include <iostream>
#include "ace/OS.h"

class User{
public:
    User();
    ~User();

    std::string get_name();
    std::string get_password();
    // std::string get_root_dir(){
    //     return this->root_dir;
    // }
    std::string get_current_dir(){
        return this->current_dir;
    }

    ACE_INET_Addr& get_local_data_addr(){
        return this->local_data_conn_addr;
    }

    void set_currrent_dir(std::string& dir){
        this->current_dir = dir;
    }

    void set_client_data_addr(ACE_INET_Addr& addr){
        this->client_data_conn_addr = addr;
    }

    void set_client_control_addr(ACE_INET_Addr& addr){
        this->client_control_conn_addr = addr;
    }

    void set_local_data_addr(ACE_INET_Addr& addr){
        this->local_data_conn_addr = addr;
    }

    void set_passive(bool tf){
        this->passive = tf;
    }



    int init_data_stream();

    int set_data_stream(ACE_SOCK_Stream& stream){
        this->data_stream = stream;
        return 0;
    }

    int set_control_stream(ACE_SOCK_Stream& stream){
        this->control_stream = stream;
        return 0;
    }

    int send_control_msg(std::string msg){
        this->control_stream.send(msg.c_str(), msg.length());
        return 0;
    }

    int send_data_msg(std::string msg){
        this->data_stream.send(msg.c_str(), msg.length());
        return 0;
    }

    int send_data_msg_buf(char* buff, int size){
        this->data_stream.send(buff, size);
        return 0;
    }

    int close_data_stream(){
        this->data_stream.close();
        return 0;
    }

    int open_data_acceptor(ACE_INET_Addr& addr){
        return this->data_acceptor.open(addr, 1);
    }

private:
    std::string name;
    std::string password;
    // std::string root_dir;
    std::string current_dir;
    enum STAT{WAIT_NAME, WAIT_PASSWORD, LOGINED} login_stat;
    enum TRANS_TYPE{BINARY, ASCII} trans_type;//transfer type
    // passive mode
    bool passive;

    ACE_INET_Addr client_data_conn_addr;
    ACE_SOCK_Stream data_stream;
    ACE_INET_Addr client_control_conn_addr;
    ACE_SOCK_Stream control_stream;
    ACE_INET_Addr local_data_conn_addr;

    ACE_SOCK_Connector connector;
    ACE_SOCK_Acceptor data_acceptor;
};












#endif