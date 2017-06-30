#include "VirtualDisk.h"

class FileSystem {
private:
    FileSystem(char * disk_name);

public:
    static int create_file_system(char * disk_name);

    ~FileSystem();

    int create_file(char * file_name);
    int delete_file(char * file_name);
    int first_free_block();

    static FileSystem * instance() { return _fs; }

    VirtualDisk * disk() { return &_disk; }

    int MAX_FILES=40;
    int BLOCKS_PER_FILE=1;

private:
    VirtualDisk _disk;
    static FileSystem * _fs;
    char buf2[VirtualDisk::BLOCK_SIZE];
    
    struct stc_bloco{
	int dir;
	int dir_len;
	int data_ind;
    };

    struct stc_diretorio{
	int used;

	char name[20];
	int size;
	int head;
	int ref_count;
	int num_blocks;
    };
    stc_diretorio* dir;
    stc_bloco fsys;
};
