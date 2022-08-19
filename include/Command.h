

#ifndef COMMAND_H
#define COMMAND_H

#include <iostream>
#include <unordered_map>
#include "User.h"


class Command{
public:
    Command(std::string, User*);
    ~Command();
    std::string handle();


private:
    std::string com;
    User* user;
    std::string ret;
    enum COM_TYPE {USER, PASS, LIST, SYST};
    std::unordered_map<std::string, COM_TYPE> com_map = {
        {"USER", USER},
        {"PASS", PASS},
        {"LIST", LIST},
        {"SYST", SYST}
    
    
    };

    int cmd_user(std::string);

    int cmd_pass(std::string);

    int cmd_list(std::string);

    int cmd_syst(std::string);

    std::string construct_ret(int, std::string);

};









#endif