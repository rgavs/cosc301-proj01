#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>
#include <assert.h>

// Ryan Gavigan
// COSC 301
// 09/20/15
// Project 1

/*
 * Linked list data structure.
 */
struct _wordnode {
    char word[256];
    struct _wordnode *next;
}; typedef struct _wordnode wordnode;

void print_gallows(int num_missed) {
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
 * Play one game of Hangperson. The secret word is passed as a
 * parameter. The function should return true if the player won,
 * and false otherwise.
 */
bool one_game(const char *word) {
    int num_guesses = 0, num_missed = 0;
	int left = (int) strlen(word);	// num chars left to guess
	bool correct = false; 			// true if current guess is in word, else false
    char status[left * 2];
    status[left * 2] = '\0';
    for(int i = 0; i < left * 2; i++){
    	if(i % 2 == 0)
	    	status[i] = '_';
	    else
	    	status[i] = ' ';
    }	// loop initializes array, alternating spaces/underscores for readability
    char guessed[32] = {'\0'};
    while(num_missed < 7 && left > 0){
        correct = false;
        if(left < strlen(word) || num_missed > 0)
			printf("\nYou have guessed: \'%s\'",guessed);
	    print_gallows(num_missed);
		char guess[128] = {'!'};
		char c = '!';
	    printf("     %s\n\n",status);
		bool again = false;
		do{ // char validation loop
            while(!isalpha(c)){
				printf("Please guess a letter: ");
				fgets(guess, 128, stdin);
				c = toupper(guess[0]);
			}
			for(int k = 0; k < num_guesses + 1; k++){
                if(num_guesses == 0){
                    guessed[num_guesses] = c;
                    num_guesses++;
                    break;
                }
                if(c == guessed[k]){
					again = true;
					printf("You have guessed: \'%s\'\n",guessed);
					printf("Please guess a letter you have not already guessed.");
					fgets(guess, 8, stdin);
					c = toupper(guess[0]);
				}
                if(k == num_guesses && !again){
                    guessed[num_guesses] = c;
                    num_guesses++;
                    break;
                }
			}
		} while(again);
		for(int x = 0; x < strlen(word); x++){
	    	if(word[x] == c){
	    		status[x * 2] = c;
	    		correct = true;
	    		left--;
	    	}
	    }
	    if(!correct)
	    	num_missed++;
    }
    print_gallows(num_missed);
	printf("     %s\n\n",status);
    if(left == 0){
    	printf("Congratulations! You have won!\n\n");
    	return true;
    }
    printf("Not a winner. The word was: \'%s\'",word);
    return false;
}

/*
 * wordbank-related functions. please don't change the
 * function prototypes for these three functions.
 */
wordnode *load_words(const char *filename, int *num_words) {
    FILE* file = fopen(filename, "r");
    wordnode * head = malloc(sizeof(wordnode)), * line = malloc(sizeof(wordnode));
    head->next = line;
    int i;
    char str[256];
    while(!feof(file)){
    	i = 0;
    	fgets(str, sizeof(str), file);
    	strtok(str," \t\n");
        for(; i < strlen(str); i++){    // testing isalpha and setting toupper
            if(!isalpha(str[i]))
                break;
	        str[i] = toupper(str[i]);
        }
        if(i == strlen(str)){           // one-time step to place a word into head->word
	    	(*num_words)++;
        	if(*num_words == 1)
        		strcpy(head->word,str);
        	else{
		        strcpy(line->word,str);
		        line->word[i] = '\0';
		        line->next = malloc(sizeof(wordnode));
		        line = line->next;
		    }
        } // end of if
    } // end of while
   	line = NULL;    // ensures line does not point to a node in the list
    free(line);
    fclose(file);
    return head;
}

void free_words(wordnode *wordlist) {
    wordnode * node = malloc(sizeof(wordnode)), * tmp = malloc(sizeof(wordnode));
    while(node != NULL){
        tmp = node->next;
        free(node);
        node = tmp;
    }
    free(tmp);
    free(node);
}

const char *choose_random_word(wordnode *wordlist, int num_words) {
    int r = random() % num_words;
    int i = 0;
    wordnode * node = wordlist;
    while(i < r){
    	node = node->next;
        i++;
    }
    return node->word;
}

#ifndef AUTOTEST
int main() {
    srandom(time(NULL));
    int num_words = 0;
    wordnode *words = load_words("/usr/share/dict/words", &num_words);
    if (num_words == 0) {
        printf("Didn't load any words?!\n");
        return 0;
    }
    char ans[2];
    while(true){
    	const char *word = choose_random_word(words, num_words);
	    one_game(word);
	    free_words(words);
	    printf("Would you like to play again? ");
	    fgets(ans,sizeof(ans),stdin);
	    if(ans[0] != 'y' && ans[0] != 'Y')
	    	printf("Thanks for playing Hangperson!\n");
	    	break;
	}
    return 0;
}
#endif
