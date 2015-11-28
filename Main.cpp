
#include "Serpent.h"


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

    Serpent::CreateFromPath(argv[1]);

    return 0;
}

