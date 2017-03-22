#ifndef TERMINAL_H_
#define TERMINAL_H_

#include <sys/types.h>
#include <map>
#include <string>
#include <vector>
#include "Process.h"

class Terminal {
public:
    Terminal();
    virtual ~Terminal();

    int run();
    pid_t pid();

private:
    std::string trim(std::string& str);
    std::vector<std::string> Tokenizer(std::string& str);
    bool extract_cmd(std::string cmd, std::vector<std::string> * tokens);

    pid_t _my_pid;
    pid_t _parent;
    typedef std::map<pid_t,Process*> ProcessMap;
    ProcessMap _processes;
    char hostname[50];
    std::string hostname2;
    char user[50];
    std::string user2;
    std::string lim;
    std::string comando, primeiro, comp;
    std::vector<std::string> token, token3;
    int ultimo,tamanho,k;
    bool block;
};

#endif /* TERMINAL_H_ */
