#include "sphere.h"

inline void push_indices(vector<GLushort>& indices, int sectors, int r, int s) {
    int curRow = r * sectors;
    int nextRow = (r+1) * sectors;

    indices.push_back(curRow + s);
    indices.push_back(nextRow + s);
    indices.push_back(nextRow + (s+1));

    indices.push_back(curRow + s);
    indices.push_back(nextRow + (s+1));
    indices.push_back(curRow + (s+1));
}

void createSphere(vector<vec3>& vertices, vector<GLushort>& indices, vector<vec2>& texcoords,
             float radius, unsigned int rings, unsigned int sectors)
{
    float const R = 1./(float)(rings-1);
    float const S = 1./(float)(sectors-1);

    for(int r = 0; r < rings; ++r) {
        for(int s = 0; s < sectors; ++s) {
            float const y = sin( -M_PI_2 + M_PI * r * R );
            float const x = cos(2*M_PI * s * S) * sin( M_PI * r * R );
            float const z = sin(2*M_PI * s * S) * sin( M_PI * r * R );

            texcoords.push_back(vec2(s*S, r*R));
            vertices.push_back(vec3(x,y,z) * radius);
            push_indices(indices, sectors, r, s);
        }
    }
}
