/*

Date: 06/06/2026
Program: Red Square
Author: Cavan Ray Theiss

Description: in order to learn how to better use the SDL2 library I'm 
making a bunch of basic games using it. This is a basic side scroller
where you control a red square. you can jump, sprint, run arounds, and 
the camera will follow you wherever you go.

*/

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

    // GET WINDOW SIZE ---------------------

    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    // INITIALIZE BASEPATH -----------------

    std::string basePath = SDL_GetBasePath();
    
    // INITIALIZE TEXT----------------------

    if(TTF_Init() < 0){
        std::cerr << "SDL_ttf initialization failed: " << TTF_GetError() << std::endl;
        return 1;
    }

    TTF_Font* scoreFont = TTF_OpenFont((basePath + "assets/arial.ttf").c_str(), 40);

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

    jumpSound = Mix_LoadWAV((basePath + "assets/jump.wav").c_str());
    if(jumpSound == NULL){
        std::cerr << "Failed to load jump sound! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return 1;
    }

    bgMusic = Mix_LoadMUS((basePath + "assets/backgroundMusic.wav").c_str());
    if(bgMusic == NULL){
        std::cerr << "Failed to load background Music! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return 1;
    }
    
    // **************************************** GAME VARIABLES ***********************************

    bool running = true;
    SDL_Event event;

    // FOR MAINTAINING GAME SPEED - DELTA TIME
    Uint64 lastTime = SDL_GetPerformanceCounter();
    Uint64 frequency = SDL_GetPerformanceFrequency();

    // FOR USING A CAMERA VIEW - SCROLLING VIEW
    float cameraX = 0.0f;
    float cameraY = 0.0f;

    // SET PLAYABLE AREA AVAILABLE
    const int worldWidth =  windowWidth;
    const int worldHeight = windowHeight;

    SDL_Rect redSquare = {windowWidth / 2,       windowHeight / 2 - 200, 40, 40}; // Initial position and size of the red square
    SDL_Rect ghostSquare = {windowWidth / 2,       windowHeight / 2 - 200, 40, 40}; // Ghost square for collision detection
    SDL_Rect bluePlatform = {windowWidth / 2 - 200, windowHeight - 240,     400, 40}; // Blue platform in center of the screen
    SDL_Rect greenPlatform = {(int)(windowWidth * 0.2f), windowHeight - 440, 400, 40}; // Green platform on the left side of the screen
    SDL_Rect yellowPlatform = {(int)(windowWidth * 0.43f), windowHeight - 590, 400, 40}; // Yellow platform on the right side of the screen

    float playerSpeed = 200.0f; // pixels per second
    float verticalVelocity = 0.0f; // Speed of the red square
    const float GRAVITY = 900.0f; // Gravity affecting the red square
    bool onGround = true; // To check if the red square is on the ground for jumping

    // **************************************** GAME LOOP ****************************************
    
    // START GAME MUSIC BEFORE LOOP BEGINS
    if (bgMusic){
        Mix_PlayMusic(bgMusic, -1);
    }
    
    while(running){

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }
        
        // Calculate delta time
        Uint64 currentTime = SDL_GetPerformanceCounter();
        float deltaTime = (float)(currentTime - lastTime) / frequency;
        lastTime = currentTime;

        // Clamp delta time
        if (deltaTime > 0.05f) deltaTime = 0.05f;         

    // **************************************** RESET SECTION ****************************************
        
        float horizontalVelocity = 0.0f; // Horizontal velocity reset each frame
        
        verticalVelocity += GRAVITY * deltaTime; // Apply gravity to vertical velocity
        
        cameraX = redSquare.x - windowWidth / 2;
        cameraY = redSquare.y - windowHeight / 2;
        
    // **************************************** KEYBOARD INPUT ***************************************

        // Handle keyboard input
        const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
        if (keyboardState[SDL_SCANCODE_LEFT]){
            horizontalVelocity = -300.0f * deltaTime;
        }
        if (keyboardState[SDL_SCANCODE_RIGHT]) {
            horizontalVelocity = 300.0f * deltaTime;
        }
        if (keyboardState[SDL_SCANCODE_SPACE] && onGround) {
            Mix_PlayChannel( -1, jumpSound, 0);
            verticalVelocity = -800.0f; 
            onGround = false;
        }
        if (keyboardState[SDL_SCANCODE_ESCAPE]) {
            running = false;
        }
        
        // **************************************** HORIZONTAL MOVEMENT ******************************
        
        ghostSquare.x += horizontalVelocity;
        ghostSquare.x = std::max(0, std::min(windowWidth - ghostSquare.w, ghostSquare.x)); // prevent moving out of bounds
        if (SDL_HasIntersection(&ghostSquare, &bluePlatform)) {
            ghostSquare.x -= horizontalVelocity; // undo movement if colliding with blue platform
        }
        if (SDL_HasIntersection(&ghostSquare, &greenPlatform)) {
            ghostSquare.x -= horizontalVelocity; // undo movement if colliding with green platform
        }
        if (SDL_HasIntersection(&ghostSquare, &yellowPlatform)) {
            ghostSquare.x -= horizontalVelocity; // undo movement if colliding with yellow platform
        }
        
        // **************************************** VERTICAL MOVEMENT ********************************
        
        ghostSquare.y += (int)(verticalVelocity * deltaTime);
        ghostSquare.y = std::min(windowHeight - ghostSquare.h, ghostSquare.y);

        bool hitWall = false; // Flag to check if we hit the wall
        if  (SDL_HasIntersection(&ghostSquare, &bluePlatform)){
            ghostSquare.y -= verticalVelocity * deltaTime; // undo movement if colliding with blue platform
            if (verticalVelocity > 0) { // If falling down, we were landing
                onGround = true;
                hitWall = true;
                verticalVelocity = 0; // reset vertical movement after landing
            } else if (verticalVelocity < 0){ // If moving up, we hit the ceiling
                hitWall = true;
                verticalVelocity = 0; // reset vertical movement after hitting ceiling
            }
        }
        if (SDL_HasIntersection(&ghostSquare, &greenPlatform)){
            ghostSquare.y -= verticalVelocity * deltaTime; // undo movement if colliding with green platform
            if (verticalVelocity > 0) { // If falling down, we were landing
                onGround = true;
                hitWall = true;
                verticalVelocity = 0; // reset vertical movement after landing
            } else if (verticalVelocity < 0){ // If moving up, we hit the ceiling
                hitWall = true;
                verticalVelocity = 0; // reset vertical movement after hitting ceiling
            }
        }
        if (SDL_HasIntersection(&ghostSquare, &yellowPlatform)){
            ghostSquare.y -= verticalVelocity * deltaTime; // undo movement if colliding with yellow platform
            if (verticalVelocity > 0) { // If falling down, we were landing
                onGround = true;
                hitWall = true;
                verticalVelocity = 0; // reset vertical movement after landing
            } else if (verticalVelocity < 0){ // If moving up, we hit the ceiling
                hitWall = true;
                verticalVelocity = 0; // reset vertical movement after hitting ceiling
            }
        }
        
        if (!hitWall && ghostSquare.y >= windowHeight - ghostSquare.h) {
            onGround = true; // We are on the ground if we hit the bottom of the window
            verticalVelocity = 0; // Stop vertical movement when hitting the ground
        }
        
        // **************************************** UPDATE RED SQUARE LOCATION ***********************
        
        redSquare.x = ghostSquare.x;
        redSquare.y = ghostSquare.y;
        
        // **************************************** RENDERING **************************************** 
  
        //Lambda to apply camera offset
        auto applyCamera = [&](SDL_Rect rect) -> SDL_Rect {
            rect.x -= (int)cameraX;
            rect.y -= (int)cameraY;
            return rect;
        };

        SDL_Rect drawRedSquare = applyCamera(redSquare);
        SDL_Rect drawBlue      = applyCamera(bluePlatform);
        SDL_Rect drawGreen     = applyCamera(greenPlatform);
        SDL_Rect drawYellow    = applyCamera(yellowPlatform);

        // Clear to dark grey
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        SDL_RenderClear(renderer);

        SDL_Rect playArea = {
            (int)-cameraX,
            (int)-cameraY,
            worldWidth,
            worldHeight,
        };

        // Draw background of playable area
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &playArea); 
        // Draw red square
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        // SDL_Rect redSquare = {100, 100, 200, 200};
        SDL_RenderFillRect(renderer, &drawRedSquare);
        // Draw blue platform
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(renderer, &drawBlue);
        // Draw green platform
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &drawGreen);
        // Draw yellow platform
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_RenderFillRect(renderer, &drawYellow);

        SDL_RenderPresent(renderer);

    }
    
    // **************************************** CLEANUP ******************************************

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
    
}
