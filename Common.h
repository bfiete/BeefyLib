#pragma once

#include "CommonBase.h"
#include "String.h"

NS_BF_BEGIN

extern String vformat(const char* fmt, va_list argPtr);
extern String StrFormat(const char* fmt ...);

NS_BF_END