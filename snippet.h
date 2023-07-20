/* Program to generate term-biased snippets for paragraphs of text.

   Skeleton program written by Alistair Moffat, ammoffat@unimelb.edu.au,
   August 2022, with the intention that it be modified by students
   to add functionality, as required by the assignment specification.

   Student Authorship Declaration:

   (1) I certify that except for the code provided in the initial skeleton
   file, the  program contained in this submission is completely my own
   individual work, except where explicitly noted by further comments that
   provide details otherwise.  I understand that work that has been developed
   by another student, or by me in collaboration with other students, or by
   non-students as a result of request, solicitation, or payment, may not be
   submitted for assessment in this subject.  I understand that submitting for
   assessment work developed by or in collaboration with other students or
   non-students constitutes Academic Misconduct, and may be penalized by mark
   deductions, or by other penalties determined via the University of
   Melbourne Academic Honesty Policy, as described at
   https://academicintegrity.unimelb.edu.au.

   (2) I also certify that I have not provided a copy of this work in either
   softcopy or hardcopy or any other form to any other student, and nor will I
   do so until after the marks are released. I understand that providing my
   work to other students, regardless of my intention or any undertakings made
   to me by that other student, is also Academic Misconduct.

   (3) I further understand that providing a copy of the assignment
   specification to any form of code authoring or assignment tutoring service,
   or drawing the attention of others to such services and code that may have
   been made available via such a service, may be regarded as Student General
   Misconduct (interfering with the teaching activities of the University
   and/or inciting others to commit Academic Misconduct).  I understand that
   an allegation of Student General Misconduct may arise regardless of whether
   or not I personally make use of such solutions or sought benefit from such
   actions.

   Signed by: Xiaoying Chen 1346651
   Dated:     6/09/2022

*/
/**************************************************************/

/**
 * Declaration: This project surves the puropose of demonstrating understanding
 * of arrays, strings, functions and types as an assignment for the course
 * COMP10002 Foundations of Algorithms in Semester 2, 2022
*/

/** 
 * Header for module which specifies the rules for creating the 
 * term-biased snippets for paragraphs of text
*/

#ifndef SNIPPET_H
#define SNIPPET_H

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

#define MATCHING_WORD 1
    /* the input word is a matching word */
#define MATCHING_WITH_PUNC 2
    /* the input word is a punctuated matching word */
#define CHAR " "
    /* insertion characters to seperate word from word */
#define CHAR_LEN (1*strlen(CHAR))
    /* length of char */
typedef char word_t[MAX_WORD_LEN+1];


/* extract a single word out of the standard input, but not
   more than "limit" characters in total. One character of
   sensible trailing punctuation is retained.  
   argument array W must be limit+1 characters or bigger
*/
int     get_word(word_t w, int limit);







/**If you wish to add further #defines, put them below this comment,
   then prototypes for the functions that you add

   The only thing you should alter above this line is to complete the
   Authorship Declaration 
*/


/* count the number of words in each paragraph generate
   an array "para" which build up a parargaph of words 
*/
int     get_paragraph(word_t w, word_t para[], int limit);

/*Print out the paragraphs in the required format by keeping track of
  the character length of line, as well as determining whether a 
  word is a key word by calling is_key_word function
*/
void    print_para(word_t para[], int limit, word_t key_word[], int num_kw);

/* Read in and store the key words in the array key_word 
   for later comparison and return the number of key word found
*/
int     store_key_word(int argc, char *argv[], word_t key_word[]);

/* is_key_word return a positive value if the word passed
   in is a key word (query term). The returned positive 
   value can be either MATCHING_WORD or MATCHING_WITH_PUNC
*/
int     is_key_word(word_t key_word[], int num_key_word, word_t w);

/* loop through the paragrpah and return the number of matches 
   found in each paragraph 
*/
int     num_match(word_t key_word[], int num_key_word, word_t para[], int n);

/* Print out the snippet with the highest score and control other 
   functions in stage three
*/
int     build_snippet(word_t h_snippet[], word_t para[], int, word_t[], int);

/* Construct the snippet (current) and return the score for this snippet
*/
double  calc_score(word_t[], word_t[], int s, int e, word_t key_w[], int);

/* upadte highest_snippet and return the buddy variable
   which is the lenght of highest_snippet
*/
int     change_snippet(word_t snippet[], word_t hi_snippet[], int snip_len);

/* Find out whether a word is punctuated 
*/
int     is_punctuated(word_t w);

#endif 