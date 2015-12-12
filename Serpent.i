%module Serpent

%{
#define SWIG_FILE_WITH_INIT
#include "Coordinates.h"
#include "Serpent.h"
%}

class Coordinates
{
public:
    Coordinates();
    ~Coordinates();
    size_t getSize() const;
    ssize_t getXVal(size_t idx) const;
    ssize_t getYVal(size_t idx) const;
    ssize_t getZVal(size_t idx) const;
};

class Serpent
{
public:
    static Serpent* CreateFromPath(const char* filename);
    Serpent();
    ~Serpent();
    Coordinates* getBlocksPos();
};
