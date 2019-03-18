#ifndef _STDAFX_H
#define _STDAFX_H
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <unistd.h>
using namespace std;
struct userData
{
    char srcUser[30];
    char dstUser[30];
    char data[2000];
    //unsigned short checksum;
};

#endif