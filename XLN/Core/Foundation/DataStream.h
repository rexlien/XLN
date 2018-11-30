#ifndef __DataStream_h__
#define __DataStream_h__

#include <XLN/Core/Config/Config.h>
#include <boost/type_traits/integral_constant.hpp>
#include <XLN/Core/Utility/SharedPtr.h>


namespace XCr {


	class XLN_CLASS_DECL DataStream
	{	
		public:
			enum StreamByteOrder
			{
				BO_BIG_ENDIAN = XLN_ENDIAN_BIG,
				BO_LIT_ENDIAN = XLN_ENDIAN_LITTLE,

				BO_FORCEDWORD = 0xfffffff
			};
		
			enum SeekFlag
			{
				SF_SET,
				SF_CUR,
				SF_END,
			};

		protected:
		/// The name (e.g. resource name) that can be used to identify the source fot his data (optional)
		
		/// Size of the data in the stream (may be 0 if size cannot be determined)
		size_t m_Size;

		
#define STREAM_TMP_SIZE 1024
		/// Temporary buffer area used for formatted read
		static char s_TmpArea[STREAM_TMP_SIZE];
	public:
		/// Constructor for creating unnamed streams
		DataStream();
		/// Constructor for creating named streams
		virtual ~DataStream() {}
		// Streaming operators
		//template<typename T> DataStream& operator>>(T& val);
		/** Read the requisite number of bytes from the stream, 
		stopping at the end of the file.
		@param buf Reference to a buffer pointer
		@param count Number of bytes to read
		@returns The number of bytes read
		*/

		virtual size_t Write(void *buf, size_t count) = 0;
		virtual size_t Read(void* buf, size_t count) = 0;
		/** Get a single line from the stream.
		@remarks
		The delimiter character is not included in the data
		returned, and it is skipped over so the next read will occur
		after it. The buffer contents will include a
		terminating character.
		@param buf Reference to a buffer pointer
		@param maxCount The maximum length of data to be read, excluding the terminating character
		@param delim The delimiter to stop at
		@returns The number of bytes read, excluding the terminating character
		*/
		virtual size_t ReadLine(char* buf, size_t maxCount, const String& delim = "\n") = 0;

		/** Returns a String containing the next line of data, optionally 
		trimmed for whitespace. 
		@remarks
		This is a convenience method for text streams only, allowing you to 
		retrieve a String object containing the next line of data. The data
		is read up to the next newline character and the result trimmed if
		required.
		@param 
		trimAfter If true, the line is trimmed for whitespace (as in 
		String.trim(true,true))
		*/
		//String& operator<< (const)
		virtual size_t GetLine( String& retString, bool trimAfter = true );// {};

		/** Returns a String containing the entire stream. 
		@remarks
		This is a convenience method for text streams only, allowing you to 
		retrieve a String object containing all the data in the stream.
		*/
		virtual void GetAsString(String& retString);

		/** Skip a single line from the stream.
		@param delim The delimiter(s) to stop at
		@returns The number of bytes skipped
		*/
		virtual size_t SkipLine(const String& delim = "\n") = 0;

		/** Skip a defined number of bytes. This can also be a negative value, in which case
		the file pointer rewinds a defined number of bytes. */
		virtual void Skip(long count) = 0;

		/** Repositions the read point to a specified byte.
		*/
		virtual XCr::Bool Seek( size_t pos ) = 0;
		virtual XCr::Bool Seek( size_t pos, SeekFlag origin ) = 0;

		/** Returns the current byte offset from beginning */
		virtual size_t Tell(void) const = 0;

		/** Returns true if the stream has reached the end.
		*/
		virtual bool Eof(void) const = 0;

		/** Returns the total size of the data to be read from the stream, 
		or 0 if this is indeterminate for this stream. 
		*/
		size_t Size(void) const { return m_Size; }

		/** Close the stream; this makes further operations invalid. */
		virtual void Close(void) = 0;
		
		template <class T>
		friend DataStream& operator>> (DataStream& stream, T& buf)
		{	
			
			StreamReadByteSizeOfT(stream, buf, boost::integral_constant<int, sizeof(T) >(),boost::integral_constant<bool, std::is_scalar<T>::value >());
			return stream;
		}

		template <class T>
		friend DataStream& operator<< (DataStream& stream, T& obj)
		{
			StreamWriteByteSizeOfT(stream, obj, boost::integral_constant<int, sizeof(T) >(),boost::integral_constant<bool, std::is_scalar<T>::value >());
			return stream;
		}

		DataStream& ByteOrderRead( void * v, XCr::UInt32 len ); 
		DataStream& ByteOrderWrite( void * v, XCr::UInt32 len ); 

		

		StreamByteOrder m_InputByteOrder;
		StreamByteOrder m_OutputByteOrder;

		public:
			
			
			

			//static const StreamByteOrder GetPlatformByteOrder() = ARCH_ENDIAN;
			StreamByteOrder SetInputByteOrder(StreamByteOrder byteOrder )
			{
				m_InputByteOrder = byteOrder;
				return m_InputByteOrder;
			}
			StreamByteOrder GetInputByteOrder() const
			{
				return m_InputByteOrder;
			}

			StreamByteOrder SetOutputByteOrder(StreamByteOrder byteOrder)
			{
				m_OutputByteOrder = byteOrder;
				return m_OutputByteOrder;
			}
			StreamByteOrder GetOutputByteOrder() const
			{
				return m_OutputByteOrder;
			}


	};

	typedef XCr::SharedPtr<DataStream> DataStreamPtr;
	/// List of DataStream items
	typedef std::list<DataStreamPtr> DataStreamList;
	/// Shared pointer to list of DataStream items
	typedef XCr::SharedPtr<DataStreamList> DataStreamListPtr;

	/** Shared pointer to allow data streams to be passed around without
	worrying about deallocation
	*/
	

	/** Common subclass of DataStream for handling data from chunks of memory.
	*/
	class  XLN_CLASS_DECL MemoryDataStream : public DataStream
	{
	protected:
		/// Pointer to the start of the data area
		Char* m_Data;
		/// Pointer to the current position in the memory
		Char* m_Pos;
		/// Pointer to the end of the memory
		Char* m_End;
		/// Do we delete the memory on close
		bool m_FreeOnClose;			
	public:

		/** Wrap an existing memory chunk in a stream.
		@param pMem Pointer to the exising memory
		@param size The size of the memory chunk in bytes
		@param freeOnClose If true, the memory associated will be destroyed
		when the stream is destroyed.
		*/
		MemoryDataStream(void* pMem, size_t size, bool freeOnClose = false);


		/** Create a stream which pre-buffers the contents of another stream.
		@remarks
		This constructor can be used to intentionally read in the entire
		contents of another stream, copying them to the internal buffer
		and thus making them available in memory as a single unit.
		@param sourceStream Another DataStream which will provide the source
		of data
		@param freeOnClose If true, the memory associated will be destroyed
		when the stream is destroyed.
		*/
		MemoryDataStream(DataStream& sourceStream, 
			bool freeOnClose = true);

		/** Create a stream which pre-buffers the contents of another stream.
		@remarks
		This constructor can be used to intentionally read in the entire
		contents of another stream, copying them to the internal buffer
		and thus making them available in memory as a single unit.
		@param sourceStream Weak reference to another DataStream which will provide the source
		of data
		@param freeOnClose If true, the memory associated will be destroyed
		when the stream is destroyed.
		*/
		MemoryDataStream(DataStreamPtr& sourceStream, 
			bool freeOnClose = true);


		/** Create a stream with a brand new empty memory chunk.
		@param size The size of the memory chunk to create in bytes
		@param freeOnClose If true, the memory associated will be destroyed
		when the stream is destroyed.
		*/
		MemoryDataStream(size_t size, bool freeOnClose = true);
		

		~MemoryDataStream();

		/** Get a pointer to the start of the memory block this stream holds. */
		Char* GetPtr(void) { return m_Data; }

		/** Get a pointer to the current position in the memory block this stream holds. */
		Char* GetCurrentPtr(void) { return m_Pos; }

		/** @copydoc DataStream::read
		*/
		size_t Read(void* buf, size_t count);
		size_t Write(void *buf, size_t count);
		/** @copydoc DataStream::readLine
		*/
		size_t ReadLine(char* buf, size_t maxCount, const String& delim = "\n");

		/** @copydoc DataStream::skipLine
		*/
		size_t SkipLine(const String& delim = "\n");

		/** @copydoc DataStream::skip
		*/
		void Skip(long count);

		/** @copydoc DataStream::seek
		*/
		XCr::Bool Seek( size_t pos );
		XCr::Bool Seek( size_t pos, SeekFlag origin );

		/** @copydoc DataStream::tell
		*/
		size_t Tell(void) const;

		/** @copydoc DataStream::eof
		*/
		bool Eof(void) const;

		/** @copydoc DataStream::close
		*/
		void Close(void);

		/** Sets whether or not to free the encapsulated memory on close. */
		void SetFreeOnClose(bool free) { m_FreeOnClose = free; }
	};

	/** Shared pointer to allow memory data streams to be passed around without
	worrying about deallocation
	*/
	typedef XCr::SharedPtr<MemoryDataStream> MemoryDataStreamPtr;
	
	

}

template<class T, class U, class X>
void StreamReadByteSizeOfT(XCr::DataStream& stream, T& buf, U& trait, X& intriscType)
{	
	//BOOST_STATIC_ASSERT(false);
	//stream.ByteOrderRead(&buf, sizeof(T));
}

template<class T>
void StreamReadByteSizeOfT(XCr::DataStream& stream, T& buf, boost::integral_constant<int, 1 >& trait, 
					   boost::integral_constant<bool, true >& intriscType )
{
	stream.ByteOrderRead(&buf, 1);
}

template<class T>
void StreamReadByteSizeOfT(XCr::DataStream& stream, T& buf, boost::integral_constant<int, 4 >& trait,
					   boost::integral_constant<bool, true >& intriscType)
{
	stream.ByteOrderRead(&buf, 4);
}

template<class T>
void StreamReadByteSizeOfT(XCr::DataStream& stream, T& buf, boost::integral_constant<int, 2 >& trait, 
					   boost::integral_constant<bool, true >& intriscType )
{
	stream.ByteOrderRead(&buf, 2);
}

template<class T, class U, class X>
void StreamWriteByteSizeOfT(XCr::DataStream& stream, T& buf, U& trait, X& intriscType)
{	
	//BOOST_STATIC_ASSERT(false);
	//stream.ByteOrderRead(&buf, sizeof(T));
}

template<class T>
void StreamWriteByteSizeOfT(XCr::DataStream& stream, T& buf, boost::integral_constant<int, 1 >& trait, 
					   boost::integral_constant<bool, true >& intriscType )
{
	stream.ByteOrderWrite(&buf, 1);
}

template<class T>
void StreamWriteByteSizeOfT(XCr::DataStream& stream, T& buf, boost::integral_constant<int, 4 >& trait,
					   boost::integral_constant<bool, true >& intriscType)
{
	stream.ByteOrderWrite(&buf, 4);
}

template<class T>
void StreamWriteByteSizeOfT(XCr::DataStream& stream, T& buf, boost::integral_constant<int, 2 >& trait, 
					   boost::integral_constant<bool, true >& intriscType )
{
	stream.ByteOrderWrite(&buf, 2);
}



#endif