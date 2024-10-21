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
