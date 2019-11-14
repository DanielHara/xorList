#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "cmocka.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define true 1
#define false 0

typedef uintptr_t ADDRESS;

typedef struct noh {
    int val;
    ADDRESS both;
} node;

typedef node* list;

typedef struct doublyLinkedListNode {
    list begin;
    list end;
} doublyLinkedList;

uintptr_t exclusiveOR (list p, list q) {
    return (uintptr_t)p ^ (uintptr_t)q;
}

// Prints the list
void printList (list lista) {
    printf("List:\n");

    list p = lista;
    if (p == NULL) {
        return;
    }

    list ant = NULL;
    while (p != NULL) {
        printf("%d ", p->val);

        list prox = exclusiveOR(p->both, ant);
        ant = p;
        p = prox;
    }
    printf("\n");
}

list listAt(list lista, int index) {
    if (index < 0) {
        return NULL;
    }

    list p = lista;
    int i = 0;

    list ant = NULL;
    while (i < index) {
        if (p == NULL) {
            return NULL;
        }

        list prox = exclusiveOR(p->both, ant);
        ant = p;
        p = prox;

        i = i + 1;
    }

    return p;
}

// Inserts val to the end of the list
doublyLinkedList add (int val, list lista) {
    list p = lista;
    list r = malloc(sizeof(node));
    r->val = val;

    if (p == NULL) {
        r->both = NULL;
        return (doublyLinkedList){ r, r };
    }

    list ant = NULL;
    while (p != NULL && exclusiveOR(p->both, ant) != NULL) {
        list prox = exclusiveOR(p->both, ant);
        ant = p;
        p = prox;
    }

    p->both = exclusiveOR(r, ant);
    r->both = p;
    return (doublyLinkedList){ lista, r };
}

static void test_several_insertion_list(void **state) {
    int n = 10;

    doublyLinkedList s = {
        begin: NULL,
        end: NULL
    };

    for (int i = 0; i < n; i++) {
        s = add(i, s.begin);
    }

    for (int i = 0; i < n; i++) {
        assert_int_equal(listAt(s.begin, i)->val, i);
        assert_int_equal(listAt(s.end, i)->val, n - i - 1);
    }

    assert_null(listAt(s.begin, n));
    assert_null(listAt(s.end, n));
}

static void test_first_insertion_list(void **state) {
    (void) state; /* unused */

    doublyLinkedList s = {
        begin: NULL,
        end: NULL
    };

    assert_null(listAt(s.begin, 0));
    assert_null(listAt(s.begin, 1));

    s = add(0, s.begin);
    assert_int_equal(listAt(s.begin, 0)->val, 0);
    assert_int_equal(listAt(s.end, 0)->val, 0);
}

int main() {
    const struct CMUnitTest tests[] = {
       cmocka_unit_test(test_first_insertion_list),
       cmocka_unit_test(test_several_insertion_list),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
