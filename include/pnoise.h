#ifndef PNOISE_H
#define PNOISE_H

//----------------------------------------------------------------------------------------------------------------------
/// git me sum nois
/// https://solarianprogrammer.com/2012/07/18/perlin-noise-cpp-11/
/// http://cs.nyu.edu/~perlin/noise/
//----------------------------------------------------------------------------------------------------------------------

#include <vector>

class PNoise
{
    public:
        //----------------------------------------------------------------------------------------------------------------------
        /// @brief ctor
        //----------------------------------------------------------------------------------------------------------------------
        PNoise();

        PNoise(unsigned int _seed);

        //----------------------------------------------------------------------------------------------------------------------
        /// @brief dtor
        //----------------------------------------------------------------------------------------------------------------------
        ~PNoise();
        //----------------------------------------------------------------------------------------------------------------------
        /// @brief handle events to this game entity object
        //----------------------------------------------------------------------------------------------------------------------
        double getNoise(double _x, double _y, double _z);

    private:

        std::vector<int> p;

        inline double fade(double _t){return _t * _t * _t * (_t * (_t * 6 - 15) + 10);}
        inline double lerp(double _t, double _a, double _b){return _a + _t * (_b - _a); }
        inline double grad(int _hash, double _x, double _y, double _z)
        {
            int h = _hash & 15;                      // CONVERT LO 4 BITS OF HASH CODE
            double u = h<8 ? _x : _y,                 // INTO 12 GRADIENT DIRECTIONS.
            v = h<4 ? _y : h==12||h==14 ? _x : _z;
            return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
        }
};

#endif // PNOISE_H
