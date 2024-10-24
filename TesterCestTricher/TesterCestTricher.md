# Tester c'est tricher, compiler c'est douter
Dans cet article, nous allons voir le concept de Continuous Integration (CI), son interet et ses inconevnients
## Histoire
Mais d'abord, comme a mon habitude, un petit point histoire.
>  In 1999, Beck elaborated more in his first full book on Extreme Programming. CruiseControl, one of the first open-source CI tools, self-published source was released in 2001.
## Pourquoi
Le but d'une CI est de pouvoir faire des tests automatises et cela a chaque commit. On appelle cela, une Continuous Integration car cela revient a verifier qu'a chaque commit qui est fait, le code est toujours fonctionel. 
Elle est surtout utile lors des merges requests, elle permet de verifier qu'il n'y a pas de breaking change.
### Fonctionnement 
Je dois d'abord, vous expliquez quelque mots de vocabulaire avant de pouvoir expliquer le fonctionnement.
- Jobs : C'est une instance d'un docker qui execute un script. Cela peut etre du code, des tests ou bien juste un `echo`. 
// TODO : photo
- Pipeline : C'est un ensemble de jobs qui sont executés par Gitlab a chaque commit.  
// TODO : photo

Je suppose que vous commencez a comprendre le fonctionnement d'une CI, le but est de lancer une instance pour chaque jobs et de verifier son return code. Si tout est bon, alors on lance le jobs suivant pour finir la pipeline. 
## Démonstration
Nous allons utiliser le format Gitlab, c'est un fichier `.yaml` que l'on nomme `.gitlab-ci.yml`. 
### Simple Compile
```yaml
image: alpine:latest

myjobname: 
  - script: 
    - make
```

### Compile with flags
Pour compiler avec des flags spécifiques, soit vous avez un rule dans votre makefile qui le fait, soit vous pouvez le faire à la main.
```yaml
myjobname_hard: 
  - script:
    - CFLAGS="-Wall -Werror" make
    # or 
    - make compile_flags
```
## Criterion + flags
### Before Scripts
### Multi stages
## Formatteur
## Cache
Dans certains cas, cela peut etre utile de mettre un cache dans notre CI. 
Le cas typique est celui du JS. C'est inutile de re-telecharger le dossier `node-modules/` á chaque pipeline. 
Ce dossier ou bien fichier sera donc sauvegarder entre chaque pipeline / jobs.

Vous pouvez neamoins le clean quand vous le souhaitez.
## Artefact ?
benchmarking in a ci
## Coverage ?

## Custom base
Vous avez pu voir que depuis le debut de cette article, nous ne precisons pas d'environment de CI. J'avais dit qu'une CI utilisait un docker mais lequel ? 

Il suffit de preciser cela, au debut de notre fichier de configuration. Vous pouvez utiliser n'importe quelle image docker, quelle soit public ou privée. 
```yaml
image: alpine:latest
```

