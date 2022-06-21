/*
 * Soubor programu pro MCU K60 na platformě FitKit3.
 * Projekt: IMP - Světelná tabule
 * Autor: Michal Šedý
 * Poslední změna: 5.11.2020
 */

#include "MK60D10.h"    // Definice pro dané MCU
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Makra pro registrové manipulace na úrovní bitů */
#define GPIO_PIN_MASK 0x1Fu
#define GPIO_PIN(x) (((1)<<(x & GPIO_PIN_MASK)))

// Kontrolní LED
#define SHIFT_LED  0x20    // Port B, bit 5 LED9
#define LED_D10  0x10      // Port B, bit 4
#define LED_D11  0x8       // Port B, bit 3
#define INPUT_LED 0x4      // Port B, bit 2 LED12

// Tlačítka pro interakci
#define BTN_SW2 0x400     // Port E, bit 10 PLAY/PAUSE
#define BTN_SW3 0x1000    // Port E, bit 12 DOWN
#define BTN_SW4 0x8000000 // Port E, bit 27 NEXT TEXT
#define BTN_SW5 0x4000000 // Port E, bit 26 UP
#define BTN_SW6 0x800     // Port E, bit 11 EXTERN TEXT

// Sloupce ve tvaru DCBA, kde A = COLBIT_1 až D = COL_BIT_4
// Binární kombinace ovládají sloupce C0 až C15
// Vždy je přístupný pouze jeden sloupec.
#define COL_BIT_1  0x100   // Port A, bit 8
#define COL_BIT_2  0x400   // Port A, bit 10
#define COL_BIT_3  0x40	   // Port A, bit 6
#define COL_BIT_4  0x800   // Port A, bit 11
#define ALL_COLUMNS 0xd40

// Řádky R0 až R7.
// Může být spuštěno více diod v řádku.
#define ROW_0  0x4000000   // Port A, bit 26
#define ROW_1  0x1000000   // Port A, bit 24
#define ROW_2  0x200       // Port A, bit 9
#define ROW_3  0x2000000   // Port A, bit 25
#define ROW_4  0x10000000  // Port A, bit 28
#define ROW_5  0x80        // Port A, bit 7
#define ROW_6  0x8000000   // Port A, bit 27
#define ROW_7  0x20000000  // Port A, bit 29
#define ALL_ROWS  0x3f000280

// Makro pro získání x-tého bitu (číslováno on 1)
#define GET_BIT(pos, num) ((num >> (pos - 1)) & 0x1)

// Makro pro odřádkování pomocí UART
#define NEW_LINE {\
					send_ch('\r');\
					send_ch('\n');\
			     }

// Konstatnty
#define RESIZE_BUDGET 10
#define MAX_TEST_TEXTS 4

// Globální proměnné
int idx_in_string = 0;			// Index v zobrazovaném řetězci ukazující na začátek displeje
int string_len = 0;				// Délka zobrazovaného řetězce
int stop_flag = 0;				// Příznak zastavení zobrazovaného textu
unsigned int compare = 0xa0;	// Porovnávaná hodnota pro LPTMR

// Předběžné deklarace pomocných funkci
void delay(long long bound);
void fatal_error();
char *resize(char *str, int len);
void send_ch(char chr);
void send_str(char* s);
char *receive_str();
void cpy_bits_to_char_arr(char *dst, const unsigned short count, ...);
unsigned diod_text_add_char(char **dst, const char *src, const unsigned dst_len, const unsigned src_len);
unsigned ascii_to_diod_text(char **diod_txt, char const *str);
void refresh_display(char string_to_display[]);


/*
 * Zpožďovaní funkce
 */
void delay(long long bound)
{
  long long i;
  for(i=0;i<bound;i++);
}

/*
 * Funkce pro případ selhání protramu (chyba mallocu).
 * Vypíše chybovou hlášku na terminál pomocí UART a rozbliká diody D9-D12.
 * Řízení programu zůstane uvězněno v této funkci.
 */
void fatal_error()
{
	// Zastavení zobrazovaného textu na displeji.
	stop_flag = 1;

	send_str("ERROR: Selhání funkce malloc. Nelze pokračovat v programu.");
	NEW_LINE;

	// Rozsvícení D9-D12
	PTB->PCOR = SHIFT_LED;
	PTB->PCOR = LED_D10;
	PTB->PCOR = LED_D11;
	PTB->PCOR = INPUT_LED;

	while(1)
	{
		delay(2000000);
		// Invertování D9-D12 (blikání)
		PTB->PTOR = SHIFT_LED;
		PTB->PTOR = LED_D10;
		PTB->PTOR = LED_D11;
		PTB->PTOR = INPUT_LED;
	}
}


/*
 * Funkce rozšířující pole charů o RESIZE_BUDGET.
 * ----------------------------------------------
 * str - stávající rozšiřovaný prostor
 * len - aktuální délka prostoru
 *
 * Funkce vrací ukazatel na nový zvětšený prostor.
 * V případě selhání vrátí NULL.
 */
char *resize(char *str, int len)
{
	char *to_ret = NULL;
	// Pokud str nic neobsahuje vytvoří se
	if(str == NULL)
	{
		to_ret = (char*)malloc(RESIZE_BUDGET);
	}
	else
	{
		to_ret = (char*)realloc(str, len + RESIZE_BUDGET);
	}

	return to_ret;
}


/*
 * Funkce odešle pomocí UART znak.
 * -------------------------------
 * chr - znak k odeslání
 */
void send_ch(char chr)
{
    while(!(UART5->S1 & UART_S1_TDRE_MASK) && !(UART5->S1 & UART_S1_TC_MASK))
    {}
    UART5->D = chr;
}

/*
 * Funkce odešle celý řetězec pomocí UART.
 */
void send_str(char* s)
{
	unsigned i;
	for(i = 0; s[i] != '\0'; i++)
	{
		send_ch(s[i]);
	}
}

/*
 * Funkce pomocí UART získá znak.
 */
unsigned char receive_ch()
{
    while(!(UART5->S1 & UART_S1_RDRF_MASK))
    {}
    return UART5->D;
}

/*
 * Funkce pomocí UART získá celý řetezec až po ENTER.
 * Jednotlivé získané znaky jsou opakovány a odesílány zpět (na terminal).
 */
char *receive_str()
{
	// Pomocné proměnné pro dynamický řetězec, do kterého je ukládán vstup.
	short free_char = 0;
	unsigned idx = 0;
	char *string = NULL;
	char chr;

	do {
		chr = receive_ch();
		// Kontrola, zda není potřeba cílový řetězec zvětšit.
		if(free_char == 0)
		{
			string = resize(string, idx); // idx vždy ukazuje na novou pozici, což je strlen
			if(string == NULL)
			{
				fatal_error();
			}
			free_char += RESIZE_BUDGET;
		}

		// Pokud je konec, bude se zapisovat '\0' pro ukončení řetězce
		// Terminálu se ale vrátí odřádkování.
		if(chr == '\r' || chr == '\n')
		{
			chr = '\0';
			NEW_LINE;
		}
		else
		{
			// Pokud ještě není vstup ukončen, vrací se stejný znak, jaký byl získán.
			send_ch(chr);
		}

		// Zápis získaného znaku a úprava čítačů.
		string[idx] = chr;
		idx++;
		free_char--;
	} while(chr != '\0');

	return string;
}

/*
 * Funkce pro nakopírování libovolného počtu znaků do řetězce dst.
 * --------------------------------------------------------------
 * dst - cílový řetězec
 * count - počet znaků předávaných funkci
 * ...   - proměnný počet předávaných znaků
 */
void cpy_bits_to_char_arr(char *dst, const unsigned short count, ...)
{
	va_list ap;
	va_start(ap, count);
	unsigned short i;
	for (i = 0; i < count; i++)
	{
		dst[i] = (char) va_arg(ap, int);
	}
	va_end(ap);
}


/*
 * Funce pro přidání již kódovaných písmen do zobrazovaného řetězce.
 * -----------------------------------------------------------------
 * dst - ukazatel na zobrazovaný řetězec
 * src - překódovaný znak
 * dst_len - délka již exitujícího zobrazovaného řetězce
 * src_len - délka kódovaného znaku
 *
 * Funkce vrací délku (počet sloupců) zobrazovaného řetězce.
 */
unsigned diod_text_add_char(char **dst, const char *src, const unsigned dst_len, const unsigned src_len)
{
	// Dst je potřeba zvětšit na velikost dst a src + 1 (mezi písmeny je jeden sloupec volný)
	char *new_str = (char *) realloc(*dst, dst_len + src_len + 1);
	if(new_str == NULL)
	{
		fatal_error();
	}

	// Zkopírování kódovaného znaku na první volnou pozici
	if(memcpy(&(new_str[dst_len]), src, src_len) == NULL)
	{
		fatal_error();
	}

	// Mezi jednotlivými písmeny bude mezera jednoho sloupce.
	new_str[dst_len+src_len] = 0b00000000;

	// Do dst uložíme nový zobrazovaný řetězec.
	*dst = new_str;

	// Délka zobrazovaného řetězce.
	return dst_len + src_len + 1;
}


/*
 * Halvní funkce pro kódování ASCII textu pro zobrazování na maticovém displeji.
 * -----------------------------------------------------------------------------
 * diod_txt - ukazatel na výstupní podobu (kódovanou)
 * src - vstupní ASCII řetězec
 *
 * Funkce vrací celkovou délku (počet sloupců) zobrazovaného (kódovaného) textu.
 */
unsigned ascii_to_diod_text(char **diod_txt, char const *str)
{
	// Inicializace proměnných
	*diod_txt = NULL;
	unsigned total_text_size = 0;
	char selected_char[5] = {0,};	// buffer pro jednolivé kódování znaků
	unsigned short char_size = 0;

	// Před zobrazovaný text přidáme mezeru 16 sloupců (prázdný displej).
	// Přidávají se pokaždé 3 prázdné sloupce, ne 4, protože za každým vložením
	// následuje automaticky prázdný sloupec, který funguje jako +1.
	memset(selected_char, 0, 5);
	cpy_bits_to_char_arr(selected_char, 3, 0b00000000, 0b00000000, 0b00000000);
	total_text_size =  diod_text_add_char(diod_txt, selected_char, total_text_size, 3);
	total_text_size =  diod_text_add_char(diod_txt, selected_char, total_text_size, 3);
	total_text_size =  diod_text_add_char(diod_txt, selected_char, total_text_size, 3);
	total_text_size =  diod_text_add_char(diod_txt, selected_char, total_text_size, 3);

	// Kódování jednotlivých vstupních znaků
	unsigned i;
	for(i = 0; str[i] != '\0'; i++)
	{
		// Reset bufferu pro kódování jednotlivých znaků.
		// Jednotlivé kódování znaků je převzato z:
		// https://howtomechatronics.com/tutorials/arduino/8x8-led-matrix-max7219-tutorial-scrolling-text-android-control-via-bluetooth/
		memset(selected_char, 0, 5);
		switch (str[i])
		{
			case ' ':
				cpy_bits_to_char_arr(selected_char, 3, 0b00000000, 0b00000000, 0b00000000);
				char_size = 3;
				break;

			case '!':
				cpy_bits_to_char_arr(selected_char, 1, 0b01011111);
				char_size = 1;
				break;

			case '"':
				cpy_bits_to_char_arr(selected_char, 3, 0b00000011, 0b00000000, 0b00000011);
				char_size = 3;
				break;

			case '#':
				cpy_bits_to_char_arr(selected_char, 5, 0b00010100, 0b00111110, 0b00010100, 0b00111110, 0b00010100);
				char_size = 5;
				break;

			case '$':
				cpy_bits_to_char_arr(selected_char, 4, 0b00100100, 0b01101010, 0b00101011, 0b00010010);
				char_size = 4;
				break;

			case '%':
				cpy_bits_to_char_arr(selected_char, 5, 0b01100011, 0b00010011, 0b00001000, 0b01100100, 0b01100011);
				char_size = 5;
				break;

			case '&':
				cpy_bits_to_char_arr(selected_char, 5, 0b00110110, 0b01001001, 0b01010110, 0b00100000, 0b01010000);
				char_size = 5;
				break;

			case '\'':
				cpy_bits_to_char_arr(selected_char, 1, 0b00000011);
				char_size = 1;
				break;

			case ')':
				cpy_bits_to_char_arr(selected_char, 3, 0b00011100, 0b00100010, 0b01000001);
				char_size = 3;
				break;

			case '(':
				cpy_bits_to_char_arr(selected_char, 3, 0b01000001, 0b00100010, 0b00011100);
				char_size = 3;
				break;

			case '*':
				cpy_bits_to_char_arr(selected_char, 5, 0b00101000, 0b00011000, 0b00001110, 0b00011000, 0b00101000);
				char_size = 5;
				break;

			case '+':
				cpy_bits_to_char_arr(selected_char, 5, 0b00001000, 0b00001000, 0b00111110, 0b00001000, 0b00001000);
				char_size = 5;
				break;

			case ',':
				cpy_bits_to_char_arr(selected_char, 2, 0b10110000, 0b01110000);
				char_size = 2;
				break;

			case '-':
				cpy_bits_to_char_arr(selected_char, 4, 0b00001000, 0b00001000, 0b00001000, 0b00001000);
				char_size = 4;
				break;

			case '.':
				cpy_bits_to_char_arr(selected_char, 2, 0b01100000, 0b01100000);
				char_size = 2;
				break;

			case '/':
				cpy_bits_to_char_arr(selected_char, 4, 0b01100000, 0b00011000, 0b00000110, 0b00000001);
				char_size = 4;
				break;

			case '0':
				cpy_bits_to_char_arr(selected_char, 4, 0b00111110, 0b01000001, 0b01000001, 0b00111110);
				char_size = 4;
				break;

			case '1':
				cpy_bits_to_char_arr(selected_char, 3, 0b01000010, 0b01111111, 0b01000000);
				char_size = 3;
				break;

			case '2':
				cpy_bits_to_char_arr(selected_char, 4, 0b01100010, 0b01010001, 0b01001001, 0b01000110);
				char_size = 4;
				break;

			case '3':
				cpy_bits_to_char_arr(selected_char, 4, 0b00100010, 0b01000001, 0b01001001, 0b00110110);
				char_size = 4;
				break;

			case '4':
				cpy_bits_to_char_arr(selected_char, 4, 0b00011000, 0b00010100, 0b00010010, 0b01111111);
				char_size = 4;
				break;

			case '5':
				cpy_bits_to_char_arr(selected_char, 4, 0b00100111, 0b01000101, 0b01000101, 0b00111001);
				char_size = 4;
				break;

			case '6':
				cpy_bits_to_char_arr(selected_char, 4, 0b00111110, 0b01001001, 0b01001001, 0b00110000);
				char_size = 4;
				break;

			case '7':
				cpy_bits_to_char_arr(selected_char, 4, 0b01100001, 0b00010001, 0b00001001, 0b00000111);
				char_size = 4;
				break;

			case '8':
				cpy_bits_to_char_arr(selected_char, 4, 0b00110110, 0b01001001, 0b01001001, 0b00110110);
				char_size = 4;
				break;

			case '9':
				cpy_bits_to_char_arr(selected_char, 4, 0b00000110, 0b01001001, 0b01001001, 0b00111110);
				char_size = 4;
				break;

			case ':':
				cpy_bits_to_char_arr(selected_char, 2, 0b01101100, 0b01101100);
				char_size = 2;
				break;

			case ';':
				cpy_bits_to_char_arr(selected_char, 2, 0b10000000, 0b01010000);
				char_size = 2;
				break;

			case '<':
				cpy_bits_to_char_arr(selected_char, 3, 0b00010000, 0b00101000, 0b01000100);
				char_size = 3;
				break;

			case '=':
				cpy_bits_to_char_arr(selected_char, 3, 0b00010100, 0b00010100, 0b00010100);
				char_size = 3;
				break;

			case '>':
				cpy_bits_to_char_arr(selected_char, 3, 0b01000100, 0b00101000, 0b00010000);
				char_size = 3;
				break;

			case '?':
				cpy_bits_to_char_arr(selected_char, 4, 0b00000010, 0b01011001, 0b00001001, 0b00000110);
				char_size = 4;
				break;

			case '@':
				cpy_bits_to_char_arr(selected_char, 5, 0b00111110, 0b01001001, 0b01010101, 0b01011101, 0b00001110);
				char_size = 5;
				break;

			case 'A':
				cpy_bits_to_char_arr(selected_char, 4, 0b01111110, 0b00010001, 0b00010001, 0b01111110);
				char_size = 4;
				break;

			case 'B':
				cpy_bits_to_char_arr(selected_char, 4, 0b01111111, 0b01001001, 0b01001001, 0b00110110);
				char_size = 4;
				break;

			case 'C':
				cpy_bits_to_char_arr(selected_char, 4, 0b00111110, 0b01000001, 0b01000001, 0b00100010);
				char_size = 4;
				break;

			case 'D':
				cpy_bits_to_char_arr(selected_char, 4, 0b01111111, 0b01000001, 0b01000001, 0b00111110);
				char_size = 4;
				break;

			case 'E':
				cpy_bits_to_char_arr(selected_char, 4, 0b01111111, 0b01001001, 0b01001001, 0b01000001);
				char_size = 4;
				break;

			case 'F':
				cpy_bits_to_char_arr(selected_char, 4, 0b01111111, 0b00001001, 0b00001001, 0b00000001);
				char_size = 4;
				break;

			case 'G':
				cpy_bits_to_char_arr(selected_char, 4, 0b00111110, 0b01000001, 0b01001001, 0b01111010);
				char_size = 4;
				break;

			case 'H':
				cpy_bits_to_char_arr(selected_char, 4, 0b01111111, 0b00001000, 0b00001000, 0b01111111);
				char_size = 4;
				break;

			case 'I':
				cpy_bits_to_char_arr(selected_char, 3, 0b01000001, 0b01111111, 0b01000001);
				char_size = 3;
				break;

			case 'J':
				cpy_bits_to_char_arr(selected_char, 4, 0b00110000, 0b01000000, 0b01000001, 0b00111111);
				char_size = 4;
				break;

			case 'K':
				cpy_bits_to_char_arr(selected_char, 4, 0b01111111, 0b00001000, 0b00010100, 0b01100011);
				char_size = 4;
				break;

			case 'L':
				cpy_bits_to_char_arr(selected_char, 4, 0b01111111, 0b01000000, 0b01000000, 0b01000000);
				char_size = 4;
				break;

			case 'M':
				cpy_bits_to_char_arr(selected_char, 5, 0b01111111, 0b00000010, 0b00001100, 0b00000010, 0b01111111);
				char_size = 5;
				break;

			case 'N':
				cpy_bits_to_char_arr(selected_char, 5, 0b01111111, 0b00000100, 0b00001000, 0b00010000, 0b01111111);
				char_size = 5;
				break;

			case 'O':
				cpy_bits_to_char_arr(selected_char, 4, 0b00111110, 0b01000001, 0b01000001, 0b00111110);
				char_size = 4;
				break;

			case 'P':
				cpy_bits_to_char_arr(selected_char, 4, 0b01111111, 0b00001001, 0b00001001, 0b00000110);
				char_size = 4;
				break;

			case 'Q':
				cpy_bits_to_char_arr(selected_char, 4, 0b00111110, 0b01000001, 0b01000001, 0b10111110);
				char_size = 4;
				break;

			case 'R':
				cpy_bits_to_char_arr(selected_char, 4, 0b01111111, 0b00001001, 0b00001001, 0b01110110);
				char_size = 4;
				break;

			case 'S':
				cpy_bits_to_char_arr(selected_char, 4, 0b01000110, 0b01001001, 0b01001001, 0b00110010);
				char_size = 4;
				break;

			case 'T':
				cpy_bits_to_char_arr(selected_char, 5, 0b00000001, 0b00000001, 0b01111111, 0b00000001, 0b00000001);
				char_size = 5;
				break;

			case 'U':
				cpy_bits_to_char_arr(selected_char, 4, 0b00111111, 0b01000000, 0b01000000, 0b00111111);
				char_size = 4;
				break;

			case 'V':
				cpy_bits_to_char_arr(selected_char, 5, 0b00001111, 0b00110000, 0b01000000, 0b00110000, 0b00001111);
				char_size = 5;
				break;

			case 'W':
				cpy_bits_to_char_arr(selected_char, 5, 0b00111111, 0b01000000, 0b00111000, 0b01000000, 0b00111111);
				char_size = 5;
				break;

			case 'X':
				cpy_bits_to_char_arr(selected_char, 5, 0b01100011, 0b00010100, 0b00001000, 0b00010100, 0b01100011);
				char_size = 5;
				break;

			case 'Y':
				cpy_bits_to_char_arr(selected_char, 5, 0b00000111, 0b00001000, 0b01110000, 0b00001000, 0b00000111);
				char_size = 5;
				break;

			case 'Z':
				cpy_bits_to_char_arr(selected_char, 4, 0b01100001, 0b01010001, 0b01001001, 0b01000111);
				char_size = 4;
				break;

			case '[':
				cpy_bits_to_char_arr(selected_char, 2, 0b01111111, 0b01000001);
				char_size = 2;
				break;

			case '\\':
				cpy_bits_to_char_arr(selected_char, 4, 0b00000001, 0b00000110, 0b00011000, 0b01100000);
				char_size = 4;
				break;

			case ']':
				cpy_bits_to_char_arr(selected_char, 2, 0b01000001, 0b01111111);
				char_size = 2;
				break;

			case '_':
				cpy_bits_to_char_arr(selected_char, 4, 0b01000000, 0b01000000, 0b01000000, 0b01000000);
				char_size = 4;
				break;

			case '`':
				cpy_bits_to_char_arr(selected_char, 2, 0b00000001, 0b00000010);
				char_size = 2;
				break;

			case 'a':
				cpy_bits_to_char_arr(selected_char, 4, 0b00100000, 0b01010100, 0b01010100, 0b01111000);
				char_size = 4;
				break;

			case 'b':
				cpy_bits_to_char_arr(selected_char, 4, 0b01111111, 0b01000100, 0b01000100, 0b00111000);
				char_size = 4;
				break;

			case 'c':
				cpy_bits_to_char_arr(selected_char, 4, 0b00111000, 0b01000100, 0b01000100, 0b00101000);
				char_size = 4;
				break;

			case 'd':
				cpy_bits_to_char_arr(selected_char, 4, 0b00111000, 0b01000100, 0b01000100, 0b01111111);
				char_size = 4;
				break;

			case 'e':
				cpy_bits_to_char_arr(selected_char, 4, 0b00111000, 0b01010100, 0b01010100, 0b00011000);
				char_size = 4;
				break;

			case 'f':
				cpy_bits_to_char_arr(selected_char, 3, 0b00000100, 0b01111110, 0b00000101);
				char_size = 3;
				break;

			case 'g':
				cpy_bits_to_char_arr(selected_char, 4, 0b10011000, 0b10100100, 0b10100100, 0b01111000);
				char_size = 4;
				break;

			case 'h':
				cpy_bits_to_char_arr(selected_char, 4, 0b01111111, 0b00000100, 0b00000100, 0b01111000);
				char_size = 4;
				break;

			case 'i':
				cpy_bits_to_char_arr(selected_char, 3, 0b01000100, 0b01111101, 0b01000000);
				char_size = 3;
				break;

			case 'j':
				cpy_bits_to_char_arr(selected_char, 4, 0b01000000, 0b10000000, 0b10000100, 0b01111101);
				char_size = 4;
				break;

			case 'k':
				cpy_bits_to_char_arr(selected_char, 4, 0b01111111, 0b00010000, 0b00101000, 0b01000100);
				char_size = 4;
				break;

			case 'l':
				cpy_bits_to_char_arr(selected_char, 3, 0b01000001, 0b01111111, 0b01000000);
				char_size = 3;
				break;

			case 'm':
				cpy_bits_to_char_arr(selected_char, 5, 0b01111100, 0b00000100, 0b01111100, 0b00000100, 0b01111000);
				char_size = 5;
				break;

			case 'n':
				cpy_bits_to_char_arr(selected_char, 4, 0b01111100, 0b00000100, 0b00000100, 0b01111000);
				char_size = 4;
				break;

			case 'o':
				cpy_bits_to_char_arr(selected_char, 4, 0b00111000, 0b01000100, 0b01000100, 0b00111000);
				char_size = 4;
				break;

			case 'p':
				cpy_bits_to_char_arr(selected_char, 4, 0b11111100, 0b00100100, 0b00100100, 0b00011000);
				char_size = 4;
				break;

			case 'q':
				cpy_bits_to_char_arr(selected_char, 4, 0b00011000, 0b00100100, 0b00100100, 0b11111100);
				char_size = 4;
				break;

			case 'r':
				cpy_bits_to_char_arr(selected_char, 4, 0b01111100, 0b00001000, 0b00000100, 0b00000100);
				char_size = 4;
				break;

			case 's':
				cpy_bits_to_char_arr(selected_char, 4, 0b01001000, 0b01010100, 0b01010100, 0b00100100);
				char_size = 4;
				break;

			case 't':
				cpy_bits_to_char_arr(selected_char, 3, 0b00000100, 0b00111111, 0b01000100);
				char_size = 3;
				break;

			case 'u':
				cpy_bits_to_char_arr(selected_char, 4, 0b00111100, 0b01000000, 0b01000000, 0b01111100);
				char_size = 4;
				break;

			case 'v':
				cpy_bits_to_char_arr(selected_char, 5, 0b00011100, 0b00100000, 0b01000000, 0b00100000, 0b00011100);
				char_size = 5;
				break;

			case 'w':
				cpy_bits_to_char_arr(selected_char, 5, 0b00111100, 0b01000000, 0b00111100, 0b01000000, 0b00111100);
				char_size = 5;
				break;

			case 'x':
				cpy_bits_to_char_arr(selected_char, 5, 0b01000100, 0b00101000, 0b00010000, 0b00101000, 0b01000100);
				char_size = 5;
				break;

			case 'y':
				cpy_bits_to_char_arr(selected_char, 4, 0b10011100, 0b10100000, 0b10100000, 0b01111100);
				char_size = 4;
				break;

			case 'z':
				cpy_bits_to_char_arr(selected_char, 3, 0b01100100, 0b01010100, 0b01001100);
				char_size = 3;
				break;

			case '{':
				cpy_bits_to_char_arr(selected_char, 3, 0b00001000, 0b00110110, 0b01000001);
				char_size = 3;
				break;

			case '|':
				cpy_bits_to_char_arr(selected_char, 1, 0b01111111);
				char_size = 1;
				break;

			default:
				cpy_bits_to_char_arr(selected_char, 8, 0b00011000, 0b00111100, 0b01111010, 0b11111101, 0b10101101, 0b01110010, 0b00111100, 0b00011000);
				char_size = 8;

		}

		// Zápis získaného kódování do výstupního diodového textu.
		// Automaticky se přidá jeden volný sloupec.
		total_text_size =  diod_text_add_char(diod_txt, selected_char, total_text_size, char_size);
	}

	// Velikost (počet sloupců) diodového textu.
	return total_text_size;
}

/*
 * Funkce pro aktualizaci displeje (přepnutí svíticího řádku).
 * Přepínání je realizováno cyklicky pomocí statické prměnné column_on.
 */
void refresh_display(char string_to_display[])
{
	// Vypnutí displeje
	PTE->PDOR |= GPIO_PDOR_PDO(GPIO_PIN(28));

	static uint32_t column_on= 0x0;  // Aktuální index aktivního sloupce.
	char row_power;  				 // Kombinace aktivních diod ve sloupci.

	// Cyklický "4 bitový" čítač indexu aktivního sloupce.
	if (column_on > 0xf)
	{
		column_on = 0x0;
	}

	// Vypnutí všech sloupců.
	PTA->PCOR = ALL_COLUMNS;
	// Zapnutí aktivního sloupce na základě binární hodnoty čítače column_on.
	// např. column_on = 0110 --> zapne se 7. sloupec
	// (GET_BIT(1, column_on) * COL_BIT_1) --> získá první bit (LSB) z column_on
	//     a podle jeho hodnoty pomocí OR vloží, nebo nevloží COL_BIT1 do PTA->PDOR. Pokud bude
	//     získaná 0, pak bude provedeno 0 | PTA->PDOR), což nic nezmění a sloupec
	//     zůstane vypnut. V případě, že bude získaná 1, bude násobením ponechána
	//     hodnota COL_BIT_1, a ta se pomocí OR zapíše do PTA->PDOR.
	PTA->PDOR |= (GET_BIT(1, column_on) * COL_BIT_1) | (GET_BIT(2, column_on) * COL_BIT_2) |
			 	 (GET_BIT(3, column_on) * COL_BIT_3) | (GET_BIT(4, column_on) * COL_BIT_4);


	// Na základě indexu v zobrazovaném textu ukazujího na začátek maticového displeje,
	// indexu aktivního sloupce a jeho maximální délky určí platnou bitovou sekvenci
	// pro aktivaci diod ve sloupci. V případě, že se text ještě neopakuje, je získana
	// kompinace pouze podle indexu jakožto součet indexu sloupce a počátku displeje.
	if(idx_in_string + column_on < string_len)
	{
		row_power = string_to_display[idx_in_string + column_on];
	}
	else
	{
		// Na zakladě údajů o aktivním sloupci, indexu počátku displeje a maximální délky
		// řetězce určí nový index bitové kombinace v opakujícím se řetězci.
		row_power = string_to_display[idx_in_string + column_on - string_len];
	}

	// Vypne diody ve sloupci, tedy všechny řádky.
	PTA->PCOR = ALL_ROWS;
	// Zapnutí správných diod v aktivním sloupci na základě binární kompinace v row_power.
	// např. column_on = 0110 0110 --> zapne R1, R2, R5 a R6.
	// (GET_BIT(1, row_power) * ROW_0) --> získá první bit (LSB) z row_power
	//     a podle jeho hodnoty pomocí OR vloží, nebo ne ROW_0 do PTA->PDOR. Pokud bude
	//     získaná 0, pak bude provedeno 0 | PTA->PDOR, což nic nezmění a sloupec
	//     zůstane vypnut. V případě, že bude získaná 1, bude násobením ponechána
	//     hodnota ROW_0, a ta se pomocí OR zapíše do PTA->PDOR.
	PTA->PDOR |= (GET_BIT(1, row_power) * ROW_0) | (GET_BIT(2, row_power) * ROW_1) | (GET_BIT(3, row_power) * ROW_2) |
				  (GET_BIT(4, row_power) * ROW_3) | (GET_BIT(5, row_power) * ROW_4) | (GET_BIT(6, row_power) * ROW_5) |
				  (GET_BIT(7, row_power) * ROW_6) | (GET_BIT(8, row_power) * ROW_7);

	// Navýšení čítače aktivního sloupce
	column_on++;

	// Zapnutí displeje
	PTE->PCOR = GPIO_PDDR_PDD(GPIO_PIN(28));
	delay(2000);
}


/*
 * Inicializace MCU - základní nastavení hodin, vypnutí watchdog.
 */
void MCUInit(void)
{
    MCG_C4 |= ( MCG_C4_DMX32_MASK | MCG_C4_DRST_DRS(0x01) );
    SIM_CLKDIV1 |= SIM_CLKDIV1_OUTDIV1(0x00);
    WDOG_STCTRLH &= ~WDOG_STCTRLH_WDOGEN_MASK;
}


/*
 * Inicializace UART
 */
void UART5Init()
{
    UART5->C2  &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK);
    UART5->BDH =  0x00;
    UART5->BDL =  0x1A; // Baud rate 115 200 Bd, 1 stop bit
    UART5->C4  =  0x0F; // Oversampling ratio 16, match address vypnuto
    UART5->C1  =  0x00; // 8 bitů dat, bez parity
    UART5->C3  =  0x00;
    UART5->MA1 =  0x00; // no match address
    UART5->MA2 =  0x00; // no match address
    UART5->S2  |= 0xC0;
    UART5->C2  |= ( UART_C2_TE_MASK | UART_C2_RE_MASK ); // Zapnout vysilac i prijimac
}


/*
 * Inicializace portů.
 */
void PortsInit(void)
{
    // Povolení hodin pro včechny porty (PORTA, PORTB, PORTE)
    SIM->SCGC5 = SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTE_MASK | SIM_SCGC5_PORTA_MASK;
    // Povolení hodin pro UART5
    SIM->SCGC1 = SIM_SCGC1_UART5_MASK;

    // Nastavení PORTB pro LED.
    PORTB->PCR[5] = PORT_PCR_MUX(0x01); // D9
    PORTB->PCR[4] = PORT_PCR_MUX(0x01); // D10
    PORTB->PCR[3] = PORT_PCR_MUX(0x01); // D11
    PORTB->PCR[2] = PORT_PCR_MUX(0x01); // D12

    // Nastaveni PORTE pro tlačítka.
    PORTE->PCR[10] = PORT_PCR_MUX(0x01); // SW2
    PORTE->PCR[12] = PORT_PCR_MUX(0x01); // SW3
    PORTE->PCR[27] = PORT_PCR_MUX(0x01); // SW4
    PORTE->PCR[26] = PORT_PCR_MUX(0x01); // SW5
    PORTE->PCR[11] = PORT_PCR_MUX(0x01); // SW6

    // Nastavení PORTA por ROW a COLUMN
    PORTA->PCR[8] = PORT_PCR_MUX(0x01);  // Column bit A
    PORTA->PCR[10] = PORT_PCR_MUX(0x01); // Column bit B
    PORTA->PCR[6] = PORT_PCR_MUX(0x01);  // Column bit C
    PORTA->PCR[11] = PORT_PCR_MUX(0x01); // Column bit D
    PORTA->PCR[26] = PORT_PCR_MUX(0x01); // Row 0
    PORTA->PCR[24] = PORT_PCR_MUX(0x01); // Row 1
    PORTA->PCR[9] = PORT_PCR_MUX(0x01);  // Row 2
    PORTA->PCR[25] = PORT_PCR_MUX(0x01); // Row 3
    PORTA->PCR[28] = PORT_PCR_MUX(0x01); // Row 4
    PORTA->PCR[7] = PORT_PCR_MUX(0x01);  // Row 5
    PORTA->PCR[27] = PORT_PCR_MUX(0x01); // Row 6
    PORTA->PCR[29] = PORT_PCR_MUX(0x01); // Row 7

    // Nastavení PORTE pro Display #EN
    PORTE->PCR[28] = PORT_PCR_MUX(0x01);

    // Nastavení PORTE pro UART5
	PORTE->PCR[8]  = PORT_PCR_MUX(0x03); // UART5_TX
	PORTE->PCR[9]  = PORT_PCR_MUX(0x03); // UART5_RX

    // Nastavení pinů jako OUTPUT.
    PTB->PDDR = GPIO_PDDR_PDD(0x3C);     // LED ports OUT
    PTA->PDDR = GPIO_PDDR_PDD(ALL_COLUMNS | ALL_ROWS);     // Display ports OUT

    // Nastavení základních hodnot.
    PTB->PDOR |= GPIO_PDOR_PDO(0x3C);    // Vypnutí kontrolních LED
	PTE->PDDR = GPIO_PDDR_PDD(GPIO_PIN(28));
}


/*
 * Obsluha přerušení LPTM vyvoleného dosažením hodnoty compare.
 * ------------------------------------------------------------
 * Nastaví se hodnota compare, která mohla být upravena tlačítky UP a DOWN.
 * V případě nenastavení stop_flag se změní stav kontrolní diody LED D9 (SHIFT LED)
 * a posune se text v maticovém displeji.
 */
void LPTMR0_IRQHandler(void)
{
	// Nastavení hodnoty compare
	LPTMR0_CMR = compare;                // !! CMR může být nastavováno pouze při TCF == 1
	LPTMR0_CSR |=  LPTMR_CSR_TCF_MASK;   // Zápis 1 do TCF (tclear the flag)

	// Pokud není text zastavem, nasune se další sloupec
    if(!stop_flag)
    {
    	// Změna stavu LED D9
    	PTB->PTOR = SHIFT_LED;

		// Posum textu v maticovém displeji.
		idx_in_string++;
		if(idx_in_string >= string_len)
		{
			idx_in_string = 0;
		}
    }
}


/*
 * Inicializace IPTMR
 */
void LPTMR0Init(int count)
{
    SIM_SCGC5 |= SIM_SCGC5_LPTIMER_MASK; // povolení hodin pro LPTMR
    LPTMR0_CSR &= ~LPTMR_CSR_TEN_MASK;   // vypnutí LPTMR
    LPTMR0_PSR = ( LPTMR_PSR_PRESCALE(0) // nastavení předděličky
                 | LPTMR_PSR_PBYP_MASK
                 | LPTMR_PSR_PCS(1)) ;   // volba hodin
    LPTMR0_CMR = count;                  // nastavení hodnoty compare
    LPTMR0_CSR =(  LPTMR_CSR_TCF_MASK    // vyčištění aktuálních přerušení
                 | LPTMR_CSR_TIE_MASK    // povolení přerušení
                );
    NVIC_EnableIRQ(LPTMR0_IRQn);         // nastavení přerušení
    LPTMR0_CSR |= LPTMR_CSR_TEN_MASK;    // zapnutí LPTMR0
}


int main(void)
{
	// Inicializace FitKitu
	MCUInit();
	PortsInit();
	UART5Init();
	LPTMR0Init(compare);
	delay(1000);

	// Proměnné pro ovládání tlačítek SW2-SW6
	int pressed_up = 0;
	int pressed_down = 0;
	int pressed_stop = 0;
	int pressed_next_text = 0;
	int pressed_extern_text = 0;

	// Proměnné zobrazovaného textu.
	char *test_texts[] = {"Ahoj svete!", "Hello world!", "Hallo Welt!", "Bonjour le monde!"};
	char test_text_id = 0;				// Index vybraného interního testového textu.
	char *text_extern = NULL;			// Externě zadaný text (PUTTY)
	char *string_to_display = NULL;		// Kódovaný text

	// Kódování ASCII textu pro 8x16 maticový display
    string_len =  ascii_to_diod_text(&string_to_display , test_texts[test_text_id]);

    // Výpis informace o spuštění programu.
    send_str("FitKit3: Světelná tabule 2020");
    NEW_LINE;
    send_str("#############################");
    NEW_LINE;
    send_str("INFO: Interní text: ");
    send_ch('\"');
	send_str(test_texts[test_text_id]);
	send_ch('\"');
	NEW_LINE;

    while (1) {

    	// Rozsvítí další řádek na displeji
    	// Obsahuje DELAY, takže odstíní zákmity při stisknutí tlačítek
    	refresh_display(string_to_display);

    	// Zmáčknutí tlačítka UP zrychluje posun textu,
    	// snižuje hodnotu compare. Testem se zabráněno aby,
    	// hodnota klesla na, nebo pod 0.
        if(!pressed_up && !(GPIOE_PDIR & BTN_SW5))
        {
            pressed_up = 1;
            if(compare >= 0x40)
			{
            	send_str("INFO: Rychlost zvýšena.");
				NEW_LINE;
            	compare -= 0x20;
            }
            else
            {
            	send_str("WARNING: Rychlost posunu textu již nejde dále zvyšovat.");
            	NEW_LINE;
            }
        }
        else if(GPIOE_PDIR & BTN_SW5)
        {
        	pressed_up = 0;
        }

        // Zmáčknutí tlačítka DOWN zpomaluje posun textu,
		// zvyšuje hodnotu compare. Testem se zabráněno aby,
		// hodnota příliš stoupla.
        if(!pressed_down && !(GPIOE_PDIR & BTN_SW3))
        {
            pressed_down = 1;
            if(compare <= 0x100)
            {
            	send_str("INFO: Rychlost snížena.");
	            NEW_LINE;
            	compare += 0x20;
            }
            else
            {
            	send_str("WARNING: Rychlost posunu textu již nejde dále snižovat.");
            	NEW_LINE;
            }
        }
        else if(GPIOE_PDIR & BTN_SW3)
        {
        	pressed_down = 0;
        }

        // Detekce stisku tlačítka SW2 (PAUSE/PLAY)
		if(!pressed_stop && !(GPIOE_PDIR & BTN_SW2))
		{
			// INFO
			send_str((stop_flag) ? "INFO: Play" : "INFO: Pause");
			NEW_LINE;

			stop_flag = (stop_flag) ? 0 : 1;
			pressed_stop = 1;

			// Pokud je nastavena pauza, zapne LED D9.
			if(stop_flag)
			{
				PTB->PCOR = SHIFT_LED;
			}

		}
		else if((GPIOE_PDIR & BTN_SW2))
		{
			pressed_stop = 0;
		}

		// Detekce stisku tlačítka SW4 (NEXT TEXT)
		if(!pressed_next_text && !(GPIOE_PDIR & BTN_SW4))
		{
			pressed_next_text = 1;
			// Zastavení současného textu.
			stop_flag = 1;

			// Dealokace již zobrazovaného textu
			if(string_to_display != NULL)
			{
				free(string_to_display);
				string_to_display = NULL;
			}
			if(text_extern != NULL)
			{
				free(text_extern);
				text_extern = NULL;
			}

			// Posun na další testový text.
			// Pokud došlo k přetečení je nastaven index 0.
			test_text_id++;
			if(test_text_id == MAX_TEST_TEXTS)
			{
				test_text_id = 0;
			}

			// INFO
			send_str("INFO: Změna interního textu na: ");
			send_str(test_texts[test_text_id]);
			NEW_LINE;

			// Kódování ASCII textu pro 8x16 maticový display
			string_len =  ascii_to_diod_text(&string_to_display , test_texts[test_text_id]);

			// Spuštění nového textu od začátku.
			idx_in_string = 0;
			stop_flag = 0;
		}
		else if((GPIOE_PDIR & BTN_SW4))
		{
			pressed_next_text = 0;
		}

		// Detekce stisku tlačítka SW6 (EXTERN TEXT)
		if(!pressed_extern_text && !(GPIOE_PDIR & BTN_SW6))
		{
			// INFO
			send_str("INFO: Načítání vnějšího vstupu.");
			NEW_LINE;

			pressed_extern_text = 1;
			// Zastavení současného textu.
			stop_flag = 1;
			// Vypnutí displeje
			PTE->PDOR |= GPIO_PDOR_PDO(GPIO_PIN(28));

			// Nastavení kontrolních diod
			PTB->PDOR |= SHIFT_LED;   // LED D9 OFF
			PTB->PDOR &= ~INPUT_LED;  // LED D12 ON

			// Dealokace již zobrazovaného textu
			if(string_to_display != NULL)
			{
				free(string_to_display);
				string_to_display = NULL;
			}
			if(text_extern != NULL)
			{
				free(text_extern);
				text_extern = NULL;
			}

			// Výzva k zadání textu a jeho načtení.
			send_str("Text>");
			text_extern = receive_str();
			// Kódování ASCII textu pro 8x16 maticový display
			string_len =  ascii_to_diod_text(&string_to_display , text_extern);

			// INFO
			send_str("INFO: Zobrazování textu: ");
			send_ch('\"');
			send_str(text_extern);
			send_ch('\"');
			NEW_LINE;

			// Spuštění textu
			idx_in_string = 0;
			stop_flag = 0;
			pressed_extern_text = 0;

			// Zapnutí displeje
			PTE->PCOR = GPIO_PDDR_PDD(GPIO_PIN(28));
			// Vypnutí kontrolní LED D12
			PTB->PDOR |= INPUT_LED;
		}
		else if(pressed_extern_text && (GPIOE_PDIR & BTN_SW6))
		{
			pressed_extern_text = 0;
		}
    }

    return 0;
}
