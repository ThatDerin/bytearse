#include <stdlib.h>

#include "chunk.h"
#include "memory.h"

void initChunk(Chunk* chunk)
{
    chunk->useCount = 0; // Used for keeping count of objects being used.
    chunk->capacity = 0; // Max capacity of the chunk array.
    chunk->code = NULL;
    chunk->lines = NULL;
    initValueArray(&chunk->constants);
}

void freeChunk(Chunk* chunk)
{
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    FREE_ARRAY(int, chunk->lines, chunk->capacity);
    freeValueArray(&chunk->constants);
    initChunk(chunk);
}

void writeChunk(Chunk* chunk, uint8_t byte, int line)
{
    // If there is no available space at current Chunk
    if (chunk->capacity < chunk->useCount + 1)
    {
        // Grows capacity based on current capacity.
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);

        chunk->code = GROW_ARRAY(uint8_t, chunk->code,
            oldCapacity, chunk->capacity);

        chunk->lines = GROW_ARRAY(int, chunk->lines, oldCapacity, chunk->capacity);

    }

    chunk->code[chunk->useCount] = byte;
    chunk->lines[chunk->useCount] = line;
    chunk->useCount++;
}

int addConstant(Chunk* chunk, Value value)
{
    writeValueArray(&chunk->constants, value);
    return chunk->constants.count - 1;
}