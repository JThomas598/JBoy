#define SDL_MAIN_HANDLED 1
#include "debugger.h"
#include "signal.h"
#include <stdio.h>
#include <sstream>
#include <SDL2/SDL.h>
#include <stdexcept>
#include <iomanip>
#include <stdbool.h>
#include <cstring>

using namespace std;

/*
DEBUG MODE FEATURES:
    -stepping
    -breakpoints
    -CPU state info
    -Current instruction info
CONSTRAINTS
    -Allow user to declare breakpoints before and during runtime.
    -Allow user to request state of system only during runtime.
    -Opening the debugger will require a rom as an numArgument. Close if none provided.
IMPLEMENTATION:
*/

typedef enum Command{
    STEP,
    RUN,
    BREAKPOINT,
    EXIT
}Command;

int handleEvent();
void parseCommand(string s, string& com, Regval16& numArg, string &strArg);

using namespace std;



int main(int numArgc, char** argv){
    SDL_Init(SDL_INIT_EVERYTHING);
    Gameboy gb;
    try{
        gb.loadGame(argv[1]);
    }
    catch(std::exception&e){
        SDL_Quit();
        cout << e.what();
        return 1;
    }
    Debugger debug(gb);
    char input[BUFSIZ];
    string com;
    Regval16 numArg = 0;
    string strArg;
    cout << "Welcome to the JK EMU Debugger." << endl;
    //runtime phase
    while(true){ 
        if(!handleEvent()){
            SDL_Quit();
            return 0;
        }
        cout << "JDB> ";
        cin.getline(input, BUFSIZ);
        try{
            parseCommand(string(input), com, numArg, strArg);
        }
        catch(std::exception& e){
            cout << e.what() << endl;
            continue;
        }
        if(com == "break" || com == "b"){
            if(debug.addBreakpoint(numArg)){
                cout << "breakpoint added at address 0x" 
                << std::hex << std::setw(4) << std::setfill('0') << numArg << endl;
            }
            else
                cout << "breakpoint already exists at specified address." << endl;
        }
        else if(com == "enable"){
            if(strArg == "vblank"){
                debug.enableSignal(VBLANK_SIGNAL);
                cout << "enabled VBLANK signal" << std::endl;
            } 
            else if(strArg == "frame"){
                debug.enableSignal(FRAME_SIGNAL);
                cout << "enabled FRAME signal" << std::endl;
            }
        }
        else if(com == "next" || com == "n")
            debug.step(numArg);
        else if(com == "read")
            std::cout << "0x" << std::hex << numArg << " = 0x" << (int)debug.readMem(numArg) << std::endl;
        else if(com == "run" || com == "r")
            debug.runToBreakpoint();
        else if(com == "status" || com == "s")
            debug.printStatus();
        else if(com == "exit" || com == "e"){
            SDL_Quit();
            return 1;
        }
        else{
            cout << "[ERROR] command does not exist." << endl;
        }
    }
}

int handleEvent(){
    SDL_Event event;
    if(SDL_PollEvent(&event)){
        switch(event.type){
            case SDL_WINDOWEVENT:
                break;
            case SDL_QUIT:
                return 0;
        }
    }
    return 1;
}

void parseCommand(string s, string& com, Regval16& numArg, string& strArg){
    if(s.size() == 0){
        return ;
    }
    istringstream iss(s);
    if(iss >> com){
        if(com == "break" || com == "next" || com == "n" || com == "read"){
           if(!(iss >> std::hex >> numArg)){
                throw std::invalid_argument("[ERROR] Invalid or non-existent argument to given command.");
            }
        }
        else if(com == "enable"){
           if(!(iss >> std::hex >> strArg)){
                throw std::invalid_argument("[ERROR] Invalid or non-existent argument to given command.");
            } 
        }
    }
    else{
        throw std::invalid_argument("[ERROR] Could not parse given command");
    }
}