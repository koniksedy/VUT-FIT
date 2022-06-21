/**
 * proj2.c
 * Autor: Michal Šedý <xsedym02>, FIT
 * Řešení: IOS - projekt 2
 * Přeloženo: gcc 5.4.0
 *			  Pro překlad použijte Makefile
 * Datum: 22.4.2019
 * Popis: Jedné se o zdrojový soubor programu implementující River Crossing Problem.
 *		  Na řece je vyhlídková loď, která může vést právě čtyři pasažéry.
 *		  Pasažéři se dělí na dvě skupinyny Hackers a Serfs. Pro plavbu jsou povoleny
 *		  pouze následnující kombinace: 4 kackers; 4 serfs; 2 hackers a 2 serfs.
 *		  Osoby, které se chtějí přepravit se shromažďuji na molu. Molo má omezenou kapacitu,
 *		  pokud se mově příchozí osoba již na molo nevejde, odchází pryč a zkusí přijít později.
 *		  Jakmile je možné na molo vytvořit vhodnou skupinu, nastoupí tato skupina na loď a odpluje.
 *		  Po dobu plavby (včeetně vystupuvání) nevstupuje na loď žádná osoba, mohou však přicházet
 *		  na molo. Kapitán opouští loď jako poslední.
 *
 * Poznámky ke spuštění:	./proj2 P H S R W C
 *
 *		  P - počet osob osob generovanách v každé kategorii; bude vytvořeno P hackers a P serfs.
 *			P > = 2 && (P % 2) == 0
 *
 *		  H - maximální hodnota doby (v milisekundách), po které je generován nový proces hackers.
 *			H >= 0 && H <= 2000
 *
 *		  S - maximální hodnota doby (v milisekundách), po které je generován nový proces serfs.
 *			S >= 0 && S <= 2000
 *
 *		  R - maximální hodnota doby (v milisekundách), plavby.
 *			R >= 0 && R <= 2000
 *
 *		  W - meximální hodnota doby (v milisekundách), po které se osoba vrací zpět na molo.
 *			W >= 20 && W <= 2000
 *
 *		  C - kapacita mola.
 *			C >= 5
 *
 *		  Všechny parametry jsou celá čísla.
 */

// Knihovny
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>

// Definice návratových kódů
#define SUCCESS 0
#define FAILED 1

// Sdílené proměnné
int *shm_hackers;			// Počet hackers na molu.  (/xsedym02-hackers-shm)
int *shm_hackers_solo;		// Počet hackers bez skupiny  (/xsedym02-hackerssolo-shm)
int *shm_serfs;				// Počet serfs na molu.  (/xsedym02-serfs-shm)
int *shm_serfs_solo;		// Počet serfs bez skupiny  (/xsedym02-serfs-shm)
int *shm_action;			// Pořadové číslo akce  (/xsedym02-action-shm)

// Semafory
sem_t *sem_molo;			// Synchronizuje počet lidí na molu  (/xsedym02-molo-sem)
sem_t *sem_hackers_queue;	// Spravuje čekající hackers na nalodění  (/xsedym02-hackersqueue-sem)
sem_t *sem_serfs_queue;		// Spravuje čekající serfs na nalodění  (/xsedym02-serfsqueue-sem)
sem_t *sem_captain;			// Synchronizje pasažéry při vylodění  (/xsedym02-captain-sem)
sem_t *sem_board;			// Zamezí nástupu na používanou loď  (/xsedym02-board-sem)
sem_t *sem_land;			// Synchronizuje procesy na plavbě  (/xsedym02-land-sem)
sem_t *sem_action;			// Synchronizuje výpis akcí  (/xsedym02-action-sem)

// Soubor pro výstup
FILE *output = NULL;

// Definice chybových kódu a chybových zpráv
enum {
	HELP,
	ARG_COUNT,
	TRY_PARSE,
	ARG_VALUE,
	FOPEN,
	FORK_GEN_H,
	FORK_GEN_S,
	FORK_CHILD_H,
	FORK_CHILD_S,
	SHM_OPEN,
	FTRUNCATE,
	MMAP,
	SEM_OPEN,
	CREATE_SOURCES,
	OPEN_SOURCES,
	FATAL_HACKER,
	FATAL_SERF,
};

const char *ERROR_MSG[] = {
	[HELP] = "=============================================================================================\n"
			 "Spuštění programu: ./proj2 P H S R W C\n"
			 "* P - počet osob osob generovanách v každé kategorii; bude vytvořeno P hackers a P serfs.\n"
			 "  P > = 2 && (P % 2) == 0\n"
			 "* H - maximální hodnota doby (v milisekundách), po které je generován nový proces hackers.\n"
			 "  H >= 0 && H <= 2000\n"
			 "* S - maximální hodnota doby (v milisekundách), po které je generován nový proces serfs.\n"
			 "  S >= 0 && S <= 2000\n"
			 "* R - maximální hodnota doby (v milisekundách), plavby.\n"
			 "  R >= 0 && R <= 2000\n"
			 "* W - meximální hodnota doby (v milisekundách), po které se osoba vrací zpět na molo.\n"
			 "  W >= 20 && W <= 2000\n"
			 "* C - kapacita mola.\n"
			 "  C >= 5\n"
			 "\n"
			 "Všechny parametry jsou celá čísla.\n"
			 "=============================================================================================\n",
	[ARG_COUNT] = "CHYBA: Chybný počet argumentů spuštění.\n",
	[TRY_PARSE] = "CHYBA: Všechny parametry musí být celočíselné.\n",
	[ARG_VALUE] = "CHYBA: Hodnota argumentů je chybně zvolena.\n",
	[FOPEN] = "CHYBA: Nepodařil se otevřít soubor pro zápis.\n",
	[FORK_GEN_H] = "CHYBA: Nepodařil se vytvořit (fork) generátor hackers.\n",
	[FORK_GEN_S] = "CHYBA: Nepodařil se vytvořit (fork) generátor serfs.\n",
	[FORK_CHILD_H] = "CHYBA: Nepodařil se vytvořit (fork) proces hackers.\n",
	[FORK_CHILD_S] = "CHYBA: Nepodařil se vytvořit (fork) proces serfs.\n",
	[SHM_OPEN] = "CHYBA: shm_open selhal.\n",
	[FTRUNCATE] = "CHYBA: ftruncate selhal.\n",
	[MMAP] = "CHYBA: mmap selhal.\n",
	[SEM_OPEN] = "CHYBA: sem_open selhal.\n",
	[CREATE_SOURCES] = "CHYBA: Nepodařilo se vytvoření paměťových zdrojů.\n",
	[OPEN_SOURCES] = "CHYBA: Nepodařilo se otevřít sdílené paměťové zdroje.\n",
	[FATAL_HACKER] = "CHYBA: Nastala falální chyba v procesu hacker.\n",
	[FATAL_SERF] = "CHYBA: Nastala fatální chyba v procesu serf.\n"
};

// Prototypy funkcí
int init_all();
void unlink_all();
int string_to_int(const char *str);
int open_sources();
void close_sources();
int hacker_proces(int id, int cap, int sleep, int sail);
int serf_proces(int id, int cap, int sleep, int sail);


int main(int argc, char *argv[])
{
	int P, H, S, R, W, C;
	pid_t pid_hackers_gen, pid_serfs_gen, pid_serf, pid_hacker;

	// Kontrola počtu parametrů
	if(argc != 7)
	{
		fprintf(stderr, "%s", ERROR_MSG[ARG_COUNT]);
		return FAILED;
	}

	// Načítání a kontrola parametrů
	P = string_to_int(argv[1]);
	H = string_to_int(argv[2]);
	S = string_to_int(argv[3]);
	R = string_to_int(argv[4]);
	W = string_to_int(argv[5]);
	C = string_to_int(argv[6]);

	// Kontrola hodnot parametrů
	if((P < 2) || ((P % 2) != 0) ||
	   (H < 0) || (H > 2000) ||
	   (S < 0) || (S > 2000) ||
	   (R < 0) || (R > 2000) ||
	   (W < 20) || (W > 2000) ||
	   (C < 5))
	{
		fprintf(stderr, "%s\n%s", ERROR_MSG[ARG_VALUE], ERROR_MSG[HELP]);
		return FAILED;
	}

	// Otevření souboru pro zápis
	output = fopen("proj2.out", "w");
	if(output == NULL)
	{
		fprintf(stderr, "%s", ERROR_MSG[FOPEN]);
		return FAILED;
	}
	
	setbuf(output, NULL);
	setbuf(stderr, NULL);

	// Vytvoření paměťových zdrojů a kontrola úspěchu
	if(init_all() != SUCCESS)
	{
		fprintf(stderr, "%s", ERROR_MSG[CREATE_SOURCES]);
		unlink_all();
		return FAILED;
	}

	// Fork - generator hackers
	pid_hackers_gen = fork();
	if(pid_hackers_gen < 0)			// Chyba - fork
	{	
		fprintf(stderr, "%s", ERROR_MSG[FORK_GEN_H]);
		unlink_all();
		exit(FAILED);
	}
	else if(pid_hackers_gen== 0)	// Child - hackers generator
	{
		// Vygeneruje P hacker procesů
		for(int i = 0; i < P; i++)
		{
			// Prodleva mezi generováním
			if(H != 0)
			{
				usleep(random() % H * 1000);
			}
			// Fork - proces hacker
			pid_hacker = fork();
			if(pid_hacker < 0)		// Chyba - fork
			{
				fprintf(stderr, "%s", ERROR_MSG[FORK_CHILD_H]);
				unlink_all();
				kill(pid_hackers_gen, SIGTERM);
				exit(FAILED);
			}
			else if(pid_hacker == 0)// Child - hacker
			{
				// Spuštění a kontroa procesu hacker
				if(hacker_proces(i+1, C, W, R) != SUCCESS)
				{
					fprintf(stderr, "%s", ERROR_MSG[FATAL_HACKER]);
					unlink_all();
					kill(pid_hackers_gen, SIGTERM);
					exit(FAILED);
				}
				exit(SUCCESS);		// Child hacker - end
			}
		}
		// Čeká se ma ukončení všech hackers
		for(int i = 0; i < P; i++)
		{
			wait(NULL);
		}
		exit(SUCCESS);				// Hacker generator - end
	}

	// Fork - generator serfs
	pid_serfs_gen = fork();
	if(pid_serfs_gen < 0)			// Chyba - fork
	{
		fprintf(stderr, "%s", ERROR_MSG[FORK_GEN_S]);
		unlink_all();
		kill(pid_hackers_gen, SIGTERM);
		exit(FAILED);
	}
	else if(pid_serfs_gen == 0)		// Child - serfs generator
	{
		// Vygeneruje P procesů serf
		srand(time(NULL) + getpid());
		for(int i = 0; i < P; i++)
		{
			// Prodleva mezi generováním
			if(S != 0)
			{
				usleep(random() % S * 1000);
			}
			// Fork - proces serf
			pid_serf = fork();
			if(pid_serf < 0)		// Chyba - fork
			{
				fprintf(stderr, "%s", ERROR_MSG[FORK_CHILD_S]);
				unlink_all();
				kill(pid_serfs_gen, SIGTERM);
				exit(FAILED);
			}
			else if(pid_serf == 0)	// Child - serf
			{
				// Spuštění a kontrola procesu serf
				if(serf_proces(i+1, C, W, R) != SUCCESS)
				{
					fprintf(stderr, "%s", ERROR_MSG[FATAL_SERF]);
					unlink_all();
					kill(pid_serfs_gen, SIGTERM);
					exit(FAILED);
				}
				exit(SUCCESS);		// Child serf - end
			}
		}
		// Čeká se na ukončení všech serfs
		for(int i = 0; i < P; i++)
		{
			wait(NULL);
		}
		exit(SUCCESS);				// Serfs generator - end
	}

	// Čekání na ukončení obou generátorů
	for(int i = 0; i < 2; i++)
	{
		wait(NULL);
	}

	unlink_all();
	return SUCCESS;
}

/**
 * Fce pro převod čísla v podobě textového řetězce na int.
 * Vstup - str - řetězec pro převod
 * Výstup - Fce vrací v případě úspěchu převedené číslo,
 *		  - v opačném případě zahlásí chybu a ukončí program.
 */
int string_to_int(const char *str)
{
	char *end = NULL;
	int n = (int)strtol(str, &end, 10);

	if(*end != '\0')
	{
		fprintf(stderr, "%s", ERROR_MSG[TRY_PARSE]);
		exit(FAILED);
	}

	return n;
}

/**
 * Fce pro vytvoření paměťových zdrojů - semafory, sdílené proměnné.
 * Výstup - Fce vrací v případě úspěšného vytvoření SUCCESS (0), v připadě chyby FAILED (0)
 */
int init_all()
{
	int err = 0;
	
	// Získání fd a kontrola
	int hackers_ID = shm_open("/xsedym02-hackers-shm", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
	int hackers_solo_ID = shm_open("/xsedym02-hackerssolo-shm", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
	int serfs_ID = shm_open("/xsedym02-serfs-shm", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
	int serfs_solo_ID = shm_open("/xsedym02-serfssolo-shm", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
	int action_ID = shm_open("/xsedym02-action-shm", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
	if((hackers_ID < 0) ||
	   (hackers_solo_ID < 0) ||
	   (serfs_ID < 0) ||
	   (serfs_solo_ID < 0) ||
	   (action_ID < 0))
	{
		fprintf(stderr, "%s", ERROR_MSG[SHM_OPEN]);
		return FAILED;
	}	

	// Roztažení na požadovanou velikost a kontrola
	err += ftruncate(hackers_ID, sizeof(int));
	err += ftruncate(hackers_solo_ID, sizeof(int));
	err += ftruncate(serfs_ID, sizeof(int));
	err += ftruncate(serfs_solo_ID, sizeof(int));
	err += ftruncate(action_ID, sizeof(int));
	if(err != 0)
	{
		fprintf(stderr, "%s", ERROR_MSG[FTRUNCATE]);
		return FAILED;
	}

	// Namapování do shm... a kontrola
	shm_hackers = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, hackers_ID, 0);
	shm_hackers_solo = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, hackers_solo_ID, 0);
	shm_serfs = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, serfs_ID, 0);
	shm_serfs_solo = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, serfs_solo_ID, 0);
	shm_action = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, action_ID, 0);
	if((shm_hackers == MAP_FAILED) ||
	   (shm_hackers_solo == MAP_FAILED) ||
	   (shm_serfs == MAP_FAILED) ||
	   (shm_serfs_solo == MAP_FAILED) ||
	   (shm_action == MAP_FAILED))
	{
		fprintf(stderr, "%s", ERROR_MSG[MMAP]);
		return FAILED;
	}

	// Uzavření fd
	close(hackers_ID);
	close(hackers_solo_ID);
	close(serfs_ID);
	close(serfs_solo_ID);
	close(action_ID);

	// Nastavení hodnot sdílených proměnných
	*shm_hackers = 0;
	*shm_hackers_solo = 0;
	*shm_serfs = 0;
	*shm_serfs_solo = 0;
	*shm_action = 1;		// Akce se číslují od 1

	// Odpojení od sdílených proměnných
	munmap(shm_hackers, sizeof(int));
	munmap(shm_hackers_solo, sizeof(int));
	munmap(shm_serfs, sizeof(int));
	munmap(shm_serfs_solo, sizeof(int));
	munmap(shm_action, sizeof(int));

	// Vytvoření semaforu a kontrola
	sem_molo = sem_open("/xsedym02-molo-sem", O_CREAT | O_EXCL, 0644, 1);
	sem_hackers_queue = sem_open("/xsedym02-hackersqueue-sem", O_CREAT | O_EXCL, 0644, 0);
	sem_serfs_queue = sem_open("/xsedym02-serfsqueue-sem", O_CREAT | O_EXCL, 0644, 0);
	sem_captain = sem_open("/xsedym02-captain-sem", O_CREAT | O_EXCL, 0644, 0);
	sem_board = sem_open("/xsedym02-board-sem", O_CREAT | O_EXCL, 0644, 1);
	sem_land = sem_open("/xsedym02-land-sem", O_CREAT | O_EXCL, 0644, 0);
	sem_action = sem_open("/xsedym02-action-sem", O_CREAT | O_EXCL, 0644, 1);
	if((sem_molo == SEM_FAILED) ||
	   (sem_hackers_queue == SEM_FAILED) ||
	   (sem_serfs_queue == SEM_FAILED) ||
	   (sem_captain == SEM_FAILED) ||
	   (sem_board == SEM_FAILED) ||
	   (sem_land == SEM_FAILED) ||
	   (sem_action == SEM_FAILED))
	{
		fprintf(stderr, "%s", ERROR_MSG[SEM_OPEN]);
		return FAILED;
	}

	// Odpojení od semaforů
	sem_close(sem_molo);
	sem_close(sem_hackers_queue);
	sem_close(sem_serfs_queue);
	sem_close(sem_captain);
	sem_close(sem_board);
	sem_close(sem_land);
	sem_close(sem_action);

	return SUCCESS;
}

/**
 * Fce pro zrušení všech paměťových zdrojů a zavření souboru pro výpis
 */
void unlink_all()
{
	// Zavření souboru pro výpis
	fclose(output);

	// Zrušení sdílených proměnných
	shm_unlink("/xsedym02-hackers-shm");
	shm_unlink("/xsedym02-hackerssolo-shm");
	shm_unlink("/xsedym02-serfs-shm");
	shm_unlink("/xsedym02-serfssolo-shm");
	shm_unlink("/xsedym02-action-shm");

	// Zřušení semaforů
	sem_unlink("/xsedym02-molo-sem");
	sem_unlink("/xsedym02-hackersqueue-sem");
	sem_unlink("/xsedym02-serfsqueue-sem");
	sem_unlink("/xsedym02-captain-sem");
	sem_unlink("/xsedym02-board-sem");
	sem_unlink("/xsedym02-land-sem");
	sem_unlink("/xsedym02-action-sem");
}

/**
 * Fce pro připojení paměťových zdrojů.
 * Výstup - Fce vrací v případě úspěšného připojení SUCCESS (0), v případě chyby FAIL (1).
 */
int open_sources()
{
	// Připojení semaforů a kontrola

	sem_molo = sem_open("/xsedym02-molo-sem", O_RDWR);
	sem_hackers_queue = sem_open("/xsedym02-hackersqueue-sem", O_RDWR);
	sem_serfs_queue = sem_open("/xsedym02-serfsqueue-sem", O_RDWR);
	sem_captain = sem_open("/xsedym02-captain-sem", O_RDWR);
	sem_board = sem_open("/xsedym02-board-sem", O_RDWR);
	sem_land = sem_open("/xsedym02-land-sem", O_RDWR);
	sem_action = sem_open("/xsedym02-action-sem", O_RDWR);
	if((sem_molo == SEM_FAILED) ||
	   (sem_hackers_queue == SEM_FAILED) ||
	   (sem_serfs_queue == SEM_FAILED) ||
	   (sem_captain == SEM_FAILED) ||
	   (sem_board == SEM_FAILED) ||
	   (sem_land == SEM_FAILED) ||
	   (sem_action == SEM_FAILED))
	{
		fprintf(stderr, "%s", ERROR_MSG[SEM_OPEN]);
		return FAILED;
	}

	// Připojení ke sdíleným proměnným

	int hackers_ID = shm_open("/xsedym02-hackers-shm", O_RDWR, S_IRUSR | S_IWUSR);
	int hackers_solo_ID = shm_open("/xsedym02-hackerssolo-shm", O_RDWR, S_IRUSR | S_IWUSR);
	int serfs_ID = shm_open("/xsedym02-serfs-shm", O_RDWR, S_IRUSR | S_IWUSR);
	int serfs_solo_ID = shm_open("/xsedym02-serfssolo-shm", O_RDWR, S_IRUSR | S_IWUSR);
	int action_ID = shm_open("/xsedym02-action-shm", O_RDWR, S_IRUSR | S_IWUSR);
	if((hackers_ID < 0) ||
	   (hackers_solo_ID < 0) ||
	   (serfs_ID < 0) ||
	   (serfs_solo_ID < 0) ||
	   (action_ID < 0))
	{
		fprintf(stderr, "%s", ERROR_MSG[SHM_OPEN]);
		return FAILED;
	}

	shm_hackers = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, hackers_ID, 0);
	shm_hackers_solo = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, hackers_solo_ID, 0);
	shm_serfs = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, serfs_ID, 0);
	shm_serfs_solo = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, serfs_solo_ID, 0);
	shm_action = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, action_ID, 0);
	if((shm_hackers == MAP_FAILED) ||
	   (shm_hackers_solo == MAP_FAILED) ||
	   (shm_serfs == MAP_FAILED) ||
	   (shm_serfs_solo == MAP_FAILED) ||
	   (shm_action == MAP_FAILED))
	{
		fprintf(stderr, "%s", ERROR_MSG[MMAP]);
		return FAILED;
	}

	close(hackers_ID);
	close(hackers_solo_ID);
	close(serfs_ID);
	close(serfs_solo_ID);
	close(action_ID);

	return SUCCESS;
}

/**
 * Fce pro odpojení od paměťových zdrojů.
 */
void close_sources()
{
	// Odpojení od semaforů
	sem_close(sem_molo);
	sem_close(sem_hackers_queue);
	sem_close(sem_serfs_queue);
	sem_close(sem_captain);
	sem_close(sem_board);
	sem_close(sem_land);
	sem_close(sem_action);

	// Odpojení od sdílených proměnných
	munmap(shm_hackers, sizeof(int));
	munmap(shm_hackers_solo, sizeof(int));
	munmap(shm_serfs, sizeof(int));
	munmap(shm_serfs_solo, sizeof(int));
	munmap(shm_action, sizeof(int));
}

/**
 * Fce vykonává proces hacker.
 * Vstup - id - id procesu hacker
 *		 - cap - maximální kapacita mola
 *		 - sleep - maximální doba uspáni procesu, když je molo plné
 *		 - sail - maximální doba plavby
 * Výstup - Fce prací v případě úspěšného provedení SUCCESS (0), v případě chyby FAILED (1)
 */
int hacker_proces(int id, int cap, int sleep, int sail)
{
	// Připojení paměťových zdrojů a kontrola
	if(open_sources() != SUCCESS)
	{
		return FAILED;
	}

	srand(time(NULL) + getpid());
	bool is_captain = false;		// pro detekci kapitána

	// VYPIS - starts
	sem_wait(sem_action);
	fprintf(output, "%d\t: HACK %d\t: starts\n", *shm_action, id);
	*shm_action += 1;
	sem_post(sem_action);

	// Pokusí se vstoupit na polo
	sem_wait(sem_molo);
	while((*shm_hackers + *shm_serfs) >= cap)
	{
		// VYPIS - leaves queue
		sem_wait(sem_action);
		fprintf(output, "%d\t: HACK %d\t: leaves queue\t: %d\t: %d\n", *shm_action, id, *shm_hackers, *shm_serfs);
		*shm_action += 1;
		sem_post(sem_action);
		
		// Odemkne molo před uspáním
		sem_post(sem_molo);
		usleep(rand() % sleep * 1000);
		
		sem_wait(sem_molo);
		
		// VYPIS - is back
		sem_wait(sem_action);
		fprintf(output, "%d\t: HACK %d\t: is back\n", *shm_action, id);
		*shm_action += 1;
		sem_post(sem_action);
	}
	
	// Podařilo se mu vstoupit na molo
	*shm_hackers += 1;			// zvýší počet hackers na molu
	*shm_hackers_solo += 1;		// uvýší počet hackers bez skupinky na molu

	// VYPIS - wait
	sem_wait(sem_action);
	fprintf(output, "%d\t: HACK %d\t: waits\t\t: %d\t: %d\n", *shm_action, id, *shm_hackers, *shm_serfs);
	*shm_action += 1;
	sem_post(sem_action);

	// Snaží se vytvořit skupinku
	if(*shm_hackers_solo == 4)
	{
		// byla vytvoře skupinka (4H)
		*shm_hackers_solo -= 4;		

		// Uvolní polo a bude čekat na loď
		sem_post(sem_molo);
		sem_wait(sem_board);

		// Uzamkne molo
		// Proces se stává kapitánem a prpouští 3H procesy na loď
		sem_wait(sem_molo);
		sem_post(sem_hackers_queue);
		sem_post(sem_hackers_queue);
		sem_post(sem_hackers_queue);
		*shm_hackers -= 4;			// 4H nastoupili z mola na loď
		is_captain = true;			// proces se stává kapitánem (vždy ten, který příjde do sk. poslední)

		// VYPIS - boards
		sem_wait(sem_action);
		fprintf(output, "%d\t: HACK %d\t: boards\t: %d\t: %d\n", *shm_action, id, *shm_hackers, *shm_serfs);
		*shm_action += 1;
		sem_post(sem_action);
	}
	else if(*shm_hackers_solo == 2 && *shm_serfs_solo >= 2)
	{
		// byla vytvořena skupinka (2H + 2S)
		*shm_hackers_solo -= 2;		
		*shm_serfs_solo -= 2;

		// Uvolní molo a bude čekat na loď
		sem_post(sem_molo);
		sem_wait(sem_board);

		// Uzamkne molo
		// Proces se stává kapitánem a propouští 1H a 2S na loď
		sem_wait(sem_molo);
		sem_post(sem_hackers_queue);
		sem_post(sem_serfs_queue);
		sem_post(sem_serfs_queue);
		*shm_hackers -= 2;		// 2H nastoupí z mola na loď
		*shm_serfs -= 2;		// 2S nastoupí z mola na loď
		is_captain = true;		// proces se stává kapitánem

		// VYPIS - boards
		sem_wait(sem_action);
		fprintf(output, "%d\t: HACK %d\t: boards\t: %d\t: %d\n", *shm_action, id, *shm_hackers, *shm_serfs);
		*shm_action += 1;
		sem_post(sem_action);

	}

	// Odemkne molo
	sem_post(sem_molo);

	// Všichni čekají na vpuštění na loď, kromě kapitána, tem projde vždy
	if(!is_captain)
	{
		sem_wait(sem_hackers_queue);
	}

	// Probíhá plavha
	if(is_captain)
	{
		if(sail > 0)
		{
			usleep(rand() % sail * 1000);
		}
		sem_post(sem_land);
		sem_post(sem_land);
		sem_post(sem_land);
		sem_post(sem_land);
	}

	// Všichni čekají, až připlují zpět
	sem_wait(sem_land);

	// Vystupují pasažéři (kapitán NE)
	if(!is_captain)
	{
		// VYPIS - membex exits
		sem_wait(sem_molo);
		sem_wait(sem_action);
		fprintf(output, "%d\t: HACK %d\t: member exits\t: %d\t: %d\n", *shm_action, id, *shm_hackers, *shm_serfs);
		*shm_action += 1;
		sem_post(sem_action);
		sem_post(sem_molo);
	}

	// Všichni vystupují
	sem_post(sem_captain);

	// Vystupuje kapitán
	if(is_captain)
	{
		// Kapitán čeká, až půjde poslední
		sem_wait(sem_captain);
		sem_wait(sem_captain);
		sem_wait(sem_captain);
		sem_wait(sem_captain);

		// VYPIS - captain exits
		sem_wait(sem_molo);
		sem_wait(sem_action);
		fprintf(output, "%d\t: HACK %d\t: captain exits\t: %d\t: %d\n", *shm_action, id, *shm_hackers, *shm_serfs);
		*shm_action += 1;
		sem_post(sem_action);
		sem_post(sem_molo);

		// Kapitán odemkne loď pro další plavbu
		sem_post(sem_board);
	}

	// Odpojení od paměťových zdrojů
	close_sources();

	return SUCCESS;
}

/**
 * Fce vykonává procesu serf.
 * Vstup - id - id procesu serf
 *		 - cap - maximální kapacita mola
 *		 - sleep - maximální doba uspáni procesu, když je molo plné
 *		 - sail - maximální doba plavby
 * Výstup - Fce vrací v případě úspěšného provedení SUCCESS (0), v případě chyby FAILED (1)
 */
int serf_proces(int id, int cap, int sleep, int sail)
{
	// Připojení paměťových zdrojů a kontrola
	if(open_sources() != SUCCESS)
	{
		return FAILED;
	}

	srand(time(NULL) + getpid());
	bool is_captain = false;		// pro detekci kapitána

	// VYPIS - starts
	sem_wait(sem_action);
	fprintf(output, "%d\t: SERF %d\t: starts\n", *shm_action, id);
	*shm_action += 1;
	sem_post(sem_action);

	// Pokusí se vstoupit na polo
	sem_wait(sem_molo);
	while((*shm_hackers + *shm_serfs) >= cap)
	{
		// VYPIS - leaves queue
		sem_wait(sem_action);
		fprintf(output, "%d\t: SERF %d\t: leaves queue\t: %d\t: %d\n", *shm_action, id, *shm_hackers, *shm_serfs);
		*shm_action += 1;
		sem_post(sem_action);
		
		// Odemkne molo před uspáním
		sem_post(sem_molo);
		usleep(rand() % sleep * 1000);
		
		sem_wait(sem_molo);
		
		// VYPIS - is back
		sem_wait(sem_action);
		fprintf(output, "%d\t: SERF %d\t: is back\n", *shm_action, id);
		*shm_action += 1;
		sem_post(sem_action);
	}
	
	// Podařilo se mu vstoupit na molo
	*shm_serfs += 1;			// zvýší počet serfs na molu
	*shm_serfs_solo += 1;		// uvýší počet serfs bez skupinky na molu

	// VYPIS - wait
	sem_wait(sem_action);
	fprintf(output, "%d\t: SERF %d\t: waits\t\t: %d\t: %d\n", *shm_action, id, *shm_hackers, *shm_serfs);
	*shm_action += 1;
	sem_post(sem_action);

	// Snaží se vytvořit skupinku
	if(*shm_serfs_solo == 4)
	{
		// byla vytvoře skupinka (4S)
		*shm_serfs_solo -= 4;		

		// Uvolní polo a bude čekat na loď
		sem_post(sem_molo);
		sem_wait(sem_board);

		// Uzamkne molo
		// Proces se stává kapitánem a prpouští 3S procesy na loď
		sem_wait(sem_molo);
		sem_post(sem_serfs_queue);
		sem_post(sem_serfs_queue);
		sem_post(sem_serfs_queue);
		*shm_serfs -= 4;			// 4S nastoupili z mola na loď
		is_captain = true;			// proces se stává kapitánem (vždy ten, který příjde do sk. poslední)

		// VYPIS - boards
		sem_wait(sem_action);
		fprintf(output, "%d\t: SERF %d\t: boards\t: %d\t: %d\n", *shm_action, id, *shm_hackers, *shm_serfs);
		*shm_action += 1;
		sem_post(sem_action);
	}
	else if(*shm_serfs_solo == 2 && *shm_hackers_solo >= 2)
	{
		// byla vytvořena skupinka (2H + 2S)
		*shm_serfs_solo -= 2;
		*shm_hackers_solo -= 2;		

		// Uvolní molo a bude čekat na loď
		sem_post(sem_molo);
		sem_wait(sem_board);

		// Uzamkne molo
		// Proces se stává kapitánem a propouští 1S a 2H na loď
		sem_wait(sem_molo);
		sem_post(sem_hackers_queue);
		sem_post(sem_hackers_queue);
		sem_post(sem_serfs_queue);
		*shm_serfs -= 2;		// 2S nastoupí z mola na loď
		*shm_hackers -= 2;		// 2H nastoupí z mola na loď
		is_captain = true;		// proces se stává kapitánem

		// VYPIS - boards
		sem_wait(sem_action);
		fprintf(output, "%d\t: SERF %d\t: boards\t: %d\t: %d\n", *shm_action, id, *shm_hackers, *shm_serfs);
		*shm_action += 1;
		sem_post(sem_action);

	}

	// Odemkne molo
	sem_post(sem_molo);

	// Všichni čekají na vpuštění na loď, kromě kapitána, tem projde vždy
	if(!is_captain)
	{
		sem_wait(sem_serfs_queue);
	}
	// Probíhá plavba
	if(is_captain)
	{
		if(sail > 0)
		{
			usleep(rand() % sail * 1000);
		}
		sem_post(sem_land);
		sem_post(sem_land);
		sem_post(sem_land);
		sem_post(sem_land);
	}

	// Všichni čekají, až připlují zpět
	sem_wait(sem_land);

	// Vystupují pasažéři (kapitán NE)
	if(!is_captain)
	{
		// VYPIS - membex exits
		sem_wait(sem_molo);
		sem_wait(sem_action);
		fprintf(output, "%d\t: SERF %d\t: member exits\t: %d\t: %d\n", *shm_action, id, *shm_hackers, *shm_serfs);
		*shm_action += 1;
		sem_post(sem_action);
		sem_post(sem_molo);
	}

	// Všichni vystupují
	sem_post(sem_captain);

	// Vystupuje kapitán
	if(is_captain)
	{
		// Kapitán čeká, až půjde poslední
		sem_wait(sem_captain);
		sem_wait(sem_captain);
		sem_wait(sem_captain);
		sem_wait(sem_captain);

		// VYPIS - captain exits
		sem_wait(sem_molo);
		sem_wait(sem_action);
		fprintf(output, "%d\t: SERF %d\t: captain exits\t: %d\t: %d\n", *shm_action, id, *shm_hackers, *shm_serfs);
		*shm_action += 1;
		sem_post(sem_action);
		sem_post(sem_molo);

		// Kapitán odemkne loď pro další plavbu
		sem_post(sem_board);
	}

	// Odpojení od paměťových zdrojů
	close_sources();

	return SUCCESS;
}
