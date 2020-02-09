#ifndef _QUEUE_H
#define _QUEUE_H
#include <stdint.h>

typedef struct {
    char *inPtr;
    char *outPtr;
    char *buffStart;
    char *buffPast;
    uint8_t size;
    uint8_t numElements;
} queue_t;

void initQueue(char *buffer, queue_t *record, uint8_t bufferSize);

void enqueue(char character, queue_t *record);
char dequeue(queue_t *record);

#endif
