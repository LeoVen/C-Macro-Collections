// Random functions
#include <stdlib.h>
#include <math.h>

double rrandom()
{
    return (double)rand() / ((double)RAND_MAX + 1.0);
}

int randint(int min, int max)
{
    return (int)floor(rrandom() * ((double)max - (double)min + 1.0) + (double)min);
}

size_t randsize(size_t min, size_t max)
{
    return (size_t)floor(rrandom() * ((double)max - (double)min + 1.0) + (double)min);
}
