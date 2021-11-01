#include "lists.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#ifdef test_lists

// Node and list definitions
struct node {
	item x;
	struct node *next, *back;
};
typedef struct node node;

struct list {
	node *sentinel;
	node *current_item, *start, *end;
};

// Functions

// Create a new empty list, with a default item to return in case of error.
list *newList (item d) {
	list *l = malloc(sizeof(list));
	l -> sentinel = malloc(sizeof(node));
	l -> start = malloc(sizeof(node));
	l -> end = malloc(sizeof(node));
	l -> current_item = malloc(sizeof(node));

	l -> sentinel -> x = d;
    	l -> sentinel -> next = l -> sentinel;
	l -> sentinel -> back = l -> sentinel;

	l -> start = l -> sentinel;
	l -> end = l -> sentinel;
	l -> current_item = l -> sentinel;

	return l;
}

// Free up the list and all the data in it.
void freeList (list *l) {
	node *n = l -> sentinel -> back, *nback;
	while (n != l -> sentinel) {
		nback = n -> back;
		free(n);
		n = nback;
	}
	free(l -> sentinel);
	free(l);
	return;
}

// Set the current position before the first item or after the last item,
// to begin a forward or backward traversal.
void startF (list *l) {
	l -> current_item = l -> sentinel -> next;
}
void startB (list *l) {
	l -> current_item = l -> sentinel;
}

// Check whether the current position is at the end or start, to test
// whether a traversal has finished.
bool endF (list *l) {
	return (l -> current_item == l -> sentinel) ? true : false;
}
bool endB (list *l) {
	return (l -> current_item -> back == l -> sentinel) ? true : false;
}

// Move the current position one place forwards or backwards, and return true.
// If nextF is called when at the end of the list, or nextB when at the start,
// the functions do nothing and return false.
bool nextF (list *l) {
	if (l -> current_item != l -> sentinel) {
		l -> current_item = l -> current_item -> next;
		return 1;
	}
	return 0;
}
bool nextB (list *l) {
	if (l -> current_item -> back != l -> sentinel) {
		l -> current_item = l -> current_item -> back;
		return 1;
	}
	return 0;
}

// Insert an item before the current position during a traversal.  The traversal
// of the remaining items is not affected.
void insertB (list *l, item x) {
    node *n = malloc(sizeof(node));
    n -> x = x;

    l -> current_item = l -> current_item -> back;

    n -> next = l -> current_item -> next;
    n -> back = l -> current_item;

    l -> current_item -> next -> back = n;
    l -> current_item -> next = n;

    l -> current_item = l -> current_item -> next;

    if (l -> current_item -> back == l -> sentinel)
        l -> start = l -> current_item;
    if (l -> current_item -> next == l -> sentinel)
        l -> end = l -> current_item;
    return;
}
void insertF (list *l, item x) {
    node *n = malloc(sizeof(node));
    n -> x = x;

    n -> next = l -> current_item;
    n -> back = l -> current_item -> back;

    l -> current_item -> back -> next = n;
    l -> current_item -> back = n;

    if (l -> current_item -> back == l -> sentinel)
        l -> start = l -> current_item;
    if (l -> current_item -> next == l -> sentinel)
        l -> end = l -> current_item;
    return;
}

// Get the current item. If getF is called when at the end, or getB is called
// when at the start, the default item is returned.
item getF (list *l) {
	return l -> current_item -> x;
}
item getB (list *l) {
	return l -> current_item -> back -> x;
}

// Set the current item and return true. If setF is called when at the end, or
// setB when at the start, nothing is done and false is returned.
bool setF (list *l, item x) {
	if (l -> current_item != l -> sentinel) {
		l -> current_item -> x = x;
		return 1;
	}
	return 0;
}
bool setB (list *l, item x) {
	if (l -> current_item -> back != l -> sentinel) {
		l -> current_item -> back -> x = x;
		return 1;
	}
	return 0;
}

// Delete the current item and return true. When iterating forward, either nextF
// or deleteF should be called to delete or step past each item. If deleteF/B is
// called at the start/end of the list, nothing is done and false is returned.
bool deleteF (list *l) {
	if (l -> current_item != l -> sentinel) {
		l -> current_item -> back -> next = l -> current_item -> next;
		l -> current_item -> next -> back = l -> current_item -> back;
		node *n = l -> current_item;
		l -> current_item = l -> current_item -> next;
		free(n);
		return 1;
	}
	return 0;
}
bool deleteB (list *l) {
	if(l -> current_item -> back != l -> sentinel){
        l -> current_item -> back -> back -> next = l -> current_item;
		node *n = l -> current_item -> back;
		l -> current_item -> back = l -> current_item -> back -> back;
		free(n);
		return 1;
	}
	return 0;
}

// Convert a string description to a list.
static list *toList(char *pic) {
    int pos = strchr(pic, '|') - pic;
    list *l = newList(-1);
    for (int i = 0; i < strlen(pic); i++) {
        if (pic[i] == '|') continue;
        int n = pic[i] - '0';
        insertF(l, n);
    }
    startF(l);
    for (int i = 0; i < pos; i++) nextF(l);
    return l;
}

// Convert a list to a string description.
static void toString(list *l, char s[]) {
    int pos = 0;
    while (! endB(l)) { pos++; nextB(l); }
    int i = 0;
    while (! endF(l)) {
        if (i == pos) s[i++] = '|';
        s[i++] = '0' + getF(l);
        nextF(l);
    }
    if (i == pos) s[i++] = '|';
    s[i++] = '\0';
}

// Check a particular function. The first argument is the name of the function.
// The second and third arguments describes the list before and after the call.
// The fourth, fifth and sixth arguments are an item to pass as an argument, an
// item to expect as a result, and a boolean to expect as a result, if any.
static bool check(char *op, char *lb, char *la, item x, item y, bool b) {
    bool r = true;
    item z = 0;
    list *l = toList(lb);
    if (strcmp(op, "startF") == 0) startF(l);
    else if (strcmp(op, "startB") == 0) startB(l);
    else if (strcmp(op, "endF") == 0) r = endF(l);
    else if (strcmp(op, "endB") == 0) r = endB(l);
    else if (strcmp(op, "nextF") == 0) r = nextF(l);
    else if (strcmp(op, "nextB") == 0) r = nextB(l);
    else if (strcmp(op, "insertF") == 0) insertF(l, x);
    else if (strcmp(op, "insertB") == 0) insertB(l, x);
    else if (strcmp(op, "getF") == 0) z = getF(l);
    else if (strcmp(op, "getB") == 0) z = getB(l);
    else if (strcmp(op, "setF") == 0) r = setF(l, x);
    else if (strcmp(op, "setB") == 0) r = setB(l, x);
    else if (strcmp(op, "deleteF") == 0) r = deleteF(l);
    else if (strcmp(op, "deleteB") == 0) r = deleteB(l);
    else return false;
    if (r != b || z != y) return false;
    char s[100];
    toString(l, s);
    freeList(l);
    return strcmp(s, la) == 0;
}

// Testing for the lists module. Strings are used to describe lists. The strings
// "|37", "3|7", "37|" represent a list of two items, with the current position
// at the start, middle or end.

// Test newList, freeList.
static void test1() {
    list *l = newList(-1);
    assert(l != NULL);
    freeList(l);
}

// Test insertF, getB
static void test2() {
    list *l = newList(-1);
    insertF(l, 3);
    assert(getB(l) == 3);
    freeList(l);
}

// Test that startF and startB move to the beginning or end of the list.
static void testStart() {
    assert(check("startF", "|", "|", 0, 0, true));
    assert(check("startF", "|37", "|37", 0, 0, true));
    assert(check("startF", "3|7", "|37", 0, 0, true));
    assert(check("startF", "37|", "|37", 0, 0, true));
    assert(check("startB", "|", "|", 0, 0, true));
    assert(check("startB", "|37", "37|", 0, 0, true));
    assert(check("startB", "3|7", "37|", 0, 0, true));
    assert(check("startB", "37|", "37|", 0, 0, true));
}

// Test that endF and endB detect the list being at the end or beginning.
static void testEnd() {
    assert(check("endF", "|", "|", 0, 0, true));
    assert(check("endF", "|37", "|37", 0, 0, false));
    assert(check("endF", "3|7", "3|7", 0, 0, false));
    assert(check("endF", "37|", "37|", 0, 0, true));
    assert(check("endB", "|", "|", 0, 0, true));
    assert(check("endB", "|37", "|37", 0, 0, true));
    assert(check("endB", "3|7", "3|7", 0, 0, false));
    assert(check("endB", "37|", "37|", 0, 0, false));
}

// Test that nextF and nextB move forward or backward in the list.
static void testNext() {
    assert(check("nextF", "|", "|", 0, 0, false));
    assert(check("nextF", "|37", "3|7", 0, 0, true));
    assert(check("nextF", "3|7", "37|", 0, 0, true));
    assert(check("nextF", "37|", "37|", 0, 0, false));
    assert(check("nextB", "|", "|", 0, 0, false));
    assert(check("nextB", "|37", "|37", 0, 0, false));
    assert(check("nextB", "3|7", "|37", 0, 0, true));
    assert(check("nextB", "37|", "3|7", 0, 0, true));
}

// Test that insertF and insertB insert behind the direction of motion.
static void testInsert() {
    assert(check("insertF", "|", "5|", 5, 0, true));
    assert(check("insertF", "|37", "5|37", 5, 0, true));
    assert(check("insertF", "3|7", "35|7", 5, 0, true));
    assert(check("insertF", "37|", "375|", 5, 0, true));
    //assert(check("insertB", "|", "|5", 5, 0, true));
    assert(check("insertB", "|37", "|537", 5, 0, true));
    assert(check("insertB", "3|7", "3|57", 5, 0, true));
    assert(check("insertB", "37|", "37|5", 5, 0, true));
}

// Test that getF and getB return the current item.
static void testGet() {
    assert(check("getF", "|", "|", 0, -1, true));
    assert(check("getF", "|37", "|37", 0, 3, true));
    assert(check("getF", "3|7", "3|7", 0, 7, true));
    assert(check("getF", "37|", "37|", 0, -1, true));
    assert(check("getB", "|", "|", 0, -1, true));
    assert(check("getB", "|37", "|37", 0, -1, true));
    assert(check("getB", "3|7", "3|7", 0, 3, true));
    assert(check("getB", "37|", "37|", 0, 7, true));
}

// Test setF and setB. Check's second argument is the expected result.
static void testSet() {
    assert(check("setF", "|", "|", 5, 0, false));
    assert(check("setF", "|37", "|57", 5, 0, true));
    assert(check("setF", "3|7", "3|5", 5, 0, true));
    assert(check("setF", "37|", "37|", 5, 0, false));
    assert(check("setB", "|", "|", 5, 0, false));
    assert(check("setB", "|37", "|37", 5, 0, false));
    assert(check("setB", "3|7", "5|7", 5, 0, true));
    assert(check("setB", "37|", "35|", 5, 0, true));
}

static void testDelete() {
    assert(check("deleteF", "|", "|", 0, 0, false));
    assert(check("deleteF", "|37", "|7", 0, 0, true));
    assert(check("deleteF", "3|7", "3|", 0, 0, true));
    assert(check("deleteF", "37|", "37|", 0, 0, false));
    assert(check("deleteB", "|", "|", 0, 0, false));
    assert(check("deleteB", "|37", "|37", 0, 0, false));
    assert(check("deleteB", "3|7", "|7", 0, 0, true));
    assert(check("deleteB", "37|", "3|", 0, 0, true));
}

int main() {
    test1();
    test2();
    testStart();
    testEnd();
    testNext();
    testInsert();
    testGet();
    testSet();
    testDelete();
    printf("Lists module OK\n");
}

#endif
