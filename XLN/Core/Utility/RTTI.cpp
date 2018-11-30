#include "RTTI.h"

using namespace XCr;

//---------------------------------------------------------------------------
RTTI::RTTI(const char* name, const RTTI* baseRTTI) :
m_Name(name), m_BaseRTTI(baseRTTI)
{
}
//---------------------------------------------------------------------------
bool RTTI::CopyName(char* nameBuffer, unsigned int maxSize) const
{   
    
    const char* name = GetName();
    if (!name || !nameBuffer)
    {
        strncpy(nameBuffer, "\0", maxSize);
        return false;
    }

    strncpy(nameBuffer, name, maxSize);
    
    return true;
}
//---------------------------------------------------------------------------
/*
bool XCr::RTTI::operator<( const RTTI& rhs ) const
{   
    const XCr::RTTI* base = this;
    do 
    {
        if(&rhs == base)
        {
            return false;
        }
        base = base->GetBaseRTTI(); 
    } while (base);

    return true;
}

bool XCr::RTTI::operator==( const RTTI& rhs ) const
{
    return true;
}
*/