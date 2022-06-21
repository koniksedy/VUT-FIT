#include "scanner.h"

#define CHECK_CHAR(ch) if(ch < 0  ch > 127) exit(ERR_LEX);

// DOPLNĚNO
char get_char_and_check(void)
{
    char ch = getc(stdin);
    if(ch < -1)
    {
        exit(ERR_LEX);
    }

    return ch;
}

/**
 *@brief Funkce spojí texty a rezervuje pro nì místo vpamìti
 *@param puvodniString      Text který bude rozšíøen o druhý string a alokován v pamìti
 *@param pridavanyChar    Text který bude pøidán do pamìti hned za prvním stringem
 *@return               Vrací VOID
 */
void appendString(char** puvodniString, char pridavanyChar)
{
    int puvodniDelka = strlen(*puvodniString);
    char* novyString = (char*)malloc(puvodniDelka*sizeof(char)+2);
    if(novyString == NULL){ // kontrola mallocu
        return;
    }else{
        strcpy(novyString, *puvodniString);
        novyString[puvodniDelka] = pridavanyChar;
        novyString[puvodniDelka+1] = '\0';
        free(*puvodniString);
        *puvodniString = novyString;
    }
}


/**
 *@brief kontroluje zda je vstup vhodny do stringu [a-zA-Z_]
 *@param vstup  kontrolovany znak
 *@return       vraci bool
 */
bool jeCharString(const char vstup){
    if(vstup >= 'a' && vstup <= 'z')
        return true;
    else if(vstup >= 'A' && vstup <= 'Z')
        return true;
    else if(vstup == '_')
        return true;
    else
        return false;
}

/**
 *@brief kontroluje zda je vstup cislice
 *@param vstup  kontrolovany znak
 *@return       vraci bool
 */
bool jeCharCislice(const char vstup){
    if(vstup >= '0' && vstup <= '9')
        return true;
    else
        return false;
}

bool maCisloNulyNaZacatku(token_t* token){
    if(token->data->str->data[0] == '0'){       // Přidáno - nedovolovals detekci nuly
        if(token->data->str->data[1] == '.' || token->data->str->data[1] == '\0'){
            return true;
        }else{
            return false;
        }
    }
    return true;
}

/**
 *@brief kontroluje zda jsou dalsi znaky na stdin dva po sobe jdouci uvozovky
 *@return               vraci bool
 */
bool dalsiDvaCharyJsouUvozovky(){
    char prvniZnak = get_char_and_check();
    if(prvniZnak != '"'){
        ungetc(prvniZnak,stdin);
        return false;
    }
    char druhyZnak = get_char_and_check();
    if(druhyZnak != '"'){
        ungetc(druhyZnak,stdin);
        ungetc(prvniZnak,stdin);
        return false;
    }
    return true;
}

/**
 *@brief kontroluje zda jsou dalsi znaky na stdin dva po sobe jdouci apostrofy
 *@return               vraci bool
 */
bool dalsiDvaCharyJsouApostrofy(){
    char prvniZnak = get_char_and_check();
    if(prvniZnak != '\''){
        ungetc(prvniZnak,stdin);
        return false;
    }
    char druhyZnak = get_char_and_check();
    if(druhyZnak != '\''){
        ungetc(druhyZnak,stdin);
        ungetc(prvniZnak,stdin);
        return false;
    }
    return true;
}

/**
 *@brief kontroluje zda jsou dalsi znaky na stdin tri po sobe jdouci uvozovky a posledni znak nebylo obracene lomitko
 *@param posledniZnak   predchozi znak, ktery nesmi byt obracene lomitko
 *@return               vraci bool
 */
bool dalsiTriCharyJsouUvozovky(char posledniZnak){
    if(posledniZnak == '\\')
        return false;
    char prvniZnak = get_char_and_check();
    if(prvniZnak != '"'){
            if(prvniZnak == EOF){
                return true;
            }
        ungetc(prvniZnak,stdin);
        return false;
    }
    char druhyZnak = get_char_and_check();
    if(druhyZnak != '"'){
        ungetc(druhyZnak,stdin);
        ungetc(prvniZnak,stdin);
        return false;
    }
    char tretiZnak = get_char_and_check();
    if(tretiZnak != '"'){
        ungetc(tretiZnak,stdin);
        ungetc(druhyZnak,stdin);
        ungetc(prvniZnak,stdin);
        return false;
    }
    return true;
}

/**
 *@brief kontroluje zda jsou dalsi znaky na stdin tri po sobe jdouci apostrofy a posledni znak nebylo obracene lomitko
 *@param posledniZnak   predchozi znak, ktery nesmi byt obracene lomitko
 *@return               vraci bool
 */
bool dalsiTriCharyJsouApostrofy(char posledniZnak){
    if(posledniZnak == '\\')
        return false;
    char prvniZnak = get_char_and_check();
    if(prvniZnak != '\''){
            if(prvniZnak == EOF){
                return true;
            }
        ungetc(prvniZnak,stdin);
        return false;
    }
    char druhyZnak = get_char_and_check();
    if(druhyZnak != '\''){
        ungetc(druhyZnak,stdin);
        ungetc(prvniZnak,stdin);
        return false;
    }
    char tretiZnak = get_char_and_check();
    if(tretiZnak != '\''){
        ungetc(tretiZnak,stdin);
        ungetc(druhyZnak,stdin);
        ungetc(prvniZnak,stdin);
        return false;
    }
    return true;
}

/**
 *@brief kotroluje zda jsou dalsi zva znaky na vstupu tecka a nejaka cislice
 *@return       vraci bool
 */
bool dalsiJePlus_MinusCislo(){
    char prvniVstup = get_char_and_check();
    if(prvniVstup == '+' || prvniVstup == '-'){
        char druhyVstup = get_char_and_check();
        if(jeCharCislice(druhyVstup)){
            ungetc(druhyVstup,stdin);
            ungetc(prvniVstup,stdin);
            return true;
        }else{
            ungetc(druhyVstup,stdin);
            ungetc(prvniVstup,stdin);
            return false;
        }
    }else{
        ungetc(prvniVstup,stdin);
        return false;
    }
}

/**
 *@brief kotroluje zda je dalsi znak na stdin cislice
 *@return               vraci bool
 */
bool nasledujeCislo(){
    char vstup = get_char_and_check();
    bool vysledek = jeCharCislice(vstup);
    ungetc(vstup,stdin);
    return vysledek;
}

/**
 *@brief kontroluje zda je dalsi znak apostrof a znak predtim nebyl obracene lomitko
 *@param input          znak ktery ma byt apostrof
 *@param posledniZnak   predchozi znak, ktery nesmi byt obracene lomitko
 *@return               vraci bool
 */
bool tedJeApostrof(char input, char posledniZnak){
    if(input == '\''){
        if(posledniZnak != '\\'){
            return false;
        }
    }
    return true;
}

/**
 *@brief kontroluje zda je dalsi znak uvozovka a znak predtim nebyl obracene lomitko
 *@param input          znak ktery ma byt uvozovka
 *@param posledniZnak   predchozi znak, ktery nesmi byt obracene lomitko
 *@return               vraci bool
 */
bool tedJeUvozovka(char input, char posledniZnak){
    if(input == '"'){
        if(posledniZnak != '\\'){
            return false;
        }
    }
    return true;
}

/**
 *@brief kontroluje zda jsou dalsi znaky na stdin tecka a cislo
 *@return          vraci bool
 */
bool dalsiJeTeckaACislo(){
    char prvniZnak = get_char_and_check();
    if(prvniZnak != '.'){
            if(prvniZnak == EOF){
                return true;
            }
        ungetc(prvniZnak,stdin);
        return false;
    }
    char druhyZnak = get_char_and_check();
    if(!jeCharCislice(druhyZnak)){
        ungetc(druhyZnak,stdin);
        ungetc(prvniZnak,stdin);
        return false;
    }
    ungetc(druhyZnak,stdin);
    ungetc(prvniZnak,stdin);
    return true;
}

/**
 *@brief zarezervuje misto v pameti pro token a nastavi ho na UNDEFINED
 *@return          vraci token_t*
 */
token_t* token_ctor(){  // musi se volat na prazdny token
    token_t* odkazDoPameti = (token_t*) malloc(sizeof(token_t));
    if(odkazDoPameti == NULL){
        // chyba mallocu
        return NULL;
    }

    odkazDoPameti->type = UNDEFINED;        // token neni definovany
    odkazDoPameti->data = NULL;             // data jsou null
    return odkazDoPameti;
}

/**
 *@brief znici cely token a nastavi ho na NULL
 *@param token     token ktery ma byt znicen
 *@return          vraci VOID
 */
void token_dtor(token_t**  token){
    if((*token)->type == STRING || (*token)->type == IDENTIFIER){
        string_dtor(&(*token)->data->str);
    }
    free((*token)->data);
    free(*token);
    *token = NULL;
}

/**
 *@brief vycisti vsechnu pamet kterou ma token zarezervovanou
 *@param token     token ktery ma byt vycisten
 *@return          vraci VOID
 */
void token_clean(token_t* token){
    if(token->data == NULL){// nema data
        token->type = UNDEFINED;
        return;
    }
    if(token->type == STRING || token->type == IDENTIFIER || token->type == COMMENT)
        string_dtor(&token->data->str);

    free(token->data);
    token->data = NULL;
}

/**
 *@brief prevadi string_t na float
 *@param token     token ktery obsahuje string_t s floatem
 *@return          vraci VOID
 */
void tokenStringToFloat(token_t* token){
    if(token->type == FLOAT){
        char* zbytekStringu;
        double hodnota = strtod(token->data->str->data,&zbytekStringu);
        string_dtor(&token->data->str);
        token->data->fl = hodnota;
    }
}

/**
 *@brief prevadi string_t na int
 *@param token     token ktery obsahuje string_t s intem
 *@return          vraci VOID
 */
void tokenStringToInt(token_t* token){
    if(token->type == INT){
        char* zbytekStringu;
        int hodnota = strtol(token->data->str->data,&zbytekStringu,10);
        string_dtor(&token->data->str);
        token->data->i = hodnota;
    }
}

/**
 *@brief funkce kontroluje zda identifikator neodpovida klicovym slovum a pokud ano, tak ho nastavi na to slovo
 *@param token     token ktery obsahuje string_t s identifikatorem
 *@return          vraci VOID
 */
void tokenNaKlicovaSlova(token_t* token){
    if(strcmp(token->data->str->data,"print") == 0){
        token_clean(token);
        token->type = PRINT;
        return;
    }
    if(strcmp(token->data->str->data,"None") == 0){
        token_clean(token);
        token->type = NONE;
        return;
    }
    if(strcmp(token->data->str->data,"if") == 0){
        token_clean(token);
        token->type = IF;
        return;
    }
    if(strcmp(token->data->str->data,"else") == 0){
        token_clean(token);
        token->type = ELSE;
        return;
    }
    if(strcmp(token->data->str->data,"def") == 0){
        token_clean(token);
        token->type = DEF;
        return;
    }
    if(strcmp(token->data->str->data,"pass") == 0){
        token_clean(token);
        token->type = PASS;
        return;
    }
    if(strcmp(token->data->str->data,"return") == 0){
        token_clean(token);
        token->type = RETURN;
        return;
    }
    if(strcmp(token->data->str->data,"while") == 0){
        token_clean(token);
        token->type = WHILE;
        return;
    }
    if(strcmp(token->data->str->data,"inputs") == 0){
        token_clean(token);
        token->type = INPUTS;
        return;
    }
    if(strcmp(token->data->str->data,"inputi") == 0){
        token_clean(token);
        token->type = INPUTI;
        return;
    }
    if(strcmp(token->data->str->data,"inputf") == 0){
        token_clean(token);
        token->type = INPUTF;
        return;
    }
    if(strcmp(token->data->str->data,"chr") == 0){
        token_clean(token);
        token->type = CHR;
        return;
    }
    if(strcmp(token->data->str->data,"len") == 0){
        token_clean(token);
        token->type = LEN;
        return;
    }
    if(strcmp(token->data->str->data,"substr") == 0){
        token_clean(token);
        token->type = SUBSTR;
        return;
    }
    if(strcmp(token->data->str->data,"ord") == 0){
        token_clean(token);
        token->type = ORD;
        return;
    }
}

/**
 *@brief funkce kontroluje zda se jenda o hex znak
 *@param znak      kontrolovany znak
 *@return          vraci bool
 */
bool JeCharHex(char znak){
    if(znak >= 'a' && znak <= 'f'){
        return true;
    }
    if(znak >= 'A' && znak <= 'F'){
        return true;
    }
    if(znak >= '0' && znak <= '9'){
        return true;
    }
    return false;
}

/**
 *@brief funkce vraci dec. vysledek hexadedimalniho znaku
 *@param znak      hex znak
 *@return          vraci int
 */
int hexNaInt(char znak){
    if(znak == '0'){
        return 0;
    }else if(znak == '1'){
        return 1;
    }else if(znak == '2'){
        return 2;
    }else if(znak == '3'){
        return 3;
    }else if(znak == '4'){
        return 4;
    }else if(znak == '5'){
        return 5;
    }else if(znak == '6'){
        return 6;
    }else if(znak == '7'){
        return 7;
    }else if(znak == '8'){
        return 8;
    }else if(znak == '9'){
        return 9;
    }else if(znak == 'a' || znak == 'A'){
        return 10;
    }else if(znak == 'b' || znak == 'B'){
        return 11;
    }else if(znak == 'c' || znak == 'C'){
        return 12;
    }else if(znak == 'd' || znak == 'D'){
        return 13;
    }else if(znak == 'e' || znak == 'E'){
        return 14;
    }else if(znak == 'f' || znak == 'F'){
        return 15;
    }else{
        return 0;
    }
}

/**
 *@brief funkce prevadi HH na char
 *@param prvniZnak      levy hex znak
 *@param druhyZnak      pravy hex znak
 *@return               Vrací vraci znak podle hodnoty ASCII z parametru
 */
char hexNaChar(unsigned char prvniZnak,unsigned char druhyZnak){
    unsigned char vracenyZnak = (hexNaInt(prvniZnak)*16) + hexNaInt(druhyZnak);
    return vracenyZnak;
}

/**
 *@brief funkce prochazi string a tvori novy, ktery pocita s escape sequenci
 *@param strTokenPuvodni        puvodni string token
 *@return                       Vrací VOID
 */
bool escapeSequence(string_t* strTokenPuvodni){
    string_t* strTokenKonecny = string_ctor();
    if(strTokenKonecny == NULL)
        return false;

    for(int i = 0; i < strTokenPuvodni->len ; i++){
        if(strTokenPuvodni->data[i] == '\\'){
            if(strTokenPuvodni->data[i+1] == 't'){
                string_put_char(&strTokenKonecny,9);
                i++;
            }else if(strTokenPuvodni->data[i+1] == 'n'){
                string_put_char(&strTokenKonecny,'\n');
                i++;
            }else if(strTokenPuvodni->data[i+1] == '\''){
                string_put_char(&strTokenKonecny,'\'');
                i++;
            }else if(strTokenPuvodni->data[i+1] == '"'){
                string_put_char(&strTokenKonecny,'"');
                i++;
            }else if(strTokenPuvodni->data[i+1] == '\\'){
                string_put_char(&strTokenKonecny,'\\');
                i++;
            }else if(strTokenPuvodni->data[i+1] == 'x'){
                if(JeCharHex(strTokenPuvodni->data[i+2]) && JeCharHex(strTokenPuvodni->data[i+3])){
                    unsigned char pridavanyChar = hexNaChar(strTokenPuvodni->data[i+2],strTokenPuvodni->data[i+3]);
                    if(pridavanyChar > 0 && pridavanyChar < 128){
                        string_put_char(&strTokenKonecny,pridavanyChar);
                        i++;
                        i++;
                        i++;
                    }else{
                        string_dtor(&strTokenKonecny);
                        return false;
                    }
                }else{
                    string_dtor(&strTokenKonecny);
                    return false;
                }
            }else{
                string_put_char(&strTokenKonecny,'\\');
            }
        }else{
            string_put_char(&strTokenKonecny,strTokenPuvodni->data[i]);
        }
    }

    string_cpy(strTokenPuvodni,strTokenKonecny);
    string_dtor(&strTokenKonecny);
    return true;
}


/**
 *@brief funkce naplnuje token
 *@param token      token do ktereho myji vyt vlozena data
 *
 *  Prvni se vycisti token a alokuji se data pro novy.
 *  Pak se se kontroluje zda nejsme na konci souboru, pokud nejsme tak podle pravidel prilozeneho automatu.
 *  Pokud nesapdne do zadne podminky, tak je nastaven na UNDEFINED s charem ktery nespadl ani do jedne podminky.
 *
 *@return           Vrací bool
 */
bool token_get_next(token_t* token){
    // token se vycisti, tim padem vzdy budeme delat s blank daty
    token_clean(token);

    // alokace dat a jeji kontrola
    token->data = (token_data_t*)malloc(sizeof(token_data_t));
    if(token->data == NULL){
        // chyba mallocu
        return false;
    }

    // nastaveni statickych promenych
    static bool bylEOL = true;
    static int pocetMezer = 0;
    // pocitani mezer, pokud byl posledni znak EOL  //
    if(bylEOL){                                     //
        char znak = get_char_and_check();                  //
        pocetMezer = 0;                            //
        while(znak == ' ' || znak == '#'){                       //
            if(znak == '#'){
                while((znak = getchar()) != '\n'){           //
                }
                token->type = GAP;
                token->data->ch = ' ';          // Doplněno
                return true;
                //pocetMezer = stack_top(GLOBALNI_STACK);
                break;
            }
            pocetMezer++;                           //
            znak = get_char_and_check();                   //
        }   
                                
         ungetc(znak,stdin);                       //
         bylEOL = false;


    }//-----------------------------------------------


    // zpracovani INDENT/DEDENT
    if(pocetMezer != stack_top(GLOBALNI_STACK)){
        if(pocetMezer > stack_top(GLOBALNI_STACK)){
            stack_push(GLOBALNI_STACK,pocetMezer);
            token->type = INDENT;
            token->data->ch = '+';
            return true;
        }

        if(pocetMezer < stack_top(GLOBALNI_STACK)){
            stack_pop(GLOBALNI_STACK);
            if(pocetMezer > stack_top(GLOBALNI_STACK)){
                /* chyba - v  tomto miste nemuze byt pocet mezer vetsi,
                muze byt jen stejny nebo mensi */
                token->type = UNDEFINED;
                token->data->ch = 'f';// nemusi se nastavovat, ale nechci to nechavat na NULL
                return false;
            }
            token->type = DEDENT;
            token->data->ch = '-';
            return true;
        }
    }

    // hlavni rozhodovaci cast, ktera uz bere CHARy a dela vsechny ostatni tokeny
    char input = get_char_and_check();
    if(input != EOF){

        if(input == ' '){//   GAP   //
            token->type = GAP;      //
            token->data->ch = input;//
            bylEOL = false;         //
            return true;//------------

        }else if(input == '"'){// STRING zkontroluje zda je jen jedna " a bere vse do dalsi //
            token->data->str = string_ctor();                                               //
            token->type = STRING;                                                           //
            bool spravneUkonceni = true;
            char posledniZnak = input;// tady si ukladam posledni pouzity znak              //
            if(dalsiDvaCharyJsouUvozovky()){// v tomhle IFu se hledaji tri znaky "//
                token->type = COMMENT;
                while(!dalsiTriCharyJsouUvozovky(input)){                         //        //
                    posledniZnak = input;                                         //        //
                    input = getc(stdin);                                          //        //
                    string_put_char(&token->data->str,input);                     //        //
                }                                                                 //        //                                //        //
                input = getc(stdin);
                if(input == EOF){
                    spravneUkonceni = false;
                }
                ungetc(input,stdin);
                return (escapeSequence(token->data->str) && spravneUkonceni);// konec tri uvozovek-----------------------------------        //
            }                                                                               //
            posledniZnak = input;                                                           //
            input = get_char_and_check();                                                            //
            while(tedJeUvozovka(input,posledniZnak)){                                       //
                    if(input == EOF){                                                       //                                 //
                        return false;                                                        //
                    }                                                                       //
                    if(input == '\n'){                                                       //                                //
                        ungetc(input,stdin);
                        return false;                                                        //
                    }
                string_put_char(&token->data->str,input);                                   //
                posledniZnak = input;                                                       //
                input = get_char_and_check();                                                        //
            }                                                                               //
            bylEOL = false;                                                                 //
            input = getc(stdin);                                             //
            if(input == EOF){
                    spravneUkonceni = false;
                }
            ungetc(input,stdin);
            return (escapeSequence(token->data->str) && spravneUkonceni);//--------------------------------------------------------------------

        }else if(input == '\''){// STRING zkontroluje zda je jen jeden ' a bere vse do dalsi//
            token->data->str = string_ctor();                                               //
            token->type = STRING;                                                           //
            bool spravneUkonceni = true;
            char posledniChar = input;// tady si ukladam posledni pouzity znak              //
            if(dalsiDvaCharyJsouApostrofy()){// v tomhle IFu se hledaji tri znaky '//       //
                token->type = COMMENT;
                while(!dalsiTriCharyJsouApostrofy(input)){                         //       //
                    posledniChar = input;                                          //       //
                    input = getc(stdin);                                           //       //
                    string_put_char(&token->data->str,input);                      //       //
                }                                                                  //       //                             //       //
                input = getc(stdin);
                if(input == EOF){
                    spravneUkonceni = false;
                }
                ungetc(input,stdin);
                return (escapeSequence(token->data->str) && spravneUkonceni);// konec tri apostrofu-----------------------------------       //
            }                                                                               //
            posledniChar = input;                                                           //
            input = get_char_and_check();                                                            //
            while(tedJeApostrof(input,posledniChar)){                                       //
                    if(input == EOF){
                        return false;                                                        //
                    }                                                                       //
                    if(input == '\n'){                                                       //                                //
                        ungetc(input,stdin);
                        return false;                                                        //
                    }
                string_put_char(&token->data->str,input);                                   //
                posledniChar = input;                                                       //
                input = get_char_and_check();                                                        //
            }                                                                               //
            bylEOL = false;                                                                 //                                              //
            input = getc(stdin);
            if(input == EOF){
                spravneUkonceni = false;
            }
            ungetc(input,stdin);
            return (escapeSequence(token->data->str) && spravneUkonceni);//--------------------------------------------------------------------

        }else if(input == '\n'){//   EOL_t  //
            token->type = EOL_t;            //
            token->data->ch = '0';          //
            bylEOL = true;                  //
            return true;//--------------------

        }else if(jeCharCislice(input)){//          INT nebo FLOAT - nejtezsi casti asi          //
            token->data->str = string_ctor();                                                   //
            token->type = INT;                                                                  //
            bool validitaCisla = true;
            bool bylaDesetinaTecka = false;                                                     //
            bool byloPouzitoE = false;                                                          //
            while(jeCharCislice(input) || input == '.' || input == 'e' || input == 'E'){        //
                if(input == '.' && nasledujeCislo() && !bylaDesetinaTecka && !byloPouzitoE){// normalni float s desetinou teckou //
                    token->type = FLOAT;                                                        //
                    string_put_char(&token->data->str,input);                                   //
                    input = get_char_and_check();                                                        //
                    string_put_char(&token->data->str,input);                                   //
                    input = get_char_and_check();                                                        //
                    bylaDesetinaTecka = true;                                                   //
                }else if((input == 'e' || input == 'E') && !byloPouzitoE){// exponencialni float//
                    if(dalsiJePlus_MinusCislo()){// nasleduje -/+ a pak cislo                   //
                        token->type = FLOAT;                                                    //
                        string_put_char(&token->data->str,input);                               //
                        input = get_char_and_check();                                                    //
                        string_put_char(&token->data->str,input);                               //
                        input = get_char_and_check();                                                    //
                        byloPouzitoE = true;                                                    //
                    }else if(nasledujeCislo()){// nasleduje jen cislo                           //
                        token->type = FLOAT;                                                    //
                        string_put_char(&token->data->str,input);                               //
                        input = get_char_and_check();                                                    //
                        byloPouzitoE = true;                                                    //
                    }                                                                           //
                    else{// prislo e, ale za nim neni ani cislo ani (+/- a cislo)               //
                        ungetc(input,stdin);                                                    //
                        validitaCisla = maCisloNulyNaZacatku(token);
                        tokenStringToFloat(token);                                              // pokud je string float, tak se type nastavi na float
                        tokenStringToInt(token);                                                // pokud je string int, tak se type nastavi na int
                        return validitaCisla;                                                            //
                    }                                                                           //
                }else if(jeCharCislice(input)){// pokud to spadne do tehle podminky tak jsme v intu //
                    string_put_char(&token->data->str,input);                                   //
                    input = get_char_and_check();                                                        //
                }else{                                                                          //
                    ungetc(input,stdin);                                                        //
                    validitaCisla = maCisloNulyNaZacatku(token);
                    bylEOL = false;                                                             //
                    tokenStringToFloat(token);                                                  // pokud je string float, tak se type nastavi na float
                    tokenStringToInt(token);                                                    // pokud je string int, tak se type nastavi na int
                    return validitaCisla;                                                                //
                }                                                                               //
            }                                                                                   //
            // tady uz prislo neco jinaho nez co patri do floatu                                //
            ungetc(input,stdin);                                                                //
            bylEOL = false;                                                                     //
            validitaCisla = maCisloNulyNaZacatku(token);
            tokenStringToFloat(token);                                                          // pokud je string float, tak se type nastavi na float
            tokenStringToInt(token);                                                            // pokud je string int, tak se type nastavi na int
            return validitaCisla;//------------------------------------------------------------------------

        }else if(jeCharString(input)){//      IDENTIFIER        // prvni muze byt jen string
            token->data->str = string_ctor();                   //
            token->type = IDENTIFIER;                           //
            while(jeCharCislice(input) || jeCharString(input)){ // dalsi uz muze byt string nebo cislice, jako treba a1, slovo5, str20
                string_put_char(&token->data->str,input);       //
                input = get_char_and_check();                            //
            }                                                   //
            ungetc(input,stdin);                                //
            bylEOL = false;                                     //
            tokenNaKlicovaSlova(token);                         //
            return true;//----------------------------------------

        }else if(input == '/'){//  OPERATOR_DELENO  //
            token->type = OPERATOR_DELENO;          //
            token->data->ch = input;                //
            input = get_char_and_check();                    //
            if(input == '/'){                       //
                token->type = OPERATOR_DELENO_INT;  // pokud je dalsi CHAR / tak se jedna o OPERATOR_DELENO_INT
                token->data->ch = input;            //
            }else{                                  //
                ungetc(input,stdin);                //
            }                                       //
            bylEOL = false;                         //
            return true;//----------------------------

        }else if(input == '*'){// OPERATOR_KRAT //
            token->type = OPERATOR_KRAT;        //
            token->data->ch = input;            //
            bylEOL = false;                     //
            return true;//------------------------

        }else if(input == '-'){//   OPERATOR_MINUS  //
            token->type = OPERATOR_MINUS;           //
            token->data->ch = input;                //
            bylEOL = false;                         //
            return true;//----------------------------

        }else if(input == '<'){//  OPERATOR_MENSI   //
            token->type = OPERATOR_MENSI;           //
            token->data->ch = input;                //
            input = get_char_and_check();                    //
            if(input == '='){                       //
                token->type = OPERATOR_MENSI_ROVNO; // pokud je dalsi CHAR = tak se jedna o OPERATOR_MENSI_ROVNO
                token->data->ch = input;            //
            }else{                                  //
                ungetc(input,stdin);                //
            }                                       //
            bylEOL = false;                         //
            return true;//----------------------------

        }else if(input == '>'){//  OPERATOR_VETSI   //
            token->type = OPERATOR_VETSI;           //
            token->data->ch = input;                //
            input = get_char_and_check();                    //
            if(input == '='){                       //
                token->type = OPERATOR_VETSI_ROVNO; // pokud je dalsi CHAR = tak se jedna o OPERATOR_VETSI_ROVNO
                token->data->ch = input;            //
            }else{                                  //
                ungetc(input,stdin);                //
            }                                       //
            bylEOL = false;                         //
            return true;//----------------------------

        }else if(input == '+'){// OPERATOR_PLUS //
            token->type = OPERATOR_PLUS;        //
            token->data->ch = input;            //
            bylEOL = false;                     //
            return true;//------------------------

        }else if(input == '!'){// OPERATOR_NEROVNOST //
            token->type = UNDEFINED;        //
            token->data->ch = input;            //
            input = get_char_and_check();
            if(input == '='){
                token->type = OPERATOR_NEROVNOST;
            }else{
                ungetc(input,stdin);
            }
            bylEOL = false;                     //
            return true;//------------------------

        }else if(input == '='){//     ASSIGN    //
            token->type = ASSIGN;               //
            token->data->ch = input;            //
            input = get_char_and_check();                //
            if(input == '='){                   // pokud je dalsi CHAR = tak se jedna o OPERATOR_ROVNOST
                token->type = OPERATOR_ROVNOST; //
                token->data->ch = input;        //
            }else{                              //
                ungetc(input,stdin);            //
            }                                   //
            bylEOL = false;                     //
            return true;//------------------------

        }else if(input == '('){// LEFT_PAR  //
            token->type = LEFT_PAR;         //
            token->data->ch = input;        //
            bylEOL = false;                 //
            return true;//--------------------

        }else if(input == ')'){// RIGHT_PAR //
            token->type = RIGHT_PAR;        //
            token->data->ch = input;        //
            bylEOL = false;                 //
            return true;//--------------------

        }else if(input == ':'){// COLON //
            token->type = COLON;        //
            token->data->ch = input;    //
            bylEOL = false;             //
            return true;//----------------

        }else if(input == ','){// COMMA //
            token->type = COMMA;        //
            token->data->ch = input;    //
            bylEOL = false;             //
            return true;//----------------

        }else if(input == '#'){//  COMMENT  // vrací token EOL_t zbytek se ignoruje
            while(input != '\n'){           //
                input = getchar();        //
            }                               //
            token->type = EOL_t;            //
            token->data->ch = '0';          //
            bylEOL = true;                  //
            return true;//--------------------

        }else{//      UNDEFINED         // Pokud vsechny selzou
            token->type = UNDEFINED;    //
            token->data->ch = input;    //
            bylEOL = false;             //
            return false;//---------------

        }

    }
    //         EOF_t        //
    token->type = EOF_t;    //
    token->data->ch = '0';  //
    bylEOL = false;         //
    return true;//------------
}
