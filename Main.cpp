
#include "Serpent.h"
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

    ssize_t posX = 1;
    ssize_t posY = 1;
    ssize_t posZ = 1;
    serpent->getTailPos(posX, posY, posZ);
    std::cout
        << "X:" << posX << ' '
        << "Y:" << posY << ' '
        << "Z:" << posZ << ' '
        << std::endl;

    delete serpent;

    return 0;
}

