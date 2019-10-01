/* This is a library module supporting lists. A list stores any number of items
and has a current position, which ranges from before the first item to after the
last item. Every operation is constant time.

The items stored in a list are of type 'item'. The typedef for this can be
changed for any particular application so that 'item' stands for any desired
type, e.g. a raw type such as int, double, char[20], struct... or a pointer type
such as char *, struct...*.

The functions with names ending in F are for forward traversal through a list,
and the ones ending in B are for backward traversal. The two sets of functions
are mirror images of each other. A typical forward traversal has the form:

    for (startF(l); ! endF(l); nextF(l)) {
        item x = getF(l);
        ...
    }

The lists provided by this module are not thread safe. */
#include <stdbool.h>

// The type of items stored in the list. Change this for different applications.
typedef int item;

// The list type is opaque (declared here, and defined in lists.c).
struct list;
typedef struct list list;

// Create a new empty list, with a default item to return in case of error.
list *newList(item d);

// Free up the list and all the data in it.
void freeList(list *l);

// Set the current position before the first item or after the last item,
// to begin a forward or backward traversal.
void startF(list *l);
void startB(list *l);

// Check whether the current position is at the end or start, to test
// whether a traversal has finished.
bool endF(list *l);
bool endB(list *l);

// Move the current position one place forwards or backwards, and return true.
// If nextF is called when at the end of the list, or nextB when at the start,
// the functions do nothing and return false.
bool nextF(list *l);
bool nextB(list *l);

// Insert an item before the current position during a traversal.  The traversal
// of the remaining items is not affected.
void insertF(list *l, item x);
void insertB(list *l, item x);

// Get the current item. If getF is called when at the end, or getB is called
// when at the start, the default item is returned.
item getF(list *l);
item getB(list *l);

// Set the current item and return true. If setF is called when at the end, or
// setB when at the start, nothing is done and false is returned.
bool setF(list *l, item x);
bool setB(list *l, item x);

// Delete the current item and return true. When iterating forward, either nextF
// or deleteF should be called to delete or step past each item. If deleteF/B is
// called at the start/end of the list, nothing is done and false is returned.
bool deleteF(list *l);
bool deleteB(list *l);
