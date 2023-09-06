#define SDL_MAIN_HANDLED 1
#include <stdio.h>
#include <sstream>
#include <SDL2/SDL.h>
#include <vector>
#include <stdexcept>
#include <iomanip>
#include <stdbool.h>
#include "debugger.h"
#include "instructions.h"

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
    -Opening the debugger will require a rom as an argument. Close if none provided.
IMPLEMENTATION:
*/

typedef enum Command{
    STEP,
    RUN,
    BREAKPOINT,
    EXIT
}Command;

int handleEvent();
void parseCommand(string s, string& com, RegVal_16& arg);
void handleCommand(string& com, RegVal_16& arg, Debugger& debug);

using namespace std;



int main(int argc, char** argv){
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
    RegVal_16 arg;
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
            parseCommand(string(input), com, arg);
        }
        catch(std::exception& e){
            cout << e.what() << endl;
            continue;
        }
        if(com == "break" || com == "b"){
            if(debug.addBreakpoint(arg)){
                cout << "breakpoint added at address 0x" 
                << std::hex << std::setw(4) << std::setfill('0') << arg << endl;
            }
            else{
                cout << "breakpoint already exists at specified address." << endl;
            }
        }
        else if(com == "next" || com == "n"){
            debug.step();
        }
        else if(com == "run" || com == "r"){
            debug.runToBreakpoint();
        }
        else if(com == "status" || com == "s"){
            debug.printStatus();
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

void parseCommand(string s, string& com, RegVal_16& arg){
    if(s.size() == 0){
        return ;
    }
    istringstream iss(s);
    if(iss >> com){
        if(com == "break"){
           if(!(iss >> std::hex >> arg)){
                throw std::invalid_argument("[ERROR] Invalid or non-existent argument to given command.");
            }
        }
    }
    else{
        throw std::invalid_argument("[ERROR] Could not parse given command");
    }
}

void handleCommand(string& com, RegVal_16& arg, Debugger& debug){

}