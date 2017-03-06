#include "terrain.h"
#include <SDL.h>
#include <iostream>
#include <ngl/NGLInit.h>
#include <ngl/ShaderLib.h>
#include <ngl/Material.h>
#include <ngl/Transformation.h>
#include <ngl/Camera.h>
#include <ngl/Light.h>
#include <ngl/VAOFactory.h>
#include <stdlib.h>
#include "pnoise.h"
#include <cmath>

Terrain::Terrain(ngl::Camera* _cam)
{
    m_name = "terrain";
    m_cam = _cam;
    m_time = 0;
    m_increment = 0.25;

    cols = 20; //width
    rows = 20; //depth

    m_modelPos = ngl::Vec3(-cols/2,-5,-rows);

    buildVAO();

    //std::cout<<m_name<<" CTOR CALLED\n";
    // now to load the shader and set the values
    // grab an instance of shader manager
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();

    // we are creating a shader called Phong
    shader->createShaderProgram("TerrainMtl");
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
    shader->attachShaderToProgram("TerrainMtl","PhongVertex");
    shader->attachShaderToProgram("TerrainMtl","PhongFragment");
    // now bind the shader attributes for most NGL primitives we use the following
    // layout attribute 0 is the vertex data (x,y,z)
    shader->bindAttribute("TerrainMtl",0,"inVert");
    // attribute 1 is the UV data u,v (if present)
    shader->bindAttribute("TerrainMtl",1,"inUV");
    // attribute 2 are the normals x,y,z
    shader->bindAttribute("TerrainMtl",2,"inNormal");

    // now we have associated this data we can link the shader
    shader->linkProgramObject("TerrainMtl");
    // and make it active ready to load values
    (*shader)["TerrainMtl"]->use();
    // the shader will use the currently active material and light0 so set them
    ngl::Material m(ngl::STDMAT::BLACKPLASTIC);
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

    std::cout<<m_name<<" CTOR CALLED in Terrain class\n";
}


void Terrain::buildVAO()
{
    for(double y = 0; y<rows; y+=m_increment)
    {
        for(double x = 0; x<cols; x+=m_increment)
        {
            m_terrain.push_back(ngl::Vec3(x,0,y));
        }
    }


    //m_vao.reset(ngl::VAOFactory::createVAO("simpleVAO",GL_TRIANGLE_STRIP));
    //m_vao->bind();

    //

    //m_vao->setData(ngl::AbstractVAO::VertexData(,m_terrain[0].m_x));
}

Terrain::~Terrain()
{
    std::cout<<m_name<<" DTOR CALLED in Terrain class\n";
}

int Terrain::update(double _timestep)
{
    //std::cout<<_timestep<<m_name<<" UPDATE CALL\n";
    PNoise pn;
    m_time+=_timestep;

    double speed = -0.0005;
    double height = 6;
    double freq = 0.01;

    //reset values

    for(double y = 0; y<rows/m_increment; ++y)
    {
        for(double x = 0; x<cols/m_increment; ++x)
        {
            m_terrain[x+(y*(rows/m_increment))]=ngl::Vec3(x*m_increment,0,y*m_increment);
        }
    }

    for(int iter = 0; iter<5; ++iter)
    {
        double i_speed = speed*pow(2,iter);
        double i_height = height/pow(2,iter);
        double i_freq = freq*pow(2,iter);
        double yoff = 0+(m_time*i_speed);

        for(double y = 0; y<rows/m_increment; ++y)
        {
            double xoff = 0;
            for(double x = 0; x<cols/m_increment; ++x)
            {
                m_terrain[y*(rows/m_increment)+x].m_y+=i_height*pn.getNoise(xoff, yoff,0);
                xoff+=i_freq;
            }
            yoff+=i_freq;
        }
    }

    return 0;
}

void Terrain::draw()
{
    //grab an instance of the shader manager
    ngl::ShaderLib *shader=ngl::ShaderLib::instance();
    (*shader)["TerrainMtl"]->use();

    ngl::VAOPrimitives *prim=ngl::VAOPrimitives::instance();

    for(auto i_pos : m_terrain)
    {
        m_transform.reset();
        m_transform.setPosition(m_modelPos+i_pos);
        //m_transform.setRotation(100*i_pos);
        m_transform.setScale(0.1,0.1,0.1);
        //LOADMATRICESTOSHADER
        //ngl::ShaderLib *shader=ngl::ShaderLib::instance();
        ngl::Mat4 MV;
        ngl::Mat4 MVP;
        ngl::Mat3 normalMatrix;
        ngl::Mat4 M;

        M=m_transform.getMatrix();

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

        // draw
        prim->draw("football");
    }
}
