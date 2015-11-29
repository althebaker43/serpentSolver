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

        void getDimensions(
                size_t& xSize,
                size_t& ySize,
                size_t& zSize
                ) const;

        bool check();

    private:

        static ssize_t GetMax(
                ssize_t val1,
                ssize_t val2
                );

        static ssize_t GetMin(
                ssize_t val1,
                ssize_t val2
                );

        static size_t GetLength(
                ssize_t hiBound,
                ssize_t loBound
                );

        Serpent();

        size_t getNumBlocks() const;

        void initializeSpace();

        void deleteSpace();

        size_t getSpace(
                ssize_t xPos,
                ssize_t yPos,
                ssize_t zPos
                ) const;

        void setSpace(
                ssize_t xPos,
                ssize_t yPos,
                ssize_t zPos,
                size_t val
                );

        void clearSpace();

        Block* myHead;

        size_t*** mySpace;

        size_t mySpaceRadius;
};

#endif
