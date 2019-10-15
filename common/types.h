/*
 * types.h
 *
 * Created: 9/27/2019 11:44:32 AM
 *  Author: juneh
 */


#ifndef TYPES_H_
#define TYPES_H_

#include <cstdint>

/**
 * Basic Type Aliases
 * ([*u|s])([f|t|*e])var([*8|16|32|64])
 * u|s          unsigned/signed, not provided defaults to unsigned
 * f|l|e        fastest/lest/exact, not provided defaults to fastest
 * 8|16|32|64   size requested or demanded, not provided defaults to 8-bit
*/

// Signed Exact/Fastest/Lest Sizing

// Signed Exact Sizing
using sevar8 = int8_t;
using sevar16 = int16_t;
using sevar32 = int32_t;
using sevar64 = int64_t;

// Signed Fastest Sizing
using sfvar8 = int_fast8_t;
using sfvar16 = int_fast16_t;
using sfvar32 = int_fast32_t;
using sfvar64 = int_fast64_t;

// Signed Tiniest Sizing
using stvar8 = int_least8_t;
using stvar16 = int_least16_t;
using stvar32 = int_least32_t;
using stvar64 = int_least64_t;

// Unsigned Exact/Fastest/Lest Sizing

// Signed Exact Sizing
using uevar8 = uint8_t;
using uevar16 = uint16_t;
using uevar32 = uint32_t;
using uevar64 = uint64_t;

// Signed Fastest Sizing
using ufvar8 = uint_fast8_t;
using ufvar16 = uint_fast16_t;
using ufvar32 = uint_fast32_t;
using ufvar64 = uint_fast64_t;

// Signed Tiniest Sizing
using utvar8 = uint_least8_t;
using utvar16 = uint_least16_t;
using utvar32 = uint_least32_t;
using utvar64 = uint_least64_t;

// Defaults

// Default to exact
using svar8 = sevar8;
using svar16 = sevar16;
using svar32 = sevar32;
using svar64 = sevar64;

using uvar8 = uevar8;
using uvar16 = uevar16;
using uvar32 = uevar32;
using uvar64 = uevar64;

// Default to unsigned
using fvar8 = ufvar8;
using fvar16 = ufvar16;
using fvar32 = ufvar32;
using fvar64 = ufvar64;

using tvar8 = utvar8;
using tvar16 = utvar16;
using tvar32 = utvar32;
using tvar64 = utvar64;

using evar8 = uevar8;
using evar16 = uevar16;
using evar32 = uevar32;
using evar64 = uevar64;

using var8 = uvar8;
using var16 = uvar16;
using var32 = uvar32;
using var64 = uvar64;

#endif /* TYPES_H_ */
