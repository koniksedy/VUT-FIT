/* Kostra programu pro 3. projekt IZP 2018/19
 * ================================================
 * Jednoducha shlukova analyza: 2D nejblizsi soused.
 * Single linkage
 * Michal Šedý <xsedym02>
 * Zahájení 3.12.2018
 * Poslední úprava 9.12.2018
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t {
    int id;
    float x;
    float y;
};

struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap)
{
    assert(c != NULL);
    assert(cap >= 0);
	
	c->size = 0;
	c->capacity = cap;
	c->obj = NULL;
}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{
	free(c->obj);
	init_cluster(c, 0);
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
        return c;

    size_t size = sizeof(struct obj_t) * new_cap;

    void *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = (struct obj_t*)arr;
    c->capacity = new_cap;
    return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
	if(c->capacity <= c->size)
	{
		if(resize_cluster(c, c->capacity + CLUSTER_CHUNK) == NULL)
		{
			clear_cluster(c);
			return;
		}
	}
	c->obj[c->size] = obj;
	c->size++;
}

/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazeny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c2 != NULL);
	
	for(int i = 0; i < c2->size; i++)
	{
		append_cluster(c1, c2->obj[i]);
		if(c1->obj == NULL)
			return;
	}
	sort_cluster(c1);
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
    assert(idx < narr);
    assert(narr > 0);

	clear_cluster(&carr[idx]);
	//Popřeskupení pole
	for(int i = idx; i < narr-1; i++)
	{
		carr[i] = carr[i+1];
	}
	return narr-1;
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);

	float vysledek = sqrtf(powf(o1->x-o2->x, 2) + powf(o1->y-o2->y, 2));
	return vysledek;

}

/*
 Pocita vzdalenost dvou shluku.
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);

	float vzdalenost;
	float min_vzdalenost = sqrtf(2)*1000;
	for(int i = 0; i < c1->size; i++)
	{
		for(int j = i; j < c2->size; j++)
		{
			vzdalenost = obj_distance(&c1->obj[i], &c2->obj[j]);
			if(vzdalenost < min_vzdalenost)
			{
				min_vzdalenost = vzdalenost;
			}
		}
	}
	return min_vzdalenost;
}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
    assert(narr > 0);
	float min_vzdalenost = sqrtf(2)*1000;
	float vzdalenost;
	for(int i = 0; i < narr; i++)
	{
		for(int j = i+1; j < narr; j++)
		{
			vzdalenost = cluster_distance(&carr[i], &carr[j]);
			if(vzdalenost < min_vzdalenost)
			{
				*c1 = i;
				*c2 = j;
				min_vzdalenost = vzdalenost;
			}
		}
	}
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = (const struct obj_t *)a;
    const struct obj_t *o2 = (const struct obj_t *)b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}

/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char *filename, struct cluster_t **arr)
{
	FILE *soubor = fopen(filename, "r");
	const int delka = 10*3+3;	//10 = delka INT_MAX,3 = na řádku jsou 3 int, 3 = to jsou 2 mezery a \0
	int n;	//Předpokládaný počet řádků v souboru
	char radek[delka];	
	char zbytek;
	int i = 0;	// Počet načtených řádků
	struct obj_t objekt;
	int id, x, y;
	int err = 0;
	
	// KONTROLA otevření souboru
	if(soubor == NULL)
	{
		fprintf(stderr, "CHYBA: Soubor se nepodařilo otevřít.\n");
		*arr = NULL;
		return 0;
	}
	//KONTROLA neprázdnosti souboru
	else if(fgets(radek, delka, soubor) == NULL)
	{
		fprintf(stderr, "CHYBA: Soubor je prázdný.\n");
		*arr = NULL;
	}

	//NAČÍTÁNÍ a následná KONTROLA předpokládaného počtu řádků
	else if(sscanf(radek, "count=%d%[^\n]", &n, &zbytek) != 1)
	{
		fprintf(stderr, "CHYBA: Nepovedlo se zpracovat prvním řádek v souboru.\n"
						"       Zkontrolute, zadali je count celé číslo.\n");
		*arr = NULL;
	}
	else if(n < 0)
	{
		fprintf(stderr, "CHYBA: V souboru musí být číslo count nezáporné.\n");
		*arr = NULL;
	}

	//ALOKACE a následná KONTROLA polel pro struktury
	else if((*arr = malloc(sizeof(struct cluster_t) * n)) == NULL)
		fprintf(stderr, "CHYBA: Nepovedla se alokace pole pro struktury.\n");

	//Všechny kontroly proběhly úspěšně
	else
	{
		while(i < n)
		{	
			//INICIALIZACE složky pole
			init_cluster(&(*arr)[i], 0);
			//KONTROLA neprázdnosti řádků
			if(fgets(radek, delka, soubor) == NULL)
			{
				fprintf(stderr, "CHYBA: Špatný počet řádků v souboru.\n");
				err = -1;
			}
			//NAČÍTÁNÍ a KONTROLA číselných hodnot z řádků
			else if(sscanf(radek, "%d %d %d %[^\n]", &id, &x, &y, &zbytek) != 3)
			{
				err = -1;
				fprintf(stderr, "CHYBA: Došlo k chybě při načítání hodnot z řádků v souboru.\n");
			}
			//KONTROLA jedinečnosti id
			else
			{
				for(int j = 0; j < i; j++)
				{
					if(id == (*arr)[j].obj->id)
					{
						err = -1;
						fprintf(stderr, "CHYBA: Kolize id.\n");
						break;
					}
				}
			}
			objekt.id = (float) id;
			objekt.x = (float) x;
			objekt.y = (float) y;
			//KONTROLA rozsahu hodnot POZN. - využití zrychleného vyhodnocování, díky tomu nenastane error
			if(err == 0 && (objekt.id < 0 || objekt.x < 0 || objekt.x > 1000 || objekt.y < 0 || objekt.y > 1000))
			{
				err = -1;
				fprintf(stderr, "CHYBA: Nesprávné hodnoty objektu.\n");
			}
			//NAHRÁVÁNÍ a KONTOROLA úspěšnosti append
			if(err == 0)
			{
				append_cluster(&(*arr)[i], objekt);
				if((*arr)[i].obj == NULL)
				{
					err = -1;
					fprintf(stderr, "CHYBA. Vyskytl se problém při alokaci paměti.\n");
				}
			}

			if(err != 0)
			{
				//DEALOKACE shluků při chybě
				for(int k = 0; k < i; k++)
				{
					dint(k);
					clear_cluster(&(*arr)[k]);
				}
				free(*arr);
				*arr = NULL;
				i = 0;
				break;
			}
			i++;
		}
	}
	fclose(soubor);
	return i;
}

/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++)
    {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}

int main(int argc, char *argv[])
{
	int fin_velikost = 1;		//požadované množství shluků
	int akt_velikost;
    struct cluster_t *clusters;	//pole shluků
	int index_shluk1;	
	int index_shluk2;
	char *zbytek = NULL;
	int navrhod = 0;

	//KONTROLA počtu argumentů
	if(argc < 2 || argc > 3)
	{
		fprintf(stderr, "CHYBA: Špatný počet argumenty spuštění.\n");
		return 1;
	}

	//NAČÍTÁNÍ fin_velikost
	if(argc == 3)
	{
		fin_velikost = (int) strtol(argv[2], &zbytek, 10);
		if(*zbytek)
		{
			fprintf(stderr, "CHYBA: Druhý agument musí být celé číslo.\n");
			return  1;
		}
		if(fin_velikost <= 0)
		{
			fprintf(stderr, "CHYBA: Drudý argument musí být celé nezáporné číslo.\n");
			return  1;
		}
	}

	akt_velikost = load_clusters(argv[1], &clusters);
	//KONTROLAL LOADU
	if(clusters == NULL)
		return 1;
	else
	{
		//SHLUKOVÁNÍ
		while(akt_velikost > fin_velikost)
		{
			find_neighbours(clusters, akt_velikost, &index_shluk1, &index_shluk2);
			merge_clusters(&clusters[index_shluk1], &clusters[index_shluk2]);
			akt_velikost = remove_cluster(clusters, akt_velikost, index_shluk2);
			if(clusters[index_shluk1].obj == NULL)
				akt_velikost = -akt_velikost;
		}

		//KONTROLA zda proběhlo shlukování správné	
		if(akt_velikost < 0)
		{
			fprintf(stderr, "CHYBA: Nepodařila se alokace paměti.\n");
			akt_velikost = -akt_velikost;
			navrhod = 1;
		}
		else
			print_clusters(clusters, akt_velikost);
	}

	//DEALOKACE shluků *clusters
	for(int i = 0; i < akt_velikost; i++)
	{
		clear_cluster(&clusters[i]);
	}
	//DEALOKACE samotného pole shluků
	free(clusters);
	return navrhod;
}
