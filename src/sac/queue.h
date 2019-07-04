/**
 * queue.h
 *
 * Creation Date: 24/06/2019
 *
 * Authors:
 * Leonardo Vencovsky (https://github.com/LeoVen)
 *
 */

/**
 * A Purely Stack Allocated Queue
 *
 * This means that the Queue has a fixed size, but it also has the advantage of
 * not requiring for manually allocating and deallocating the struct.
 *
 * Generating a collection with too big of a storage can be dangerous and might
 * quickly cause a stack overflow.
 *
 * It is recommended to not generate any Stack Allocated Collection with less
 * than 1000 internal storage.
 */

#ifndef CMC_SAC_QUEUE_H
#define CMC_SAC_QUEUE_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define SAC_QUEUE_GENERATE(PFX, SNAME, FMOD, V, SIZE)    \
    SAC_QUEUE_GENERATE_HEADER(PFX, SNAME, FMOD, V, SIZE) \
    SAC_QUEUE_GENERATE_SOURCE(PFX, SNAME, FMOD, V, SIZE)

#define SAC_QUEUE_WRAPGEN_HEADER(PFX, SNAME, FMOD, K, V, SIZE) \
    SAC_QUEUE_GENERATE_HEADER(PFX, SNAME, FMOD, V, SIZE)

#define SAC_QUEUE_WRAPGEN_SOURCE(PFX, SNAME, FMOD, K, V, SIZE) \
    SAC_QUEUE_GENERATE_SOURCE(PFX, SNAME, FMOD, V, SIZE)

/* HEADER ********************************************************************/
#define SAC_QUEUE_GENERATE_HEADER(PFX, SNAME, FMOD, V, SIZE)                      \
                                                                                  \
    /* Queue Structure */                                                         \
    typedef struct SNAME##_s                                                      \
    {                                                                             \
        /* Internal Storage */                                                    \
        V buffer[SIZE];                                                           \
                                                                                  \
        /* Current amount of elements */                                          \
        size_t count;                                                             \
                                                                                  \
        /* Index representing the front of the queue */                           \
        size_t front;                                                             \
                                                                                  \
        /* Index representing the back of the queue */                            \
        size_t rear;                                                              \
                                                                                  \
        /* Function that returns an iterator to the start of the queue */         \
        struct SNAME##_iter_s (*it_start)(struct SNAME##_s *);                    \
                                                                                  \
        /* Function that returns an iterator to the end of the queue */           \
        struct SNAME##_iter_s (*it_end)(struct SNAME##_s *);                      \
                                                                                  \
    } SNAME, *SNAME##_ptr;                                                        \
                                                                                  \
    /* Queue Iterator */                                                          \
    typedef struct SNAME##_iter_s                                                 \
    {                                                                             \
        /* Target queue */                                                        \
        struct SNAME##_s *target;                                                 \
                                                                                  \
        /* Cursor's position (index) */                                           \
        size_t cursor;                                                            \
                                                                                  \
        /* Keeps track of relative index to the iteration of elements */          \
        size_t index;                                                             \
                                                                                  \
        /* If the iterator has reached the start of the iteration */              \
        bool start;                                                               \
                                                                                  \
        /* If the iterator has reached the end of the iteration */                \
        bool end;                                                                 \
                                                                                  \
    } SNAME##_iter, *SNAME##_iter_ptr;                                            \
                                                                                  \
    /* Collection Functions */                                                    \
    /* Collection Allocation and Deallocation */                                  \
    FMOD SNAME PFX##_new(void);                                                   \
    FMOD void PFX##_clear(SNAME *_queue_);                                        \
    /* Collection Input and Output */                                             \
    FMOD bool PFX##_enqueue(SNAME *_queue_, V element);                           \
    FMOD bool PFX##_dequeue(SNAME *_queue_);                                      \
    /* Conditional Input and Output */                                            \
    FMOD bool PFX##_enqueue_if(SNAME *_queue_, V element, bool condition);        \
    FMOD bool PFX##_dequeue_if(SNAME *_queue_, bool condition);                   \
    /* Element Access */                                                          \
    FMOD V PFX##_peek(SNAME *_queue_);                                            \
    /* Collection State */                                                        \
    FMOD bool PFX##_contains(SNAME *_queue_, V element, int (*comparator)(V, V)); \
    FMOD bool PFX##_empty(SNAME *_queue_);                                        \
    FMOD bool PFX##_full(SNAME *_queue_);                                         \
    FMOD size_t PFX##_count(SNAME *_queue_);                                      \
    FMOD size_t PFX##_capacity(void);                                             \
                                                                                  \
    /* Iterator Functions */                                                      \
    /* Iterator Initialization */                                                 \
    FMOD void PFX##_iter_init(SNAME##_iter *iter, SNAME *target);                 \
    /* Iterator State */                                                          \
    FMOD bool PFX##_iter_start(SNAME##_iter *iter);                               \
    FMOD bool PFX##_iter_end(SNAME##_iter *iter);                                 \
    /* Iterator Movement */                                                       \
    FMOD void PFX##_iter_to_start(SNAME##_iter *iter);                            \
    FMOD void PFX##_iter_to_end(SNAME##_iter *iter);                              \
    FMOD bool PFX##_iter_next(SNAME##_iter *iter);                                \
    FMOD bool PFX##_iter_prev(SNAME##_iter *iter);                                \
    /* Iterator Access */                                                         \
    FMOD V PFX##_iter_value(SNAME##_iter *iter);                                  \
    FMOD V *PFX##_iter_rvalue(SNAME##_iter *iter);                                \
    FMOD size_t PFX##_iter_index(SNAME##_iter *iter);                             \
                                                                                  \
    /* Default Value */                                                           \
    static inline V PFX##_impl_default_value(void)                                \
    {                                                                             \
        V _empty_value_;                                                          \
                                                                                  \
        memset(&_empty_value_, 0, sizeof(V));                                     \
                                                                                  \
        return _empty_value_;                                                     \
    }                                                                             \
                                                                                  \
/* SOURCE ********************************************************************/
#define SAC_QUEUE_GENERATE_SOURCE(PFX, SNAME, FMOD, V, SIZE)                                          \
                                                                                                      \
    /* Implementation Detail Functions */                                                             \
    static SNAME##_iter PFX##_impl_it_start(SNAME *_queue_);                                          \
    static SNAME##_iter PFX##_impl_it_end(SNAME *_queue_);                                            \
                                                                                                      \
    FMOD SNAME PFX##_new(void)                                                                        \
    {                                                                                                 \
        SNAME result;                                                                                 \
                                                                                                      \
        memset(&result, 0, sizeof(SNAME));                                                            \
                                                                                                      \
        result.it_start = PFX##_impl_it_start;                                                        \
        result.it_end = PFX##_impl_it_end;                                                            \
                                                                                                      \
        return result;                                                                                \
    }                                                                                                 \
                                                                                                      \
    FMOD void PFX##_clear(SNAME *_queue_)                                                             \
    {                                                                                                 \
        memset(_queue_->buffer, 0, sizeof(_queue_->buffer));                                          \
                                                                                                      \
        _queue_->count = 0;                                                                           \
        _queue_->front = 0;                                                                           \
        _queue_->rear = 0;                                                                            \
    }                                                                                                 \
                                                                                                      \
    FMOD bool PFX##_enqueue(SNAME *_queue_, V element)                                                \
    {                                                                                                 \
        if (PFX##_full(_queue_))                                                                      \
            return false;                                                                             \
                                                                                                      \
        _queue_->buffer[_queue_->rear] = element;                                                     \
                                                                                                      \
        _queue_->rear = (_queue_->rear == SIZE - 1) ? 0 : _queue_->rear + 1;                          \
        _queue_->count++;                                                                             \
                                                                                                      \
        return true;                                                                                  \
    }                                                                                                 \
                                                                                                      \
    FMOD bool PFX##_dequeue(SNAME *_queue_)                                                           \
    {                                                                                                 \
        if (PFX##_empty(_queue_))                                                                     \
            return false;                                                                             \
                                                                                                      \
        _queue_->buffer[_queue_->front] = PFX##_impl_default_value();                                 \
                                                                                                      \
        _queue_->front = (_queue_->front == SIZE - 1) ? 0 : _queue_->front + 1;                       \
        _queue_->count--;                                                                             \
                                                                                                      \
        return true;                                                                                  \
    }                                                                                                 \
                                                                                                      \
    FMOD bool PFX##_enqueue_if(SNAME *_queue_, V element, bool condition)                             \
    {                                                                                                 \
        if (condition)                                                                                \
            return PFX##_enqueue(_queue_, element);                                                   \
                                                                                                      \
        return false;                                                                                 \
    }                                                                                                 \
                                                                                                      \
    FMOD bool PFX##_dequeue_if(SNAME *_queue_, bool condition)                                        \
    {                                                                                                 \
        if (condition)                                                                                \
            return PFX##_dequeue(_queue_);                                                            \
                                                                                                      \
        return false;                                                                                 \
    }                                                                                                 \
                                                                                                      \
    FMOD V PFX##_peek(SNAME *_queue_)                                                                 \
    {                                                                                                 \
        if (PFX##_empty(_queue_))                                                                     \
            return PFX##_impl_default_value();                                                        \
                                                                                                      \
        return _queue_->buffer[_queue_->front];                                                       \
    }                                                                                                 \
                                                                                                      \
    FMOD bool PFX##_contains(SNAME *_queue_, V element, int (*comparator)(V, V))                      \
    {                                                                                                 \
        for (size_t i = _queue_->front, j = 0; j < _queue_->count; i = (i + 1) % _queue_->count, j++) \
        {                                                                                             \
            if (comparator(_queue_->buffer[i], element) == 0)                                         \
                return true;                                                                          \
        }                                                                                             \
                                                                                                      \
        return false;                                                                                 \
    }                                                                                                 \
                                                                                                      \
    FMOD bool PFX##_empty(SNAME *_queue_)                                                             \
    {                                                                                                 \
        return _queue_->count == 0;                                                                   \
    }                                                                                                 \
                                                                                                      \
    FMOD bool PFX##_full(SNAME *_queue_)                                                              \
    {                                                                                                 \
        return _queue_->count >= SIZE;                                                                \
    }                                                                                                 \
                                                                                                      \
    FMOD size_t PFX##_count(SNAME *_queue_)                                                           \
    {                                                                                                 \
        return _queue_->count;                                                                        \
    }                                                                                                 \
                                                                                                      \
    FMOD size_t PFX##_capacity(void)                                                                  \
    {                                                                                                 \
        return SIZE;                                                                                  \
    }                                                                                                 \
                                                                                                      \
    FMOD void PFX##_iter_init(SNAME##_iter *iter, SNAME *target)                                      \
    {                                                                                                 \
        iter->target = target;                                                                        \
        iter->cursor = target->front;                                                                 \
        iter->index = 0;                                                                              \
        iter->start = true;                                                                           \
        iter->end = PFX##_empty(target);                                                              \
    }                                                                                                 \
                                                                                                      \
    FMOD bool PFX##_iter_start(SNAME##_iter *iter)                                                    \
    {                                                                                                 \
        return PFX##_empty(iter->target) || iter->start;                                              \
    }                                                                                                 \
                                                                                                      \
    FMOD bool PFX##_iter_end(SNAME##_iter *iter)                                                      \
    {                                                                                                 \
        return PFX##_empty(iter->target) || iter->end;                                                \
    }                                                                                                 \
                                                                                                      \
    FMOD void PFX##_iter_to_start(SNAME##_iter *iter)                                                 \
    {                                                                                                 \
        iter->cursor = iter->target->front;                                                           \
        iter->index = 0;                                                                              \
        iter->start = true;                                                                           \
        iter->end = PFX##_empty(iter->target);                                                        \
    }                                                                                                 \
                                                                                                      \
    FMOD void PFX##_iter_to_end(SNAME##_iter *iter)                                                   \
    {                                                                                                 \
        if (PFX##_empty(iter->target))                                                                \
            iter->cursor = 0;                                                                         \
        else                                                                                          \
            iter->cursor = (iter->target->rear == 0) ? SIZE - 1 : iter->target->rear - 1;             \
                                                                                                      \
        iter->index = iter->target->count - 1;                                                        \
        iter->start = PFX##_empty(iter->target);                                                      \
        iter->end = true;                                                                             \
    }                                                                                                 \
                                                                                                      \
    FMOD bool PFX##_iter_next(SNAME##_iter *iter)                                                     \
    {                                                                                                 \
        if (iter->end)                                                                                \
            return false;                                                                             \
                                                                                                      \
        iter->start = PFX##_empty(iter->target);                                                      \
                                                                                                      \
        if (iter->index == iter->target->count - 1)                                                   \
            iter->end = true;                                                                         \
        else                                                                                          \
        {                                                                                             \
            iter->cursor = (iter->cursor + 1) % (SIZE);                                               \
            iter->index++;                                                                            \
        }                                                                                             \
                                                                                                      \
        return true;                                                                                  \
    }                                                                                                 \
                                                                                                      \
    FMOD bool PFX##_iter_prev(SNAME##_iter *iter)                                                     \
    {                                                                                                 \
        if (iter->start)                                                                              \
            return false;                                                                             \
                                                                                                      \
        iter->end = PFX##_empty(iter->target);                                                        \
                                                                                                      \
        if (iter->index == 0)                                                                         \
            iter->start = true;                                                                       \
        else                                                                                          \
        {                                                                                             \
            iter->cursor = (iter->cursor == 0) ? SIZE - 1 : iter->cursor - 1;                         \
            iter->index--;                                                                            \
        }                                                                                             \
                                                                                                      \
        return true;                                                                                  \
    }                                                                                                 \
                                                                                                      \
    FMOD V PFX##_iter_value(SNAME##_iter *iter)                                                       \
    {                                                                                                 \
        if (PFX##_empty(iter->target))                                                                \
            return PFX##_impl_default_value();                                                        \
                                                                                                      \
        return iter->target->buffer[iter->cursor];                                                    \
    }                                                                                                 \
                                                                                                      \
    FMOD V *PFX##_iter_rvalue(SNAME##_iter *iter)                                                     \
    {                                                                                                 \
        if (PFX##_empty(iter->target))                                                                \
            return NULL;                                                                              \
                                                                                                      \
        return &(iter->target->buffer[iter->cursor]);                                                 \
    }                                                                                                 \
                                                                                                      \
    FMOD size_t PFX##_iter_index(SNAME##_iter *iter)                                                  \
    {                                                                                                 \
        return iter->index;                                                                           \
    }                                                                                                 \
                                                                                                      \
    static SNAME##_iter PFX##_impl_it_start(SNAME *_queue_)                                           \
    {                                                                                                 \
        SNAME##_iter iter;                                                                            \
                                                                                                      \
        PFX##_iter_init(&iter, _queue_);                                                              \
        PFX##_iter_to_start(&iter);                                                                   \
                                                                                                      \
        return iter;                                                                                  \
    }                                                                                                 \
                                                                                                      \
    static SNAME##_iter PFX##_impl_it_end(SNAME *_queue_)                                             \
    {                                                                                                 \
        SNAME##_iter iter;                                                                            \
                                                                                                      \
        PFX##_iter_init(&iter, _queue_);                                                              \
        PFX##_iter_to_end(&iter);                                                                     \
                                                                                                      \
        return iter;                                                                                  \
    }

#endif /* CMC_SAC_QUEUE_H */
