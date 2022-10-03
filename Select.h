#ifndef SELECT_H_INCLUDED
#define SELECT_H_INCLUDED
#include<netinet/in.h>
#include<string.h>

class Select
{
    fd_set m_master; // tap hop file de theo doi
    fd_set m_copy; // tap hop file theo doi tai thoi diem bat ky
    int m_idmax;
public: 
    int getIdMax(){return m_idmax;}

    Select();
    
    void AddListFd(int id);
    
    void MastoCopy();
    
    int Wait();
    
    int CheckIsSet(int i);
    
    void RemoveListFd(int id);
   
};
#endif