#define SDL_MAIN_HANDLED 1
#include <stdio.h>
#include "SDL2/SDL.h"
#include "gameboy.h"

using namespace std;


int handleEvent(SDL_Event* event, Gameboy& gb);
void handleJoypadEvent(SDL_KeyboardEvent* key);
string omitFileExt(const std::string& filepath);
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
        gb.loadSram(omitFileExt(argv[1]) + ".sav");
    }
    catch(std::exception&e){
        cout << e.what();
        return 1;
    }
    while(true){ 
        if(signal.signalRaised(FRAME_SIGNAL)){
            SDL_Event event;
            if(SDL_PollEvent(&event) && handleEvent(&event, gb) == QUIT){
                gb.saveSram(omitFileExt(argv[1]) + ".sav"); 
                SDL_Quit();
                return 0; 
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
                case SDLK_a:
                    if(keyEvent.type == SDL_KEYDOWN){
                        newState = currState & ~LEFT_PAD_MASK;
                    }
                    else{
                        newState = currState | LEFT_PAD_MASK;
                    }
                    break;
                case SDLK_d:   
                    if(keyEvent.type == SDL_KEYDOWN){
                        newState = currState & ~RIGHT_PAD_MASK;
                    }
                    else{
                        newState = currState | RIGHT_PAD_MASK;
                    }
                    break;
                case SDLK_w:   
                    if(keyEvent.type == SDL_KEYDOWN){
                        newState = currState & ~UP_PAD_MASK;
                    }
                    else{
                        newState = currState | UP_PAD_MASK;
                    }
                    break;
                case SDLK_s:   
                    if(keyEvent.type == SDL_KEYDOWN){
                        newState = currState & ~DOWN_PAD_MASK;
                    }
                    else{
                        newState = currState | DOWN_PAD_MASK;
                    }
                    break;
                case SDLK_l:   
                    if(keyEvent.type == SDL_KEYDOWN){
                        newState = currState & ~A_BUTTON_MASK;
                    }
                    else{
                        newState = currState | A_BUTTON_MASK;
                    }
                    break;
                case SDLK_k:   
                    if(keyEvent.type == SDL_KEYDOWN){
                        newState = currState & ~B_BUTTON_MASK;
                    }
                    else{
                        newState = currState | B_BUTTON_MASK;
                    }
                    break;
                case SDLK_i:   
                    if(keyEvent.type == SDL_KEYDOWN){
                        newState = currState & ~SELECT_BUTTON_MASK;
                    }
                    else{
                        newState = currState | SELECT_BUTTON_MASK;
                    }
                    break;
                case SDLK_o:
                    if(keyEvent.type == SDL_KEYDOWN){
                        newState = currState & ~START_BUTTON_MASK;
                    }
                    else{
                        newState = currState | START_BUTTON_MASK;
                    }
                    break;
                case SDLK_ESCAPE:
                    if(keyEvent.type == SDL_KEYDOWN){
                        newState = currState & ~ALL_BUTTON_MASK;
                    }
                    else{
                        newState = currState |= ALL_BUTTON_MASK;
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

string omitFileExt(const std::string& filepath){
    size_t periodPos = filepath.find_last_of('.');
    if(periodPos == string::npos){
        return "";
    }
    return filepath.substr(0, periodPos);
}
