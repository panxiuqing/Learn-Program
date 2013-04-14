#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pl0.h"

int print_lexical_table()
{
    printf("\n--------------------begin lexical analysis---------------------\n");
    printf ("%-6s%-6s%-15s%-15s%s\n", "LINE", "CHAR", "WORD", "TYPE", "VALUE");
    int i = 0;
    for (i = 0; i < nAllIndex; i++) {
        printf ("%-6d%-6d%-15s%-15s", allWords[i].linenum, allWords[i].pos_in_line, allWords[i].szWord, symStringMap[allWords[i].eType]);
        switch(allWords[i].eType) {
        case INTEGER:
            printf ("%d\n", (int)allWords[i].value);
            break;
        case FLOATNUM:
            printf ("%f\n", allWords[i].value);
            break;
        case UNDEFINED:
            printf("\t!!! ERROR\n");
            break;
        default:
            printf ("\n");
            break;
        }
    }
    printf("\n--------------------end lexical analysis------------------------\n");
}

/* if word in remainwords set to remainword type, else set to identify */
enum symbol getWordType(char *szWord)
{
    int i = 0;
    for (i = 0; i < NUM_REMAIN_WORDS; i++) {
        if (!strcmp(szWord, remainWordMap[i].szWord)) {
            return remainWordMap[i].eType;
        }
    }
    return IDENTIFY;
}

int error(enum errors e)
{
    switch(e) {
    case OUT_OF_LEXICAL_LENGTH: printf("The Word too Length\n");
        break;
    default: break;
    }
    fclose(fPointer);
    exit(1);
}

int lexical_analysis()
{
    int linenum = 1;
    int pos_in_line = 0;
    unsigned char cNow = ' ';   /* The Char get from fPointer */
    enum symbol etype = UNDEFINED; /* etype of current word */
    int nWordIndex = 0;            /* index of the Char in current word */
    nAllIndex = 0;                 /* index of current word in source file */
    GETCHAR;

    while (eTypeMap[cNow] != ENDOFFILE) { /* EOF */
        if (eTypeMap[cNow] == SEPERATER) { /* '\n', '\t', ' ' */
            if (cNow == '\n') {
                pos_in_line = 0;
                linenum++;
            }
            GETCHAR;
            continue;
        }
        nWordIndex = 0;
        /* identify or remain word */
        if (eTypeMap[cNow] <= UPPERC) { /* a-z or A-Z */
            do {
                if (nWordIndex < MAX_LENGTH_LEXICAL) {
                    szWord[nWordIndex++] = cNow;
                }
                else {
                    error(OUT_OF_LEXICAL_LENGTH);
                }
                GETCHAR;
            } while(eTypeMap[cNow] <= UNDERLINE); /* a-z or A-Z or 0-9 or _ */
            szWord[nWordIndex] = 0;     /* end of string */
            ADD_WORD(szWord, getWordType(szWord));
            continue;
        }
        
        /* integer or float */
        if (eTypeMap[cNow] == DIGITC) {
            int num = 0;
            float fnum = 0;
            int count = 1;
            int endp = 0;       /* period not followed by digit */
            etype = INTEGER;
            do {
                num = 10 * num + cNow - '0';
                szWord[nWordIndex++] = cNow;
                GETCHAR;
            } while(eTypeMap[cNow] == DIGITC); /* get number of integer */
            if (eTypeMap[cNow] == PERIOD) {    /* followed by a period */
                GETCHAR;
                if (eTypeMap[cNow] == DIGITC) { /* period followed by a digit means float */
                    etype = FLOATNUM; /* modify etype to FLOATNUM */
                    szWord[nWordIndex++] = '.';
                    fnum = num;
                    do {
                        count *= 10.0;
                        fnum += (cNow - '0') / count;
                        szWord[nWordIndex++] = cNow;
                        GETCHAR;
                    } while (eTypeMap[cNow] == DIGITC);
                }
                else {            /* period not followed by digit means end of program */
                    endp = 1;
                }
            }
            szWord[nWordIndex] = 0;
            allWords[nAllIndex].value = fnum ? fnum : num;
            ADD_WORD(szWord, etype);
            if (endp == 1) {
                ADD_WORD(".", PERIOD);
            }
            continue;
        }
        
        if (eTypeMap[cNow] == COLON) { /* : */
            szWord[nWordIndex++] = ':';
            etype = UNDEFINED;
            GETCHAR;
            if (eTypeMap[cNow] == CONSTDEF) { /* := */
                szWord[nWordIndex++] = '=';
                etype = BECOMES;
                GETCHAR;
            }
            szWord[nWordIndex] = 0;
            ADD_WORD(szWord, etype);
            continue;
        }
        
        if (eTypeMap[cNow] == LSS || eTypeMap[cNow] == GRTR || eTypeMap[cNow] == CONSTDEF) { /* <, >, = */
            szWord[nWordIndex++] = cNow;
            etype = eTypeMap[cNow];
            GETCHAR;
            if (eTypeMap[cNow] == CONSTDEF) { /* <=, >=, == */
                szWord[nWordIndex++] = cNow;
                etype += 1;
                GETCHAR;
            }
            szWord[nWordIndex] = 0;
            ADD_WORD(szWord, etype);
            continue;
        }
        
        else {
            szWord[nWordIndex++] = cNow;
            szWord[nWordIndex] = 0;
            etype = eTypeMap[cNow] == UNDERLINE ? UNDEFINED : eTypeMap[cNow];
            GETCHAR;
            ADD_WORD(szWord, etype);
            continue;
        }
    }
    print_lexical_table();
    return 0;
}
