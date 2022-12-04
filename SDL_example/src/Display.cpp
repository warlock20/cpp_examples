//
// Created by kmathews on 15.08.21.
//

#include "Display.h"

bool Display::init() {
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        std::cout<< "SDL could not initialize! SDL_Error: "<< SDL_GetError()<<"\n";
        success = false;
    }
    else
    {
        //Create window
        gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            std::cout<< "Window could not be created! SDL_Error: "<< SDL_GetError() << "\n";
            success = false;
        }
        else
        {
            //Get window surface
            gScreenSurface = SDL_GetWindowSurface( gWindow );
        }
    }
    return success;
}

void Display::close()
{
    //Deallocate surface
    SDL_FreeSurface( gHelloWorld );
    gHelloWorld = NULL;

    //Destroy window
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}

bool Display::loadMedia()
{
    //Loading success flag
    bool success = true;

    //Load splash image
    gHelloWorld = SDL_LoadBMP( "../src/extras/hello_world.bmp" );

    if( gHelloWorld == NULL )
    {
        std::cout<<"Unable to load image %s! SDL Error: " << SDL_GetError()<<"\n";
        success = false;
    }
    return success;
}