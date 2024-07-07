#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#ifndef N
#define N 5
#endif
#ifndef FS
#define FS 38
#endif

struct node {
  int data;
  int fibdata;
  struct node* next;
};

int fib(const int n) {
  if (n < 2) {
    return n;
  } else {
    int x = fib(n - 1);
    int y = fib(n - 2);
    return x + y;
  }
}

void processwork(struct node* p) {
  int n = p->data;
  p->fibdata = fib(n);
}

struct node* init_list(struct node* p) {
  struct node* head = (struct node*)malloc(sizeof(struct node));
  p = head;
  p->data = FS;
  p->fibdata = 0;
  for (int i = 0; i < N; i++) {
    struct node* temp  =  (struct node*)malloc(sizeof(struct node));
    p->next = temp;
    p = temp;
    p->data = FS + i + 1;
    p->fibdata = i+1;
  }

  p->next = nullptr;

  return head;
}

int main(int argc, char *argv[]) {

  struct node *p = nullptr;
  struct node *temp = nullptr;
  struct node *head = nullptr;

  printf("Process linked list\n");
  printf("  Each linked list node will be processed by function 'processwork()'\n");
  printf("  Each ll node will compute %d fibonacci numbers beginning with %d\n",N,FS);

  p = init_list(p);
  head = p;

  const double start = omp_get_wtime();

  #pragma omp parallel num_threads(8)
  {
    #pragma omp single
    while (p != nullptr) {
      #pragma omp task firstprivate(p)
      processwork(p);
      p = p->next;
    }
  }

  const double end = omp_get_wtime();
  p = head;
  while (p != nullptr) {
    printf("%d : %d\n",p->data, p->fibdata);
    temp = p->next;
    free (p);
    p = temp;
  }
  free (p);

  printf("Compute Time: %f seconds\n", end - start);

  return 0;
}

