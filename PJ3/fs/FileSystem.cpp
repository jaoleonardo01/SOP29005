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
	this->data_ind=VirtualDisk::DISK_BLOCKS-1;
	this->dir=1;
	this->dir_len=1;
	memset(fat,0,VirtualDisk::BLOCK_SIZE);
	memcpy(fat,&this,sizeof(this);
	_disk.write_block(0,fat);
	cout << "FS criado" << endl;
}

int FileSystem::create_file(char * file_name){

	int i=first_free_block();
	_fs->fat[i]=
	

}


int FileSystem::delete_file(char * file_name){
	int i,j;

	for(i=0;i<MAX_FILES;i++) {
		if(strcmp(dir[i].name,file_name) == 0) {
			dir[i].used = 0;
			strcpy(dir[i].name,"");
			dir[i].length = 0;
			dir[i].count = 0;

			for(j=0;j<MAX_FILEDES;j++) {
				if(filedes[j].file == i) {
					filedes[j].used = 0;
					filedes[j].file = -1;
					filedes[j].read_offset = 0;
					filedes[j].write_offset = 0;
				}
			}

			char buf[BLOCK_SIZE] = "";
			block_read(DISK_BLOCKS-1,buf);
			// free allocated blocks to file
			for(j=0;j<dir[i].num_blocks;j++) {
				buf[dir[i].first+j] = '\0';
			}

			dir[i].first = -1;
			dir[i].num_blocks = 0;
			block_write(DISK_BLOCKS-1,buf);

			return i;
		}
	}

return -1; // file not found in directory
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
