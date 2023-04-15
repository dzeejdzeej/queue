#ifndef QUEUE_H
#define QUEUE_H

#include <stdint.h> // unit_8
#include <stddef.h> // size_t
#include <stdbool.h>

typedef struct Queue_Observability
{
    size_t no_of_enqueue;               // sum of every enqueue call
    size_t no_of_enqueue_when_full;     // sum of enqueue calls when queue is full
    size_t no_of_successful_enqueue;    // sum of successful enqueue calls
    size_t no_of_dequeue;               // sum of every dequeue call
    size_t no_of_dequeue_when_empty;    // sum of dequeue calls when queue is empty
    size_t no_of_successful_dequeue;    // sum of successful dequeue calls

} Queue_Observability;

typedef struct Queue Queue;

Queue*   queue_new(size_t max_num_of_elems, size_t size_of_elem);
void     queue_delete(Queue* queue);
bool     queue_enqueue(Queue* queue, const void* restrict elem);
bool     queue_dequeue(Queue* queue, void* restrict elem);
bool     queue_is_active(const Queue* queue);
bool     queue_is_full(const Queue* queue);
bool     queue_is_empty(const Queue* queue);

Queue_Observability get_queue_observability(const Queue* queue);

#endif