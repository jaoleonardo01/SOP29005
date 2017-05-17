#include <iostream>
#include <fstream>
#include <uuid/uuid.h>
#include <unistd.h>
#include <errno.h>
#include "mutex.h"
#include "semaphore.h"
#include "thread.h"
 
  using namespace std;
 
  const int PRODS = 10;
  const int REP = 1;
  const int CONS =10;
  const int BUF_SIZE = 35;
  int end=0; 
  Thread * prods[PRODS];
  Thread * cons[CONS];
  Mutex moutro, moutro1, mproducer, mconsumer, mend;
  Semaphore vazio[BUF_SIZE];
  Semaphore cheio[BUF_SIZE];
 
  uuid_t buffer[BUF_SIZE];
  static int prod_pos = 0;
  static int cons_pos = 0;
 
  bool finished = false;

 
  void* producer(void * arg)
  {
	moutro.lock();
	cout << "Producer was born!" << endl;
	moutro.unlock();
	int trep = REP;
	int prod=0;
	char fname[36+1];


	while(trep--)
	{
		mproducer.lock();
		prod=prod_pos++;
		if(++prod_pos == BUF_SIZE) prod_pos = 0;
		mproducer.unlock();
		vazio[prod].p();
		uuid_generate(buffer[prod]);
		uuid_unparse(buffer[prod], fname);
		string name(fname,sizeof(uuid_t)*2 + 4);
		ofstream file(name.c_str());
		file << name;
		name.sync();
		file.close();
		cheio[prod].v();
	}

 
	exit(REP);
  }
 
  void* consumer(void* arg)
  {
  	moutro1.lock();
	cout << "Consumer was born!" << endl;
	moutro1.unlock();
	char fname[36+1];
	int w=0;
	int consumo=0; 
	while(true)
	{

		if(finished) exit(consumo);
		// sync();
 
		mconsumer.lock();
		w=cons_pos++; 
		if(cons_pos == BUF_SIZE) cons_pos = 0;
		mconsumer.unlock();

		cheio[w].p();
		consumo++;
		cout << "Consumido: "<< consumo << endl;
		uuid_unparse(buffer[w], fname);
		vazio[w].v();
		ifstream file(fname);
		if(!file.good()) continue;
		string str;
		file >> str;
		file.sync();
		cout << "Consumed: " << str << endl;
	}
	if(remove(fname)) cerr << "Error: " << errno << endl;
  	
	exit(consumo);
 }

 
 int main(){
    int teste=0;
    int xablau=0;
    cout << "Massive Producer x Consumer Problem\n";

    for(int i=0;i<BUF_SIZE;i++){
	cheio[i].p();
    }

 
    // Create
    for(int i = 0; i < PRODS; i++)
    prods[i] = new Thread(&producer, 0, 0);
    for(int i = 0; i < CONS; i++)
    cons[i] = new Thread(&consumer, 0, 0);
 
    // Join
    int status = 0;
    int produced = 0;
    int con = 0;
    for(int i = 0; i < PRODS; i++)
    {
    	prods[i]->join(&status);
    	produced += status;
    }
    sleep(2);

    if(produced == (PRODS*REP)){
    	for(int i = 0;i < BUF_SIZE;i++){
    		xablau = (int) cheio[i];
    		if(xablau == 0)teste++;
    	}
    
    	if(teste == BUF_SIZE)finished = true;
    } 


    for(int i = 0; i < CONS; i++){
	    cons[i]->join(&status);
	    con += status;
    }
 
    cout << "Total produced: " << produced << endl;
    cout << "Total consumed: " << con << endl;
    return 0;
}
