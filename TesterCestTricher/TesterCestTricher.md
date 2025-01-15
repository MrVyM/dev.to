#Tester c'est tricher, compiler c'est douter 

Dans cet article, nous allons voir le concept de Continuous Integration (CI), son intérêt et ses inconvénients et une démonstration.

## Histoire

Mais d'abord, comme à mon habitude, un petit point histoire.

> En 1999, Kent Beck a approfondi le sujet dans son premier livre sur l'Extreme Programming. En 2001, CruiseControl, l'un des premiers outils open-source de CI, a vu le jour.

## Mais why ?  
Le but d'une CI est de faire des tests automatisés à chaque commit. Cela garantit que le code reste fonctionnel en permanence. On appelle cela une Continuous Integration car, à chaque modification, le code est vérifié pour s'assurer qu'il n'y a pas de régressions.

### Avantages

- **Détection précoce des erreurs** : Les problèmes sont identifiés rapidement, ce qui permet de réagir sans attendre.  
- **Amélioration de la qualité** : En testant systématiquement, on garantit un code plus robuste.  
- **Gain de temps** : Les pipelines automatisés réduisent la nécessité de tests manuels répétitifs.

### Inconvénients

- **Coût initial** : Mettre en place une CI peut demander des efforts et des compétences initiales importantes.  
- **Temps d’exécution** : Les pipelines complexes peuvent rallonger le temps avant qu'un développeur puisse valider son code.

## Fonctionnement

Avant de voir le fonctionnement, un petit lexique :  

- **Jobs** : Une instance d'un conteneur (souvent Docker) qui exécute un script. Cela peut inclure des commandes, des tests ou des actions simples comme un `echo`.  
- **Pipeline** : Une série de jobs organisés de manière séquentielle ou parallèle. Chaque commit déclenche cette série pour valider les changements.  

Le principe est simple : chaque job retourne un code de statut (succès ou échec). Si un job échoue, la pipeline s’arrête ou ignore les étapes suivantes selon la configuration.

## Trêve de blabla

Nous allons utiliser un exemple basé sur GitLab CI. On la configure via un fichier `.gitlab-ci.yml`.

### Basique, simple, simple, basique

```yaml
image: bookworm-slim:latest

myjobname:
  script:
    - make
```

### Les flags

Pour ajouter des flags de compilation, deux approches sont possibles :  

1. Via une règle dans le Makefile.  
2. En passant les flags directement dans la commande CI.  

```yaml
myjobname_hard:
  script:
    - CFLAGS="-Wall -Werror" make
    # ou
    - make compile_flags
```

### Tests avec Criterion et flags

Criterion est une bibliothèque de tests unitaires en C.

#### Où est passé Criterion ?  

Avant d’exécuter des tests avec Criterion, il est souvent nécessaire d'installer Criterion, eh oui !

```yaml
before_script:
  - apt-get update && apt-get install -y libcriterion-dev
script:
  - ./configure
  - make test
```

#### Multistaging
Diviser les tests unitaires et fonctionnels en plusieurs stages garantit :  
- une bonne organisation  
- une meilleure visibilité des résultats  

```yaml
stages:
  - build
  - test

build:
  stage: build
  script:
    - make all

test-unit:
  stage: test
  script:
    - make unit-test

test-functional:
  stage: test
  script:
    - make functional-test
```

### Tu t'es fait clang ?  

Le formatage du code est super important pour maintenir une base de code propre.  

```yaml
clang_format:
  stage: format
  before_script:
    - apt-get -qq update && apt-get -qq install -y clang-format autotools-dev autoconf-archive gcovr libcriterion-dev
  script:
    - clang-format -i $(find src/ -type f -name "*.c") --dry-run --Werror
```

### Cache

Dans certains cas, c'est utile de mettre en cache des fichiers ou dossiers pour éviter de les recharger à chaque pipeline.  
Un exemple courant est le dossier `node_modules/` en JavaScript.

```yaml
cache:
  paths:
    - node_modules/

install:
  script:
    - npm install
```

Bien entendu, vous pouvez nettoyer le cache au besoin avec une option supplémentaire dans la configuration de pipeline.

### Artefacts

Les artefacts sont les fichiers générés par la CI qui peuvent être partagés entre jobs ou téléchargés.  
Par exemple, les rapports de tests ou de couverture.

```yaml
artifacts:
  paths:
    - build/
    - reports/
```

### Coverage de tests

On peut mesurer la couverture de tests en intégrant des outils comme gcovr ou Cobertura dans votre pipeline CI.

```yaml
test-coverage:
  stage: test
  script:
    - gcovr --html --html-details -o coverage.html
  artifacts:
    paths:
      - coverage.html
```

#### Rapporteur

Ce bloc vous permet d'intégrer le coverage report dans votre Merge Request, vous pourrez ainsi voir le code qui n'est pas couvert mais aussi votre pourcentage de coverage. 

```yaml
coverage-report:
  script:
    # do coverage
  coverage: /^TOTAL.*\s+(\d+\%)$/
  artifacts:
    name: coverage.xml
    expire_in: 2 days
    reports:
      coverage_report:
        coverage_format: cobertura
        path: coverage.xml

```
### Environnement custom  

Vous pouvez préciser l’environnement de base pour votre CI en sélectionnant une image Docker spécifique.

```yaml
image: gcc:latest
```

En utilisant un peu ce que l'on vient de voir, ça donnerait quelque chose comme ça :  

```yaml
image: gcc

stages:
  - format
  - build
  - test
  - clean

clang-format:
  stage: format
  script:
    - clang-format -i $(find src/ -type f -name "*.c") --dry-run --Werror

build:
  stage: build
  script:
    - autoreconf --install
    - ./configure
    - make all

test-unit:
  stage: test
  script:
    - autoreconf --install
    - ./configure
    - make unit-test
```

> Attention aux `.h` et il manque des `before_script`.

### Petit Bonus  

On peut aussi vérifier les trash-files pour s’assurer que le `make clean` fait bien son travail.

```yaml
trash-file:
  stage: clean
  needs: []
  before_script:
    - apt-get -qq update && apt-get -qq install -y tree
  script:
    - tree > /tmp/REF
    - make && make clean
    - tree > /tmp/TEST
    - diff /tmp/REF /tmp/TEST
```

## Conclusion

La Continuous Integration est un outil extrêmement puissant. Il peut parfois être difficile à mettre en place, mais les gains sont immenses.
