#pragma once
#include "AssertionHelper.h"

#define TESTIMG_NR	7

#define STOREDATASET 0
#define DATASET_PATH "./_dataset"

#define USE_PRESET_ROI  0

#if USE_PRESET_ROI == 1
#if TESTIMG_NR == 1
#define PRESET_ROI {314, 375,457, 397, 461, 584, 318, 595};
#elif TESTIMG_NR == 2
#define PRESET_ROI {43, 350, 473, 355, 488, 748, 37, 770};
#elif TESTIMG_NR == 3
#define PRESET_ROI {246, 397,526, 364,655, 624,338, 704};
#elif TESTIMG_NR == 4
#define PRESET_ROI {268, 450, 562, 422, 715, 660, 374, 744};
#elif TESTIMG_NR == 5
#define PRESET_ROI {178, 378, 488, 381, 545, 593, 152, 602};
#endif
#endif
