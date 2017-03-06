# This specifies the exe name
TARGET=SDLNGL
# where to put the .o files
OBJECTS_DIR=obj

# this demo uses SDL so add the paths using the sdl2-config tool
QMAKE_CXXFLAGS+=$$system(sdl2-config  --cflags)
message(output from sdl2-config --cflags added to CXXFLAGS= $$QMAKE_CXXFLAGS)

LIBS+=$$system(sdl2-config  --libs)
message(output from sdl2-config --libs added to LIB=$$LIBS)

# where to put moc auto generated file
MOC_DIR=moc

# Auto include all .cpp files in the project src directory (can specifiy individually if required)
SOURCES+= $$PWD/src/*.cpp

# same for the .h files
HEADERS+= $$PWD/include/*.h

# and add the include dir into the search path for Qt and make
INCLUDEPATH +=./include

# where our exe is going to live (root of project)
DESTDIR=./

# were are going to default to a console app
CONFIG += console

# add the glsl shader files
OTHER_FILES+= shaders/*.glsl \

NGLPATH=$$(NGLDIR)
isEmpty(NGLPATH){ # note brace must be here
        message("including $HOME/NGL")
        include($(HOME)/NGL/UseNGL.pri)
}
else{ # note brace must be here
        message("Using custom NGL location")
        include($(NGLDIR)/UseNGL.pri)
}

DISTFILES += \
    shaders/PhongFragment.glsl \
    shaders/PhongVertex.glsl
