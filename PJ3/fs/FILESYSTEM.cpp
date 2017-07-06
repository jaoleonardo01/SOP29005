#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <iostream>
#include "VirtualDisk.h"
#include "File.h"

using namespace std;
int main(){
    char disco = 'F';
    FileSystem::create_file_system(&disco);
    cout << "teste"<< endl;

    return 0;
}
