#include "stdafx.h"
#include "timeutils.h"
#include  <time.h>


TimeUtils::TimeUtils()
{
}

TimeUtils::~TimeUtils()
{
}

string TimeUtils::getCompleteTime()
{
	time_t now;
	struct tm tm_now;
	time(&now);
	localtime_s(&tm_now, &now);

	string result = to_string(tm_now.tm_year + 1900)
		+ (tm_now.tm_mon < 9 ? "0" + to_string(tm_now.tm_mon + 1) : to_string(tm_now.tm_mon + 1))
		+ (tm_now.tm_mday < 10 ? "0" + to_string(tm_now.tm_mday) : to_string(tm_now.tm_mday))
		+ "_"
		+ (tm_now.tm_hour < 10 ? "0" + to_string(tm_now.tm_hour) : to_string(tm_now.tm_hour))
		+ (tm_now.tm_min < 10 ? "0" + to_string(tm_now.tm_min) : to_string(tm_now.tm_min))
		+ (tm_now.tm_sec < 10 ? "0" + to_string(tm_now.tm_sec) : to_string(tm_now.tm_sec));
	return result;

}

