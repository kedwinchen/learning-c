#ifndef __NODE_H__
#define __NODE_H__

typedef struct node {
        struct node *left;
        struct node *right;
        struct node *above;
        void *value;
} node_t;

void * getValue(node_t *);
node_t * getLeft(node_t *);
node_t * getRight(node_t *);
node_t * createNode(void *, node_t *, node_t *, node_t *);
int destroyNode(node_t *);
int rippleDestroy(node_t *);
#endif /* __NODE_H__ */
