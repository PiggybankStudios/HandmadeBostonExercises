/*
File:   arena.cpp
Author: Taylor Robbins
Date:   08\04\2023
Description: 
	** None 
*/

Arena globalArenasBuffer[32];

Arena* ArenaAlloc(u64 cap)
{
	Arena* result = nullptr;
	for (u64 aIndex = 0; aIndex < ArrayCount(globalArenasBuffer); aIndex++)
	{
		if (!globalArenasBuffer[aIndex].initialized)
		{
			result = &globalArenasBuffer[aIndex];
			break;
		}
	}
	Assert(result != nullptr);
	ClearPointer(result);
	result->initialized = true;
	result->base = malloc(cap);
	result->used = 0;
	result->size = cap;
	return result;
}

void ArenaRelease(Arena* arena)
{
	NotNull2(arena, arena->base);
	free(arena->base);
	ClearPointer(arena);
}

void ArenaSetAutoAlign(Arena* arena, u64 align)
{
	NotNull2(arena, arena->base);
	arena->autoAlign = align;
}

u64 ArenaPos(Arena* arena)
{
	NotNull2(arena, arena->base);
	return arena->used;
}

void* ArenaPushInternal(Arena* arena, u64 size, bool autoAlign)
{
	NotNull2(arena, arena->base);
	Assert(arena->used + size <= arena->size);
	if (autoAlign && arena->autoAlign > 0)
	{
		ArenaPushAligner(arena, arena->autoAlign);
	}
	void* result = ((u8*)arena->base) + arena->used;
	arena->used += size;
	return result;
}

void* ArenaPushNoZero(Arena* arena, u64 size)
{
	return ArenaPushInternal(arena, size, true);
}

void* ArenaPushAligner(Arena* arena, u64 alignment)
{
	NotNull2(arena, arena->base);
	u64 currentAddress = ((u64)arena->base) + arena->used;
	u64 alignmentNeeded = alignment - (currentAddress % alignment);
	if (alignmentNeeded == alignment) { alignmentNeeded = 0; }
	return ArenaPushInternal(arena, alignmentNeeded, false);
}

void* ArenaPush(Arena* arena, u64 size)
{
	NotNull2(arena, arena->base);
	void* result = ArenaPushNoZero(arena, size);
	MyMemSet(result, 0x00, size);
	return result;
}

void ArenaPopTo(Arena* arena, u64 pos)
{
	NotNull2(arena, arena->base);
	Assert(pos <= arena->size);
	arena->used = pos;
}

void ArenaPop(Arena* arena, u64 size)
{
	NotNull2(arena, arena->base);
	Assert(arena->used >= size);
	arena->used -= size;
}

void ArenaClear(Arena* arena)
{
	NotNull2(arena, arena->base);
	arena->used = 0;
}
