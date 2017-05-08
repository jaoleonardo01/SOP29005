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

int end=0; 

Thread * prods[PRODS];

Thread * cons[CONS];

Mutex mproducer, mconsumer, mend;

Semaphore vazio[BUF_SIZE];

Semaphore cheio[BUF_SIZE];

 

uuid_t buffer[BUF_SIZE];

static int prod_pos = 0;

static int cons_pos = 0;

 

bool finished = false;



 

void* producer(void * arg)

{

	cout << "Producer was born!" << endl;

	int trep = REP;

	int prod=0;

	char fname[36+1];





	while(trep--)

	{

		mproducer.lock();

		prod=prod_pos;

		if(++prod_pos == BUF_SIZE) prod_pos = 0;

		mproducer.unlock();

		vazio[prod].p();	

		uuid_generate(buffer[prod]);

		cheio[prod].v();

		uuid_unparse(buffer[prod], fname);

		string name(fname,sizeof(uuid_t)*2 + 4);

		ofstream file(name.c_str());

		file << name;

		file.close();

	}



 

	exit(REP);

}

 

void* consumer(void* arg)

{

	cout << "Consumer was born!" << endl;



	char fname[36+1];

	int j=0;

	int consumo=0; 

	while(true)

	{



	if(finished) exit(consumo);

		sync();		 

		mconsumer.lock();

		consumo=cons_pos; 

		if(++cons_pos == BUF_SIZE) cons_pos = 0;

		mconsumer.unlock();



		cheio[consumo].p();

		uuid_unparse(buffer[consumo], fname);

 		j++;

		vazio[consumo].v();

		{

			ifstream file(fname);

			if(!file.good()) continue;

			string str;

			file >> str;

			cout << "Consumed: " << str << endl;

		}

		if(remove(fname)) cerr << "Error: " << errno << endl;

		mend.lock();

		end++;

		mend.unlock();

		

	}



	exit(consumo);

}

 

int main(){



    for(int i=0;i<BUF_SIZE;i++){

	vazio[i].p();

    }



    cout << "Massive Producer x Consumer Problem\n";

 

    // Create

    for(int i = 0; i < PRODS; i++)

    	prods[i] = new Thread(&producer, 0,0);

    for(int i = 0; i < CONS; i++)

    	cons[i] = new Thread(&consumer, 0,0);

 

    // Join

    int status = 0;

    int produced = 0;

    int con = 0;

    for(int i = 0; i < PRODS; i++)

    {

    	prods[i]->join(&status);

    	produced += status;

    }

   sleep(1);

   if(end == produced){

	for(int i = 0;i < BUF_SIZE;i++){

		cheio[i].v();

		vazio[i].v();

	}

    

   

    finished = true;

   } 



	 for(int i = 0; i < CONS; i++)

    {

    	cons[i]->join(&status);

    	con += status;

    }

 

    cout << "Total produced: " << produced << endl;

    cout << "Total consumed: " << con << endl;

    return 0;

}
