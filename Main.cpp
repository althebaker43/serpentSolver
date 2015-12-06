
#include "Serpent.h"
#include "Block.h"
#include <iostream>
#include <fstream>


static void DumpDimensions(
        std::ostream&   outputStream,
        Serpent*        serpent
        );

static bool IsSerpentCube(
        Serpent*    serpent
        );

int
main(
        int     argc,
        char*   argv[]
    )
{
    if (argc < 2)
    {
        return 1;
    }

    std::cout << "Reading from " << argv[1] << std::endl;

    Serpent* serpent = Serpent::CreateFromPath(argv[1]);

    std::ofstream dimStream("dim.csv");

    size_t stepCount = 0;

    std::cout << "Starting compression." << std::endl;

    while (true)
    {
        DumpDimensions(dimStream, serpent);

        if (stepCount > 1000)
        {
            std::cout << "Number of steps exceeded 1000." << std::endl;
            break;
        }
        stepCount++;

        if (serpent->compress() == false)
        {
            std::cout << "Compression failed." << std::endl;
            break;
        }

        if (IsSerpentCube(serpent) == true)
        {
            std::cout << "Cube achieved." << std::endl;
            break;
        }
    }

    DumpDimensions(dimStream, serpent);

    dimStream.close();

    delete serpent;

    return 0;
}

void
DumpDimensions(
        std::ostream&   outputStream,
        Serpent*        serpent
        )
{
    size_t sizeX = 0;
    size_t sizeY = 0;
    size_t sizeZ = 0;

    serpent->getDimensions(sizeX, sizeY, sizeZ);

    outputStream
        << sizeX << ' '
        << sizeY << ' '
        << sizeZ << ' '
        << std::endl;
}

bool
IsSerpentCube(
        Serpent*    serpent
        )
{
    size_t sizeX = 0;
    size_t sizeY = 0;
    size_t sizeZ = 0;

    serpent->getDimensions(sizeX, sizeY, sizeZ);

    if(
            (sizeX == 3) &&
            (sizeY == 3) &&
            (sizeZ == 3)
      )
    {
        return true;
    }
    else
    {
        return false;
    }
}

