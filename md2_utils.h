#ifndef MD2_UTILS_H
#define MD2_UTILS_H

struct MD2Header
{
    int ident;        // identifies as quake II header  (IDP2)
    int version;      // mine is 8
    int skinwidth;    // width of texture
    int skinheight;   // height of texture
    int framesize;    // number of bytes per frame
    int numSkins;     // number of textures
    int numXYZ;       // number of points
    int numST;        // number of texture
    int numTris;      // number of triangles
    int numGLcmds;
    int numFrames;    // total number of frames
    int offsetSkins;  // offset to skin names (64 bytes each)
    int offsetST;     // offset of texture s-t values
    int offsetTris;   // offset of triangle mesh
    int offsetFrames; // offset of frame data (points)
    int offsetGLcmds;
    int offsetEnd;    // end of file
};

struct MD2FramePoint
{
    unsigned char v[3];             // the vertex
    unsigned char lightNormalIndex;
};

struct MD2Frame
{
    float scale[3];                 // vetex scaling
    float translate[3];             // vertex translation
    char name[16];                  // name of this model
    MD2FramePoint fp[1];               // start of a list of framePoints
};

struct MD2Mesh
{
    unsigned short meshIndex[3];     // indices to triangle vertices
    unsigned short stIndex[3];       // indices to texture coordinates
};

struct Mesh_UV {
  unsigned short s;
  unsigned short t;
 };

// animation
typedef struct {
    int     first_frame;            // first frame of the animation
    int     last_frame;             // number of frames
    int     fps;                    // number of frames per second
} Animation;



#endif // MD2_UTILS_H
