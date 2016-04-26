/*** NO CHANGES TO THIS FILE ARE ALLOWED EXCEPT WHERE INDICATED ***/
/* MAB management functions for HOSTD */

/* Include files */
#include "mab.h"

/*** CODE COMMENTS MAY BE ADDED BEYOND THIS POINT ***/

/*** START OF SECTION MARKER ***/
/*** ADDITIONAL FUNCTION PROTOTYPES MAY BE ADDED HERE ***/
/*** END OF SECTION MARKER ***/

/*******************************************************
 * MabPtr memChk (MabPtr arena, int size);
 *    - check for memory available
 *
 * returns address of a memory block or NULL
 *******************************************************/
MabPtr memChk(MabPtr arena, int size)
{
    return NULL; /*** REPLACE THIS LINE WITH YOUR CODE ***/
}

/*******************************************************
 * MabPtr memAlloc (MabPtr arena, int size);
 *    - allocate a memory block
 *
 * returns address of block or NULL if failure
 *******************************************************/
MabPtr memAlloc(MabPtr arena, int size)
{
    return NULL; /*** REPLACE THIS LINE WITH YOUR CODE ***/
}

/*******************************************************
 * MabPtr memFree (MabPtr mab);
 *    - de-allocate a memory block
 *
 * returns address of block or merged block
 *******************************************************/
MabPtr memFree(MabPtr m)
{
    return NULL; /*** REPLACE THIS LINE WITH YOUR CODE ***/
}

/*******************************************************
 * MabPtr memMerge(Mabptr m);
 *    - merge m recursively with its Buddy if free
 *
 * returns m
 *******************************************************/
MabPtr memMerge(MabPtr m)
{
    return NULL; /*** REPLACE THIS LINE WITH YOUR CODE ***/
}

/*******************************************************
 * MabPtr memSplit(MabPtr m, int size);
 *    - split m into two with first mab having size
 *
 * returns m or NULL if unable to supply size bytes
 *******************************************************/
MabPtr memSplit(MabPtr m, int size)
{
    return NULL; /*** REPLACE THIS LINE WITH YOUR CODE ***/
}

/*** START OF SECTION MARKER ***/
/*** ADDITIONAL FUNCTION IMPLEMENTATIONS MAY BE ADDED HERE ***/
/*** END OF SECTION MARKER ***/
/*** END OF CODE; DO NOT ADD MATERIAL BEYOND THIS POINT ***/
