/* Thi Tran
 * RedID: 825223653
 * CS 480 - Operating Systems
 * Professor: Ben Shen
 * Assignment 2 - POSIX Threads
 */

#ifndef DICTTREE_H
#define DICTTREE_h
#define NUMOfCHARS 27 /* a-z plus ' */
#include <iostream>


using namespace std;

struct dictNode
{


    // isWord isTRUE if the node represents
    // the last character of a word
    bool isWord;

    // Collection of nodes that represent subsequent characters in
    // words. Each node in the next dictNode*
    // array corresponds to a particular character that continues
    // the word if the node at the array index is NOT NULL:
    // e.g., if next[0] is NOT NULL, it means ‘a’ would be
    // one of the next characters that continues the current word;
    // while next[0] being NULL means there are no further words
    // with the next letter as ‘a’ along this path.
    struct dictNode *next[NUMOfCHARS];


};

// Declare all functions needed


/* Constructor to create a new node for building the dictionary tree */
struct dictNode *createANewNode();

/* Function to ignore case-sensitivity */
int indexOfChar(char);

/* Function to add node for building the trie */
void insert(struct dictNode *, string);

/* Function to search words from test file */
int search(struct dictNode *, string);

/* Function to count number of words starting from a prefix */
int count(int &, struct dictNode *);





#endif   // DICTTREE_H
