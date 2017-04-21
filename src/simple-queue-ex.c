/* simple-queue-ex.c -- simple use of lcfqueue.h. */
#ifndef _STDIO_H
  #include <stdio.h>
#endif
#ifndef _STDLIB_H
  #include <stdlib.h>
#endif
#include "lcfqueue.h"

int main(void) {
  printf("\nInitializing queue test...\n\n");
  
  Queue q;
  initQueue(&q);
  Node nd;
  int tempLength;
  
  for (int i = 0; i < 25; i++) {
    if (enqueue(&q, i)) printf("Enqueued %d.\n",i);
      else printf("Failed on allocate memory.\n");
  }
  
  printf("\nQueue current has %d elements.\nProceeding to dequeue them.\n\n", q.length);
  
  tempLength = q.length;
  for (int j = 0; j < tempLength; j++) {
    printf("Dequeuing node %d... ", j);
    nd = dequeue(&q);
    printf("Element %d value is %d. Queue has %d remaining elements.\n", j, nd.value, q.length);
  }
  printf("\n");
  
  if (isQueueEmpty(&q)) printf("Queue is empty.\n");
    else printf("Queue is not empty. Queue has %d elements.\n", q.length);

  printf("\nDone.\n");
  
  return 0;
}
