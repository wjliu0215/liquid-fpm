//
//
//

#include <stdio.h>
#include <stdlib.h>

#include "liquidfpm.internal.h"

// 
//#define LIQUIDFPM_q32_log2_poly_p2  0x857cabee  // -0.342937403262120
//#define LIQUIDFPM_q32_log2_poly_p1  0x207603ca  //  2.028812209786359
//#define LIQUIDFPM_q32_log2_poly_p0  0x9af957dc  // -1.685874806524239

#define LIQUIDFPM_q32_log2_poly_p2  q32_float_to_fixed( -0.342937403262120 )
#define LIQUIDFPM_q32_log2_poly_p1  q32_float_to_fixed(  2.028812209786359 )
#define LIQUIDFPM_q32_log2_poly_p0  q32_float_to_fixed( -1.685874806524239 )

//
// use second-order polynomial fit
//
q32_t log2_q32( q32_t _x )
{
    if (_x <= 0) {
        perror("error: log2_q32(), x must be > 0");
        return 0;
    }

    // base index
    int b = msb_index(_x) - 1;

    // compute fractional portion
    q32_t x = _x ^ (1<<b);
    q32_t f = (b > q32_fracbits) ? x >> (b-q32_fracbits) : x << (q32_fracbits-b);
    f += 1<<q32_fracbits;

    printf("    f = %12.8f\n", q32_fixed_to_float(f));
    q32_t x2 = q32_mul(f,f);
    printf("    x2= %12.8f\n", q32_fixed_to_float(x2));

    q32_t v0    = LIQUIDFPM_q32_log2_poly_p0;
    q32_t v1    = q32_mul(LIQUIDFPM_q32_log2_poly_p1, f);
    q32_t v2    = q32_mul(LIQUIDFPM_q32_log2_poly_p2, x2);
    printf("    v0 = %12.8f\n", q32_fixed_to_float(v0));
    printf("    v1 = %12.8f\n", q32_fixed_to_float(v1));
    printf("    v2 = %12.8f\n", q32_fixed_to_float(v2));

    q32_t polyval = v0 + v1 + v2;
    printf("    p = %12.8f\n", q32_fixed_to_float(polyval));

    printf("    log2(%f) = %d + %12.10f\n", q32_fixed_to_float(_x), b-q32_fracbits, q32_fixed_to_float(polyval));

    b -= q32_fracbits;
    return polyval + ((b<0) ? -(abs(b)<<q32_fracbits) : (b<<q32_fracbits) );
#if 0
    // apply 8-bit mask
    f &= 0x00ff;

    // normalize by decimal position
    b -= q32_fracbits;

    q32_t frac = q32_log2_fraction_table[f];
    printf("    log2(%f) = %d + %12.10f\n", q32_fixed_to_float(_x), b, q32_fixed_to_float(frac));

    if (b >= 0 ) {
        // resulting log will be positive
        return (b << q32_fracbits) + frac;
    } else {
        // resulting log will be negative
        return -(abs(b) << q32_fracbits) + frac;
    }

    return 0;
#endif
}
