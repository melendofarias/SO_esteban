// threadtest.cc 
//	Simple test case for the threads assignment.
//
//	Create several threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield, 
//	to illustrate the inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.
//
// Parts from Copyright (c) 2007-2009 Universidad de Las Palmas de Gran Canaria
//

#include "copyright.h"
#include "system.h"
#include "synch.h"
//----------------------------------------------------------------------
// SimpleThread
// 	Loop 10 times, yielding the CPU to another ready thread 
//	each iteration.
//
//	"name" points to a string with a thread name, just for
//      debugging purposes.
//----------------------------------------------------------------------
//Lock *lock = new Lock("Lock");

Puerto *puerto = new Puerto();

Semaphore *sem = new Semaphore("Semaphore",1);

void
SimpleThread(void* name)
{
    // Reinterpret arg "name" as a string
    char* threadName = (char*)name;
    //lock -> Acquire();
  sem -> P();    
    // If the lines dealing with interrupts are commented,
    // the code will behave incorrectly, because
    // printf execution may cause race conditions.
    for (int num = 0; num < 10; num++) {
        //IntStatus oldLevel = interrupt->SetLevel(IntOff);
	printf("*** thread %s looped %d times\n", threadName, num);
	//interrupt->SetLevel(oldLevel);
       // currentThread->Yield();
    }
    //lock -> Release();    
     sem->V();  
    //IntStatus oldLevel = interrupt->SetLevel(IntOff);
    printf(">>> Thread %s has finished\n", threadName);
    //interrupt->SetLevel(oldLevel);
}

//----------------------------------------------------------------------
// ThreadTest
// 	Set up a ping-pong between several threads, by launching
//	ten threads which call SimpleThread, and finally calling 
//	SimpleThread ourselves.
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// Funciones utilizadas para TestPuerto.
//----------------------------------------------------------------------
void
Productor(int n)
{
	DEBUG('w',"enviando el numero: %d\n",n);
	puerto->Send(n);
}
void
Consumidor()
{
	int *num;
	puerto->Receive(num);
	DEBUG('w',"Consumidor recibiendo el numero: %d\n",*num);
}
void
Test_1(void* n)
{
	DEBUG('w', "Escritor, %d\n",n);	
	int k = 2 ;
	Productor(k);
	
}
void 
Test_2(void* name)
{
	DEBUG('w', "Lector, %d\n",name);
	Consumidor();
}
//----------------------------------------------------------------
//----------------------------------------------------------------
void
ThreadTest()
{
    
	DEBUG('t', "Entering SimpleTest");
    for ( int k=1; k<=10; k++) {
      char* threadname = new char[100];
      sprintf(threadname, "Hilo %d", k);
      Thread* newThread = new Thread (threadname,false);
      newThread->Fork (SimpleThread, (void*)threadname);
    }
    SimpleThread( (void*)"Hilo 0");

}
void
PuertoTest()
{
	DEBUG('w', "Prueba consumidor-Productor\n");
	for(int k=0; k<5; k++)
	{ 
	  Thread* newThread = new Thread ("Escritores",false);
      newThread->Fork (Test_1, (void*)k);	
	}	
	for(int k=0; k<5; k++)
	{ 
	  Thread* newThread = new Thread ("Lectores",false);
      newThread->Fork (Test_2, (void*)k);	
	}
}

void
JoinTest(){

}
//--------------------------------------------------------
//-----------Test Main------------------------------------
//--------------------------------------------------------

void 
Test()
{

	printf("Test: t --> Thread\n");
	printf("Test: p --> Puerto\n");
	printf("Test: j --> Join\n");
	printf("Ingrese test a realizar : ");
	char test = getchar();
	if (test == 't' )
		ThreadTest();
	else if (test == 'p')
		PuertoTest();
	else if (test == 'j')
		JoinTest();

}

