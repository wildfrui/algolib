#include <stdio.h>
#include <stdlib.h>

#define SIZE 255

// a node in a binary tree can have a right and a left child
typedef struct node {
    int candy;
    struct node *left, *right;
} node;

// implementating a stack to keep track of the pending subtrees
typedef struct stack {
    node* values[SIZE];
    int highest_used;
} stack;

node* new_house(int candy);
node* non_house(node* left, node* right);
int is_empty_stack(stack* s);
int tree_candy(node* tree);

// whenever making a choice of
int main(void) {
    node* four = new_house(4);
    node* nine = new_house(9);
    node* B = non_house(four, nine);
    node* fifteen = new_house(15);
    node* C = non_house(B, fifteen);
    int total = tree_candy(C);
    printf("%d\n", total);
    return 0;
}

// the node that contents some value
node* new_house(int candy) {
    node* house = malloc(sizeof(node));
    if (house == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    house->candy = candy;
    house->right = NULL;
    house->left = NULL;
    return house;
}

// the node that dpesnt't contain any value
node* non_house(node* left, node* right) {
    node* non_house = malloc(sizeof(node));
    if (non_house == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    non_house->left = left;
    non_house->right = right;
    return non_house;
}

// stack operations implementation

stack* new_stack(void) {
    stack* s = malloc(sizeof(stack));
    if (s == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    // -1 means empty stack
    s->highest_used = -1;
    return s;
}

void push_stack(stack* s, node* value) {
    s->highest_used++;
    s->values[s->highest_used] = value;
}

node* pop_stack(stack* s) {
    node* ret = s->values[s->highest_used];
    s->highest_used--;
    return ret;
}

int is_empty_stack(stack* s) { return s->highest_used == -1; }

// starting from a root of a tree
int tree_candy(node* tree) {
    int total = 0;
    stack* s = new_stack();
    while (tree != NULL) {
        if (tree->left && tree->right) {
            push_stack(s, tree->left);
            tree = tree->right;
        } else {
            total = total + tree->candy;
            if (is_empty_stack(s))
                tree = NULL;
            else
                tree = pop_stack(s);
        }
    }
    return total;
}