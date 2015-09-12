#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>
#include <assert.h>


/*
 * linked list data structure definitions
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
    int missed = 0;
    char c = ' ';
    char guessed[7] = {'\?'};
    const char * secret = word;
    char * status[sizeof(char) * 2 - 1];
    for(int i = 0;i < strlen(*status);i++){
        if(i % 2 == 0)
            status[i] = "_";
        else
            status[i] = " ";
    }
    while(missed < 7){
        print_gallows(0);
        printf("      %s",*status);
        while(!isalpha(c) || strchr(guessed,c)){
            printf("Please guess a letter.");
            c = toupper(getc(stdin));
        }
        guessed[missed] = c;
        if(!strchr(secret, c)){
            missed++;
        }
        else{
            for(int j = 0;j < strlen(secret); j++){
                if(secret[j] == c){
                    (*status)[j * 2] = c;
                }
            }
        }
        if(!strchr(*status,'_'))
            return true;
    }
    return false;
}

/* 
 * wordbank-related functions. please don't change the
 * function prototypes for these three functions.
 * load_words takes the name of the file that should be
 * opened and words read from, and a pointer to an int
 * that should be indirectly modified to store the number
 * of words loaded from the file.  The function should 
 * return the linked list of words.
 */
wordnode *load_words(const char *filename, int *num_words) {
    char * str = malloc(sizeof(char)*256);
    FILE* file = fopen(filename,"r");
    wordnode * line = malloc(sizeof(wordnode *));
    wordnode * head = malloc(sizeof(wordnode *));
    (*head).next = line;
    while(!feof(file)){
        fgets(str,sizeof(str), file);
        str = strtok(str," /t/n");
        for(int i = 0;i < strlen(str);i++){
            str[i] = toupper(str[i]);
            if(!isalpha(str[i]))
                break;
            if(i == strlen(str) - 1){
                (*num_words)++;
                strncpy((*line).word,str,sizeof((*line).word));
                (*line).next = malloc(sizeof(wordnode *));
                line = (*line).next;
            } // end if
        }// end for
    } // end while
    (*line).next = NULL;
    free(line);
    return head;
}

/*
 * Completely free and deallocate the linked list of words.
 */
void free_words(wordnode *wordlist) {
    wordnode * node = wordlist;
    wordnode * tmp = (*node).next;
    while((*node).next != NULL){
        free(node);
        *node = *(*tmp).next;
        *tmp = *node;
    }
}

/*
 * Choose one random word from the linked list and return it.
 */
const char *choose_random_word(wordnode *wordlist, int num_words) {
    int x = random() % num_words;
    wordnode *tmp = wordlist;
    for(int i = 0;i < x; i++){
        tmp = (*tmp).next;
    }
    return strdup((*tmp).word);
}


/*
 * The main function is where everything starts.  Choose a random
 * word and call one_game.  Start first on getting the word-loading
 * to work correctly, then handle the game playing capabilities.
 */
#ifndef AUTOTEST
int main() {
    printf("sizeof type int = %lu",sizeof(int));
    srandom(time(NULL));
    int num_words = 0;
    wordnode *words = load_words("/usr/share/dict/words", &num_words);
    if (num_words == 0) {
        printf("Didn't load any words?!\n");
        return 0;
    }
    bool game = true;
    while(game){
        const char *word = choose_random_word(words, num_words);
        one_game(word);
        free_words(words);
        printf("Would you like to play again? y/N");
        char tmp = getc(stdin);
        if(tmp != 'Y' && tmp != 'y')
            break;
    }
    return 0;
}
#endif