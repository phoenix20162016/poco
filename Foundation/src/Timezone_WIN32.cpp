//
// Timezone_WIN32.cpp
//
// Library: Foundation
// Package: DateTime
// Module:  Timezone
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Timezone.h"
#include "Poco/UnicodeConverter.h"
#include "Poco/Exception.h"
#include "Poco/UnWindows.h"
#include <ctime>


namespace Poco {


int Timezone::utcOffset()
{
	TIME_ZONE_INFORMATION tzInfo;
	DWORD dstFlag = GetTimeZoneInformation(&tzInfo);
	return -tzInfo.Bias*60;
}


int Timezone::dst()
{
	TIME_ZONE_INFORMATION tzInfo;
	DWORD dstFlag = GetTimeZoneInformation(&tzInfo);
	return dstFlag == TIME_ZONE_ID_DAYLIGHT ? -tzInfo.DaylightBias*60 : 0;
}


int Timezone::dst(const Poco::Timestamp& timestamp)
{
	if (isDst(timestamp))
	{
		TIME_ZONE_INFORMATION tzInfo;
		GetTimeZoneInformation(&tzInfo);
		return -tzInfo.DaylightBias*60;
	}
	else return 0;
}


bool Timezone::isDst(const Timestamp& timestamp)
{
	std::time_t time = timestamp.epochTime();
	struct std::tm local;
	if (localtime_s(&local, &time))
		throw Poco::SystemException("cannot get local time DST flag");
	return local.tm_isdst > 0;
}


int Timezone::utcOffset(const Poco::Timestamp& timestamp)
{
	std::time_t time = timestamp.epochTime();
	struct tm local;
	if (localtime_s(&local, &time))
		throw Poco::SystemException("cannot get UTC offset");
	struct tm utc;
	if (gmtime_s(&utc, &time))
		throw Poco::SystemException("cannot get UTC offset");
	std::time_t utctime = std::mktime(&utc);
	return time - utctime;
}


std::string Timezone::name()
{
	std::string result;
	TIME_ZONE_INFORMATION tzInfo;
	DWORD dstFlag = GetTimeZoneInformation(&tzInfo);
	WCHAR* ptr = dstFlag == TIME_ZONE_ID_DAYLIGHT ? tzInfo.DaylightName : tzInfo.StandardName;
	UnicodeConverter::toUTF8(ptr, result);
	return result;
}


std::string Timezone::standardName()
{
	std::string result;
	TIME_ZONE_INFORMATION tzInfo;
	DWORD dstFlag = GetTimeZoneInformation(&tzInfo);
	WCHAR* ptr = tzInfo.StandardName;
	UnicodeConverter::toUTF8(ptr, result);
	return result;
}


std::string Timezone::dstName()
{
	std::string result;
	TIME_ZONE_INFORMATION tzInfo;
	DWORD dstFlag = GetTimeZoneInformation(&tzInfo);
	WCHAR* ptr = tzInfo.DaylightName;
	UnicodeConverter::toUTF8(ptr, result);
	return result;
}


} // namespace Poco
