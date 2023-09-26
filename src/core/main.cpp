#define SDL_MAIN_HANDLED 1
#include <stdio.h>
#include "SDL2/SDL.h"
#include "gameboy.h"

using namespace std;


int handleEvent(SDL_Event* event, Gameboy& gb);
void handleJoypadEvent(SDL_KeyboardEvent* key);
int timedPollEvent();

constexpr int EVENT_POLLS_PER_SEC = 100;


typedef enum EventResult{
    CONTINUE,
    QUIT
}EventResult;


int main(int argc, char** argv){
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_SetHint(SDL_HINT_WINDOWS_DISABLE_THREAD_NAMING, "1");
    Signal signal;
    signal.enableSignal(FRAME_SIGNAL);
    Gameboy gb;
    try{
        gb.loadGame(argv[1]);
    }
    catch(std::exception&e){
        cout << e.what();
        return 1;
    }
    while(true){ 
        if(signal.signalRaised(FRAME_SIGNAL)){
            SDL_Event event;
            if(SDL_PollEvent(&event)){
                if(handleEvent(&event, gb) == QUIT){
                   SDL_Quit();
                   return 0; 
                }
            }
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

int timedPollEvent(){
    static std::chrono::high_resolution_clock::time_point lastPollTime = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double> timeBetweenPolls(1.0 / EVENT_POLLS_PER_SEC);
    std::chrono::high_resolution_clock::time_point currentPollTime = std::chrono::high_resolution_clock::now();
    currentPollTime = std::chrono::high_resolution_clock::now();
    if((currentPollTime - lastPollTime) > timeBetweenPolls){
        lastPollTime = currentPollTime;
    }
    return CONTINUE;
}

int handleEvent(SDL_Event* event, Gameboy& gb){
    switch(event->type){
        case SDL_QUIT:
            return QUIT;
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            SDL_KeyboardEvent keyEvent = event->key;
            Regval8 currState = gb.getJoypad();
            Regval8 newState;
            switch(keyEvent.keysym.sym){
                case SDLK_LEFT:
                    if(keyEvent.type == SDL_KEYDOWN){
                        newState = currState & ~LEFT_PAD_MASK;
                    }
                    else{
                        newState = currState | LEFT_PAD_MASK;
                    }
                    break;
                case SDLK_RIGHT:   
                    if(keyEvent.type == SDL_KEYDOWN){
                        newState = currState & ~RIGHT_PAD_MASK;
                    }
                    else{
                        newState = currState | RIGHT_PAD_MASK;
                    }
                    break;
                case SDLK_UP:   
                    if(keyEvent.type == SDL_KEYDOWN){
                        newState = currState & ~UP_PAD_MASK;
                    }
                    else{
                        newState = currState | UP_PAD_MASK;
                    }
                    break;
                case SDLK_DOWN:   
                    if(keyEvent.type == SDL_KEYDOWN){
                        newState = currState & ~DOWN_PAD_MASK;
                    }
                    else{
                        newState = currState | DOWN_PAD_MASK;
                    }
                    break;
                case SDLK_z:   
                    if(keyEvent.type == SDL_KEYDOWN){
                        newState = currState & ~A_BUTTON_MASK;
                    }
                    else{
                        newState = currState | A_BUTTON_MASK;
                    }
                    break;
                case SDLK_x:   
                    if(keyEvent.type == SDL_KEYDOWN){
                        newState = currState & ~B_BUTTON_MASK;
                    }
                    else{
                        newState = currState | B_BUTTON_MASK;
                    }
                    break;
                case SDLK_c:   
                    if(keyEvent.type == SDL_KEYDOWN){
                        newState = currState & ~SELECT_BUTTON_MASK;
                    }
                    else{
                        newState = currState | SELECT_BUTTON_MASK;
                    }
                    break;
                case SDLK_v:   
                    if(keyEvent.type == SDL_KEYDOWN){
                        newState = currState & ~START_BUTTON_MASK;
                    }
                    else{
                        newState = currState | START_BUTTON_MASK;
                    }
                    break;
                default:
                    newState = currState;
                    break;
            }
            gb.setJoypad(newState); 
    }
    return CONTINUE;
}
