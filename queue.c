#include "queue.h"
#include <errno.h>
#include "queue.h"

void initQueue(char *buffer, queue_t *record, uint8_t bufferSize) {
    record->inPtr = record->outPtr = record->buffStart = buffer;
    record->buffPast = buffer + bufferSize;
    record->size = bufferSize;
    record->numElements = 0;
}

void enqueue(char character, queue_t *record) {
    uint8_t retVal = 0;
    if (record->numElements >= record->size)
        retVal = 1;
    else {
        if (record->inPtr == record->buffPast)
            record->inPtr = record->buffStart;
        *record->inPtr++ = character;
        record->numElements++;
    }
    errno = retVal;
}
char dequeue(queue_t *record){
    char retVal = 0;
    if (record->numElements == 0){
        errno = 1;
    }else{
        retVal = *record->outPtr++;
        record->numElements--;
        errno = 0;
        if (record->outPtr == record->buffPast){
            record->outPtr = record->buffStart;
        }
    }
    return retVal;
}
