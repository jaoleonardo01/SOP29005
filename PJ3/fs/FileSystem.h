#include "VirtualDisk.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

class FileSystem {

private:
    FileSystem(char * disk_name);

public:
    static int create_file_system(char * disk_name);

    ~FileSystem();

    int create_file(char * file_name);
    int delete_file(char * file_name);
    int first_free_block();
    int del_dir_file(char* file_name);
    int list_file(char* file_name);


    static FileSystem * instance() { return _fs; }

    VirtualDisk * disk() { return &_disk; }

    static FileSystem * _fs;

    int MAX_FILES=40;
    int BLOCKS_PER_FILE=1;
    #define LOOP 32
    #define MAX_F_NAME 64
    #define MAX_FILE_SIZE 16384

private:
    VirtualDisk _disk;
    char buf2[VirtualDisk::BLOCK_SIZE];
public:    
    struct stc_file{
        int32_t active;
        int32_t first_block;
        int32_t offset_read;
        int32_t offset_write;
           
    };
    
    struct stc_bloco{
	int32_t dir; 
	int32_t dir_len; // tamanho da tabela de diretorios
	
        int32_t data_ind; //informações de dados, inicia os dados.
        
        int32_t begin_FAT; // inicio da FAT
        int32_t len_FAT; //comprimento da tabela FAT
    };

    struct stc_diretorio{
	int32_t used=0; //se está em uso
	char name[20]; // nome do arquivo
	int32_t size; // tamanho do arquivo
	int32_t head; // primeiro bloco de dados
	int32_t ref_count; //numero de descritores usados
        int32_t num_blocks;
    };
    stc_file descricaoarquivo[LOOP];
    stc_diretorio dir[60];
    stc_bloco fsys;
};

