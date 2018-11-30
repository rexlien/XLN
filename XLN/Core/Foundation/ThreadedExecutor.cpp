//
// Created by Rex Lien on 3/2/17.
//

#include "ThreadedExecutor.h"

using namespace XCr;

ThreadedExecutor::ThreadedExecutor() {
    m_ThreadExit = true;
}

ThreadedExecutor::~ThreadedExecutor() {

}


void ThreadedExecutor::add(folly::Func func) {

}

void ThreadedExecutor::OnThreadPreRun() {

}


void ThreadedExecutor::OnThreadRun()
{


}

void ThreadedExecutor::OnThreadExit() {

}


void ThreadedExecutor::ThreadRun()
{

    OnThreadPreRun();
    while(!m_ThreadExit)
    {
        OnThreadRun();
    }
    OnThreadExit();
}


void ThreadedExecutor::Start() {

    if(m_ThreadExit)
    {
        m_ThreadExit = false;
        m_RunningThread = std::move(std::thread(&ThreadedExecutor::ThreadRun, this));
    }
}

void ThreadedExecutor::Stop()
{

    if(!m_ThreadExit)
    {   m_ThreadExit = true;
        if(m_RunningThread.joinable())
            m_RunningThread.join();
    }
}

