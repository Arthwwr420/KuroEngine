#include<SDL3/SDL.h>
#include<iostream>
#include "KuroRender.h"
#include<time.h>
#include<thread>

int HEIGHT=720,WIDTH=1280;
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

Mesh CUBOIDE(){
    Mesh kaka;
    
    kaka.t = {
        {0.0f,0.0f,0.0f,1.0f, 0.0f,1.0f,0.0f,1.0f, 1.0f,1.0f,0.0f,1.0f},
        {0.0f,0.0f,0.0f,1.0f, 1.0f,1.0f,0.0f,1.0f, 1.0f,0.0f,0.0f,1.0f},

        {1.0f,0.0f,0.0f,1.0f, 1.0f,1.0f,0.0f,1.0f, 1.0f,1.0f,1.0f,1.0f},
        {1.0f,0.0f,0.0f,1.0f, 1.0f,1.0f,1.0f,1.0f, 1.0f,0.0f,1.0f,1.0f},

        {1.0f,0.0f,1.0f,1.0f, 1.0f,1.0f,1.0f,1.0f, 0.0f,1.0f,1.0f,1.0f},
        {1.0f,0.0f,1.0f,1.0f, 0.0f,1.0f,1.0f,1.0f, 0.0f,0.0f,1.0f,1.0f},

        {0.0f,0.0f,1.0f,1.0f, 0.0f,1.0f,1.0f,1.0f, 0.0f,1.0f,0.0f,1.0f},
        {0.0f,0.0f,1.0f,1.0f, 0.0f,1.0f,0.0f,1.0f, 0.0f,0.0f,0.0f,1.0f},

        {0.0f,1.0f,0.0f,1.0f, 0.0f,1.0f,1.0f,1.0f, 1.0f,1.0f,1.0f,1.0f},
        {0.0f,1.0f,0.0f,1.0f, 1.0f,1.0f,1.0f,1.0f, 1.0f,1.0f,0.0f,1.0f},

        {1.0f,0.0f,1.0f,1.0f, 0.0f,0.0f,1.0f,1.0f, 0.0f,0.0f,0.0f,1.0f},
        {1.0f,0.0f,1.0f,1.0f, 0.0f,0.0f,0.0f,1.0f, 1.0f,0.0f,0.0f,1.0f},
    };
    
    /*kaka.t = {
        {-1.0f,-1.0f,-1.0f,1.0f, -1.0f,1.0f,-1.0f,1.0f, 1.0f,1.0f,-1.0f,1.0f},
        {-1.0f,-1.0f,-1.0f,1.0f, 1.0f,1.0f,-1.0f,1.0f, 1.0f,-1.0f,-1.0f,1.0f},

        {1.0f,-1.0f,-1.0f,1.0f, 1.0f,1.0f,-1.0f,1.0f, 1.0f,1.0f,1.0f,1.0f},
        {1.0f,-1.0f,-1.0f,1.0f, 1.0f,1.0f,1.0f,1.0f, 1.0f,-1.0f,1.0f,1.0f},

        {1.0f,-1.0f,1.0f,1.0f, 1.0f,1.0f,1.0f,1.0f, -1.0f,1.0f,1.0f,1.0f},
        {1.0f,-1.0f,1.0f,1.0f, -1.0f,1.0f,1.0f,1.0f, -1.0f,-1.0f,1.0f,1.0f},

        {-1.0f,-1.0f,1.0f,1.0f, -1.0f,1.0f,1.0f,1.0f, -1.0f,1.0f,-1.0f,1.0f},
        {-1.0f,-1.0f,1.0f,1.0f, -1.0f,1.0f,-1.0f,1.0f, -1.0f,-1.0f,-1.0f,1.0f},

        {-1.0f,1.0f,-1.0f,1.0f, -1.0f,1.0f,1.0f,1.0f, 1.0f,1.0f,1.0f,1.0f},
        {-1.0f,1.0f,-1.0f,1.0f, 1.0f,1.0f,1.0f,1.0f, 1.0f,1.0f,-1.0f,1.0f},

        {1.0f,-1.0f,1.0f,1.0f, -1.0f,-1.0f,1.0f,1.0f, -1.0f,-1.0f,-1.0f,1.0f},
        {1.0f,-1.0f,1.0f,1.0f, -1.0f,-1.0f,-1.0f,1.0f, 1.0f,-1.0f,-1.0f,1.0f},
    };*/

    return kaka;
}

int mainloop(){
    //Setup 
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow(":v", WIDTH,HEIGHT,0);
    renderer = SDL_CreateRenderer(window,NULL);

    if (window==NULL){
        SDL_Log("Ventana no creada");
        SDL_Quit();
        return 1;
    }

    SDL_Event event;

    //Hacer Cubo
    Mesh cubo = CUBOIDE();  

    SDL_SetRenderDrawColor(renderer,0,0,0,0xff);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer,0xFF,0xFF,0xFF,0xFF);
    SDL_RenderLine(renderer,0,0,WIDTH,HEIGHT);

    SDL_RenderPresent(renderer);

    int degrees = 0;
    float radians = 0;

    cubo.trans.t.z = 3;

    while (1){
        if (SDL_PollEvent(&event))
            if(event.type==SDL_EVENT_QUIT)
                break;

        KuroCam camera;
        camera.aspectRatio = (float)HEIGHT/(float)WIDTH;
        camera.FOV = 90;
        camera.fNear = 0.1f;
        camera.fFar = 1000.0f;

        radians = M_PI*degrees/180;
        degrees++;
        if(degrees > 360) degrees = 0;

        cubo.trans.r.x = radians;
        //cubo.trans.r.y = radians;
        cubo.trans.r.z = radians;

        
        SDL_SetRenderDrawColor(renderer,0,0,0,0xff);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer,0xFF,0xFF,0xFF,0xFF);
        RenderMesh(renderer, camera, cubo, WIDTH, HEIGHT);

        SDL_RenderPresent(renderer);

        std::this_thread::sleep_for<int64_t, std::milli>(std::chrono::milliseconds(1000/35));

    }
    

    SDL_Quit();
    return 0;
}

int main(){

    std::cout << "Hello World" << std::endl;

    mainloop();
    return 0;
}