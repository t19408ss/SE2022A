#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct list {
  struct list *next;
  int data;
};

int main() {
  struct list *c, *p;
  int i;
  c = malloc(sizeof(struct list));
  p = c;
  for (i = 0; i < 100; i++) {
    c->data = i + 1;
    c->next = malloc(sizeof(struct list));
    c = c->next;
  }
  for (i = 0; i < 100; i++) {
    printf("%d ", p->data);
    p = p->next;
  }
  printf("\n");
  exit(0);
}
