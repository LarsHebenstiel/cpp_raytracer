#ifndef MACROS_H
#define MACROS_H

/*
 * #define USE_FLOAT_AS_DOUBLE in order to use doubles instead of floats
 * Idea taken from pbrt: https://www.pbrt.org
 */
#ifdef USE_FLOAT_AS_DOUBLE
    typedef double Float;
#else
    typedef float Float;
#endif // USE_FLOAT_AS_DOUBLE

#endif