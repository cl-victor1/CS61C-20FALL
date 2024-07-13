#include <stddef.h>
#include "ll_cycle.h"

int ll_has_cycle(node *head) {
    /* your code here */
    node *tortoise; 
    node *hare;
    tortoise = head;
    hare = head;
    if (head == NULL) {
        return 0; // No cycle if the list is empty
    }
    do {
        hare = hare->next;
        if (hare == NULL) {
            return 0;
        }
        hare = hare->next;
        if (hare == NULL) {
            return 0;
        }
        tortoise = tortoise->next;
    }
    while (tortoise != hare);
    return 1;
}