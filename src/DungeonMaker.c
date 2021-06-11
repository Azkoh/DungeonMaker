#include "DungeonMaker.h"

// Destroy all the objects in case of error or exit
void destroy(){
    // Destroy Window
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    //Destroy renderer
    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    // Quit
    SDL_Quit();
}

// Save and Load code
// Visual Studio requires fopen_s by default so that will have to be used
#ifndef __WIN32__
void save(){
    FILE *ptr = fopen("save.hex", "w");
    for (int y = 0; y<11; y++){
        for (int x = 0; x<11; x++){
            // Map data is stored as a 2d array y is the row and x is the colum
            fputc(positions[y][x], ptr);
        }
    }
    fclose(ptr);
}

void load(){
    FILE *ptr = fopen("save.hex", "r");
    for (int y = 0; y<11; y++){
        for (int x = 0; x<11; x++){
            positions[y][x] = fgetc(ptr);
        }
    }
    fclose(ptr);

}
#else
// See Linux code above for documentation
void save() {
    FILE** ptr = NULL;
    fopen_s(ptr, "save.hex", "w");
    for (int y = 0; y<11; y++){
        for (int x = 0; x<11; x++){
            fputc(positions[y][x], *ptr);
        }
    }
    fclose(*ptr);
}

void load() {
    FILE** ptr = NULL;
    fopen_s(ptr, "save.hex", "r");
    for (int y = 0; y<11; y++){
        for (int x = 0; x<11; x++){
            positions[y][x] = fgetc(*ptr);
        }
    }
    fclose(*ptr);

}
#endif

void init(){
    // Initalize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("SDL Init failed, Error: %s\n", SDL_GetError());
        destroy();
        exit(1);
    }
    // Create the window
    gWindow =  SDL_CreateWindow("Dungeon Maker", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    // Create the renderer from the window
    renderer = SDL_CreateRenderer(gWindow, -1, 0);

    // Check if window initalized correctly
    if (gWindow == NULL){
        printf("Window failed to open, Error: %s\n", SDL_GetError());
        destroy();
        exit(1);
    }
    // Check if the renderer initalized correctly
    if (renderer == NULL){
        printf("Renderer failed to initalize, Error: %s\n", SDL_GetError());
        destroy();
        exit(1);
    }

    // Set the current block to empty
    currentBlock = Empty;
    // Don't quit
    quit = 0;

    // Define all the sizes for all the clickable objects
    // TO-DO: Move all this to an object to be declared earlier
    D1ObjSize.x = 510;
    D1ObjSize.y = 40;
    D1ObjSize.w = 60;
    D1ObjSize.h = 60;

    D2ObjSize.x = 510;
    D2ObjSize.y = 120;
    D2ObjSize.w = 60;
    D2ObjSize.h = 60;


    DTObjSize.x = 510;
    DTObjSize.y = 200;
    DTObjSize.w = 60;
    DTObjSize.h = 60;

    D2TObjSize.x = 510;
    D2TObjSize.y = 280;
    D2TObjSize.w = 60;
    D2TObjSize.h = 60;

    TurnButtonObjSize.x = 550;
    TurnButtonObjSize.y = 350;
    TurnButtonObjSize.w = 40;
    TurnButtonObjSize.h = 40;

    XObjSize.x = 490;
    XObjSize.y = 350;
    XObjSize.w = 40;
    XObjSize.h = 40;

    SavObjSize.x = 460;
    SavObjSize.y = 420;
    SavObjSize.w = 60;
    SavObjSize.h = 40;

    LoadObjSize.x = 560;
    LoadObjSize.y = 420;
    LoadObjSize.w = 60;
    LoadObjSize.h = 40;
}

void loadImage(){
    // Create surface from image
    // Create a texture from that surface
    D1Sur = SDL_LoadBMP("images/1D.bmp");
    D1Tex = SDL_CreateTextureFromSurface(renderer, D1Sur);

    D2Sur = SDL_LoadBMP("images/2D.bmp");
    D2Tex = SDL_CreateTextureFromSurface(renderer, D2Sur);

    DTSur = SDL_LoadBMP("images/TD.bmp");
    DTTex = SDL_CreateTextureFromSurface(renderer, DTSur);

    D2TSur = SDL_LoadBMP("images/2DTurn.bmp");
    D2TTex = SDL_CreateTextureFromSurface(renderer, D2TSur);

    TurnSur = SDL_LoadBMP("images/right.bmp");
    TurnTex = SDL_CreateTextureFromSurface(renderer, TurnSur);

    XSur = SDL_LoadBMP("images/X.bmp");
    XTex = SDL_CreateTextureFromSurface(renderer, XSur);

    SavSur = SDL_LoadBMP("images/save.bmp");
    SavTex = SDL_CreateTextureFromSurface(renderer, SavSur);

    LoadSur = SDL_LoadBMP("images/load.bmp");
    LoadTex = SDL_CreateTextureFromSurface(renderer, LoadSur);

    // Free all the surfaces because they're useless now
    free(D1Sur);
    free(D2Sur);
    free(DTSur);
    free(D2TSur);
    free(TurnSur);
    free(XSur);
    free(SavSur);
    free(LoadSur);
}

void renderGrid(){
    // Set color to black
    SDL_SetRenderDrawColor(renderer, 0,0,0,255);
    // Y = rows
    for (int y = 0; y<12; y++){
        // Draw horizontal lines
        SDL_RenderDrawLine(renderer, 0, y*40, 440, y*40);
        // Draw vertical lines
        SDL_RenderDrawLine(renderer, y*40,0,y*40, 440);
        // Begin to render rooms
        for (int x = 0; x<11; x++){
            // Make sure the user didn't just now click on the room
            if (lastPos != &positions[y][x]){
                constantOfRatio = 40;
                center = 0;
            }
            // Shrink the currently selected room, to show that it is selected
            else{
                constantOfRatio = 30;
                center = 5;
            }
            // Set the dimensions of the currently rendered room
            rct.x = x*40+center;
            rct.y = y*40+center;
            rct.w = constantOfRatio;
            rct.h = constantOfRatio;

            // Select which room to use based on which room it is rendering
            switch(positions[y][x]){
                case EndRoom0:
                    SDL_RenderCopy(renderer, D1Tex, NULL,&rct);
                    //SDL_RenderCopyEx(renderer, D1Tex, NULL, &rct, 180.0, NULL, SDL_FLIP_NONE);

                    break;
                case EndRoom90:
                    SDL_RenderCopyEx(renderer, D1Tex, NULL, &rct, 90.0, NULL, SDL_FLIP_NONE);
                    break;
                case EndRoom180:
                    SDL_RenderCopyEx(renderer, D1Tex, NULL, &rct, 180.0, NULL, SDL_FLIP_NONE);
                    break;
                case EndRoom270:
                    SDL_RenderCopyEx(renderer, D1Tex, NULL, &rct, 270.0, NULL, SDL_FLIP_NONE);
                    break;
                case Hallway0:
                    SDL_RenderCopy(renderer, D2Tex, NULL,&rct);
                    //SDL_RenderCopyEx(renderer, D2Tex, NULL, &rct, 180.0, NULL, SDL_FLIP_NONE);
                    break;
                case Hallway90:
                    //SDL_RenderCopy(renderer, D2Tex, NULL,&rct);
                    SDL_RenderCopyEx(renderer, D2Tex, NULL, &rct, 90.0, NULL, SDL_FLIP_NONE);
                    break;
                case TRoom0:
                    SDL_RenderCopy(renderer, DTTex, NULL,&rct);
                    break;
                case TRoom90:
                    SDL_RenderCopyEx(renderer, DTTex, NULL, &rct, 90.0, NULL, SDL_FLIP_NONE);
                    break;
                case TRoom180:
                    SDL_RenderCopyEx(renderer, DTTex, NULL, &rct, 180.0, NULL, SDL_FLIP_NONE);
                    break;
                case TRoom270:
                    SDL_RenderCopyEx(renderer, DTTex, NULL, &rct, 270, NULL, SDL_FLIP_NONE);
                    break;

                case TurnRoom0:
                    SDL_RenderCopy(renderer, D2TTex, NULL,&rct);
                    break;
                case TurnRoom90:
                    SDL_RenderCopyEx(renderer, D2TTex, NULL, &rct, 90.0, NULL, SDL_FLIP_NONE);
                    break;
                case TurnRoom180:
                    SDL_RenderCopyEx(renderer, D2TTex, NULL, &rct, 180.0, NULL, SDL_FLIP_NONE);
                    break;
                case TurnRoom270:
                    SDL_RenderCopyEx(renderer, D2TTex, NULL, &rct, 270.0, NULL, SDL_FLIP_NONE);
                    break;
            }
        }
    }
}

// Check collisions between rectangles
int checkCollision(int mx, int my, SDL_Rect rct){
    if ((mx >= rct.x && mx <= rct.x+rct.w)){
        if ((my >= rct.y && my <= rct.y+rct.h)){
            return 1;
        }
    }
    else{
        return 0;
    }
    return -1;
}

// Rotate currently selected objects, the BEST way
void rotateSelected(){
        switch (*lastPos){
            case EndRoom0:
                *lastPos = (unsigned char) EndRoom90;
                break;
            case EndRoom90:
                *lastPos = (unsigned char) EndRoom180;
                break;
            case EndRoom180:
                *lastPos = (unsigned char) EndRoom270;
                break;
            case EndRoom270:
                *lastPos = (unsigned char) EndRoom0;
                break;

            case Hallway0:
                *lastPos = (unsigned char) Hallway90;
                break;
            case Hallway90:
                *lastPos = (unsigned char) Hallway0;
                break;

            case TRoom0:
                *lastPos = (unsigned char) TRoom90;
                break;
            case TRoom90:
                *lastPos = (unsigned char) TRoom180;
                break;
            case TRoom180:
                *lastPos = (unsigned char) TRoom270;
                break;
            case TRoom270:
                *lastPos = (unsigned char) TRoom0;
                break;

            case TurnRoom0:
                *lastPos = (unsigned char) TurnRoom90;
                break;
            case TurnRoom90:
                *lastPos = (unsigned char) TurnRoom180;
                break;
            case TurnRoom180:
                *lastPos = (unsigned char) TurnRoom270;
                break;
            case TurnRoom270:
                *lastPos = (unsigned char) TurnRoom0;
                break;
        }

}

// Main loop
void mainLoop(){
    while (!quit){
        // Set default size
        rct.w = 40;
        rct.h = 40;
        // Switch the current event
        while (SDL_PollEvent(&e) != 0){
            switch (e.type){
                // Quit on, well, quit
                case SDL_QUIT:
                    quit = 1;
                    break;
                // Detect key presses
                case SDL_KEYDOWN:
                    switch (e.key.keysym.sym){
                        // On s, save
                        case SDLK_s:
                            save();
                            break;
                        // On l, load
                        case SDLK_l:
                            load();
                            break;
                        // On z, remove selected roo,
                        case SDLK_z:
                            *lastPos = 0;
                            break;
                        // Arrow key right, rotate
                        case SDLK_RIGHT:
                            rotateSelected();
                            break;
                    }
                    break;
                // Detect mouse down
                case SDL_MOUSEBUTTONDOWN:
                    // Get mouse position and assign it to MouseX and MouseY
                    SDL_GetMouseState(&mouseX, &mouseY);

                    // Check if mouse is in grid
                    if (mouseX <= 440 && mouseY <=440){
                        // Check if it was a left click
                        if (e.button.button == SDL_BUTTON_LEFT){
                            // Check if the selected block is empty or selected
                            if (positions[mouseY/40][mouseX/40] == Empty || lastPos == &positions[mouseY/40][mouseX/40]){
                                // Replaced the selected block with the current block
                                positions[mouseY/40][mouseX/40] = currentBlock;
                            }
                        }
                        // Check if it was a right click
                        else if(e.button.button == SDL_BUTTON_RIGHT){
                            // Delete the current selected block
                            positions[mouseY/40][mouseX/40] = Empty;
                        }
                        // Assign lastPos to the address of the selected block
                        lastPos = &positions[mouseY/40][mouseX/40];
                    }
                    else{
                        // Set sizes for all the objects
                        D1ObjSize.w = 60;
                        D1ObjSize.h = 60;
                        D2ObjSize.w = 60;
                        D2ObjSize.h = 60;
                        DTObjSize.w = 60;
                        DTObjSize.h = 60;
                        D2TObjSize.w = 60;
                        D2TObjSize.h = 60;
                        // Check collision between the mouse click position and the clickable object
                        // Spaget code edition
                        if (checkCollision(mouseX,mouseY, D1ObjSize) == 1){
                            currentBlock = EndRoom0;
                            D1ObjSize.w = 50;
                            D1ObjSize.h = 50;
                        }
                        else if (checkCollision(mouseX,mouseY, D2ObjSize) == 1){
                            currentBlock = Hallway0;
                            D2ObjSize.w = 50;
                            D2ObjSize.h = 50;
                        }
                        else if (checkCollision(mouseX,mouseY, DTObjSize) == 1){
                            currentBlock = TRoom0;
                            DTObjSize.w = 50;
                            DTObjSize.h = 50;
                        }
                        else if (checkCollision(mouseX,mouseY, D2TObjSize) == 1){
                            currentBlock = TurnRoom0;
                            D2TObjSize.w = 50;
                            D2TObjSize.h = 50;
                        }
                        else if (checkCollision(mouseX,mouseY, TurnButtonObjSize) == 1){
                            TurnButtonObjSize.w = 30;
                            TurnButtonObjSize.h = 30;
                            rotateSelected();
                        }

                        else if (checkCollision(mouseX,mouseY, XObjSize) == 1){
                            XObjSize.w = 30;
                            XObjSize.h = 30;
                            *lastPos = 0;
                        }

                        else if (checkCollision(mouseX,mouseY, SavObjSize) == 1){
                            SavObjSize.w = 40;
                            SavObjSize.h = 30;
                            save();
                        }
                        else if (checkCollision(mouseX,mouseY, LoadObjSize) == 1){
                            LoadObjSize.w = 40;
                            LoadObjSize.h = 30;
                            load();
                        }
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    // On Mouse up, set the scale back to normal
                    if (TurnButtonObjSize.w == 30){
                        TurnButtonObjSize.w = 40;
                        TurnButtonObjSize.h = 40;
                    }
                    else if (XObjSize.w == 30){
                        XObjSize.w = 40;
                        XObjSize.h = 40;
                    }
                    else if (SavObjSize.w == 40){
                        SavObjSize.w = 60;
                        SavObjSize.h = 40;
                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Save Successful", "Map Saved Successfully", gWindow);
                    }
                    else if (LoadObjSize.w == 40){
                        LoadObjSize.w = 60;
                        LoadObjSize.h = 40;
                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Load Successful", "Map Load Successfully", gWindow);

                    }
                    break;
                default:
                    break;
            }
        }
        // Draws background color
        SDL_SetRenderDrawColor(renderer, 200, 200, 242, 255);
        // Clear the renderer
        SDL_RenderClear(renderer);

        // Render the grid
        renderGrid();

        // Set position of base room
        rct.x = 200;
        rct.y = 440;
        // Render base room
        SDL_RenderCopy(renderer, D1Tex, NULL,&rct);

        // Render all the clickable objects
        SDL_RenderCopy(renderer, D1Tex, NULL,&D1ObjSize);

        SDL_RenderCopy(renderer, D2Tex, NULL,&D2ObjSize);

        SDL_RenderCopy(renderer, DTTex, NULL,&DTObjSize);

        SDL_RenderCopy(renderer, D2TTex, NULL,&D2TObjSize);

        SDL_RenderCopy(renderer, TurnTex, NULL,&TurnButtonObjSize);

        SDL_RenderCopy(renderer, XTex, NULL,&XObjSize);

        SDL_RenderCopy(renderer, SavTex, NULL,&SavObjSize);

        SDL_RenderCopy(renderer, LoadTex, NULL,&LoadObjSize);

        // Update the stffs
        SDL_RenderPresent(renderer);

        SDL_UpdateWindowSurface(gWindow);

    }
}

int main(int argc, char* args[]){
    printf("Starting...");
    // initalize
    init();
    // Load the images
    loadImage();
    // Start the main loop
    mainLoop();
    // End and destroy
    destroy();

    return 0;
}
