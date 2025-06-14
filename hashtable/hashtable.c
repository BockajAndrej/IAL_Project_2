/*
 * Tabulka s rozptýlenými položkami
 *
 * S využitím datových typů ze souboru hashtable.h a připravených koster
 * funkcí implementujte tabulku s rozptýlenými položkami s explicitně
 * zretězenými synonymy.
 *
 * Při implementaci uvažujte velikost tabulky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptylovací funkce která přidělí zadanému klíči index z intervalu
 * <0,HT_SIZE-1>. Ideální rozptylovací funkce by měla rozprostírat klíče
 * rovnoměrně po všech indexech. Zamyslete sa nad kvalitou zvolené funkce.
 */
int get_hash(char *key)
{
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++)
    result += key[i];
  return (result % HT_SIZE);
}

/*
 * Inicializace tabulky — zavolá sa před prvním použitím tabulky.
 */
void ht_init(ht_table_t *table)
{
  // Preiteruje cez vsetky prvky table a nastavy ich na 
  for (int i = 0; i < HT_SIZE; i++)
    (*table)[i] = NULL;
}

/*
 * Vyhledání prvku v tabulce.
 *
 * V případě úspěchu vrací ukazatel na nalezený prvek; v opačném případě vrací
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key)
{
  int auxVar = get_hash(key);
  ht_item_t *item = (*table)[auxVar];

  while (item != NULL)
  {
    if (strcmp(item->key, key) == 0)
      return item;
    item = item->next;
  }
  return NULL;
}

/*
 * Vložení nového prvku do tabulky.
 *
 * Pokud prvek s daným klíčem už v tabulce existuje, nahraďte jeho hodnotu.
 *
 * Při implementaci využijte funkci ht_search. Pri vkládání prvku do seznamu
 * synonym zvolte nejefektivnější možnost a vložte prvek na začátek seznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value)
{
  int auxVar = get_hash(key);
  ht_item_t *exist = ht_search(table, key);

  if (exist == NULL)
  {
    ht_item_t *new = (ht_item_t *)malloc(sizeof(ht_item_t));
    if (new == NULL)
      return;

    char *key_word = (char *)malloc(sizeof(char) * (strlen(key) + 1));
    if (key_word == NULL)
      return;
    strcpy(key_word, key);
    // Naplnenie hodnotami
    new->key = key_word;
    new->value = value;
    new->next = NULL;

    new->next = (*table)[auxVar];
    (*table)[auxVar] = new;
  }
  else
    exist->value = value;
}

/*
 * Získání hodnoty z tabulky.
 *
 * V případě úspěchu vrací funkce ukazatel na hodnotu prvku, v opačném
 * případě hodnotu NULL.
 *
 * Při implementaci využijte funkci ht_search.
 */
float *ht_get(ht_table_t *table, char *key)
{
  ht_item_t *element = ht_search(table, key);
  if (element == NULL)
    return NULL;
  return &(element->value);
}

/*
 * Smazání prvku z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje přiřazené k danému prvku.
 * Pokud prvek neexistuje, funkce nedělá nic.
 *
 * Při implementaci NEPOUŽÍVEJTE funkci ht_search.
 */
void ht_delete(ht_table_t *table, char *key)
{
  int hash = get_hash(key);
  ht_item_t *item = (*table)[hash];
  ht_item_t *prev = NULL;

  while (item != NULL)
  {
    if (strcmp(item->key, key) == 0)
    {
      if (prev == NULL)
        (*table)[hash] = item->next;
      else
        prev->next = item->next;
      free(item->key);
      free(item);
      return;
    }
    prev = item;
    item = item->next;
  }
}

/*
 * Smazání všech prvků z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje a uvede tabulku do stavu po
 * inicializaci.
 */
void ht_delete_all(ht_table_t *table)
{
  ht_item_t *item;
  ht_item_t *futur;
  for (int i = 0; i < HT_SIZE; i++)
  {
    item = (*table)[i];
    futur = NULL;
    while (item != NULL)
    {
      futur = item->next;
      free(item->key);
      free(item);
      item = futur;
    }
    (*table)[i] = NULL;
  }
}
