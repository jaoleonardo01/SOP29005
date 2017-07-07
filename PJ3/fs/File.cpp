#include <dirent.h>;
#include <unistd.h>
#include <sys/types.h>
#include "VirtualDisk.h";
#include "FileSystem.h";
#include "File.h";
using namespace std;

int BLC_P_F=1;

File::File(char * file_name){
	nome=file_name;

	int teste = FileSystem::_fs->list_file(file_name);

	if(teste < 0) { //arquivo inexistente
		int teste2 = FileSystem::_fs->create_file(file_name);

		if(teste2 < 0) { // erro, num. max de arquivos atingido
			cerr << "erro, num. max de arquivos atingido\n";
		} else {
			cout<<"arquivo "<< file_name <<"  criado\n";
		}
	} else { // file with same name already exists
		cerr << "ja existe arquivo com o mesmo nome\n";
	}

}


File::~File(){ //deve fechar o arquivo
	
	int teste = FileSystem::_fs->list_file(this->nome);//necessario implementar no FileSystem
	FileSystem::_fs->dir[FileSystem::_fs->descricaoarquivo[teste].first_block].ref_count--;
	FileSystem::_fs->descricaoarquivo[teste].active = 0;
	cout<<"Arquivo fechado: " << FileSystem::_fs->dir[FileSystem::_fs->descricaoarquivo[teste].first_block].name;
}


int File::read(int file, char * buf, size_t bytes){

	if(!FileSystem::_fs->descricaoarquivo[file].active) {
		cerr<<"Arquivo inexiste\n"<<endl;
		return -1;
	}

	int i, j, rcount = 0;
	int offset = FileSystem::_fs->descricaoarquivo[file].offset_read;
	char buf4[VirtualDisk::BLOCK_SIZE] = "";

	int head = FileSystem::_fs->dir[FileSystem::_fs->descricaoarquivo[file].first_block].head;
	int bcount = 0;
	
	if(offset >= VirtualDisk::BLOCK_SIZE) {
		if(head + BLC_P_F - 1 < head + offset/VirtualDisk::BLOCK_SIZE) {
			cerr<<"memoria excedida\n"<<endl;
			return -1;
		}
		head += offset/VirtualDisk::BLOCK_SIZE;
		bcount = offset/VirtualDisk::BLOCK_SIZE;
		offset = offset % VirtualDisk::BLOCK_SIZE;
	}

	FileSystem::_fs->disk()->read_block(head,buf4);

	/* reading from the first block */
	for(i=offset;i<VirtualDisk::BLOCK_SIZE;i++) {
		buf[i-offset] = buf4[i];
		rcount++;
		if(rcount == (int)bytes) {
			FileSystem::_fs->descricaoarquivo[file].offset_read += (int32_t)rcount;
			strcpy(buf4,"");
			return (int)rcount;
		}
	}

	if(rcount == (int)bytes) {
		strcpy(buf4,"");
		FileSystem::_fs->descricaoarquivo[file].offset_read += (int)rcount;
		return (int)rcount;
	}

		strcpy(buf4,"");
	while(rcount < bytes && bcount < BLC_P_F) {
		FileSystem::_fs->disk()->read_block(head,buf4);
		for(j=0;j<VirtualDisk::BLOCK_SIZE;j++) {
			buf[i-offset] = buf4[j];
			rcount++;
			i++;
			if(rcount == (int)bytes) {
				FileSystem::_fs->descricaoarquivo[file].offset_read += (int)rcount;
				return (int)rcount;
			}
		}
		bcount++;
		head++;
		strcpy(buf4,"");
	}
	FileSystem::_fs->descricaoarquivo[file].offset_read += (int)rcount;
	return rcount;
}

int File::write(int file, char * buf, size_t bytes){

		int i, j, wcount = 0;
		char buf2[VirtualDisk::BLOCK_SIZE] = "";

		int offset = FileSystem::_fs->descricaoarquivo[file].offset_write;

		int head = FileSystem::_fs->dir[FileSystem::_fs->descricaoarquivo[file].first_block].head;
		int bcount2 = 0;

		if(offset >= VirtualDisk::BLOCK_SIZE) {
			if(head + BLC_P_F - 1 < head + offset/VirtualDisk::BLOCK_SIZE) {
				cerr<<"memoria excedida\n"<<endl;
				return -1;
			}
			head += offset/VirtualDisk::BLOCK_SIZE;
			bcount2 = offset/VirtualDisk::BLOCK_SIZE;
			offset = offset % VirtualDisk::BLOCK_SIZE;
		}

		FileSystem::_fs->disk()->read_block(head,buf2);

		for(i=offset;i<VirtualDisk::BLOCK_SIZE;i++) {
			if(wcount == (int)bytes or (i-offset) == strlen(buf)) {
				FileSystem::_fs->disk()->write_block(head,buf2);
				FileSystem::_fs->descricaoarquivo[file].offset_write += wcount;
				return wcount;
			}

			buf2[i] = buf[i-offset];
			wcount++;
		}

		FileSystem::_fs->disk()->write_block(head,buf2);

		if(wcount == (int)bytes) {
			FileSystem::_fs->descricaoarquivo[file].offset_write += wcount;
			return wcount;
		}

		strcpy(buf2,"");

		while(wcount < (int)bytes) {
			for(j=0;j<VirtualDisk::BLOCK_SIZE;j++) {
				buf2[j] = buf[i-offset];
				wcount++;
				i++;
				if(i == strlen(buf) or wcount == (int)bytes) {
					FileSystem::_fs->descricaoarquivo[file].offset_write += wcount;
					FileSystem::_fs->disk()->write_block(head+bcount2-1,buf2);
					return wcount;
				}
			}

			FileSystem::_fs->disk()->write_block(head+bcount2-1,buf2);
			bcount2++;
		}

		FileSystem::_fs->descricaoarquivo[file].offset_write += wcount;
		return wcount;
	cerr<<"Erro ao abrir arquivo\n"<<endl;
	return -1;
}

int File::size(char * file_name){

	int teste = FileSystem::_fs->list_file(file_name);
	return FileSystem::_fs->dir[teste].size;

}

int File::lseek(int filedes, int offset){

	FileSystem::_fs->descricaoarquivo[filedes].offset_write=offset;
	FileSystem::_fs->descricaoarquivo[filedes].offset_read=offset;
}
