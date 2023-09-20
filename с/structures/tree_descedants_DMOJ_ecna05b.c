#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 10
#define MAX_NODES 1000

typedef struct node {
    char* name;
    // num of children in an array
    int num_children;
    // number of descedants
    struct node** children;
    int score;
} node;

node* find_node(node** nodes, int num_nodes, char* name);
void* malloc_safe(int size);
int compare(const void* v1, const void* v2);
void output_info(node* nodes[], int num_nodes);
int read_tree(node** nodes, int num_lines);
void score_all(node** nodes, int num_nodes, int d);

int main(void) {
    int num_cases, case_num;
    int n, d, num_nodes;
    node** nodes = malloc_safe(sizeof(node) * MAX_NODES);
    scanf("%d", &num_cases);
    for (case_num = 1; case_num <= num_cases; case_num++) {
        printf("Tree %d:\n", case_num);
        scanf("%d %d", &n, &d);
        num_nodes = read_tree(nodes, n);
        score_all(nodes, num_nodes, d);
        qsort(nodes, num_nodes, sizeof(node*), compare);
        output_info(nodes, num_nodes);
        if (case_num < num_cases) printf("\n");
    }
    return 0;
}

node* find_node(node** nodes, int num_nodes, char* name) {
    for (int i = 0; i < num_nodes; i++) {
        if (strcmp(nodes[i]->name, name) == 0) {
            return nodes[i];
        }
    }
    return NULL;
}

void* malloc_safe(int size) {
    char* mem = malloc(size);
    if (mem == NULL) {
        fprintf(stderr, "cannot malloc");
        exit(1);
    };
    return mem;
}

node* new_node(char* name) {
    node* n = malloc_safe(sizeof(node));
    n->name = name;
    n->num_children = 0;
    return n;
}

int read_tree(node** nodes, int num_lines) {
    node *parent_node, *child_node;
    char *parent_name, *child_name;
    int num_nodes = 0;
    int num_children = 0;
    for (int i = 0; i < num_lines; i++) {
        parent_name = malloc_safe(MAX_NAME + 1);
        scanf("%s", parent_name);
        scanf("%d", &num_children);
        parent_node = find_node(nodes, num_nodes, parent_name);
        if (parent_node == NULL) {
            parent_node = new_node(parent_name);
            nodes[num_nodes] = parent_node;
            num_nodes++;
        } else {
            free(parent_name);
            parent_node->children = malloc_safe(sizeof(node) * num_children);
            parent_node->num_children = num_children;
            for (int j = 0; j < num_children; j++) {
                child_name = malloc_safe(MAX_NAME + 1);
                scanf("%s", child_name);
                child_node = find_node(nodes, num_nodes, child_name);
                if (child_node == NULL) {
                    child_node = new_node(child_name);
                    nodes[num_nodes] = child_node;
                    num_nodes++;
                } else {
                    free(child_name);
                }
                parent_node->children[j] = child_node;
            }
        }
    }
    return num_nodes;
}
// Rule 1 If d equals one, then the number of descendants at distance d equals
// the number of children of n. Rule 2 If d is greater than one, then the number
// of descendants at distance d equals the sum of the nodes at distance d – 1 in
// each subtree of n.
int score_one(node* n, int d) {
    int total;
    if (d == 1) {
        return n->num_children;
    }
    total = 0;
    for (int i = 0; i < n->num_children; i++) {
        total = total + score_one(n->children[i], d - 1);
    }
    return total;
}

void score_all(node** nodes, int num_nodes, int d) {
    for (int i = 0; i < num_nodes; i++) {
        nodes[i]->score = score_one(nodes[i], d);
    }
}

// Any qsort comparison function, like this one, has the same signature: it
// takes two void pointers. These pointers are const to signify that we should
// not make any changes to the elements that they point to.Void pointers must be
// cast before we can perform comparisons or oth- erwise access the underlying
// elements. Remember that qsort calls compare with pointers to two elements
// from our array, but, because our array is an array of pointers, what gets
// passed to compare is two pointers to pointers to elements. Therefore, we
// first cast the void pointers to type const node**, and then we apply * to
// give us values for n1 and n2 that are of type const node*. Now we can use n1
// and n2 as pointers to nodes.

int compare(const void* v1, const void* v2) {
    const node* n1 = *(const node**)v1;
    const node* n2 = *(const node**)v2;
    if (n1->score > n2->score) return -1;
    if (n1->score < n2->score) return 1;
    return strcmp(n1->name, n2->name);
}

void output_info(node* nodes[], int num_nodes) {
    int i = 0;
    while (i < 3 && i < num_nodes && nodes[i]->score > 0) {
        printf("%s %d\n", nodes[i]->name, nodes[i]->score);
        i++;
        while (i < num_nodes && nodes[i]->score == nodes[i - 1]->score) {
            printf("%s %d\n", nodes[i]->name, nodes[i]->score);
            i++;
        }
    }
}
