#define GAC_PRIVATE_HEADER
#define GAC_DEBUG
#include "../src/gac.h"
#include <unistd.h>


int main(void)
{
    gac_cfg_t cfg = {0};
    int err = 0;

    GAC_INIT(cfg, err);

    (void) err;

    for (int i = 0; i < 10; i++)
        galloc(50);


    sleep(5);

    gac_print_allocations();
}
