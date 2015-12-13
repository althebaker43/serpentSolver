
#include "Serpent.h"
#include "Block.h"
#include <iostream>
#include <fstream>


static void DumpDimensions(
        std::ostream&   outputStream,
        Serpent*        serpent
        );

static void DumpCoordinates(
        std::ostream&   outputStream,
        Serpent*        serpent
        );

static void WriteValuesRow(
        std::ostream&               outputStream,
        const std::vector<ssize_t>& vals
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

    std::ofstream posStream("coordinates.txt");
    DumpCoordinates(posStream, serpent);

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

    std::ofstream stepStream("procedure.steps");
    serpent->writeSteps(stepStream);
    stepStream.close();

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

void
DumpCoordinates(
        std::ostream&   outputStream,
        Serpent*        serpent
        )
{
    std::vector<ssize_t> xPos;
    std::vector<ssize_t> yPos;
    std::vector<ssize_t> zPos;

    serpent->getBlocksPos(xPos, yPos, zPos);

    WriteValuesRow(outputStream, xPos);
    WriteValuesRow(outputStream, yPos);
    WriteValuesRow(outputStream, zPos);
}

void
WriteValuesRow(
        std::ostream&               outputStream,
        const std::vector<ssize_t>& vals
        )
{
    bool firstVal = true;

    for(
            std::vector<ssize_t>::const_iterator iter = vals.begin();
            iter != vals.end();
            ++iter
       )
    {
        if (firstVal == false)
        {
            outputStream << ' ';
        }
        else
        {
            firstVal = false;
        }

        outputStream << *iter;
    }

    outputStream << std::endl;
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

