/*
File:   arena.h
Author: Taylor Robbins
Date:   08\04\2023
*/

#ifndef _ARENA_H
#define _ARENA_H

struct Arena
{
	bool initialized;
	u64 autoAlign;
	void* base;
	u64 used;
	u64 size;
};

Arena* ArenaAlloc(u64 cap);
void ArenaRelease(Arena* arena);
void ArenaSetAutoAlign(Arena* arena, u64 align);

u64 ArenaPos(Arena* arena);

void* ArenaPushNoZero(Arena* arena, u64 size);
void* ArenaPushAligner(Arena* arena, u64 alignment);
void* ArenaPush(Arena* arena, u64 size);

void ArenaPopTo(Arena* arena, u64 pos);
void ArenaPop(Arena* arena, u64 size);
void ArenaClear(Arena* arena);

#endif //  _ARENA_H
