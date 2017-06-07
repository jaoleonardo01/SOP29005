class FileSystem {
private:
    FileSystem(char * disk_name);

public:
    static int create_file_system(char * disk_name);

    ~FileSystem();

    int create_file(char * file_name);
    int delete_file(char * file_name);

    static FileSystem * instance() { return _fs; }

    VirtualDisk * disk() { return &_disk; }

private:
    VirtualDisk _disk;
    static FileSystem * _fs;
};   
