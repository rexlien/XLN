//
//  Task.cpp
//  LGCocosSample
//
//  Created by Rex Lien on 8/17/16.
//
//

#include "Task.h"

using namespace XGf;


Task::~Task()
{

}

CurlDownloadTask::CurlDownloadTask()
{
    m_ErrorCode = 0;
    memset(m_CurlErrbuf, 0, sizeof(m_CurlErrbuf));

    
}

CurlDownloadTask::~CurlDownloadTask()
{
    
}
