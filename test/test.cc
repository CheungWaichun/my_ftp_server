#include "ace/INET_Addr.h"
#include <iostream>
using namespace std;

int main(){
    // ACE_INET_Addr addr;
    // ACE_INET_Addr addr2 = (ACE_INET_Addr)"127.0.0.1:8888";
    // cout<<addr.get_host_addr()<<endl;
    // cout<<addr2.get_host_addr()<<endl;
    // cout<<addr.get_ip_address()<<endl;
    // cout<<addr2.get_ip_address()<<endl;
    // cout<<addr.get_host_name()<<endl;
    // cout<<addr2.get_host_name()<<endl;


    // char buf[128] = {0};
    // buf[0] = 'a';
    // buf[1] = 'c';
    // string str = buf;
    // str.push_back('b');
    // printf("%s\n", buf);
    // cout<<str<<endl;

    char* buf2 = new char[128];
    cout<<sizeof(buf2)<<endl;
    cout<<sizeof('a')<<endl;




    return 0;
}