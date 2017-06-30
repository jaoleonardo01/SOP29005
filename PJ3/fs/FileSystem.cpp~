#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "FileSystem.h"
#include "VirtualDisk.h"

using namespace std;

int FileSystem::create_file_system(char * disk_name){

	if (!disk_name) {
		fprintf(stderr, "invalid filesystem name\n");
		return 0;
	}
	
	// criar disco caso não exista
	VirtualDisk::create_disk(disk_name);//cria disco



	_fs = new FileSystem(disk_name); //usar disco que já existe

//	VirtualDisk::VirtualDisk(disk_name);//abre disco VirtualDisk * fs = 
//	VirtualDisk::write_block(0,buf2);//metadados


	//init _fs
}

FileSystem::FileSystem(char * disk_name) : _disk(disk_name)
{
	fsys.data_ind=VirtualDisk::DISK_BLOCKS-1;
	fsys.dir=1;
	fsys.dir_len=1;
	memset(buf2,0,VirtualDisk::BLOCK_SIZE);
	memcpy(buf2,&fsys,sizeof(struct stc_bloco));
	_disk.write_block(0,buf2);
	cout << "FS criado" << endl;
}

int FileSystem::create_file(char * file_name){


	int i;

	for(i=0;i<MAX_FILES;i++) {
		if(dir[i].used == 0) {
			dir[i] = *new stc_diretorio;
			struct stc_diretorio* entry = new stc_diretorio;
			if(entry == NULL) {
				cout<<"diretorio nao pode ser criado"<<endl;
				return -1;
			}

			dir[i].used = 1;
			strcpy(dir[i].name,file_name);
			dir[i].size = 0;
			dir[i].head = first_free_block();
			dir[i].ref_count = 0;
			dir[i].num_blocks = BLOCKS_PER_FILE;
			return i;
		}
	}
}

int FileSystem::delete_file(char * file_name){

}

FileSystem::~FileSystem(){}

int FileSystem::first_free_block()
{
	char buf[VirtualDisk::BLOCK_SIZE] = "";
	_disk.read_block(fsys.data_ind,buf);
	int i,j,temp;

	for(i=3;i<VirtualDisk::BLOCK_SIZE;i++) {
		if(buf[i] == '\0') {
			for(j=0;j<BLOCKS_PER_FILE;j++) {
				if(buf[i+j] == '1') {
					i += BLOCKS_PER_FILE;
					continue;
				}
			}
			for(j=0;j<BLOCKS_PER_FILE;j++) {
				buf[i+j] = '1';
				cout << "bloco alocado :" << (i+j) << "->" << buf[i+j] << endl;
			}
			_disk.write_block(fsys.data_ind,buf);
			return i;
		}
	}

	return -1; // no free blocks are available to allocate
}
