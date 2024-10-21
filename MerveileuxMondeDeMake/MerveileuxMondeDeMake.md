# Le merveilleux monde de Make

Les Makefiles constituent un outil essentiel dans le développement de logiciels que ce soit en C/C++ ou autre. Ils permettent une gestion efficace des projets en automatisant le processus de compilation, de nettoyage et de tests. 

Dans cet article, nous explorerons les bonnes pratiques pour la création et l'utilisation de Makefiles dans des projets C quelque soit leurs complexites.

### Make, ca sert a quoi ?

Make est un programme qui a pour but de générer des fichier. Il permet de générer des pdfs, des exécutables et bien plus.

> Dans les années 1970, la compilation des programmes devient de plus en plus longue et complexe, nécessitant de nombreuses étapes interdépendantes. La plupart des systèmes alors utilisés reposent sur des script shell, nécessitant de répéter toutes les étapes lors de la moindre correction. C'est dans ce contexte que Make fut développé par le docteur Stuart Feldman en 1977. En gérant les dépendances entre fichiers sources et fichiers compilés, Make permet de ne compiler que ce qui est nécessaire à la suite de la modification d'un fichier source.

Il existe un makefile par défaut, qui définit un grand nombre de règles afin de générer la plupart des fichiers avec lesquels, vous serez amené a travailler.

Pour voir ce makefile, par défaut : `make -p`

### Hello World 

Pour débuter, nous allons prendre un cas très simple celui d'un fichier source unique, dans le même dossier que notre makefile. 

```sh
.
└── main.c 
```
Dans ce cas, il existe 2 facons de faire.

```sh
$ make main # Je souligne l'absence du .c
```

Dans cette commande, nous demandons a make de créer le fichier `main`. Mais comment sait-il qu'il doit utiliser le fichier main.c pour faire notre binaire ? Cela est défini dans les règles par défaut.

Nous pouvons aussi creer un fichier `Makefile`

```makefile=
# Makefile
my_compilation_rule: 
    gcc main.c
```

Une fois, le fichier crée, il suffit d'exécuter la commande `make` (qui, par défaut, exécute la première règle du fichier).
Nous pouvons aussi mettre en argument le nom de la règle que nous voulons exécuter `make my_compilation_rule`.

### Plus on est de fou, plus on rit

Notre projet grossit, nous avons maintenant un deuxième fichier source.
```sh
.
├── foo.c
├── foo.h
└── main.c 
```

Dans l'objectif de faire un makefile propre, nous allons déclarer une variable OBJ qui contiendra nos différents fichiers. On en profite pour déclarer un variable CC pour notre compilateur.

```makefile
# makefile
CC = gcc
OBJ = main.o foo.o


# Cette variable sera utilisée dans la règle implicite qui compile les .o  
CFLAGS = # My C Flags for the compiler
LDFLAGS = # My linker flags
TARGET = a.out

all: $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)
```

Cette fois-ci, nous avons ajouter des dépendances à notre règle `all`. Maintenant, make sait qu'il faut d'abord faire les fichiers .o avant de pouvoir faire le linkage de tous les fichiers en un binaire.

```makefile
# makefile
SRC = $(wildcard *.c) # main.c foo.c
OBJ = $(SRC:.c=.o) # main.o foo.o
```

Dans ce makefile, nous avons choisi de ne pas récupérer, à la main, les différents fichiers source. Nous récupérons automatiquement les fichiers source via la fonction `wildcard`. Cette fonction agit comme le globbing d'un shell. Dans cette example, elle récupère tous les fichiers terminant par `.c`. 

Nous les convertissons en `.o` via cette syntax `$(NAME:old=new)` qui n'est rien de moins qu'un replace. La suite de notre makefile reste néanmoins la même chose.

### Clean

Comme vous avez pu le remarquer, un makefile peut facilement générer un grand nombre de fichiers. C'est pour cela que nous allons créer une nouvelle règle dans notre makefile qui a pour but de supprimer tous les fichiers issus de la compilation. 

```makefile

# ...

clean: 
    $(RM) $(OBJ)
    $(RM) $(TARGET)
```

Pour les petits curieux qui se demandent ce que signifie la variable RM. C'est un simple alias avec la commande `rm -f`. Il faut savoir que cette variable peut très bien override. On peut très bien voir `RM = echo` ou bien `RM = mv to/path/` (à la place du remove), cela peut être très utile dans certains projets.

#### Phony

Nous l'avons dit au début de cet article, mais make est un utilitaire qui permet de générer des fichiers. Mais que se passe-t-il, si l'on appelle la règle clean et qui existe un fichier `clean` dans notre dossier. 
```sh
make: 'clean' is up to date.
```

Pour éviter ce genre de problème, il faut spécifier dans notre makefile que notre règle n'a pas pour but de générer un fichier. Pour cela, nous utilisons le mot clé `phony`.

Il suffit d'écrire. 
```makefile
.PHONY: my_rule1 my_rule12 # ... 
```

### On range le repo
Maintenant, que nous avons réussi à faire un makefile propre, nous allons pouvoir faire évoluer notre projet en mettant nos fichiers dans un dossier `src`.

```
.
├── Makefile
└── src/
    ├── foo.c
    ├── foo.h
    └── main.c
```

Pour cela, il nous suffit de rechercher la liste de nos .c dans le dossier src/ puis de les transformer en .o. On peut modifier le code que nous avons fait ci-dessus.

```makefile=
SRCDIR = src
SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(SRC:$(SRCDIR)/%.c=$(SRCDIR)/%.o)
# qui est équivalent à
# OBJ = src/main.o \
#       src/main.o

```

### Where GDB ?
Il peut être utile d'avoir un règle debug, afin de ne pas avoir à mettre un `-g` dans nos CFLAGS par défaut.

Pour cela, il suffit de :
```makefile
debug: CFLAGS += -g
debug: all
```
On remarque que la règle debug n'a pas de corps. Elle se contente d'ajouter un flag et d'appeler notre règle `all`.

### Tester, c'est tricher 

Pour cet article, nous allons voir comment faire un Makefile pour une test suite criterion.
```
.
├── Makefile
├── src/
...
└── tests/
    └── foo_test.c
```

Pour pouvoir lancer, nos tests, il va falloir que l'on trouve les fichiers source du dossier `tests`, puis linker tous nos fichiers en excluant le fichier main.o.

```makefile
TEST_DIR = tests
SRC_TEST = $(wildcard $(TEST_DIR)/*.c)
OBJ_TEST = $(SRC:$(TEST_DIR)/%.c=$(TEST_DIR)/%.o)

# ... 

check: $(filter-out $(SRC_DIR)/main.o, $(OBJ)) $(OBJ_TEST)
$(CC) $^ -o $(TARGET) $(LDFLAGS)
```
On remarque que le fichier main.o est exclus en utilisant la fonction `filter-out`. On voit aussi l'appel d'une variable pour le moment inconnu. Il en existe un grand nombre.

- `$^`  : La liste des dépendances de la règle

- `$<` : Le nom de la première dépendance

- `$@` : Le nom de la règle


### On teste les tests

Si l'on parle de test, nous devons forcément parler de coverage. Pour cela rien de plus simple, nous ajoutons les flags a nos variables puis exécutons `GCOVR`

```makefile=
coverage: CFLAGS += -fPIC --coverage
coverage: LDFLAGS += -lgcov -lcriterion
coverage: check
gcovr --print-summary
```

### Like a pro
La création d'un dossier build est la cerise sur le gâteau. Elle permet au développeur de faire `rm -rf build/` pour faire un clean.

Pour cela, nous allons devoir modifier notre variable `OBJ` et nous allons devoir créer un règle pour la compilation des .o. Nous ne pouvons plus utiliser les règles implicite.

```makefile
BUILD_DIR = build
OBJ = $(SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Compilation des fichiers source
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
$(CC) $(CFLAGS) -c $< -o $@
    
# Creer le repertoire de build
$(BUILD_DIR):
mkdir -p $(BUILD_DIR)
```
Alors on remarque que cette fois-ci, nous n'avons pas défini une règle, mais avec une variable. Nous avons même utilisé une sorte de regex pour la définir. 

Nous avons aussi dû mettre un prérequis en plus, la règle $(build_dir). Le fait de mettre un `|` signifie qu'il doit d'abord vérifier qu'il est nécessaire de faire cette règle.

## Bonus

### Bibliothèque statique
Pour créer une bibliothèque statique à partir des fichiers source, vous pouvez ajouter des règles au Makefile :
```makefile
LIB_TARGET = libmylib.a
LIB_OBJS = $(filter-out $(BUILD_DIR)/main.o, $(OBJS))

$(LIB_TARGET): $(LIB_OBJS)
ar rcs $@ $^
```
Une fois, la librairie crée, vous pourrez simplement la link avec votre binaire.

### Makefile récursif
Si votre projet comporte des sous-modules, vous pouvez utiliser un Makefile récursif. Imaginons un projet avec la structure suivante.

```sh
.
├── Makefile
└── src/
    ├── features1/
        ...
        └── Makefile
    ├── features2/
        ...
        └── Makefile
    ├── features3/
        ...
        └── Makefile
    ├── features4/
        ...
        └── Makefile
    ├── main.c
    └── Makefile
```
Pour cela, il y a deux approches possibles. La première serait de faire un seul makefile à la racine de notre projet et faire un find de nos fichiers .c. Nous verrons comment faire par la suite.

La deuxième, c'est de faire une structure récursif via le Makefile suivant. 
```makefile
# src/makefile 
SUBDIRS = $(wildcard */.)

.PHONY: subdirs $(SUBDIRS)

subdirs: $(SUBDIRS)

$(SUBDIRS):
    $(MAKE) -C $@
```

Avec ce fichier, nous allons pouvoir mettre un makefile dans chaque dossier features et lui et lui seul gérera la compilation de la features. Ils seront appelés par le `src/Makefile` via la règles subdirs.
```makefile
# Makefile

export CFLAGS = -Wall -Werror -pedantic

# ...
```
N'oubliez pas d'export vos variables dans votre Makefile afin qu´elle s'applique sur les Makefiles suivants.

Ce genre d'architecture nécessite, la plupart du temps, de la combiner avec la création de librairie statique.
### Répertoires infinis

Nous avons vu comment utiliser la fonction `wildcard` mais comment faire pour recuperer l'intégralité de nos fichiers sources ?

```sh
├── Makefile
└── src/
    └── foo/
        └── bar/
            ...
                └── foobar/
                    └── barfoo/
                        └── here.c
```
La prise en charge des répertoires "infinis" peut se faire via des règles génériques ou bien par l'intermédiaire d'un sub-shell.

Par exemple :
```makefile 
# Attention, il faut que votre systeme possede la command find

SRC = $(shell find $(SRC_DIR) -name "*.c")
```  
ou bien
```makefile
# Je laisse les plus aventureux d'entre vous dechiffrer ce code

rwildcard = $(foreach d, $(wildcard $(1:=/*)), $(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))
SRC = $(rwildcard src,*.c)
```
ou encore 
```makefile
SRC = $(wildcard src/*.c) \
      $(wildcard src/*/*.c) \
      # ... # 
      $(wildcard src/*/*/*/*/*/*/*/*/*/*/.c)
# S'il vous plait, ne faites pas ca 
```

## $ make conclusion

En conclusion, l'utilisation judicieuse des Makefiles est cruciale pour la gestion efficace de projets. Il permet d'économiser énormément de temps lors du processus de développement. Ce n'est pas pour rien que la quasi-intégralité de l'industrie utilise cet outil. 

Cette article n'étant bien entendu, pas exhaustif, je vous invite à vous renseigner par vous meme afin de découvrir les autres fonctionnalités de cet outil.
