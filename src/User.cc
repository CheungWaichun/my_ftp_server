
#include "../include/User.h"

User::User(){
    name = "admin";
    password = "123456";
    login_stat = WAIT_NAME;
    trans_type = ASCII;
    current_dir = get_current_dir_name();
    // std::cout<<current_dir<<std::endl;
    passive = false;
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
    // positive
    this->connector.connect(data_stream, client_data_conn_addr);
    std::cout<<"data_stream built."<<std::endl;
    return 0;
}