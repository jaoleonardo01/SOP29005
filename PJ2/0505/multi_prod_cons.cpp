#include <iostream>
#include <fstream>
#include <uuid/uuid.h>
#include <unistd.h>
#include <errno.h>
#include "mutex.h"
#include "semaphore.h"
#include "thread.h"

 
using namespace std;
 
const int PRODS = 1;
const int REP = 10;
 
const int CONS = 1;
 
const int BUF_SIZE = 1;
 
Thread * prods[PRODS];
Thread * cons[CONS];
 
uuid_t buffer[BUF_SIZE];
static int prod_pos = 0;
static int cons_pos = 0;
 
bool finished = false;

Mutex mut;

Semaphore sem1(1),sem2(0);
 
void* producer(void * arg)
{
	cout << "Producer was born!" << endl;
 
	int rep = REP;
 
	char fname[36+1];


	while(rep--)
	{
		sem1.p();
		if(++prod_pos == BUF_SIZE) prod_pos = 0;
		uuid_generate(buffer[prod_pos]);
		uuid_unparse(buffer[prod_pos], fname);
 
		string name(fname,sizeof(uuid_t)*2 + 4);
		ofstream file(name.c_str());
		file << name;
		sem2.v();
		file.close();
	}

 
	exit(REP);
}
 
void* consumer(void* arg)
{
	cout << "Consumer was born!" << endl;
 
	char fname[36+1];
	int consumed = 0;
 
 
	while(true)
	{

		if(finished) exit(consumed);
 
		consumed++;
 
		if(++cons_pos == BUF_SIZE) cons_pos = 0;
		uuid_unparse(buffer[cons_pos], fname);
		sem1.v();
 
		{
			ifstream file(fname);
			if(!file.good()) continue;
			string str;
			sem2.p();
			file >> str;
			sem1.v();
			cout << "Consumed: " << str << endl;
		}
 
		if(remove(fname)) cerr << "Error: " << errno << endl;
	}

	exit(consumed);
}
 
int main()
{
    cout << "Massive Producer x Consumer Problem\n";
 
    // Create
    for(int i = 0; i < PRODS; i++)
    	prods[i] = new Thread(&producer, 0, 0);
    for(int i = 0; i < CONS; i++)
    	cons[i] = new Thread(&consumer, 0, 0);
 
    // Join
    int status = 0;
    int produced = 0;
    int consumed = 0;
    for(int i = 0; i < PRODS; i++)
    {
    	prods[i]->join(&status);
    	produced += status;
    }
 
    finished = true;
 
    for(int i = 0; i < CONS; i++)
    {
    	cons[i]->join(&status);
    	consumed += status;
    }
 
    cout << "Total produced: " << produced << endl;
    cout << "Total consumed: " << consumed << endl;
 
    return 0;
}
