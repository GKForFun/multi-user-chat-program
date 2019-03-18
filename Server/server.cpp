#include "server.h"
using namespace std;


void Server::AddConn(pair<string,int> a)
{
    curConn.push_back(a);
}

bool Server::DelConn(string name)
{
    int pos = FindConn(name);
    if(pos ==-1)
    {
        return false;
    }
    curConn.erase(curConn.begin()+pos);
    return true;
}

int Server::FindConn(string name)
{
    for(vector<pair<string,int>>::iterator iter = curConn.begin();iter!=curConn.end();)
    {
        if(iter->first != name)
        {
            iter++;
        }
        else
        {
            return (iter-curConn.begin());
        }
    }
    return -1;
}

int Server::GetSock(string name)
{
    for(vector<pair<string,int>>::iterator iter = curConn.begin();iter!=curConn.end();)
    {
        if(iter->first != name)
        {
            iter++;
        }
        else
        {
            return iter->second;
        }
    }
    return -1;
}

string Server::GetNuname(int sock)
{
    char buff[2000];
    memset((void*)buff,0,1999);
    while(true)
    {
        if(send(sock,"enter user name:",sizeof("enter user name:"),0) == 0)
        {
            cout << "send failed" <<endl;
        }
        int n = recv(sock,buff,1999,0);
        buff[n] = '\0';
        string name = buff;
        if(FindConn(name)==-1)
        {
            send(sock,"OK!",sizeof("OK!"),0);
            return name;
        }
        memset((void*)buff,0,1999);
        if(send(sock,"the name has been used,enter new name:",sizeof("the name has been used,enter new name:"),0) == 0)
        {
            cout << "send failed" <<endl;
        }
    }
}

userData Server::GetUsrData(pair<string,int> src)
{
    userData uData;
    char buff[2500];
    memset((void*)buff,0,2500);
    int n = recv(src.second,buff,sizeof(userData),0);
    if(n == 0)
    {
        pthread_exit(0);
    }
    cout << n <<endl;
    buff[n] = '\0';
    strncpy(uData.srcUser,buff,30);
    uData.srcUser[29] = '\0';
    cout << uData.srcUser << endl;
    strncpy(uData.dstUser,(buff+30),30);
    uData.dstUser[29] = '\0';
    cout << uData.dstUser <<endl;
    strncpy(uData.data,(buff+60),2000);
    uData.data[1999] = '\0';
    cout << uData.data << endl;
    return uData;
}

void Server::SendData(userData& data,int sock)
{
    int n = send(sock,(void*)&data,2060,0);
    if(n == 0)
    {
        cout << "send to " << data.dstUser << "failed" <<endl;
        return ;
    }
    return ;
}

unsigned short Server::check_sum(unsigned short *a, int len)  
{  
    unsigned int sum = 0;  
    while (len > 1) {  
        sum += *a++;  
        len -= 2;  
    }
    if (len) {  
        sum += *(unsigned char *)a;  
    }  
    while (sum >> 16) {  
        sum = (sum >> 16) + (sum & 0xffff);  
    }  
    return (unsigned short)(~sum);
} 

void* Server::ClientThread(void* args)
{
    threadParam* cParam = (threadParam*)args;
    string name = cParam->serv->GetNuname(cParam->sock);
    cout << "uanme:"<< name << endl;
    pair<string,int> t;
    t.first = name;
    t.second = cParam->sock;
    cParam->serv->AddConn(t);
    userData data;
    while(true)
    {
        data = cParam->serv->GetUsrData(t);
        int pos = cParam->serv->FindConn(data.dstUser);
        if(pos != -1)
        {
            cParam->serv->SendData(data,cParam->serv->GetSock(data.dstUser));
        }
    }
    close(cParam->sock);
}

void* Server::ListenThread( void* args )
{
      threadParam* param = (threadParam*)args;
      int sockfd;
      struct sockaddr_in sAddr;
      struct sockaddr_in cAddr;
      if((sockfd = socket(AF_INET,SOCK_STREAM,0))==-1)
      {
          cout << "socket() error" <<endl;
          exit(1);
      }
      sAddr.sin_family = AF_INET;
      sAddr.sin_port = htons(CONPORT);
      sAddr.sin_addr.s_addr = INADDR_ANY;
      if(bind(sockfd,(struct sockaddr*)&sAddr,sizeof(struct sockaddr))==-1)
      {
          cout << "bind() error" <<endl;
          exit(1);
      }
      listen(sockfd,SOMAXCONN);
      cout << "start listen " <<endl;
      while(1)
      {
          int clientfd;
          int sin_size = sizeof(struct sockaddr_in);
          if((clientfd = accept(sockfd,(sockaddr*)&cAddr,(socklen_t*)&sin_size))==-1)
          {
              cout << "accpet() error" <<endl;
          }
          pthread_t id;
          threadParam cParam;
          cParam.serv = param->serv;
          cParam.sock = clientfd;
          int ret = pthread_create(&id,NULL,ClientThread,(void*)&cParam);
      }
}


void Server::Init()
{
    pthread_t id;
    threadParam param;
    param.serv = this;
    param.sock = 0;
    int ret = pthread_create(&id,NULL,ListenThread,(void*)&param);
    if(ret)
    {
        cout << "create listen thread error" <<endl;
    }
    pthread_join(id,NULL);
}