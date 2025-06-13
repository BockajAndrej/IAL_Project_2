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
  // Nastaví kořen stromu na NULL, čímž inicializuje prázdný strom
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
  {
    // Strom je prázdny nebo jsme dosáhli listu bez hledaného klíče
    return false; // Nenalezeno
  }
  else
  {
    if (tree->key == key)
    {
      // Nalezli jsme uzel s požadovaným klíčem
      (*value) = &tree->content;
      return true; // Nalezeno
    }
    else
    {
      if (key < tree->key)
      {
        // Hledaný klíč je menší než aktuální, pokračujeme vlevo
        return bst_search(tree->left, key, value);
      }
      else
      {
        // Hledaný klíč je větší než aktuální, pokračujeme vpravo
        return bst_search(tree->right, key, value);
      }
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
    // Pokud strom je prázdný, vytvoříme nový uzel
    (*tree) = (bst_node_t *)malloc(sizeof(bst_node_t));
    if (*tree != NULL)
    {
      // Inicializujeme nový uzel s daným klíčem a hodnotou
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
      // Klíč je menší než aktuální, pokračujeme vlevo
      bst_insert(&(*tree)->left, key, value);
    }
    else
    {
      if (key > (*tree)->key)
      {
        // Klíč je větší než aktuální, pokračujeme vpravo
        bst_insert(&(*tree)->right, key, value);
      }
      else
      {
        // Klíč již existuje, nahrazujeme jeho hodnotu
        if ((*tree)->content.value != NULL)
        {
          // Uvolníme předchozí hodnotu, pokud existuje
          free((*tree)->content.value);
        }
        // Aktualizujeme typ a hodnotu
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
  // Base case: Pokud není pravý potomek, aktuální uzel je nejpravější
  if ((*tree)->right == NULL)
  {
    // Uvolníme obsah cílového uzlu
    free(target->content.value);

    // Přeneseme obsah aktuálního (nejpravějšího) uzlu do cílového
    target->content = (*tree)->content;
    target->key = (*tree)->key;

    // Dočasně uchováme ukazatel na aktuální uzel
    bst_node_t *temp = *tree;

    // Nahrazujeme aktuální uzel jeho levým potomkem
    *tree = (*tree)->left;

    // Uvolníme původní nejpravější uzel
    free(temp);
    return;
  }

  // Rekurzivně pokračujeme na pravém podstromu, abychom našli nejpravější uzel
  bst_replace_by_rightmost(target, &(*tree)->right);
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
  // Pokud strom není prázdný, pokračujeme hledáním uzlu k odstranění
  if (*tree != NULL)
  {
    bst_node_t *current = *tree;

    if (key < current->key)
    {
      // Klíč k odstranění je menší, hledáme v levém podstromu
      bst_delete(&current->left, key);
    }
    else if (key > current->key)
    {
      // Klíč k odstranění je větší, hledáme v pravém podstromu
      bst_delete(&current->right, key);
    }
    else
    {
      // Nalezli jsme uzel s požadovaným klíčem
      if (current->left == NULL && current->right == NULL)
      {
        // Uzel nemá žádné potomky, můžeme jej jednoduše odstranit
        free(current->content.value); // Uvolníme obsah
        free(current);                // Uvolníme samotný uzel
        *tree = NULL;                 // Nastavíme ukazatel na NULL
      }
      else if (current->left != NULL && current->right != NULL)
      {
        // Uzel má oba podstromy, najdeme jeho nástupce
        bst_replace_by_rightmost(current, &current->left);
      }
      else
      {
        // Uzel má pouze jeden podstrom
        bst_node_t *child = (current->left != NULL) ? current->left : current->right;
        free(current->content.value); // Uvolníme obsah
        free(current);                // Uvolníme samotný uzel
        *tree = child;                // Předáme ukazatel na podstrom
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
    // Rekurzivně zrušíme levý podstrom
    bst_dispose(&(*tree)->left);

    // Rekurzivně zrušíme pravý podstrom
    bst_dispose(&(*tree)->right);

    // Uvolníme obsah aktuálního uzlu
    free((*tree)->content.value);
    (*tree)->content.value = NULL;

    // Uvolníme samotný uzel
    free(*tree);
    *tree = NULL; // Nastavíme ukazateľ na NULL
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
    return; // Základní případ: prázdný strom

  // Zpracujeme aktuální uzel před jeho podstromy
  bst_add_node_to_items(tree, items);

  // Rekurzivně procházíme levý podstrom
  bst_preorder(tree->left, items);

  // Rekurzivně procházíme pravý podstrom
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
    return; // Základní případ: prázdný strom

  // Rekurzivně procházíme levý podstrom
  bst_inorder(tree->left, items);

  // Zpracujeme aktuální uzel mezi podstromy
  bst_add_node_to_items(tree, items);

  // Rekurzivně procházíme pravý podstrom
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
    return; // Základní případ: prázdný strom

  // Rekurzivně procházíme levý podstrom
  bst_postorder(tree->left, items);

  // Rekurzivně procházíme pravý podstrom
  bst_postorder(tree->right, items);

  // Zpracujeme aktuální uzel po podstromech
  bst_add_node_to_items(tree, items);
}