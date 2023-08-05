/*
File:   debug.cpp
Author: Taylor Robbins
Date:   08\05\2023
*/

void WriteToStdOutAndDebugOut(const char* line, bool doNewLine)
{
	printf("%s%s", line, doNewLine ? "\n" : "");
	#if WINDOWS_COMPILATION
	OutputDebugStringA(line);
	if (doNewLine) { OutputDebugStringA("\n"); }
	#endif
}

void WriteLine_D(const char* line)
{
	WriteToStdOutAndDebugOut(line, true);
}
void PrintLine_D(const char* formatStr, ...)
{
	char printBuffer[512];
	va_list args;
	va_start(args, formatStr);
	int printResult = MyVaListPrintf(&printBuffer[0], sizeof(printBuffer), formatStr, args);
	va_end(args);
	if (printResult >= 0)
	{
		WriteToStdOutAndDebugOut(&printBuffer[0], true);
	}
}
