#ifndef DUNGEONMAKER_H
#define DUNGEONMAKER_H


// Include needed libraries
#include <stdio.h>

#ifdef __WIN32__
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

#include <time.h>

// Define screen size
#define SCREEN_WIDTH (int) 640
#define SCREEN_HEIGHT (int) 480

// Define the window and renderer we will be using
SDL_Window* gWindow = NULL;
SDL_Renderer * renderer = NULL;

// These are all the textures for all of the images in this application
// The surface is for loading the image
// The Texture is loaded from the surface
SDL_Surface * D1Sur;
SDL_Texture * D1Tex;

SDL_Surface * D2Sur;
SDL_Texture * D2Tex;

SDL_Surface * DTSur;
SDL_Texture * DTTex;

SDL_Surface * D2TSur;
SDL_Texture * D2TTex;

SDL_Surface * TurnSur;
SDL_Texture * TurnTex;

SDL_Surface * XSur;
SDL_Texture * XTex;

SDL_Surface * SavSur;
SDL_Texture * SavTex;

SDL_Surface * LoadSur;
SDL_Texture * LoadTex;

// Define our event object
SDL_Event e;

// just a general rectangle object, to be removed in the future
SDL_Rect rct;

// Define the collision boxes of each button
SDL_Rect D1ObjSize;

SDL_Rect D2ObjSize;

SDL_Rect DTObjSize;

SDL_Rect D2TObjSize;

SDL_Rect TurnButtonObjSize;

SDL_Rect XObjSize;

SDL_Rect SavObjSize;

SDL_Rect LoadObjSize;

// All grid information is stored here
unsigned char positions[20][20];

// This pointer will point to the last editied value of positions
unsigned char *lastPos;

// A simple boolean to determine if it is time to stop the application
bool quit;

// Used to store the mouse position during mouse events
int mouseX;
int mouseY;

// These are used to center our rooms when they're in the grid
// Constant of ratio is their new w+h
int constantOfRatio;
// The center is added to their respective x+y values to center the objects in the square
int center;

// all the different room types
enum types{
    Empty,
    EndRoom0,
    EndRoom90,
    EndRoom180,
    EndRoom270,

    Hallway0,
    Hallway90,

    TRoom0,
    TRoom90,
    TRoom180,
    TRoom270,

    TurnRoom0,
    TurnRoom90,
    TurnRoom180,
    TurnRoom270
};

// To store the currently selected block
types currentBlock;


// Boring Method stuff
void destroy();
void save();
void load();
void init();
void loadImage();
void renderGrid();
int checkCollision(int mx, int my, SDL_Rect rct);
void rotateSelected();
void mainLoop();

#endif // DUNGEONMAKER_H
