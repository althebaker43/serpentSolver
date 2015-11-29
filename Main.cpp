
#include "Serpent.h"
#include "Block.h"
#include <iostream>


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

    Serpent* serpent = Serpent::CreateFromPath(argv[1]);

    size_t sizeX = 0;
    size_t sizeY = 0;
    size_t sizeZ = 0;
    serpent->getDimensions(sizeX, sizeY, sizeZ);
    std::cout
        << "X:" << sizeX << ' '
        << "Y:" << sizeY << ' '
        << "Z:" << sizeZ << ' '
        << std::endl;

    if (serpent->check() == true)
    {
        std::cout << "Check passed." << std::endl;
    }
    else
    {
        std::cout << "Check failed." << std::endl;
    }

    delete serpent;

    return 0;
}

