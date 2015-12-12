%module Serpent

%{
#define SWIG_FILE_WITH_INIT
#include "Coordinates.h"
#include "Serpent.h"
%}

class Coordinates
{
public:
    size_t getSize() const;
    int getXVal(size_t idx) const;
    int getYVal(size_t idx) const;
    int getZVal(size_t idx) const;
};

class Serpent
{
public:
    static Serpent* CreateFromPath(const char* filename);
    Serpent();
    ~Serpent();
    Coordinates* getBlocksPos();
};
