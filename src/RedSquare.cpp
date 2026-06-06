// **************************************** HEADERS ****************************************** 
#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <string>
#include <random>
#include <ctime>
// All headers to be combined into single include file

int main(){
    // **************************************** WINDOW SETUP ************************************* 

    // INITIALIZE WINDOW

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    
    // INITIALIZE TEXT

    // INITIALIZE AUDIO

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
        std::cerr << "SDL_mixer couldnot initialize! Error: " << Mix_GetError() << std::endl;
        return 1;
    }

    Mix_Chunk* jumpSound = NULL;
    Mix_Music* bgMusic = NULL;

    jumpSound = Mix_LoadWAV("assets/jump.wav");
    if(jumpSound == NULL){
        std::cerr << "Failed to load jump sound! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return 1;
    }
    
    // **************************************** GAME VARIABLES ***********************************
    // **************************************** GAME LOOP ****************************************
  while(){
    // **************************************** KEYBOARD INPUT ***************************************
        // **************************************** HORIZONTAL MOVEMENT ******************************
        // **************************************** VERTICAL MOVEMENT ********************************
        // **************************************** UPDATE RED SQUARE LOCATION ***********************
        // **************************************** RENDERING **************************************** 
  }
    // **************************************** CLEANUP ******************************************
}
