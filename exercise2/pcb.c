 /*** NO CHANGES TO THIS FILE ARE ALLOWED EXCEPT WHERE INDICATED ***/
/* PCB management functions for HOSTD */

/* Include Files */
#include "pcb.h"

/*** CODE COMMENTS MAY BE ADDED BEYOND THIS POINT ***/

/*** START OF SECTION MARKER ***/
/*** ADDITIONAL FUNCTION PROTOTYPES MAY BE ADDED HERE ***/
/*** END OF SECTION MARKER ***/

/*******************************************************
 * PcbPtr startPcb(PcbPtr process) - start (or restart)
 *    a process
 * returns:
 *    PcbPtr of process
 *    NULL if start (restart) failed
 ******************************************************/
PcbPtr startPcb (PcbPtr p)
{
    if (p->pid == 0)
    {
        switch (p->pid = fork())
        {
            case -1:
                fprintf(stderr, "FATAL: Could not fork process!\n");
                exit(EXIT_FAILURE);
            case 0:
                p->pid = getpid();
                p->status = PCB_RUNNING;
                printPcbHdr();
                printPcb(p);
                fflush(stdout);
                execv(p->args[0], p->args);
                fprintf(stderr, "Could not run process!\n");
                exit(EXIT_FAILURE);
        }
    }
    else
    {
        kill(p->pid, SIGCONT);
    }
    p->status = PCB_RUNNING;
    return p;
}

/*******************************************************
 * PcbPtr suspendPcb(PcbPtr process) - suspend
 *    a process
 * returns:
 *    PcbPtr of process
 *    NULL if suspension failed
 ******************************************************/
PcbPtr suspendPcb(PcbPtr p)
{
    if (!p)
    {
        fprintf(stderr, "ERROR: Can not suspend a NULL process\n");
        return NULL;
    }
    else
    {
        if (kill(p->pid, SIGTSTP)) {
        	fprintf(stderr, "ERROR: Unable to suspend PID %d", (int)p->pid);
        	return NULL;
        }
        waitpid(p->pid, NULL, WUNTRACED);
        return p;
    }
}

/*******************************************************
 * PcbPtr terminatePcb(PcbPtr process) - terminate
 *    a process
 * returns:
 *    PcbPtr of process
 *    NULL if termination failed
 ******************************************************/
PcbPtr terminatePcb(PcbPtr p)
{
    if (!p)
    {
        fprintf(stderr, "ERROR: Can not terminate a NULL process\n");
        return NULL;
    }
    else
    {
        if (kill(p->pid,SIGINT)) {
        	fprintf(stderr, "ERROR: Unable to terminate PID %d", (int)p->pid);
        	return NULL;
        }
        waitpid(p->pid, NULL, WUNTRACED);
        return p;
    }

}

/*******************************************************
 * PcbPtr printPcb(PcbPtr process)
 *  - print process attributes on stdout
 *  returns:
 *    PcbPtr of process
 ******************************************************/
PcbPtr printPcb(PcbPtr p)
{
    if (!p)
    {
        fprintf(stderr, "ERROR: Could not print process control block parameters\n");
        return NULL;
    }
    else
    {
    	printf("Arrival time | Priority | CPU Time | Memory Allocated\n");
    	printf("      %d      |     %d    |     %d    |          %d     \n", p->arrival_time, p->priority, p->remaining_cpu_time, p->mbytes);
    	return p;
    }
}

/*******************************************************
 * void printPcbHdr() - print header for printPcb
 *  returns:
 *    void
 ******************************************************/
void printPcbHdr()
{
    /*** WRITE YOUR CODE HERE ***/
}

/*******************************************************
 * PcbPtr createnullPcb() - create inactive Pcb.
 *
 * returns:
 *    PcbPtr of newly initialised Pcb
 *    NULL if malloc failed
 ******************************************************/
PcbPtr createnullPcb()
{
    PcbPtr new_process_Ptr;
    if (!(new_process_Ptr = (PcbPtr)malloc(sizeof(Pcb))))
    {
        fprintf(stderr, "ERROR: Could not create new process control block\n");
        return NULL;
    }
    new_process_Ptr->pid = 0;
    new_process_Ptr->args[0] = "./process";
    new_process_Ptr->args[1] = NULL;
    new_process_Ptr->arrival_time = 0;
    new_process_Ptr->priority = 0;
    new_process_Ptr->remaining_cpu_time = 0;
    new_process_Ptr->mbytes = 0;
    new_process_Ptr->mem_block = NULL;
    new_process_Ptr->status = PCB_UNINITIALIZED;
    new_process_Ptr->next = NULL;
    
    return new_process_Ptr;
}

/*******************************************************
 * PcbPtr enqPcb (PcbPtr headofQ, PcbPtr process)
 *    - queue process (or join queues) at end of queue
 *
 * returns head of queue
 ******************************************************/
PcbPtr enqPcb(PcbPtr q, PcbPtr p)
{

    if (!q) {
    	q = p;
    	q->next = NULL;
    } else {
    	PcbPtr current_proc = q;	//Temporary pointer to current process.
    	//Loop until we get to the end of the queue.
    	while(current_proc->next) {
    		current_proc = current_proc->next;
    	}
    	//Add the process to the queue.
        p->next = NULL;
    	current_proc->next = p;
    }
    return q;
}

/*******************************************************
 * PcbPtr deqPcb (PcbPtr * headofQ);
 *    - dequeue process - take Pcb from head of queue.
 *
 * returns:
 *    PcbPtr if dequeued,
 *    NULL if queue was empty
 *    & sets new head of Q pointer in adrs at 1st arg
 *******************************************************/
PcbPtr deqPcb(PcbPtr * hPtr)
{
    if (hPtr) {
        PcbPtr head = *hPtr;
        *hPtr = head->next;
        return head;
    }
        return NULL;
}

/*** START OF SECTION MARKER ***/
/*** ADDITIONAL FUNCTION IMPLEMENTATIONS MAY BE ADDED HERE ***/


/*******************************************************
* PcbPtr deq_hrrn_Pcb(PcbPtr* queue_head_ptr, int timer)
*   - Dequeues process with the highest response ratio from queue.
*   - In a tie, select the process closest to the head.
*
* returns:
*   PcbPtr of dequeued PCB
*   NULL if either queue_head_ptr is NULL or *queue_head_pre is NULL
********************************************************/

PcbPtr deq_hrrn_Pcb(PcbPtr* queue_head_ptr, int timer) {

    if (!queue_head_ptr || !(*queue_head_ptr)) {
        return NULL;
    }

    int index = 0;
    int highest_index = 0;
    int highest_ratio = 0;
    int current_ratio = 0;
    PcbPtr highest_process = NULL;
    PcbPtr current_process = *queue_head_ptr;



    //Calculate the response ratio.
    while (current_process) {
        int cputime = current_process->remaining_cpu_time;
        current_ratio = ((float)timer/cputime) + 1;
        if (current_ratio > highest_ratio) {
            highest_ratio = current_ratio;
            highest_process = current_process;
            highest_index = index;
        }
        current_process = current_process->next;
        index++;
    }
    printf("Highest Ratio: %d", highest_ratio);





    return highest_process;

}

/*** END OF SECTION MARKER ***/
/*** END OF CODE; DO NOT ADD MATERIAL BEYOND THIS POINT ***/
