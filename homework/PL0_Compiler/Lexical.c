#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pl0.h"

int print_lexical_table()
{
    printf("--------------------begin---------------------\n");
    int i = 0;
    for (i = 0; i < nAllIndex; i++) {
        if (allWords[i].eType == INTEGER) {
          printf("%-6d%-15s%-15s%d\n", i, allWords[i].szWord, "INTEGER", (int)allWords[i].value);
        }
        else if (allWords[i].eType == FLOATNUM) {
          printf("%-6d%-15s%-15s%f\n", i, allWords[i].szWord, "FLOAT", allWords[i].value);
        }
        else {
            printf("%-6d%-15s%-15s\n", i, allWords[i].szWord, symStringMap[allWords[i].eType]);
        }
    }
    printf("--------------------end------------------------\n");
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

int error(enum symbol s)
{
  switch(s) {
  case UNDEFINED: printf("Get an undefined charactor\n");
    break;
  default: break;
  }
  fclose(fPointer);
  exit(1);
}

int lexical_analysis()
{
    unsigned char cNow = ' ';
    enum symbol etype = UNDEFINED;
    int nWordIndex = 0, num = 0;
    nAllIndex = 0;
    cNow = fgetc(fPointer);

    while (eTypeMap[cNow] != ENDOFFILE) {
        if (eTypeMap[cNow] == SEPERATER) {
            cNow = fgetc(fPointer);
            continue;
        }
        nWordIndex = 0;

        if (eTypeMap[cNow] <= UPPERC) {
            do {
                if (nWordIndex < MAX_LENGTH_LEXICAL) {
                    szWord[nWordIndex++] = cNow;
                }
                cNow = fgetc(fPointer);
            } while(eTypeMap[cNow] <= UNDERLINE);

            szWord[nWordIndex] = 0;     //end of string
            strcpy(allWords[nAllIndex].szWord, szWord);  //copy to global table
            allWords[nAllIndex++].eType = getWordType(szWord);     //assign etype to word
            continue;
        }

        if (eTypeMap[cNow] == DIGITC) {
            int num = 0;
            float fnum = 0;
            int count = 1;
            allWords[nAllIndex].eType = INTEGER;
            do {
                num = 10 * num + cNow - '0';
                szWord[nWordIndex++] = cNow;
                cNow = fgetc(fPointer);
            } while(eTypeMap[cNow] == DIGITC);
            if (eTypeMap[cNow] == PERIOD) {
              cNow = fgetc(fPointer);
              if (eTypeMap[cNow] == DIGITC) {
                allWords[nAllIndex].eType = FLOATNUM;
                szWord[nWordIndex++] = '.';
                fnum = num;
                do {
                  count *= 10.0;
                  fnum += (cNow - '0') / count;
                  szWord[nWordIndex++] = cNow;
                  cNow = fgetc(fPointer);
                } while (eTypeMap[cNow] == DIGITC);
              }
              else {
                allWords[nAllIndex].eType = PERIOD;
                strcpy(allWords[nAllIndex++].szWord, ".");
                continue;
              }
            }
            szWord[nWordIndex] = 0;
            strcpy(allWords[nAllIndex].szWord, szWord);

            if (nWordIndex > MAX_NUM_BIT) {
            }
            allWords[nAllIndex++].value = fnum ? fnum : num;
            continue;
        }

        if (eTypeMap[cNow] == COLON) {
            cNow = fgetc(fPointer);
            if (eTypeMap[cNow] == CONSTDEF) {
                strcpy(allWords[nAllIndex].szWord, ":=");
                allWords[nAllIndex++].eType = BECOMES;
                cNow = fgetc(fPointer);
            }
            else {
                allWords[nAllIndex++].eType = UNDEFINED;
                error(UNDEFINED);
            }
            continue;
        }

        if (eTypeMap[cNow] == LSS || eTypeMap[cNow] == GRTR || eTypeMap[cNow] == CONSTDEF) {
            szWord[nWordIndex++] = cNow;
            szWord[nWordIndex] = 0;
            etype = eTypeMap[cNow];
            cNow = fgetc(fPointer);
            if (eTypeMap[cNow] == CONSTDEF) {
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
            allWords[nAllIndex++].eType = eTypeMap[cNow];
            cNow = fgetc(fPointer);
            continue;
        }
    }
    print_lexical_table();
    return 0;
}
