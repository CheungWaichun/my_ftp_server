

#ifndef COMMAND_H
#define COMMAND_H

#include <iostream>
#include <unordered_map>
#include "User.h"
#include "ace/OS.h"
#include "ace/INET_Addr.h"


class Command{
public:
    Command(std::string, User*);
    ~Command();
    void handle();

    ACE_INET_Addr port_string_to_INET(std::string);

private:
    std::string com;
    User* user;
    // std::string ret;
    // int stat = 0;//waiting connection
    enum COM_TYPE {USER=1, PASS, LIST, SYST, QUIT, PORT, TYPE};
    std::unordered_map<std::string, COM_TYPE> com_map = {
        {"USER", USER},
        {"PASS", PASS},
        {"LIST", LIST},
        {"SYST", SYST},
        {"QUIT", QUIT},
        {"PORT", PORT},
        {"TYPE", TYPE}
    
    };


    int cmd_list(std::string);

    int cmd_pass(std::string);

    int cmd_port(std::string);

    int cmd_quit();

    int cmd_syst(std::string);

    int cmd_type(std::string);

    int cmd_user(std::string);

    std::string construct_ret(int, std::string);

};









#endif