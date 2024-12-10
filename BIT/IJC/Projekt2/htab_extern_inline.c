/**
 * htab_extern_inline.c
 * Autor: Michal Šedý
 * Řešení: IJD-DU2 b), 13.4.2019
 * Přeloženo: gcc 7.4.0
 * Popis: Modul obsahující externí definice inline funkcí.
 */

#include "htab.h"

extern inline bool htab_iterator_valid(htab_iterator_t it);
extern inline bool htab_iterator_equal(htab_iterator_t it1, htab_iterator_t it2);
