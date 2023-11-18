# MemeOS

## Introduction

meme-os est une version de [SECOS](https://github.com/sduverger/secos) développée par Adrien Jakubiak et Nicolas Rigal et qui a pour but le développement d'un OS dans le cadre du cours d'OS.

## Pré-requis

Un ordinateur, un compilateur C, qemu... Je pense que c'est suffisant !

## Installation et dépendences

Si en tant que correcteur ce n'est pas déjà fait, voici comment il est possible d'installer tout ce qu'il y a à installer en deux commandes :

```bash
bash$ sudo apt-get install qemu-system-x86 qemu-kvm gcc-multilib make git
bash$ git clone https://github.com/Zefinder/meme_os.git
```

## Implémentation

MemeOS est un système d'exploitation 32 bits multitâches ayant 2 tâches principales :
- Écrire dans une variable 
- Lire la variable et l'afficher à l'écran

D'autres fonctionnalités sont développées comme :
- Création d'un gestionnaire de tâches
- Création d'un shell avec quelques commandes disponibles (**TODO**)
- Création de nouvelles tâches qui tournent en parallèle (**TODO**)

Beaucoup de travail a été réalisé pour fournir un OS de cette qualité. Nous espérons qu'il vous plaira.

## Spécifications

Les spécifications ne sont pas nombreuses mais il est quand même intéressant de les noter

### L'utilisateur

L'utilisateur a des droits restreints (normal) et est forcément lancé par le pépin. Il a de base une page mémoire allouée mais il peut en demander une deuxième s'il s'étend trop (page fault). Bien sûr, il ne peut pas accéder aux données du pépin ! Chaque tâche est isolée des autres, faisant qu'il est impossible qu'une tâche puisse lire dans celle d'un autre.

Pour demander la création d'une tâche, un appel système est lancé. Voici la liste des différents appels système implémentés : 
|  Id   |          Description          |
| :---: | :---------------------------: |
|   0   |   Créer une nouvelle tâche    |
|   1   |       Arrêter une tâche       |
|   2   |       Lancer le compte        |
|   3   |       Arrêter le compte       |
|   4   |        Lire le compte         |
|   5   | Ecrire sur la sortie standard |
|   6   | Ecrire sur l'entrée standard  |

**TODO Ajouter les syscall au fur et à mesure**

### Le kernel

Le kernel (aussi appelé pépin de MemeOS) est composé d'un gestionnaire de tâches qui permet à un utilisateur de créer de nouvelles tâches et d'un ordonnanceur. La politique de pagination du pépin est une politique que nous avons décidé d'appeler "la politique de Sauron" : le kernel voit tout ce qu'il se passe du côté utilisateur et peut intervenir sur n'importe quelle tâche utilisateur. Néanmoins, quand une tâche fait un appel système, il ne peut accéder qu'à ses pages à lui et à une partie de la mémoire pépin (appelée mémoire partagée). 

Le gestionnaire de tâches contient un tableau de tâches qui s'exécutent en même temps. Il est possible d'exécuter jusqu'à 10 tâches de manière simultanée (9 si on enlève le shell). Si une 11e tâche est demandée, alors elle sera refusée. Si une tâche finit, alors elle libère sa place dans le tableau de tâches et une autre peut la remplacer. 

L'ordonnanceur est un simple mécanisme de round-robin sur les tâches qui sont dans le tableau de tâches du gestionnaire. A noter que si la tâche a fait un appel système et que son nombre de quanta passe à 0, alors l'ordonnanceur réinitialisera son nombre de quantum et continuera à compter jusqu'à ce que le pépin rende la main à l'utilisateur. S'il passe une nouvelle fois à 0, alors le nombre de quantum ne sera pas réinitialisé et restera à 0 (il saute alors son prochain tour). 

## Organisation de la mémoire

| Adresse de début | Adresse de fin | Utilité             | Accès kernel | Accès utilisateur |
| :--------------: | :------------: | :------------------ | :----------: | :---------------: |
|     0x300000     |    0x33FFFF    | Mémoire kernel      |     [x]      |        [ ]        |
|     0x340000     |    0x34FFFF    | Pile kernel         |     [x]      |        [ ]        |
|     0x350000     |    0x366FFF    | PGD et PTB          |     [x]      |        [ ]        |
|     0x367000     |    0x3FFFFF    | Non utilisé         |     [ ]      |        [ ]        |
|     0x400000     |    0x413FFF    | Mémoire utilisateur |     [x]      |        [x]        |
|     0x414000     |    0x4FFFFF    | Mémoire partagée    |     [x]      |        [x]        |

- La mémoire kernel est la mémoire utilisée par le kernel pour stocker ses fonctions et autres variables fixes
- La pile kernel est l'espace utilisé pour la pile du pépin, rien de plus...
- Les PGD et PTB sont stockées après la pile. Il y a au maximum 11 PGD, donc les PGD s'arrêtent à 0x35AFFF. L'espace kernel à paginer (de 0x300000 à 0x36FFFF et de 0x400000 à 0x4FFFFF) demande 2 tables de pages. Chaque tâche utilisateur demande une table de pages (2 pages de 0x400000 à 0x413FFF et tout de 0x414000 à 0x4FFFFF). On a donc au maximum 12 tables de page. Une PGD a une taille de 0x1000 et une PTB aussi, donc on a besoin de 23*0x1000 = 0x17000 d'espace pour PGD et PTB.
- L'espace non utilisé est là pour avoir une cohérence entre l'espace utilisateur et l'espace pépin. Il peut servir à ajouter des tâches simultanées.
- La mémoire utilisateur est l'endroit où les tâches pourront vivre et demander des pages supplémentaires, le tout dans une harmonie tout relative.
- La mémoire partagée est un mécanisme de partage de données entre tous les utilisateurs. Avec un appel système, une tâche peut demander une page dans cet espace et pouvoir écrire dedans. Pour avoir plus de détails il faut regarder la section suivante !

### Mémoire partagée



## Remerciements

Merci à Anaïs Gantet pour le cours, les TPs et l'aide apportée durant les TPs. 

**TODO A changer (retirer ?)**
## Arborescence

Le code de MemeOS est organisé de la façon suivante :

```bash
$ cd secos-ng ; ls
kernel  README  tp0  tp1  tp2  tp3  tp4  tp5  tp_exam  utils
```

`kernel` contient l'implémentation de quelques composants de base tels que : 

* `printf` / `debug` pour pouvoir afficher des messages sur le port série
* `memcpy`, `memset`, ...
* L'accès aux structures multiboot de Grub
* Les structures matérielles du mode protégé pour x86 : GDT/IDT/LDT,
  descripteurs, sélecteurs, tables de pages, PTE/PDE, TSS, registres de
  contrôle (CR), registres de flags, etc.

Note : `kernel/include` contient notamment des définitions de structures
utilisant des bitfields permettant d'accéder facilement aux champs des
structures matérielles sans avoir à faire de masques ou décallages, utiles
dès le `tp0`.

 Le noyau est simpliste. Il démarre en mode protégé en 32 bits grâce à Grub. A
 ce titre, il accède à la mémoire physique directement sans pagination. Le
 mode protégé a été partiellement initialisé par Grub, mais il faudra, au fil
 des TPs, le configurer petit à petit. Chaque répertoire `tpX` possède le
 squelette de code et l'environnement nécessaire pour créer un noyau incluant
 de plus en plus de fonctionnalités bas niveau :

* [`tp0`](./tp0/README.md) : Démarrage
* [`tp1`](./tp1/README.md) : Segmentation
* [`tp2`](./tp2/README.md) : Interruptions et exceptions
* [`tp3`](./tp3/README.md) : Niveau de privilèges
* [`tp4`](./tp4/README.md) : Pagination
* [`tp5`](./tp5/README.md) : Appels systèmes
* [`tp_exam`](./tp_exam/README.md) : OS complet comportant 2 applications

Dans chacun des répertoires, le fichier `README.md` contient l'énoncé, et le
fichier `tp.c` est celui dans lequel les développements sont attendus. Note :
Il n'est pas nécessaire de modifier l'environnement de compilation ni les
autres fichiers du noyau.

**Note : si une question est marquée d'une `*`, c'est que le code rédigé pour
  y répondre peut à nouveau servir dans la suite des TP, voire pour le TP
  exam.**

## Démarrage d'un TP

Pour démarrer le noyau d'un TP en particulier, il suffit de recompiler le
noyau (`make`), puis de le lancer avec `make qemu`. Par exemple pour le `tp0` :

```bash
$ cd tp0
$ make clean all
    AS    ../kernel/core/entry.s
    CC    ../kernel/core/start.c
    CC    ../kernel/core/print.c
    CC    ../kernel/core/uart.c
    CC    ../kernel/core/pic.c
    CC    tp.c
    LD    kernel.elf

$ make qemu
hit <ctl+a> <h> <enter>, for Qemu help
vvfat . chs 1024,16,63

secos-a241db6-59e4545 (c) Airbus
kernel mem [0x302010 - 0x303820]
```

QEMU va alors démarrer une VM équipée d'un processeur x86 32 bits et de 128MB
de RAM. Au lancement, le terminal courant va être utilisé par le mode monitor
de QEMU et va afficher des messages de logs. Pour quitter (et stopper la VM),
utiliser la séquence **\<ctrl-a\> \<x\>**.

Note : La VM démarre sur une disquette "grub.floppy" qui contient le
bootloader grub. Celui-ci est configuré pour charger le fichier `kernel.elf`
depuis le disque dur "HDD0". Le fichier `kernel.elf` est notre noyau d'OS, il
se trouve dans le répertoire de TP courant.

## Utilisation de l'environnement QEMU

Le monitor de QEMU est utile car permet d'inspecter et debugger la machine
virtuelle.

Pour accéder au "monitor" de QEMU, utiliser la séquence **\<ctrl-a\>**
puis **\<c\>** puis **\<enter\>**. Cela va afficher :

```bash
QEMU 2.xx.yy monitor - type 'help' for more information
(qemu)
```

Par défaut, le port série de la VM est redirigé dans votre terminal(stdio). La
VM ne dispose pas d'affichage graphique. Pour rebasculer dans l'affichage des
logs envoyées sur le port série, tapez de nouveau la
séquence **\<ctrl-a\> \<c\> \<enter\>**. Le monitor est multiplexé avec le
port série sur stdio.

Pour quitter QEMU en mode monitor : **q**.

Note : Le lancement de SECOS sous QEMU est configurable via le fichier
[`config.mk`](./utils/config.mk) qui définit les options à passer à QEMU au
démarrage.