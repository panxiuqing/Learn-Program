#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pl0.h"

int print_lexical_table()
{
    printf("\n--------------------begin lexical analysis---------------------\n");
    int i = 0;
    for (i = 0; i < nAllIndex; i++) {
      printf ("%-6d%-15s%-15s", i, allWords[i].szWord, symStringMap[allWords[i].eType]);
      switch(allWords[i].eType) {
      case INTEGER: printf ("%d\n", (int)allWords[i].value);
        break;
      case FLOATNUM: printf ("%f\n", allWords[i].value);
        break;
      case UNDEFINED: printf ("\t!!! ERROR\n");
        break;
      default: printf ("\n");
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
    unsigned char cNow = ' ';   /* The Char get from fPointer */
    enum symbol etype = UNDEFINED; /* etype of current word */
    int nWordIndex = 0;            /* index of the Char in current word */
    nAllIndex = 0;                 /* index of current word in source file */
    cNow = fgetc(fPointer);

    while (eTypeMap[cNow] != ENDOFFILE) { /* EOF */
        if (eTypeMap[cNow] == SEPERATER) { /* '\n', '\t', ' ' */
            cNow = fgetc(fPointer);
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
                cNow = fgetc(fPointer);
            } while(eTypeMap[cNow] <= UNDERLINE); /* a-z or A-Z or 0-9 or _ */

            szWord[nWordIndex] = 0;     /* end of string */
            strcpy(allWords[nAllIndex].szWord, szWord);  /* copy to global table */
            allWords[nAllIndex++].eType = getWordType(szWord);     /* get etype of word */
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
                cNow = fgetc(fPointer);
            } while(eTypeMap[cNow] == DIGITC); /* get number of integer */
            if (eTypeMap[cNow] == PERIOD) {    /* followed by a period */
              cNow = fgetc(fPointer);
              if (eTypeMap[cNow] == DIGITC) { /* period followed by a digit means float */
                etype = FLOATNUM; /* modify etype to FLOATNUM */
                szWord[nWordIndex++] = '.';
                fnum = num;
                do {
                  count *= 10.0;
                  fnum += (cNow - '0') / count;
                  szWord[nWordIndex++] = cNow;
                  cNow = fgetc(fPointer);
                } while (eTypeMap[cNow] == DIGITC);
              }
              else {            /* period not followed by digit means end of program */
                endp = 1;
              }
            }
            szWord[nWordIndex] = 0;
            strcpy(allWords[nAllIndex].szWord, szWord);
            allWords[nAllIndex].eType = etype;
            allWords[nAllIndex++].value = fnum ? fnum : num;
            if (endp == 1) {
              allWords[nAllIndex].eType = PERIOD;
              strcpy(allWords[nAllIndex].szWord, ".");
            }
            continue;
        }

        if (eTypeMap[cNow] == COLON) { /* : */
            cNow = fgetc(fPointer);
            if (eTypeMap[cNow] == CONSTDEF) { /* := */
                strcpy(allWords[nAllIndex].szWord, ":=");
                allWords[nAllIndex++].eType = BECOMES;
                cNow = fgetc(fPointer);
            }
            else {
                strcpy(allWords[nAllIndex].szWord, ":");
                allWords[nAllIndex++].eType = UNDEFINED;
            }
            continue;
        }

        if (eTypeMap[cNow] == LSS || eTypeMap[cNow] == GRTR || eTypeMap[cNow] == CONSTDEF) { /* <, >, = */
            szWord[nWordIndex++] = cNow;
            szWord[nWordIndex] = 0;
            etype = eTypeMap[cNow];
            cNow = fgetc(fPointer);
            if (eTypeMap[cNow] == CONSTDEF) { /* <=, >=, == */
              szWord[nWordIndex++] = cNow;
              szWord[nWordIndex] = 0;
              etype += 1;
              cNow = fgetc(fPointer);
            }
            strcpy(allWords[nAllIndex].szWord, szWord);
            allWords[nAllIndex++].eType = etype;
            continue;
        }

        else {
            allWords[nAllIndex].szWord[0] = cNow;
            allWords[nAllIndex].szWord[1] = 0;
            allWords[nAllIndex++].eType = eTypeMap[cNow] == UNDERLINE ? UNDEFINED : eTypeMap[cNow];
            cNow = fgetc(fPointer);
            continue;
        }
    }
    print_lexical_table();
    return 0;
}
