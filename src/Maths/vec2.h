#ifndef __VEC2_H__
#define __VEC2_H__

//-----------------------------------------------------------
typedef struct vec2
{
    double x, y;
} vec2;

// MACROS
//-----------------------------------------------------------
#define VEC2_INIT_ZERO \
    (vec2) { 0.0, 0.0 }
#define VEC2_INIT_VALUE(VAL) \
    (vec2) { (VAL), (VAL) }

#endif // __VEC2_H__