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
  const int CONS =1;
  const int BUF_SIZE = 1;
  int end=0;
  Thread * prods[PRODS];
  Thread * cons[CONS];
  Mutex moutro, moutro1, mproducer, mconsumer, mend;
  //Semaphore s_prod[BUF_SIZE];
  //Semaphore s_cons[BUF_SIZE];
  Semaphore s_prod(1);
  Semaphore s_cons(0);

  uuid_t buffer[BUF_SIZE];
  static int prod_pos = 0;
  static int cons_pos = 0;

  bool finished = false;


  void* producer(void * arg){
        moutro.lock();
        cout << "Producer was born!" << endl;
        moutro.unlock();
        int trep = REP;
        char fname[36+1];

        while(trep--) {
                if(prod_pos == BUF_SIZE) exit(REP);
		s_prod.p();
                uuid_generate(buffer[prod_pos]);
                uuid_unparse(buffer[prod_pos], fname);
                string name(fname,sizeof(uuid_t)*2 + 4);
                ofstream file(name.c_str());
                file << name;
                //name.sync();
                file.close();
		prod_pos++;
		trep--;		
		s_cons.v();
        }
        exit(REP);
  }

  void* consumer(void* arg){
        moutro1.lock();
        cout << "Consumer was born!" << endl;
        moutro1.unlock();
        char fname[36+1];
	int prep=REP;
        while(prep--) {
                if(cons_pos==BUF_SIZE) exit(prep);
		s_cons.p();
                uuid_unparse(buffer[cons_pos], fname);
                ifstream file(fname);
                string str;
                file >> str;
                cout << "Consumed: " << str << endl;
		remove(fname);
		s_prod.v();
		prep--;
		cons_pos++;
        }
        //if(remove(fname)) cerr << "Error: " << errno << endl;<<<----------------------------------

        exit(prep);
 }


 int main(){
    int teste=0;
    int xablau=0;
    cout << "Massive Producer x Consumer Problem\n";

    /*for(int i=0;i<BUF_SIZE;i++){
        s_cons[i].p();
    }
    */

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

    //sleep(1);


    /*
    if(produced == (PRODS*REP)){
        for(int i = 0;i < BUF_SIZE;i++){
                xablau = (int) s_cons[i];
                if(xablau == 0)teste++;
        }

        if(teste == BUF_SIZE)finished = true;
    }
    */

    finished = true;

    for(int i = 0; i < CONS; i++){
            cons[i]->join(&status);
            con += status;
    }

    cout << "Total produced: " << produced << endl;
    cout << "Total consumed: " << con << endl;
    return 0;
}
