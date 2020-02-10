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

void initQueue(queue_t *record, char *buffer, uint8_t bufferSize);

int enqueue(char character, queue_t *record);
int dequeue(char *character, queue_t *record);

#endif
