/*
 * Dict.h
 *
 *  Created on: Feb 22, 2023
 *      Author: amann
 */

#ifndef TOOLBOX_DICT_H_
#define TOOLBOX_DICT_H_

#include <stdint.h>

typedef struct {
    char *key;
    void *value;
    struct TOOL_Dict *next;
} TOOL_Dict;

/*Create and initialize an empty dictionary. Think carefully about what the single argument to this method should be. This method should allocate the entire dictionary -- including the struct. Be careful here.
This method should return a 0 on success and non-0 on failure*/
int initDict(TOOL_Dict* Dict);

/*Frees all of the dictionary's dynamically allocated memory -- the linked list, the individual words -- and the dictionary struct, itself.
This method should return a 0 on success and non-0 on failure.*/
int destroyDict(TOOL_Dict* Dict);

/*Accepts a word, represented as a char * and add it to the dictionary, which should be passed by reference.
Your initial implementation should probably be via adding new words at the head. But, for your final version, you should maintain the list in sorted order using an insertion sort.

This method should return a 0 on success and non-0 on failure.*/
int addWordDict(TOOL_Dict* Dict);

/*-- Accepts a word, represented as a char * and removes it from the dictionary, which should be passed by reference.
This method should return a 0 on success and non-0 on failure.*/
int removeWordDict(TOOL_Dict* Dict, uint32_t word);

/* -- Accepts a word, represented as a char * and searches the dictionary, passed by reference, for it. If found, this method returns 0. If not found, this method returns 1.*/
int verifyWordDict(TOOL_Dict* Dict, uint32_t word);

/* Accepts an integer representing the index of the word in the dictionary, from the 0th word - the (N - 1)th word. The requested word is communicated to the caller via an argument. Returns 0 upon success and non-0 if the index is out of bounds.*/
int getWordDict(TOOL_Dict* Dict, uint8_t idx);


#endif /* TOOLBOX_DICT_H_ */
