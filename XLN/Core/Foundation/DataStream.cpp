#include "DataStream.h"
#include <assert.h>

#include <XLN/Core/Utility/StringTool.h>
//#include <XLN/Core/Foundation/MemoryDefine.h>


namespace XCr
{
	char DataStream::s_TmpArea[STREAM_TMP_SIZE];
	
	size_t DataStream::GetLine( String& retStr, bool trimAfter)
	{	
		size_t ret = this->ReadLine(s_TmpArea, m_Size);
		if(ret > 0)
		    retStr.assign(s_TmpArea);
		if(trimAfter)
			StringUtil::Trim(retStr);
		return ret;
	}

	void DataStream::GetAsString(String& retString)
	{	
	    retString.reserve(m_Size);
	    retString.resize(m_Size);
		Read(&retString[0], m_Size);
	
	}

	DataStream& DataStream::ByteOrderRead( void *v, XCr::UInt32 len ) 
	{
		if( m_InputByteOrder == XLN_ARCH_ENDIAN )
		{
			if(!Read( v, len ))
			{	//DOTHROW(XCr::Exception::ERR_INTERNAL_ERROR, XLN_WIDEN("Stream Read failed"));
			}
		}
		else 
		{
			for( int i=len-1; i>=0; i-- ) 
			{
				if(!Read( (XCr::UInt8 *)(v) + i, 1 ))
				{	//DOTHROW(XCr::Exception::ERR_INTERNAL_ERROR, XLN_WIDEN("Stream Read failed"));
			    }
			}
		}
		return *this;
	}

	DataStream& DataStream::ByteOrderWrite( void * v, XCr::UInt32 len )
	{
		if(m_OutputByteOrder == XLN_ARCH_ENDIAN)
		{	if(!Write(v, len))
			{	//DOTHROW(XCr::Exception::ERR_INTERNAL_ERROR, XLN_WIDEN("Stream Write failed"));
		    }	
		}
		else
		{
			for( int i=len-1; i>=0; i-- ) 
			{
				if(!Write( (XCr::UInt8 *)(v) + i, 1 ))
			   	{	//DOTHROW(XCr::Exception::ERR_INTERNAL_ERROR, XLN_WIDEN("Stream Write failed"));
			    }
			}
		}
		return *this;
	}

	DataStream::DataStream()
	{	
		
		m_Size = 0;
		m_InputByteOrder = static_cast<StreamByteOrder>(XLN_ARCH_ENDIAN);
		m_OutputByteOrder = static_cast<StreamByteOrder>(XLN_ARCH_ENDIAN);
		

	}
	MemoryDataStream::MemoryDataStream(void* pMem, size_t size, bool freeOnClose)
	//	: DataStrem()
	{	assert(pMem);
		m_Data = reinterpret_cast<Char *>(pMem);
		m_Size = size;
		m_Pos = m_Data;
		m_End = m_Data + size;
		m_FreeOnClose = freeOnClose;

	}



	MemoryDataStream::MemoryDataStream(DataStream& sourceStream, 
					 bool freeOnClose)
	{
		m_Size = sourceStream.Size();
		m_Data = XLN_NEW Char[m_Size];
		sourceStream.Read(m_Data, m_Size);
		sourceStream.Seek(0);
		m_Pos = m_Data;
		m_End = m_Data + m_Size;
		m_FreeOnClose = freeOnClose;


	}


	MemoryDataStream::MemoryDataStream(DataStreamPtr& sourceStream, 
					 bool freeOnClose)
	{
		m_Size = sourceStream->Size();
		m_Data = XLN_NEW Char[m_Size];
		sourceStream->Read(m_Data, m_Size);
		sourceStream->Seek(0);

		m_Pos = m_Data;
		m_End = m_Data + m_Size;
		m_FreeOnClose = freeOnClose;
		
		

	}



	MemoryDataStream::MemoryDataStream(size_t size, bool freeOnClose)
	{	m_Size = size;
		m_Data = XLN_NEW Char[m_Size];
		memset(m_Data, 0, m_Size);
		m_Pos = m_Data;
		m_End = m_Data + m_Size;
		m_FreeOnClose = freeOnClose;

	}



	MemoryDataStream::~MemoryDataStream()
	{	Close();

	}


	size_t MemoryDataStream::Read(void* buf, size_t count)
	{	assert((m_Pos + count) <= m_End);
		
		std::size_t cnt = count;
		if (m_Pos + cnt > m_End)
			cnt = m_End - m_Pos;
		if (cnt == 0)
			return 0;

		memcpy(buf, m_Pos, cnt);
		m_Pos += cnt;
		return cnt;

	}
	/** @copydoc DataStream::readLine
	*/
	size_t MemoryDataStream::ReadLine(char* buf, size_t maxCount, const String& delim)
	{	
		
		bool trimCR = false;
		if (delim.find_first_of('\n') != String::npos)
		{
			trimCR = true;
		}

		size_t readCount = strcspn(m_Pos, delim.c_str());
		if(readCount > maxCount)
			readCount = maxCount;

		if(readCount + m_Pos > m_End)
			readCount = m_End - m_Pos; 
		
		

		memcpy(buf, m_Pos,readCount);
		
		m_Pos += readCount + 1;
		
		if(trimCR && buf[readCount - 1] == '\r' )
		{
			readCount--;
		}
		

		buf[readCount] = '\0';

		return readCount;
		

	}

	/** @copydoc DataStream::skipLine
	*/
	size_t MemoryDataStream::SkipLine(const String& delim)
	{

		size_t readCount = strcspn(m_Pos, delim.c_str());
		if(readCount + m_Pos > m_End)
			readCount = m_End - m_Pos; 
		m_Pos += readCount + 1;

		return readCount;



	}

	/** @copydoc DataStream::skip
	*/
	void MemoryDataStream::Skip(long count)
	{	long cnt = count;
		if(cnt + m_Pos > m_End)
			cnt = (long)(m_End - m_Pos); 
		
		m_Pos += cnt;
		
	}

	/** @copydoc DataStream::seek
	*/
	XCr::Bool MemoryDataStream::Seek( size_t pos )
	{	//assert(m_Data);
		assert(m_Data + pos <= m_End);
		m_Pos = m_Data + pos;
		return true;
		
	}

	XCr::Bool MemoryDataStream::Seek( size_t pos, SeekFlag origin )
	{	
		//assert(m_Data + origin + pos <= m_End)
		if(origin == SF_CUR)
			m_Pos += pos;
		else if(origin == SF_SET)
			m_Pos = m_Data + pos;
		else
			m_Pos = m_End - pos;
		assert(m_Pos <= m_End);
		//m_Pos = origin + pos;
		return true;
		
	}
	/** @copydoc DataStream::tell
	*/
	size_t MemoryDataStream::Tell(void) const
	{	//assert(m_Data);
		return m_Pos - m_Data;
	}

	/** @copydoc DataStream::eof
	*/
	bool MemoryDataStream::Eof(void) const
	{	//assert(m_Data);
		return (m_Pos >= m_End);

	}

	/** @copydoc DataStream::close
	*/
	void MemoryDataStream::Close(void)
	{	if(m_Data && m_FreeOnClose)
		{	XLN_DELETE [] m_Data;
			m_Data = 0;
		}

	}

	size_t MemoryDataStream::Write( void *buf, size_t count )
	{
		assert((m_Pos + count) <= m_End);

		std::size_t cnt = count;
		if (m_Pos + cnt > m_End)
			cnt = m_End - m_Pos;
		if (cnt == 0)
			return 0;

		memcpy(m_Pos, buf, cnt);
		m_Pos += cnt;
		return cnt;
	}
}

