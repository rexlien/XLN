#ifndef __StringTool_h__
#define __StringTool_h__


#include <XLN/Core/Config/Config.h>

//// If we're using the GCC 3.1 C++ Std lib
//#if COMPILER == COMPILER_GNUC && COMP_VER >= 310 && !defined(STLPORT)
//
//#include <ext/hash_map>
//namespace __gnu_cxx
//{
//	template <> struct hash< Ogre::_StringBase >
//	{
//		size_t operator()( const Ogre::_StringBase _stringBase ) const
//		{
//			/* This is the PRO-STL way, but it seems to cause problems with VC7.1
//			and in some other cases (although I can't recreate it)
//			hash<const char*> H;
//			return H(_stringBase.c_str());
//			*/
//			/** This is our custom way */
//			register size_t ret = 0;
//			for( Ogre::_StringBase::const_iterator it = _stringBase.begin(); it != _stringBase.end(); ++it )
//				ret = 5 * ret + *it;
//
//			return ret;
//		}
//	};
//}
//
//// If we're using plain vanilla VC7 Std lib
//#elif !defined( _STLP_HASH_FUN_H )
//
//#	if _DEFINE_DEPRECATED_HASH_CLASSES
//namespace std
//#	else
//namespace stdext
//#	endif
//{
//	template<> std::size_t hash_compare< XCr::String, 
//		std::less< XCr::String > >::operator()( const XCr::String& _stringBase ) const
//	{
//		/* This is the PRO-STL way, but it seems to cause problems with VC7.1
//		and in some other cases (although I can't recreate it)
//		hash<const char*> H;
//		return H(_stringBase.c_str());
//		*/
//		/** This is our custom way */
//		register size_t ret = 0;
//		for( XCr::String::const_iterator it = _stringBase.begin(); it != _stringBase.end(); ++it )
//			ret = 5 * ret + *it;
//
//		return ret;
//	}
//}
//
//#endif



namespace XCr
{
		//TODO: add unicode version
		class XLN_CLASS_DECL StringUtil
		{
			public:
				//typedef std::ostringstream StrStreamType;

				/** Removes any whitespace characters, be it standard space or
				TABs and so on.
				@remarks
				The user may specify wether they want to trim only the
				beginning or the end of the String ( the default action is
				to trim both).
				*/
				static void Trim( String& str, bool left = true, bool right = true );

				/** Returns a StringVector that contains all the substrings delimited
				by the characters in the passed <code>delims</code> argument.
				@param
				delims A list of delimiter characters to split by
				@param
				maxSplits The maximum number of splits to perform (0 for unlimited splits). If this
				parameters is > 0, the splitting process will stop after this many splits, left to right.
				*/
				static std::vector< String > Split( const String& str, const String& delims = "\t\n ", unsigned int maxSplits = 0);
                
                static void Split( std::vector< String >& outToken, const String& str, const String& delims = "\t\n ", unsigned int maxSplits = 0);
				/** Upper-cases all the characters in the string.
				*/
				static void ToLowerCase( String& str );

				/** Lower-cases all the characters in the string.
				*/
				static void ToUpperCase( String& str );


				/** Returns whether the string begins with the pattern passed in.
				@param pattern The pattern to compare with.
				@param lowerCase If true, the end of the string will be lower cased before
				comparison, pattern should also be in lower case.
				*/
				static bool StartsWith(const String& str, const String& pattern, bool lowerCase = true);

				/** Returns whether the string ends with the pattern passed in.
				@param pattern The pattern to compare with.
				@param lowerCase If true, the end of the string will be lower cased before
				comparison, pattern should also be in lower case.
				*/
				static bool EndsWith(const String& str, const String& pattern, bool lowerCase = true);

				/** Method for standardising paths - use forward slashes only, end with slash.
				*/
				static String StandardisePath( const String &init);

				/** Method for splitting a fully qualified filename into the base name
				and path.
				@remarks
				Path is standardised as in standardisePath
				*/
				static void SplitFilename(const String& qualifiedName, String* outPath, String* outFileName, String* outExt);
				static void WSplitFilename(const WString& qualifiedName, WString* outPath, WString* outFileName, WString* outExt);

				/** Simple pattern-matching routine allowing a wildcard pattern.
				@param str String to test
				@param pattern Pattern to match against; can include simple '*' wildcards
				@param caseSensitive Whether the match is case sensitive or not
				*/
				static bool Match(const String& parseStr, const String& pattern, bool caseSensitive = true);
				static String GetFileName(const XCr::String& filePath);
				static String GetExt(const XCr::String& filePath);
				static String GetRelativeFrom(const XCr::String& root, const XCr::String& file);


				static WString WGetFileName(const XCr::WString& filePath);
				static WString WGetExt(const XCr::WString& filePath);
				static WString WGetRelativeFrom(const XCr::WString& root, const XCr::WString& file);
				
				static XCr::Bool WIsAbsolutePath(const XCr::WString& path);
				static XCr::Bool WIsFile(const XCr::WString& path);
				static XCr::WString WConcatPath(const XCr::WString& p0, const XCr::WString& p1);


				static XCr::WString Convert(const XCr::String& str);
				static XCr::String Convert(const XCr::WString& str);
				//static XCr::Bool WIsReservedPath(XCr)
				


				/// Constant blank string, useful for returning by ref where local does not exist
				static const String BLANK;
				static const WString WBLANK;
		};


//#if COMPILER == COMPILER_GNUC && COMP_VER >= 310 && !defined(STLPORT)
//		typedef ::__gnu_cxx::hash< String > _StringHash;
//#elif !defined( _STLP_HASH_FUN_H )
//#	if _DEFINE_DEPRECATED_HASH_CLASSES
//		typedef std::hash_compare< String, std::less< String > > _StringHash;
//#	else
//		typedef stdext::hash_compare< String, std::less< String > > _StringHash;
//#	endif
//#else
//		typedef std::hash< String > _StringHash;
//#endif

} 




#endif