/*
File:   fuzz_main.cpp
Author: Taylor Robbins
Date:   08\05\2023
Description: 
	** None 
*/

int LLVMFuzzerTestOneInput(const void* data, int size)
{
	UNUSED(data);
	UNUSED(size);
	Arena* mainArena = ArenaAlloc(Megabytes(1));
	ArenaSetAutoAlign(mainArena, Kilobytes(1));
	
	PrintLine_D("mainArena: %p (%p)", mainArena, mainArena->base);
	PrintLine_D("ArenaPush(64): %p", ArenaPush(mainArena, 64));
	PrintLine_D("ArenaPush(3): %p", ArenaPush(mainArena, 3));
	PrintLine_D("ArenaPushAligner(4k): %p", ArenaPushAligner(mainArena, Kilobytes(4)));
	PrintLine_D("ArenaPush(32): %p", ArenaPush(mainArena, 32));
	
	return 0;
}
