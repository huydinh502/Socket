#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <map> //map
#include <utility>// pair
#include <poll.h> //poll

#include "Header/Server.h" // class Server
#include "Header/Select.h" // class Select
#include "Header/Mapstore.h" // class Mapstore

using namespace std;

int main(int argc, char* argv[])
{
    
    Server s;
    Mapstore m(10);
    //Create Socket
    if (s.CreateSock() == -1)
    {
        cerr<<"Create socket fail";
        exit(1); // 1 la fail, 0 la success
    }  else cout <<"Create socket success\n";
    //Set Socket Reuse
    if (s.SetSockResuse()== -1)
    {
       cerr<<"Set reuse fail";
       exit(1);
    } else cout<<"Set reuse success\n";
    //Bind
    if (s.Bind(argv[1]) == -1)
    {
        cerr<<"Bind fail";
        exit(1);
    } else cout<< "Bind success\n";
    //Listen
    if (s.Listen(10) == -1)
    {
        cerr<<"Listen fail";
        exit(1);
    } else cout <<"Listen success\n";
    
    Select sel;
    sel.AddListFd(s.getIdSock()); // add id cua socket server vao select


    while(true)
    {
        sel.MastoCopy(); 
        
        int n_select = sel.Wait(); 

        if (n_select < 0)
        {
            cerr <<"Select fail";
            exit(1);
        }
        
    for (int i = 0; i <= sel.getIdMax();i++) 
    {
        if (sel.CheckIsSet(i)) 
        {
            int close_fd = 0; // Dung de close sau khi giao tiep xong
            
            if (i == s.getIdSock())
            {   
                int idclient;
                idclient = s.Accept(); 
                sel.AddListFd(idclient);                 
                cout << "New connection id: "<< idclient <<'\n';
            }
            else
            {// Nhận dữ liệu hoặc sự kiện đóng kết nối từ client
                
                int n_recv = s.RecieveMessage(i);
               
                if(n_recv ==-1)
                {
                    cerr<<"Recieve message from id "<< i <<" fail";
                    close_fd = 1;
                } 
                else if (n_recv != 0)
                {
                    cout << "Recieve message from id: "<<i<< endl;
                     
                    if(!s.ReadIp(i))
                    {
                        cerr<< "Read Ip from id "<< i<<" fail";
                        exit(1);
                    }
                   
                    string command = s.getCommand();
                    string key = s.getKey();
                    string value = s.getValue();
                    string ip = s.getIP(); 
                    string filename = s.getFilename();              
                    string respon;
                  
                    if(command=="PUT")
                    {
                        cout << "PUT from connection id: "<<i<<'\n';
                        cout << "Command: PUT "<<key<<' '<<value << endl;
                        
                        struct valueIP v;
                        v.value = value;
                        v.clientIP = ip;
                        if(m.AddMap(key, v))
                        {
                            cout<<"PUT success\n";
                            respon = string("PUT success");// bao lai cho client
                        }
                        else
                        {
                            cout<<"PUT fail\n";
                            respon = string("PUT fail");// bao lai cho client
                        }                                 
                    }
                    else if(command=="GET")
                    {
                        
                        cout << "GET from connection id: "<<i<<'\n';
                        cout << "Command: GET "<< key << endl;
                        if(m.isExistKey(key))
                        {
                            cout <<"GET success\n";
                            respon = "Value: " + m.GetPair().second.value;
                        }
                        else respon = string("GET fail");      
                         
                    }
                    else if(command=="DEL")
                    {
                        cout << "DEL from connection id: "<<i<<'\n';
                        cout << "Command: DEL "<< key <<'\n';
                        if(m.DeleteOnePair(key,ip))
                        {
                          cout<<"DELETE success\n";
                          respon = "Key: " + key + " are deleted"; 
                        } else respon = string("DELETED fail");
                        
                    }
                    else if(command=="CLOSE")
                    {       
                        cout << "CLOSE from connection id: "<<i<<'\n';
                                                                                                               
                        if(m.DeleteMultiPair(ip))
                        {
                            cout<<"Client id "<<i<<" closed\n";
                            respon = string("CLOSE success");
                            close_fd = 1; 
                        } else respon = string("CLOSE fail");                     
                    }  
                    else if(command=="PUTFILE")
                    {    
                        cout << "PUTFILE from connection id: "<<i<<'\n';
                        FILE* pFile;
                        pFile = fopen("text1_1.txt", "w");
                        
                        if(s.RecieveFile(pFile,i))
                        {
                           respon = string("PUT file success");
                        }else respon = string("PUT file fail");
                        
                    }                    
                    else if (command=="DELFILE")
                    {
                        int rem = remove(filename.c_str());
                        
                        if(rem == 0)
                        {   
                            respon = string("Delete file success");
                        }
                        else
                        {
                            respon = string("Delete file fail");
                        }
                    } 
                    else if(command=="GETFILE")
                    {
                        cout << "GETFILE from connection id: "<<i<<'\n';
                        FILE* pFile;
                        pFile = fopen(filename.c_str(), "r");
                        
                        s.SendFile(pFile,i);

                    }    
                    
                    if(s.SendMessage(respon,i)==-1)
                    {
                        cerr<<"Send message to client id "<<i<<" fail"<< endl;
                        close_fd = 1;
                    }
                     
                }
                
            }
                if (close_fd == 1)  //Chi khi error hoac recv ==0, khong nhan dc gi
                {
                    sel.RemoveListFd(i);
                    s.CloseClient(i);
                    
                }// else not listen server
            }//if FD_ISSET
        }//for
        
    } //While true
    
    return 0;
}