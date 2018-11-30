//
//  CurlPool.hpp
//  LGCocosSample
//
//  Created by Rex Lien on 8/15/16.
//
//

#ifndef CurlPool_hpp
#define CurlPool_hpp

#include <stdio.h>
#include "boost/lockfree/queue.hpp"
#include "curl/curl.h"
//#include "tbb/concurrent_queue.h"

class CurlPool
{
public:
    CurlPool();
    ~CurlPool();
    
    CURL* Obtain();
    
    void Return(CURL *curl);
    
    void Clear();
    
private:
    
    
    //tbb::concurrent_queue<CURL*> m_Pool2;
    
    boost::lockfree::queue<CURL*> *m_Pool;
    
};




#endif /* CurlPool_hpp */
