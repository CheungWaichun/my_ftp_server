
#include "../include/User.h"

User::User(){
    name = "admin";
    password = "123456";
    login_stat = WAIT_NAME;
    trans_type = ASCII;
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
    this->connector.connect(data_stream, client_data_conn_addr);
    return 0;
}