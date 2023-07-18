#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h> 
#include <strings.h>

#include <search_struct.h>

/* extract a single word out of the standard input, but not
   more than "limit" characters in total. One character of
   sensible trailing punctuation is retained.  
   argument array W must be limit+1 characters or bigger
*/
int
get_word(word_t W, int limit) {
    int c, i = 0;

    //Determine whether it is a PARA_SEPARATOR
    while ((c=getchar())!=EOF && !isalnum(c)) {
        if (c==PARA_SEPARATOR[i]) {
            i++;
            if (i==strlen(PARA_SEPARATOR)) {
                return PARA_END; 
            }
            continue; 
        }
    }
    if (c==EOF) {
        return EOF;
    }
    /* ok, first character of next word has been found */
    if (isalnum(c)) {
        *W = c;
        W += 1;
        limit -= 1;
    }
    while (limit>0 && (c=getchar())!=EOF && isalnum(c)) {
        /* another character to be stored */
        *W = c;
        W += 1;
        limit -= 1;
    }
    /* take a look at that next character, is it a sensible trailing
       punctuation? */
    if (strchr(TERM_PUNCT, c) && (limit>0)) {
        /* yes, it is */
        *W = c;
        W += 1;
        limit -= 1;
    }
    /* now close off the string */
    *W = '\0';
    return WORD_FND;
}


/* count the number of words in each paragraph generate
   an array "para" which build up a parargaph of words 
*/
int get_paragraph(word_t w, word_t para[], int limit) {
    int word_count = 0;
    int return_type;
    
    /*word_count increase by one every time a word has been successfully 
      read in */
    while ((return_type = get_word(w, MAX_WORD_LEN))==WORD_FND && limit > 0) {
        strcpy(para[word_count], w);
        word_count++;
        limit--;
    } 
    /*return the word_count of para when PARA_END or EOF found*/
    return word_count;
}


/************************Stage Two Functions*************************/

/*Print out the paragraphs in the required format by keeping track of
  the character length of line, as well as determining whether a 
  word is a key word by calling is_key_word function
*/
void    
print_para(word_t para[], int word_count, word_t key_word[], int num_key_word) {
    int char_len_count = 0, match = 0, word_len;
    
    for (int i = 0; i < word_count; i++) {
        /* is_key_word return a positive value if the word passed
           in is a key word (query term). The returned positive 
           value can be either MATCHING_WORD or MATCHING_WITH_PUNC
        */
        match = is_key_word(key_word, num_key_word, para[i]);
        //Case 1: It is a key word
        if (match!=NO_MATCH) {
            //need new line character as exceed max_len
            if ((char_len_count+strlen(para[i])+BBOLD_LEN) > MAX_OUTPUT_LINE) {
                printf("\n");   
                char_len_count = strlen(para[i]) + BBOLD_LEN + CHAR_LEN;
            //don't need new line 
            } else {
                if(i!=0)
                    printf("%s", CHAR);
                char_len_count += strlen(para[i]) + BBOLD_LEN + CHAR_LEN;
            }
            // Print the exact matching word
            if (match==MATCHING_WORD) {
                printf("%s%s%s", BBOLD, para[i], BBOLD);
            // Print the matching word with a punc at the end 
            } else {
                printf("%s", BBOLD);
                word_len = strlen(para[i]);
                //Print individual char except for the punc
                for (int j=0; j<word_len-1; j++) {
                    printf("%c", para[i][j]);
                }
                //Print the rest of the formating char
                printf("%s%c", BBOLD, para[i][word_len-1]);
            } 
            
        //Case2: It is not a key word (NO_MATCH)
        } else {
            if ((char_len_count + strlen(para[i])) > MAX_OUTPUT_LINE) {
                printf("\n");
                char_len_count = strlen(para[i]) + CHAR_LEN;
            } else {
                if(i!=0)
                    printf("%s", CHAR);
                char_len_count += strlen(para[i]) + CHAR_LEN;
            }
            printf("%s", para[i]);
        }
    }
}



/* Read in and store the key words in the array key_word 
   for later comparison and return the number of key word found
*/
int store_key_word(int argc, char *argv[], word_t key_word[]) {
    int number_key_word = 0;
    if (argc > MAX_TERMS)
        exit(EXIT_FAILURE);
    for (int i=1; i<argc; i++) {
        number_key_word++;
        strcpy(key_word[i-1], argv[i]);
    } 
    return number_key_word;
}


/* is_key_word return a positive value if the word passed
   in is a key word (query term). The returned positive 
   value can be either MATCHING_WORD or MATCHING_WITH_PUNC
*/
int     
is_key_word(word_t key_word[], int num_key_word, word_t w) {
    for (int i=0; i<num_key_word; i++) {
        if (!strncasecmp(key_word[i], w, strlen(key_word[i]))) {
            // exact matching word 
            if (strlen(key_word[i])==strlen(w))
                return MATCHING_WORD;
            // matching with a punct at the end 
            if (strchr(TERM_PUNCT, w[strlen(key_word[i])+1]))
                return MATCHING_WITH_PUNC;
        }
    }
    return NO_MATCH;
}


/* loop through the paragrpah and return the number of matches 
   found in each paragraph 
*/
int
num_match(word_t key_word[], int num_key_word, word_t para[], int word_count) {
    int n_match = 0;
    for (int i=0; i<word_count;i++) {
        if (is_key_word(key_word, num_key_word, para[i])!=NO_MATCH)
            n_match++;
    }
    return n_match;
}



/************************Stage Three Functions*************************/

/* Print out the snippet with the highest score and control other 
   functions in stage three
*/
int  
build_snippet(word_t highest_snippet[], word_t para[], int word_count, 
		  word_t key_word[], int n_key_w) {
    static int para_count = 0;
    double score = 0, max_score = 0;
    int snip_len, hi_snip_len, hi_start = 0;
    word_t snippet[MAX_SNIPPET_LEN];

    //Case one: the paragraph has less than MIN_SNIPPET_LEN (20 words)
    //return straight away
    if (word_count < MIN_SNIPPET_LEN) {
        hi_start = word_count;
        max_score = calc_score(snippet, para, 0, word_count, key_word, n_key_w);
        hi_snip_len = change_snippet(snippet, highest_snippet, word_count);
        
    } else {
    //Case two: the paragrpah has more than MIN_SNIPPET_LEN (20 words) 
        for (int start=0; start<=word_count-MIN_SNIPPET_LEN; start++) {
        /* Calculate the score for each combinition of snippet, if the 
           score for the current snippet is higher than max_score, 
           replace hi_snippet with the current score and update
           max_score as well as start that give max_score*/
            
            //If the length of paragraph is higher than MAX_SNIPPET_LEN
            if (start + MAX_SNIPPET_LEN > word_count) {
                for(int end=start+MIN_SNIPPET_LEN; end<=word_count; end++) {
                    snip_len = end - start;
                    //calc score 
                    score = calc_score(snippet, para, start, end, 
                    	key_word, n_key_w);
                    if (score > max_score) {
                        //update if current score is higher than max_score
                        max_score = score; 
                        hi_start = start;
                        hi_snip_len = change_snippet(snippet, 
                        	highest_snippet, snip_len);
                    }
                }
            } else {
                for(int end=start+MIN_SNIPPET_LEN; 
                    end<=start+MAX_SNIPPET_LEN; end++) {
                    snip_len = end - start;
                    //calc score
                    score = calc_score(snippet, para, start,
						end, key_word, n_key_w);
                    if (score > max_score) {
                        //update if current score is higher than max_score
                        max_score = score;
                        hi_start = start;
                        hi_snip_len = change_snippet(snippet, 
							highest_snippet, snip_len);
                    }
                }
            }
        }
    }
    para_count++;
    printf("======= Stage 3 [para %d; start %d; length %d; score %.2f]\n",
    	para_count, hi_start, hi_snip_len, max_score);
    print_para(highest_snippet, hi_snip_len, key_word, n_key_w);
    /* Add dots if the last word is punctuated and it needs dot 
       or it does not end with a punctuated word
    */
    if (is_punctuated(highest_snippet[hi_snip_len - 1])) {
        if (strchr(NEEDS_DOTS, highest_snippet[hi_snip_len - 1]
        	[strlen(highest_snippet[hi_snip_len - 1]) - 1]))
            printf("%s", DDOTS); 
    } else {
        printf("%s", DDOTS); 
    }
    printf("\n");
    return 0;
}


/* Construct the snippet (current) and return the score for this snippet
*/
double 
calc_score(word_t snippet[], word_t para[], int start, 
		   int end, word_t key_word[], int n_key_w) {
    double score = 0, exceed_len;
    int visited_key_word = 0, word_len = 0;
    word_t key_term_found[n_key_w];
    
    //Add 15/(s+10)pts, where s is the start word of snippet
    score += 15.0 / (start + 10);
    //Add 0.6 if the snippet start with the word following a punctuated word
    if (start == 0) {
        score += 0.6;
    } else {
        if (is_punctuated(para[start-1])) {
            score += 0.6;
        }
    }
    //Add 0.3 points if the snippet ends with a punctuated word
    if (is_punctuated(para[end-1])) {
        score += 0.3;
    }
    //Subtract 0.1 points for wd exceeds MIN SNIPPET LEN in length
    exceed_len = (end-start) - MIN_SNIPPET_LEN;
    score -= 0.1 * exceed_len;
    
    /*Add one for every repetition of key word 
      Add l/2 for every time reading in a new key word
    */
    for (int i=start; i<end; i++) {
        strcpy(snippet[i-start], para[i]);
        if (is_key_word(key_word, n_key_w, para[i])!=NO_MATCH) {
            //If its repeated
            if (is_key_word(key_term_found, 
            	visited_key_word, para[i])!=NO_MATCH) {
                score += 1;
                
            //If its a new key_word
            } else {
                /* If the new key word is punctuated, remove 
                   punctuation before storing into the key_term_found
                   (int) visited_key_word counts the number for key 
                   word that has been found
                */
                if (is_punctuated(para[i])) {
                    word_len = strlen(para[i]) - 1;
                    score += word_len / 2.0;
                    for (int j=0; j<word_len; j++) {
                        key_term_found[visited_key_word][j] = para[i][j];
                    }
                    key_term_found[visited_key_word][word_len] = '\0';
                /* If not punctuated, copy across*/
                } else {
                    score += strlen(para[i]) / 2.0;
                    strcpy(key_term_found[visited_key_word], para[i]);
                }
                visited_key_word++;
            }
        }
    }
    return score;
}



/* upadte highest_snippet and return the buddy variable
   which is the lenght of highest_snippet
*/
int
change_snippet(word_t snippet[], word_t highest_snippet[], int snip_len) {
    for (int i=0; i<snip_len;i++) {
        strcpy(highest_snippet[i], snippet[i]);
    }
    return snip_len;
}



/* Find out whether a word is punctuated 
*/
int is_punctuated(word_t w) {
    if (strchr(TERM_PUNCT, w[strlen(w) - 1]))
        return 1;
    return 0;
}


//Algorithms are fun!
                