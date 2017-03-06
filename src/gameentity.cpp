#include "gameentity.h"
#include <SDL.h>
#include <iostream>
#include <ngl/NGLInit.h>
#include <ngl/ShaderLib.h>
#include <ngl/Material.h>
#include <ngl/Transformation.h>
#include <ngl/Camera.h>
#include <ngl/Light.h>

GameEntity::GameEntity()
{
    m_name = "default";
    std::cout<<m_name<<" CTOR CALLED in Game Entity class\n";
}

GameEntity::~GameEntity()
{
    std::cout<<m_name<<" DTOR CALLED in Game Entity class\n";
}

int GameEntity::event(SDL_Event& _event)
{
    std::cout<<_event.type<<m_name<<" EVENT HANDLE CALL \n";

    return 0;
}

int GameEntity::update(double _timestep)
{
    std::cout<<_timestep<<m_name<<" UPDATE CALL\n";
    return 0;
}

void GameEntity::draw()
{

}
