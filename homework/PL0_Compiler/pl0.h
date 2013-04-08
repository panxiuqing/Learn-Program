#ifndef PL0_H
#define PL0_H

#define NUM_REMAIN_WORDS 13  //number of remained words
#define MAX_LENGTH_LEXICAL  10 //max length of lexical
#define NUM_SYMBOL  41
#define MAX_WORDS 1000      //max words to store
#define MAX_NUM_BIT 10      //max bit of number

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

enum errors {
  OUT_OF_LEXICAL_LENGTH,
};

enum object {
    CONSTANT,
    VARIABLE,
    PROCEDURE,
};

typedef struct {
    char szWord[10];
    enum symbol eType;
    float value;
} word_etype_t;

FILE *fPointer;

char szWord[MAX_LENGTH_LEXICAL];
int nAllIndex;
word_etype_t allWords[MAX_WORDS];
word_etype_t remainWordMap[NUM_REMAIN_WORDS];
enum symbol eTypeMap[256];
char symStringMap[NUM_SYMBOL][10];

/* function define */
int print_source_file();
int init();
int lexical_analysis();
int print_lexical_table();
enum symbol getWordType(char *);
int error(enum errors);

#endif // PL0_H
