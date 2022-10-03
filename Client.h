#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
using namespace std;
class Client
{
    int m_idSock;
    struct sockaddr_in m_clientaddr;
    char m_buff[1025];
    
public:
    string getMessage(){return string(m_buff);}

    Client()
    {
        memset(m_buff,0, sizeof(m_buff));
    }
    
    int CreateSock()
    {
        m_idSock = socket(AF_INET, SOCK_STREAM,0);
        return m_idSock;
    }

    int Connect(int portnum, const char* addr);
    
    int SendMessage(string message);
    
    int RecieveMessage();
    
    //m_buff used in here
    void SendFile(FILE *pFile);
    
    void RecieveFile(FILE* pDown);
    
};
#endif