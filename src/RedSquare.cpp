#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <string>
#include <random>
#include <ctime>

int main() {

    // **************************************** WINDOW SETUP **************************************** 

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

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

    // **************************************** GAME VARIABLES **************************************** 

    bool running = true;
    SDL_Event event;

    Uint64 lastTime = SDL_GetPerformanceCounter();
    Uint64 frequency = SDL_GetPerformanceFrequency();

    float playerX = 400.0f;
    float playerSpeed = 200.0f; // pixels per second

    SDL_Rect redSquare = {960, 400, 40, 40}; // Initial position and size of the red square
    SDL_Rect ghostSquare = {960, 400, 40, 40}; // Ghost square for collision detection
    SDL_Rect bluePlatform = {960, 1200, 400, 40}; // Blue platform in center of the screen
    SDL_Rect greenPlatform = {500, 1000, 400, 40}; // Green platform on the left side of the screen
    SDL_Rect yellowPlatform = {1100, 850, 400, 40}; // Yellow platform on the right side of the screen

    float verticalVelocity = 0.0f; // Speed of the red square
    const float GRAVITY = 800.0f; // Gravity affecting the red square
    bool onGround = true; // To check if the red square is on the ground for jumping

    if (bgMusic){
        Mix_PlayMusic(bgMusic, -1);
    }

    // **************************************** GAME LOOP **************************************** 

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }

        // Calculate delta time
        Uint64 currentTime = SDL_GetPerformanceCounter();
        float deltaTime = (float)(currentTime - lastTime) / frequency;
        lastTime = currentTime;

        // Clamp delta time
        if (deltaTime > 0.05f) deltaTime = 0.05f;




        // getting window size to prevent the square from moving out of bounds
        int windowWidth, windowHeight;
        SDL_GetWindowSize(window, &windowWidth, &windowHeight);
        

        float horizontalVelocity = 0.0f; // Horizontal velocity reset each frame
        verticalVelocity += GRAVITY * deltaTime; // Apply gravity to vertical velocity

    // **************************************** KEYBOARD INPUT **************************************** 

        // Handle keyboard input
        const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
        if (keyboardState[SDL_SCANCODE_LEFT]) {
            horizontalVelocity = -300.0f * deltaTime; 
        }
        if (keyboardState[SDL_SCANCODE_RIGHT]) {
            horizontalVelocity = 300.0f * deltaTime;
        }
        if (keyboardState[SDL_SCANCODE_SPACE] && onGround) {
            Mix_PlayChannel( -1, jumpSound, 0);
            verticalVelocity = -600.0f * deltaTime; 
            onGround = false;
        }
        if (keyboardState[SDL_SCANCODE_ESCAPE]) {
            running = false;
        }

        // **************************************** HORIZONTAL MOVEMENT **************************************** 
        ghostSquare.x += horizontalVelocity;
        ghostSquare.x = std::max(0, std::min(windowWidth - ghostSquare.w, ghostSquare.x)); // prevent moving out of bounds
        if (SDL_HasIntersection(&ghostSquare, &bluePlatform)|SDL_HasIntersection(&ghostSquare, &greenPlatform)|SDL_HasIntersection(&ghostSquare, &yellowPlatform)) {
            ghostSquare.x -= horizontalVelocity; // undo movement if colliding with blue platform
        }

        // **************************************** VERTICAL MOVEMENT **************************************** 
        ghostSquare.y += (int)(verticalVelocity * deltaTime);
        ghostSquare.y = std::min(windowHeight - ghostSquare.h, ghostSquare.y);

        bool hitWall = false; // Flag to check if we hit the wall
        if  (SDL_HasIntersection(&ghostSquare, &bluePlatform)|SDL_HasIntersection(&ghostSquare, &greenPlatform)|SDL_HasIntersection(&ghostSquare, &yellowPlatform)) {
            ghostSquare.y -= verticalVelocity; // undo movement if colliding with blue platform
            if (verticalVelocity > 0) { // If falling down, we are on the ground
                onGround = true;
                verticalVelocity = 0; // Stop vertical movement when hitting the ground
                hitWall = true;
            } else if (verticalVelocity < 0) { // If moving up, we hit the ceiling
                verticalVelocity = 0; // Stop vertical movement when hitting the ceiling
            }   hitWall = true;
        }

        if (!hitWall && ghostSquare.y >= windowHeight - ghostSquare.h) {
            onGround = true; // We are on the ground if we hit the bottom of the window
            verticalVelocity = 0; // Stop vertical movement when hitting the ground
        }

        // **************************************** UPDATE RED SQUARE LOCATION **************************************** 
        redSquare.x = ghostSquare.x;
        redSquare.y = ghostSquare.y;

        // **************************************** RENDERING **************************************** 

        // Clear to black
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        // Draw red square
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        // SDL_Rect redSquare = {100, 100, 200, 200};
        SDL_RenderFillRect(renderer, &redSquare);
        // Draw blue platform
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(renderer, &bluePlatform);
        // Draw green platform
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &greenPlatform);
        // Draw yellow platform
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_RenderFillRect(renderer, &yellowPlatform);

        SDL_RenderPresent(renderer);

    }

    // **************************************** CLEANUP **************************************** 

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
