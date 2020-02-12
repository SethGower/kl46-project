#include "queue.h"

void initQueue(queue_t *record, char *buffer, uint8_t bufferSize) {
    record->inPtr = buffer;
    record->outPtr = buffer;
    record->buffStart = buffer;
    record->buffPast = buffer + bufferSize;
    record->size = bufferSize;
    record->numElements = 0;
}

int enqueue(char character, queue_t *record) {
    if (record->numElements < record->size) {
        *record->inPtr++ = character;
        if (record->inPtr >= record->buffPast) {
            record->inPtr = record->buffStart;
        }
        record->numElements++;
    } else {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int dequeue(char *character, queue_t *record) {
    if (record->numElements != 0) {
        *character = *record->outPtr++;
        if (record->outPtr >= record->buffPast) {
            record->outPtr = record->buffStart;
        }
        record->numElements--;
    } else {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
