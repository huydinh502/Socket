#include "Header/Client.h"

int Client::Connect(int portnum, const char* addr)
{
    m_clientaddr.sin_family = AF_INET; 
    m_clientaddr.sin_port = htons(portnum); 
    m_clientaddr.sin_addr.s_addr = inet_addr(addr);
    socklen_t len = sizeof (struct sockaddr_in);
    return connect(m_idSock,(struct sockaddr*)&m_clientaddr,len);
}

int Client::SendMessage(string message)
{
    struct pollfd fds;
    fds.fd = m_idSock; // add id cua file fd can theo doi
    fds.events = POLLOUT; // co the ghi du lieu vao
    int ret = poll(&fds, 1, -1);// block khong gioi han
    return send(m_idSock,message.c_str(), message.size(), 0);
}

int Client::RecieveMessage()
{
    struct pollfd fds;
    fds.fd = m_idSock; // add id cua file fd can theo doi
    fds.events = POLLIN; // doc du lieu
    int ret = poll(&fds,1,-1);
    if(ret==-1) return -1;
    memset(m_buff,0,1024);
    return recv(m_idSock,m_buff,1024,0);
}

void Client::SendFile(FILE *pFile)
{
    fseek(pFile,0, SEEK_END);
    size_t lenfile = ftell(pFile);
    rewind(pFile); //fseek(pFile,0, 0);                            

    struct pollfd fds;
    fds.fd = m_idSock; // add id cua file fd can theo doi
    fds.events = POLLOUT; // co the ghi du lieu vao
    int n = 0;
    while(true)   
    {
        int r = fread(m_buff,1,1024,pFile); // doc lan luot 1024 byte
        n+=r;
        int ret = poll(&fds, 1, -1);//                      
        int n_sent = send(m_idSock,m_buff,strlen(m_buff),0); 
        memset(m_buff,0, sizeof(m_buff));
        if(n==lenfile) break;
    }  
    fclose(pFile);   
}

void Client::RecieveFile(FILE* pDown)
{
    rewind(pDown);
    struct pollfd fds;
    fds.fd = m_idSock; // add id cua file fd can theo doi
    fds.events = POLLIN; // co the doc du lieu
    while(true)
    {   
        int ret = poll(&fds,1,1000); // cho de dc read
        if(ret ==-1)
        {
            cout << "Error recieve\n";
            break;
        }
        else if(ret == 0)
        {
            fclose(pDown);
            break;
        }
        else
        {
            int r = recv(m_idSock,m_buff,1024,0);
            fputs(m_buff,pDown);
            memset(m_buff,0, sizeof(m_buff));
        }

    }      
}