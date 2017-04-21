/* bfs-queue-ex.c -- implements Breadth-First Search in a grid graph using lcfqueue.h and a reverse linking
   construction algorithm to connect the vertex. Shows how to use pointer-to-customType in the queue. */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NEIGHBOR_MAX 4 // Maximun amount of neighbors for any given node.
#define NEIGHBOR_TOP 0
#define NEIGHBOR_RIGHT 1
#define NEIGHBOR_BOT 2
#define NEIGHBOR_LEFT 3


/* --- How to use a custom type on the queue: --- */

// First define the custom struct to be used in the queue. Here is mine:
typedef struct graph_node {
  char tile;
  char x,y;
  struct graph_node * neighbors[NEIGHBOR_MAX];
  struct graph_node * track;
  bool processed;  
} GNode;

// Now set the VAL_TYPE. This can also be done before the typedef struct, but it we do here for clarity.
#define VAL_TYPE GNode *

// Now include the lcfqueue.h and we are all set to go. Piece of cake!
#include "lcfqueue.h"


/* --- Now, the rest of the program. --- */

// The grid graph and it's data. beginning is the first node of the graph, while entrance
// and exit are for using with Breadth-First Search algorithm plus early exit.
typedef struct graph {
  GNode * beginning;
  GNode * entrance;
  GNode * exit;
  char width;
  char height;
} Graph;

// This one is not implemented yet. Skip it. Skip it now!
Graph newGraph(int width, int height);
/* operation:          */
/* preconditions:      */
/* postconditions:     */
/* additional info:    */

void buildNewGraph(Graph *, int width, int height, GNode * arr[][height]);
/* operation:          Sets up the graph, creates all GNodes for it, links them, places nodes' addresses in an array */
/*                     of pointer-to-GNode, sets their's tile to 'O' and sets Graph's beginning and exit to 0.       */
/* preconditions:      A pointer to a uninitialized Graph. Will cause memory leak if used in a initialized Graph.    */
/* postconditions:     Leaves the Graph ready to use, handle, print, etc.                                            */
/* additional info:    We have a purposely left behind memory leak here. And by purpose I mean laziness. But it will */
/*                     will only occur if you call this function again on a already initialized Graph, because it    */
/*                     won't call any free(). You've been warned.                                                    */
/*                     It uses a reverse linking method to set up the "edges" of the graph when creating the nodes.  */
/*                     Actually, I just link them as a ordinary linked list. There are no real edges. Guess why...   */
/*                     Additionally, the reason we link each GNode to a indexer array is for easy accessing them by  */
/*                     their's projected x,y position in the grid with functions. It may be more resource intensive  */
/*                     but is less coding. Yeah, I know... did I already mentioned my laziness?                      */

// Create a blank node.
GNode * newNode();
/* operation:          Creates a blank new node.                        */
/* preconditions:      A pointer to store the returned address.         */
/* postconditions:     A initialized, blank and linked to nothing node. */
/* additional info:    */

// Create a blank node and sets it's x and y. For use with and index array (pointer-to-GNode).
GNode * newNodeAt(int x, int y);
/* operation:          Calls newNode(), sets GNode x,y and return it's address.                      */
/* preconditions:      A uninitialized pointer-to-GNode and a grid/array/something to use it's x,y.  */
/* postconditions:     A initialized GNode with proper x,y set. Unless you gave garbage to x,y.      */
/* additional info:    Used in buildNewGraph() in conjunct with the passed pointer to bi-dimensional */
/*                     array of pointer to GNode.                                                    */

// Returns the address of the node at specified projected location.
GNode * getNodeAt(int x, int y);
/* operation:          Returns the address of a GNode at the specified x,y.                  */
/* preconditions:      A pointer-to-GNode to hold the returned value and a initialized array */
/*                     of pointer-to-GNode.                                                  */
/* postconditions:     The GNode address.                                                    */
/* additional info:    */

// Sets the entrance, or statring point, for running the Breadth First Search algorithm. Not implemented.
void setGraphEntrance(GNode *);
/* operation:          */
/* preconditions:      */
/* postconditions:     */
/* additional info:    */

// For using a early exit on Breadth First. Not implemented.
void setGraphExit(GNode *);
/* operation:          */
/* preconditions:      */
/* postconditions:     */
/* additional info:    */

// Restore graph's nodes to theirs initial stats, like processed=false, tile='0', track=0. TODO: document it here.
void resetGraph(int width, int height, GNode * arr[][height], Graph * g);
/* operation:          */
/* preconditions:      */
/* postconditions:     */
/* additional info:    */

// Alter node display glyph.
void switchPassable(int width, int height, GNode * arr[][height], int x, int y);
/* operation:          Switch if node is passable or not.           */
/* preconditions:      Initialized GNode and array index.           */
/* postconditions:     GNode has its links broken or reconstructed. */
/* additional info:    Beacause C arrays reasons, we need to pass the size of the second part of the */
/*                     array for it to initialize. But hey! At least we DO have VLAs, right? -_-'    */

// Set node neighbor to point to specified address.
void setNodeNeighbor(GNode * node, int neighbor, GNode * neigh_address);
/* operation:          Sets the address to point to of a neighbor. (pseudo edge).           */
/* preconditions:      Two initialized GNodes.                                              */
/* postconditions:     Link node->neighbor is made.                                         */
/* additional info:    This is a directed link. Call again giving the proper params to make */
/*                     the neighbor->node link.                                             */

// Look for neighbors and unlink them, then free allocated memory. Not implemented.
bool destroyGNode(GNode *);
/* operation:          */
/* preconditions:      */
/* postconditions:     */
/* additional info:    */

// Print out options.
void printOptions();
/* operation:          Print out the available options.         */
/* preconditions:      Nothing. Just call it.                   */
/* postconditions:     Display the available options to select. */
/* additional info:    */

void printVerticesAsGrid(int width, int height, GNode * arr[][height]);
/* operation:          Uses the indexes array to print the glyph of each GNode at its projected location in the array. */
/* preconditions:      Initialized and filled bidimensional array of pointer to GNode.                                 */
/* postconditions:     A grid-like view of the graph through the use of printed GNodes->tile characters.               */
/* additional info:    Looks pretty. */

// Discards anything until \n, inclusive, from input buffer/stream for avoiding bugs.
void clearInput();
/* operation:          Discards extra input. */
/* preconditions:      A input function called just before. */
/* postconditions:     Clean input buffer. */
/* additional info:    */

// For links testing. Will not doc this because it will pe removed later.
void testLings(Graph *);

// Run Breadth-First Search algorithm on the graph from it's entrance to it's exit.
void runBreadthFirstSerach(Graph *);


/* --------------------------------------- MAIN --------------------------------------- */

int main(void)
{
  printf("\n--- BREADTH-FIRST SEARCH - LCF IMPLEMENTATION ---\n");
  printf("\nBreadth-First Search & Reverse Linking Grid Graph algorithms implementation.\n");
  
  // Graph and displayArr size.
  int graph_width = 8, graph_height = 5;
  
  // Display array, for being printed.
  GNode * displayArr[graph_width][graph_height];
  Graph graph_main;
  
  // Control variables.
  char input;
  char node_glyph;
  int node_x, node_y;
  
  printf("\nSize of QElem: %d\n", sizeof(QElem));
  printf("Size of Queue: %d\n", sizeof(Queue));
  printf("Size of GNode: %d\n", sizeof(GNode));
  printf("Size of Graph: %d\n", sizeof(Graph));
  printf("Size of Display Array: %d\n", sizeof(displayArr));
  
  while (1)
  {
    printOptions();
    
    printf(" Option: ");
    input = getchar();
    clearInput();
    
    // Builds a new graph. Every call beyond the first is unnecessary and will leak memory.
    // See `buildNewGraph()` doc for details.
    if (input == '1') {
      printf("\nSELECTED %c\n", input);
      printf(" Building new graph...\n");
      buildNewGraph(&graph_main, graph_width, graph_height, &displayArr);
      printf("\nDone.\n");
      continue;
    }
    
    // Uses the displayArr to build a visual representation of the graph. Only vertices are shown.
    if (input == '2') {
      printf("\nSELECTED %c\n", input);
      printf(" Printing graph nodes...\n\n");
      printVerticesAsGrid(graph_width, graph_height, &displayArr);
      printf("\n");
      continue;
    }
    
    // Modify the glyph of a node.
    if (input == '3') {
      printf("\nSELECTED %c\n", input);
      printf(" Pleas, enter x y to to switch node between passable/unpassable.\n  Params: ");
      scanf("%d %d", &node_x, &node_y);
      clearInput();
      switchPassable(graph_width, graph_height, &displayArr, node_x, node_y);
      //switchPassable(getNodeAt(node_x, node_y), node_glyph); // Future update
      printf(" Node at (%d,%d) switched.\n", node_x, node_y);
      continue;
    }
    
    // Set entrance and exit of the graph
    if (input == '4') {
      printf("\nSELECTED %c\n", input);
      printf(" Give x1 y1 and then x2 y2 for, respectively, set the entrance and exit node.\n");
      printf(" Oh, and I almost forgot. This function isn't implemented yet. Auto-setting both... ");
      graph_main.entrance = displayArr[0][0];
      graph_main.exit = displayArr[graph_width - 1][graph_height - 1];
      graph_main.entrance->tile = 'S';
      graph_main.exit->tile = 'E';
      printf("done.\n");
      continue;
    }
    
    // Breadth-First Search.
    if (input == '5') {
      printf("\nSELECTED %c\n", input);
      printf(" Executing Breadth-First Search from graph's entrance to graph's exit:\n\n");
      runBreadthFirstSerach(&graph_main);
      printf(" Breadth-First Search completed.\n");
      printVerticesAsGrid(graph_width, graph_height, &displayArr);
      resetGraph(graph_width, graph_height, &displayArr, &graph_main);
      continue;
    }
    
    // If no valid option was selected, we finish the program.
    break;
  }
  
  printf("\n Bye.\n\n");
  
  return 0;
}



/* ------------------------------------- FUNTIONS ------------------------------------- */

// Options display.
void printOptions()
{
  printf("\nOPTIONS AVAILABLE:\n");
  printf("  1 - Build graph and display grid;\n");
  printf("  2 - Display vertices from graph as a grid;\n");
  printf("  3 - Switch a node at x,y to be passable or not.\n");
  printf("  4 - Set graph entrance and exit. They will be marked with S and E, respectively.\n");
  printf("  5 - Run Breadth-First Search algorithm on the graph. Requires steps 1 and 4 performed.\n");
  printf("  Type anything else to exit.\n\n");
}

// Node glyph setter.
void switchPassable(int width, int height, GNode * arr[][height], int x, int y)
{
  bool destroy = false; // Lets first assume there is no links, so it is for rebuilding them.
  for (int i = 0; i < NEIGHBOR_MAX; i++){
    // It there is at least one link, then it is for us to destroy it and anyone else.
    if (arr[x][y]->neighbors[i] != 0) destroy = true;
  }
  
  // Now we know for sure what to do. Let's do it.
  if (destroy == true) {
    if (arr[x][y]->neighbors[NEIGHBOR_TOP] != 0)
      arr[x][y]->neighbors[NEIGHBOR_TOP]->neighbors[NEIGHBOR_BOT] = 0;
    
    if (arr[x][y]->neighbors[NEIGHBOR_RIGHT] != 0)
      arr[x][y]->neighbors[NEIGHBOR_RIGHT]->neighbors[NEIGHBOR_LEFT] = 0;
    
    if (arr[x][y]->neighbors[NEIGHBOR_BOT] != 0)
      arr[x][y]->neighbors[NEIGHBOR_BOT]->neighbors[NEIGHBOR_TOP] = 0;
    
    if (arr[x][y]->neighbors[NEIGHBOR_LEFT] != 0)
      arr[x][y]->neighbors[NEIGHBOR_LEFT]->neighbors[NEIGHBOR_RIGHT] = 0;
    
    arr[x][y]->tile = '#';
  }
  
  if (destroy == false) {
    if (arr[x][y]->neighbors[NEIGHBOR_TOP] != 0)
      arr[x][y]->neighbors[NEIGHBOR_TOP]->neighbors[NEIGHBOR_BOT] = arr[x][y];
      
    if (arr[x][y]->neighbors[NEIGHBOR_RIGHT] != 0)
      arr[x][y]->neighbors[NEIGHBOR_RIGHT]->neighbors[NEIGHBOR_LEFT] = arr[x][y];
      
    if (arr[x][y]->neighbors[NEIGHBOR_BOT] != 0)
      arr[x][y]->neighbors[NEIGHBOR_BOT]->neighbors[NEIGHBOR_TOP] = arr[x][y];
      
    if (arr[x][y]->neighbors[NEIGHBOR_LEFT] != 0)
      arr[x][y]->neighbors[NEIGHBOR_LEFT]->neighbors[NEIGHBOR_RIGHT] = arr[x][y];
      
    arr[x][y]->tile = 'O';
  }
}

void resetGraph(int width, int height, GNode * arr[][height], Graph * g)
{
  printf(" Reseting graph status... ");
  for (int a = 0; a < height; a++)
  {
    for (int b = 0; b < width; b++)
    {
      for (int i = 0; i < NEIGHBOR_MAX; i++){
        if (arr[b][a]->neighbors[i] != 0) {
          arr[b][a]->tile = 'O';
          arr[b][a]->track = 0;
          arr[b][a]->processed = false;
          break;
        }
      }
    }
  }
  if (g->entrance != 0) g->entrance->tile = 'S';
  if (g->exit != 0) g->exit->tile = 'E';
  printf("done.\n");
}

void runBreadthFirstSerach(Graph * g)
{
  Queue * frontier = newQueue();
  enqueue(frontier, g->entrance);
  GNode * exit = g->exit;
  GNode * explorer = 0;
  while (explorer != exit)
  {
    // Gets next on queue
    explorer = dequeue(frontier);
    
    // We are processing this node right now, so we mark it as processed.
    explorer->processed = true;
    
    // Cycle through all neighbors and, for each, check if is a valid existent node and if
    // has not been processed yet, adding it to the frontier if so.
    for (int n = 0; n < NEIGHBOR_MAX; n++){
      if (explorer->neighbors[n] != NULL && explorer->neighbors[n] != 0 && explorer->neighbors[n]->processed == false) {
        // Sets track to point to the node which we reached explorer->neighbors[n] from.
        explorer->neighbors[n]->track = explorer;
        // Adds explorer->neighbors[n] to the frontier.
        enqueue(frontier, explorer->neighbors[n]);
      }
    }
  }
  
  // explorer is on the exit, so lets mark it as F for Final. I like the word Final. Like in Final Destination or something alike.
  explorer->tile = 'F';
  
  // We reach here when explorer finds the exit.
  // Now explorer will trackback the path marking with 'X' all nodes walked.
  while (explorer->track != 0)
  {
    explorer = explorer->track;
    explorer->tile = 'X';
  }
  
  // Now place the 'S' back on it's place
  explorer->tile = 'S';
  
  /* Nooot working.
  // Theoretically, we should be done by now. Let's just reset all nodes' track and processed now.
  explorer = g->beginning; // Should be at top-left of the graph.
  explorer->track = 0;
  explorer->processed = false;
  int run = 1;
  while (run == 1)
  {
     printf("\nREACHED\n");
     while (explorer->neighbors[NEIGHBOR_RIGHT] != 0 && explorer->neighbors[NEIGHBOR_RIGHT] != NULL)
     {
       explorer = explorer->neighbors[NEIGHBOR_RIGHT];
       printf("  |  %p", explorer);
       explorer->track = 0;
       explorer->processed = false;
     }
     
     // If there is still a row below, move to it and go on. Otherwise, break out the loop.
     if (explorer->neighbors[NEIGHBOR_BOT] != 0) {
       explorer = explorer->neighbors[NEIGHBOR_BOT];
       explorer->track = 0;
       explorer->processed = false;
     } else { run = 0; }
     
     // Cycle row from right to left
     while (explorer->neighbors[NEIGHBOR_LEFT] != 0)
     {
       explorer = explorer->neighbors[NEIGHBOR_RIGHT];
       explorer->track = 0;
       explorer->processed = false;
     }
     
     // Again, if there is still a row below, move to it and go on. Otherwise, break out the loop.
     if (explorer->neighbors[NEIGHBOR_BOT] != 0) {
       explorer = explorer->neighbors[NEIGHBOR_BOT];
       explorer->track = 0;
       explorer->processed = false;
     } else { run = 0; }
  }
  */
}

void testLings(Graph * g)
{
  GNode * node_curr = g->beginning;
  // step 1
  node_curr->tile = 'S';
  node_curr = node_curr->neighbors[NEIGHBOR_RIGHT];
  // step 2
  node_curr->tile = 'X';
  node_curr = node_curr->neighbors[NEIGHBOR_RIGHT];
  // step 3
  node_curr->tile = 'X';
  node_curr = node_curr->neighbors[NEIGHBOR_RIGHT];
  // step 4
  node_curr->tile = 'X';
  node_curr = node_curr->neighbors[NEIGHBOR_BOT];
  // step 5
  node_curr->tile = 'X';
  node_curr = node_curr->neighbors[NEIGHBOR_BOT];
  // step 6
  node_curr->tile = 'X';
  node_curr = node_curr->neighbors[NEIGHBOR_LEFT];
  // step 7
  node_curr->tile = 'X';
  node_curr = node_curr->neighbors[NEIGHBOR_BOT];
  // step 8
  node_curr->tile = 'X';
  node_curr = node_curr->neighbors[NEIGHBOR_RIGHT];
  // step 9
  node_curr->tile = 'X';
  node_curr = node_curr->neighbors[NEIGHBOR_RIGHT];
  // step 10
  node_curr->tile = 'X';
  node_curr = node_curr->neighbors[NEIGHBOR_RIGHT];
  // step 11
  node_curr->tile = 'X';
  node_curr = node_curr->neighbors[NEIGHBOR_TOP];
  // step 12
  node_curr->tile = 'F';
  
}

// Get address of node at x,y
GNode * getNodeAt(int x, int y)
{
  return 0;
}

void clearInput()
{
  printf(" Flushing rubbish...");
  while (getchar() != '\n') printf(" .");
  printf(" done.\n");
}

// Vertex display.
void printVerticesAsGrid(int width, int height, GNode * arr[][height])
{
  printf(" Printing glyphs of nodes:\n\n");
  for (int a = 0; a < height; a++)
  {
    for (int b = 0; b < width; b++)
    {
      printf("  %c",arr[b][a]->tile);
    }
    printf("\n\n");
  }
}

// Creates new blank graph.
Graph newGraph(int width, int height)
{
  Graph * g = (Graph *)malloc(sizeof(Graph));
  g->width = width;
  g->height = height;
}

// Create a node and return it's address.
GNode * newNode()
{
  GNode * n = (GNode *)malloc(sizeof(GNode));
  n->processed = false;
  n->track = 0;
  for (int i = 0; i < NEIGHBOR_MAX; i++){
    n->neighbors[i] = 0;
  }
  n->tile = 'O';
  return n;
}

// Create a node and set it's x and y. Return it's address.
GNode * newNodeAt(int x, int y)
{
  GNode * n = newNode();
  n->x = x;
  n->y = y;
  return n;
}

// Neighbor setter.
void setNodeNeighbor(GNode * node, int neighbor, GNode * neigh_address)
{
  node->neighbors[neighbor] = neigh_address;
}

// Graph constructor.
void buildNewGraph(Graph * g, int width, int height, GNode * arr[][height]) // TODO: Check for and free() memory
{
  // The base defaults.
  g->width = width;
  g->height = height;
  g->entrance = 0;
  g->exit = 0;
  
  Queue * queue_prev_row = newQueue();
  Queue * queue_curr_row = newQueue();
  GNode * node_curr;
  GNode * node_neigh;
  
  // Debug shit
  int size = 0;
  QElem * elem;
  
  for (int y = 0; y < height; y++)
  {
    printf("\n ROW %d >>>\n", y);
    for (int x = 0; x < width; x++)
    {
      // Debug
      printf("   C.%d | START === | Qp->l:%d | Qc->l:%d | PUSH ==>", x, queue_prev_row->length, queue_curr_row->length);
      
      // New GNode.
      node_curr = newNodeAt(x,y);
      // Do not enqueue elements of the last row in queue_prev_row.
      if (y < height - 1) enqueue(queue_prev_row, node_curr);
      // Enqueue current element for later linking.
      enqueue(queue_curr_row, node_curr);
      
      // Debug
      printf(" | Qp->l:%d | Qc->l:%d | POP ==>", queue_prev_row->length, queue_curr_row->length);
      
      // If we are not at the first row, create edges to and from current node's top neighbor.
      if (y > 0) // There is nothing to dequeue at first row.
      {
        node_neigh = dequeue(queue_prev_row); // This is the top neighbor to the current node.
        setNodeNeighbor(node_curr, NEIGHBOR_TOP, node_neigh); // Link edge node_curr->node_neigh.
        setNodeNeighbor(node_neigh, NEIGHBOR_BOT, node_curr); // Link edge node_neigh->node_curr.
      }
      
      // If we are not at this row's first element, create edges to and from current node's left neighbor.
      if (x > 0) // There is nothing do dequeue at first node of each row.
      {
        node_neigh = dequeue(queue_curr_row); // This is the left neighbor to the current node.
        setNodeNeighbor(node_curr, NEIGHBOR_LEFT, node_neigh); // Link edge node_curr->node_neigh.
        setNodeNeighbor(node_neigh, NEIGHBOR_RIGHT, node_curr); // Link edge node_neigh->node_curr.
      }
      
      // Debug
      printf(" | Qp->l:%d | Qc->l:%d |\n", queue_prev_row->length, queue_curr_row->length);
      
      arr[x][y] = node_curr;
    }
    
    // Each X loop end, we still need to pop X3 and link X4->X3 and X3->X4.
    // Since we are still to set the new node_curr... :P
    node_neigh = dequeue(queue_curr_row); // This is the left neighbor to the current node.
    setNodeNeighbor(node_curr, NEIGHBOR_LEFT, node_neigh); // Link edge node_curr->node_neigh.
    setNodeNeighbor(node_neigh, NEIGHBOR_RIGHT, node_curr); // Link edge node_neigh->node_curr.
    
    printf(" Final queue_prev_row->length %d\n",queue_prev_row->length);
    printf(" Final queue_curr_row->length %d\n",queue_curr_row->length);
  }
  
  // Free allocated memory.
  // In this algorithm, both queues will be already empty when we reach here. Also, the dequeued
  // QElem are freed by themselves, and the rest are just pointers to data that must persist.
  free(queue_prev_row);
  free(queue_curr_row);
  
  // Now point the beginning of the given graph to the node pointed by the first element of the array.
  // This choice is completely arbitrary and could be literally any node of the graph.
  g->beginning = arr[0][0];
}





















