#include "ace/INET_Addr.h"
#include <iostream>
using namespace std;

int main(){
    ACE_INET_Addr addr;
    ACE_INET_Addr addr2 = (ACE_INET_Addr)"127.0.0.1:8888";
    cout<<addr.get_host_addr()<<endl;
    cout<<addr2.get_host_addr()<<endl;
    cout<<addr.get_ip_address()<<endl;
    cout<<addr2.get_ip_address()<<endl;
    cout<<addr.get_host_name()<<endl;
    cout<<addr2.get_host_name()<<endl;

    return 0;
}