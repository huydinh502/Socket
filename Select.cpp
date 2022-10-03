#include "Header/Select.h"

Select::Select()
{
    FD_ZERO(&m_master);
    FD_ZERO(&m_copy);
    m_idmax = 0;
}

void Select::AddListFd(int id)
{
    FD_SET(id,&m_master);
    if(id>m_idmax) m_idmax = id;
}

void Select::MastoCopy()
{
    memcpy(&m_copy,&m_master,sizeof(m_master)); // nhu vay cung dc m_copy = m_master;
}

int Select::Wait()
{
    return select(m_idmax+1, &m_copy,NULL, NULL, NULL);
}

int Select::CheckIsSet(int i)
{
    return FD_ISSET(i,&m_copy);
}

void Select::RemoveListFd(int id)
{
    FD_CLR(id,&m_master);
}