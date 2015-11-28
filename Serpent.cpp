
#include "Serpent.h"
#include "Block.h"
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
    Serpent* serpent = new Serpent();
    Block* curBlock = serpent->getHead();

    while (pathStream.good() == true)
    {
        std::getline(pathStream, curLine);

        if (curLine.length() < 1)
        {
            continue;
        }

        switch (curLine[0])
        {
            case 'u': curBlock = curBlock->addNext(Block::DIR_UP); break;
            case 'd': curBlock = curBlock->addNext(Block::DIR_DOWN); break;
            case 'f': curBlock = curBlock->addNext(Block::DIR_FORWARD); break;
            case 'b': curBlock = curBlock->addNext(Block::DIR_BACKWARD); break;
            case 'r': curBlock = curBlock->addNext(Block::DIR_RIGHT); break;
            case 'l': curBlock = curBlock->addNext(Block::DIR_LEFT); break;
            default: break;
        };
    }

    pathStream.close();

    return serpent;
}


Serpent::Serpent() :
    myHead(new Block())
{
}

Serpent::~Serpent()
{
    delete myHead;
}

Block*
Serpent::getHead() const
{
    return myHead;
}

void
Serpent::getTailPos(
        ssize_t& xPos,
        ssize_t& yPos,
        ssize_t& zPos
        ) const
{
    Block* curBlock = getHead();
    xPos = 1;
    yPos = 1;
    zPos = 1;

    while (curBlock != NULL)
    {
        Block::Direction nextDir;
        curBlock = curBlock->getNext(nextDir);

        switch (nextDir)
        {
            case Block::DIR_UP: ++zPos; break;
            case Block::DIR_DOWN: --zPos; break;
            case Block::DIR_FORWARD: ++xPos; break;
            case Block::DIR_BACKWARD: --xPos; break;
            case Block::DIR_RIGHT: --yPos; break;
            case Block::DIR_LEFT: ++yPos; break;
            default: break;
        };
    }
}

