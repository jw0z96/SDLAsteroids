#include "asteroid.h"
#include <SDL.h>
#include <iostream>
#include <ngl/NGLInit.h>
#include <ngl/ShaderLib.h>
#include <ngl/Material.h>
#include <ngl/Transformation.h>
#include <ngl/Camera.h>
#include <ngl/Light.h>

Asteroid::Asteroid(ngl::Camera* _cam, double _x, double _y)
{
    m_name = "asteroid";
    m_cam = _cam;
    m_modelVel = ngl::Vec3(0.0,0.0,0.01);
    m_modelPos = ngl::Vec3(_x,_y,-50.0);
    //birthTime = SDL_GetTicks();
    std::cout<<m_name<<" CTOR CALLED in Asteroid class\n";
}

Asteroid::~Asteroid()
{
    std::cout<<m_name<<" DTOR CALLED in Asteroid class\n";
}

int Asteroid::event(SDL_Event& _event)
{
    //std::cout<<_event.type<<m_name<<" EVENT HANDLE CALL\n";

    return 0;
}

int Asteroid::update(double _timestep)
{
    //std::cout<<_timestep<<m_name<<" UPDATE CALL\n";
    //std::cout<<keyX<<","<<keyY<<"\n";

    if(m_modelPos.m_z > 2){return 1;}

    m_modelPos+=m_modelVel*_timestep;



    return 0;
}

void Asteroid::draw()
{
    //grab an instance of the shader manager
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    (*shader)["AsteroidShader"]->use();

    m_transform.reset();
    m_transform.setPosition(m_modelPos);
    //m_transform.setRotation(0,-90,0);

    //LOADMATRICESTOSHADER
    //ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    ngl::Mat4 MV;
    ngl::Mat4 MVP;
    ngl::Mat3 normalMatrix;
    ngl::Mat4 M;

    M = m_transform.getMatrix();

    MV=  M*m_cam->getViewMatrix();
    MVP= M*m_cam->getVPMatrix();
    normalMatrix=MV;
    normalMatrix.inverse();
    shader->setShaderParamFromMat4("MV",MV);
    shader->setShaderParamFromMat4("MVP",MVP);
    shader->setShaderParamFromMat3("normalMatrix",normalMatrix);
    shader->setShaderParamFromMat4("M",M);

    //std::cout<<m_name<<" DRAW CALL\n";
    // get the VBO instance and draw the built in teapot
    ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();
    // draw
    prim->draw("teapot");
}
