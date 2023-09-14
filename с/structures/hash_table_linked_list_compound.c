#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_BITS 17
#define WORD_LENGTH 16
#define hashsize(n) ((unsigned long)1 << (n))
#define hashmask(n) (hashsize(n) - 1)

typedef struct word_node {
    char **word;
    struct word_node *next;
} word_node;

char *read_line(int size);
int in_hash_table(word_node *hash_table[], char *find, unsigned find_len);
unsigned long oaat(char *key, unsigned long len, unsigned long bits);
void identify_compound_words(char *words[], word_node *hash_table[],
                             int total_words);

int main(void) {
    // 1 << NUM_BITS is a shortcut for computing 2 in power of 17. The ooat
    // function needs this type of a number we store each word in thw words
    // array
    static char *words[1 << NUM_BITS] = {NULL};
    // and store a pointer to that word in the hash table
    static word_node *hash_table[1 << NUM_BITS] = {NULL};
    // counting total numbers of words
    int total = 0;
    // pointers to a word and a word_node respectively
    char *word;
    word_node *wordptr;
    unsigned length, word_code;
    // reading a word
    word = read_line(WORD_LENGTH);
    // while there is a word
    while (*word) {
        // storing the word
        words[total] = word;
        // storing a pointer to the word
        wordptr = malloc(sizeof(word_node));
        if (wordptr == NULL) {
            fprintf(stderr, "malloc error\n");
            exit(1);
        }
        length = strlen(word);
        word_code = oaat(word, length, NUM_BITS);
        // word can store the memory address of a pointer to a character array
        // that's why &
        wordptr->word = &words[total];
        // inserting a word in the start of the linked list by hash code
        wordptr->next = hash_table[word_code];
        hash_table[word_code] = wordptr;
        word = read_line(WORD_LENGTH);
        total++;
    }
    identify_compound_words(words, hash_table, total);
    return EXIT_SUCCESS;
}

char *read_line(int size) {
    char *str;
    int ch;
    int len = 0;
    str = malloc(size);
    if (str == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    while ((ch = getchar()) != EOF && (ch != '\n')) {
        str[len++] = ch;
        if (len == size) {
            str = realloc(str, size);
            if (str == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                exit(1);
            }
        }
    }
    str[len] = '\0';
    return str;
}

int in_hash_table(word_node *hash_table[], char *find, unsigned find_len) {
    unsigned word_code;
    word_node *word_ptr;
    word_code = oaat(find, find_len, NUM_BITS);
    word_ptr = hash_table[word_code];
    while (word_ptr) {
        if ((strlen(*(word_ptr->word)) == find_len) &&
            (strncmp(*(word_ptr->word), find, find_len) == 0)) {
            return 1;
        }
        word_ptr = word_ptr->next;
    }
    return 0;
}

// it generates all possшble splits, and then searches the hash table for both
// the prefix string (prior to the split point) and suffix string (from the
// split point on).
void identify_compound_words(char *words[], word_node *hash_table[],
                             int total_words) {
    int i, j, len;
    for (i = 0; i < total_words; i++) {
        len = strlen(words[i]);
        for (j = 1; j < len; j++) {
            if (in_hash_table(hash_table, words[i], j) &&
                in_hash_table(hash_table, &words[i][j], len - j)) {
                printf("%s", words[i]);
                break;
            }
        }
    }
}

unsigned long oaat(char *key, unsigned long len, unsigned long bits) {
    unsigned long hash, i;
    for (hash = 0, i = 0; i < len; i++) {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    // limit the hash value to the desired number of bits.
    return hash & hashmask(bits);
}
