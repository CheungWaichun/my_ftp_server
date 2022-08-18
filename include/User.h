
#ifndef USER_H
#define USER_H

#include <string>



class User{
public:
    User();
    ~User();




private:
    std::string name;
    std::string password;
    enum {WAIT_NAME, WAIT_PASSWORD, LOGINED} login_stat;



};












#endif