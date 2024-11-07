/*
 * Binární vyhledávací strom — rekurzivní varianta
 *
 * S využitím datových typů ze souboru btree.h a připravených koster funkcí
 * implementujte binární vyhledávací strom pomocí rekurze.
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializace stromu.
 *
 * Uživatel musí zajistit, že inicializace se nebude opakovaně volat nad
 * inicializovaným stromem. V opačném případě může dojít k úniku paměti (memory
 * leak). Protože neinicializovaný ukazatel má nedefinovanou hodnotu, není
 * možné toto detekovat ve funkci.
 */
void bst_init(bst_node_t **tree)
{
  (*tree) = NULL;
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * ukazatel na obsah daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 *
 * Funkci implementujte rekurzivně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, bst_node_content_t **value)
{
  if (tree == NULL)
    return false; // Nenasli sme
  else
  {
    if (tree->key == key)
    {
      if (*value == NULL)
      {
        *value = (bst_node_content_t *)malloc(sizeof(bst_node_content_t));
        if (*value == NULL)
          return false;
      }
      (*value)->type = tree->content.type;
      (*value)->value = tree->content.value;
      return true; // Nasli sme
    }
    else
    {
      if (key < tree->key)
        return bst_search(tree->left, key, value);
      else
        return bst_search(tree->right, key, value);
    }
  }
}

/*
 * Vložení uzlu do stromu.
 *
 * Pokud uzel se zadaným klíče už ve stromu existuje, nahraďte jeho hodnotu.
 * Jinak vložte nový listový uzel.
 *
 * Výsledný strom musí splňovat podmínku vyhledávacího stromu — levý podstrom
 * uzlu obsahuje jenom menší klíče, pravý větší.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, bst_node_content_t value)
{
  if ((*tree) == NULL)
  {
    (*tree) = (bst_node_t *)malloc(sizeof(bst_node_t));
    if (*tree != NULL)
    {
      (*tree)->key = key;
      (*tree)->content.type = value.type;
      (*tree)->content.value = value.value;
      (*tree)->left = NULL;
      (*tree)->right = NULL;
    }
    return;
  }
  else
  {
    if (key < (*tree)->key)
    {
      bst_insert(&(*tree)->left, key, value);
    }
    else
    {
      if (key > (*tree)->key)
        bst_insert(&(*tree)->right, key, value);
      else
      {
        (*tree)->content.type = value.type;
        (*tree)->content.value = value.value;
      }
    }
  }
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 *
 * Klíč a hodnota uzlu target budou nahrazeny klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 *
 * Tato pomocná funkce bude využitá při implementaci funkce bst_delete.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree)
{
}

/*
 * Odstranění uzlu ze stromu.
 *
 * Pokud uzel se zadaným klíčem neexistuje, funkce nic nedělá.
 * Pokud má odstraněný uzel jeden podstrom, zdědí ho rodič odstraněného uzlu.
 * Pokud má odstraněný uzel oba podstromy, je nahrazený nejpravějším uzlem
 * levého podstromu. Nejpravější uzel nemusí být listem.
 *
 * Funkce korektně uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkci implementujte rekurzivně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key)
{
  if (*tree != NULL)
  {
    // Ruseny kluc je v pravom podstrome
    if (key < (*tree)->key)
    {
      bst_node_t **auxVar = &(*tree)->left;
      bst_delete(auxVar, key);
      (*tree)->left = *auxVar;
    }
    else
    {
      // Ruseny kluc je v lavom podstrome
      if (key > (*tree)->key)
      {
        bst_node_t **auxVar = &(*tree)->right;
        bst_delete(auxVar, key);
        (*tree)->right = *auxVar;
      }
      // Najdeny uzol s danym klucom
      else
      {
        // Ruseny nema ziadneho syna
        if (((*tree)->left == NULL) && ((*tree)->right == NULL))
        {
          free((*tree));
          *tree = NULL;
        }
        // Ruseny ma dvoch synov
        else if (((*tree)->left != NULL) && ((*tree)->right != NULL))
        {
          bst_node_t *auxVar = (*tree)->right;
          bst_node_t *auxVar2 = auxVar;
          while (auxVar->left != NULL)
            auxVar = auxVar->left;
          while (auxVar2->left != auxVar)
            auxVar2 = auxVar2->left;

          (*tree)->key = auxVar->key;
          (*tree)->content.type = auxVar->content.type;
          (*tree)->content.value = auxVar->content.value;
          auxVar2->left = NULL;
          free(auxVar->left);
        }
        // Ruseny ma prave praveho syna
        else if ((*tree)->left == NULL)
        {
          bst_node_t *tmp = *tree;
          *tree = (*tree)->right;
          free(tmp);
        }
        // Ruseny ma prave laveho syna
        else
        {
          bst_node_t *tmp = *tree;
          *tree = (*tree)->left;
          free(tmp);
        }
      }
    }
  }
}

/*
 * Zrušení celého stromu.
 *
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených
 * uzlů.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree)
{
  if (*tree != NULL)
  {
    // Recursively dispose of the left and right subtrees
    bst_dispose(&(*tree)->left);
    bst_dispose(&(*tree)->right);

    // Free the current node and set it to NULL
    free(*tree);
    *tree = NULL;
  }
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items)
{
  if (tree == NULL)
    return;
  bst_add_node_to_items(tree, items);
  bst_preorder(tree->left, items);
  bst_preorder(tree->right, items);
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items)
{
  if (tree == NULL)
    return;
  bst_inorder(tree->left, items);
  bst_add_node_to_items(tree, items);
  bst_inorder(tree->right, items);
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items)
{
  if (tree == NULL)
    return;
  bst_postorder(tree->left, items);
  bst_postorder(tree->right, items);
  bst_add_node_to_items(tree, items);
}
