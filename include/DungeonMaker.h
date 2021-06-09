#ifndef DUNGEONMAKER_H
#define DUNGEONMAKER_H

#include <stdio.h>
#include <SDL.h>
#include <time.h>

#define SCREEN_WIDTH (int) 640
#define SCREEN_HEIGHT (int) 480


SDL_Window* gWindow = NULL;

SDL_Surface* gScreenSurface = NULL;

SDL_Surface* gHelloWorld = NULL;

SDL_Renderer * renderer = NULL;

SDL_Surface * D1Sur;
SDL_Texture * D1Tex;

SDL_Surface * D2Sur;
SDL_Texture * D2Tex;

SDL_Surface * DTSur;
SDL_Texture * DTTex;

SDL_Surface * D2TSur;
SDL_Texture * D2TTex;

SDL_Event e;

SDL_Rect rct;

SDL_Rect D1ObjSize;

SDL_Rect D2ObjSize;

SDL_Rect DTObjSize;

SDL_Rect D2TObjSize;


unsigned char positions[20][20];

unsigned char *lastPos;

bool quit;

int mouseX;
int mouseY;

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

types currentBlock;


void destroy();
void save();
void load();
void init();
void loadImage();
void renderGrid();
int checkCollision(int mx, int my, SDL_Rect rct);
void rotateSelected(int toLeft);

#endif // DUNGEONMAKER_H
