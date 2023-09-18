#include <stdio.h>
#include <stdlib.h>

#define SIZE 255
#define TEST_CASES 5

typedef struct tree_node {
    int candy;
    struct tree_node *left, *right;
} tree_node;

int max(int v1, int v2);
int tree_height(tree_node *root);
int num_streets(tree_node *root);
int tree_candy(tree_node *root);
void tree_solve(tree_node *tree);
tree_node *read_tree_helper(char *line, int *pos);
tree_node *read_tree(char *line);

int main(void) {
    int i;
    char line[SIZE + 1];
    tree_node *tree;
    for (i = 0; i < TEST_CASES; i++) {
        gets(line);
        tree = read_tree(line);
        tree_solve(tree);
    }
    return EXIT_SUCCESS;
}

int max(int v1, int v2) {
    if (v1 > v2)
        return v1;
    else
        return v2;
}

// finding a tree height
// 1. If the root of the tree is a house node, then the tree’s height is zero.
// 2. If the root of the tree is a nonhouse node, then the tree’s height is one
// more than the maximum of the left subtree’s height and the right subtree’s
// height.
int tree_height(tree_node *root) {
    if (!root->left && !root->right) {
        return 0;
    }
    return 1 + max(tree_height(root->left), tree_height(root->right));
}

// finding a number of streets
// 1. If the root of the tree is a house node, then the number of streets we
// walk is zero.
// 2. If the root of the tree is a nonhouse node, then the number of streets we
// walk is the number of streets we walk for the left subtree plus the number of
// streets we walk for the right subtree plus 4 because we have to walk one
// street from H to F and then walk one street from F back to H after we finish
// with the F subtree. We have to do similarly for G.
int num_streets(tree_node *root) {
    if (!root->left && !root->right) {
        return 0;
    }
    return num_streets(root->left) + num_streets(root->right) + 4;
}

int tree_candy(tree_node *root) {
    if (!root->left && !root->right) return root->candy;
    return tree_candy(root->left) + tree_candy(root->right);
}

void tree_solve(tree_node *tree) {
    int candy = tree_candy(tree);
    int height = tree_height(tree);
    int tree_streets = num_streets(tree) - height;
    printf("%d %d\n", tree_streets, candy);
}

tree_node *read_tree_helper(char *line, int *pos) {
    tree_node *tree;
    tree = malloc(sizeof(tree_node));
    if (tree == NULL) {
        fprintf(stderr, "malloc error\n");
        exit(1);
    }
    if (line[*pos] == '(') {
        (*pos)++;
        tree->left = read_tree_helper(line, pos);
        (*pos)++;
        tree->right = read_tree_helper(line, pos);
        (*pos)++;
        return tree;
    } else {
        tree->left = NULL;
        tree->right = NULL;
        tree->candy = line[*pos] - '0';
        (*pos)++;
        if (line[*pos] != ')' && line[*pos] != ' ' && line[*pos] != '\0') {
            tree->candy = tree->candy * 10 + line[*pos] - '0';
            (*pos)++;
        }
        return tree;
    }
}

tree_node *read_tree(char *line) {
    int pos = 0;
    return read_tree_helper(line, &pos);
}