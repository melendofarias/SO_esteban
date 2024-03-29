// synch.cc 
//	Routines for synchronizing threads.  Three kinds of
//	synchronization routines are defined here: semaphores, locks 
//   	and condition variables (the implementation of the last two
//	are left to the reader).
//
// Any implementation of a synchronization routine needs some
// primitive atomic operation.  We assume Nachos is running on
// a uniprocessor, and thus atomicity can be provided by
// turning off interrupts.  While interrupts are disabled, no
// context switch can occur, and thus the current thread is guaranteed
// to hold the CPU throughout, until interrupts are reenabled.
//
// Because some of these routines might be called with interrupts
// already disabled (Semaphore::V for one), instead of turning
// on interrupts at the end of the atomic operation, we always simply
// re-set the interrupt state back to its original value (whether
// that be disabled or enabled).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "synch.h"
#include "system.h"

//----------------------------------------------------------------------
// Semaphore::Semaphore
// 	Initialize a semaphore, so that it can be used for synchronization.
//
//	"debugName" is an arbitrary name, useful for debugging.
//	"initialValue" is the initial value of the semaphore.
//----------------------------------------------------------------------

Semaphore::Semaphore(const char* debugName, int initialValue)
{
    name = debugName;
    value = initialValue;
    queue = new List<Thread*>;
}

//----------------------------------------------------------------------
// Semaphore::Semaphore
// 	De-allocate semaphore, when no longer needed.  Assume no one
//	is still waiting on the semaphore!
//----------------------------------------------------------------------

Semaphore::~Semaphore()
{
    delete queue;
}

//----------------------------------------------------------------------
// Semaphore::P
// 	Wait until semaphore value > 0, then decrement.  Checking the
//	value and decrementing must be done atomically, so we
//	need to disable interrupts before checking the value.
//
//	Note that Thread::Sleep assumes that interrupts are disabled
//	when it is called.
//----------------------------------------------------------------------

void
Semaphore::P()
{
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts
    
    while (value == 0) { 			// semaphore not available
	queue->Append(currentThread);		// so go to sleep
	currentThread->Sleep();
    } 
    value--; 					// semaphore available, 
						// consume its value
    
    interrupt->SetLevel(oldLevel);		// re-enable interrupts
}

//----------------------------------------------------------------------
// Semaphore::V
// 	Increment semaphore value, waking up a waiter if necessary.
//	As with P(), this operation must be atomic, so we need to disable
//	interrupts.  Scheduler::ReadyToRun() assumes that threads
//	are disabled when it is called.
//----------------------------------------------------------------------

void
Semaphore::V()
{
    Thread *thread;
    IntStatus oldLevel = interrupt->SetLevel(IntOff);

    thread = queue->Remove();
    if (thread != NULL)	   // make thread ready, consuming the V immediately
	scheduler->ReadyToRun(thread);
    value++;
    interrupt->SetLevel(oldLevel);
}

// Dummy functions -- so we can compile our later assignments 
// Note -- without a correct implementation of Condition::Wait(), 
// the test case in the network assignment won't work!
Lock::Lock(const char* debugName) {
      s = new Semaphore(debugName, 1);
      th = NULL;
 
}
Lock::~Lock() {
        delete s;
}
bool Lock::isHeldByCurrentThread(){
        return (th == currentThread);
        }
void Lock::Acquire() {
        ASSERT(!isHeldByCurrentThread());
        s-> P();
        th = currentThread;
}
void Lock::Release() {
        ASSERT(isHeldByCurrentThread())
        s -> V();
        th = NULL;
        }

Condition::Condition(const char* debugName, Lock* conditionLock) {
        name = debugName;
        list = new List <Semaphore*>;
        if(conditionLock == NULL)
                lock = new Lock("default");
        else 
               lock = conditionLock;
 }

Condition::~Condition() {
        delete lock;
        delete list;
 }

void Condition::Wait() {
        ASSERT(lock->isHeldByCurrentThread()); 
        Semaphore *sem = new Semaphore(name, 0);
        list->Append(sem);
        lock->Release();
        sem -> P();
        lock ->Acquire();
}

void Condition::Signal() { 
      ASSERT(lock->isHeldByCurrentThread());
      Semaphore *sem;        
      if(!(list-> IsEmpty()))
        {
		  sem = list-> Remove();
          sem -> V();
        }
}
void Condition::Broadcast() { 
        while(!list->IsEmpty()) { 
                Signal();
        }
}
// Transferencia de mensajes entre procesos.

Puerto::Puerto(){
	plock = new Lock("Puerto");
	pListen =0;
	pWriter = 0;
	leerbuffer = false;
	escbuffer = false;
	esperar_esc = new Condition("escribir", plock);
	esperar_lec = new Condition("leer", plock);
	esc_buffer = new Condition("acc_buffer",plock);
	leer_buffer = new Condition("leer_buff", plock);
}

Puerto::~Puerto(){
 delete plock;
 delete esperar_esc;
 delete esperar_lec;   
 delete esc_buffer;   
 delete leer_buffer;
}

void Puerto::Send(int msg){
	plock->Acquire();
	pWriter++;
	esperar_esc->Signal();
    while(pListen==0 || escbuffer)
	 	esperar_lec->Wait();
		
	escbuffer=true;
	pListen--;
	//RC INICIO
	buf = msg+pListen;
	//RC FIN
	
	leer_buffer->Signal();
	esc_buffer->Wait();
	
	escbuffer=false;
	leerbuffer=false;
	esperar_esc->Signal();
	plock->Release();
}

void Puerto::Receive(int *msg){
	plock->Acquire();
	pListen++;
	while(pWriter == 0 || leerbuffer )
 		esperar_esc-> Wait(); 
	
	pWriter--;
	esperar_lec->Signal();
	//Despierta un send
	leerbuffer=true;
	if(!escbuffer)
		leer_buffer->Wait();
	
	//RC INICIO
	*msg = buf;
	//RC FIN
	//avisa al send qu ya uso el buffer
	esc_buffer->Signal();
	plock->Release();
}
