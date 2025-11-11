#include<vector>

struct Vect3 {
    float x,y,z,n;
};

struct Transform {
    Vect3 t = {0.0f,0.0f,0.0f,1.0f};
    Vect3 r = {0.0f,0.0f,0.0f,1.0f};
    Vect3 s = {1.0f,1.0f,1.0f,1.0f};
};

Vect3 operator+(Vect3 a, Vect3 b){ //EXPECTS 2 NORMALIZED VECTORS
    Vect3 res;
    res.x = a.x+b.x;
    res.y = a.y+b.y;
    res.z = a.z+b.z;
    res.n = a.n;
    return res;
}

Vect3 operator+(Vect3 a, float b){
    Vect3 res;
    res.x = a.x + b;
    res.y = a.x + b;
    res.z = a.x + b;
    res.n = a.n;
    return res;
}

struct Triangle{
    Vect3 p[3];
};

struct Mesh{
    std::vector<Triangle> t;
    Transform trans;
}; 

struct mat4{
    float m[4][4]={0};
};

bool Normalize(Vect3 &vec){
    if (vec.n == 0) return false;
    vec.x /= vec.n; 
    vec.y /= vec.n; 
    vec.z /= vec.n; 
    vec.n /= vec.n;
    return true;
}


bool MultiplyMatVec(Vect3 &out, Vect3& in, mat4& mat){
    out.x = in.x*mat.m[0][0] + in.y*mat.m[1][0] + in.z*mat.m[2][0] + in.n*mat.m[3][0];
    out.y = in.x*mat.m[0][1] + in.y*mat.m[1][1] + in.z*mat.m[2][1] + in.n*mat.m[3][1];
    out.z = in.x*mat.m[0][2] + in.y*mat.m[1][2] + in.z*mat.m[2][2] + in.n*mat.m[3][2];
    out.n = in.x*mat.m[0][3] + in.y*mat.m[1][3] + in.z*mat.m[2][3] + in.n*mat.m[3][3];

    Normalize(out);

    return true;
}

bool Vect3Translate(Vect3& vector, //Vector to transform
                     Vect3& transform //Normalized Vector with Translation Values
                    ){
    if(transform.n != 1){
        throw "Invalid Transform vector: Not Normlized";
    } 
    vector= vector + transform;
    return true;
}

bool Vect3Rotate(Vect3& vector, //Vector to transform
                 Vect3& transform //Normalized Vector with Rotation Values (in radians)
                ){
     if(transform.n != 1){
        throw "Invalid Transform vector: Not Normlized";
    } 

    Vect3 temp;
    if(transform.x != 0){
        mat4 Xmatrix = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, cosf(transform.x), sinf(transform.x), 0.0f,
        0.0f, -sinf(transform.x), cosf(transform.x), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
        };
        
        MultiplyMatVec(temp, vector, Xmatrix);
        vector = temp;
    }

    if(transform.y != 0){
        mat4 Ymatrix = {
        cosf(transform.y), 0.0f, -sinf(transform.y), 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        sinf(transform.y), 0.0f, cosf(transform.y), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
        };

        MultiplyMatVec(temp, vector, Ymatrix);
        vector = temp;
    }

    if(transform.z != 0){
        mat4 Zmatrix = {
        cosf(transform.x), -sinf(transform.x), 0.0f, 0.0f,
        sinf(transform.x), cosf(transform.x), 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
        };
        
        MultiplyMatVec(temp, vector, Zmatrix);
        vector = temp;
    }
    


    return true;
}


bool VectorNormal(Vect3 &out, Triangle&in){
    Vect3 line1 = {
        in.p[1].x - in.p[0].x,
        in.p[1].y - in.p[0].y,
        in.p[1].z - in.p[0].z,
        1
    };
    Vect3 line2 = {
        in.p[2].x - in.p[0].x,
        in.p[2].y - in.p[0].y,
        in.p[2].z - in.p[0].z,
        1
    };

    out.x = line1.y*line2.z - line1.z*line2.y;
    out.y = line1.z*line2.x - line1.x*line2.z;
    out.z = line1.x*line2.y - line1.y*line2.x;

    float l = sqrtf(out.x*out.x + out.y*out.y + out.z*out.z);
    out.x /= l; out.y /= l; out.z /= l; 
    return true;

}




