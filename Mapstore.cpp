#include "Header/Mapstore.h"

Mapstore::Mapstore(int size)
{
    m_sizemap = size;
}

bool Mapstore::AddMap(string key, struct valueIP v)
{
    if(!isExistKey(key)&&m_store.size()<m_sizemap)
    {
        m_store.insert(pair<string, struct valueIP>(key,v));
        return true;
    }
    return false;
}
bool Mapstore::DeleteOnePair(string key, string ip)
{
    for (m_it = m_store.begin(); m_it != m_store.end(); m_it++) {
        if(m_it->first==key&&m_it->second.clientIP==ip)
        {   
            m_store.erase(m_it);
            return true;
        }
    }
    return false;
}

bool Mapstore::DeleteMultiPair(string ip)
{
    for (m_it = m_store.begin(); m_it != m_store.end();) {                            
        if(m_it->second.clientIP== ip)
        {
            m_store.erase(m_it++);
        }
        else
        ++m_it;    
    }
    return true;
}
    
bool Mapstore::isExistKey(string key)
{
    for (m_it = m_store.begin(); m_it != m_store.end(); m_it++) {
        if(m_it->first==key)
        {      
            m_pair.first = key;
            m_pair.second = m_it->second;         
            return true; // key da ton tai
        }
    }
    return false;
}

