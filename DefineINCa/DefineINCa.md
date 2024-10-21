Le but de cet article est de vous faire découvrir le magnifique univers des macros en C.

# Une directive préprocesseur
En C, les lignes qui commencent par un # sont interprétées par le compilateur lors de la compilation des fichiers sources. On les appelle des directives du préprocesseur. Les macros en font partie.

Petit point historique : 
> Les macros en langage C ont été introduites avec la première norme du langage C, appelée ANSI C (ou C89), 
> qui a été standardisée par l'[American National Standards Institute](https://www.ansi.org) (ANSI) en 1989.
> 
> Cependant, avant cette standardisation, les macros faisaient déjà partie du langage C classique (ou K&R C) utilisé dans les années 1970. 
> Le compilateur C original, développé par Dennis Ritchie pour le système d'exploitation UNIX, incluait déjà une forme rudimentaire de macros via le préprocesseur, permettant des définitions avec #define.

# Define 
```c
#define SENS_DE_LA_VIE 3.14

/* ... */

printf("%f\n", SENS_DE_LA_VIE);
```
Le `define` a un fonctionnement assez simple à comprendre : le compilateur remplace toutes les occurrences dans le code par la valeur définie. Il fonctionne avec la syntaxe suivante `#define <MACRO_NAME> <value>`. On a pour convention de mettre le nom en majuscule, la valeur quant à elle est optionnelle.

Un peu comme un "Ctrl-f et remplacer".

## Mama, la macro
On peut utiliser les `define` pour définir des fonctions que l'on pourra utiliser dans notre code.

```c
#define INC(a) a++ 
#define MULTI_LINE(a,b) a = b; \
                        b = 0; 


INC(my_variable); 
MULTI_LINE(my_variable, foobar) 
// Je souligne le fait qu'il peut ne pas y avoir de ';' en fin de ligne 

// Cela donnera  
my_variable++;
my_variable = foobar;
foobar = 0;
```

## If or not if
Nous pouvons déclarer des macros de manière conditionnelle. 
Si un nom est déjà défini alors on exécute le bout de code suivant. 
```c
#ifdef DEBUG
// Je souligne qu'il est rarement conseillé d'utiliser des printf() en debug
// et que nous avons brisé la règle du nom des macros en MAJ.
#define return printf("(%s:%d)\n", __FUNCTION__, __LINE__);  return
#endif /* ! DEBUG */

int main(void) {
    return 1;
}
```
Dans ce cas, j'utilise un `#ifndef`, mais il existe aussi :
- `#ifdef`
- `#if`
- `#else`
- `#elif`
```c
#if (X == 1)
#define Y 2
#elif (X == 2)
#define Y "Ami de la bonne blague, bonjour !"
#else
#define Y NULL
#endif /* ! X */

/* ... */

int main(void) {
    #if (X == 1)
    printf("%d\n", Y);
    #else 
    printf("%s\n", Y);
    #endif /* ! X */
}
```

On aime bien signaler avec un commentaire en bloc la fin des `#if`. C'est une convention qui permet de mieux se repérer dans le code. 

## Macros prédéfinies
Vous avez pu voir dans l'exemple précédent que j'utilisais les mots-clés `__FUNCTION__` et `__LINE__`. 
Comme vous pouvez vous en douter, ce sont des macros que le compilateur va remplacer par la bonne valeur.

Il existe une liste de macros prédéfinies [Common Predifined](https://gcc.gnu.org/onlinedocs/cpp/Common-Predefined-Macros.html).

À noter qu'il existe des macros dites [System specific](https://gcc.gnu.org/onlinedocs/cpp/System-specific-Predefined-Macros.html).

Petite liste non exhaustive : 
- `__DATE__` : Jan 14 2012
- `__GNUC__` : Version majeure de GCC
- `__TIME__` : 15:12:18
- `__INCLUDE_LEVEL__` : La profondeur des includes en commençant par 0
- `__BASE_FILE__` : Le nom du fichier actuel

## Vers l'infini et au-delà des arguments
```c
// Ici, l'opérateur ## est l'opérateur de concaténation
#define DEBUG_PRNTF(fmt, ...) printf("LOG" ## fmt, __VA_ARGS__);
```
Ici, on peut voir que l'on génère des macros variadiques, surtout utiles lors de la création de logs. 
(Même si ce n'est pas une bonne idée de faire des logs avec des `printf`.)

## X-Macro

Pour cela, nous allons devoir créer un fichier externe, souvent nommé en `*.def` bien qu'il n'existe pas de convention.
```c
// color.def
X(NC, "\e[0m", "No Color", 0x000000) 
X(BLACK, "\e[0;30m", "Black", 0x000000) 
X(GRAY, "\e[1;30m", "Gray", 0x808080) 
X(RED, "\e[0;31m", "Red", 0xFF0000) 
X(LIGHT_RED, "\e[1;31m", "Light Red", 0xFF8080) 
X(GREEN, "\e[0;32m", "Green", 0x00FF00) 
X(LIGHT_GREEN, "\e[1;32m", "Light Green", 0x80FF80) 
X(BROWN, "\e[0;33m", "Brown", 0xA52A2A) 
X(YELLOW, "\e[1;33m", "Yellow", 0xFFFF00) 
X(BLUE, "\e[0;34m", "Blue", 0x0000FF) 
X(LIGHT_BLUE, "\e[1;34m", "Light Blue", 0xADD8E6) 
X(PURPLE, "\e[0;35m", "Purple", 0x800080) 
X(LIGHT_PURPLE, "\e[1;35m", "Light Purple", 0xEE82EE) 
X(CYAN, "\e[0;36m", "Cyan", 0x00FFFF) 
X(LIGHT_CYAN, "\e[1;36m", "Light Cyan", 0xE0FFFF) 
X(LIGHT_GRAY, "\e[0;37m", "Light Gray", 0xD3D3D3) 
X(WHITE, "\e[1;37m", "White", 0xFFFFFF)
```

```c
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
```
Ce genre de macro est extrêmement utile. Je dois reconnaître qu'on la retrouve rarement dans un code source, mais elle permet de modifier le fonctionnement du programme sans pour autant devoir modifier le code source. Fun fact, elle est souvent utilisée dans la création de kernels. Elle permet de générer les structures globales comme l'IDT et la GDT.

## Les problèmes 
__**Attention**__ : Petite mise au point d'abord, les macros sont des outils formidables mais il faut faire attention. Vous ne devez surtout pas utiliser ce genre de macro :
```c
#define MIN(a,b) (a < b ? a : b)
```

Prenons un exemple : `MIN(2 + 5, fibo(25))`
### Problème n°1 
`MIN(2 + 5, fibo(25))` => `(2 + 5 < fibo(25) ? 2 + 5 : fibo(25))`

Ici le problème est la priorité de calcul. Le compilateur va d'abord effectuer la comparaison puis l'addition, donc 2 + (1). On corrige cela par l'ajout de parenthèses en utilisant les arguments des macros.
```c
#define MIN(a,b) ((a) < (b) ? (a) : (b))
```
Comme vous ne savez jamais ce que vos utilisateurs vont passer en paramètre, mettez toujours des parenthèses sur les arguments.
### Problème n°2 
`MIN(2 + 5, fibo(25))` => `(2 + 5 < fibo(25) ? 2 + 5 : fibo(25))`

On remarque que le compilateur fait un remplacement bête et méchant, ce qui veut dire que l'on va calculer deux fois `fibo(25)`. Je vous laisse imaginer si c'est une implémentation récursive.

Pour fixer ce problème, nous déclarons une variable intermédiaire avant le `if`. 
## Macros utiles

```c
#define MIN(a, b)                                                              \
    ({                                                                         \
        __typeof__(a) _a = a;                                                  \
        __typeof__(b) _b = b;                                                  \
        (_a) > (_b) ? (_b) : (_a);                                             \
    })

#define ABS(a)                                                                 \
    ({                                                                         \
        __typeof__(a) _a = a;                                                  \
        0 < (_a) ? (_a) : -(_a);                                               \
    })

#define MAX(a, b)                                                              \
    ({                                                                         \
        __typeof__(a) _a = a;                                                  \
        __typeof__(b) _b = b;                                                  \
        (_a) < (_b) ? (_b) : (_a);                                             \
    })

#define CLAMP(a, x, b) MAX(a, MIN(x, b))
// Pour les tableaux uniquement
#define COUNT_OF(arr) sizeof(arr) / sizeof(arr[0])
```

## Là, on s'amuse
Ici, c'est du code purement overkill juste pour le fun. Je ne vous conseille pas forcément d'utiliser ces macros dans votre code.
Je me fais juste plaisir (faut bien dans la vie).

### Un auto free

```c
#define DEFER(free_call) __attribute__((cleanup(free_call)))

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
```

Je vous laisse tester avec un petit `-fsanitize=address`. C'est vraiment une dinguerie. On pourrait même voir une amélioration de la fonction auto_free qui prend en paramètre une chaîne de caractères du nom de notre structure pour faire un switch.

### Get time

Fonction plus chill où l'on calcule juste le temps d'exécution de notre fonction. Très utile pour faire du benchmark.

```c
#define MEASURE_TIME(block) {                                \
    clock_t start_time = clock();                            \
    block                                                    \
    clock_t end_time = clock();                              \
    double elapsed = ((double)(end_time - start_time)) / CLOCKS_PER_SEC * 1000.0; \
    printf("Execution time: %.3f ms\n", elapsed);            \
}
```

### Define Error

Petite X-macro qui prend une macro en argument et qui l'expand.

```c
#define ERROR_LIST(X)          \
    X(ERROR_FILE_NOT_FOUND, "File not found")    \
    X(ERROR_INVALID_INPUT, "Invalid input")      \
    X(ERROR_OUT_OF_MEMORY, "Out of memory")      \
    X(ERROR_UNKNOWN, "Unknown error")

#define DEFINE_ERROR_ENUM(code, message) code,
enum ErrorCode {
    ERROR_LIST(DEFINE_ERROR_ENUM)
};

#define DEFINE_ERROR_STRING(code, message) case code: return message;
const char* get_error_message(enum ErrorCode error_code) {
    switch (error_code) {
        ERROR_LIST(DEFINE_ERROR_STRING)
        default: return "Unrecognized error";
    }
}

/* ... */
enum ErrorCode error = ERROR_OUT_OF_MEMORY;
printf("Error: %s\n", get_error_message(error));
```

### Génération de tests automatisés

Ici, on génère carrément des fonctions entières avec une macro, parce que le C n'a aucune limite. Moi aussi :eyes:
```c
#define GENERATE_TEST_FUNC(func, test_value, wanted_value) \
    void test_##func(void) { \
        printf("Test de " #func " avec valeur %d : ", test_value); \
        if (func(test_value)) { \
            printf("Succès\n"); \
        } else { \
            printf("Échec\n"); \
        } \
    }

/* ... */
GENERATE_TEST_FUNC(fibo, 10, 55);

test_fibo();
```

## RTFM

Il est maintenant l'heure de conclure. Nous avons vu plein de choses très cool. Et si jamais vous êtes tentés, libre à vous de découvrir les macros par vous-même.
Donc, conclusion : **RTFM**.

> PS : Pour ce qui est du titre, les macros ne sont pas récursives, elles ne s'expandent qu'avec une profondeur de 1.
