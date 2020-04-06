#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

unsigned int ns[] = { 1000, 5000, 10000, 20000, 30000, 40000, 45000, 60000};

// each list node contains an integer key value and pointer to next list node
struct node {
    int key;
    struct node *next;
};

// list's beginning is called the head of list
struct node *head = NULL;


struct node* createNODE(int value, struct node *top) {      //tworzenie nowego wezla, elementu listy
    struct node* newItem = malloc(sizeof(struct node));
    newItem->key = value;
    newItem->next = top;
    return newItem;
}

struct node* list_insert(int value) {      //dodanie elementu na poczatek listy
    struct node* newItem = malloc(sizeof(struct node));
    head = createNODE(value, head);
    return head;
}

struct node* list_search(int value) {     //szukanie elementu od poczatku listy
    struct node *iter = head;
    while ((iter != NULL) && (iter->key != value)) {
        iter = iter->next;
    }
    return iter;
}

void list_delete(int value) {
    if (head->key == value) {    //usuwanie pierwszego elementu z listy
        head=head->next;
    }
    else {
        struct node* prev = head;     //usuwanie elementu w srodku listy
        struct node* iter = head->next;
        while ((iter != NULL) && (iter->key != value)) {
            prev = iter;
            iter = iter->next;
        }
        if (iter != NULL) {prev->next = iter->next;}
    }
}

unsigned int list_size() {      //liczba elementów na liscie
    int size = 0;
    struct node* iter = head;
    while (iter != NULL){
        size= size +1;
        iter = iter ->next;
    }
    return size;
}

/*
 * Fill an array with increasing values.
 *
 * Parameters:
 *      int *t:     pointer to the array
 *      int n:      number of elements in the array
 */
void fill_increasing(int *t, int n) {
    for (int i = 0; i < n; i++) {
        t[i] = i;
    }
}

/*
 * Reorder array elements in a random way.
 *
 * Parameters:
 *      int *t:     pointer to the array
 *      int n:      number of elements in the array
 */
void shuffle(int *t, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % i;
        int temp = t[i];
        t[i] = t[j];
        t[j] = temp;
    }
}

int main() {
    bool no_yes[] = { false, true };

    for (unsigned int i = 0; i < sizeof(no_yes) / sizeof(*no_yes); i++) {
        bool enable_shuffle = no_yes[i];

        for (unsigned int j = 0; j < sizeof(ns) / sizeof(*ns); j++) {
            unsigned int n = ns[j];

            // always create an array of size `n` and fill it with increasing values
            int *t = malloc(n * sizeof(*t));
            fill_increasing(t, n);

            // if true, reorder array elements randomly
            if (enable_shuffle) {
                shuffle(t, n);
            }

            // insert elements in the order present in array `t`
            clock_t insertion_time = clock();
            for (unsigned int k = 0; k < n; k++) {
                struct node *iter = list_insert(t[k]);
                assert(iter != NULL);       // inserted element cannot be NULL
                assert(iter->key == t[k]);  // inserted element must contain the expected value
            }
            insertion_time = clock() - insertion_time;

            // reorder array elements before searching
            shuffle(t, n);

            // search for every element in the order present in array `t`
            clock_t search_time = clock();
            for (unsigned int k = 0; k < n; k++) {
                struct node *iter = list_search(t[k]);
                assert(iter != NULL);       // found element cannot be NULL
                assert(iter->key == t[k]);  // found element must contain the expected value
            }
            search_time = clock() - search_time;

            // reorder array elements before deletion
            shuffle(t, n);

            // delete every element in the order present in array `t`
            for (unsigned int k = 0, l = n; k < n; k++, l--) {
                assert(list_size() == l);   // list size must be equal to the expected value
                list_delete(t[k]);
            }
            assert(list_size() == 0);       // after all deletions, the list size is zero
            assert(head == NULL);           // after all deletions, the list's head is NULL

            free(t);

            printf("%d\t%s\t%f\t%f\n", n, enable_shuffle ? "true" : "false",
                    (double)insertion_time / CLOCKS_PER_SEC,
                    (double)search_time / CLOCKS_PER_SEC);
        }
    }
    return 0;
}
