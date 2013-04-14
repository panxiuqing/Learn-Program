#ifndef PL0_H
#define PL0_H

#define NUM_REMAIN_WORDS 13  //number of remained words
#define MAX_LENGTH_LEXICAL  10 //max length of lexical
#define NUM_SYMBOL  41
#define NUM_ERROR 13
#define MAX_WORDS 1000      //max words to store
#define MAX_NUM_BIT 10      //max bit of number

#define GETCHAR \
    cNow = fgetc(fPointer); \
    pos_in_line++
    

#define ADD_WORD(_wd, _et) \
    allWords[nAllIndex].linenum = linenum;                           \
    strcpy(allWords[nAllIndex].szWord, _wd);                         \
    allWords[nAllIndex].eType = _et; \
    allWords[nAllIndex].pos_in_line = pos_in_line - nWordIndex; \
    nAllIndex++

#define ETYPE_IN_COMPARE(e) e == EQL || e == NEQL || e == LSS || e == LSSEQL || e == GRTR || e == GRTREQL 

enum symbol {
    LOWERC,                       /* [a-z] */
    UPPERC,                       /* [A-Z] */
    DIGITC,                       /* [0-9] */
    UNDERLINE,                    /* [_] */
    BEGINSYM,   //begin
    CALLSYM,    //call
    CONSTSYM,   //const
    DOSYM,      //do
    ENDSYM,     //end
    IFSYM,      //if
    ODDSYM,     //odd
    PROCSYM,    //procedure
    READSYM,    //read
    THENSYM,    //then
    VARSYM,     //var
    WHILESYM,   //while
    WRITESYM,   //write
    IDENTIFY,   //like sDA3F
    INTEGER,     //like 2362
    FLOATNUM,      /* like 12.42 */
    PLUS,       //+
    MINUS,      //-
    TIMES,      //*
    SLASH,      ///
    CONSTDEF,     /* = */
    EQL,        //==
    NEQL,       //#
    LSS,        //<
    LSSEQL,     //<=
    GRTR,       //>
    GRTREQL,    //>=
    LPAREN,     //(
    RPAREN,     //)
    COMMA,      //,
    SEMICOLON,  //;
    PERIOD,     //.
    BECOMES,    //:=
    COLON,        /* : */
    SEPERATER,    /* ' ', '\t', '\n' */
    UNDEFINED,          /* undefined char */
    ENDOFFILE,    /* end of file */
};

enum parsError {
    BAD_END,
    NEED_SEMICOLON,
    NEED_DO,
    NEED_END,
    NEED_NUMBER,
    NEED_THEN,
    NEED_LPAREN,
    NEED_RPAREN,
    NEED_BECOMES,
    NEED_COMPARE,
    NEED_CONSTDEF,
    NEED_IDENTIFY,
    NOT_FACTOR,
};

enum errors {
  OUT_OF_LEXICAL_LENGTH,
};

enum object {
    CONSTANT,
    VARIABLE,
    PROCEDURE,
};

typedef struct {
    int linenum;
    int pos_in_line;
    char szWord[10];
    enum symbol eType;
    float value;
} word_etype_t;

typedef enum parsError error_t;

typedef struct {
    error_t err_type;
    word_etype_t *word;
} err_struct;

FILE *fPointer;
char szWord[MAX_LENGTH_LEXICAL];
int nAllIndex;
word_etype_t allWords[MAX_WORDS];
word_etype_t remainWordMap[NUM_REMAIN_WORDS];
enum symbol eTypeMap[256];
char symStringMap[NUM_SYMBOL][10];

int nErrorIndex;
err_struct allParsingError[MAX_WORDS];
char errorMessage[NUM_ERROR][60];

/* function define */
int print_source_file();
int init();
int lexical_analysis();
int print_lexical_table();
enum symbol getWordType(char *);
int error(enum errors);

int parsing_analysis(int);
int subProgramParsing(int);
int constDefParsing(int);
int oneConstDefParsing(int);
int varDefParsing(int);
int procedureDefParsing(int);
int statementParsing(int);
int expressionParsing(int);
int itemParsing(int);
int factorParsing(int);
int conditionParsing(int);
int parsing_error(enum parsError);
int parsingPrintLn(int, const char *, int);

#endif // PL0_H
