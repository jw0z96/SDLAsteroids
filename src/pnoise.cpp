#include "pnoise.h"
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <numeric>
#include <random>
#include <algorithm>


PNoise::PNoise()
{
    p.resize(256);

    // Fill p with values from 0 to 255
    std::iota(p.begin(), p.end(), 0);

    // Initialize a random engine with seed
    std::default_random_engine engine(1337);

    // Suffle  using the above random engine
    std::shuffle(p.begin(), p.end(), engine);

    // Duplicate the permutation vector
    p.insert(p.end(), p.begin(), p.end());
}

PNoise::~PNoise()
{

}


PNoise::PNoise(unsigned int _seed)
{
    p.resize(256);

    // Fill p with values from 0 to 255
    std::iota(p.begin(), p.end(), 0);

    // Initialize a random engine with seed
    std::default_random_engine engine(_seed);

    // Suffle  using the above random engine
    std::shuffle(p.begin(), p.end(), engine);

    // Duplicate the permutation vector
    p.insert(p.end(), p.begin(), p.end());
}

double PNoise::getNoise(double _x, double _y, double _z)
{
    int X = int(floor(_x)) & 255;                  // FIND UNIT CUBE THAT
    int Y = int(floor(_y)) & 255;                  // CONTAINS POINT.
    int Z = int(floor(_z)) & 255;

    _x -= floor(_x);                               // FIND RELATIVE X,Y,Z
    _y -= floor(_y);                              // OF POINT IN CUBE.
    _z -= floor(_z);

    double u = fade(_x);                                // COMPUTE FADE CURVES
    double v = fade(_y);                                // FOR EACH OF X,Y,Z.
    double w = fade(_z);

    int A = p[X]+Y;
    int AA = p[A]+Z;
    int AB = p[A+1]+Z;      // HASH COORDINATES OF
    int B = p[X+1]+Y;
    int BA = p[B]+Z;
    int BB = p[B+1]+Z;      // THE 8 CUBE CORNERS,

    return lerp(w,lerp(v,lerp(
                u,grad(p[AA],_x,_y,_z),grad(p[BA  ], _x-1, _y  , _z   )),
                lerp(u, grad(p[AB  ], _x  , _y-1, _z   ),
                grad(p[BB  ], _x-1, _y-1, _z   ))),
                lerp(v, lerp(u, grad(p[AA+1], _x  , _y  , _z-1 ),
                grad(p[BA+1], _x-1, _y  , _z-1 )),
                lerp(u, grad(p[AB+1], _x  , _y-1, _z-1 ),
                grad(p[BB+1], _x-1, _y-1, _z-1 ))));
}
