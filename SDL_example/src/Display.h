//
// Created by kmathews on 15.08.21.
//
#include <SDL2/SDL.h>
#include <iostream>
#ifndef SDL_EXAMPLE_DISPLAY_H
#define SDL_EXAMPLE_DISPLAY_H


class Display {
    //Screen dimension constants
    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 480;

public:

    //The window we'll be rendering to
    SDL_Window* gWindow = NULL;

    //The surface contained by the window
    SDL_Surface* gScreenSurface = NULL;

    //The image we will load and show on the screen
    SDL_Surface* gHelloWorld = NULL;


    /**
     *
     * Methods
     *
     *
     * **/

    bool init();
    void close();
    bool loadMedia();
};


#endif //SDL_EXAMPLE_DISPLAY_H
