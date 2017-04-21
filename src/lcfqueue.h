/* lcfqueue.h -- A simple and effective FIFO/Queue implementation that can work with any type of data. */
#ifndef LCFQUEUE_H_
#define LCFQUEUE_H_

#ifndef _STDBOOL_H
#include <stdbool.h> /* Requires C99 standard or newer.       */
/* Probably your compiler uses a newer one already, so chill. */
/* Case it don't, throw -std=c11 at your gcc params.          */
/* If you don't use gcc, don't ask ME. Google it.             */
#endif

/* --- Type to be used in the queue --- */
#ifndef VAL_TYPE
#define VAL_TYPE int //Default value. See below how to easily change it.
#endif
// Simply go to your source file and, before you include this header file, use
// #define VAL_TYPE you_desired_type then include this header file. Here is an example:
/* my_source_file.c -- implements lcfqueue in a custom way
#define VAL_TYPE float
#include "lcfqueue.h"
...done, you are good to go coding...
// Yes, that simple. =P
// Or, if you want to use a custom structure, just call #include "lcfqueue.h" after
// creating your type and using #define on it. Here is an example:
#define VAL_TYPE MyCustomType
typedef struct my_custom_type {
  char name[21];
  float value;
} MyCustomType;
#inculde "lcfqueue.h"
...done, now go do some coding...
*/
// That is it. You have a fully operational queue for anything you need. Fun!
// You could even do #define VAL_TYPE MyCustomType* for holding a pointer-to-MyCustomType
// Just remember to malloc() and free() properly if doing a custom like this.


/* -- Type definitions -- */

// Node/element definition
typedef struct queue_elem {
  VAL_TYPE value; // This can be anything. Really.
  struct queue_elem * next;
  struct queue_elem * prev;
} QElem;

// Queue definition
typedef struct queue {
  QElem *head;
  QElem *tail;
  int length;
} Queue;


/* -- Function prototypes and how to -- */

// Initializer
Queue * newQueue();
/* operation:          Initializes a queue.                   */
/* preconditions:      Use like this: Queue * q = newQueue(); */
/* postconditions:     A empty Queue is initialized on *q     */

// Queue Element Initializer
QElem * newQElem(const VAL_TYPE, const QElem *, const QElem *);
/* operation:        Initializes a Queue element holding VAL_TYPE                         */
/* preconditions:    A initialized Queue. This function is called from enqueue(VAL_TYPE); */
/*                   not manually. If you wanna use it, ok, but... really?                */
/* postconditions:   When properly called within enqueue(VAL_TYPE), the new QElem is at   */
/*                   the queue's tail.                                                    */

// Empty Queue Element Initializer
QElem * newEmptyQElem();
/* operation:        Initializes a Queue Element with no linking or VAL_TYPE data.   */
/* preconditions:    Use like this: QElem elem = newEmptyQElem();                    */
/* postconditions:   A empty QElem which is not tied to any Queue or holds any data. */

// Push procedure
bool enqueue(Queue *, const VAL_TYPE);
/* operation:        Push a new element to the end of the queue with the specified  */
/*                   value.                                                         */
/* preconditions:    A pointer to a initialized queue and the VAL_TYPE data.        */
/* postconditions:   A new element is pushed into the queue with value set to val   */
/*                   and returns true on success and false on failure in pushing.   */

// Pop procedure
VAL_TYPE dequeue(Queue *);
/* operation:         Pop the element at the head of pq and return its VAL_TYPE.      */
/* preconditions:     A initialized not empty queue, and a var or pointer to VAL_TYPE */
/*                    in the caller function to hold the returned VAL_TYPE data.      */
/* postconditions:    Return the poped VAL_TYPE data or NULL if the queue was empty   */
/*                    and points head to the next element.                            */

// Emptiness verification
bool isQueueEmpty(const Queue * pq);
/* operation:            Determines if there are elements on the queue.           */
/* preconditions:        pq is a initialized queue.                               */
/* postconditions:       Returns true if empty and false if there is at least one */
/*                       element                                                  */



/* --- Function actual implementation --- */

// Initializer -- throws zero/empty at everything.
Queue * newQueue()
{
  Queue * q = (Queue *)malloc(sizeof(Queue));
  q->head = q->tail = 0;
  q->length = 0;
  return q;
}

// Queue Element Initializer, not empty
QElem * newQElem(const VAL_TYPE val, const QElem * p, const QElem * n) {
  // Alloc the needed memory and ge the adress.
  QElem * elem = (QElem *)malloc(sizeof(QElem));
  
  // If malloc() failed, elem is null and there is nothing we can do except abort and return it.
  if (elem == NULL) return elem;
  
  // If malloc() went ok, we set up the data for elem and then return it.
  elem->value = val;
  elem->next = n;
  elem->prev = p;
  
  // Elem is a pointer, so we simply return it as it is.
  return elem;
}

// Queue Element Initializer, empty version
QElem * newEmptyQElem() {
  // Just return the allocated address.
  return (QElem *)malloc(sizeof(QElem));
}

// Push operation
bool enqueue(Queue * q, const VAL_TYPE val) {
  // Creates new QElem, with no next and current tail as it's elem->prev, holding the pointer-to-ELEM_TYPE in it's elem->value.
  // If queue was empty, this element receives prev->0 and next->0, which is the desired result for such situation.
  QElem * elem = newQElem(val, q->tail, 0);
  
  // If malloc failed, elem is NULL and we must abort.
  if (elem == NULL) return false;
  
  // If the queue was empty, this new element is both tail and head.
  if (q->length == 0) {
    q->tail = q->head = elem;
    q->length++;
    return true;
  }
  
  // If we reached this point, elem is fine and the queue has at least one element(current tail) that shall come before this new one.
  
  // Current tail has a elem->next now and it is this new element.
  q->tail->next = elem;
  
  // Now we change the current tail to this new element.
  q->tail = elem;
  
  // Increment queue length counter
  q->length += 1;
  
  // Done. Everything went fine.
  return true;
}

// Pop operation
VAL_TYPE dequeue(Queue * q) {
  // If queue is empty, there is nothing to dequeue
  if (q->length == 0) return 0;

  // Retrieve the pointer stored in the head element of the queue and put on temporary variable.
  VAL_TYPE retVal = q->head->value;
  
  if (q->length == 1){
    // Free current head, which is also the tail, and set them to 0;
    free(q->head);
    q->head = q->tail = 0;
  }
  else {
    // There are still at least one element besides this one being poped now. Lets point head to it.
    q->head = q->head->next;
    // Free memory used by the previous head element by using it's current head->prev address.
    free(q->head->prev);
  }
  
  // This is not actually necessary, so we commented.
  // q->head->prev = 0;
  
  // Decrement queue length
  q->length -= 1;
  
  // Return the dequeued value.
  return retVal;
}

// Empty? -- simple true/false for emptiness checking.
bool isQueueEmpty(const Queue * q) {
  if (q->length == 0) return true;
  return false;
}

#endif
