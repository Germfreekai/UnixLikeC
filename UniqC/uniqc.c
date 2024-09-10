/*
 * Unix Like C
 *
 * uniqc.c
 *
 * Remove duplicates from an ordered list
 * The list must be already ordered
 */

/*
 * For the sake of overdoing, I will use linked list
 * and as much pointers as possible
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Object
typedef struct _object
{
    char *obj;
}Object;

// Node
// we do this so node can store any type of data
// which in this case is struct object
typedef struct _node
{
    void *data;
    struct _node *next;
}Node;

// Struct of nodes
typedef struct _linkedlist
{
    Node *head;
    Node *tail;
    Node *current;
}LinkedList;

// Pointer to functions
typedef void(*DISPLAY)(void*);
typedef int(*COMPARE)(void*, void*);

// Functions
void initLinkedList(LinkedList *linkedlist);
void addHead(LinkedList *linkedlist, void *data);
void *addTail(LinkedList *linkedlist, void *data, COMPARE compare);
void displayLinkedList(LinkedList *list, DISPLAY display);
void freeLinkedList(LinkedList *linkedlist);

int compareObj(Object *o1, Object *o2)
{
    return strcmp(o1->obj, o2->obj);
}

int displayObj(Object *obj)
{
    fprintf(stdout, "%s\n", obj->obj);
}

int main(int argv, char **argc)
{
    // Approach I am going to follow, based on a interview I had
    // I will get only last index
    
    LinkedList *linkedlist = (LinkedList*)malloc(sizeof(LinkedList));
    initLinkedList(linkedlist);

    Object *obj;

    for (int i = 1; i < argv; i++)
    {
        obj = malloc(sizeof(Object));
        obj->obj = malloc(sizeof(strlen(argc[i])) + 1);  // we add space for \0
        strcpy(obj->obj, argc[i]);
        addTail(linkedlist, obj, (int (*)(void*, void*))compareObj);
    }

    displayLinkedList(linkedlist, (DISPLAY)displayObj);

    freeLinkedList(linkedlist);

    return EXIT_SUCCESS;
}

void initLinkedList(LinkedList *linkedlist)
{
    linkedlist->head = NULL;
    linkedlist->tail = NULL;
    linkedlist->current = NULL;
}

void addHead(LinkedList *linkedlist, void *data)
{
    // Add beginning
    Node *node = (Node*)malloc(sizeof(Node));
    node->data = data;

    // If empty
    // both head and tail point to node
    if (! linkedlist->head)
    {
        linkedlist->tail = node;
        node->next = NULL;
    }
    else
    {
        // point curren to first, as it will push it it
        node->next = linkedlist->head;
    }
    // replace head with current node
    linkedlist->head = node;
}

void *addTail(LinkedList *linkedlist, void *data, COMPARE compare)
{
    // Add at end
    Node *node = (Node*)malloc(sizeof(Node));
    node->data = data;  // data is an Object *obj
    node->next = NULL;  // we are adding at the end

    // if empty
    // add to beginning
    if (! linkedlist->head)
    {
        linkedlist->head = node;
    }
    // If head exits, we need to find the last node
    // and append at the end
    else
    {
        // we only add if object not present
        if (compare(linkedlist->tail->data, node->data))
        {
            // we point last node to new node
            linkedlist->tail->next = node;
        }
        else
        {
            goto notadd;
        }
    }

    // we add new node to end
    linkedlist->tail = node;

    return NULL;

notadd:
    Object *obj = (Object*)node->data;  // we need to cast it
    free(obj->obj);
    free(obj);
    free(node);
    return NULL;
}

void displayLinkedList(LinkedList *linkedlist, DISPLAY display)
{
    Node *current = linkedlist->head;
    while (current != NULL)
    {
        // we send the data (Obj *obj)
        display(current->data);
        current = current->next;
    }
}

void freeLinkedList(LinkedList *linkedlist)
{
    Node *current = linkedlist->head;
    Node *next;
    while (current != NULL)
    {
        next = current->next;  // save next node
        Object *obj = (Object*)current->data;  // Cast object
        free(obj->obj);
        free(obj);
        free(current);
        current = next;  // move to the next node;
    }
    
    free(linkedlist);
}
