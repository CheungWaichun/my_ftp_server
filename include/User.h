/// \file User.h
/// \brief 记录用户的信息和状态
/// \author zhangweijun (zhangweijun@scutech.com)

#ifndef USER_H
#define USER_H

#include <cstring>
#include <string>
#include "ace/INET_Addr.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Acceptor.h"
#include <iostream>
#include "ace/OS.h"
#include "ace/FILE_IO.h"


class User{
private:

    enum STAT{WAIT_NAME, WAIT_PASSWORD, LOGINED};
    
    //transfer type
    enum TRANS_TYPE{Image, ASCII};

public:
    User();
    ~User();

    std::string get_name();
    std::string get_password();

    void set_passive(bool tf){
        this->is_passive = tf;
    }

    int get_login_stat(){
        return this->login_stat;
    }

    void set_login_stat(int stat){
        this->login_stat = STAT(stat);
    }

    char get_trans_type();

    void set_trans_type(TRANS_TYPE tp){
        this->trans_type = tp;
    }

    void set_trans_type(char tp);


    std::string get_current_dir(){
        return this->current_dir;
    }

    void set_currrent_dir(std::string dir){
        this->current_dir = dir;
    }

    void set_dir_to_be_renamed(std::string dir){
        this->dir_to_be_renamed = dir;
    }

    std::string get_dir_to_be_renamed(){
        return this->dir_to_be_renamed;
    }


    ACE_INET_Addr& get_local_data_addr(){
        return this->local_data_conn_addr;
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





    int init_data_stream();

    void set_data_stream(ACE_SOCK_Stream& stream){
        this->data_stream = stream;
    }

    void set_control_stream(ACE_SOCK_Stream& stream){
        this->control_stream = stream;
    }

    int send_control_msg(std::string msg){
        return control_stream.send(msg.c_str(), msg.length());
    }

    int send_control_msg(int, std::string);

    int send_data_msg(std::string msg);

    int send_data_msg_buf(char*, int);

    int send_data_msg_file(ACE_FILE_IO&);

    int recv_data_msg_buf(char*, int);

    int recv_data_msg_file(ACE_FILE_IO&);

    int close_data_stream(){
        return this->data_stream.close();
    }

    int open_data_acceptor(ACE_INET_Addr& addr){
        return this->data_acceptor.open(addr, 1);
    }

    // void set_file_io(ACE_FILE_IO& file_io){
    //     this->file_io = file_io;
    // }

    void clear();



private:
    std::string name;
    std::string password;

    STAT login_stat;
    TRANS_TYPE trans_type;
    // passive mode
    bool is_passive;

    std::string root_dir;
    std::string current_dir;
    std::string dir_to_be_renamed;

    ACE_INET_Addr client_data_conn_addr;
    ACE_SOCK_Stream data_stream;
    ACE_INET_Addr client_control_conn_addr;
    ACE_SOCK_Stream control_stream;
    ACE_INET_Addr local_data_conn_addr;

    ACE_SOCK_Connector connector;
    ACE_SOCK_Acceptor data_acceptor;

    // ACE_FILE_IO file_io;

    std::string trim_to_CRLF(char*, size_t);
    std::string trim_to_LF(char*, size_t);
};












#endif