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
	// criar disco caso não exista
	VirtualDisk::create_disk(disk_name);//cria disco
	_fs = new FileSystem(disk_name); 
        
}

FileSystem::FileSystem(char * disk_name) : _disk(disk_name)
{
	_disk.create_disk(disk_name); //Monta o Disco
        fsys.data_ind=VirtualDisk::DISK_BLOCKS-1;
	fsys.dir=1;
	fsys.dir_len=65;
        fsys.begin_FAT=66;
        fsys.len_FAT= 8; //segundo calculos chegamos a 8 blocos o espaçamento da FAT
	memset(buf2,0,VirtualDisk::BLOCK_SIZE);//0 nas posições do buf2
	memcpy(buf2,&fsys,sizeof(struct stc_bloco)); //carregando as info no stc_bloco
	_disk.write_block(0,buf2); //escreve as info do buf2 no bloco 0
	cout << "Sistema de arquivos foi criado..." << endl;
}

int FileSystem::create_file(char * file_name){
	int i;
        int sum_b_l=fsys.begin_FAT + fsys.len_FAT;
        stc_bloco * blc = new stc_bloco;
        int32_t Buffer_FAT[VirtualDisk::DISK_BLOCKS];
	for(i=0;i<MAX_FILES;i++) {
		if(_fs->dir[i].used == 0) {
			_fs->dir[i] = *new stc_diretorio;
			struct stc_diretorio* entry = new stc_diretorio;
			if(entry == NULL) {
				cout<<"diretorio nao pode ser criado"<<endl;
				return -1;
			}
                        _fs->dir[i].used=1;
			strcpy(_fs->dir[i].name,file_name);
			_fs->dir[i].size = 0;
			_fs->dir[i].head = first_free_block();
                        _fs->dir[i].ref_count=0;
                        memset(buf2,0,VirtualDisk::BLOCK_SIZE); 
                        memcpy(buf2,&dir,sizeof(struct stc_diretorio)); 
	
                        for (int aux1= blc->dir; aux1 < blc->dir_len+1; aux1++){
                            _fs->_disk.write_block(aux1,buf2); 
                        }
                        for(int aux2; aux2 < sum_b_l; aux2++){
                            Buffer_FAT[aux2]= _FATSYS;
                        }
                        for(int aux3=sum_b_l;aux3<VirtualDisk::DISK_BLOCKS;aux3++){
                            Buffer_FAT[aux3] = _livre; 
                        }
			return i;
		}
	}
}

int FileSystem::delete_file(char * file_name){

}

FileSystem::~FileSystem(){
  int c=0,aux=0,tamanho=fsys.dir_len;  
  char buffer[VirtualDisk::BLOCK_SIZE]="";
  char* ponteiro_char = "";
  for(c=0;c<LOOP;c++){
      if(descricaoarquivo[c].active==1){
          descricaoarquivo[c].offset_read=0;
          descricaoarquivo[c].active=0;
          descricaoarquivo[c].first_block=0;     
      }
  }
  for(aux=0;aux<tamanho;aux++){
      memcpy(buffer,ponteiro_char,VirtualDisk::BLOCK_SIZE);
      _disk.write_block(fsys.data_ind,buffer);
      ponteiro_char += VirtualDisk::BLOCK_SIZE;
  }
  free(dir);  
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

	return -1; // no free blocks are available to allocate
}
