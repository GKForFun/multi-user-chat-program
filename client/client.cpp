#include "client.h"
using namespace std;
#define PORT 27000
void client::GetNameFromServ()
{
    while(1)
    {
        char buffer[1024] = {0};
        read( sock , buffer, 1024);
        cout << buffer <<endl;
        cin >> uname;
        send(sock , uname , strlen(uname) , 0 );
        read(sock,buffer,1024);
        if(strcmp(buffer,"OK!")==0)
        {
            break;
        }
    }
}
void *client::ListenThread(void *ptr)
{
    threadParam *data = (threadParam*)ptr;
    int ret = 0;
    char buff[2500] = {0};
    cout << "start listenthread"<<endl;
    while(1)
    {
        ret = read(data->sock,buff,sizeof(userData));
        if(ret > 0)
        {
            userData *nData = new userData; 
            strncpy(nData->srcUser,buff,30);
            strncpy(nData->dstUser,(buff+30),30);
            strncpy(nData->data,(buff+60),2000);
            nData->srcUser[29] ='\0';
            nData->dstUser[29] ='\0';
            nData->data[1999] ='\0';
            cout << nData->srcUser << ":" <<endl;
            cout << "   " << nData->data <<endl; 
            free(nData);
            memset(buff,'0',2500);
        }
    }
}

void *client::SendThread(void *ptr)
{
    threadParam *data = (threadParam*)ptr;
    int ret = 0;
    char sendbuf[2000] = {0};
    char dstUser[30] = {0};
    cout << "start sendthread" <<endl;
    while(1)
    {
        cout << "enter dstuser:";
        cin >> dstUser;
        cout << "enter data:";
        cin >> sendbuf;
        userData *t = new userData;
        strncpy(t->srcUser,data->c->GetUname(),30);
        strncpy(t->dstUser,dstUser,30);
        strncpy(t->data,sendbuf,strlen(sendbuf)+1);
        if((send(data->sock,(void*)t,sizeof(userData),0)<0))
        {
            cout << "send failed" <<endl;
        }
        memset(sendbuf,'0',2000);
        memset(dstUser,'0',30);
    }
}
void client::Init()
{
    pthread_t id[2];
    struct sockaddr_in addr;
    struct sockaddr_in serv_addr;
    if((sock = socket(AF_INET,SOCK_STREAM,0))<0)
    {
        cout << "socket creation error" <<endl;
        return;
    }
    memset(&serv_addr,'0',sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if(inet_pton(AF_INET,servAddr,&serv_addr.sin_addr)<=0)
    {
        cout << "invalid address" <<endl;
        return;
    }
    if(connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0)
    {
        cout << "connection failed" <<endl;
        return;
    }
    GetNameFromServ();
    int ret = 0;
    threadParam t;
    t.sock = sock;
    t.c = this;
    if((ret = pthread_create(&id[0],NULL,client::ListenThread,&t)))
    {
        cout << "listen thread create failed"<<endl;
        return;
    }
    if((ret = pthread_create(&id[1],NULL,client::SendThread,&t)))
    {
        cout << "listen thread create failed"<<endl;
        return;
    }
    pthread_join(id[0],NULL);
    pthread_join(id[1],NULL);
}