#ifndef _SERVER_H
#define _SERVER_H
#include "stdafx.h"
#define CONPORT 27000
#define BACKLOG 10
using namespace std;
class Server
{
public:
// main function
    void Init();
    static void *ListenThread(void *ptr);
    static void *ClientThread(void *ptr);
    void SaveOfflineMsg(string dstip,string data);
    string FindOfflineMsg(string ip);
// assistant function
    int FindConn(string name);
    int GetSock(string name);
    void AddConn(pair<string,int> a);
    bool DelConn(string name);
    string GetNuname(int sock);
    userData GetUsrData(pair<string,int> src);
    void SendData(userData& data,int sock);
    unsigned short check_sum(unsigned short *a, int len);
    vector<pair<string,int>> curConn;
private:
    pthread_mutex_t mutex;
};
struct threadParam
{
    Server* serv;
    int sock;
};
#endif