#include <iostream>
#include "thread.h"
#include "semaphore.h"
#include "mutex.h"
 
using namespace std;
 
int REP = 5;
char buffer;
Mutex mut;
Semaphore sem(1),sem2(0);
 
void * producer(void * arg)
{
    mut.lock();
    cout << "Producer was born!\n";
    mut.unlock();
 
    char data = -1;
    for(int i = 0; i < REP; i++) {
    mut.lock(); 
    cout << "Producing ...\n";
    mut.unlock();
    data = (char) i + 0x61;
	sem.p();
    buffer = data;
	sem2.v();
    mut.lock();
    cout << "Stored... " << data << endl;
    mut.unlock();
    }
    exit(REP);
}
 
void * consumer(void * arg)
{
    mut.lock();
    cout << "Consumer was born!\n";
    mut.unlock();
    char data = -1;
    for(int i = 0; i < REP; i++) {
    mut.lock();
    cout << "Retrieving ...\n";
    mut.unlock();
	sem2.p();
    data = buffer;
	sem.v();

    mut.lock();
    cout << "Consumed... " << data << endl;
    mut.unlock();
    }

 
    exit(REP);
}
 
int main()
{
    mut.lock();
    cout << "The Producer x Consumer Problem\n";
    mut.unlock();
 
    Thread prod(&producer, &REP, sizeof(int));
    Thread cons(&consumer, &REP, sizeof(int));
 
    int status;
    prod.join(&status);
    if(status == REP){
    	mut.lock();
    	cout << "Producer went to heaven!\n";
    	mut.unlock();
    }
    else{
    	mut.lock();
    	cout << "Producer went to hell!\n";
    	mut.unlock(); 
    }
    cons.join(&status);
    if(status == REP){
    	mut.lock();
    	cout << "Consumer went to heaven!\n";
    	mut.unlock();
    }
    else{
    	mut.lock();
    	cout << "Consumer went to hell!\n";
    	mut.unlock(); 
    }

    return 0;
}
