/**
 * Hlavičkový soubor modulu fronty.
 * Autor: Michal Šedý <xsedym02>
 * Standard: gnu11
 * Poslední úprava: 27.4.2020
 */

#ifndef __PORT_QUEUE_H__
#define __PORT_QUEUE_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct queue_item {
    unsigned short val;
    struct queue_item *next;
} queue_item_t;

typedef struct {
    queue_item_t *first;
    queue_item_t *last;
} port_queue_t;


/**
 * Konstruktor fronty.
 * ----------------------------------------
 * Alokuje v paměti místo pro struktůru fronty portů a vrátí získaný ukazatel.
 * Ukazatele first a last nastaví na NULL. Pokud se alokace nepovede, vrací NULL.
 */
port_queue_t *port_queue_ctor();

/**
 * Destruktor fronty.
 * ----------------------------------------
 * queue - fronta určená k dealokaci, bude nastaveno na NULL
 * ----------------------------------------
 * Dealokuje sktrukturu fronty portů i s jejimi případnými položkami.
 * Ukazatel na frontu změní na NULL, aby volající jasně mohl detekovat dealokaci fronty.
 */
void port_queue_dtor(port_queue_t **queue);

/**
 * Vloží element do fronty.
 * ----------------------------------------
 * queue - fronta pro vkládání
 * p     - vkládaný element
 * ----------------------------------------
 * Pokud se vložení do fronty nezdaří (nepovedená alokace, neexistující fronta),
 * vrátí funkce hodnotu false, v opačném případě true.
 */
bool put(port_queue_t *queue, unsigned short p);

/**
 * Získá element z fronty.
 * ----------------------------------------
 * queue - fronta pro získání elementu
 * ----------------------------------------
 * Získá první element z fronty, vrátí jej a tento prvek z fronty odstraní.
 * Pokud je funkce zavolána na prázdnou frontu, dojde k přístupu na NULL
 * a vyvolání chyby SEGMENTATION FAULT.
 */
unsigned short get(port_queue_t *queue);

/**
 * Vrací boolovshou hodnotu prázdnosti fronty.
 * ----------------------------------------
 * queue - fronta pro zjištění prázdnosti
 * ----------------------------------------
 * Pokud je fronta prázdná, nebo neexistuje (NULL) je navrácena hodnota true,
 * v opačném případě false.
 */
bool is_empty(port_queue_t *queue);

/**
 * Provede hloubkovou kopii fronty.
 * ----------------------------------------
 * src - fronta pro kopírování
 * ----------------------------------------
 * Funkce vrátí ukazatel na hloubkovou kopii fronty src.
 * Pokud se kopie nezdaří (chyba alokace, zdroj neexistuje, ...), je vrácen NULL.
 */
port_queue_t *port_queue_cpy(port_queue_t *src);

#endif