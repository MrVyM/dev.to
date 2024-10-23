# Tester c'est tricher, compiler c'est douter
Dans cet article, nous allons voir le concept de Continuous Integration (CI), son interet et ses inconevnients
## Histoire
Mais d'abord, comme a mon habitude, un petit point histoire.
>  In 1999, Beck elaborated more in his first full book on Extreme Programming. CruiseControl, one of the first open-source CI tools, self-published source was released in 2001.
## Pourquoi
### Fonctionnement 
Pipeline :
Jobs :
## Démonstration
Nous allons utiliser le format Gitlab, c'est un fichier `.yaml` que l'on nomme `.gitlab-ci.yml`. 
### Simple Compile
```yaml
base: image 

build: 
  - script: 
    - make
```

### Compile with flags
## Criterion + flags
#### Before Scripts
#### Multi stages
### Formatteur
## Cache
Dans certains cas, cela peut etre utile de mettre un cache dans notre CI. 
Le cas typique est celui du JS. C'est inutile de re-telecharger le dossier `node-modules/` á chaque pipeline. 
Ce dossier ou bien fichier sera donc sauvegarder entre chaque pipeline / jobs.

Vous pouvez neamoins le clean quand vous le souhaitez.
## Artefact ?
benchmarking in a ci
## Coverage ?
## Custom base

