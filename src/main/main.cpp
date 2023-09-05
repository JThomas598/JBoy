#define SDL_MAIN_HANDLED 1
#include <stdio.h>
#include <SDL2/SDL.h>
#include "gameboy.h"

using namespace std;


int handleEvent();
void limitCycleRate();

int main(int argc, char** argv){
    SDL_Init(SDL_INIT_EVERYTHING);
    Gameboy gb;
    try{
        gb.loadGame(argv[1]);
    }
    catch(std::exception&e){
        cout << e.what();
        return 1;
    }
    while(true){ 
        if(!handleEvent()){
            SDL_Quit();
            return 0;
        }
            try{
                gb.emulateCycle();
            }
            catch(std::exception&e){
                cout << e.what();
                return 1;
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
