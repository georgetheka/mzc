#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "mz/type.h"
#include "mz/logger.h"
#include "test/linkedlist.c"
#include "test/arraylist.c"

char *(*testSuite)(void);

int test_runner(char *desc, char *(*testSuite)(void)) {
  char *result = (*testSuite)();
  printf("TEST SUITE: %s\n", desc);
  printf("---------------------------------\n");
  if (result != 0) {
    printf("%s\n", result);
  } else {
    printf("ALL TESTS PASSED\n");
  }
  printf("\n");
  return result != 0;
}

int main(int argc, char *argv[]) {

  int r1 = test_runner("linkedlist", &mz_linkedlist_tests);
  int r2 = test_runner("arraylist", &mz_arraylist_tests);
  printf("TESTS RUN = %d\n", tests_run);

  return r1 == r2 == 1;
}
