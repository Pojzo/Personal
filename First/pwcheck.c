#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 

#define min(a, b) ((a < b) ? a : b)
#define max(a, b) ((a > b) ? a : b)

// konstanty k normalnej ulohe
#define DEFAULT_ARGUMENTS 3         // pocet argumentov bez --stats
#define MAX_ARGUMENTS     4         // maximalny pocet argumentov na vstupe
#define BUFFER_LENGTH     100       // maximalna dlzka hesla na vstupe
#define MAX_LEVEL         4         // maximalna hodnota argumentu 'level'
#define STATS_ARGUMENT    "--stats" // volitelny argument
#define NUM_ASCII_CHARACTERS  128

// konstanty k premiovej ulohe 
#define LEVEL_ARGUMENT "-l"      // volitelny prepinac v premiovej ulohe
#define PARAM_ARGUMENT "-p"      // volitelny argument v premiovej ulohe
#define DEFAULT_LEVEL  1         // defaultna hodnota, ak nie je zadany prepinac --level
#define DEFAULT_PARAM  1         // defaultna hodnota, ak nie je zadany prepinac --param

// struct ktory sluzi na ulozenie argumentov z prikazoveho riadku
struct Arguments  {
    unsigned level;
    unsigned param;
    bool stats;
};

// struct ktory sluzi na ukladanie statistik o heslach ak je specifikovany --stats
struct Stats {
    unsigned min_length;
    unsigned num_words;
    unsigned total_length;
    unsigned distinct_chars;
    float average_length;
    bool chars[NUM_ASCII_CHARACTERS];
    /* chars je pole boolov, ktore ma na zaciatku na vsetkych indexoch false
       pocas behu programu nastavujeme chars[char] = true, kde char je charakter zo zadaneho hesla
       napriklad: heslo, vo funkci UpdateStats sa stane -> stats.chars[104] = true;  'h'
                                                           stats.chars[101] = true;  'e'
                                                           stats.chars[115] = true;  's' 
                                                           stats.chars[108] = true;  'l'
                                                           stats.chars[111] = true;  'o'

       na konci iba spocitame kolko boolov z stats.chars je true - pocet roznych znakov v hesle
     */
};


// prototypy funkcii

// Kontrola argumentov z prikazoveho riadku
bool CorrectLevelArgument(const char* level); // funkcia vrati true ak je argument 'level' v spravnom formate 
bool CorrectParamArgument(const char* param); // funkcia vrati true ak je argument 'param' v spravnom formate
bool CorrectStatsArgument(const char* stats);  // funckia vrati true ak je volitelny argument --stats vo spravnom formate 
bool CorrectNumberOfArguments(int argc);      // funkcia vrati true ak je na vstupe spravny pocet argumentov
bool HandleArguments();                       // funkcia vrati true ak su argumenty na vstupe v spravnom formate a so spravnymi hodnotami

struct Arguments CreateArguments(int, const char**);

// Pomocne funkcie 
unsigned StringLength(const char*);               // funkcia vrati dlzku vstupneho stringu
bool StringsEqual(const char*, const char*);      // funkcia vrati true ak sa dva vstupne stringy rovnaju
void RemoveNewLine(char*, unsigned); // funkcia vrati string bez posledneho charakteru


// Kontrola individualnych charakterov
bool CharUpper(char);           // funkcia vrati true ak vstupny char je velke pismeno
bool CharLower(char);           // funkcia vrati true ak vstupny char je male pismeno
bool IsDigit(char);             // funkcia vrati true ak vstupny char je cislica
bool IsSpecialCharacter(char);  // funkcia vrati true ak char je specialny charakter (podla definicie v zadani)

// Kontrola hesla na vstupe
bool ContainsUpperCase(const char*, unsigned);          // funkcia vrati true ak string obsahuje velke pismeno
bool ContainsLowerCase(const char*, unsigned);          // funkcia vrati true ak string obsahuje male pismeno
bool ContainsDigit(const char*, unsigned);              // funkcia vrati true ak string obsahuje cislicu
bool ContainsSpecialCharacter(const char*, unsigned);   // funkcia vrati true ak string obsahuje specialny charakter (podla definicie v zadani)
bool NRepeatingCharacters(const char*, unsigned, unsigned); // funkcia vrati true ak string neobsahuje sekvenciu rovnakych znakov dlzky aspon n

// Kontrola jednotlivych pravidiel
bool FirstRule(const char*, unsigned);            
bool SecondRule(const char*, unsigned, unsigned);
bool ThirdRule(const char*, unsigned, unsigned);
bool FourthRule(const char*, unsigned);

// Statistika
struct Stats CreateStats();                             // funkcia vrati struct stats s inicializovanymi hodnotami
void UpdateStats(struct Stats*, const char*, unsigned); // funkcia aktualizuje statistiku s novym heslom
void CountDistinctCharacters(struct Stats*);            // funkcia spocita pocit roznych charakterov zo vsetkych hesiel a updatne struct stats
void PrintStats(struct Stats);                          // funkcia vypise statistiky

// Hlavny beh programu
bool PasswordPassed(const char*, unsigned, unsigned, unsigned); // funkcia vrati true ak heslo preslo pravidlami
void CheckPasswordsWStats(struct Arguments);                    // funkcia nacita po jednom hesla zo vstupu a updatuje pri tom statistiky 
void CheckPasswordsWOStats(struct Arguments);                   // funckia natica po jednom hesla zo vstupu bez toho aby robil statistiky


int main(int argc, const char **argv) {
    // Ak nieco nie je v poriadku s argumentami z prikazoveho riadku, program ukoncime
    if (!HandleArguments(argc, argv)) {
        return 1;
    }

    struct Arguments arguments = CreateArguments(argc, argv);

    if(arguments.stats) {
        CheckPasswordsWStats(arguments);
        return 0;
    }
    CheckPasswordsWOStats(arguments);
    return 0;
}

bool CorrectLevelArgument(const char* level) {
    // ak ma argument level vacsiu dlzku ako 1, nie je validny.. napr. 14, dfdf, 000
    if (StringLength(level) > 1) {
        printf("Argument 'level' musi mat hodnotu 1 - %d\n", MAX_LEVEL);
        return false;
    }
    int level_int = atoi(level);
    // ak sa argument level nachadza v danom rozmedzi, tak je validny
    if (!((level_int > 0) && (level_int <= MAX_LEVEL))) {
        printf("Argument 'level' musi mat hodnotu 1 - %d\n", MAX_LEVEL);
        return false;
    }
    return true;
}

bool CorrectParamArgument(const char* param) {
    if (param[0] == '-') {
        printf("Param musi byt aspon 1\n");
        return false;
    }
    // iterujeme cez argument param
    for(unsigned i=0; i<StringLength(param); i++) {
        // ak hocijaky charakter v param nie je cislica, tak argument param nie je validny, napr. 1g
        if (!IsDigit(param[i])) {
            printf("Nespravny format argumentu 'param'\n");
            return false;
        }
    }    
    if (atoi(param) < 1) {
        printf("Param by mal byt aspon 1\n");
        return false;
    }
    return true;
}

bool CorrectStatsArgument(const char* stats) {
    // ak volitelny argument == --stats, returne true
    return StringsEqual(stats, STATS_ARGUMENT);
}

bool CorrectNumberOfArguments(int argc) {
    // ak je pocet argumentov mensi ako minimalny/defaulty pocet, tak nepokracujeme a vyhodime error
    if (argc < DEFAULT_ARGUMENTS) {
        printf("Nedostatocny pocet argumentov\n");
        return false;
    } 
    // taktiez ak je vacsi ako maximalny pocet  
    else if (argc > MAX_ARGUMENTS) {
        printf("Prilis velky pocet argumentov\n");
        return false;
    }
    return true;
}

bool HandleArguments(int argc, const char** argv) {
    // ak je na vstupe nespravny pocet argumentov, rovno mozne program ukoncit
    if (!CorrectNumberOfArguments(argc)) {
        return false;
    }
    // ak aspon jeden z argumentov - level, param nema spravny format, vstup nie je spravny
    if (!(CorrectLevelArgument(argv[1]) && CorrectParamArgument(argv[2]))) {
        return false;
    }
    // ak je pocet argumentov maximalny, tak uvazujeme ze user specifikoval volitelny argument --stats)
    if (argc == MAX_ARGUMENTS) {
        if (!(CorrectStatsArgument(argv[3]))) {
            printf("Neznamy volitelny argument %s\n", argv[3]);
            return false;
        }
    }
    return true;
}
struct Arguments CreateArguments(int argc, const char** argv) {
    struct Arguments arguments;
    
    // postupne priradime structu arguments hodnoti z argumentov
    arguments.level = atoi(argv[1]);
    arguments.param  = atoi(argv[2]);
    if (argc == MAX_ARGUMENTS) {
        arguments.stats = true;
    }
    else {
        arguments.stats = false;
    }
    
    return arguments;
}


unsigned StringLength(const char* string) {
    unsigned length = 0;
    // iterujeme cez string, vzdy posuvame pointer na string o jedno dopredu, az kym nie je NULL
    while (*string) {
        ++length;
        ++string;
    }
    return length;
}

bool StringsEqual(const char* s1, const char* s2) {
    unsigned s1_len = StringLength(s1);
    unsigned s2_len = StringLength(s2);
    // ak sa dlzka jedneho stringu nerovna tomu druhemu, tak automaticky sa nemozu rovnat
    if (s1_len != s2_len) {
        return false;
    }
    for(unsigned i=0; i<s1_len; i++) {
        // ak sa stringy nerovnaju v jednom charakteri, ukoncime porovnavanie
        if (s1[i] != s2[i]) {
            return false;
        }
    } 
    return true;
}

void RemoveNewLine(char* string, unsigned string_len) {
    string[string_len -1] = '\0';
}
 
bool CharUpper(char c) {
    return ((c >= 66) && (c <= 90)); 
}

bool CharLower(char c) {
    return ((c >= 97) && (c <= 122));
}

bool IsDigit(char c) {
    return ((c >= 47) && (c <= 58));
}

bool IsSpecialCharacter(char c) {
    return (((c >= 33) && (c < 47)) || ((c > 58) && (c < 66)) || ((c > 90) && (c < 97)) || ((c > 122) && (c < 127)));
}

bool ContainsUpperCase(const char* string, unsigned str_len) {
    // iterujeme cez string na kontrolu kazdeho charakteru
    for(unsigned i=0; i<str_len; i++) {
        if (CharUpper(string[i])) {
            return true;
        }
    }
    return false;
}

bool ContainsLowerCase(const char* string, unsigned str_len) {
    // iterujeme cez string na kontrolu kazdeho charakteru
    for(unsigned i=0; i<str_len; i++) {
        if (CharLower(string[i])) {
            return true;
        }
    }
    return false;
}

bool ContainsDigit(const char* string, unsigned str_len) {
    // iterujeme cez string na kontrolu kazdeho charakteru
    for(unsigned i=0; i<str_len; i++) {
        if (IsDigit(string[i])) {
            return true;
        }
    }
    return false;
}

bool ContainsSpecialCharacter(const char* string, unsigned str_len) {
    // iterujeme cez string na kontrolu kazdeho charakteru
    for(unsigned i=0; i<str_len; i++) {
        if (IsSpecialCharacter(string[i])) {
            return true;
        }
    }
    return false;
}

bool NRepeatingCharacters(const char* string, unsigned str_len, unsigned x) {
    if (str_len == 0) {
        return false;
    }
    if (x == 1) { // ak je parameter 1, tak vsetky hesla, ktore maju dlzku > 0 nie su bezpecne
        return true;
    }
    unsigned longest_seq = 1; // najdlhsia sekvencia na zaciatku musi byt jedna, lebo kontrolujeme od drueho charakteru
    char current_char = string[0]; // aktualny charakter
    // iterujeme cez string na kontrolu kazdeho charakteru
    for(unsigned i=1; i<str_len; i++) {
        if (string[i] == current_char) {
            ++longest_seq; // ak s aktualny znak rovna tomu predchadzajucemu, zvacsime najdlhsiu sekvenciu o 1
        }    
        else {
            longest_seq = 1; // ak sa nerovna, resetujeme obidve premenne
            current_char = string[i];
        }
        if (longest_seq >= x) {
            return true;
        }
    } 
    return false;
}

bool FirstRule(const char* password, unsigned password_len) { // heslo musi obsahovat aspon jedno male a aspon jedno velke pismeno
    return (ContainsUpperCase(password, password_len) && ContainsLowerCase(password, password_len));
}

bool SecondRule(const char* password, unsigned password_len, unsigned x) { // Heslo musi splnat aspon x podmienok specifikovanych v zadani 
    unsigned passed = 0;
    // skontrolujeme vsetky podmienky
    passed = ContainsUpperCase(password, password_len) + ContainsLowerCase(password, password_len) + ContainsDigit(password, password_len) + ContainsSpecialCharacter(password, password_len);
    // ak hesla splna x alebo viac podmienok, tak preslo druhym pravidlom
    return passed >= x;
}

bool ThirdRule(const char* password, unsigned password_len, unsigned x) { // heslo nesmie obsahovat sekvenciu opakujucich sa charakterov aspon dlzky x
    // returnujeme negaciu funkcie NRepeatingCharacters.. ak sa tam nachadza sekvencia x charakterov, tak heslo pravidlom nepreslo
    return (!NRepeatingCharacters(password, password_len, x));
}

/*
bool FourthRule(const char* password, unsigned password_len, unsigned x) {
    return true;
}
*/

bool PasswordPassed(const char* password, unsigned password_length, unsigned level, unsigned param) {
    bool passed = FirstRule(password, password_length);

    if (level >= 2) {
        passed = passed && SecondRule(password, password_length, param);
    }
    if (level >= 3) {
        passed = passed && ThirdRule(password, password_length, param);
    }
    if (level == MAX_LEVEL) {
        passed = passed && true;
    }
    return passed;
}

struct Stats CreateStats() {
    struct Stats stats;
    stats.min_length = 10000;
    stats.num_words = 0;
    stats.total_length = 0;
    for(int i=0; i<NUM_ASCII_CHARACTERS; i++) {
        stats.chars[i] = false;
    }
    return stats;
}

void UpdateStats(struct Stats* stats, const char* password, unsigned password_length) {
    stats->num_words++;
    stats->total_length += password_length;
    stats->min_length = min(stats->min_length, password_length);
    int index;
    for(unsigned i=0; i<password_length; i++) {
        index = password[i];
        stats->chars[index] = true; 
    }
}

void CountDistinctCharacters(struct Stats* stats) {
    for(unsigned i=0; i<NUM_ASCII_CHARACTERS; i++) {
        stats->distinct_chars += stats->chars[i];
    }
}

void PrintStats(struct Stats stats) {
    printf("Statistika:\n");
    printf("Ruznych znaku: %d\n", stats.distinct_chars);
    printf("Minimalni delka: %d\n", stats.min_length);
    printf("Prumerna delka: %.2f\n", stats.average_length);
}

void CheckPasswordsWStats(struct Arguments arguments) {
    struct Stats stats = CreateStats();

    char buffer[BUFFER_LENGTH]; // buffer pre nacitavanie hesiel 
    unsigned password_length;   // dlzka nacitaneho hesla
    bool passed;       // premenna urcujuca ci heslo preslo danym poctom pravidiel
    // pokial je na vstupe heslo, nacitame ho do buffera
    while(fgets(buffer, BUFFER_LENGTH, stdin)) {
        password_length = StringLength(buffer); 
        RemoveNewLine(buffer, password_length); // odstranime z hesla posledny charakter - '\n'

        UpdateStats(&stats, buffer, --password_length); // updateneme statistiky s novym heslom
        passed = PasswordPassed(buffer, password_length, arguments.level, arguments.param); // funkcia PasswordPassed nam povie ci heslo preslo pravidlami
        if (passed) { // ak heslo preslo pravidlami, tak ho vypiseme
            printf("%s\n", buffer);
        }
    }
    // aktualizujeme premiernu dlzku hesla 
    stats.average_length = (double)stats.total_length / (double)stats.num_words;
    CountDistinctCharacters(&stats);
    
    // na zaver vypiseme statistiku
    PrintStats(stats);
}

void CheckPasswordsWOStats(struct Arguments arguments) {  
    char buffer[BUFFER_LENGTH]; // buffer pre nacitavanie hesiel 
    unsigned password_length;   // dlzka nacitaneho hesla
    bool passed;                // premenna urcujuca ci heslo preslo danym poctom pravidiel

    while(fgets(buffer, BUFFER_LENGTH, stdin)) { // nacitame heslo do buffera
        password_length = StringLength(buffer); 
        RemoveNewLine(buffer, password_length); // odstranime z hesla posledny charakter - '\n'
        --password_length;

        passed = PasswordPassed(buffer, password_length, arguments.level, arguments.param); // funkcia PasswordPassed nam povie ci heslo preslo pravidlami
        if (passed) { // ak heslo preslo pravidlami, tak ho vypiseme
            printf("%s\n", buffer);
        }
    }
}

