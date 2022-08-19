
#ifndef USER_H
#define USER_H

#include <string>



class User{
public:
    User();
    ~User();
    std::string get_name();
    std::string get_password();




private:
    std::string name;
    std::string password;
    enum {WAIT_NAME, WAIT_PASSWORD, LOGINED} login_stat;



};












#endif