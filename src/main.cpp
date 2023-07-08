#include <iostream>
#include "registers.h"
#include <windows.h>
using namespace std;

int main(int argc, char** argv){
    Registers reg;
    reg.Write(ACC, (RegVal_8)7);
    reg.Print();
}