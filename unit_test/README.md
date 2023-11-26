# Tests unitaires

Afin de pouvoir vérifier son travail et pour s'assurer que tout fonctionne parfaitement en limitant les dégâts, nous avons implémenté une unité de tests unitaires.

Attention, ces tests unitaires ne sont pas compilés comme le pépin. La librarie standard de C est présente ainsi que toutes les super fonctionnalités intéressantes qu'il est possible d'inclure. Si on veut utiliser les fonctions du kernel de MemeOs, ben on peut pas donc on fait pas ! Si jamais la fonctionnalité repose sur une fonctionnalité pépin, alors on ne peut pas la tester... Néanmoins, le chemin `kernel/include` est utilisé comme répertoire d'inclusion par gcc comme pour les TP (pour les headers contenant seulement des constantes).

## Créer un test

Pour créer un test, il faut créer un fichier C avec les fonctions des tests unitaires que l'on veut tester. On ajoute aussi une fonction `main` lançant les fonctions de test voulues. Il y a un super utilitaire de test qui s'appelle `test_utils`, faut pas hésiter à l'utiliser !

Si une fonction de test contient des assertions fausses, on affiche le message qui est dans la fonction d'assertion et on passe au fichier de test suivant. On ne teste pas les fonctions d'après ! Il est possible d'afficher des messages intermédiaires avec `printf` comme d'habitude.

Un petit exemple (test.c):
```c
#include <test_utils.h>

void test_addition() {
    assertEquals(2, 1+1, "Addition problem...");
    assertEquals(3, 1+1, "Get good loser!");
}

void test_tableau() {
    int tab1[] = {1, 2, 3, 4};
    int tab2[] = {1, 2, 3, 4};
    int tab3[] = {1, 3, 2, 4};
    assertEquals(tab1, tab2, "Should be equal tabs!");
    assertNotEquals(tab1, tab3, "Should be not equals since 2 and 3 are inverted...");
}

int main() {
    test_tableau();
    test_addition();

    return 0;
}
```

Ce fichier de test va être considéré faux car le deuxième `assertEquals` de `test_addition` est pas bon (ça aurait dû être un `assertNotEquals`). Le test va donc retourner :
```
[Error] Values not equal... Expected <3> but got <2> : Get good loser!
```

**Attention** dans le cas des tests avec les **tableaux**, si le `got` est plus grand que l'`expected`, seules les données qui vont jusqu'à l'expected seront testées. Ainsi les tableaux `expected={1, 2, 3}` et `got={1, 2, 3, 4}` seront considérés comme égaux.

## Lancer un test

Pour lancer un test il suffit tout simplement de taper `make` et tout s'exécutera tout seul. Comment ça marche ? C'est tout simple ! Le Makefile va en réalité générer un script shell qui va lancer les exécutables uns à uns et tester leur code de retour. Si le code de retour est 0 alors tout s'est déroulé sans accroc ! Sinon aïe aïe aïe c'est qu'il y a une erreur et le message du test correspondant est affiché.
