#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string>
#include<string.h>
#include <poll.h> //poll
using namespace std;
class Server{
    int m_idSock;
    struct sockaddr_in m_serveraddr;
    char m_message[1025];
    char m_buff[1025];
    string m_ip;
    string m_key;
    string m_value;
    string m_command;
    string m_filename;
   
public:
    string getCommand(){return m_command;}
    string getKey(){return m_key;}
    string getValue(){return m_value;}
    string getIP(){return m_ip;}
    string getFilename(){return m_filename;}
    char* getMessage(){return m_message;}
    int getIdSock(){return m_idSock;}
    
    Server();

    int CreateSock();
    
    int SetSockResuse();
    
    int Bind(const char* port);
    
    int Listen(int n);
   
    int Accept();
    
    bool RecieveMessage(int id);
    
    bool ReadIp(int id);
    
    void CloseClient(int id)
    {
        close(id);
    }
    int SendMessage(string mess,int id)
    {
        return send(id, mess.c_str(), mess.size(),0);
    }

    bool RecieveFile(FILE* pDown, int id)
    {
        rewind(pDown);
        struct pollfd fds;
        fds.fd = id; // add id cua file fd can theo doi
        fds.events = POLLIN; // co the doc du lieu
        while(true)
        {   
            int ret = poll(&fds,1,5000); // cho de dc read
            if(ret ==-1)
            {
                return false;
            }
            else if(ret == 0) // qua thoi gian timeout se dung
            {
                fclose(pDown);
                break;
            }
            else
            {
                int r = recv(id,m_buff,1024,0);
                fputs(m_buff,pDown);
                memset(m_buff,0, sizeof(m_buff));
            }

        }
        return true;      
    }

    void SendFile(FILE *pFile, int id)
    {
        fseek(pFile,0, SEEK_END);
        size_t lenfile = ftell(pFile);
        rewind(pFile); //fseek(pFile,0, 0);                            

        struct pollfd fds;
        fds.fd = id; // add id cua file fd can theo doi
        fds.events = POLLOUT; // co the ghi du lieu vao
        int n = 0;
        while(true)   
        {
            int r = fread(m_buff,1,1024,pFile); // doc lan luot 1024 byte
            n+=r;
            int ret = poll(&fds, 1, -1);//                      
            int n_sent = send(id,m_buff,strlen(m_buff),0); 
            memset(m_buff,0, sizeof(m_buff));
            if(n==lenfile) break;
        }  
        fclose(pFile);   
    }
    
};
#endif