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
    /*int i = 0;*/
    /*for (i = 0; i < nErrorIndex; i++) {*/
    /*printf("ERROR at %d:%d: %s\n", allParsingError[i].word->linenum, allParsingError[i].word->pos_in_line, errorMessage[allParsingError[i].err_type]);*/
    /*}*/
    return 0;
}

int parsing_error(error_t e)
{
    /*allParsingError[nErrorIndex].err_type = e;*/
    /*allParsingError[nErrorIndex].word = &allWords[nAllIndex];*/
    /*nErrorIndex++;*/
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
    }
    PARSING_PRINT("END_OF_PROGRAM");
    printParsingError();
    return 0;
}

int subProgramParsing(int nLayer)
{
    if (allWords[nAllIndex].eType == CONSTSYM) {
        PARSING_PRINT("CONSTENT_DEF");
        constDefParsing(nLayer+1);
    }

    if (allWords[nAllIndex].eType == VARSYM) {
        PARSING_PRINT("VARIABLE_DEF");
        varDefParsing(nLayer+1);
    }

    while (allWords[nAllIndex].eType == PROCSYM) {
        PARSING_PRINT("ONE_PROCEDURE_DEF");
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
        PARSING_PRINT("COMMA");
        parsingPrintLn(nLayer, "ONE_CONSTENT_DEF", 0);
        oneConstDefParsing(nLayer+1);
    }
    if (allWords[nAllIndex].eType == SEMICOLON) {
        PARSING_PRINT("SEMICOLON");
    }
    else {
        parsing_error(NEED_SEMICOLON);
    }

    return 0;
}

int oneConstDefParsing(int nLayer)
{
    if (allWords[nAllIndex].eType == IDENTIFY) {
        PARSING_PRINT("IDENTIFY");
        if (allWords[nAllIndex].eType == CONSTDEF) {
            PARSING_PRINT("CONSTDEF");
            if (allWords[nAllIndex].eType == INTEGER || allWords[nAllIndex].eType == FLOATNUM) {
                PARSING_PRINT("NUMBER");
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
        PARSING_PRINT("VARIABLE");
        while (allWords[nAllIndex].eType == COMMA) {
            PARSING_PRINT("COMMA");
            parsingPrintLn(nLayer, "ONE_VARIABLE_DEF", 0);
            if (allWords[nAllIndex].eType == IDENTIFY) {
                PARSING_PRINT("VARIABLE");
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
        PARSING_PRINT("SEMICOLON");
    }
    return 0;
}

int procedureDefParsing(int nLayer)
{
    if (allWords[nAllIndex].eType == IDENTIFY) {
        PARSING_PRINT("IDENTIFY");
        if (allWords[nAllIndex].eType == SEMICOLON) {
            PARSING_PRINT("SEMICOLON");
            subProgramParsing(nLayer+1);
            if (allWords[nAllIndex].eType == SEMICOLON) {
                PARSING_PRINT("SEMICOLON");
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
            PARSING_PRINT("IDENTIFY");
            if (allWords[nAllIndex].eType == BECOMES) {
                PARSING_PRINT("BECOMES");
                expressionParsing(nLayer+1);
                if (allWords[nAllIndex].eType == SEMICOLON) {
                    PARSING_PRINT("SEMICOLON");
                }
                else {
                    parsing_error(NEED_SEMICOLON);
                }
            }
            else {
                parsing_error(NEED_BECOMES);
            }
            break;
        case CALLSYM:
            parsingPrintLn(nLayer-1, "CALL_STATEMENT", 0);
            PARSING_PRINT("CALL");
            if (allWords[nAllIndex].eType == IDENTIFY) {
                PARSING_PRINT("IDENTIFY");
            }
            else {
                parsing_error(NEED_IDENTIFY);
            }
            break;
        case BEGINSYM:
            parsingPrintLn(nLayer-1, "BEGIN_STATEMENT", 0);
            PARSING_PRINT("BEGIN");
            statementParsing(nLayer+1);
            while (allWords[nAllIndex].eType == SEMICOLON) {
                PARSING_PRINT("SEMICOLON");
                statementParsing(nLayer+1);
            }
            if (allWords[nAllIndex].eType == ENDSYM) {
                PARSING_PRINT("END");
            }
            else {
                parsing_error(NEED_END);
            }
            break;
        case IFSYM:
            parsingPrintLn(nLayer-1, "IF_STATEMENT", 0);
            PARSING_PRINT("IF");
            if (allWords[nAllIndex].eType == LPAREN) {
                PARSING_PRINT("LEFT_PAREN");
                conditionParsing(nLayer+1);
                if (allWords[nAllIndex].eType == RPAREN) {
                    PARSING_PRINT("RIGHT_PAREN");
                    statementParsing(nLayer+1);
                    if (allWords[nAllIndex].eType == ELSESYM) {
                        PARSING_PRINT("ELSE");
                        statementParsing(nLayer+1);
                    }
                }
                else {
                    parsing_error(NEED_RPAREN);
                }
            }
            else {
                parsing_error(NEED_LPAREN);
            }
            break;
        case WHILESYM:
            parsingPrintLn(nLayer-1, "WHILE_STATEMENT", 0);
            PARSING_PRINT("WHILE");
            conditionParsing(nLayer+1);
            if (allWords[nAllIndex].eType == DOSYM) {
                PARSING_PRINT("DO");
                statementParsing(nLayer+1);
            }
            else {
                parsing_error(NEED_DO);
            }
            break;
        case READSYM:
            parsingPrintLn(nLayer-1, "READ_STATEMENT", 0);
            PARSING_PRINT("READ");
            if (allWords[nAllIndex].eType == LPAREN) {
                PARSING_PRINT("LEFT_PAREN");
                if (allWords[nAllIndex].eType == IDENTIFY) {
                    PARSING_PRINT("IDENTIFY");
                    while (allWords[nAllIndex].eType == COMMA) {
                        PARSING_PRINT("COMMA");
                        if (allWords[nAllIndex].eType == IDENTIFY) {
                            PARSING_PRINT("IDENTIFY");
                        }
                        else {
                            parsing_error(NEED_IDENTIFY);
                        }
                    }
                    if (allWords[nAllIndex].eType == RPAREN) {
                        PARSING_PRINT("RIGHT_PAREN");
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
            PARSING_PRINT("WRITE");
            if (allWords[nAllIndex].eType == LPAREN) {
                PARSING_PRINT("LEFT_PAREN");
                expressionParsing(nLayer+2);
                while (allWords[nAllIndex].eType == COMMA) {
                    PARSING_PRINT("COMMA");
                    expressionParsing(nLayer+1);
                }
                if (allWords[nAllIndex].eType == RPAREN) {
                    PARSING_PRINT("RIGHT_PAREN");
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
        PARSING_PRINT("ODD");
        expressionParsing(nLayer+1);
    }
    else {
        expressionParsing(nLayer+1);
        if (ETYPE_IN_COMPARE(allWords[nAllIndex].eType)) {
            PARSING_PRINT(symStringMap[allWords[nAllIndex].eType]);
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
        PARSING_PRINT("PLUS");
    }
    else if (allWords[nAllIndex].eType == MINUS) {
        PARSING_PRINT("MINUE");
    }
    itemParsing(nLayer+1);
    while (allWords[nAllIndex].eType == PLUS || allWords[nAllIndex].eType == MINUS) {
        PARSING_PRINT(symStringMap[allWords[nAllIndex].eType]);
        itemParsing(nLayer+1);
    }
    return 0;
}

int itemParsing(int nLayer)
{
    parsingPrintLn(nLayer-1, "ITEM", 0);
    factorParsing(nLayer+1);
    while (allWords[nAllIndex].eType == TIMES || allWords[nAllIndex].eType == SLASH) {
        PARSING_PRINT(symStringMap[allWords[nAllIndex].eType]);
        factorParsing(nLayer+1);
    }
    return 0;
}

int factorParsing(int nLayer)
{
    parsingPrintLn(nLayer-1, "FACTOR", 0);
    switch(allWords[nAllIndex].eType) {
        case IDENTIFY:
            PARSING_PRINT("IDENTIFY");
            break;
        case INTEGER:
        case FLOATNUM:
            PARSING_PRINT("NUMBER");
            break;
        case LPAREN:
            PARSING_PRINT("LEFT_PAREN");
            expressionParsing(nLayer+1);
            if (allWords[nAllIndex].eType == RPAREN) {
                PARSING_PRINT("RIGHT_PAREN");
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

