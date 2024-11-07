# Disque dur : SAS, SATA, SCSI ou IDE

Notre but dans cet article est de mieux comprendre le concept des interfaces de programmation, des couches physiques et des jeux de commandes, et plus simplement des systèmes de stockage qui nous entourent.

Mais d'abord, un "rapide" récapitulatif de l'état actuel de nos périphériques de stockage :

| Norme de stockage  | Interface matérielle        | Couche physique                          | Jeux de commandes                  |
|--------------------|-----------------------------|------------------------------------------|------------------------------------|
| PATA               | IDE (Integrated Drive Electronics) | Connecteur 40/44 broches, câble parallèle | ACS (ATA Command Set)              |
| SATA               | SATA (Serial ATA)           | Connecteur SATA, câble série             | ACS (ATA Command Set)              |
| SAS (Serial Attached SCSI) | SCSI (Small Computer System Interface) | Connecteur SAS, câble série              | SCSI (Small Computer System Interface) |
| NVMe (Non-Volatile Memory Express) | PCIe (Peripheral Component Interconnect Express) | Bus PCIe | AHCI (Advanced Host Controller Interface) |

Si vous vous demandez, oui, les constructeurs ont pris un malin plaisir à utiliser le même nom entre l'interface et la norme.

## Norme de stockage

### ATA 
> Date : 1986  
> Qui : Western Digital  

ATA signifie **Advanced Technology Attachment**.

C'est le véritable concurrent du protocole SCSI. C'est aussi un ensemble de normes avec l'interface matérielle (IDE), la couche physique (PATA ou bien SATA) et les jeux de commandes (ACS).

C'est une version plus low-cost de SCSI, qui avait pour but de réutiliser des éléments de ce dernier mais en simplifiant le connecteur (la couche physique) et le jeu d'instruction.

Contrairement au SAS, cette norme est en semi-duplex, ce qui signifie qu'elle ne peut exécuter qu'une seule action simultanément : soit lire, soit écrire.

### SAS 

> Date : 1980  
> Qui : Shugart / NCR Corporation  

C'est un ensemble de normes qui s'applique sur les couches physiques et les jeux de commandes. 

Nous devons revenir dans les années 80, à cette époque, il n'y a pas des milliers de façons de communiquer avec un périphérique de stockage. L'un d'entre eux est le protocole **SCSI** (Small Computer System Interface).

C'est un protocole propriétaire mais très performant pour son temps. Son grand avantage est le fait de déporter la logique dans le périphérique, contrairement à ses concurrents qui utilisent allègrement la puissance de l'ordinateur. Et dans les années 80, on n'avait pas encore la puissance de calcul, ce protocole a donc été particulièrement apprécié par l'industrie.

De plus, la norme a l'avantage de pouvoir écrire en duplex, c'est-à-dire lire et écrire des données simultanément.

![PATA Pin out](./PATA_Pinout.png)

## Interface matérielle

### IDE

IDE est une norme qui intègre le contrôleur de disque directement sur le disque dur lui-même. Avant IDE, les contrôleurs de disque étaient séparés, ce qui compliquait la configuration et limitait la compatibilité.  
Cela signifie qu'un disque dur est dit compatible IDE s'il possède une puce IDE.

### SATA

C'est une ré-implémentation de l'interface matérielle parallèle en série (Serial ATA).

> Qu'est-ce qu'un port parallèle ?  
> Contrairement au port série, un port parallèle peut transférer un ensemble de 8 bits à la fois sur huit fils différents.

### PCIe

L'interface matérielle PCIe (Peripheral Component Interconnect Express) est une interface de communication utilisée principalement pour connecter des périphériques internes tels que des cartes graphiques, des cartes réseau et d'autres cartes d'extension.

Elle utilise un bus série, ce qui signifie que les données sont transmises bit par bit sur une seule ligne, contrairement aux anciennes interfaces parallèles.

PCIe fonctionne avec des connecteurs spécifiques et des voies de transmission (lanes) qui permettent des transferts de données très rapides. 

Les cartes PCIe sont insérées dans des slots PCIe sur la carte mère, et l'interface est disponible en différentes versions (x1, x4, x8, x16) en fonction du nombre de voies utilisées, offrant ainsi une bande passante ajustée aux besoins des périphériques.

PCIe est largement utilisé dans les systèmes modernes en raison de sa vitesse de transmission élevée, de sa flexibilité et de sa capacité à évoluer pour prendre en charge des dispositifs nécessitant des débits importants.

## Couche physique

Globalement, c'est la partie la plus simple de l'article.  
(Enfin, en électronique, ce n'est jamais simple).

Une couche physique, cela signifie que c'est la norme qui régit la façon de réaliser le câble et le connecteur.

![Couche Physique](https://upload.wikimedia.org/wikipedia/commons/5/52/ASUS_ATA_cable_80wire_detail_20041201.jpg)

Dans notre cas, c'est un câble dit "IDE".  
Ce genre de câble est la version parallèle, il contient 80 fils, ce qui permet de connecter deux appareils à la carte mère. Le premier sera appelé "master" et le deuxième "slave".

Je ne vais pas détailler les différents câbles qui existent.

## Jeux de commandes

### ACS (Commandes)
> Date : années 80  
> Qui : Shugart / NCR Corporation

Le **ATA Command Set** (ACS) est la liste des commandes que le système peut envoyer au périphérique.  
On pourrait citer comme exemples :  
- WRITE_SECTOR  
- SLEEP  
- PACKET  
- IDENTIFY PACKET DEVICE

### SCSI

ACS étant un détournement du jeu de SCSI, on retrouve de grandes similitudes entre les deux jeux. Cependant, on peut les différencier en creusant un peu, notamment en ce qui concerne les rapports d'erreurs.

### AHCI

**AHCI** (Advanced Host Controller Interface) permet une communication optimisée entre le système d'exploitation et les périphériques de stockage, en offrant des fonctionnalités avancées comme le contrôle de file d'attente (pour améliorer les lectures/écritures simultanées) et le mode **NCQ** (Native Command Queuing), qui permet au disque de réorganiser les commandes d'entrée/sortie pour une performance maximale.

Ce jeu de commandes facilite également des fonctionnalités telles que la gestion de l'alimentation et le **hot-plug** (permettre l'ajout et le retrait de périphériques sans éteindre le système).

AHCI a été conçu pour améliorer les performances et la gestion des périphériques de stockage SATA en optimisant l'utilisation des ressources et la vitesse des transferts.

## Conclusion / Performance

Nous n'avons toujours pas parlé de la partie performance de ces différences, donc voici le mot de la fin.

| Interface    | Vitesse max (Gb/s)       | Type de connexion | Utilisation principale                    |
|--------------|--------------------------|-------------------|-------------------------------------------|
| **SATA III** | 6 Gb/s                   | Série            | SSD et HDD domestiques                   |
| **SAS**      | 22,5 Gb/s                | Série            | Disques professionnels et serveurs       |
| **PCIe 5.0** | 32 Gb/s par ligne        | Série            | SSD NVMe internes                        |
| **NVMe**     | Dépend de PCIe           | PCIe             | SSD haute performance                    |
| **U.2**      | Selon PCIe (16-32 Gb/s)  | PCIe et NVMe     | Serveurs et stations de travail          |
| **M.2**      | Selon PCIe (16-32 Gb/s)  | PCIe ou SATA     | Ordinateurs portables et de bureau       |
| **Thunderbolt 3/4** | 40 Gb/s           | Série (USB-C)    | Stockage externe rapide                  |
| **USB 4**    | 40 Gb/s                  | Série (USB-C)    | Stockage externe polyvalent              |
