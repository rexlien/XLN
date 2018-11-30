#include "NetworkService.h"

using namespace XLN;

int64_t NetworkService::heartbeat(int64_t localtime)
{
    std::cout << "recieved" << localtime << std::endl;
    return std::numeric_limits<int64_t>::max();
}