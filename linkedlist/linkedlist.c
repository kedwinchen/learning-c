#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "node.h"
/******************************************************************************/
/**
 * Given that the `node_t` structure contains three `node_t*`, which are `left`,,
 * `right`, and `above`, `up` will not be used.
 */
/******************************************************************************/

#ifndef __linkedlist_t__
#define __linkedlist_t__
typedef struct linkedlist {
        node_t *head;
        node_t *tail;
        node_t *current;
        uint64_t size;
} linkedlist_t;
#endif /* __linkedlist_t__ */

/**
 * Spawns a root of the tree
 */
linkedlist_t * create_list(void * value)
{
        linkedlist_t *list = (linkedlist_t *)malloc(sizeof(linkedlist_t));
        list->head = createNode(value, NULL, NULL, NULL);
        list->tail = list->head;
        list->size++;
        list->current = list->head;
        return list;
}

node_t * prev(linkedlist_t * list)
{
        return list->current->left;
}

node_t * next(linkedlist_t * list)
{
        return list->current->right;
}

node_t * reset_head(linkedlist_t * list)
{
        return list->current = list->head;
}

node_t * advance(linkedlist_t * list)
{
        return list->current = next(list);
}

node_t * retreat(linkedlist_t * list)
{
        return list->current = prev(list);
}

/**
 * Insert a `node_t` containing `value` before the current node
 * Returns a pointer to the node inserted
 */
node_t * insert_node(linkedlist_t *list, void *value)
{
        node_t *newNode = createNode(value, NULL, list->current->left, list->current);
        if( list->current == list->head) {
                list->head = newNode;
        }
        list->current->left->right = newNode;

        return newNode;
}

/**
 * Add a `node_t` containing `value` at the end of the list
 */
void append_value(linkedlist_t *list, void *value)
{
        /* If the list does not exist, create one */
        if( list == NULL ) {
                list = create_list(value);
        }
        else {
                node_t *newNode = createNode(value, NULL, list->tail, NULL);
                list->tail->right = newNode;
                list->tail = list->tail->right;
        }

        (list->size)++;
}


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

node_t * test_list(void)
{
        node_t *root = createNode("root", NULL, NULL, NULL);

        /* This works... */
        node_t *rr = createNode("right", root, NULL, NULL);
        root->right = rr;

        /* This is preferred, as this sets both pointers appropriately */
        root->left = createNode("aadkjf;ft", root, NULL, NULL);

        return root;
}


int main(int argc, char *argv[])
{
        node_t *root;
        root = test_list();
        printf("root.value: %s\n", (char *)root->value);
        printf("root.left value: %s\n", (char *)root->left->value);
        printf("root.right.above.value: %s\n", (char *)root->right->above->value);
        rippleDestroy(root);
        return 0;
}
