#include "Header/Server.h"

Server::Server()
{
     memset(m_message,0,sizeof(m_message));
}

int Server::CreateSock()
{
    m_idSock = socket(AF_INET, SOCK_STREAM,0);
    return m_idSock;
}

int Server::SetSockResuse()
{
    int i = 1;
    return setsockopt(m_idSock,SOL_SOCKET,SO_REUSEADDR,&i,sizeof(int));
}

int Server::Bind(const char* port)
{   m_serveraddr.sin_family = AF_INET;
    m_serveraddr.sin_port = htons(atoi(port));
    m_serveraddr.sin_addr.s_addr = INADDR_ANY;
    return bind(m_idSock,(struct sockaddr*)&m_serveraddr,sizeof(m_serveraddr));
}
int Server::Listen(int n)
{
    return listen(m_idSock,n);
}
int Server::Accept()
{
    return accept(m_idSock,(struct sockaddr*)NULL,NULL);
}

bool Server::RecieveMessage(int id)
{
    int nrecv = recv(id,m_message,1024,0);
    if(nrecv == -1)
    {
        return false;
    }
    else if(nrecv!=0)
    {
        char* mess[3];
        int len = strlen(m_message);
        mess[0] = m_message;
        int n=1;
        for(int i=0;i<len;i++)
        {
            if(m_message[i]==' ')
            {
                m_message[i]= 0;
                mess[n] = &m_message[i+1];
                n+=1;
            }
        } 
        m_command = string(mess[0]);
        if(m_command.size()==7)
        {
            m_filename = string(mess[1]);
            m_key=m_value="";
        }
        else if(m_command!="CLOSE") 
        {
            m_key = string(mess[1]);
            if(m_command=="PUT") //PUT ma khong co key
            {
                m_value = string(mess[2]);
            }
            else m_value = "";
            m_filename ="";
        }
        
        memset(m_message,0, sizeof(m_message));
        return true;
    }  
}

bool Server::ReadIp(int id)
{
    struct sockaddr_in addr;
    socklen_t addr_size = sizeof(struct sockaddr_in);
    int res = getpeername(id, (struct sockaddr *)&addr, &addr_size);
    if(res==-1) 
        return false;
    else {
        m_ip = string(inet_ntoa(addr.sin_addr));
        return true;
    }
}