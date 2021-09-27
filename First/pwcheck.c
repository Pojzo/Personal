#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 

// konstanty k normalnej ulohe
#define DEFAULT_PARAMS 3   // pocet argumentov bez --stats
#define MAX_ARGUMENTS     4   // maximalny pocet argumentov na vstupe
#define BUFFER_LENGTH  50  // maximalna dlzka hesla na vstupe
#define MAX_LEVEL      4   // maximalna hodnota argumentu 'level'
#define STATS_ARGUMENT "--stats" // volitelny argument

// konstanty k premiovej ulohe 
#define LEVEL_ARGUMENT "-l"      // volitelny prepinac v premiovej ulohe
#define PARAM_ARGUMENT "-p"      // volitelny argument v premiovej ulohe
#define DEFAULT_LEVEL  1         // defaultna hodnota, ak nie je zadany prepinac --level
#define DEFAULT_PARAM  1         // defaultna hodnota, ak nie je zadany prepinac --param

// struct ktory sluzi na ulozenie argumentov z prikazoveho riadku
struct Arguments  {
    unsigned level;
    unsigned param;
};

// prototypy funkcii

// Kontrola argumentov z prikazoveho riadku
bool CorrectLevelArgument(const char* level); // funkcia vrati true ak je argument 'level' v spravnom formate 
bool CorrectParamArgument(const char* param); // funkcia vrati true ak je argument 'param' v spravnom formate
bool CorrectStatsArgument(const char* stats);  // funckia vrati true ak je volitelny argument --stats vo spravnom formate 
bool CorrectNumberOfArguments(int argc);      // funkcia vrati true ak je na vstupe spravny pocet argumentov
bool HandleArguments();                       // funkcia vrati true ak su argumenty na vstupe v spravnom formate a so spravnymi hodnotami

struct Arguments CreateArguments(const char**);

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
bool ContainsUpperCase(const char*, unsigned);        // funkcia vrati true ak vstupny string obsahuje velke pismeno
bool ContainsLowerCase(const char*, unsigned);        // funkcia vrati true ak vstupny string obsahuje male pismeno
bool ContainsDigit(const char*, unsigned);            // funkcia vrati true ak vstupny string obsahuje cislicu
bool ContainsSpecialCharacter(const char*, unsigned); // funkcia vrati ak string obsahuje specialny charakter (podla definicie v zadani)

// Kontrola jednotlivych pravidiel
bool FirstRule(const char*, unsigned);
bool SecondRule(const char*, unsigned, unsigned);
bool ThirdRule(const char*, unsigned);
bool FourthRule(const char*, unsigned);

// Hlavny beh programu
char* ReadPasword(char*); // funkcia vrati jedno heslo (jeden riadok) zo vstupu
void CheckPasswords();    // funkcia cita po jednom riadku hesla zo vstupu a kontroluje ich podla argumentov z prikazoveho riadku

int main(int argc, const char **argv) {
    // Ak nieco nie je v poriadku s argumentami z prikazoveho riadku, program ukoncime
    if (!HandleArguments(argc, argv)) {
        return 1;
    }

    struct Arguments arguments = CreateArguments(argv);

    CheckPasswords(arguments);

}

bool CorrectLevelArgument(const char* level) {
    // ak ma argument level vacsiu dlzku ako 1, nie je validny.. napr. 14, dfdf, 000
    if (StringLength(level) > 1) {
        printf("Argument 'level' musi mat hodnotu 1 - %d\n", MAX_LEVEL);
        return false;
    }
    int level_int = atoi(level);
    // ak sa argument level nachadza v danom rozmedzi, tak je validny
    if (!((level_int > 0) && (level_int < MAX_LEVEL))) {
        printf("Argument 'level' musi mat hodnotu 1 - %d\n", MAX_LEVEL);
        return false;
    }
    return true;
}

bool CorrectParamArgument(const char* param) {
    // iterujeme cez argument param
    for(unsigned i=0; i<StringLength(param); i++) {
        // ak hocijaky charakter v param nie je cislica, tak argument param nie je validny, napr. 1g
        if (!IsDigit(param[i])) {
            printf("Nespravny format argumentu 'param'\n");
            return false;
        }
    }    
    return true;
}

bool CorrectStatsArgument(const char* stats) {
    // ak volitelny argument == --stats, returne true
    return StringsEqual(stats, STATS_ARGUMENT);
}

bool CorrectNumberOfArguments(int argc) {
    // ak je pocet argumentov mensi ako minimalny/defaulty pocet, tak nepokracujeme a vyhodime error
    if (argc < DEFAULT_PARAMS) {
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
struct Arguments CreateArguments(const char** argv) {
    struct Arguments arguments;
    
    // postupne priradime structu arguments hodnoti z argumentov
    arguments.level = atoi(argv[1]);
    arguments.param  = atoi(argv[2]);
    
    return arguments;
}

unsigned StringLength(const char* string) {
    unsigned length = 0;
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
    for(unsigned i=0; i<str_len; i++) {
        if (CharUpper(string[i])) {
            return true;
        }
    }
    return false;
}

bool ContainsLowerCase(const char* string, unsigned str_len) {
    for(unsigned i=0; i<str_len; i++) {
        if (CharLower(string[i])) {
            return true;
        }
    }
    return false;
}

bool ContainsDigit(const char* string, unsigned str_len) {
    for(unsigned i=0; i<str_len; i++) {
        if (IsDigit(string[i])) {
            return true;
        }
    }
    return false;
}

bool ContainsSpecialCharacter(const char* string, unsigned str_len) {
    for(unsigned i=0; i<str_len; i++) {
        if (IsSpecialCharacter(string[i])) {
            return true;
        }
    }
    return false;
}

bool FirstRule(const char* password, unsigned password_len) {
    return (ContainsUpperCase(password, password_len) && ContainsLowerCase(password, password_len));
}

bool SecondRule(const char* password, unsigned password_len, unsigned x) {
    unsigned passed = 0;
    passed = ContainsUpperCase(password, password_len) + ContainsLowerCase(password, password_len) + ContainsDigit(password, password_len) + ContainsSpecialCharacter(password, password_len);
    return passed >= x;
}

void Print(bool upper, bool lower, bool digit, bool special) {
    printf("Upper case: %s\n", upper ? "True" : "False");
    printf("Lower case: %s\n", lower ? "True" : "False");
    printf("Digit: %s\n", digit ? "True" : "False");
    printf("Special: %s\n", special ? "True" : "False");
}

void RemoveNewLine(char* string, unsigned string_len) {
    string[string_len -1] = '\0';
}

void CheckPasswords(struct Arguments arguments) {
    char buffer[BUFFER_LENGTH];
    unsigned password_length;
    bool password_passed;
    while(fgets(buffer, BUFFER_LENGTH, stdin)) {
        password_length = StringLength(buffer); // dlzka hesla na vstupe
        RemoveNewLine(buffer, password_length); // odstranime z hesla posledny charakter - '\n'
        password_length = StringLength(buffer); // dlzka hesla na vstupe

        password_passed = FirstRule(buffer, password_length);

        if (arguments.level >= 2) {
            password_passed = password_passed && SecondRule(buffer, password_length, arguments.param);
        }
        if (password_passed) {
            printf("%s\n", buffer);
        }
    }
}
