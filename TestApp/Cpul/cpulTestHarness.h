#pragma once

#include "Cpul/cpulDefines.h"

#ifdef USE_VS_TEST

    #include "Cpul/cpulVsTest.h"

#else

    #include "Cpul/cpulTest.h"
    #include "Cpul/cpulTestResult.h"
    #include "Cpul/cpulFailure.h"
    #include "Cpul/cpulTestRegistry.h"

#endif

