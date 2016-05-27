/*** NO CHANGES TO THIS FILE ARE ALLOWED EXCEPT WHERE INDICATED ***/
/* MAB management functions for HOSTD */

/* Include files */
#include "mab.h"

/*** CODE COMMENTS MAY BE ADDED BEYOND THIS POINT ***/

/*** START OF SECTION MARKER ***/
/*** ADDITIONAL FUNCTION PROTOTYPES MAY BE ADDED HERE ***/
MabPtr createMab(int);
int checkMemory(MabPtr *, int);
int getOrder(int);
int addToList(MabPtr,MabPtr);
void printTree(MabPtr *);
/*** END OF SECTION MARKER ***/

/*******************************************************
 * MabPtr memAlloc (MabPtr * lists, int size);
 *    - allocate a memory block
 *
 * returns address of block or NULL if failure
 *******************************************************/
MabPtr memAlloc(MabPtr * lists, int size)
{
	//Get the order of the required size.
	int order = getOrder(size);

	//Search through the list and see if there are any free blocks.
	//If there are, allocate them and return it.
	MabPtr currentMab = lists[order];
	while (currentMab) {
		if (currentMab->allocated == 0) {
			currentMab->allocated = 1;
			return currentMab;
		}
		currentMab = currentMab->next;
	}

	//If there are no free blocks, recursively go up a level until a free block is found.
	currentMab = memAlloc(lists,size*2);

	//Split the free Mab into two and get it's left child.
	MabPtr allocedMab = memSplit(lists,currentMab,size);

	//Return the allocated block.
	return allocedMab;

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
    MabPtr result = memMerge(lists,m);

    if (result == m->parent) {
    	return memFree(lists,m->parent);
    }
    return result;



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
	//If we reach the last (1024) block.
	if (!m->parent) {
		return m;
	}
    MabPtr parent = m->parent;
	MabPtr left_child = parent->left_child;
	MabPtr right_child = parent->right_child;

	if (right_child->allocated == 0 && left_child->allocated == 0) {
		return parent;
	}

	return m;
}

/*******************************************************
 * MabPtr memSplit(MabPtr * lists, MabPtr m, int size);
 *    - split m into two if needed
 * children
 * returns NULL if there is an error
 *******************************************************/
MabPtr memSplit(MabPtr * lists, MabPtr m, int size)
{

	//Create left and right children.
	MabPtr left = createMab(m->size/2);
	MabPtr right = createMab(m->size/2);
	//Adopt the children
	m->left_child = left;
	m->right_child = right;
	//Children accept their new parents
	left->parent = m;
	right->parent = m;
	//Set offsets
	left->offset = m->offset;
	right->offset = left->offset + left->size;
	//Assign next and prev.
	left->next = right;
	right->prev = left;

	//Get the order of the child list.
	int child_order = getOrder(m->size) + 1;

	//If list is empty, start it.
	if (!lists[child_order]) {
		lists[child_order] = left;
	//Else find the last Mab on the list and assign the left child to be the next one.
	} else {
		MabPtr current = lists[child_order];
		while (!current->next) {
			current = current->next;
		}
		current->next = left;
	}


	//fprintf(stderr,"Split block of size %d into two blocks of size %d.\n", m->size, right->size);
	//Allocate block and return.
	left->allocated = 1;
	return left;
}

/*** START OF SECTION MARKER ***/
/*** ADDITIONAL FUNCTION IMPLEMENTATIONS MAY BE ADDED HERE ***/

MabPtr createMab(int size) {
	Mab newMab;
	MabPtr newMabPtr = (MabPtr) malloc(sizeof(newMab));
	newMabPtr->next = NULL;
	newMabPtr->prev = NULL;
	newMabPtr->parent = NULL;
	newMabPtr->left_child = NULL;
	newMabPtr->right_child = NULL;
	newMabPtr->size = size;
	newMabPtr->allocated = 0;
	newMabPtr->offset = 0;
	return newMabPtr;
}

int checkMemory(MabPtr * lists, int size) {
	int order = getOrder(size);

	for (int i = order; i >= 0; --i) {
		MabPtr currentlist = lists[i];
		while (currentlist) {
			if (currentlist->allocated == 0) {
				return 1;
			}
			currentlist = currentlist->next;
		}
	}
	return 0;
}

int getOrder(int size) {
	if (size > 512) {
		return 0;
	}
	if (size > 256) {
		return 1;
	}
	if (size > 128) {
		return 2;
	}
	if (size > 64) {
		return 3;
	}
	if (size > 32) {
		return 4;
	}
	if (size > 16) {
		return 5;
	}
	if (size > 8) {
		return 6;
	}
	if (size > 4) {
		return 7;
	}
	if (size > 2) {
		return 8;
	}
	if (size > 1) {
		return 9;
	}
	return 10;
}

int addToList(MabPtr list, MabPtr item) {
	MabPtr current = list;
	if (!current) {
		list = item;
		return 0;
	}

	while(current->next) {
		current = current->next;
	}

	current->next = item;
	item->prev = current;
	return 0;
}


void printTree(MabPtr *lists) {
	int maxwidth = 64;
	int depth = 0;
	while (depth < 6) {
		//MabPtr currentlist = lists[depth];
		int num = depth + 1;
		int node_size = 60/num;

		for (int j = 0; j < num; ++j) {
			fprintf(stderr,"(");
			for (int i = 0; i < (node_size-2)/2; ++i) {
				fprintf(stderr," ");
			}
			fprintf(stderr,"|");
			for (int i = 0; i < (node_size-2)/2; ++i) {
				fprintf(stderr," ");
			}
			fprintf(stderr,")");
		}
		fprintf(stderr,"\n");
		depth++;
	}

}


			
			
/*** END OF SECTION MARKER ***/
/*** END OF CODE; DO NOT ADD MATERIAL BEYOND THIS POINT ***/
