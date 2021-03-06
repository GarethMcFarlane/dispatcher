 /*** NO CHANGES TO THIS FILE ARE ALLOWED EXCEPT WHERE INDICATED ***/
/* PCB management functions for HOSTD */

/* Include Files */
#include "pcb.h"
#include "mab.h"

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
        p->status = PCB_SUSPENDED;
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
        p->status = PCB_TERMINATED;
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
        MabPtr memblock = p->mem_block;
    	printf("%d |      %d       |     %d    |          %d             |         %d            |    %d     \n", (int)p->pid, p->arrival_time, p->priority, p->scheduled_service_time, memblock->size, memblock->offset);
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
    printf("PID   | Arrival Time | Priority | Scheduled Running Time | Allocated Memory (MB) | Memory Offset (MB)\n");
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

    //If the queue is null, return NULL;
    if (!queue_head_ptr || !(*queue_head_ptr)) {
        return NULL;
    }
    float highest_response_ratio = 0.0;
    PcbPtr process = *queue_head_ptr;
    PcbPtr highest_process = NULL;

    //First, loop to find the highest ratio.
    while (process) {
        int remaining_cpu_time = process->remaining_cpu_time;
        int waiting_time = timer - process->arrival_time - process->scheduled_service_time + process->remaining_cpu_time;
        float response_ratio = (((float)waiting_time)/remaining_cpu_time) + 1;
        if (response_ratio > highest_response_ratio) {
            highest_process = process;
            highest_response_ratio = response_ratio;
        }
        process = process->next;
    }


    //If the element is at the start of the queue, dequeue it
    //and set the list to it's next element.
    if (*queue_head_ptr == highest_process) {
        *queue_head_ptr = highest_process->next;
        return highest_process;
    }

    //Go back to the start of the queue and find the element before the dequeued
    //element.
    process = *queue_head_ptr;
    while (process->next != highest_process) {
        process = process->next;
    }
    //'Skip' the dequeued process.
    process->next = highest_process->next;

    return highest_process;

}




/*** END OF SECTION MARKER ***/
/*** END OF CODE; DO NOT ADD MATERIAL BEYOND THIS POINT ***/
