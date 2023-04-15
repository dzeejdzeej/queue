#include <queue.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define QUEUE_SERIAL_NUMBER 0xdeadbeef

struct Queue
{
    uint32_t            serial_number;    // magic nubmer
    size_t              size_of_elem;     // size of single elem
    size_t              num_of_elems;     // actual number of elems in queue
    size_t              max_num_of_elems; // max avaliable number of elems in queue
    size_t              head;             // points out the latest added element to queue
    size_t              tail;             // points out the oldest element in queue
    Queue_Observability observability;    // struct with observability counters
    uint8_t             buffer[];         // buffer as a flexible array member
};

Queue* queue_new(const size_t max_num_of_elems, const size_t size_of_elem)
{
    if (max_num_of_elems == 0)
    {
        return NULL;
    }
    
    if (size_of_elem == 0)
    {
        return NULL;
    }
    
    // allocate memory for whole structure and specified size of FAM buffer
    Queue* const queue = calloc(1, sizeof(*queue) + (max_num_of_elems * size_of_elem));
    if (queue == NULL)
    {
        return NULL;
    }

    queue->max_num_of_elems = max_num_of_elems;
    queue->size_of_elem     = size_of_elem;
    queue->num_of_elems     = 0;
    queue->head             = 0;
    queue->tail             = 0;
    queue->observability    = (Queue_Observability){0};

    // if everything is ok, at the end fill the serial/magic number
    queue->serial_number = QUEUE_SERIAL_NUMBER;

    return queue;
}

void queue_delete(Queue* const queue)
{
    if (queue == NULL)
    {
        return;
    }

    free(queue);
}

/*
    When queue exist and have allocated memory return true, otherwise false
*/
bool queue_is_active(const Queue* const queue)
{
    if (queue == NULL)
    {
        return false;
    }
    
    return queue->serial_number == QUEUE_SERIAL_NUMBER;
}

bool queue_is_full(const Queue* const queue)
{   
    if(queue_is_active(queue) == false)
    {
        return false;
    }

    return queue->num_of_elems == queue->max_num_of_elems;
}

bool queue_is_empty(const Queue* const queue)
{   
    if(queue_is_active(queue) == false)
    {
        return true;
    }

    return queue->num_of_elems == 0;
}

/*
    Returns true if enqueue operation has succesed.
    Queue and elem memory space don't overcome each other.
    In normal case, when compiler see two different types, it applicable restrict to them
    But when one of them is void* compiler will not do that, so we need to point it out directly
*/
bool queue_enqueue(Queue* const queue, const void* const restrict elem)
{
    if (queue_is_active(queue) == false)
    {
        return false;
    }

    queue->observability.no_of_enqueue++;

    if (elem == NULL)
    {
        return false;
    }

    if (queue_is_full(queue))
    {
        queue->observability.no_of_enqueue_when_full++;
        return false;
    }

    // const pointer to buffer index for current element in queue
    // multiple by elem_size since type is not fixed
    uint8_t* const elem_in_queue = &queue->buffer[queue->head * queue->size_of_elem];
    memcpy(elem_in_queue, elem, queue->size_of_elem);

    // queue can wrap up
    queue->head++;
    if (queue->head >= queue->max_num_of_elems)
    {
        queue->head = 0;
    }
    queue->num_of_elems++;

    queue->observability.no_of_successful_enqueue++;
    return true;
}

bool queue_dequeue(Queue* const queue, void* const restrict elem)
{
    if (queue_is_active(queue) == false)
    {
        return false;
    }

    queue->observability.no_of_dequeue++;

    if (queue_is_empty(queue) == true)
    {
        queue->observability.no_of_dequeue_when_empty++;
        return false;
    }

    if (elem == NULL)
    {
        return false;
    }

    // copy last elem of queue to passed pointer
    const uint8_t* const elem_in_queue = &queue->buffer[queue->tail * queue->size_of_elem];
    memcpy(elem, elem_in_queue, queue->size_of_elem);

    queue->tail++;
    if (queue->tail >= queue->max_num_of_elems)
    {
        queue->tail = 0;
    }
    queue->num_of_elems--;

    queue->observability.no_of_successful_dequeue++;
    return true;
}

/*
    Get queue observabitlity counters.
    Return by value since we don't allow to modify orginal counters
*/
Queue_Observability get_queue_observability(const Queue* queue)
{
    if (queue_is_active(queue) == false)
    {
        return (Queue_Observability){0};
    }

    return queue->observability;
}