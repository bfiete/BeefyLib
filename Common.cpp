#pragma warning(disable:4996)

#include "Common.h"
#include <cstdarg>

USING_NS_BF;

String Beefy::vformat(const char* fmt, va_list argPtr)
{
	// We draw the line at a 1MB string.
	const int maxSize = 1000000;

	//va_list checkArgPtr = argPtr;
	//va_start(checkArgPtr, fmt);
	int argIdx = 0;
	char* newFmt = NULL;
	char tempBuff[2048];
	char* tempBuffPtr = tempBuff;
	for (int i = 0; fmt[i] != 0; i++)
	{
		if (fmt[i] == '%')
		{

			if (fmt[i + 1] == '%')
			{
				i++;
				continue;
			}

#ifdef BF32
			bool isLongAddr = false;
#else
			bool isLongAddr = true;
#endif
			if ((fmt[i + 1] == 'l') && (fmt[i + 2] == '@'))
			{
				isLongAddr = true;
				i++;
			}

			if (fmt[i + 1] == '@')
			{
				if (newFmt == NULL)
				{
					newFmt = (char*)malloc(strlen(fmt) + 1);
					strcpy(newFmt, fmt);
					//newFmt = strdup(fmt);
				}
				newFmt[i + 1] = 's';

				const char*& argValPtr = *(const char**)((intptr*)argPtr + argIdx);

				if (isLongAddr)
				{
					int64 iVal = *(int64*)((intptr*)argPtr + argIdx);
#ifdef BF32
					argIdx++; // Takes two spots
#endif
					argValPtr = tempBuffPtr;

					int leftVal = (int)(iVal >> 32);
					if (leftVal != 0)
					{
						sprintf(tempBuffPtr, "%x", leftVal);
						tempBuffPtr += strlen(tempBuffPtr);
					}
					tempBuffPtr[0] = '\'';
					tempBuffPtr++;
					sprintf(tempBuffPtr, "%08x", (int)iVal);
					tempBuffPtr += strlen(tempBuffPtr) + 1;
				}
				else
				{
					int32 iVal = (int32)(intptr)argValPtr;
					argValPtr = tempBuffPtr;
					sprintf(tempBuffPtr, "%08x", iVal);
					tempBuffPtr += strlen(tempBuffPtr) + 1;
				}

				if (newFmt[i] == 'l')
					newFmt[i] = '+';
			}
			else
			{
				//const char*& argValPtr = va_arg(checkArgPtr, const char*);
			}

			argIdx++;
		}
	}
	if (newFmt != NULL)
	{
		Beefy::String retVal = vformat(newFmt, argPtr);
		free(newFmt);
		return retVal;
	}

	// If the string is less than 161 characters,
	// allocate it on the stack because this saves
	// the malloc/free time.
	const int bufSize = 161;
	char stackBuffer[bufSize];

	int attemptedSize = bufSize - 1;

	int numChars = 0;

#ifdef _WIN32
	numChars = _vsnprintf(stackBuffer, attemptedSize, fmt, argPtr);
#else
	numChars = vsnprintf(stackBuffer, attemptedSize, fmt, argPtr);
#endif

	if ((numChars >= 0) && (numChars <= attemptedSize))
	{
		// Needed for case of 160-character printf thing
		stackBuffer[numChars] = '\0';

		// Got it on the first try.
		return String(stackBuffer);
	}

	// Now use the heap.
	char* heapBuffer = NULL;

	while (((numChars == -1) || (numChars > attemptedSize)) &&
		(attemptedSize < maxSize))
	{
		// Try a bigger size
		attemptedSize *= 2;
		heapBuffer = (char*)realloc(heapBuffer, (attemptedSize + 1));

#ifdef _WIN32
		numChars = _vsnprintf(heapBuffer, attemptedSize, fmt, argPtr);
#else
		numChars = vsnprintf(heapBuffer, attemptedSize, fmt, argPtr);
#endif

	}

	if (numChars == -1)
	{
		free(heapBuffer);
		return "";
	}

	heapBuffer[numChars] = 0;

	Beefy::String aResult = Beefy::String(heapBuffer);

	free(heapBuffer);
	return aResult;
}

String Beefy::StrFormat(const char* fmt ...)
{
	va_list argList;
	va_start(argList, fmt);
	String aResult = vformat(fmt, argList);
	va_end(argList);

	return aResult;
}