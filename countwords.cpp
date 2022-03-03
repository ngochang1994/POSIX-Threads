/* Thi Tran
 * RedID: 825223653
 * CS 480 - Operating Systems
 * Professor: Ben Shen
 * Assignment 2 - POSIX Threads
 */

#include "dicttree.h"
#include <fstream>
#include <string.h>
#include <iostream>


using namespace std;




// Function to count the words searched in the text file
int count(int &result, struct dictNode *dictnode)
{
  if (dictnode->isWord)
    {
      result += 1;
    }


  for (int i = 0; i < NUMOfCHARS; i++)
    {
      if (!dictnode->next[i])
	{
	  // If a node is NULL, move to the next index
	  continue;
	}
      else
	{
	  count(result, dictnode->next[i]);   // Count number of words starting from that node
	}
    }

}



/**************************************************/




// Function to search words from the test file
int search(struct dictNode *dictnode, string str)
{
  struct dictNode *current = dictnode;

  int result = 0;		// Declare the number of words counted


  for (int i = 0; i < str.size (); i++)
    {
      int index = indexOfChar(str[i]);	// Call indexOfChar

      if (current->next[index] == NULL)
	{

	  // If a node does NOT exist --> There's no further word
	  current = NULL;
	  break;
	}
      else
	{
	  current = current->next[index];
	}

    }


  if (current != NULL)		// If current is searched, call countWord to count words starting from that prefix
    {
      count(result, current);   // Call count function

      return result;   // Return number of words counted
    }

}



