#include "player.h"
#include <SDL.h>
#include <iostream>
#include <ngl/NGLInit.h>
#include <ngl/ShaderLib.h>
#include <ngl/Material.h>
#include <ngl/Transformation.h>
#include <ngl/Camera.h>
#include <ngl/Light.h>
#include "bullet.h"

Player::Player(ngl::Camera* _cam)
{
    m_name = "player";
    m_cam = _cam;
    keyX = 0;
    keyY = 0;
    DOT_VEL = 0.001;
    firing = false;
    lastFired = SDL_GetTicks();
    firingWait = 100;

    //std::cout<<m_name<<" CTOR CALLED\n";
    // now to load the shader and set the values
    // grab an instance of shader manager
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();

    // we are creating a shader called Phong
    shader->createShaderProgram("Phong");
    // now we are going to create empty shaders for Frag and Vert
    shader->attachShader("PhongVertex",ngl::ShaderType::VERTEX);
    shader->attachShader("PhongFragment",ngl::ShaderType::FRAGMENT);
    // attach the source
    shader->loadShaderSource("PhongVertex","shaders/PhongVertex.glsl");
    shader->loadShaderSource("PhongFragment","shaders/PhongFragment.glsl");
    // compile the shaders
    shader->compileShader("PhongVertex");
    shader->compileShader("PhongFragment");
    // add them to the program
    shader->attachShaderToProgram("Phong","PhongVertex");
    shader->attachShaderToProgram("Phong","PhongFragment");
    // now bind the shader attributes for most NGL primitives we use the following
    // layout attribute 0 is the vertex data (x,y,z)
    shader->bindAttribute("Phong",0,"inVert");
    // attribute 1 is the UV data u,v (if present)
    shader->bindAttribute("Phong",1,"inUV");
    // attribute 2 are the normals x,y,z
    shader->bindAttribute("Phong",2,"inNormal");

    // now we have associated this data we can link the shader
    shader->linkProgramObject("Phong");
    // and make it active ready to load values
    (*shader)["Phong"]->use();
    // the shader will use the currently active material and light0 so set them
    ngl::Material m(ngl::STDMAT::GOLD);
    // load our material values to the shader into the structure material (see Vertex shader)
    m.loadToShader("material");

    shader->setShaderParam3f("viewerPos",m_cam->getEye().m_x,m_cam->getEye().m_y,m_cam->getEye().m_z);

    // now create our light this is done after the camera so we can pass the
    // transpose of the projection matrix to the light to do correct eye space
    // transformations
    ngl::Mat4 iv=m_cam->getViewMatrix();
    iv.transpose();
    ngl::Light m_light(ngl::Vec3(0,10,0),ngl::Colour(1,1,1,1),ngl::Colour(1,1,1,1),ngl::LightModes::POINTLIGHT );
    m_light.setTransform(iv);
    // load these values to the shader as well
    m_light.loadToShader("light");

    std::cout<<m_name<<" CTOR CALLED in Player class\n";
}

Player::~Player()
{
    std::cout<<m_name<<" DTOR CALLED in Player class\n";
}

int Player::event(SDL_Event& _event)
{
    //std::cout<<_event.type<<m_name<<" EVENT HANDLE CALL\n";
    if( _event.type == SDL_KEYDOWN && _event.key.repeat == 0){
        switch( _event.key.keysym.sym){
            case SDLK_UP: ++keyY; break;// std::cout << "up\n"; break;
            case SDLK_DOWN: --keyY; break;// std::cout << "down\n"; break;
            case SDLK_LEFT: --keyX; break;//std::cout << "left\n"; break;
            case SDLK_RIGHT:  ++keyX; break;//std::cout << "right\n"; break;
            case SDLK_SPACE: firing = true; break;
            }
        }
    else if( _event.type == SDL_KEYUP && _event.key.repeat == 0){
        switch( _event.key.keysym.sym ){
            case SDLK_UP: --keyY; break;
            case SDLK_DOWN: ++keyY; break;
            case SDLK_LEFT: ++keyX; break;
            case SDLK_RIGHT: --keyX; break;
            case SDLK_SPACE: firing = false; break;
            }
        }

    return 0;
}

int Player::update(double _timestep)
{
    //std::cout<<_timestep<<m_name<<" UPDATE CALL\n";
    //std::cout<<keyX<<","<<keyY<<"\n";

    if(keyX != 0){m_modelVel.m_x+=keyX*DOT_VEL;}
    else{m_modelVel.m_x *= 0.8;}
    if(keyY != 0){m_modelVel.m_y+=keyY*DOT_VEL;}
    else{m_modelVel.m_y *= 0.8;}

    m_modelPos.m_x+=m_modelVel.m_x*_timestep;
    m_modelPos.m_y+=m_modelVel.m_y*_timestep;

    if(firing){fire();}

    for(int i; i<m_bullets.size(); i++)
    {
        int j = m_bullets[i]->update(_timestep);
        if(j)
        {
            m_bullets.erase(m_bullets.begin()+i); i--;
        }
    }

    return 0;
}

void Player::draw()
{
    //grab an instance of the shader manager
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    (*shader)["Phong"]->use();

    m_transform.reset();
    m_transform.setPosition(m_modelPos);
    m_transform.setRotation(0,-90,0);

    //LOADMATRICESTOSHADER
    //ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    ngl::Mat4 MV;
    ngl::Mat4 MVP;
    ngl::Mat3 normalMatrix;
    ngl::Mat4 M;

    M.m_m[3][0] = m_modelPos.m_x;
    M.m_m[3][1] = m_modelPos.m_y;
    M.m_m[3][2] = m_modelPos.m_z;

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
    prim->draw("troll");

    for(auto i_bullet : m_bullets)
    {
        i_bullet->draw();
    }
}

void Player::fire()
{
    if(SDL_GetTicks()-lastFired > firingWait)
    {
        std::cout << "fire!!\n";
        lastFired = SDL_GetTicks();
        m_bullets.emplace_back(new Bullet(m_cam, m_modelPos));
    }
}
