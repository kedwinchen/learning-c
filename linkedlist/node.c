#include <stdlib.h>
#include <string.h>
#include "node.h"

void * getValue(node_t *node) {
	return (node->value);
}

node_t * getLeft(node_t *node) {
	return node->left;
}

node_t * getRight(node_t *node) {
	return node->right;
}

node_t * createNode(void *value, node_t *up, node_t *lft, node_t *rht) {
	node_t *ptr = malloc(sizeof(node_t));
	ptr->value = value;
        ptr->above = up;
        ptr->left = lft;
        ptr->right = rht;
	return ptr;
}

int destroyNode(node_t *node) {
        if(node == NULL) {
                return 1;
        }
        /* clear value */
        node->value = NULL;
        /* zero memory (optional) */
        memset(node, 0, sizeof(node_t));
        /* free node */
        free(node);
        /* clear pointer */
        node = NULL;
        return 0;
}

int rippleDestroy(node_t *node) {
        if(node == NULL) {
                return 1;
        }
        else {
                /* free children if defined */
                if ( node->left != NULL ) {
                        rippleDestroy(node->left);
                }
                if ( node->right != NULL ) {
                        rippleDestroy(node->right);
                }
                destroyNode(node);
        }
        return 0;
}
