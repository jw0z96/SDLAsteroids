#include "bullet.h"
#include <SDL.h>
#include <iostream>
#include <ngl/NGLInit.h>
#include <ngl/ShaderLib.h>
#include <ngl/Material.h>
#include <ngl/Transformation.h>
#include <ngl/Camera.h>
#include <ngl/Light.h>

Bullet::Bullet(ngl::Camera* _cam, ngl::Vec3 _pos)
{
    m_name = "bullet";
    m_cam = _cam;
    m_modelVel = ngl::Vec3(0.0,0.0,-0.01);
    m_modelPos = _pos;
    m_life = 0;
    //std::cout<<m_name<<" CTOR CALLED in Asteroid class\n";
}

Bullet::~Bullet()
{
    std::cout<<m_name<<" DTOR CALLED in bullet class\n";
}

int Bullet::event(SDL_Event& _event)
{
    //std::cout<<_event.type<<m_name<<" EVENT HANDLE CALL\n";

    return 0;
}

int Bullet::update(double _timestep)
{
    //std::cout<<_timestep<<m_name<<" UPDATE CALL\n";

    if(m_modelPos.m_z < -50){return 1;}

    m_modelPos+=m_modelVel*_timestep;

    return 0;
}

void Bullet::draw()
{
    //grab an instance of the shader manager
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    (*shader)["AsteroidShader"]->use();

    m_transform.reset();
    m_transform.setPosition(m_modelPos);
    m_transform.setScale(0.2,0.2,0.2);
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
    prim->draw("football");
}

