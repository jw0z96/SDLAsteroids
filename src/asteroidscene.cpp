#include "asteroidscene.h"
#include <iostream>
#include <SDL.h>
#include <ngl/NGLInit.h>
#include <ngl/Camera.h>
#include <ngl/Light.h>
#include <cmath>

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

    // set the shape using FOV 45 Aspect Ratio based on Width and Height
    // The final two are near and far clipping planes of 0.5 and 10
    m_cam->setShape(45,(float)720.0/576.0,0.05,350);

    //create the player game entity
    //m_gameEntities.emplace_back(new Player(m_cam));
    //m_gameEntities.emplace_back(new Emmiter(m_cam));
    //m_gameEntities.emplace_back(new Terrain(m_cam));


    m_player = new Player(m_cam);
    m_emmiter = new Emmiter(m_cam);
    //m_terrain = new Terrain(m_cam);

}

AsteroidScene::~AsteroidScene() //DTOR
{
    std::cout<<"SCENE DTOR CALLED\n";
    //delete m_light;
    delete m_cam;
    delete m_player;
    delete m_emmiter;
    //delete m_terrain;
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

    /*for(auto i_gameEntity : m_gameEntities)
    {
       i_gameEntity->event(_event);
    }*/

    m_player->event(_event);
    m_emmiter->event(_event);
    //m_terrain->event(_event);

    return 0;
}

int AsteroidScene::updateHandler(double _timestep)
{
    //std::cout<<_timestep<<" UPDATE CALL\n";

    /*
    for(auto i_gameEntity : m_gameEntities)
    {
        i_gameEntity->update(_timestep);
    }
    */

    /*
    m_cam->set(ngl::Vec3(0,1,3),
               m_gameEntities[0]->getPos(),
               ngl::Vec3(0.0,1.0,0.0));
    */

    m_player->update(_timestep);
    m_emmiter->update(_timestep);
    //m_terrain->update(_timestep);

    for(int i = 0; i<m_emmiter->getNumOfAsteroids(); ++i)
    {
        ngl::Vec3 dVec = m_emmiter->getAsteroidPos(i)-m_player->getPos();
        double pdist = fabs(sqrt(pow(dVec.m_x,2)+pow(dVec.m_y,2)+pow(dVec.m_z,2)));
        if(pdist<m_emmiter->getAsteroidSize(i)){std::cout<<"ouch!!\n";m_emmiter->killAsteroid(i);}
    }

    for(int i = 0; i<m_emmiter->getNumOfAsteroids(); ++i)
    {
        for(int j = 0; j<m_player->getNumOfBullets(); ++j)
        {
            ngl::Vec3 bVec = m_emmiter->getAsteroidPos(i)-m_player->getBulletPos(j);
            double bdist = fabs(sqrt(pow(bVec.m_x,2)+pow(bVec.m_y,2)+pow(bVec.m_z,2)));
            if(bdist<m_emmiter->getAsteroidSize(i)){std::cout<<"hit!!\n";m_emmiter->killAsteroid(i);m_player->killBullet(j);}
        }
    }

    return 0;
}

void AsteroidScene::draw()  //DRAW
{
    //std::cout<<"DRAW CALL\n";
    // clear the screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    /*for(auto i_gameEntity : m_gameEntities)
    {
        i_gameEntity->draw();
    }*/

    m_player->draw();
    m_emmiter->draw();
    //m_terrain->draw();

}
