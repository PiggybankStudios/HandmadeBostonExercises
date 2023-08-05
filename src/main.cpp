/*
File:   main.cpp
Author: Taylor Robbins
Date:   08\04\2023
Description: 
	** The main entry point, and the one file that needs to be compiled (we include all other files from this one) 
*/

#include "gylib/gy_defines_check.h"

#include <Windows.h>

#define GYLIB_LOOKUP_PRIMES_10
#include "gylib/gy.h"

#include "arena.h"

#include "debug.cpp"

int main()
{
	Arena* mainArena = ArenaAlloc(Megabytes(1));
	ArenaSetAutoAlign(mainArena, Kilobytes(1));
	
	PrintLine_D("mainArena: %p (%p)", mainArena, mainArena->base);
	PrintLine_D("ArenaPush(64): %p", ArenaPush(mainArena, 64));
	PrintLine_D("ArenaPush(3): %p", ArenaPush(mainArena, 3));
	PrintLine_D("ArenaPushAligner(4k): %p", ArenaPushAligner(mainArena, Kilobytes(4)));
	PrintLine_D("ArenaPush(32): %p", ArenaPush(mainArena, 32));
	
	return 0;
}

#include "arena.cpp"
