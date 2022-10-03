#ifndef MAPSTORE_H_INCLUDED
#define MAPSTORE_H_INCLUDED
#include<string>
#include<map>
#include<utility>
using namespace std;

struct valueIP
{
    string value;
    string clientIP;
};

class Mapstore
{  
    map<string,struct valueIP> m_store;
    map<string, valueIP>::iterator m_it;
    pair<string, struct valueIP> m_pair;
    int m_sizemap;
public:
    pair<string, struct valueIP> GetPair(){return m_pair;}

    Mapstore(int size);
   
    bool AddMap(string key, struct valueIP v);
    
    bool DeleteOnePair(string key, string ip);
    
    bool DeleteMultiPair(string ip);
    
    bool isExistKey(string key);
};
#endif