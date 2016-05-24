/*** NO CHANGES TO THIS FILE ARE ALLOWED EXCEPT WHERE INDICATED ***/
/* MAB management functions for HOSTD */

/* Include files */
#include "mab.h"

/*** CODE COMMENTS MAY BE ADDED BEYOND THIS POINT ***/

/*** START OF SECTION MARKER ***/
/*** ADDITIONAL FUNCTION PROTOTYPES MAY BE ADDED HERE ***/
MabPtr createMab();
int checkMemory(MabPtr *, int);
/*** END OF SECTION MARKER ***/

/*******************************************************
 * MabPtr memAlloc (MabPtr * lists, int size);
 *    - allocate a memory block
 *
 * returns address of block or NULL if failure
 *******************************************************/
MabPtr memAlloc(MabPtr * lists, int size)
{
	MabPtr currentMab = *lists;
	while (currentMab) {
		if (size <= currentMab->size && currentMab->allocated == 0) {
			return memSplit(lists,currentMab,size);
		}
		currentMab = currentMab->next;
	}
    return NULL;
}

/*******************************************************
 * MabPtr memFree (MabPtr * lists, MabPtr mab);
 *    - de-allocate a memory block
 *
 * returns address of block or merged block
 *******************************************************/
MabPtr memFree(MabPtr * lists, MabPtr m)
{
    m->allocated = 0;
    //Try and recursively merge with buddy.
    return NULL;
}

/*******************************************************
 * MabPtr memMerge(MabPtr * lists, Mabptr m);
 *    - merge m recursively with its Buddy if free
 *
 * returns a pointer to one of the following:
 * - merged block if m's Buddy is free
 * - m if m is free but its Buddy is not
 * - NULL if there is an error
 *******************************************************/
MabPtr memMerge(MabPtr * lists, MabPtr m)
{
    return NULL; /*** REPLACE THIS LINE WITH YOUR CODE ***/
}

/*******************************************************
 * MabPtr memSplit(MabPtr * lists, MabPtr m, int size);
 *    - split m into two if needed
 * children
 * returns NULL if there is an error
 *******************************************************/
MabPtr memSplit(MabPtr * lists, MabPtr m, int size)
{
	//Base case:
	if (size > m->size/2) {
		m->allocated = 1;
		return m;
	//Split into halves and call function again.  
	} else {
		MabPtr split = createMab();
		m->size = m->size/2;
		split->offset = m->offset + m->size;
		split->size = m->size;
		split->next = m->next;
		split->prev = m;
		m->next = split;
		if (split->next) {
			split->next->prev = split;
		}
		return memSplit(lists,m,size);
	}
}

/*** START OF SECTION MARKER ***/
/*** ADDITIONAL FUNCTION IMPLEMENTATIONS MAY BE ADDED HERE ***/

MabPtr createMab() {
	Mab newMab;
	MabPtr newMabPtr = (MabPtr) malloc(sizeof(newMab));
	newMabPtr->next = NULL;
	newMabPtr->prev = NULL;
	newMabPtr->parent = NULL;
	newMabPtr->left_child = NULL;
	newMabPtr->right_child = NULL;
	newMabPtr->size = MEMORY_SIZE;
	newMabPtr->allocated = 0;
	newMabPtr->offset = 0;
	return newMabPtr;
}

int checkMemory(MabPtr * lists, int size) {
	MabPtr current = *lists;
	while (current) {
		if (size <= current->size && current->allocated == 0) {
			return TRUE;
		}
		current = current->next;
	}
	return FALSE;
}
/*** END OF SECTION MARKER ***/
/*** END OF CODE; DO NOT ADD MATERIAL BEYOND THIS POINT ***/
