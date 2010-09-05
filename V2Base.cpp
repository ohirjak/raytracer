/*
 * @file: V2Base.cpp
 * @author: Ondrej Hirjak, 2006 - 2010
 * @description: Basic functions.
 */

#include "V2Base.h"


using namespace V2;


String::String()
{
	buffer[0] = 0;
}


String::String(const String &s)
{
	strncpy(buffer, s, kStringLength);
}


String::String(const char *s)
{
	strncpy(buffer, s, kStringLength);
}


const bool String::operator ==(const String &s)
{
	return strcmp(buffer, s) == 0;
}


const String String::operator +(const String s) const
{
	String output;

#if WIN
	_snprintf(output, kStringLength, "%s%s", Ptr(), s.Ptr());
#else
	snprintf(output, kStringLength, "%s%s", Ptr(), s.Ptr());
#endif
	output.buffer[kStringLength -1] = 0;

	return (output);
}


const String String::operator +(const char *s)
{
	String output;

#if WIN
	_snprintf(output, kStringLength, "%s%s", Ptr(), s);
#else
	snprintf(output, kStringLength, "%s%s", Ptr(), s);
#endif
	output.buffer[kStringLength -1] = 0;

	return (output);
}

const String String::operator +(const int i) const
{
	String output;

#if WIN
	_snprintf(output, kStringLength, "%s%d", Ptr(), i);
#else
	snprintf(output, kStringLength, "%s%d", Ptr(), i);
#endif
	output.buffer[kStringLength -1] = 0;

	return (output);
}


bool String::operator <(const String& s) const
{
	return strcmp(Ptr(), s.Ptr()) < 0;
}


const String GenericException::GetTypeString() const
{
	return "Generic exception";
}


const String GenericException::GetErrorString() const
{
	return error + " in file " + file + " at line " + line;
}

