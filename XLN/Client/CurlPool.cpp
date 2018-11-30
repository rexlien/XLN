//
//  CurlPool.cpp
//  LGCocosSample
//
//  Created by Rex Lien on 8/15/16.
//
//

#include "CurlPool.h"


CurlPool::CurlPool()
{
    m_Pool = new boost::lockfree::queue<CURL*>(256);
}
CurlPool::~CurlPool()
{
    CURL* curl = 0;
    while(m_Pool->pop(curl))
    {
        curl_easy_cleanup(curl);
    }
    delete m_Pool;
}

CURL* CurlPool::Obtain()
{
    CURL* curl = 0;
    bool success = m_Pool->pop(curl);
    if(success)
    {
        curl_easy_reset(curl);
        return curl;
        
    }
    else
    {
        return curl_easy_init();
    }
}

void CurlPool::Return(CURL *curl)
{
    //curl_easy_reset(curl);
    m_Pool->push(curl);
}

void Clear()
{
    
}

