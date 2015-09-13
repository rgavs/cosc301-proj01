#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>
#include <assert.h>

// Ryan Gavigan
// COSC 301
// 09/05/15
// Project 1

/*
 * Linked list data structure definitions.  You can choose
 * to use these data structure and type definitions or modify
 * them to suit your taste.
 */
struct _wordnode {
    char word[256];
    struct _wordnode *next;
};
typedef struct _wordnode wordnode;

/*
 * Be happy you didn't have to write this function.
 * Prints a low-tech ASCII gallows.  Max number of misses
 * is 7 (i.e., if there are 7 misses, the player loses and
 * the poor sap gets hung).
 */
void print_gallows(int num_missed) {
    // make sure that we got called in a reasonable way
    assert(num_missed >= 0 && num_missed <= 7);

    printf("\n\n       |||========|||\n");
    if (num_missed > 0) {
        printf ("       |||         |\n");
    } else {
        printf ("       |||          \n");
    }

    if (num_missed > 1) {
        printf ("       |||         O\n");
    } else {
        printf ("       |||          \n");
    }

    if (num_missed > 2) {
        if (num_missed > 4) {
            printf ("       |||        /|\\\n");
        } else if (num_missed > 3) {
            printf ("       |||        /| \n");
        } else {
            printf ("       |||        /  \n");
        }
    } else {
        printf ("       |||           \n");
    }

    if (num_missed > 5) {
        if (num_missed > 6) {
            printf ("       |||        / \\\n");
        } else {
            printf ("       |||        /  \n");
        }
    } else {
        printf ("       |||           \n");
    }

    printf ("       |||\n");
    printf ("       |||\n");
    printf ("     =================\n\n");
}

/*
 * Play one game of Hangperson.  The secret word is passed as a
 * parameter.  The function should return true if the player won,
 * and false otherwise.
 */
bool one_game(const char *word) {
    bool result = 0;
    int num_missed = 0;
    char *status = malloc((sizeof(char[strlen(word)]) + 1) * sizeof(char) * 2);
    for(int i = 0; i < strlen(status); i++){
    	status[i] = '_'; }
    printf("%s ",word);
    while(result == 0){
	    print_gallows(num_missed);
	    printf("     ");
		for(int i = 0; i < strlen(status); i++){
			printf("%c ",status[i]);
		}
		printf("\n\nPlease guess a character:");
		char guess = toupper(fgetc(stdin));
	    bool in = false;
	    for(int j = 0; j < strlen(status); j++){
	    	if(word[j] == guess){
	    		status[j] = guess;
	    		in = true;
	    		break;
	    	}
	    }
	    if(!in){
	    	num_missed++;
	    }
	    printf("num_missed = %d \n",num_missed);
    }
    return false;
}

/*
 * wordbank-related functions. please don't change the
 * function prototypes for these three functions.
 */
wordnode *load_words(const char *filename, int *num_words) {
    FILE* file = fopen(filename, "r");
    wordnode *head = malloc(sizeof(wordnode));
    //wordnode *prev = malloc(sizeof(wordnode));
    //prev = head;
    //(*prev).next = malloc(sizeof(wordnode));
    wordnode *line = (*head).next;
    wordnode *tmp = line;
    while(!feof(file)){ 
        fgets((*line).word, sizeof((*line).word), file);
        (*num_words)++;
        int i = 0;
        if(strlen((*line).word) == 0){
        	(*num_words)--;
        	break;
        }
        for(; i < strlen((*line).word); i++){
            /*if(strlen((*line).word)){
                break;
            }*/
            if(!isalpha((*line).word[i])){
                (*num_words)--;
                line->next = tmp;
                break;
            }
            line->word[i] = toupper((*line).word[i]);
        }
        line->word[i] = '\0';
        line->next = tmp;
        (*line) = *tmp;
    }
    free(line);
    //free(prev);
    fclose(file);
    return head->next;
}

void free_words(wordnode *wordlist) {
    wordnode *node = wordlist;
    wordnode tmp;
    while((*node).next){
        tmp = *(*node).next;
        free(node);
        *node = tmp;
    }
    free(&node);
}

const char *choose_random_word(wordnode *wordlist, int num_words) {
    int r = random() % num_words;
    printf("r = %d, and num_words = %d\n",r, num_words );
    int i = 0;
    wordnode *node = (*wordlist).next;
    while(i < r){
    	printf("node.word = %s",(*node).word);
        node = (*node).next;
        i++;
    }
    printf("the word is: %s",(*node).word);
    return (*node).word;
}


/*
 * The main function is where everything starts.  Choose a random
 * word and call one_game.  Start first on getting the word-loading
 * to work correctly, then handle the game playing capabilities.
 */
#ifndef AUTOTEST
int main() {
    srandom(time(NULL));
    int num_words = 0;
    wordnode *words = load_words("/usr/share/dict/words", &num_words);
    if (num_words == 0) {
        printf("Didn't load any words?!\n");
        return 0;
    }
    const char *word = choose_random_word(words, num_words);
    printf("the word is: %s",word);
    one_game(word);
    free_words(words);
    return 0;
}
#endif
