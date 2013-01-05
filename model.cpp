#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "common/texture.hpp"
#include "model.h"

using namespace std;

Animation Model_MD2::anim_list[ 21 ] = {
    // first, last, fps

    {   0,  39,  9 },   // STAND
    {  40,  45, 10 },   // RUN
    {  46,  53, 10 },   // ATTACK
    {  54,  57,  7 },   // PAIN_A
    {  58,  61,  7 },   // PAIN_B
    {  62,  65,  7 },   // PAIN_C
    {  66,  71,  7 },   // JUMP
    {  72,  83,  7 },   // FLIP
    {  84,  94,  7 },   // SALUTE
    {  95, 111, 10 },   // FALLBACK
    { 112, 122,  7 },   // WAVE
    { 123, 134,  6 },   // POINT
    { 135, 153, 10 },   // CROUCH_STAND
    { 154, 159,  7 },   // CROUCH_WALK
    { 160, 168, 10 },   // CROUCH_ATTACK
    { 196, 172,  7 },   // CROUCH_PAIN
    { 173, 177,  5 },   // CROUCH_DEATH
    { 178, 183,  7 },   // DEATH_FALLBACK
    { 184, 189,  7 },   // DEATH_FALLFORWARD
    { 190, 197,  7 },   // DEATH_FALLBACKSLOW
    { 198, 198,  5 },   // BOOM
};

Model_MD2::Model_MD2() {
    for (int i = 0; i < 512; ++i ) {
        Faces_Triangles[i] = new float[14096];
        Faces_Textures[i] = new float[14096];
    }
    cur_trinagles = vector<float>(14096);
    cur_uv = vector<float>(14096);


    glGenBuffers(1, &vertexbuffer);
    glGenBuffers(1, &uvbuffer);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    framenr = 0;
    repeat_animation = true;
    animation = STAND;
    next_animation = STAND;
/*
    frame_length[0] = 39;	// stand
    frame_length[1] = 5;	// run
    frame_length[2] = 7;	// attack
    frame_length[3] = 3;	//	"pain1"
    frame_length[4] = 3;	//	"pain2" 3
    frame_length[5]	= 3;	// "pain3" 3
    frame_length[6] = 5;	// "jump" 5
    frame_length[7] = 11;	// "flip" 11
    frame_length[8] = 10;	// "salute" 10
    frame_length[9] = 16;	// "taunt" 16
    frame_length[10] = 10;	// "wave" 10


    frame_start[0] = 0;		// stand
    frame_start[1] = 40;	// run
    frame_start[2] = 46;	// shoot
    frame_start[3] = 50;	// pain1
    frame_start[4] = 39 + 5+ 7 + 3;
    frame_start[5] = frame_length[0] + frame_length[1] + frame_length[2] + frame_length[3] + frame_length[4];
*/

}

int Model_MD2::Load(const char* filename, const char* textureFilename) {
    int texWidth;
    int texHeight;
    texturen = loadBMP_custom(textureFilename, texWidth, texHeight);
    MD2Header *mdh;
    MD2Mesh* m;

    TotalConnectedTriangles = 0;

    if (strstr(filename,".md2") == NULL) {
        printf("File does not have a .MD2 extension. ");
        return 1;
    }

    FILE* file = fopen(filename,"r");

    if (file == NULL) {
        printf("File can't be opened\n");
        return 1;
    }

    // get size of file
    fseek( file, 0, SEEK_END );
    size_t  fileSize = ftell( file );
    fseek( file, 0, SEEK_SET );

    // read in entire file
    char* buffer;
    buffer = (char*) malloc( fileSize+1 );
    fread( buffer, sizeof( char ), fileSize, file );


    // start analyzing the buffer
    mdh = (MD2Header *)buffer;

    printf("mdh framesize %i \n", mdh->framesize);

    std::cout << mdh->numFrames << std::endl;

    for (int z = 0; z< mdh->numFrames; z++) {
        std::vector<float> Points;
        std::vector<float> TextureCoords;
        MD2Frame * frm = (MD2Frame*)&buffer[ mdh->offsetFrames + z*mdh->framesize  ];
        m = (MD2Mesh*)&buffer[mdh->offsetTris];

        /* offset to points of frame */
        for(int i=0; i<mdh->numXYZ; i++) {
            Points.push_back(frm->scale[0] * frm->fp[i].v[0] + frm->translate[0]); // X
            Points.push_back(frm->scale[1] * frm->fp[i].v[1] + frm->translate[1]); // Y
            Points.push_back(frm->scale[2] * frm->fp[i].v[2] + frm->translate[2]); // Z
        }
        this->TotalConnectedTriangles = mdh->numTris * 3;

        //-- create texture coordinate list ---------------------------

        for(int i=0; i<mdh->numST; i++ ) {
            Mesh_UV* mUV = (Mesh_UV *)&buffer[mdh->offsetST + i*4 ];
            TextureCoords.push_back((float) mUV->s / texWidth);
            TextureCoords.push_back((float) mUV->t / texHeight);
        }

        int n = 0;
        int ti = 0;

        for(int i=0; i<mdh->numTris; i++ ) {
            this->Faces_Triangles[z][n] = Points[ 3*m[i].meshIndex[0] ];
            this->Faces_Triangles[z][n+1] = Points[ 3*m[i].meshIndex[0]+1 ];
            this->Faces_Triangles[z][n+2] = Points[ 3*m[i].meshIndex[0]+2 ];

            this->Faces_Textures[z][ti] = TextureCoords[ 2*m[i].stIndex[0] ];
            this->Faces_Textures[z][ti+1] = TextureCoords[ 2*m[i].stIndex[0]+1 ];

            n+=3;
            ti += 2;

            this->Faces_Triangles[z][n] = Points[ 3*m[i].meshIndex[1] ];
            this->Faces_Triangles[z][n+1] = Points[3* m[i].meshIndex[1]+1 ];
            this->Faces_Triangles[z][n+2] = Points[3* m[i].meshIndex[1]+2 ];

            this->Faces_Textures[z][ti] = TextureCoords[ 2*m[i].stIndex[1] ];
            this->Faces_Textures[z][ti+1] = TextureCoords[ 2*m[i].stIndex[1] + 1 ];

            n+=3;
            ti += 2;

            this->Faces_Triangles[z][n] = Points[3* m[i].meshIndex[2] ];
            this->Faces_Triangles[z][n+1] = Points[3* m[i].meshIndex[2]+1 ];
            this->Faces_Triangles[z][n+2] = Points[3* m[i].meshIndex[2]+2 ];

            this->Faces_Textures[z][ti] = TextureCoords[ 2*m[i].stIndex[2] ];
            this->Faces_Textures[z][ti+1] = TextureCoords[ 2*m[i].stIndex[2] + 1 ];

            n+=3;
            ti += 2;
        }
    }
    printf("\n");

    return 0;
}

void Model_MD2::Do(AnimationType new_animation, bool repeat, AnimationType return_animation) {
    if (animation != new_animation) {
        animation = new_animation;
        framenr = 0;
    }

    repeat_animation = repeat;
    next_animation = return_animation;
}

void Model_MD2::Draw() {
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, TotalConnectedTriangles * 3 * sizeof(float), &cur_trinagles[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, TotalConnectedTriangles * 2 * sizeof(float), &cur_uv[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindTexture(GL_TEXTURE_2D, texturen);

    glDrawArrays(GL_TRIANGLES, 0, TotalConnectedTriangles * 3);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}


void Model_MD2::processAnimation() {
    static clock_t last_frame_time = clock();
    clock_t current_time = clock();
    float ratio = (current_time - last_frame_time) / (float)frame_change_interval;

    int cur_frame = framenr + startFrame(animation);
    int next_frame;
    if (getNextFramenr() == 0 & !repeat_animation) {
        next_frame = startFrame(next_animation);
    } else {
        next_frame = getNextFramenr() + startFrame(animation);
    }

    interpolate(cur_trinagles, Faces_Triangles[cur_frame], Faces_Triangles[next_frame], TotalConnectedTriangles * 3, ratio);
    interpolate(cur_uv, Faces_Textures[cur_frame], Faces_Textures[next_frame], TotalConnectedTriangles * 2, ratio);

    if (ratio >= 1) {
        if (getNextFramenr() == 0 & !repeat_animation) {
            animation = next_animation;
            repeat_animation = true;
        }
        last_frame_time = current_time;
        framenr = getNextFramenr();
    }
}

int Model_MD2::startFrame(Model_MD2::AnimationType animation) {
    return anim_list[animation].first_frame;
}

int Model_MD2::animationLength(Model_MD2::AnimationType animation) {
    return anim_list[animation].last_frame - anim_list[animation].first_frame;
}

void Model_MD2::interpolate(vector<float>& dest, float *cur_frame, float *next_frame, int size, float ratio) {
    float p = ratio;
    float q = 1 - p;
    for (int i = 0; i < size; ++i) {
        dest[i] = q * cur_frame[i] + p * next_frame[i];
    }
}

int Model_MD2::getNextFramenr() {
    return (framenr + 1) % (animationLength(animation) + 1);
}
