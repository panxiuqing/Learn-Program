#include <stdio.h>
#include "pl0.h"


int parsingPrintLn(int nLayer, const char *s, int f)
{
    int i = 0;
    if (f) {
        printf("%-6d%-15s", allWords[nAllIndex].linenum, allWords[nAllIndex].szWord);
    }
    else {
        printf("%21s", "---------------------");
    }
    for (i = 0; i < nLayer; i ++) {
        printf("  ");
    }
    printf("[%d]%-20s\n", nLayer, s);
    return 0;
}

int printParsingError()
{
    int i = 0;
    for (i = 0; i < nErrorIndex; i++) {
        printf("ERROR at %d:%d: %s\n", allParsingError[i].word->linenum, allParsingError[i].word->pos_in_line, errorMessage[allParsingError[i].err_type]);
    }
    return 0;
}

int parsing_error(error_t e)
{
    allParsingError[nErrorIndex].err_type = e;
    allParsingError[nErrorIndex].word = &allWords[nAllIndex];
    nErrorIndex++;
    return 0;
}

int parsing_analysis(int e)
{
    nAllIndex = 0;
    int nLayer = 0;
    if (e) {
        return 1;
    }

    subProgramParsing(nLayer);
    if (allWords[nAllIndex].eType != PERIOD) {
        parsing_error(BAD_END);
        seekNextValid(PERIOD);
    }
    parsingPrintLn(nLayer, "END_OF_PROGRAM", 1);
    printParsingError();
    return 0;
}

int subProgramParsing(int nLayer)
{
    if (allWords[nAllIndex].eType == CONSTSYM) {
        parsingPrintLn(nLayer, "CONSTENT_DEF", 1);
        nAllIndex++;
        constDefParsing(nLayer+1);
    }
    
    if (allWords[nAllIndex].eType == VARSYM) {
        parsingPrintLn(nLayer, "VARIABLE_DEF", 1);
        nAllIndex++;
        varDefParsing(nLayer+1);
    }

    while (allWords[nAllIndex].eType == PROCSYM) {
        parsingPrintLn(nLayer, "ONE_PROCEDURE_DEF", 1);
        nAllIndex++;
        procedureDefParsing(nLayer+1);
    }

    statementParsing(nLayer+1);
    return 0;
}

int constDefParsing(int nLayer)
{
    parsingPrintLn(nLayer, "ONE_CONSTENT_DEF", 0);
    oneConstDefParsing(nLayer+1);
    while (allWords[nAllIndex].eType == COMMA) {
        parsingPrintLn(nLayer+1, "COMMA", 1);
        parsingPrintLn(nLayer, "ONE_CONSTENT_DEF", 0);
        nAllIndex++;
        oneConstDefParsing(nLayer+1);
    }
    if (allWords[nAllIndex].eType == SEMICOLON) {
        parsingPrintLn(nLayer, "SEMICOLON", 1);
        nAllIndex++;
    }
    else {
        parsing_error(NEED_SEMICOLON);
        seekNextValid(VARSYM);
    }
    
    return 0;
}

int oneConstDefParsing(int nLayer)
{
    if (allWords[nAllIndex].eType == IDENTIFY) {
        parsingPrintLn(nLayer, "IDENTIFY", 1);
        nAllIndex++;
        if (allWords[nAllIndex].eType == CONSTDEF) {
            parsingPrintLn(nLayer, "CONSTDEF", 1);
            nAllIndex++;
            if (allWords[nAllIndex].eType == INTEGER || allWords[nAllIndex].eType == FLOATNUM) {
                parsingPrintLn(nLayer, "NUMBER", 1);
                nAllIndex++;           
            }
            else {
                parsing_error(NEED_NUMBER);
            }
        }
        else {
            parsing_error(NEED_CONSTDEF);
        }
    }
    else {
        parsing_error(NEED_IDENTIFY);
    }
    return 0;
}

int varDefParsing(int nLayer)
{
    parsingPrintLn(nLayer, "ONE_VARIABLE_DEF", 0);
    if (allWords[nAllIndex].eType == IDENTIFY) {
        parsingPrintLn(nLayer+1, "VARIABLE", 1);
        nAllIndex++;
        while (allWords[nAllIndex].eType == COMMA) {
            parsingPrintLn(nLayer+1, "COMMA", 1);
            parsingPrintLn(nLayer, "ONE_VARIABLE_DEF", 0);
            nAllIndex++;
            if (allWords[nAllIndex].eType == IDENTIFY) {
                parsingPrintLn(nLayer+1, "VARIABLE", 1);
                nAllIndex++;
            }
            else {
                parsing_error(NEED_IDENTIFY);
            }
        }
    }
    else {
        parsing_error(NEED_IDENTIFY);
    }
    if (allWords[nAllIndex].eType == SEMICOLON) {
        parsingPrintLn(nLayer+1, "SEMICOLON", 1);
        nAllIndex++;
    }
    return 0;
}

int procedureDefParsing(int nLayer)
{
    if (allWords[nAllIndex].eType == IDENTIFY) {
        parsingPrintLn(nLayer, "IDENTIFY", 1);
        nAllIndex++;
        if (allWords[nAllIndex].eType == SEMICOLON) {
            parsingPrintLn(nLayer, "SEMICOLON", 1);
            nAllIndex++;
            subProgramParsing(nLayer+1);
            if (allWords[nAllIndex].eType == SEMICOLON) {
                parsingPrintLn(nLayer, "SEMICOLON", 1);
                nAllIndex++;
            }
            else {
                parsing_error(NEED_SEMICOLON);
            }
        }
        else {
            parsing_error(NEED_SEMICOLON);
        }
    }
    return 0;
}

int statementParsing(int nLayer)
{
    switch(allWords[nAllIndex].eType) {
    case IDENTIFY:
        parsingPrintLn(nLayer-1, "ASSIGN_STATEMENT", 0);
        parsingPrintLn(nLayer, "IDENTIFY", 1);
        nAllIndex++;
        if (allWords[nAllIndex].eType == BECOMES) {
            parsingPrintLn(nLayer, "BECOMES", 1);
            nAllIndex++;
            expressionParsing(nLayer+1);
        }
        else {
            parsing_error(NEED_BECOMES);
        }
        break;
    case CALLSYM:
        parsingPrintLn(nLayer-1, "CALL_STATEMENT", 0);
        parsingPrintLn(nLayer, "CALL", 1);
        nAllIndex++;
        if (allWords[nAllIndex].eType == IDENTIFY) {
            parsingPrintLn(nLayer, "IDENTIFY", 1);
            nAllIndex++;
        }
        else {
            parsing_error(NEED_IDENTIFY);
        }
        break;
    case BEGINSYM:
        parsingPrintLn(nLayer-1, "BEGIN_STATEMENT", 0);
        parsingPrintLn(nLayer, "BEGIN", 1);
        nAllIndex++;
        statementParsing(nLayer+1);
        while (allWords[nAllIndex].eType == SEMICOLON) {
            parsingPrintLn(nLayer, "SEMICOLON", 1);
            nAllIndex++;
            statementParsing(nLayer+1);
        }
        if (allWords[nAllIndex].eType == ENDSYM) {
            parsingPrintLn(nLayer, "END", 1);
            nAllIndex++;
        }
        else {
            parsing_error(NEED_END);
        }
        break;
    case IFSYM:
        parsingPrintLn(nLayer-1, "IF_STATEMENT", 0);
        parsingPrintLn(nLayer, "IF", 1);
        nAllIndex++;
        conditionParsing(nLayer+1);
        if (allWords[nAllIndex].eType == THENSYM) {
            parsingPrintLn(nLayer, "THEN", 1);
            nAllIndex++;
            statementParsing(nLayer+1);
        }
        else {
            parsing_error(NEED_THEN);
        }
        break;
    case WHILESYM:
        parsingPrintLn(nLayer-1, "WHILE_STATEMENT", 0);
        parsingPrintLn(nLayer, "WHILE", 1);
        nAllIndex++;
        conditionParsing(nLayer+1);
        if (allWords[nAllIndex].eType == DOSYM) {
            parsingPrintLn(nLayer, "DO", 1);
            nAllIndex++;
            statementParsing(nLayer+1);
        }
        else {
            parsing_error(NEED_DO);
        }
        break;
    case READSYM:
        parsingPrintLn(nLayer-1, "READ_STATEMENT", 0);
        parsingPrintLn(nLayer, "READ", 1);
        nAllIndex++;
        if (allWords[nAllIndex].eType == LPAREN) {
            parsingPrintLn(nLayer, "LEFT_PAREN", 1);
            nAllIndex++;
            if (allWords[nAllIndex].eType == IDENTIFY) {
                parsingPrintLn(nLayer, "IDENTIFY", 1);
                nAllIndex++;
                while (allWords[nAllIndex].eType == COMMA) {
                    parsingPrintLn(nLayer, "COMMA", 1);
                    nAllIndex++;
                    if (allWords[nAllIndex].eType == IDENTIFY) {
                        parsingPrintLn(nLayer, "IDENTIFY", 1);
                        nAllIndex++;
                    }
                    else {
                        parsing_error(NEED_IDENTIFY);
                    }
                }
                if (allWords[nAllIndex].eType == RPAREN) {
                    parsingPrintLn(nLayer, "RIGHT_PAREN", 1);
                    nAllIndex++;
                }
                else {
                    parsing_error(NEED_RPAREN);
                }
            }
            else {
                parsing_error(NEED_IDENTIFY);
            }
        }
        else {
            parsing_error(NEED_LPAREN);
        }
        break;
    case WRITESYM:
        parsingPrintLn(nLayer-1, "WRITE_STATEMENT", 0);
        parsingPrintLn(nLayer, "WRITE", 1);
        nAllIndex++;
        if (allWords[nAllIndex].eType == LPAREN) {
            parsingPrintLn(nLayer, "LEFT_PAREN", 1);
            nAllIndex++;
            expressionParsing(nLayer+2);
            while (allWords[nAllIndex++].eType == COMMA) {
                parsingPrintLn(nLayer, "COMMA", 1);
                nAllIndex++;
                expressionParsing(nLayer+1);
            }
            if (allWords[nAllIndex++].eType == RPAREN) {
                parsingPrintLn(nLayer, "RIGHT_PAREN", 1);
                nAllIndex++;
            }
            else {
                parsing_error(NEED_RPAREN);
            }
        }
        else {
            parsing_error(NEED_LPAREN);
        }
        break;
    default:
        break;
    }       
    return 0;
}

int conditionParsing(int nLayer)
{
    parsingPrintLn(nLayer-1, "CONDITION", 0);
    if (allWords[nAllIndex].eType == ODDSYM) {
        parsingPrintLn(nLayer, "ODD", 1);
        nAllIndex++;
        expressionParsing(nLayer+1);
    }
    else {
        expressionParsing(nLayer+1);
        if (ETYPE_IN_COMPARE(allWords[nAllIndex].eType)) {
            parsingPrintLn(nLayer, symStringMap[allWords[nAllIndex].eType], 1);
            nAllIndex++;
            expressionParsing(nLayer+1);
        }
        else {
            parsing_error(NEED_COMPARE);
        }
    }
    return 0;
}

int expressionParsing(int nLayer)
{
    parsingPrintLn(nLayer-1, "EXPRESSION", 0);
    if (allWords[nAllIndex].eType == PLUS) {
        parsingPrintLn(nLayer, "PLUS", 1);
        nAllIndex++;
    }
    else if (allWords[nAllIndex].eType == MINUS) {
        parsingPrintLn(nLayer, "MINUE", 1);
        nAllIndex++;
    }
    itemParsing(nLayer+1);
    while (allWords[nAllIndex].eType == PLUS || allWords[nAllIndex].eType == MINUS) {
        parsingPrintLn(nLayer, symStringMap[allWords[nAllIndex].eType], 1);
        nAllIndex++;
        itemParsing(nLayer+1);
    }
    return 0;
}

int itemParsing(int nLayer)
{
    parsingPrintLn(nLayer-1, "ITEM", 0);
    factorParsing(nLayer+1);
    while (allWords[nAllIndex].eType == TIMES || allWords[nAllIndex].eType == SLASH) {
        parsingPrintLn(nLayer, symStringMap[allWords[nAllIndex].eType], 1);
        nAllIndex++;
        factorParsing(nLayer+1);
    }
    return 0;
}

int factorParsing(int nLayer)
{
    parsingPrintLn(nLayer-1, "FACTOR", 0);
    switch(allWords[nAllIndex].eType) {
    case IDENTIFY:
        parsingPrintLn(nLayer, "IDENTIFY", 1);
        nAllIndex++;
        break;
    case INTEGER:
    case FLOATNUM:
        parsingPrintLn(nLayer, "NUMBER", 1);
        nAllIndex++;
        break;
    case LPAREN:
        parsingPrintLn(nLayer, "LEFT_PAREN", 1);
        nAllIndex++;
        expressionParsing(nLayer+1);
        if (allWords[nAllIndex].eType == RPAREN) {
            parsingPrintLn(nLayer, "RIGHT_PAREN", 1);
            nAllIndex++;
        }
        else {
            parsing_error(NEED_RPAREN);
        }
        break;
    default:
        parsing_error(NOT_FACTOR);
        break;
    }
    return 0;
}
        
