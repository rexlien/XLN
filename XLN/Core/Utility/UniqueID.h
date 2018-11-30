#ifndef __UniqueID_h__
#define __UniqueID_h__

#include <XLN/Core/Config/Config.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>


namespace XCr
{
	namespace Util
	{
        
        class XLN_CLASS_DECL UniqueID
        {
            public:
                UniqueID();
                ~UniqueID();
                
                static void Generate(UniqueID& id);
                bool operator== (const UniqueID& rhs) const
                {
                    //return memcmp(m_Bytes, id.m_Bytes, NUMBYTEUID);
                    return m_UID == rhs.m_UID;
                }
                
                UniqueID& operator= (const UniqueID& rhs)
                {   
                    //memcpy(m_Bytes, lhs.m_Bytes, NUMBYTEUID);
                    m_UID = rhs.m_UID;
                    return *this;
                }
                
                bool operator < (const UniqueID& rhs) const
                {
                    return m_UID < rhs.m_UID;
                }
                /*
                XCr::UInt8* operator&()
                {
                    return m_UID.data;
                }
                */
                XCr::UInt8* GetData();
                const XCr::UInt8* GetData() const;
                XCr::UInt32 GetByteLength() const;
                XCr::UInt32 GetHash() const;
                XCr::String ToString();
                
                void SetUID(const char* str);
                static UniqueID GenStrUID(const char* str);
                static UniqueID GenStrUID(const XCr::String& str, bool caseSensitive);
                
            private:
               
                boost::uuids::uuid m_UID;
                
                
                
            
        
        };
        
		/*
        struct UIDHash : public boost::unary_function<UniqueID, std::size_t> {
            std::size_t operator()(UniqueID const& id) const
            {
                return id.GetHash();
            }
        };
		*/

		struct UIDHasher
		{
			std::size_t operator()(UniqueID const& id) const
			{
				return id.GetHash();
			}
		};

        
        
        /*
        class StringGenUID : public UniqueID
        {
        
            public:
                Generate(UniqueID& id);
                StringGenUID();
                StringGenUID(const XCr::Char* string);
                  
            private:
                //boost::uuids::uuid m_UIDSeed;
                //boost::uuids::name_generator m_NameSeed;
        };
*/

	}



}




#endif