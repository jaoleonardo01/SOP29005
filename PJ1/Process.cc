/*
 * Process.cpp
 *
 *  Created on: Oct 28, 2015
 *      Author: arliones
 */
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include "Process.h"
#include <unistd.h>
#include <sys/wait.h>
#include <sys/errno.h>
#include <signal.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <string.h>

using namespace std;

Process::Process(string & program, vector<string> & params, bool block)
{
	vector<char*> pc2;

	
	for(int i=0;i<params.size();i++){
		pc2.push_back(string_to_c_convert(params[i])); //Conversao necessaria, conforme requisitos do exec;
	}		
	pc2.push_back(NULL);
		
	pid2=fork();
	if(pid2==-1){
		perror("fork falhou");
	}
	else if(pid2==0){
		if(execvp(pc2[0], &pc2[0])<0) cout<<"Comando nao encontrado!"<<endl;
	}

	else if(block==true)	waitpid(pid2,&_status,0);
}

Process::~Process()
{
    //...
}

int Process::pid()
{
    return pid2;
}

//...



// HERE IS HOW TO TRANSFORM A vector<string> TO A char * const []
// C++ STD provides a series of algorithms. We will use one of them: transform.
// transform will take a element series from a container (ex. vector) and call
// the function pointer passed as argument to transform each member of the series.
// The result of the transformation is stored in another container.
//
// For instance, to transform all strings in a vector<string> to c-like strings in
// a vector<char*>, use transform like this:
// vector<string> input;
// vector<char *> argv;
// transform(input.begin(), input.end(), back_inserter(argv), string_to_c_convert);
//
// If you need, you can cast the vector<char*> argv to a c-like array of char*:
// char * const args[] = (char*const*)&argv;
char * Process::string_to_c_convert(const string & s)
{
    char *pc = new char[s.size()+1];
    strcpy(pc, s.c_str());
    return pc;
}
