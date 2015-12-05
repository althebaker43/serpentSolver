#ifndef SERPENT_H
#define SERPENT_H

#include <ios>
#include <stdlib.h>
#include <vector>

class Block;

class Serpent
{
    public:

        enum Axis{
            AXIS_X,
            AXIS_Y,
            AXIS_Z
        };

        typedef std::vector<Axis> Axes;

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

        void getBounds(
                ssize_t& xMin,
                ssize_t& yMin,
                ssize_t& zMin,
                ssize_t& xMax,
                ssize_t& yMax,
                ssize_t& zMax
                ) const;

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

        void getMaxSizeAxes(
                Axes& axes  /**< Set of axes sorted by maximum size */
                );

        Block* myHead;

        size_t*** mySpace;

        size_t mySpaceRadius;
};

#endif
