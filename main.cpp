#include "view.h"

int main()
{
    MandConfig conf = {};
    MandConfigCtor(&conf);

    MandRun(&conf);

    MandConfigDtor(&conf);
    return 0;
}
