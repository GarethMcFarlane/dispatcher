/*** NO CHANGES TO THIS FILE ARE ALLOWED EXCEPT WHERE INDICATED ***/

/*** START OF SECTION MARKER ***/
/*** PLEASE UPDATE THIS SECTION. NO CODE INCLUDING INCLUDE STATEMENTS MAY BE ADDED. ***/
/*
    HOSTD - Hypothetical Operating System Testbed Dispatcher

    usage:

        hostd <TESTFILE>
        where <TESTFILE> is the name of a job list
*/

/************************************************************************************************************************

    ** Revision history **

    Current version: 1.1 bis
    Date: 7 April 2016

    1.1: Added instructions to allow additional functions in hostd.c and pcb.c
    1.0: Original version

    Contributors:
    1. COMP3520 teaching staff
       Centre for Distributed and High Performance Computing
       School of Information Technologies
       The University of Sydney
       NSW 2006
       Australia

    2. Dr Ian G Graham

    Copyright of this code and associated material is vested in the original contributors.

    This code is NOT in the Public Domain. Unauthorised posting of this code or derivatives thereof is not permitted.

    ** DO NOT REMOVE THIS NOTICE. Just update the revision history whenever you make changes to the code. **

 ***********************************************************************************************************************/
/*** END OF SECTION MARKER ***/

/* Include files */
#include "hostd.h"

/*** START OF SECTION MARKER ***/
/*** ADDITIONAL FUNCTION PROTOTYPES MAY BE ADDED HERE ***/
/*** END OF SECTION MARKER ***/

int main (int argc, char *argv[])
{
    /*** START OF SECTION MARKER ***/
    /*** You may add variable declarations in this section. ***/

    FILE * input_list_stream = NULL;
    PcbPtr input_queue = NULL;
    PcbPtr current_process = NULL;
    PcbPtr process = NULL;
    int timer = 0;

/*** END OF SECTION MARKER ***/

/*** START OF SECTION MARKER ***/
/*** COMMENTS MAY BE ADDED, MODIFIED OR DELETED IN THIS SECTION ***/

//  1. Populate the FCFS queue

    if (argc == 0)
    {
        fprintf(stderr, "FATAL: Bad arguments array\n");
    }
    else if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <TESTFILE>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (!(input_list_stream = fopen(argv[1], "r")))
    {
        fprintf(stderr, "ERROR: Could not open \"%s\"\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        int val_check;
        process = createnullPcb();
        if ((val_check = fscanf(input_list_stream, "%d, %d, %d, %d",
          &(process->arrival_time), &(process->priority),
          &(process->scheduled_service_time), &(process->mbytes))) == 4)
        {
            process->remaining_cpu_time = process->scheduled_service_time;
            process->status = PCB_INITIALIZED;
            input_queue = enqPcb(input_queue, process);
        }
        else if (val_check >= 0)
        {
            free(process);
            fprintf(stderr, "ERROR: Invalid input file \"%s\"\n", argv[1]);
            exit(EXIT_FAILURE);
        }
        else if (ferror(input_list_stream))
        {
            free(process);
            fprintf(stderr, "ERROR: Could not read input file \"%s\"\n", argv[1]);
            exit(EXIT_FAILURE);
        }
        else {
            free(process);
            break;
        }
    }
/*** END OF SECTION MARKER ***/

/*** START OF SECTION MARKER ***/
/*** CODE AND COMMENTS MAY BE ADDED, DELETED OR MODIFIED IN THIS SECTION ***/

/*** COMMENTS THAT BEGIN WITH // ARE FOR EXERCISE 4 ONLY ***/
/*** YOU NEED TO WRITE YOUR OWN COMMENTS ***/

//  2. Whenever there is a running process or the FCFS queue is not empty:

    while (1) // REPLACE THIS LINE WITH YOUR CODE (ONE LINE ONLY)
    {
//      i. If there is a currently running process;
        if (1) // REPLACE THIS LINE WITH YOUR CODE (ONE LINE ONLY)
        {
//          a. Decrement the process's remaining_cpu_time variable;
            puts("I am a duck."); // REPLACE THIS LINE WITH YOUR CODE (ONE LINE ONLY)
            
//          b. If the process's allocated time has expired:
            if (1) // REPLACE THIS LINE WITH YOUR CODE (ONE LINE ONLY)
            {
//              A. Terminate the process;
                puts("I am a duck."); // REPLACE THIS LINE WITH YOUR CODE (ONE LINE ONLY)
                
//              B. Deallocate the PCB (process control block)'s memory
                free(current_process);
                current_process = NULL;
            }
        }

//      ii. If there is no running process and there is a process ready to run:
        if (1) // REPLACE THIS LINE WITH YOUR CODE (ONE LINE ONLY)
        {
//          Dequeue the process at the head of the queue, set it as currently running and start it
            puts("Quack, quack!"); // REPLACE THIS LINE WITH YOUR CODE (AT LEAST TWO LINES)
        }
        
//      iii. Let the dispatcher sleep for one second;
        printf("%d\n", timer); // REPLACE THIS LINE WITH YOUR CODE (ONE LINE ONLY)
        
//      iv. Increment the dispatcher's timer;
        break; // REPLACE THIS LINE WITH YOUR CODE (ONE LINE ONLY)
        
//      v. Go back to 4.
    }
    
//  3. Terminate the HOST dispatcher
    exit(EXIT_SUCCESS);
}

/*** END OF SECTION MARKER ***/

/*** START OF SECTION MARKER ***/
/*** ADDITIONAL FUNCTION IMPLEMENTATIONS MAY BE ADDED HERE ***/
/*** END OF SECTION MARKER ***/
/*** END OF CODE MARKER; DO NOT ADD MATERIAL BEYOND THIS POINT ***/
