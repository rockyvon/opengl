#pragma once
#include "stdafx.h"

static int stringReplace(string &str, const string &before, const string &after)
{
	string::size_type pos = 0;
	string::size_type a = before.size();
	string::size_type b = after.size();
	while ((pos = str.find(before, pos)) != string::npos)
	{
		str.replace(pos, a, after);
		pos += b;
	}
	return 0;
}
