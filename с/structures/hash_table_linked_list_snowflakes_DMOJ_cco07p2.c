#include <stdio.h>
#include <stdlib.h>

#define SIZE 100000

// linked list node
typedef struct node {
    int key[6];
    struct node *next;
} node;

int hash_code(int arr[]);
// void identify_identical(node *buckets[]);
// int identical_right(int node1[], int node2[], int start);
// int identical_left(int node1[], int node2[], int start);
// int are_identical(int node1[], int node2[]);

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

    // identify_identical(buckets);

    return 0;
}

int hash_code(int arr[]) {
    int i, sum = 0;
    for (i = 0; i < 6; i++) {
        sum += arr[i];
    }
    return sum % SIZE;
}

// void identify_identical(node *buckets[]) {
//     node *node_1, *node_2;
//     for (int i = 0; i < SIZE; i++) {
//         node_1 = buckets[i];
//         while (node_1 != NULL) {
//             node_2 = node_1->next;
//             while (node_2 != NULL) {
//                 printf("%d %d", node_1->key[0], node_2->key[0]);
//                 if (are_identical(node_1->key, node_2->key)) {
//                     printf("Twins found");
//                     return;
//                 }
//                 node_2 = node_2->next;
//             }
//             node_1 = node_1->next;
//         }
//     }
//     printf("Twins not found");
// }

// int identical_right(int node1[], int node2[], int start) {
//     int offset;
//     for (offset = 0; offset < 6; offset++) {
//         if (node1[offset] != node2[(start + offset) % 6]) return 0;
//     }
//     return 1;
// }

// int identical_left(int node1[], int node2[], int start) {
//     int offset, node2_index;
//     for (offset = 0; offset < 6; offset++) {
//         node2_index = start - offset;
//         if (node2_index < 0) node2_index = node2_index + 6;
//         if (node1[offset] != node2[node2_index]) return 0;
//     }
//     return 1;
// }

// int are_identical(int node1[], int node2[]) {
//     int start;
//     for (start = 0; start < 6; start++) {
//         if (identical_right(node1, node2, start)) return 1;
//         if (identical_left(node1, node2, start)) return 1;
//     }
//     return 0;
// }