/*
 * Author: arliones.hoeller
 * Institution: IFSC SJE
 * Date: June 2017
 *
 */

#ifndef __VIRTUAL_DISK_H__
#define __VIRTUAL_DISK_H__

class VirtualDisk {
public:
    static const int DISK_BLOCKS = 8192; // Number of blocks on the disk
    static const int BLOCK_SIZE = 4096;  // Block size

    static int create_disk(char * vdisk_name); // Create an empty virtual disk file

    VirtualDisk(char * vdisk_name); // Open a virtual disk file
    ~VirtualDisk(); // Close a previously opened disk file

    int write_block(int block, char * buffer); // Write a block of data to the disk
    int read_block (int block, char * buffer); // Read a block of data from the disk

private:
    int active; // is the virtual disk open (active)?
    int handle; // file handle to virtual disk
};

#endif // __VIRTUAL_DISK_H__

