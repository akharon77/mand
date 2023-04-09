#include "mand.h"

int main()
{
    MandConfig conf = {};
    MandConfigCtor(&conf);

    MandRun(&conf);

    MandConfigDtor(&conf);
    return 0;
}
