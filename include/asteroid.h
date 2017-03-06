#ifndef ASTEROID_H
#define ASTEROID_H

//----------------------------------------------------------------------------------------------------------------------
/// This is a derived class, the event, update, draw
/// functions are to be refefined in the derived class.
//----------------------------------------------------------------------------------------------------------------------

#include "gameentity.h"

class Asteroid: public GameEntity
{
    public:
        //----------------------------------------------------------------------------------------------------------------------
        /// @brief ctor
        //----------------------------------------------------------------------------------------------------------------------
        Asteroid(ngl::Camera* _cam, double _x, double _y);
        //----------------------------------------------------------------------------------------------------------------------
        /// @brief dtor
        //----------------------------------------------------------------------------------------------------------------------
        ~Asteroid();
        //----------------------------------------------------------------------------------------------------------------------
        /// @brief handle events to this game entity object
        //----------------------------------------------------------------------------------------------------------------------
        int event(SDL_Event& _event);
        //----------------------------------------------------------------------------------------------------------------------
        /// @brief update this game entity object
        //----------------------------------------------------------------------------------------------------------------------
        int update(double _timestep);
        //----------------------------------------------------------------------------------------------------------------------
        /// @brief draw this game entity object
        //----------------------------------------------------------------------------------------------------------------------
        void draw();
    private:
        //----------------------------------------------------------------------------------------------------------------------
        /// @brief when was i born?
        //----------------------------------------------------------------------------------------------------------------------
        //double birthTime;
        //bool m_life;
};

#endif // ASTEROID_H