#include "il.h"

#define DEFINE_NAME(BaseName) \
    const char* BaseName##Instr::Name(){ \
        return #BaseName; \
    }
FOR_EACH_INSTRUCTION(DEFINE_NAME)
#undef DEFINE_NAME