#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>

#include <search_functions.h>

    /* maximum number of characters per word */
#define MAX_WORD_LEN 23
    /* maximum number of words per paragraph */
#define MAX_PARA_LEN 10000

    /* return code from get_word if end of paragraph found */
#define PARA_END 1
    /* return code from get_word if a word successfully found */
#define WORD_FND 2

    /* terminating punctuation that may follow a word */
#define TERM_PUNCT ".,;:!?"
    /* terminating punctuation that needs dots added */
#define NEEDS_DOTS ",;:"
    /* the string the separates paragraphs */
#define PARA_SEPARATOR "\n\n"
    /* insertion characters to indicate "bold" */
#define BBOLD "**"
    /* length of those characters when wrapped around a word */
#define BBOLD_LEN (2*strlen(BBOLD))
    /* insertion characters to indicate omitted text */
#define DDOTS "..."

    /* maximum words in any output snippet */
#define MAX_SNIPPET_LEN 30
    /* minimum words in any output snippet */
#define MIN_SNIPPET_LEN 20
    /* maximum length of output lines */
#define MAX_OUTPUT_LINE 72

    /* maximum terms allowed on command line */
#define MAX_TERMS 50
	/* signal for no match between word and query term */
#define NO_MATCH (-1)

typedef char word_t[MAX_WORD_LEN+1];
int     get_word(word_t w, int limit);

/* If you wish to add further #defines, put them below this comment,
   then prototypes for the functions that you add

   The only thing you should alter above this line is to complete the
   Authorship Declaration 
*/
#define MATCHING_WORD 1
    /* the input word is a matching word */
#define MATCHING_WITH_PUNC 2
    /* the input word is a punctuated matching word */
#define CHAR " "
    /* insertion characters to seperate word from word */
#define CHAR_LEN (1*strlen(CHAR))
    /* length of char */

int     get_paragraph(word_t w, word_t para[], int limit);
void    print_para(word_t para[], int limit, word_t key_word[], int num_kw);
int     store_key_word(int argc, char *argv[], word_t key_word[]);
int     is_key_word(word_t key_word[], int num_key_word, word_t w);
int     num_match(word_t key_word[], int num_key_word, word_t para[], int n);
int     build_snippet(word_t h_snippet[], word_t para[], int, word_t[], int);
double  calc_score(word_t[], word_t[], int s, int e, word_t key_w[], int);
int     change_snippet(word_t snippet[], word_t hi_snippet[], int snip_len);
int     is_punctuated(word_t w);
/****************************************************************/