#ifndef MODEL_HP
#define MODEL_HP

#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
#include <vector>
#include <ctime>

#include "md2_utils.h"

class Model_MD2
{
public:
    enum AnimationType {
        STAND,
        RUN,
        ATTACK,
        PAIN_A,
        PAIN_B,
        PAIN_C,
        JUMP,
        FLIP,
        SALUTE,
        FALLBACK,
        WAVE,
        POINT,
        CROUCH_STAND,
        CROUCH_WALK,
        CROUCH_ATTACK,
        CROUCH_PAIN,
        CROUCH_DEATH,
        DEATH_FALLBACK,
        DEATH_FALLFORWARD,
        DEATH_FALLBACKSLOW,
        BOOM,
        MAX_ANIMATIONS
    };

    Model_MD2();
    int Load(const char *filename, const char* textureFilename);
    void Do(AnimationType new_animation, bool repeat = false, AnimationType return_animation = STAND);
    void Draw();
    void processAnimation();
    AnimationType getCurrentAnimation() const  {
        return animation;
    }

private:
    int startFrame(AnimationType animation);
    int animationLength(AnimationType animation);
    int getNextFramenr();
    //void copy(float* src);
    void interpolate(std::vector<float>& dest, float *cur_frame, float *next_frame, int size, float ratio);

    GLuint vertexbuffer;
    GLuint uvbuffer;
    GLuint	texturen;
    std::vector<float> cur_trinagles;
    std::vector<float> cur_uv;

    float* Faces_Triangles[512];
    float* Faces_Textures[512];
    int TotalConnectedTriangles;

    AnimationType animation;
    AnimationType next_animation;
    bool repeat_animation;
    int framenr;
    static const clock_t frame_change_interval = 150000;

    static Animation anim_list[21];
};


#endif // MODEL_HP
