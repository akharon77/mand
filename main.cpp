#include "mand.h"
#include "view.h"

int main()
{
    MandConfig conf = {};
    MandConfigCtor(&conf, WIDTH, HEIGHT);

    MandRun(&conf);

    MandConfigDtor(&conf);
    return 0;
}
