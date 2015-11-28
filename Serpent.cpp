
#include "Serpent.h"
#include <iostream>
#include <fstream>


Serpent*
Serpent::CreateFromXML(
        const char* filename
        )
{
    return NULL;
}

Serpent*
Serpent::CreateFromPath(
        const char* filename
        )
{
    std::ifstream pathStream(filename);
    std::string curLine;
    ssize_t posX = 1;
    ssize_t posY = 1;
    ssize_t posZ = 1;

    while (pathStream.good() == true)
    {
        std::getline(pathStream, curLine);

        if (curLine.length() < 1)
        {
            continue;
        }

        switch (curLine[0])
        {
            case 'u': ++posZ; break;
            case 'd': --posZ; break;
            case 'f': ++posX; break;
            case 'b': --posX; break;
            case 'r': ++posY; break;
            case 'l': --posY; break;
            default: break;
        };
    }

    pathStream.close();

    std::cout
        << "X:" << posX << ' '
        << "Y:" << posY << ' '
        << "Z:" << posZ << ' '
        << std::endl;

    return NULL;
}
