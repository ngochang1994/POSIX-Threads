/* Thi Tran
 * RedID: 825223653
 * CS 480 - Operating Systems
 * Professor: Ben Shen
 * Assignment 2 - POSIX Threads
 */

#include "dicttree.h"
#include <iostream>
#include <fstream>
#include <string.h>


using namespace std;


// Function to create a new node
struct dictNode *createANewNode()
{
  struct dictNode *node = new dictNode;

  node->isWord = false;


  for (int i = 0; i < NUMOfCHARS; i++)
    {
      node->next[i] = NULL;
    }



  return node;
}

/*************************************************/


// Function to ignore case sensitivity
int indexOfChar(char ch)
{
  int index;

  if (ch == '\'')
    {
      index = NUMOfCHARS - 1;
    }
  else if (ch >= 'A' && ch <= 'Z')

    {
      index = ch - 'A';
    }
  else
    {
      index = ch - 'a';
    }
  return index;

}





/**************************************************/


// Function to insert word to dictionary tree
void insert(struct dictNode *dictnode, string str)
{
  struct dictNode *current = dictnode;


  for (int i = 0; i < str.size (); i++)
    {
      int index = indexOfChar(str[i]);	// Call indexOfChar

      if (!current->next[index])
	{
	  // Create a new node if the current node does not exist
	  current->next[index] = createANewNode ();
	}

      // Go to the next node
      current = current->next[index];
    }

  // Mark current node
  current->isWord = true;
}



/**************************************************/



