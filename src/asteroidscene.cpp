#include "asteroidscene.h"
#include <iostream>
#include <SDL.h>
#include <ngl/NGLInit.h>
#include <ngl/Camera.h>
#include <ngl/Light.h>

#include "player.h"
#include "emmiter.h"
#include "terrain.h"

AsteroidScene::AsteroidScene()  //CTOR
{
    std::cout<<"SCENE CTOR CALLED\n";
    // Grey Background
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    // enable depth testing for drawing
    glEnable(GL_DEPTH_TEST);

    // Now we will create a basic Camera from the graphics library
    // This is a static camera so it only needs to be set once
    // First create Values for the camera position
    ngl::Vec3 from(0,3,6);
    ngl::Vec3 to(0,1,0);
    ngl::Vec3 up(0,1,0);
    // now load to our new camera
    m_cam= new ngl::Camera(from,to,up);

    //m_cam->move(5,0,-5);
    // set the shape using FOV 45 Aspect Ratio based on Width and Height
    // The final two are near and far clipping planes of 0.5 and 10
    m_cam->setShape(45,(float)720.0/576.0,0.05,350);

    //create the player game entity
    m_gameEntities.emplace_back(new Player(m_cam));
    m_gameEntities.emplace_back(new Emmiter(m_cam));
    m_gameEntities.emplace_back(new Terrain(m_cam));
    //m_player = m_gameEntities[0];

    //create the emmiter game entity
    //m_gameEntities.emplace_back(new GameEntity("emmiter"));
    //m_emmiter = m_gameEntities[1];
}

AsteroidScene::~AsteroidScene() //DTOR
{
    std::cout<<"SCENE DTOR CALLED\n";
    //delete m_light;
    delete m_cam;
}

void AsteroidScene::resize(int _w, int _h)
{
    //std::cout<<"RESIZE CALLED\n";
    glViewport(0,0,_w,_h);
    m_cam->setShape(45,(float)_w/_h,0.05,350);
}

int AsteroidScene::eventHandler(SDL_Event& _event)
{
    //std::cout<<_event.type<<" EVENT HANDLE CALL\n";
    for(auto i_gameEntity : m_gameEntities)
    {
        i_gameEntity->event(_event);
    }
    return 0;
}

int AsteroidScene::updateHandler(double _timestep)
{
    //std::cout<<_timestep<<" UPDATE CALL\n";
    for(auto i_gameEntity : m_gameEntities)
    {
        i_gameEntity->update(_timestep);
    }

    /*
    m_cam->set(ngl::Vec3(0,1,3),
               m_gameEntities[0]->getPos(),
               ngl::Vec3(0.0,1.0,0.0));
               */
    return 0;
}

void AsteroidScene::draw()  //DRAW
{
    //std::cout<<"DRAW CALL\n";
    // clear the screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(auto i_gameEntity : m_gameEntities)
    {
        i_gameEntity->draw();
    }

}
