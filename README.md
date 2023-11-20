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

**MemeOS** est un système d'exploitation 32 bits multitâches ayant 2 tâches principales :
- Écrire dans une variable (**TODO**)
- Lire la variable et l'afficher à l'écran (**TODO**)

D'autres fonctionnalités sont développées comme :
- Création d'un gestionnaire de tâches (**TODO**)
- Création d'un shell avec quelques commandes disponibles (**TODO**)
- Création de nouvelles tâches qui tournent en parallèle (**TODO**)

Beaucoup de travail a été réalisé pour fournir un OS de cette qualité. Nous espérons qu'il vous plaira.

## Spécifications

Les spécifications ne sont pas nombreuses mais il est quand même intéressant de les noter

### L'utilisateur

L'**utilisateur** a des droits **restreints** (normal) et est forcément lancé par le pépin. Il a de base **une page mémoire** allouée mais il peut en demander une **deuxième** s'il s'étend trop (en cas de première page fault). Bien sûr, il ne peut pas accéder aux **données du pépin** ! Chaque tâche est **isolée** des autres, faisant qu'il est impossible qu'une tâche puisse lire dans celle d'un autre.

Pour demander la **création d'une tâche**, un appel système est lancé. Voici la liste des différents **appels système** implémentés : 
|  Id   |          Description          |
| :---: | :---------------------------: |
|   0   |   Créer une nouvelle tâche    |
|   1   |       Arrêter une tâche       |
|   2   |       Lancer le compte        |
|   3   |       Arrêter le compte       |
|   4   |        Lire le compte         |
|   5   | Ecrire sur la sortie standard |
|   6   | Ecrire sur l'entrée standard  |
|   7   |  Demander une page partagée   |
|   8   |       Exécuter du code        |
|   9   |   Lire la mémoire partagée    |

**TODO Ajouter les syscall au fur et à mesure**

### Le kernel

Le **kernel** (aussi appelé **pépin** de MemeOS) est composé d'un **gestionnaire de tâches** qui permet à un utilisateur de créer de nouvelles tâches et d'un **ordonnanceur**. La **politique de pagination** du pépin est une politique que nous avons décidé d'appeler **"la politique de Sauron"** : le kernel voit tout ce qu'il se passe du côté **utilisateur** et peut intervenir sur n'importe quelle **tâche utilisateur**. Néanmoins, quand une tâche fait un **appel système**, il ne peut accéder qu'à ses pages à lui et à une partie de la mémoire pépin (appelée **mémoire partagée**). 

Le **gestionnaire de tâches** contient un tableau de tâches qui s'exécutent **en même temps**. Il est possible d'exécuter jusqu'à **10 tâches** de manière simultanée (9 si on enlève le shell). Si une 11e tâche est demandée, alors elle sera **refusée**. Si une tâche finit, alors elle **libère sa place** dans le tableau de tâches et une autre peut la **remplacer**. 

L'**ordonnanceur** est un simple mécanisme de **round-robin** sur les tâches qui sont dans le tableau de tâches du **gestionnaire**. A noter que si la tâche a fait un **appel système** et que son nombre de quanta passe à 0, alors l'ordonnanceur **réinitialisera** son nombre de quantum et continuera à compter jusqu'à ce que le pépin rende la main à l'utilisateur. S'il passe une nouvelle fois à 0, alors le nombre de quantum ne sera pas **réinitialisé et restera à 0** (il saute alors son prochain tour). 

## Organisation de la mémoire

| Adresse de début | Adresse de fin | Utilité             |    Accès kernel    | Accès utilisateur  |
| :--------------: | :------------: | :------------------ | :----------------: | :----------------: |
|     0x300000     |    0x33FFFF    | Mémoire kernel      | :heavy_check_mark: |        :x:         |
|     0x340000     |    0x34FFFF    | Pile kernel         | :heavy_check_mark: |        :x:         |
|     0x350000     |    0x366FFF    | PGD et PTB          | :heavy_check_mark: |        :x:         |
|     0x367000     |    0x36FFFF    | Non utilisé         |        :x:         |        :x:         |
|     0x370000     |    0x37013f    | GDT                 | :heavy_check_mark: |        :x:         |
|     0x370140     |    0x3FFFFF    | Non utilisé         |        :x:         |        :x:         |
|     0x400000     |    0x413FFF    | Mémoire utilisateur | :heavy_check_mark: | :heavy_check_mark: |
|     0x414000     |    0x4FFFFF    | Mémoire partagée    | :heavy_check_mark: | :heavy_check_mark: |

- La **mémoire kernel** est la mémoire utilisée par le kernel pour stocker ses fonctions et autres variables fixes
- La **pile kernel** est l'espace utilisé pour la pile du pépin, rien de plus...
- Les **PGD et PTB** sont stockées après la pile. Il y a au maximum 11 PGD, donc les PGD s'arrêtent à `0x35AFFF`. L'espace kernel à paginer (de `0x300000` à `0x36FFFF` et de `0x400000` à `0x4FFFFF`) demande 2 tables de pages. Chaque tâche utilisateur demande une table de pages (2 pages de `0x400000` à `0x413FFF` et tout de `0x414000` à `0x4FFFFF`). On a donc au maximum 12 tables de page. Une PGD a une taille de **0x1000** et une PTB aussi, donc on a besoin de 23*0x1000 = **0x17000** d'espace pour PGD et PTB.
- L'**espace non utilisé** est là pour avoir une cohérence entre l'espace utilisateur et l'espace pépin. Il peut servir à ajouter des tâches simultanées.
- La **mémoire utilisateur** est l'endroit où les tâches pourront vivre et demander des pages supplémentaires, le tout dans une harmonie tout relative.
- La **mémoire partagée** est un mécanisme de partage de données entre tous les utilisateurs. Avec un appel système, une tâche peut demander une page dans cet espace et pouvoir écrire dedans. Pour avoir plus de détails il faut regarder la section suivante !

### Mémoire partagée

La **mémoire partagée** est un espace mémoire commune à tous les utilisateurs. Tout le monde a au minimum le droit de la lire au moyen d'un appel système de lecture. Elle n'est donc pas appropriée au stockage des mots de passe (sauf si on veut vivre dangereusement !). Les utilisateurs et le kernel peuvent demander jusqu'à **21 pages** chacun, soit `0x15000` d'espace maximum par tâche (+ pépin).

Lors du demande d'une page partagée, on peut la rendre accessible à **l'écriture** des autres utilisateurs et même à **l'exécution** ! Vos plus belles ROP-chains commencent ici !

## Remerciements

Merci à Anaïs Gantet pour le cours, les TPs et l'aide apportée durant les TPs. 
