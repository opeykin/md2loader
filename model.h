#ifndef MODEL_HP
#define MODEL_HP

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

    typedef glm::vec3 vec3;
    typedef glm::vec2 vec2;
    typedef std::vector<vec3> Triangles;
    typedef std::vector<vec2> UV;

    //enum ANIMATION {STAND = 0, RUN, ATTACK, PAIN1, PAIN2, PAIN3 };

    Model_MD2();
    int Load(const char *filename, const char* textureFilename);
    void Do(AnimationType new_animation);
    void Draw();
    void process_animation();

private:
    int startFrame(AnimationType animation);
    int animationLength(AnimationType animation);
    int getNextFramenr();
    void copy(float* src);
    void interpolate(float ratio, float* cur_frame, float* next_frame);

    GLuint vertexbuffer;
    GLuint uvbuffer;
    GLuint	texturen;
    Triangles cur_trinagles;

    float* Faces_Triangles[512];
    float* Faces_Textures[512];
    int TotalConnectedTriangles;

    AnimationType animation;
    int framenr;
    static const clock_t frame_change_interval = 150000;

//    int frame_length[11];
 //   int frame_start[6];
    static Animation anim_list[21];
};


#endif // MODEL_HP
