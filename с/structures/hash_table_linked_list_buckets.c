#include <stdio.h>
#include <stdlib.h>

#define SIZE 100000

int hash_code(int arr[]);

// linked list node
typedef struct node {
    int key[6];
    struct node *next;
} node;

int main(void) {
    // defining a one-dimensional array of pointers to nodes (bucket as a linked
    // list)
    static node *buckets[SIZE] = {NULL};

    // defining a pointer to the nodes allocated
    node *bucket;

    // filling the keys of nodes
    int n, i, j, hash_index;
    scanf("%d", &n);
    for (i = 0; i < n; i++) {
        bucket = malloc(sizeof(node));
        if (bucket != NULL) {
            for (j = 0; j < 6; j++) {
                scanf("%d", &bucket->key[j]);
            }
            // the use of the hash function
            hash_index = hash_code(bucket->key);
            // point the inserted node next pointer to the first node in the
            // list
            bucket->next = buckets[hash_index];
            // set the start of the list to point to the inserted node
            buckets[hash_index] = bucket;
        } else {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }

        for (int i = 0; i < n; i++) {
            node *curr = buckets[i];
            while (curr != NULL) {
                node *temp = curr->next;
                free(curr);
                curr = temp;
            }
        }
    }

    // freeing memory
    for (int i = 0; i < n; i++) {
        node *curr = buckets[i];
        while (curr != NULL) {
            node *temp = curr->next;
            free(curr);
            curr = temp;
        }
    }

    return 0;
}

int hash_code(int arr[]) {
    int i, sum = 0;
    for (i = 0; i < 6; i++) {
        sum += arr[i];
    }
    return sum % SIZE;
}