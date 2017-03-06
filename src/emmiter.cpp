#include "emmiter.h"
#include <SDL.h>
#include <iostream>
#include <ngl/NGLInit.h>
#include <ngl/ShaderLib.h>
#include <ngl/Material.h>
#include <ngl/Transformation.h>
#include <ngl/Camera.h>
#include <ngl/Light.h>

#include "asteroid.h"
#include <cstdlib>
#include <SDL.h>

Emmiter::Emmiter(ngl::Camera* _cam)
{
    m_name = "emmiter";
    m_cam = _cam;
    lastSpawned = SDL_GetTicks();

    //std::cout<<m_name<<" CTOR CALLED\n";
    // now to load the shader and set the values
    // grab an instance of shader manager
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();

    // we are creating a shader called Phong
    shader->createShaderProgram("AsteroidShader");
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
    shader->attachShaderToProgram("AsteroidShader","PhongVertex");
    shader->attachShaderToProgram("AsteroidShader","PhongFragment");
    // now bind the shader attributes for most NGL primitives we use the following
    // layout attribute 0 is the vertex data (x,y,z)
    shader->bindAttribute("AsteroidShader",0,"inVert");
    // attribute 1 is the UV data u,v (if present)
    shader->bindAttribute("AsteroidShader",1,"inUV");
    // attribute 2 are the normals x,y,z
    shader->bindAttribute("Phong",2,"inNormal");

    // now we have associated this data we can link the shader
    shader->linkProgramObject("AsteroidShader");
    // and make it active ready to load values
    (*shader)["AsteroidShader"]->use();
    // the shader will use the currently active material and light0 so set them
    ngl::Material m(ngl::STDMAT::CHROME);
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

    std::cout<<m_name<<" CTOR CALLED in Emmiter class\n";
}

Emmiter::~Emmiter()
{
    std::cout<<m_name<<" DTOR CALLED in Emmiter class\n";
}

float randomFloat(float LO, float HI)
{
    return LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
}

int Emmiter::event(SDL_Event& _event)
{
    //std::cout<<_event.type<<m_name<<" EVENT HANDLE CALL\n";
    /*
    if( _event.type == SDL_KEYDOWN && _event.key.repeat == 0)
    {
        switch( _event.key.keysym.sym)
        {
            case SDLK_SPACE:
                std::cout<<"fire!\n";
                m_asteroids.emplace_back(new Asteroid(m_cam, randomFloat(-5,5), randomFloat(-1,1)));
                break;
        }
    }

    */
    return 0;
}

int Emmiter::update(double _timestep)
{
    if(SDL_GetTicks()-lastSpawned > randomFloat(20,200))
    {
        lastSpawned = SDL_GetTicks();
        m_asteroids.emplace_back(new Asteroid(m_cam, randomFloat(-10,10), randomFloat(-1,1)));

    }

    //std::cout<<_timestep<<m_name<<" UPDATE CALL\n";
    for(int i; i<m_asteroids.size();i++)
    {
        int life = m_asteroids[i]->update(_timestep);
        if(life){m_asteroids.erase(m_asteroids.begin()+i); i--;}
    }
    return 0;
}

void Emmiter::draw()
{
    for(auto i_asteroid : m_asteroids)
    {
        i_asteroid->draw();
    }
}
