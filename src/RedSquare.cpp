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
