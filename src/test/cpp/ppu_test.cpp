#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <iostream>

using namespace std;

constexpr int WIDTH =  160, HEIGHT = 144;

int main(int argc, char** argv){
    cout << "hello world" << endl;
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow("Hello World", 
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH,
        HEIGHT,
        SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Event windowEvent;
    SDL_Surface* windowSurface = SDL_GetWindowSurface(window);
    SDL_Surface* imageSurface = SDL_LoadBMP("ladybug.bmp");
    if(imageSurface == nullptr){
        cout << "Image loading error: " << SDL_GetError() << endl;
    }
    SDL_BlitSurface(imageSurface, NULL, windowSurface, NULL);
    SDL_UpdateWindowSurface(window);
    while(true){
        if(SDL_PollEvent(&windowEvent)){
            if(SDL_QUIT == windowEvent.type){
                break;
            }
        }
    }
    SDL_DestroyWindow(window);
    return 0;
}
