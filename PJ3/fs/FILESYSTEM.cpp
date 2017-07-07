#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <iostream>
#include "VirtualDisk.h"
#include "File.h"
#include "FileSystem.h"

using namespace std;
int main(int argc, char** argv) {
     
    bool sair=false;

    while ( not sair ){
        
        int opcao=0;
        
        cout << endl;
        cout << "#################################################" << endl;
        cout << "########### Filesystem - Virtual Disk ###########" << endl;
        cout << "#################################################" << endl;
        cout << endl;
        cout << "Selecione a opcao:" << endl;
        cout << "1 - Criar disco: "<< endl;
        cout << "2 - Criar arquivo:" << endl;
        cout << "3 - Deletar arquivo:" << endl;
	cout << "4 - Tamanho do arquivo (nome):" << endl;
	cout << "5 - Detalhes do arquivo (nome):" << endl;
	cout << "6 - Listar primeiro bloco livre:" << endl;
        cout << "0 - Para sair" << endl;
        cout << "OPCAO: ";
        cin >> opcao;
        
        switch (opcao){
		case 1:
		{		
			string j;
			char *j2=new char[200];
			cout<<"Digite um nome para o disco:\n"<<endl;
			cin >> j;
			j2=(char *)j.c_str();
			FileSystem::create_file_system(j2);
                	break;
            	}
            	case 2:
		{
			string a;
			char *a2=new char[200];
                	cout << "Digite um nome para o novo arquivo:\n" << endl;
			cin >> a;
			a2=(char *)a.c_str();
                	FileSystem::_fs->create_file(a2);
                	break;
                }
            	case 3:
		{
			string ab;
			char *ab2=new char[200];
	                cout << "Digite o nome do arquivo a ser deletado:\n" << endl;
			cin >> ab;
			ab2=(char *)ab.c_str();
			FileSystem::_fs->delete_file(ab2);
	                break;
		}          

            	case 4:
		{
			string t;
			char *t2=new char[200];
	                cout << "Digite o nome do arquivo a ser deletado:\n" << endl;
			cin >> t;
			t2=(char *)t.c_str();
			FileSystem::_fs->delete_file(t2);
	                break;
		}     

		case 5:
		{
			string l;
			char *l2=new char[200];
	                cout << "Listagem por nome (arquivo):\n" << endl;
			cin >> l;
			l2=(char *)l.c_str();
			cout<<l2<<endl<<endl;
			if(int j=FileSystem::_fs->list_file(l2) != -1){
				 j=FileSystem::_fs->list_file(l2);
				 cout<<"\n\nArquivo existe e esta no bloco: "<<FileSystem::_fs->dir[j].head <<endl;
				 cout<<"\n\ncom nome: "<<FileSystem::_fs->dir[j].name <<endl;
        		      	 cout<<"\n\ne com tamanho: "<<FileSystem::_fs->dir[j].size <<"\n\n\n\n\n\n\ "<< endl;
			}
			else cout<<"Arquivo inexistente\n\n"<<endl;

	                break;
		}   

		case 6:
		{
			cout<<"\n\nO primeiro bloco livre: \n\n" <<FileSystem::_fs->first_free_block()<<endl;
	                break;
		}              
                
            default:
	    { 
		sair=true;
		break;
	    }	
	}
    }   
    return 0;
}
