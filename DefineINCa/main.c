
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define SENS_DE_LA_VIE 3.14

#define INC(a) a++ 
#define MULTI_LINE(a,b) a = b; \
                        b = 0; 


#ifdef DEBUG
// Je souligne qu'il est rarement conseillé d'utiliser des printf() en debug
// et que nous avons brisé la règle du nom des macros en MAJ.
#define return printf("(%s:%d)\n", __FUNCTION__, __LINE__);  return
#endif /* ! DEBUG */

#if (X == 1)
#define Y 2
#elif (X == 2)
#define Y "Ami de la bonne blague, bonjour !"
#else
#define Y NULL
#endif /* ! X */

#define DEBUG_PRNTF(fmt, ...) printf("LOG" ## fmt, __VA_ARGS__);


typedef struct {
    const char *name;        
    const char *ansi_code;  
    const char *description;
    unsigned int rgb;      
} Color;

#define X(NAME, ANSI, DESC, RGB) { #NAME, ANSI, DESC, RGB },
Color colors[] = {
    #include "color.def"
};
#undef X

#define X(NAME, ANSI, DESC, RGB) printf("%s (%s) = %s\n", #NAME, DESC, #RGB);
void print_colors() {
    // Bien entendu, on pourrait itérer sur la structure créée mais c'est une illustration
    #include "color.def"
}
#undef X


int main(int argc, char *argv[])
{

  printf("%f\n", SENS_DE_LA_VIE);

    #if (X == 1)
    printf("%d\n", Y);
    #elif (X == 2)
    printf("%s\n", Y);
    #else 
    printf("%p\n", Y);
    #endif /* ! X */
  return EXIT_SUCCESS;
}
