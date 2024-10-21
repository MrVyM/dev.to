#include <stdlib.h>
#define DEFER(free_call) __attribute__((cleanup(free_call)))

#define MEASURE_TIME(block) {                                \
    clock_t start_time = clock();                            \
    block                                                    \
    clock_t end_time = clock();                              \
    double elapsed = ((double)(end_time - start_time)) / CLOCKS_PER_SEC * 1000.0; \
    printf("Execution time: %.3f ms\n", elapsed);            \
}

void auto_free(void *ptr) {
    void **p = (void **)ptr;
    if (*p) {
        free(*p);
        *p = NULL;
    }
}

int main() {
  DEFER(auto_free) char* buffer = malloc(10);
  return 0;
}
