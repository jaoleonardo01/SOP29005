#include "Terminal.h"
#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

int main() {

    Terminal terminal;

//Abaixo tratamento para 'ctrl + c';

    signal(SIGINT, SIG_IGN);
    signal(SIGTERM, SIG_DFL);

    return terminal.run();
}
