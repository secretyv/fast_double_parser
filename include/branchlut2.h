#ifndef BRANCHLUT2_H
#define BRANCHLUT2_H

// https://github.com/fmtlib/format-benchmark/blob/master/src/itoa-benchmark/branchlut2.cpp

#include <stdint.h>

namespace itoa_branchlut2 {

   void itoa(uint32_t, char*);
   void itoa( int32_t, char*);
   void itoa(uint64_t, char*);
   void itoa( int64_t, char*);

}  // namespace itoa_branchlut2

#endif BRANCHLUT2_H