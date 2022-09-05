#include "time.h"

time_t time() {
    time_t t;
    __time(&t);
    return t;
}
