#ifndef SERPENT_H
#define SERPENT_H

#include <ios>

class Serpent
{
    public:

        static Serpent* CreateFromXML(
                const char* filename
                );

        static Serpent* CreateFromPath(
                const char* filename
                );

        void writeXML(
                std::ostream& outputStream
                );
};

#endif
