#include <queue.h>
#include <assert.h>

static void queue_constructor_test(void);
static void queue_is_active_test(void);
static void queue_is_empty_test(void);
static void queue_is_full_test(void);
static void queue_enqueue_test(void);
static void queue_dequeue_test(void);
static void queue_all_functionalities_test(void);

int main(void)
{
    queue_constructor_test();
    queue_is_active_test();
    queue_is_empty_test();
    queue_is_full_test();
    queue_enqueue_test();
    queue_dequeue_test();
    queue_all_functionalities_test();

    return 0;
}

static void queue_constructor_test(void)
{
    // TEST queue_new returns NULL when max number of elems equals zero
    {
        register const size_t max_num_of_elems = 0;
        register const size_t size_of_elem = sizeof(int);

        Queue* const q = queue_new(max_num_of_elems, size_of_elem);
        assert(q == NULL);

        queue_delete(q);
    }

    // TEST queue_new returns NULL when size of element equals zero
    {
        register const size_t max_num_of_elems = 10;
        register const size_t size_of_elem = 0;

        Queue* const q = queue_new(max_num_of_elems, size_of_elem);
        assert(q == NULL);

        queue_delete(q);
    }

    // TEST queue_new retunrs NULL when size of elem and number of elems are zeros
    {
        register const size_t max_num_of_elems = 0;
        register const size_t size_of_elem = 0;

        Queue* const queue = queue_new(max_num_of_elems, size_of_elem);
        assert(queue == NULL);

        queue_delete(queue);
    }

    // TEST queue_new returns proper pointer when max number nad size of elems are positivie
    {
        register const size_t max_num_of_elems = 10;
        register const size_t size_of_elem = sizeof(int);

        Queue* const queue = queue_new(max_num_of_elems, size_of_elem);
        assert(queue != NULL);

        queue_delete(queue);
    }
}

static void queue_is_active_test(void)
{
    assert(queue_is_active(NULL) == false);

    register const size_t max_num_of_elems = 10;
    register const size_t size_of_elem = sizeof(int);
    Queue* const queue = queue_new(max_num_of_elems, size_of_elem);
    
    assert(queue != NULL);
    assert(queue_is_active(queue) == true);

    queue_delete(queue);
}

static void queue_is_empty_test(void)
{
    assert(queue_is_empty(NULL) == true);

    register const size_t max_num_of_elems = 10;
    register const size_t size_of_elem = sizeof(int);
    Queue* const queue = queue_new(max_num_of_elems, size_of_elem);

    assert(queue != NULL);
    assert(queue_is_empty(queue) == true);

    queue_delete(queue);
}

static void queue_is_full_test(void)
{
    assert(queue_is_full(NULL) == false);

    register const size_t max_num_of_elems = 10;
    register const size_t size_of_elem = sizeof(int);
    Queue* const queue = queue_new(max_num_of_elems, size_of_elem);

    assert(queue != NULL);
    assert(queue_is_full(queue) == false);

    queue_delete(queue);
}

static void queue_enqueue_test(void)
{
    // both pointers wrong
    {
        assert(queue_enqueue(NULL, NULL) == false);
    }

    // one pointer wrong (lack of queue)
    {
        const int value = 1;
        assert(queue_enqueue(NULL, &value) == false);
    }

    // one pointer wrong (lack of elem)
    {
        register const size_t max_no_of_elems = 10;
        register const size_t elem_size = sizeof(int);
        Queue* queue = queue_new(max_no_of_elems, elem_size);

        assert(queue_enqueue(queue, NULL) == false);

        queue_delete(queue);
    }

    // both passed arguments OK
    {
        register const size_t max_no_of_elems = 10;
        register const size_t elem_size = sizeof(int);
        Queue* queue = queue_new(max_no_of_elems, elem_size);

        const int value = 5;
        assert(queue_enqueue(queue, &value) == true);

        queue_delete(queue);
    }

    // try to add elem to full queue
    {
        register const size_t max_no_of_elems = 1;
        register const size_t elem_size = sizeof(int);
        Queue* queue = queue_new(max_no_of_elems, elem_size);

        const int value1 = 5;
        assert(queue_enqueue(queue, &value1) == true);

        assert(queue_is_full(queue) == true);

        const int value2 = 3;
        assert(queue_enqueue(queue, &value2) == false);

        queue_delete(queue);
    }
}

static void queue_dequeue_test(void)
{
    // both pointers wrong
    {
        assert(queue_dequeue(NULL, NULL) == false);
    }

    // one pointer wrong (lack of queue)
    {
        int elem = 0;
        assert(queue_dequeue(NULL, &elem) == false);
    }

    // one pointer wrong (lack of elem)
    {
        register const size_t max_no_of_elems = 10;
        register const size_t elem_size = sizeof(int);
        Queue* queue = queue_new(max_no_of_elems, elem_size);

        assert(queue_dequeue(queue, NULL) == false);

        queue_delete(queue);
    }

    // try dequeue on empty queue
    {
        register const size_t max_no_of_elems = 10;
        register const size_t elem_size = sizeof(int);
        Queue* queue = queue_new(max_no_of_elems, elem_size);
        int elem = 0;

        assert(queue_is_empty(queue) == true);
        assert(queue_dequeue(queue, &elem) == false);

        queue_delete(queue);
    }

    // succesfull dequeue
    {
        register const size_t max_no_of_elems = 10;
        register const size_t elem_size = sizeof(int);
        Queue* queue = queue_new(max_no_of_elems, elem_size);

        const int value1 = 5;
        assert(queue_enqueue(queue, &value1) == true);
        const int value2 = 3;
        assert(queue_enqueue(queue, &value2) == true);

        int elem = 0;
        assert(queue_dequeue(queue, &elem) == true);
        assert(elem == 5);

        queue_delete(queue);
    }  
}

static void queue_all_functionalities_test(void)
{
    // Add 1 element to queue which size is 10.
    {
        register const size_t max_no_of_elems = 10;
        register const size_t elem_size = sizeof(int);
        Queue* queue = queue_new(max_no_of_elems, elem_size);

        assert(queue_is_active(queue) == true);
        assert(queue_is_empty(queue) == true);
        assert(queue_is_full(queue) == false);

        const int value = 13;
        assert(queue_enqueue(queue, &value) == true);

        assert(queue_is_empty(queue) == false);
        assert(queue_is_full(queue) == false);

        const Queue_Observability counters = get_queue_observability(queue);   
        assert(counters.no_of_enqueue == 1);
        assert(counters.no_of_enqueue_when_full == 0);
        assert(counters.no_of_successful_enqueue == 1);

        queue_delete(queue);
    }

    // Add 1 element to queue which size is 1.
    {
        register const size_t max_no_of_elems = 1;
        register const size_t elem_size = sizeof(int);
        Queue* queue = queue_new(max_no_of_elems, elem_size);

        assert(queue_is_empty(queue) == true);
        assert(queue_is_full(queue) == false);

        const int value = 13;
        assert(queue_enqueue(queue, &value) == true);

        assert(queue_is_empty(queue) == false);
        assert(queue_is_full(queue) == true);

        const Queue_Observability counters = get_queue_observability(queue);
        assert(counters.no_of_enqueue == 1);
        assert(counters.no_of_enqueue_when_full == 0);
        assert(counters.no_of_successful_enqueue == 1);

        queue_delete(queue);
    }

        // Try to add 2 elements to queue which size is 1.
    {
        register const size_t max_no_of_elems = 1;
        register const size_t elem_size = sizeof(int);
        Queue* queue = queue_new(max_no_of_elems, elem_size);

        assert(queue_is_empty(queue) == true);
        assert(queue_is_full(queue) == false);

        const int value1 = 13;
        assert(queue_enqueue(queue, &value1) == true);

        assert(queue_is_empty(queue) == false);
        assert(queue_is_full(queue) == true);

        const int value2 = 7;
        assert(queue_enqueue(queue, &value2) == false);

        assert(queue_is_empty(queue) == false);
        assert(queue_is_full(queue) == true);

        const Queue_Observability counters = get_queue_observability(queue);
        assert(counters.no_of_enqueue == 2);
        assert(counters.no_of_enqueue_when_full == 1);
        assert(counters.no_of_successful_enqueue == 1);

        queue_delete(queue);
    }

    // Check enqueue and dequeue functionalities, fill whole queue and then release 
    {
        register const size_t max_no_of_elems = 5;
        register const size_t elem_size = sizeof(int);
        Queue* queue = queue_new(max_no_of_elems, elem_size);

        // at first try to dequeue the empty queue
        assert(queue_is_empty(queue) == true);
        int elem = 0;
        assert(queue_dequeue(queue, &elem) == false);
        assert(elem == 0);

        // try to add 6 elems to queue with 5 spots
        const int elem_array[6] = {1, 0, 3, 8, 9, 13};
        for (int i = 0; i < 5; ++i)
        {
            assert(queue_enqueue(queue, &elem_array[i]) == true);
        }
        assert(queue_is_full(queue) == true);
        assert(queue_enqueue(queue, &elem_array[5]) == false);

        //dequeue all elems
        for (int i = 0; i < 5; ++i)
        {
            int val = 0;
            assert(queue_dequeue(queue, &val) == true);
            assert(val == elem_array[i]);
        }

        // now try to add last elem of array
        assert(queue_enqueue(queue, &elem_array[5]) == true);

        // check the observability counters
        const Queue_Observability counters = get_queue_observability(queue); 
        assert(counters.no_of_enqueue == 7); // all queue_enqueue() calls
        assert(counters.no_of_enqueue_when_full == 1); // one try of enqueue to full queue
        assert(counters.no_of_successful_enqueue == 6); // total 6 successfuly enqueue() calls
        assert(counters.no_of_dequeue == 6); // all queue_dequeue() calls
        assert(counters.no_of_dequeue_when_empty == 1); // one try of dequeue from empty queue
        assert(counters.no_of_successful_dequeue == 5); // total 5 successfuly dequeue() calls

        queue_delete(queue);
    }

}