#include "client.h"
using namespace std;
int main(int argc,char ** argv)
{
    char *addr;
    if(argc!=2)
    {
        cout << "default addr:127.0.0.1"<<endl;
        cout << "usage client <ip>" <<endl;
        addr = "127.0.0.1";
    }
    else
    {
        addr = argv[1];
    }
    client c(addr);
    c.Init();
    return 0;
}