#ifndef _CLIENT_H
#define _CLIENT_H
#include "stdafx.h"
using namespace std;
class client
{
public:
    client(char* addr){servAddr = addr;};
    void Init();
    static void *ListenThread(void *ptr);
    static void *SendThread(void *ptr);
    userData GetUsrData();
    void SendData(userData& data,int sock);
    void GetNameFromServ();
    inline char* GetUname(){return uname;};
private:
    int sock;
    char uname[30] = {0};
    char* servAddr;
};
struct threadParam
{
    client *c;
    int sock;
};
#endif