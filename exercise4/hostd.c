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
#include "mab.h"


/*** START OF SECTION MARKER ***/
/*** ADDITIONAL FUNCTION PROTOTYPES MAY BE ADDED HERE ***/
    PcbPtr deq_hrrn_Pcb(PcbPtr *, int);
    MabPtr createMab();
    //MabPtr memFree(PcbPtr *, PcbPtr);
    int checkMemory(MabPtr *, int);
/*** END OF SECTION MARKER ***/

    int main (int argc, char *argv[])
    {
    /*** START OF SECTION MARKER ***/
    /*** You may add variable declarations in this section. ***/

    //Memory blocks
        MabPtr mem = createMab();

    //Input stream from file.
        FILE * input_list_stream = NULL;

    //Queues.
        PcbPtr input_queue = NULL;
        PcbPtr user_queue = NULL;
        PcbPtr realtime_queue = NULL;
        PcbPtr priority_queue_1 = NULL;
        PcbPtr priority_queue_2 = NULL;
        PcbPtr priority_queue_3 = NULL;

    //Currently active process.
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
                process->args[0] = "process";
                process->args[1] = NULL;
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



        while (input_queue || user_queue || realtime_queue || current_process || priority_queue_1 || priority_queue_2 || priority_queue_3) 
        {
            while (input_queue && input_queue->arrival_time <= timer) {
                PcbPtr deq_process = deqPcb(&input_queue);

                if (deq_process->priority == 0) {
                    realtime_queue = enqPcb(realtime_queue,deq_process);
                } else {
                    user_queue = enqPcb(user_queue,deq_process);
                }
                
            }


            while (user_queue && checkMemory(&mem,user_queue->mbytes)) {
                PcbPtr user_deq = deqPcb(&user_queue);
                user_deq->mem_block = memAlloc(&mem,user_deq->mbytes);
                switch (user_deq->priority) {
                    case 1:   
                    priority_queue_1 = enqPcb(priority_queue_1, user_deq);
                    break;
                    case 2:
                    priority_queue_2 = enqPcb(priority_queue_2, user_deq);
                    break;
                    case 3:
                    priority_queue_3 = enqPcb(priority_queue_3, user_deq);
                    break;
                }
            }


//      i. If there is a currently running process;
            if (current_process) 
            {
//          a. Decrement the process's remaining_cpu_time variable;
                current_process->remaining_cpu_time--;
//          b. If the process's allocated time has expired:
                if (current_process->remaining_cpu_time == 0) 
                {
//              A. Terminate the process;
                    terminatePcb(current_process);
//              B. Deallocate the PCB (process control block)'s memory
                    memFree(&mem, current_process->mem_block);
                    free(current_process);
                    current_process = NULL;

                } else if (current_process->priority == 1){
                    current_process->priority = 2;
                    if (priority_queue_1 || priority_queue_2) {
                        suspendPcb(current_process);
                        priority_queue_2 = enqPcb(priority_queue_2, current_process);
                        current_process = NULL;
                    }
                } else if (current_process->priority == 2) {
                    current_process->priority = 3;
                    if (priority_queue_1 || priority_queue_2 || priority_queue_3) {
                        suspendPcb(current_process);
                        priority_queue_3 = enqPcb(priority_queue_3,current_process);
                        current_process = NULL;
                    }
                } else if (priority_queue_1 || priority_queue_2) {
                    suspendPcb(current_process);
                    priority_queue_3 = enqPcb(priority_queue_3,current_process);
                    current_process = NULL;
                }
            }


//      ii. If there is no running process and there is a process ready to run:
            int process_ready = priority_queue_1 || priority_queue_2 || priority_queue_3;
            if (!current_process && process_ready)
            {
                if (priority_queue_1) {
                 current_process = startPcb(deqPcb(&priority_queue_1));
             } else if (priority_queue_2) {
                current_process = startPcb(deqPcb(&priority_queue_2));
            } else {
                current_process = startPcb(deq_hrrn_Pcb(&priority_queue_3,timer));
            }

            
        }
        
//      iii. Let the dispatcher sleep for one second;
        sleep(1);
        
//      iv. Increment the dispatcher's timer;
        timer++;
        
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
