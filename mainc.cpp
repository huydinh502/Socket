#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <poll.h>
#include <ctype.h>

#include "Header/Client.h"
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
    Client c;
    
    //Create Socket
    if (c.CreateSock() == -1)
    {
        cerr<<"Create socket fail";
        exit(1); // 1 la fail, 0 la success
    }  else cout <<"Create socket success\n";
    
    if (c.Connect(atoi(argv[2]), argv[1]) ==-1) // Ket noi bi loi
    {
        cerr<<"Connect server fail";
        exit(0);
    }  else cout <<"Connect server success\n";  
    
    
    fd_set master;
    FD_ZERO(&master);
    FD_SET(0,&master);
   
    while (true)
    {
        cout<<"Command of client:\n";

        int s = select(1,&master, NULL, NULL, NULL); // khoa chuong trinh cho su kien ben client
      
        //int r = read(0,huy,sizeof(huy)); // read de file fd ko con trang thai san sang doc
        
        if(s==-1)
        {
            cerr << "Select error";
            exit(1);
        }
        
        if(FD_ISSET(0, &master))
        {
            char message[1024];
            memset(message,0, 1024);

            int r = read(0,message, 1024);
           
            if(r==-1)
            {
                cerr<<"Read fail";
                exit(0);
            }

            message[r-1]= 0;

            string command;
            string key;
            string value;
            string filename;
            char* input[3];
            int n=1;
            input[0]=message;
            
            for(int i=0; i<r ; i++)
            {
                if(message[i]==' ')
                {
                    message[i]=0;
                    input[n]=&message[i+1];
                    n+=1;
                }
            }
            command = string(input[0]);  
            if(command.size()==3 || command =="CLOSE")
            {
                string send;
                if(command!="CLOSE")
                {      
                    key = string(input[1]);            
                    if(command=="PUT")
                    {      
                        value = string(input[2]);  
                        send = command+" "+key+" "+value;
                    }
                    else send = command+" "+key;
                } else send = command;
                
                if(c.SendMessage(send)==-1)
                {
                    cerr <<"Send fail";
                    exit(1);
                } else cout<< "Send success\n";

                if(c.RecieveMessage()==-1)
                {
                    cerr <<"Recieve fail";
                    exit(1);
                } 
                string mess = c.getMessage();
              
                cout << mess << endl;
                if(mess=="CLOSE success") break;
            }
            else
            {  
                filename = string(input[1]);
                
                if(command=="PUTFILE")
                {
                    if(c.SendMessage(command+" "+filename)==-1)
                    {
                        cerr <<"Send fail";
                        exit(1);
                    } else cout<< "Send success\n";
                    
                    FILE *pFile;              
                    pFile = fopen(filename.c_str(),"r");// key luc nay la ten file
                    c.SendFile(pFile);
                    if(c.RecieveMessage()==-1)
                    {
                        cerr <<"Recieve fail";
                        exit(1);
                    } 
                    string mess = c.getMessage();
                    if(mess =="Closed!") 
                    {
                        cout << "Client disconnected !\n";
                        break;
                    }
                    cout << c.getMessage()<< endl;
                    
                }  
                else if(command=="GETFILE")
                {
                    if(c.SendMessage(command+" "+filename)==-1)
                    {
                        cerr <<"Send fail";
                        exit(1);
                    } else cout<< "Send success\n";

                    FILE* pDown;
                    pDown= fopen("download.txt","w");
                    c.RecieveFile(pDown);
                    
                } 
                else if(command=="DELFILE")
                {
                    if(c.SendMessage(command+" "+filename)==-1)
                    {
                        cerr <<"Send fail";
                        exit(1);
                    } else cout<< "Send success\n";

                    if(c.RecieveMessage()==-1)
                    {
                        cerr <<"Recieve fail";
                        exit(1);
                    } 
                    cout << c.getMessage()<< endl;              
                }  
                else if(command =="GETFILE")
                {
                    FILE* pFile;
                    pFile = fopen("filedownload.txt", "w");
                    c.RecieveFile(pFile);
                    
                }           
            }    
                memset(message,0, 1024);                           
        
        }//if FD_ISSET
              
    } // while true
    return 0;
}//main
