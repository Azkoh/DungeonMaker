#include "DungeonMaker.h"

void destroy(){

    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    SDL_Quit();
}

void save(){
    FILE *ptr = fopen("save.hex", "w");
    for (int x = 0; x<11; x++){
        for (int y = 0; y<11; y++){
            fputc(positions[x][y], ptr);
        }
    }
    fclose(ptr);
}

void load(){
    FILE *ptr = fopen("save.hex", "r");
    for (int x = 0; x<11; x++){
        for (int y = 0; y<11; y++){
            positions[x][y] = fgetc(ptr);
        }
    }
    fclose(ptr);

}

void init(){
    if (SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("SDL Init failed, Error: %s\n", SDL_GetError());
        destroy();
        exit(1);
    }

    gWindow =  SDL_CreateWindow("Eternal Horizons", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(gWindow, -1, 0);

    if (gWindow == NULL){
        printf("Window failed to open, Error: %s\n", SDL_GetError());
        destroy();
        exit(1);
    }

    // Initalize some random values
    currentBlock = Empty;
    quit = false;

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
    SDL_SetRenderDrawColor(renderer, 0,0,0,255);
    for (int y = 0; y<12; y++){
        SDL_RenderDrawLine(renderer, 0, y*40, 440, y*40);
        SDL_RenderDrawLine(renderer, y*40,0,y*40, 440);
        for (int x = 0; x<11; x++){
            if (lastPos != &positions[y][x]){
                constantOfRatio = 40;
                center = 0;
            }
            else{
                constantOfRatio = 30;
                center = 5;
            }
            rct.x = x*40+center;
            rct.y = y*40+center;
            rct.w = constantOfRatio;
            rct.h = constantOfRatio;
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

int checkCollision(int mx, int my, SDL_Rect rct){
    if ((mx >= rct.x && mx <= rct.x+rct.w)){
        if ((my >= rct.y && my <= rct.y+rct.h)){
            return 1;
        }
    }
    else{
        return 0;
    }
}

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

int main(int argc, char* args[]){
    printf("Starting...");

    constantOfRatio = 40;
    center = 0;

    init();

    loadImage();

    while (!quit){
        rct.w = 40;
        rct.h = 40;
        while (SDL_PollEvent(&e) != 0){
            switch (e.type){
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                    switch (e.key.keysym.sym){
                        case SDLK_s:
                            save();
                            break;
                        case SDLK_l:
                            load();
                            break;
                        case SDLK_z:
                            *lastPos = 0;
                            break;
                        case SDLK_RIGHT:
                            rotateSelected();
                            break;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    SDL_GetMouseState(&mouseX, &mouseY);

                    if (mouseX <= 440 && mouseY <=440){
                        if (e.button.button == SDL_BUTTON_LEFT){
                            if (positions[mouseY/40][mouseX/40] == Empty || lastPos == &positions[mouseY/40][mouseX/40]){
                                positions[mouseY/40][mouseX/40] = currentBlock;
                            }
                        }
                        else if(e.button.button == SDL_BUTTON_RIGHT){
                            positions[mouseY/40][mouseX/40] = Empty;
                        }
                        lastPos = &positions[mouseY/40][mouseX/40];
                    }
                    else{
                        D1ObjSize.w = 60;
                        D1ObjSize.h = 60;
                        D2ObjSize.w = 60;
                        D2ObjSize.h = 60;
                        DTObjSize.w = 60;
                        DTObjSize.h = 60;
                        D2TObjSize.w = 60;
                        D2TObjSize.h = 60;
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
                    }
                    else if (LoadObjSize.w == 40){
                        LoadObjSize.w = 60;
                        LoadObjSize.h = 40;
                    }
                    break;
                default:
                    break;
            }
        }
        Uint32 frameStart;
        Uint32 frameTime;
        frameStart = SDL_GetTicks();
        SDL_SetRenderDrawColor(renderer, 242, 242, 242, 255);
        SDL_RenderClear(renderer);

        renderGrid();

        rct.x = 200;
        rct.y = 440;
        SDL_RenderCopy(renderer, D1Tex, NULL,&rct);


        SDL_RenderCopy(renderer, D1Tex, NULL,&D1ObjSize);

        SDL_RenderCopy(renderer, D2Tex, NULL,&D2ObjSize);

        SDL_RenderCopy(renderer, DTTex, NULL,&DTObjSize);

        SDL_RenderCopy(renderer, D2TTex, NULL,&D2TObjSize);

        SDL_RenderCopy(renderer, TurnTex, NULL,&TurnButtonObjSize);

        SDL_RenderCopy(renderer, XTex, NULL,&XObjSize);

        SDL_RenderCopy(renderer, SavTex, NULL,&SavObjSize);

        SDL_RenderCopy(renderer, LoadTex, NULL,&LoadObjSize);


        SDL_RenderPresent(renderer);

        frameTime = SDL_GetTicks() - frameStart;



        SDL_UpdateWindowSurface(gWindow);

    }

    destroy();

    return 0;
}
