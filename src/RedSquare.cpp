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

    // INITIALIZE WINDOW---------------------

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Red Square", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1920, 1080, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

        SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    
    // INITIALIZE TEXT----------------------

    if(TTF_Init() < 0){
        std::cerr << "SDL_ttf initialization failed: " << TTF_GetError() << std::endl;
        return 1;
    }

    TTF_Font* scoreFont = TTF_OpenFont("assets/arial.ttf", 40);

    if(!scoreFont){
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return 1;
    }    

    SDL_Color textColor = { 255, 255, 255, 255};
    
    // INITIALIZE AUDIO--------------------

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

    bgMusic = Mix_LoadMUS("assets/backgroundMusic.wav");
    if(bgMusic == NULL){
        std::cerr << "Failed to load background Music! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return 1;
    }
    
    // **************************************** GAME VARIABLES ***********************************

    bool running = true;
    SDL_Event event;

    Uint64 lastTime = SDL_GetPerformanceCounter();
    Uint64 frequency = SDL_GetPerformanceFrequency();
    
    float playerSpeed = 200.0f; // pixels per second

    SDL_Rect redSquare = {960, 400, 40, 40}; // Initial position and size of the red square
    SDL_Rect ghostSquare = {960, 400, 40, 40}; // Ghost square for collision detection
    SDL_Rect bluePlatform = {960, 1200, 400, 40}; // Blue platform in center of the screen
    SDL_Rect greenPlatform = {500, 1000, 400, 40}; // Green platform on the left side of the screen
    SDL_Rect yellowPlatform = {1100, 850, 400, 40}; // Yellow platform on the right side of the screen

    float verticalVelocity = 0.0f; // Speed of the red square
    const float GRAVITY = 800.0f; // Gravity affecting the red square
    bool onGround = true; // To check if the red square is on the ground for jumping
    
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
