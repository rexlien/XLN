#include "UniqueID.h"
#include <XLN/Core/Utility/StringTool.h>
#include <boost/uuid/nil_generator.hpp>


//#pragma comment(lib, "Rpcrt4.lib")

using namespace XCr::Util;


static boost::uuids::uuid s_IDSeed = boost::uuids::nil_uuid();
static boost::uuids::random_generator s_RndGenerator;


XCr::Util::UniqueID XCr::Util::UniqueID::GenStrUID( const char* str )
{	/*
	XCr::UInt32 ret = 0;
	char c;
	while(c = *(str++))
	{
		ret = Rotate(ret, 7) + c;
	}
	*/
	UniqueID id;
    boost::uuids::name_generator s_NameGenerator(s_IDSeed);
    id.m_UID = s_NameGenerator(str);
    return id;
	//return ret;
}


UniqueID XCr::Util::UniqueID::GenStrUID( const XCr::String& str, bool caseSensitive)
{	
	const XCr::String* targetStr = &str;

	if(!caseSensitive)
	{	XCr::String tmp(*targetStr);
		XCr::StringUtil::ToLowerCase(tmp);
		targetStr = &tmp;
	}
	
	return XCr::Util::UniqueID::GenStrUID(targetStr->c_str());
}



void XCr::Util::UniqueID::Generate( UniqueID& id )
{   
    //UUID uid;
    //::UuidCreate(&uid);
    //memcpy(id.m_Bytes, (void*)&uid, NUMBYTEUID);
    //boost::uuids::random_generator(id.m_UID);
    id.m_UID = s_RndGenerator();
}

XCr::Util::UniqueID::UniqueID()
{
	m_UID = boost::uuids::nil_uuid();
    //Generate(*this);
    //m_UIDSeed = boost::uuids::nil_uuid();
    //boost::uuids::random_generator(m_UID);
}

XCr::String XCr::Util::UniqueID::ToString()
{   
    
    XCr::Ostringstream strStream;
    
    char buf[32];
    for(std::size_t i = 0; i < m_UID.size(); i++)
    {   
        sprintf(buf, "%d", m_UID.data[i]);//m_Bytes[i]);
        strStream << buf << " ";
    }
    
    return strStream.str();
    
}

void XCr::Util::UniqueID::SetUID(const char* str)
{	
    XCr::UInt32 length = strlen(str);
    if(length != 32)
    {   
        return;
    }	
    
    for(XCr::UInt32 i=0; i < 16; i++)
    {
        XCr::UChar ch1, ch2;
        ch1 = str[i << 1];
        ch2 = str[(i << 1) + 1];
        XCr::UInt32 dig1, dig2 = 0;

        if(isdigit(ch1)) 
			dig1 = ch1 - '0';
        else if(ch1>='A' && ch1<='F') 
			dig1 = ch1 - 'A' + 10;
        else if(ch1>='a' && ch1<='f')
			dig1 = ch1 - 'a' + 10;

        if(isdigit(ch2)) 
			dig2 = ch2 - '0';
        else if(ch2>='A' && ch2<='F') 
			dig2 = ch2 - 'A' + 10;
        else if(ch2>='a' && ch2<='f') 
			dig2 = ch2 - 'a' + 10;
        
        m_UID.data[i] = dig1 * 16 + dig2;
    }
	XCr::UInt32 temp;

	temp = m_UID.data[0];
	m_UID.data[0] = m_UID.data[3];
	m_UID.data[3] = temp;

	temp = m_UID.data[1];
	m_UID.data[1] = m_UID.data[2];
	m_UID.data[2] = temp;

	temp = m_UID.data[4];
	m_UID.data[4] = m_UID.data[5];
	m_UID.data[5] = temp;

	temp = m_UID.data[6];
	m_UID.data[6] = m_UID.data[7];
	m_UID.data[7] = temp;
}

XCr::Util::UniqueID::~UniqueID()
{

}

XCr::UInt8* XCr::Util::UniqueID::GetData()
{
    return m_UID.data;
}

const XCr::UInt8* XCr::Util::UniqueID::GetData() const
{
    return m_UID.data;
}
XCr::UInt32 XCr::Util::UniqueID::GetByteLength() const
{
    return static_cast<XCr::UInt32>(m_UID.size());
}

XCr::UInt32 XCr::Util::UniqueID::GetHash() const
{
    return static_cast<XCr::UInt32>(boost::uuids::hash_value(m_UID));
}
/*
XCr::Util::StringGenUID::StringGenUID()
{
    m_UIDSeed = boost::uuids::nil_uuid();
    boost::uuids::name_generator s_NameGenerator(m_UIDSeed);
    return s_NameGenerator(ServiceClass::GetServiceName().c_str())
}
*/