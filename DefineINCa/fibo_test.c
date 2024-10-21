#include <stdio.h>
#include <stdlib.h>

#define GENERATE_TEST_FUNC(func, test_value, wanted_value) \
    void test_##func(void) { \
        printf("Test de " #func " avec valeur %d : ", test_value); \
        if (func(test_value)) { \
            printf("Succès\n"); \
        } else { \
            printf("Échec\n"); \
        } \
    }

int fibo(int n) {
  return 1;
}

GENERATE_TEST_FUNC(fibo, 10, 55);
int main(int argc, char *argv[])
{
  test_fibo();
  return EXIT_SUCCESS;
}
