/*
 * Použití binárních vyhledávacích stromů.
 *
 * S využitím Vámi implementovaného binárního vyhledávacího stromu (soubory ../iter/btree.c a ../rec/btree.c)
 * implementujte triviální funkci letter_count. Všimněte si, že výstupní strom může být značně degradovaný
 * (až na úroveň lineárního seznamu). Jako typ hodnoty v uzlu stromu využijte 'INTEGER'.
 *
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Vypočítání frekvence výskytů znaků ve vstupním řetězci.
 *
 * Funkce inicilializuje strom a následně zjistí počet výskytů znaků a-z (case insensitive), znaku
 * mezery ' ', a ostatních znaků (ve stromu reprezentováno znakem podtržítka '_'). Výstup je
 * uložen ve stromu (klíč vždy lowercase).
 *
 * Například pro vstupní řetězec: "abBccc_ 123 *" bude strom po běhu funkce obsahovat:
 *
 * key | value
 * 'a'     1
 * 'b'     2
 * 'c'     3
 * ' '     2
 * '_'     5
 *
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce.
 */
char getNormalised(char c);

char getNormalised(char c){
    if( c >= 'A' && c <= 'Z')
        return c-('A'-'a');
    else if (c >= 'a' && c <= 'z')
        return c;
    else if(c == ' ')
        return ' ';
    return '_';
}

void letter_count(bst_node_t **tree, char *input)
{
    bst_init(tree);
    int pole[256] = {
        0,
    };

    for (int i = 0; '\0' != input[i]; i++)
    {
        // printf("%c - ", input[i]);
        // printf("%c\n", getNormalised(input[i]));
        pole[(int)getNormalised(input[i])]++;
    }

    for (int i = 0; i < 256; i++)
    {
        if (pole[i] != 0)
        {
            bst_node_content_t item = {
                .type = INTEGER,
                .value = malloc(sizeof(int))};
            *((int *)(item.value)) = pole[i];
            bst_insert(tree, i, item);
        }
    }
}