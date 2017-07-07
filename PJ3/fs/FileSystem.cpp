#include <dirent.h>;
#include "VirtualDisk.h";
#include "FileSystem.h";

#define _livre -1;
#define _eof  -2;
#define _FATSYS -3;

using namespace std;

FileSystem * FileSystem::_fs;

int FileSystem::create_file_system(char * disk_name){

	if (!disk_name) {
		fprintf(stderr, "Nome do sistema de arquivos invalido!\n");
		return 0;
	}
	VirtualDisk::create_disk(disk_name);//cria disco
	_fs = new FileSystem(disk_name); 
 
}

FileSystem::FileSystem(char * disk_name) : _disk(disk_name)
{
	_disk.create_disk(disk_name);
        fsys.data_ind=VirtualDisk::DISK_BLOCKS-1;
	fsys.dir=1;
	fsys.dir_len=65;
        fsys.begin_FAT=66;
        fsys.len_FAT= 8;
	memset(buf2,0,VirtualDisk::BLOCK_SIZE);
	memcpy(buf2,&fsys,sizeof(struct stc_bloco));
	_disk.write_block(0,buf2);
	cout << "Sistema de arquivos foi criado..." << endl;
}

int FileSystem::create_file(char * file_name){



	for(int ki=0;ki<MAX_FILES;ki++) {
		if(dir[ki].used == 0) {
			dir[ki] = *new stc_diretorio;
			struct stc_diretorio* entry = new stc_diretorio;
			if(entry == NULL) {
				cout<<"diretorio nao pode ser criado"<<endl;
				return -1;
			}

			dir[ki].used = 1;
			strcpy(dir[ki].name,file_name);
			dir[ki].size = 0;
			dir[ki].head = first_free_block();
			dir[ki].ref_count = 0;
			dir[ki].num_blocks = BLOCKS_PER_FILE;
			return ki;
		}
	}
}

int FileSystem::delete_file(char * file_name){
    int end_int = del_dir_file(file_name);
    if (end_int > 0){
        cout << "Arquivo " << file_name << "deletado!" << endl;
    }
}

int FileSystem::del_dir_file(char * file_name){
    int aux=0, aux1=0;
    char buff[VirtualDisk::BLOCK_SIZE]= "";
    for(;aux<MAX_FILES;aux++){
        if(strcmp(_fs->dir[aux].name,file_name)== 0){
            dir[aux].used = 0;
            dir[aux].size = 0;
            strcpy(dir[aux].name,"");
            dir[aux].ref_count = 0;
            for(;aux1<LOOP;aux1++){
                if(descricaoarquivo[aux1].first_block == aux){
                    descricaoarquivo[aux1].first_block = -1;
                    descricaoarquivo[aux1].offset_read = 0;
                    descricaoarquivo[aux1].offset_write = 0;
                    descricaoarquivo[aux1].active = 0;
                }
            }
            _fs->_disk.read_block(VirtualDisk::DISK_BLOCKS-1,buff);
            for(int auxxx=0;auxxx<dir[aux].num_blocks;auxxx++){
                buff[dir[aux].head + auxxx] = '\0';
            }
            dir[aux].head = -1;
            dir[aux].num_blocks = 0;
            _fs->_disk.write_block(VirtualDisk::DISK_BLOCKS-1,buff);
            return aux;
        }
    }
    return -1;
}

FileSystem::~FileSystem(){
  int c=0,aux=0,tamanho=fsys.dir_len;  
  char buffer[VirtualDisk::BLOCK_SIZE]="";
  char* ponteiro_char = "";
  struct stc_diretorio* queue;
  struct stc_diretorio* d;
  for(c=0;c<LOOP;c++){
      if(descricaoarquivo[c].active==1){
          descricaoarquivo[c].offset_read=0;
          descricaoarquivo[c].active=0;
          descricaoarquivo[c].first_block=0;     
      }
  }
  for(aux=0,queue=(struct stc_diretorio*)d;aux<tamanho;aux++){
      memcpy(buffer,ponteiro_char,VirtualDisk::BLOCK_SIZE);
      _disk.write_block(fsys.data_ind,buffer);
      ponteiro_char += VirtualDisk::BLOCK_SIZE;
  }
  free(d);  
  cout << "Desmontou o sistema de arquivos" << endl;
}
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
	return -1; 
}

int FileSystem::list_file(char* file_name){


	for(int ik=0;ik<MAX_FILES;ik++) {
		if(dir[ik].used == 1 && strcmp(dir[ik].name,file_name) == 0) {
			return ik;
		}
	}

	return -1; // arquivo n encontrado
}
