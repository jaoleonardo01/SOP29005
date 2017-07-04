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

    int MAX_FILES=64;
    int BLOCKS_PER_FILE=1;

private:
	VirtualDisk _disk;
	static FileSystem * _fs;
	char buf2[VirtualDisk::BLOCK_SIZE];
	int dir,dir_len,data_ind,used,length,first,count,num_blocks;
	char name[64];
	int fat[8191];
};
