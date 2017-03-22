/*
 * Terminal.cpp
 *
 *  Created on: Oct 28, 2015
 *      Author: arliones
 */

#include "Terminal.h"
#include <unistd.h>
#include <string>
#include <iostream>
#include <string>
#include <signal.h>
#include <sys/types.h>
#include <ctype.h>

using namespace std;

Terminal::Terminal()
{
    //...
}

Terminal::~Terminal()
{
    //...
}

int Terminal::run()
{
	getlogin_r(user, sizeof user);
	gethostname(hostname, sizeof hostname);

//Verificar se o usuario eh root ou nao.

	user2=user;
	if (user2.compare("root") != 0){
		lim="$";
	} else lim="#";

LOOP:
	do{
 		cout << "<" << user << "/" << hostname << ">" << lim << " ";
		getline(cin,comando);		
		if(comando.empty()) goto LOOP; //tratamento caso comando zerado.
		comando=trim(comando); //retira os espacos no comeco e fim da sequencia de strings.
		token=Tokenizer(comando);
		primeiro=token[0];
		tamanho=token.size();
		for(k=1;k<tamanho;k++){
			token3.push_back(token[k]);
		}
		comp=token[tamanho-1];
		if(comp.compare("&")==0) block=false;
		else block=true;
		if(comp.compare("exit")==0) break;
		Process Processo(primeiro, token, block);
	} while(true);
}



// HERE IS HOW TO TRIM SPACES FROM THE BORDERS OF A STRING
string Terminal::trim(string& str)
{
    size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last-first+1));
}
//Tokenizer

std::vector<std::string> Terminal::Tokenizer(std::string& str)
{
	std::vector<std::string> token2;
	char delim=' ';
	int aux0=0, aux1=0,aux2=0;
	string palavra;
	while(aux0!=string::npos){
		aux0=str.find(delim,aux1);
		aux2=aux0-aux1;
		palavra=str.substr(aux1,aux2);
		aux1=aux0+1;
		token2.push_back(palavra);
		//cout << "teste teste"<< token2[0]  << endl;
	}
	
	return token2;
}
