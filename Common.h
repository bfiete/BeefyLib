#pragma once

#include "CommonBase.h"
#include "String.h"
#include <stdarg.h>

NS_BF_BEGIN

extern String vformat(const char* fmt, va_list argPtr);
extern String StrFormat(const char* fmt ...);
extern void OutputDebugStrF(const char* fmt ...);

NS_BF_END