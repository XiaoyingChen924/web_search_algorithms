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
/* main program controls all the action
*/
#include <search_struct.h>


int
main(int argc, char *argv[]) {
    word_t w, para[MAX_PARA_LEN], 
    key_word[MAX_TERMS], highest_snippet[MAX_SNIPPET_LEN];
    int para_count = 0, word_count, num_key_word, n_match;
    
    /* generate an array of key_word and exit if exceed
       MAX_TERMS
    */
    num_key_word = store_key_word(argc, argv, key_word);
    
    while((word_count = get_paragraph(w, para, MAX_PARA_LEN))!=0) {
        /***********Stage_ONE************/
        para_count++;
        printf("\n");
        printf("======= Stage 1 [para %d; %d words]\n\n",
        para_count, word_count);
        /***********Stage_TWO************/
        n_match = num_match(key_word, num_key_word, para, word_count);
        printf("======= Stage 2 [para %d; %d words; %d matches]\n",
        para_count, word_count, n_match);
        print_para(para, word_count, key_word, num_key_word);
        printf("\n\n");
        /***********Stage_THREE************/
        build_snippet(highest_snippet, para, word_count,
				key_word, num_key_word);
    }
    printf("\nta daa!\n");
    return 0; 
}

/****************************************************************/















