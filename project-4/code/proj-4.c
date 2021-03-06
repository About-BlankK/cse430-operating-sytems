/*===========================================
 *  Authors:
 *      Tyler Brockett
 *      Man Do
 *  Project:
 *      Project 4
 *  Description:
 *      Readers and Writers.
 *  Class:
 *      CSE 430 Operating Systems - Partha Dasgupta
 *  Organization:
 *      Arizona State University
 ============================================*/

#include <stdio.h>
#include <unistd.h>
#include "sem.h"

void reader();
void writer();

int wwc, wc, rwc, rc = 0;	//writer waiting count, writer count, reader waiting count, reader count

Semaphore *mutex;
Semaphore *rsem;
Semaphore *wsem;

void reader();
void writer();

int main()
{
    runQ = InitQueue();

    printf("Project 4");
    mutex = InitSem(1);
    rsem = InitSem(0);
    wsem = InitSem(0);

    start_thread(writer);
    start_thread(writer);
    start_thread(reader);
    start_thread(reader);
    start_thread(reader);

    run();

    return 0;
}

void reader(){
	while(1){
		//Reader Entry
		printf("\n[reader #%p]\tentering\n", runQ->head);
		P(mutex);
		if(wwc>0 || wc>0){
			printf("\n[reader #%p]\tneed to wait for a writer to finish.", runQ->head);
			rwc++;
			printf("\nwwc:%d \twrc:%d \twc:%d \trc:%d\n", wwc, rwc, wc, rc);
			V(mutex);
			P(rsem);
			printf("\n[reader #%p]\twaking up.", runQ->head);
			rwc--;
			printf("\nwwc:%d \twrc:%d \twc:%d \trc:%d\n", wwc, rwc, wc, rc);
		}
		rc++;
		printf("\nwwc:%d \twrc:%d \twc:%d \trc:%d\n", wwc, rwc, wc, rc);
		if(rwc>0)
			V(rsem);
		else
			V(mutex);
		//printf("\n[reader #%p]\tgoing into CS.", runQ->head);
		sleep(1);
		//Reader Exit
		printf("\n[reader #%p]\texiting, get mutex", runQ->head);
		P(mutex);
		rc--;
		printf("\n[reader #%p]\tleaving", runQ->head);
		printf("\nwwc:%d \twrc:%d \twc:%d \trc:%d\n", wwc, rwc, wc, rc);
		if(rc==0 && wwc>0)
			V(wsem);
		else
			V(mutex);
		sleep(1);
	}
}

void writer(){
	while(1){
		//Writer Entry
		printf("\n[writer #%p]\tentering\n", runQ->head);
		P(mutex);
		if(rc>0 || wc>0){
			printf("\n[writer #%p]\twriter waiting count increasing 1.", runQ->head);
			wwc++;
			printf("\nwwc:%d \twrc:%d \twc:%d \trc:%d\n", wwc, rwc, wc, rc);
			V(mutex);
			P(wsem);
			printf("\n[writer #%p]\twaking up.", runQ->head);
			wwc--;
			printf("\nwwc:%d \twrc:%d \twc:%d \trc:%d\n", wwc, rwc, wc, rc);
		}
		wc++;
		printf("\nwwc:%d \twrc:%d \twc:%d \trc:%d\n", wwc, rwc, wc, rc);
		V(mutex);
		sleep(1);
		//printf("\n[writer #%p]\tgoing into CS.", runQ->head);
		//Writer Exit
		printf("\n[writer #%p]\texiting", runQ->head);
		P(mutex);
		wc--;
		printf("\nwwc:%d \twrc:%d \twc:%d \trc:%d\n", wwc, rwc, wc, rc);
		if(rwc>0){
			printf("\n[writer #%p]\tremoving a reader.", runQ->head);
			V(rsem);
		}
		else if(wwc>0){
			printf("\n[writer #%p]\tremoving a writer", runQ->head);
			V(wsem);
		}
		else{
			printf("\n[writer #%p]\treleasing the resource.", runQ->head);
			V(mutex);
		}
		sleep(1);
	}
}
