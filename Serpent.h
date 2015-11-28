#ifndef SERPENT_H
#define SERPENT_H

#include <ios>
#include <stdlib.h>

class Block;

class Serpent
{
    public:

        static Serpent* CreateFromXML(
                const char* filename
                );

        static Serpent* CreateFromPath(
                const char* filename
                );

        ~Serpent();

        Block* getHead() const;

        void getTailPos(
                ssize_t& xPos,
                ssize_t& yPos,
                ssize_t& zPos
                ) const;

    private:

        Serpent();

        Block* myHead;
};

#endif
