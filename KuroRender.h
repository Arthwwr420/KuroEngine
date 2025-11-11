#include<SDL3/SDL.h>
#include<cmath>
#include"3dobj.h"

struct KuroCam{
    Vect3 pos, rot;
    float fNear;
    float fFar;
    float FOV;
    float aspectRatio; // H/W
    static float fixedFOV;
};

float KuroCam::fixedFOV = 0.0f;

bool SetCamFixedFOV(KuroCam& cam){
    cam.fixedFOV = 1.0f / tanf(cam.FOV*0.5f / 180.0f * 3.1415926f);
    return true;
}

bool Create4x4MatFromCam(mat4& matrix, KuroCam *camera){
    matrix.m[0][0] = camera->aspectRatio*camera->fixedFOV;
    matrix.m[1][1] = camera->fixedFOV;
    matrix.m[2][2] = camera->fFar/(camera->fFar-camera->fNear);
    matrix.m[3][2] = (- camera->fFar * camera->fNear)/(camera->fFar-camera->fNear);
    matrix.m[2][3] = 1.0f;
    matrix.m[3][3] = 0.0f;

    return true;
}

bool AdjustViewForScreen(Vect3& vector,  int width, int height){
    vector = vector + Vect3{1.0f,1.0f,0,0};
    vector.x *= 0.5f*(float)width;
    vector.y *= 0.5f*(float)height;

    return true;

}

bool RenderTriangleEdges(SDL_Renderer *renderer, const Triangle& tri){
    SDL_RenderLine(renderer, tri.p[0].x, tri.p[0].y, tri.p[1].x, tri.p[1].y);
    SDL_RenderLine(renderer, tri.p[2].x, tri.p[2].y, tri.p[1].x, tri.p[1].y);
    SDL_RenderLine(renderer, tri.p[0].x, tri.p[0].y, tri.p[2].x, tri.p[2].y);
    return true;
}

bool RenderMesh(SDL_Renderer* renderer, KuroCam &cam, Mesh &mesh, const int width, const int height){
    mat4 matrix;
    SetCamFixedFOV(cam);
    Create4x4MatFromCam(matrix, &cam);
    for (Triangle tri : mesh.t){
        Triangle trans = {0};
        Vect3 temp = {0};
        Vect3 temp2;
        for (int i = 0; i<3 ; i++){
            //Transform each point
            temp2 = tri.p[i]; //FIXME: I'on want no extra memory beign wasted here
            //Rotation
            Vect3Rotate(temp2, mesh.trans.r);

            //Translation
            Vect3Translate(temp2, mesh.trans.t);

            //Projection
            MultiplyMatVec(temp, temp2,matrix);
            AdjustViewForScreen(temp,width,height);

            //FINISH
            trans.p[i] = temp;
        }
        
        VectorNormal(temp, trans);
        if ( temp.z < 0)
            RenderTriangleEdges(renderer, trans);
    }
    return true;
}

