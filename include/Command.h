

#ifndef COMMAND_H
#define COMMAND_H

#include <iostream>
#include <unordered_map>
#include "User.h"


class Command{
public:
    Command(std::string, User*);
    ~Command();
    std::pair<int, std::string> handle();


private:
    std::string com;
    User* user;
    std::string ret;
    int stat = 0;//waiting connection
    enum COM_TYPE {USER=1, PASS, LIST, SYST, QUIT};
    std::unordered_map<std::string, COM_TYPE> com_map = {
        {"USER", USER},
        {"PASS", PASS},
        {"LIST", LIST},
        {"SYST", SYST},
        {"QUIT", QUIT}
    
    
    };

    int cmd_user(std::string);

    int cmd_pass(std::string);

    int cmd_list(std::string);

    int cmd_syst(std::string);

    int cmd_quit();

    std::string construct_ret(int, std::string);

};









#endif