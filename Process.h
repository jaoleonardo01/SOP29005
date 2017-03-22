/*
 * Process.h
 *
 *  Created on: Oct 28, 2015
 *      Author: arliones
 */

#ifndef PROCESS_H_
#define PROCESS_H_

#include <sys/types.h>
#include <string>
#include <vector>
#include <string.h>

class Process {
public:
    Process(std::string & program, std::vector<std::string> & params, bool block);
    virtual ~Process();

    //pid_t wait(int *status);
    //int wait();
    int pid();
    int status();

private:
    void exec(std::string & program, std::vector<std::string> & params);
    static char * string_to_c_convert(const std::string & s);
    int pid2, ppid2;

private:
    pid_t  _my_pid;
    pid_t  _parent;
    int _status;
    std::string param1,param2;
};

#endif /* PROCESS_H_ */
