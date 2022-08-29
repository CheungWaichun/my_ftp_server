

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
    enum COM_TYPE {USER=1, PASS, LIST, SYST, QUIT, PORT, 
                   TYPE, CWD, PWD, OPTS, NLST, DELE, RMD,
                   PASV};
    std::unordered_map<std::string, COM_TYPE> com_map = {
        {"USER", USER},
        {"PASS", PASS},
        {"LIST", LIST},
        {"SYST", SYST},
        {"QUIT", QUIT},
        {"PORT", PORT},
        {"TYPE", TYPE},
        {"CWD",  CWD},
        {"PWD",  PWD},
        {"OPTS", OPTS},
        {"NLST", NLST},
        {"DELE", DELE},
        {"RMD" , RMD} ,
        {"PASV", PASV}
    };

    int cmd_cwd(std::string&);

    int cmd_dele(std::string&);

    int cmd_list(std::string);

    int cmd_nlst(std::string);

    int cmd_opts(std::string);

    int cmd_pass(std::string);

    int cmd_pasv();

    int cmd_port(std::string);

    int cmd_pwd();

    int cmd_quit();

    int cmd_rmd(std::string&);

    int cmd_syst(std::string);

    int cmd_type(std::string);

    int cmd_user(std::string);

    std::string construct_ret(int, std::string);

    // std::string get_file_dir(std::string&);

    std::string get_formal_path(std::string&);

};









#endif