#include <iostream>
#include "gameboy.h"
#include "cpu.h"
#include <windows.h>
using namespace std;

int main(int argc, char** argv){
    Gameboy gb;
    std::string game(argv[1]);
    try{
        gb.LoadGame(game);
    }
    catch(std::exception&e){
        cout << e.what();
        return 1;
    }
    std::string input;
    while(true){
        cout << "n: emulate cycle | p: print cpu status | e: exit" << std::endl;
        cin >> input;
        if(input == "n"){
            try{
                gb.EmulateCycle(); 
            }
            catch(std::exception& e){
                cout << e.what();
                return 1;
            }
        }
        else if(input == "p"){
            gb.printStatus(); 
        }
        else if(input == "e"){
            break;
        }
        else{
            cout << "invalid entry. try again" << std::endl;
        }
    }
    return 0;
}