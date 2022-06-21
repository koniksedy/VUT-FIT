#include "string_dynamic.h"
#define INFO ;//fprintf(stderr, "---------------------------------\nmodul-----> %s\nfunkce ------> %s\nline-----> %d\n---------------------------------\n", __FILE__, __func__, __LINE__);
// FUNGUJE
string_t* string_ctor(){
    INFO;
    string_t* odkazDoPameti = (string_t*)malloc(sizeof(string_t));
    if(odkazDoPameti == NULL){
        //chyba mallocu
        return NULL;
    }
    odkazDoPameti->data = (char*)calloc(BUDGET,sizeof(char));
    if(odkazDoPameti->data == NULL){
        //chyba mallocu zahlas error
        //znič odkazDoPmeti a vrať NULL
    }
    odkazDoPameti->len =0;
    odkazDoPameti->free_spac = (BUDGET-1);
    return odkazDoPameti;
}

// NEFUNGUJE
bool string_realoc(string_t* stringPtr){
    INFO;
    int newBudget = strlen(stringPtr->data) + BUDGET + 1;
    char* noveData = realloc(stringPtr->data,newBudget);

    if(noveData == NULL){
        //chyba mallocu
        return false;
    }
    stringPtr->data = noveData;
    int stareFree = stringPtr->free_spac;
    stringPtr->free_spac = stareFree + BUDGET;

    return true;
}

// FUNGUJE
// Potřebuješ doplnut jednu * k stringPtr, abys jej mohl při nepspěšném reallocu poslat na string_dtor.
void string_put_char(string_t** stringPtr, char pridavanyChar){
    INFO;

    if((*stringPtr)->free_spac > 0){
        (*stringPtr)->data[(*stringPtr)->len] = pridavanyChar;
        (*stringPtr)->data[(*stringPtr)->len+1] = '\0';
        (*stringPtr)->len++;
        (*stringPtr)->free_spac--;
    }else{
        bool uspesnostRealocu = string_realoc((*stringPtr));
        if(uspesnostRealocu == false){
            // chyba reallocu zahlas ereror
            // pošli na dtor
            string_dtor(stringPtr);
            return;
        }
        (*stringPtr)->data[(*stringPtr)->len] = pridavanyChar;
        (*stringPtr)->data[(*stringPtr)->len+1] = '\0';
        (*stringPtr)->len++;
        (*stringPtr)->free_spac--;
    }
}


// FUNGUJE
// přidat taky hvězdičku
void string_clean(string_t** stringPtr ){
    INFO;
    free((*stringPtr)->data);
    (*stringPtr)->data = (char*) malloc(sizeof(char)*BUDGET);

    if((*stringPtr)->data == NULL){
        //chyba mallocu zahlas error
        // pošli na dtor
        string_dtor(stringPtr);
    }
    (*stringPtr)->len =0;
    (*stringPtr)->free_spac = (BUDGET-1);
    (*stringPtr)->data[0]='\0';
}

// FUNGUJE
unsigned int string_len(string_t* stringPtr){
    INFO;
    return stringPtr->len;
}

bool string_cmp_string(string_t* stringOnePtr, string_t* stringTwoPtr){
    bool vysledek = strcmp(stringOnePtr->data, stringTwoPtr->data);
    if(vysledek == 0){
        return true;
    }else{
        return false;
    }
}

bool string_cmp_constant_string(string_t* stringOnePtr, char* stringTwoPtr){
    INFO;
    bool vysledek = strcmp(stringOnePtr->data, stringTwoPtr);
    if(vysledek == 0){
        return true;
    }else{
        return false;
    }
}

// FUNGUJE
char* string_get(string_t* stringPtr){
    INFO;
    return stringPtr->data;
}

// FUNGUJE
bool string_cpy(string_t* destStringPtr, string_t* stringTwoPtr){
    INFO;
    int delkaCopStringu = strlen(stringTwoPtr->data);
    free(destStringPtr->data);
    destStringPtr->data = calloc((delkaCopStringu +1),sizeof(char));
    if(destStringPtr->data == NULL){
        // calloc selhal
        // zahlaš error
        // neřešíš co s tim to si zařídí volající
        // změnit počítadla stringu
        destStringPtr->free_spac = 0;
        destStringPtr->len = 0;
        return false;
    }
    strcpy(destStringPtr->data,stringTwoPtr->data);
    destStringPtr->data[delkaCopStringu]='\0';
    destStringPtr->len = delkaCopStringu;
    destStringPtr->free_spac = 0;
    return true;
}

// Funguje
bool string_cat(string_t* destStringPtr, string_t* stringTwoPtr){
    INFO;

    if(destStringPtr == NULL){
        // chyba - predany NULL
        return false;
    }
    if(stringTwoPtr == NULL){
        // chyba - predany NULL
        return false;
    }
    if(stringTwoPtr->data == NULL && stringTwoPtr->data[0] == '\0'){
        // pridavani prazdneho retezce, nic se nestane a vrati true
        return true;
    }
    int delkaCelehoStringu = strlen(destStringPtr->data) + strlen(stringTwoPtr->data);
    char tempString[delkaCelehoStringu+2];
    strcpy(tempString,destStringPtr->data);
    strcat(tempString,stringTwoPtr->data);
    char *tmp_str = (char*)realloc(destStringPtr->data, delkaCelehoStringu+1);
    if(tmp_str == NULL)
    {
        // Zahlaš chybu
        // Jinak nic neděláš
        return false;
    }
    destStringPtr->data = tmp_str;
    strcpy(destStringPtr->data,tempString);
    destStringPtr->data[delkaCelehoStringu] = '\0';
    destStringPtr->free_spac = 0;
    destStringPtr->len=delkaCelehoStringu;
    return true;
}

// FUNGUJE
void string_dtor(string_t** stringPtr){
    INFO;
    if(*stringPtr == NULL) return;
    free((*stringPtr)->data);
    free(*stringPtr);
    *stringPtr = NULL;
}
