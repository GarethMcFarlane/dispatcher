/*** NO CHANGES TO THIS FILE ARE ALLOWED. PLEASE DO NOT SUBMIT THIS FILE FOR YOUR ASSIGNMENT. ***/

/* MAB include header file for HOSTD */

#ifndef HD_MAB
#define HD_MAB

/* Include files */
#include <stdlib.h>
#include <stdio.h>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

/*** END OF SECTION MARKER ***/
/*** YOU MAY ADD DEFINITIONS OR PROTOTYPES BEYOND THIS POINT ***/
/*** DO NOT CHANGE OR DELETE EXISTING DEFINITIONS OR PROTOTYPES ***/

/* Memory Management Definitions *******************************/

#define MEMORY_SIZE       1024
#define RT_MEMORY_SIZE    64
#define USER_MEMORY_SIZE  (MEMORY_SIZE - RT_MEMORY_SIZE)

/* Custom Data Types */
struct mab {
    int offset;
    int size;
    int allocated;
    struct mab * next;
    struct mab * prev;
};

typedef struct mab Mab;
typedef Mab * MabPtr;

/* Function Prototypes */
MabPtr memChk(MabPtr, int);
MabPtr memAlloc(MabPtr, int);
MabPtr memFree(MabPtr);
MabPtr memMerge(MabPtr);
MabPtr memSplit(MabPtr, int);
#endif

/*** PLEASE DO NOT SUBMIT THIS FILE FOR YOUR ASSIGNMENT. ***/
