#include "StringTool.h"

using namespace XCr;

/** Removes any whitespace characters, be it standard space or
TABs and so on.
@remarks
The user may specify wether they want to trim only the
beginning or the end of the String ( the default action is
to trim both).
*/
void StringUtil::Trim( String& str, bool left, bool right )
{	static const String delim = " \t\r";
    if(left)
    {	//size_t leftTrim = str.find_first_not_of(delim);
	    //if(leftTrim != String::npos && leftTrim != 0)
	    str.erase(0, str.find_first_not_of(delim));
    }

    if(right)
    {	size_t rightTrim = str.find_last_not_of(delim);
	    //if(rightTrim != String::npos)
	    str.erase(rightTrim + 1);
    }

}

/** Returns a StringVector that contains all the substrings delimited
by the characters in the passed <code>delims</code> argument.
@param
delims A list of delimiter characters to split by
@param
maxSplits The maximum number of splits to perform (0 for unlimited splits). If this
parameters is > 0, the splitting process will stop after this many splits, left to right.
*/
std::vector< String > StringUtil::Split( const String& str, const String& delims, 
 unsigned int maxSplits)
{	
    std::vector<String> vec;
    size_t left = str.find_first_not_of(delims);
    size_t right = 0;

    unsigned int curSplit = 1;
    String tmp;
    while(left != String::npos && ((maxSplits == 0) || curSplit <= maxSplits))
    {	
	    right = str.find_first_of(delims, left);
    	
	    tmp.assign(str, left, right - left);
	    vec.push_back(tmp);
	    left = str.find_first_not_of(delims, right);

	    curSplit++;
    		
    }

    return vec;
}

void XCr::StringUtil::Split( std::vector< String >& outToken, const String& str, const String& delims, unsigned int maxSplits)
{   outToken.clear();
    size_t left = str.find_first_not_of(delims);
    size_t right = 0;

    unsigned int curSplit = 1;
     
    String tmp;
    while(left != String::npos && ((maxSplits == 0) || curSplit <= maxSplits))
    {	
        right = str.find_first_of(delims, left);
        tmp.assign(str, left, right - left);
        outToken.push_back(tmp);
        left = str.find_first_not_of(delims, right);

        curSplit++;

    }
}
/** Upper-cases all the characters in the string.
*/
void StringUtil::ToLowerCase( String& str )
{	transform(str.begin(), str.end(), str.begin(), tolower);

}

/** Lower-cases all the characters in the string.
*/
void StringUtil::ToUpperCase( String& str )
{	transform(str.begin(), str.end(), str.begin(), toupper);

}


/** Returns whether the string begins with the pattern passed in.
@param pattern The pattern to compare with.
@param lowerCase If true, the end of the string will be lower cased before
comparison, pattern should also be in lower case.
*/
bool StringUtil::StartsWith(const String& str, const String& pattern, bool lowerCase)
{	size_t len = pattern.length();

    if(len > str.length() || len == 0)
	    return false;

    String src = str.substr(0, len);
    String tmpPattern = pattern;

    if(lowerCase)
    {	ToLowerCase(tmpPattern);
	    ToLowerCase(src);

    }

    return (src == tmpPattern);
}

/** Returns whether the string ends with the pattern passed in.
@param pattern The pattern to compare with.
@param lowerCase If true, the end of the string will be lower cased before
comparison, pattern should also be in lower case.
*/
bool StringUtil::EndsWith(const String& str, const String& pattern, bool lowerCase)
{	size_t len = pattern.length();
    size_t strLen = str.length();	

    if(len == 0 || len > strLen)
	    return false;
    String src = str.substr(strLen - len, strLen);
    String tmpPattern = pattern;

    if(lowerCase)
    {	
	    ToLowerCase(tmpPattern);
	    ToLowerCase(src);

    }

    return (tmpPattern == src);
}

/** Method for standardising paths - use forward slashes only, end with slash.
*/
String StringUtil::StandardisePath( const String &init)
{	String res(init);

replace(res.begin(), res.end(), '\\', '/');
if(res.at(res.length() - 1) != '/')
	res.append("/");
return res;
}

/** Method for splitting a fully qualified filename into the base name
and path.
@remarks
Path is standardised as in standardisePath
*/
void StringUtil::SplitFilename(const String& qualifiedName, String* outPath, String* outFileName, String* outExt)
{	
static const String pathdelim = "\\/"; 

size_t startPosFileName = qualifiedName.find_last_of(pathdelim);
if(startPosFileName != String::npos)
{	
	if(outPath)
		*outPath = qualifiedName.substr(0, startPosFileName);
	startPosFileName += 1;
	
}	
else
{
	startPosFileName = 0;
}

String fileName = qualifiedName.substr(startPosFileName, qualifiedName.length());
size_t extentionPos = fileName.find_last_of('.');
if(extentionPos != String::npos)
{	if(outFileName)
	{	*outFileName = fileName.substr(0, extentionPos);

	}
	if(outExt)
		*outExt = fileName.substr(extentionPos, fileName.length());
}
else
{
	if(outFileName)
		*outFileName = fileName;
}
}

/** Simple pattern-matching routine allowing a wildcard pattern.
@param str String to test
@param pattern Pattern to match against; can include simple '*' wildcards
@param caseSensitive Whether the match is case sensitive or not
*/
bool StringUtil::Match(const String& parseStr, const String& pattern, bool caseSensitive)
{	

String tmpString = parseStr;
String tmpPattern = pattern;
if(!caseSensitive)
{	StringUtil::ToLowerCase(tmpPattern);
	StringUtil::ToLowerCase(tmpString);
}

const char* pat = tmpPattern.c_str();
const char* str = tmpString.c_str();

char c2;

while (true) 
{
	if (*pat == 0) 
	{
		if (*str == 0) return true;
		else           return false;
	}
	if ((*str == 0) && (*pat != '*')) return false;
	if (*pat=='*') 
	{
		pat++;
		if (*pat==0) return true;
		while (true) 
		{
			if (Match(XCr::String(str), XCr::String(pat))) return true;
			if (*str==0) return false;
			str++;
		}
	}
	if (*pat=='?') goto match;
	if (*pat=='[') 
	{
		pat++;
		while (true) 
		{
			if ((*pat==']') || (*pat==0)) return false;
			if (*pat==*str) break;
			if (pat[1] == '-') 
			{
				c2 = pat[2];
				if (c2==0) return false;
				if ((*pat<=*str) && (c2>=*str)) break;
				if ((*pat>=*str) && (c2<=*str)) break;
				pat+=2;
			}
			pat++;
		}
		while (*pat!=']') 
		{
			if (*pat==0) 
			{
				pat--;
				break;
			}
			pat++;
		}
		goto match;
	}

	if (*pat=='\\') 
	{
		pat++;
		if (*pat==0) return false;
	}
	if (*pat!=*str) return false;

	match:
		pat++;
		str++;
}
}

String StringUtil::GetFileName( const XCr::String& filePath )
{	
	 XCr::String name;
	 XCr::String ext;
	 SplitFilename(filePath, NULL, &name, &ext);
	 return name + ext;
}

String StringUtil::GetExt( const XCr::String& filePath )
{
	 XCr::String ext;
	 SplitFilename(filePath, NULL, NULL, &ext);
	 return ext;
}

String XCr::StringUtil::GetRelativeFrom( const XCr::String& root, const XCr::String& file )
{
	if(root.size() >= file.size())
		return file;
	if(root == file.substr(0, root.size())) 
		return file.substr(root.size(), file.size());

	return file;
}

void XCr::StringUtil::WSplitFilename( const WString& qualifiedName, WString* outPath, WString* outFileName, WString* outExt )
{
	 static const WString pathdelim = XLN_WIDEN("\\/");

	 size_t startPosFileName = qualifiedName.find_last_of(pathdelim);
	 if(startPosFileName != String::npos)
	 {	
		 if(outPath)
			 *outPath = qualifiedName.substr(0, startPosFileName);
		 startPosFileName += 1;

	 }	
	 else
	 {
		 startPosFileName = 0;
	 }

	 WString fileName = qualifiedName.substr(startPosFileName, qualifiedName.length());
	 size_t extentionPos = fileName.find_last_of(XLN_WIDEN('.'));
	 if(extentionPos != String::npos)
	 {	if(outFileName)
		 {	*outFileName = fileName.substr(0, extentionPos);

			}
		 if(outExt)
			 *outExt = fileName.substr(extentionPos, fileName.length());
	 }
	 else
	 {
		 if(outFileName)
			 *outFileName = fileName;
	 }
}

XCr::WString XCr::StringUtil::WGetFileName( const XCr::WString& filePath )
{
	 XCr::WString name;
	 XCr::WString ext;
	 WSplitFilename(filePath, NULL, &name, &ext);
	 return name + ext;
}

XCr::WString XCr::StringUtil::WGetExt( const XCr::WString& filePath )
{
	 XCr::WString ext;
	 WSplitFilename(filePath, NULL, NULL, &ext);
	 return ext;
}

XCr::WString XCr::StringUtil::WGetRelativeFrom( const XCr::WString& root, const XCr::WString& file )
{
	 if(root.size() >= file.size())
		 return WBLANK;
	 if(root == file.substr(0, root.size())) 
		 return file.substr(root.size(), file.size());

	 return WBLANK;
}

XCr::Bool XCr::StringUtil::WIsAbsolutePath( const XCr::WString& path )
{	
	if(path.size() < 2)
		return false;
	return (path[1] == XLN_WIDEN(':'));
}

XCr::WString XCr::StringUtil::WConcatPath( const XCr::WString& p0, const XCr::WString& p1 )
{
	return p0 + p1;
}

XCr::WString XCr::StringUtil::Convert( const XCr::String& str )
{	

	WString wStr;
	for(std::size_t i = 0; i < str.size(); i++)
	{	
		 wchar_t wc = str[i];// << 8;
		 wStr.push_back(wc);
	}
	return wStr;

}

XCr::String XCr::StringUtil::Convert( const XCr::WString& wStr )
{
	String str;
	for(std::size_t i = 0; i < wStr.size(); i++)
	{	
		char c = wStr[i];
		str.push_back(c);
	}

	return str;
}

XCr::Bool XCr::StringUtil::WIsFile( const XCr::WString& path )
{
	if(path[path.size()-1] != XLN_WIDEN('/') || path[path.size()-1] != XLN_WIDEN('\\'))
		return false;
	return true;
}


const String StringUtil::BLANK;
const WString StringUtil::WBLANK;