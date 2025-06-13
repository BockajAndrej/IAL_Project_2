/*
 * Binární vyhledávací strom — iterativní varianta
 *
 * S využitím datových typů ze souboru btree.h, zásobníku ze souboru stack.h
 * a připravených koster funkcí implementujte binární vyhledávací
 * strom bez použití rekurze.
 */

#include "../btree.h"
#include "stack.h"
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
 * Funkci implementujte iterativně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, bst_node_content_t **value)
{
  while (tree != NULL)
  {
    if (tree->key > key)
      tree = tree->left;
    else if (tree->key < key)
      tree = tree->right;
    else
    {
      *value = &tree->content;
      return true;
    }
  }
  return false;
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
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, bst_node_content_t value)
{
  bst_node_t **auxVar = tree;
  while ((*auxVar) != NULL)
  {
    if ((*auxVar)->key > key)
      auxVar = &((*auxVar)->left);
    else if ((*auxVar)->key < key)
      auxVar = &(*auxVar)->right;
    else
    {
      if ((*auxVar)->content.value != NULL)
        free((*auxVar)->content.value);
      (*auxVar)->content = value;
      return;
    }
  }

  (*auxVar) = (bst_node_t *)malloc(sizeof(bst_node_t));
  if ((*auxVar) != NULL)
  {
    (*auxVar)->key = key;
    (*auxVar)->content = value;
    (*auxVar)->left = NULL;
    (*auxVar)->right = NULL;
  }
  else
  {
    free((*auxVar)->content.value);
    fprintf(stderr, "Memory allocation failed for key '%c'\n", key);
  }
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 *
 * Klíč a hodnota uzlu target budou nahrazené klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 *
 * Tato pomocná funkce bude využita při implementaci funkce bst_delete.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree)
{
  bst_node_t *parent = *tree;
  bst_node_t *current = (*tree)->left;

  while (current->right != NULL)
  {
    parent = current;
    current = current->right;
  }

  free(target->content.value);
  target->content = current->content;
  target->key = current->key;

  parent->right = current->left;

  // Uvolnenie pamate
  free(current);
  current = NULL;
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
 * Funkci implementujte iterativně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key)
{
  bst_node_t *current = *tree;
  bst_node_t *parent = *tree;
  bool dir = false;

  while (current != NULL)
  {
    // Ruseny kluc je v pravom podstrome
    if (key < current->key)
    {
      dir = false;
      parent = current;
      current = current->left;
    }
    // Ruseny kluc je v lavom podstrome
    else if (key > current->key)
    {
      dir = true;
      parent = current;
      current = current->right;
    }
    else
      break;
  }
  if (current == NULL)
    return;
  // Najdeny uzol s danym klucom
  // Ruseny nema ziadneho syna
  if ((current->left == NULL) && (current->right == NULL))
  {
    if (dir)
      parent->right = NULL;
    else
      parent->left = NULL;
    free(current->content.value);
    current->content.value = NULL;
    free(current);
    current = NULL;
  }
  // Ruseny ma dvoch synov
  else if ((current->left != NULL) && (current->right != NULL))
  {
    bst_replace_by_rightmost(current, &current);
  }
  // Ruseny ma prave praveho syna
  else if (current->left == NULL)
  {
    if (dir)
      parent->right = current->right;
    else
      parent->left = current->right;
    // Uvolnenie nejprv obsahu a nasledne celeho uzlu
    free(current->content.value);
    current->content.value = NULL;
    free(current);
    current = NULL;
  }
  // Ruseny ma prave laveho syna
  else
  {
    if (dir)
      parent->right = current->left;
    else
      parent->left = current->left;
      // Uvolnenie nejprv obsahu a nasledne celeho uzlu
    free(current->content.value);
    current->content.value = NULL;
    free(current);
    current = NULL;
  }
}

/*
 * Zrušení celého stromu.
 *
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených
 * uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree)
{
  if (*tree == NULL)
    return; // Nothing to delete

  // Initialize the stack for node pointers
  stack_bst_t stack;
  stack_bst_init(&stack);

  // Push the root node onto the stack
  stack_bst_push(&stack, *tree);

  while (!stack_bst_empty(&stack))
  {
    // Pop a node from the stack
    bst_node_t *node = stack_bst_pop(&stack);

    // Push the right and left children onto the stack, if they exist
    if (node->right)
    {
      stack_bst_push(&stack, node->right);
    }
    if (node->left)
    {
      stack_bst_push(&stack, node->left);
    }

    // Free the current node
    free(node->content.value);
    node->content.value = NULL;
    free(node);
    node = NULL;
  }

  // Set the original tree root to NULL
  *tree = NULL;
}

/*
 * Pomocná funkce pro iterativní preorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu.
 * Nad zpracovanými uzly zavolá bst_add_node_to_items a uloží je do zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit, bst_items_t *items)
{
  while (tree != NULL)
  {
    stack_bst_push(to_visit, tree);
    bst_add_node_to_items(tree, items);
    tree = tree->left;
  }
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_preorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items)
{
  stack_bst_t stack;
  stack_bst_init(&stack);
  bst_leftmost_preorder(tree, &stack, items);

  while (!stack_bst_empty(&stack))
  {
    tree = stack_bst_pop(&stack);
    bst_leftmost_preorder(tree->right, &stack, items);
  }
}

/*
 * Pomocná funkce pro iterativní inorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit)
{
  while (tree != NULL)
  {
    stack_bst_push(to_visit, tree);
    tree = tree->left;
  }
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_inorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items)
{
  stack_bst_t stack;
  stack_bst_init(&stack);
  bst_leftmost_inorder(tree, &stack);

  while (!stack_bst_empty(&stack))
  {
    tree = stack_bst_pop(&stack);
    bst_add_node_to_items(tree, items);
    bst_leftmost_inorder(tree->right, &stack);
  }
}

/*
 * Pomocná funkce pro iterativní postorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů. Do zásobníku bool hodnot ukládá informaci, že uzel
 * byl navštíven poprvé.
 *
 * Funkci implementujte iterativně pomocí zásobníku uzlů a bool hodnot a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit)
{
  while (tree != NULL)
  {
    stack_bst_push(to_visit, tree);
    stack_bool_push(first_visit, true);
    tree = tree->left;
  }
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_postorder a
 * zásobníku uzlů a bool hodnot a bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items)
{
  bool fromLeft;
  stack_bst_t s;
  stack_bst_init(&s);
  stack_bool_t sb;
  stack_bool_init(&sb);
  bst_leftmost_postorder(tree, &s, &sb);

  while (!stack_bst_empty(&s))
  {
    tree = stack_bst_top(&s);
    fromLeft = stack_bool_pop(&sb);

    if (fromLeft)
    {
      stack_bool_push(&sb, false);
      bst_leftmost_postorder(tree->right, &s, &sb);
    }
    else
    {
      stack_bst_pop(&s);
      bst_add_node_to_items(tree, items);
    }
  }
}
