/**
 * Modul implementující frontu portů.
 * Autor: Michal Šedý <xsedym02>
 * Standard: gnu11
 * Poslední úprava: 27.4.2020
 */

#include "port_queue.h"

/**
 * Konstruktor fronty.
 * ----------------------------------------
 * Alokuje v paměti místo pro struktůru fronty portů a vrátí získaný ukazatel.
 * Ukazatele first a last nastaví na NULL. Pokud se alokace nepovede, vrací NULL.
 */
port_queue_t *port_queue_ctor()
{
    port_queue_t *queue = (port_queue_t*) malloc(sizeof(port_queue_t));
    if(queue == NULL)
    {
        return NULL;
    }

    queue->first = NULL;
    queue->last = NULL;

    return queue;
}

/**
 * Destruktor fronty.
 * ----------------------------------------
 * queue - fronta určená k dealokaci, bude nastaveno na NULL
 * ----------------------------------------
 * Dealokuje sktrukturu fronty portů i s jejimi případnými položkami.
 * Ukazatel na frontu změní na NULL, aby volající jasně mohl detekovat dealokaci fronty.
 */
void port_queue_dtor(port_queue_t **queue)
{
    if(*queue == NULL)
    {
        return;
    }

    queue_item_t *tmp = (*queue)->first;
    queue_item_t *to_del;

    while(tmp != NULL)
    {
        to_del = tmp;
        tmp = tmp->next;
        free(to_del);
    }

    free(*queue);
    *queue = NULL;
}

/**
 * Vloží element do fronty.
 * ----------------------------------------
 * queue - fronta pro vkládání
 * p     - vkládaný element (číslo portu)
 * ----------------------------------------
 * Pokud se vložení do fronty nezdaří (nepovedená alokace, neexistující fronta),
 * vrátí funkce hodnotu false, v opačném případě true.
 */
bool put(port_queue_t *queue, unsigned short p)
{
    if(queue == NULL)
    {
        return false;
    }

    queue_item_t *new = (queue_item_t*) malloc(sizeof(queue_item_t));
    if(new == NULL)
    {
        return false;
    }

    new->val = p;
    new->next = NULL;
    // Rozdíl při vkládání do částečně zaplnéné a prázdné fronty.
    if(is_empty(queue))
    {
        queue->last = new;
        queue->first = new;
    }
    else
    {
        queue->last->next = new;
        queue->last = new;
    }
    
    return true;
}

/**
 * Získá element z fronty.
 * ----------------------------------------
 * queue - fronta pro získání elementu
 * ----------------------------------------
 * Získá první element z fronty, vrátí jej a tento prvek z fronty odstraní.
 * Pokud je funkce zavolána na prázdnou frontu, dojde k přístupu na NULL
 * a vyvolání chyby SEGMENTATION FAULT.
 */
unsigned short get(port_queue_t *queue)
{
    queue_item_t *req_item = queue->first;
    unsigned short to_ret = req_item->val;

    if(queue->first == queue->last)
    {
        queue->first = NULL;
        queue->last = NULL;
    }
    else
    {
        queue->first = queue->first->next;
    }

    free(req_item);

    return to_ret;
}

/**
 * Vrací booleovskou hodnotu prázdnosti fronty.
 * ----------------------------------------
 * queue - fronta pro zjištění prázdnosti
 * ----------------------------------------
 * Pokud je fronta prázdná, nebo neexistuje (NULL) je navrácena hodnota true,
 * v opačném případě false.
 */
bool is_empty(port_queue_t *queue)
{   
    if(queue == NULL)
    {
        return true;
    }

    return queue->first == NULL || queue->last == NULL;
}

/**
 * Provede hloubkovou kopii fronty.
 * ----------------------------------------
 * src - fronta pro kopírování
 * ----------------------------------------
 * Funkce vrátí ukazatel na hloubkovou kopii fronty src.
 * Pokud se kopie nezdaří (chyba alokace, zdroj neexistuje, ...), je vrácen NULL.
 */
port_queue_t *port_queue_cpy(port_queue_t *src)
{
    if(src == NULL)
    {
        return NULL;
    }

    port_queue_t *new = port_queue_ctor();
    if(new == NULL)
    {
        return NULL;
    }

    for(queue_item_t *tmp = src->first; tmp != NULL; tmp = tmp->next)
    {
        if(!put(new, tmp->val))
        {
            port_queue_dtor(&new);
            return NULL;
        }
    }

    return new;
}