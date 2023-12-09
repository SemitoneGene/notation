// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.

#include "Constants.h"

namespace komp
{
          
    // Where all 12 octaves sit relative to YPos == 0
    int	OctavePositionTable[] =
    {
        -BASE_YPOS +  0,
        -BASE_YPOS +  7,
        -BASE_YPOS + 14,
        -BASE_YPOS + 21,
        -BASE_YPOS + 28,
        -BASE_YPOS + 35,
        -BASE_YPOS + 42,
        -BASE_YPOS + 49,
        -BASE_YPOS + 56,
        -BASE_YPOS + 63,
        -BASE_YPOS + 70,
        -BASE_YPOS + 77
    };
    
    /*
     The ChordChart gives ASCII IDs for the given chords in all twelve
     keys. The blank character is B225. Entries in FretsA are combined
     with FA, FretsB with FB, and FretsC with FC.
     
     Numbers in brackets in comments are the new and old indices,
     respectively.
     
     TODO: Build ChordSuper into a structure with this
     */
    
    //// Bits for various fret fonts
#define FA					0
#define FB					256
#define FC					512
    
    int ChordChart[64][12] =
    {
        /* major        [0] [0] */
        FA +  67, FA +  81, FA +  68, FA +  82, FA +  69, FA +  70,
        FA +  83, FA +  71, FA +  84, FA +  65, FA +  85, FA +  66,
        
        /* minor        [1] [2] */
        FA +  99, FA + 113, FA + 100, FA + 114, FA + 101, FA + 102,
        FA + 115, FA + 103, FA + 116, FA +  97, FA + 117, FA +  98,
        
        /* +            [2] [20] */
        FA +  60, FA +  61, FA +  62, FA +  63, FA +  72, FA +  61,
        FA +  62, FA +  63, FA +  76, FA +  77, FA +  62, FA +  63,
        
        /* dim          [3] [19] */
        FA + 165, FA + 166, FA + 170, FA + 173, FA + 174, FA + 175,
        FA + 176, FA + 177, FA + 178, FA + 179, FA + 180, FA + 181,
        
        /* dim7         [4] [32] */
        FA +  73, FA +  74, FA +  75, FA +  73, FA +  74, FA +  78,
        FA +  73, FA +  74, FA +  78, FA +  79, FA +  74, FA +  78,
        
        /* 5            [5] [63] */
        FA + 224, FA + 225, FA + 226, FA + 227, FA + 231, FA + 233,
        FA + 241, FA + 243, FA + 244, FA + 250, FA + 251, FA + 161,
        
        /* 6            [6] [6] */
        FA +  33, FA +  34, FA +  35, FA +  36, FA +  37, FA +  38,
        FA +  39, FA +  40, FA +  41, FA +  42, FA + 169, FC +  59,
        
        /* 6/9          [7] [15] */
        FA + 142, FA + 143, FA + 144, FA + 145, FA + 146, FA + 147,
        FA + 148, FA + 142, FA + 143, FA + 149, FA + 150, FA + 151,
        
        /* (add2)       [8] [30] */
        FA + 183, FA + 184, FA + 185, FA + 187, FA + 188, FA + 189,
        FA + 190, FA + 191, FA + 192, FA + 193, FA + 194, FA + 195,
        
        /* (add9)       [9] [31] */
        FA + 183, FA + 184, FA + 185, FA + 187, FA + 188, FA + 189,
        FA + 190, FA + 191, FA + 192, FA + 193, FA + 194, FA + 195,
        
        /* (omit3)     [10] [48] */
        FA + 224, FA + 225, FA + 226, FA + 227, FA + 231, FA + 233,
        FA + 241, FA + 243, FA + 244, FA + 250, FA + 251, FA + 161,
        
        /* (omit5)     [11] [47] */
        FB +  33, FB +  34, FB +  35, FB +  36, FB +  37, FB +  38,
        FB +  39, FB +  40, FB +  41, FB +  42, FB +  43, FB +  44,
        
        /* maj7        [12] [10] */
        FA +  48, FA +  49, FA +  50, FA +  51, FA +  52, FA +  53,
        FA +  54, FA +  55, FA +  56, FA +  57, FA +  58, FA +  59,
        
        /* maj7(b5)    [13] [60] */
        FB +  82, FB +  83, FB +  84, FB +  85, FB +  86, FB +  87,
        FB +  88, FB +  89, FB +  90, FB +  91, FB +  92, FB +  93,
        
        /* maj7(6/9)     [14] [54] */
        FC +  41, FC +  42, FC +  43, FC +  44, FC +  45, FC +  46,
        FC +  47, FC +  49, FC +  50, FC +  51, FC +  52, FC +  58,
        
        /* maj7(+5)     [15] [29] */
        FB +  94, FB +  95, FB +  96, FB +  97, FB +  98, FB +  99,
        FB + 100, FB + 101, FB + 102, FB + 103, FB + 104, FB + 105,
        
        /* maj7(#11)    [16] [53] */
        FB +  82, FB +  83, FB +  84, FB +  85, FB +  86, FB +  87,
        FB +  88, FB +  89, FB +  90, FB +  91, FB +  92, FB +  93,
        
        /* maj9         [17] [11] */
        FB +  57, FB +  58, FB +  59, FB +  60, FB +  61, FB +  62,
        FB +  63, FB +  65, FB +  66, FB +  67, FB +  68, FB +  69,
        
        /* maj9(b5)     [18] [59] */
        FB + 106, FB + 107, FB + 108, FB + 109, FB + 110, FB + 111,
        FB + 112, FB + 113, FB + 114, FB + 115, FB + 116, FB + 117,
        
        /* maj9(+5)     [19] [58] */
        FC +  75, FC +  76, FC +  77, FC +  79, FB + 191, FB + 192,
        FB + 193, FC +  94, FC +  80, FC +  81, FC +  82, FC +  88,
        
        /* maj9(#11)    [20] [39] */
        FB + 106, FB + 107, FB + 108, FB + 109, FB + 110, FB + 111,
        FB + 112, FB + 113, FB + 114, FB + 115, FB + 116, FB + 117,
        
        /* maj13        [21] [46] */
        FB + 118, FB + 119, FB + 120, FB + 121, FB + 122, FB + 123,
        FB + 124, FB + 125, FB + 126, FB + 128, FB + 129, FB + 130,
        
        /* maj13(b5)    [22] [55] */
        FB +  82, FB +  83, FB +  84, FB +  85, FB +  86, FB +  87,
        FB +  88, FB +  89, FB +  90, FB +  91, FB +  92, FB +  93,
        
        /* maj13(#11)   [23] [42] */
        FB +  82, FB +  83, FB +  84, FB +  85, FB +  86, FB +  87,
        FB +  88, FB +  89, FB +  90, FB +  91, FB +  92, FB +  93,
        
        /* 7            [24] [4] */
        FA + 130, FA + 206, FA + 235, FA + 229, FA + 228, FA + 236,
        FA + 234, FA + 237, FA + 230, FA + 129, FA + 232, FA + 245,
        
        /* 7(b5)        [25] [13] */
        FB +  70, FB +  71, FB +  72, FB +  73, FB +  74, FB +  75,
        FB +  70, FB +  71, FB +  72, FB +  79, FB +  74, FB +  81,
        
        /* 7(b9)        [26] [14] */
        FA +  74, FA +  75, FB + 169, FB + 170, FA +  78, FA +  73,
        FA +  74, FA +  75, FA +  73, FB + 174, FA +  75, FA +  73,
        
        /* 7(#9)        [27] [33] */
        FB + 175, FB + 176, FB + 177, FB + 178, FC +  89, FC +  90,
        FB + 181, FB + 182, FB + 183, FB + 184, FB + 185, FB + 186,
        
        /* 7(#11)       [28] [34] */
        FB + 187, FB + 188, FB + 189, FB + 190, FB + 187, FB + 188,
        FB + 191, FB + 192, FB + 193, FB + 194, FB + 195, FB + 190,
        
        /* 7(b5,b9)     [29] [35] */
        FA + 131, FB + 198, FB + 199, FB + 200, FB + 201, FB + 203,
        FB + 204, FB + 205, FB + 206, FB + 207, FB + 208, FB + 209,
        
        /* 7(b5,#9)     [30] [36] */
        FB + 210, FB + 211, FB + 212, FB + 213, FB + 214, FB + 215,
        FB + 216, FB + 217, FB + 218, FB + 219, FB + 220, FB + 221,
        
        /* 7(b9,#9)     [31] [56] */
        FA + 130, FA + 206, FA + 235, FA + 229, FA + 228, FA + 236,
        FA + 234, FA + 237, FA + 230, FA + 129, FA + 232, FA + 245,
        
        /* 9            [32] [8] */
        FA + 197, FA + 198, FA + 199, FA + 200, FA + 201, FA + 203,
        FA + 204, FA + 205, FA + 208, FA + 209, FA + 210, FA + 211,
        
        /* 9(b5)        [33] [12] */
        FB + 187, FB + 188, FB + 189, FB + 190, FB + 187, FB + 188,
        FB + 191, FB + 192, FB + 193, FB + 194, FB + 195, FB + 190,
        
        /* 9(#11)       [34] [17] */
        FB + 187, FB + 188, FB + 189, FB + 190, FB + 187, FB + 188,
        FB + 191, FB + 192, FB + 193, FB + 194, FB + 195, FB + 190,
        
        /* 11           [35] [24] */
        FB +  45, FB +  46, FB +  47, FB +  48, FB +  49, FB +  50,
        FB +  51, FB +  52, FB +  53, FB +  54, FB +  55, FB +  56,
        
        /* 13           [36] [26] */
        FB + 232, FB + 233, FB + 234, FB + 235, FB + 244, FB + 245,
        FB + 246, FB + 247, FB + 248, FB + 249, FB + 250, FB + 231,
        
        /* 13(b5)       [37] [50] */
        FB + 104, FB + 105, FC +  91, FC +  92, FB + 173, FB + 179,
        FB + 180, FB + 196, FB + 197, FB + 222, FB + 223, FB + 224,
        
        /* 13(b9)       [38] [27] */
        FB +  80, FB + 131, FB + 132, FB + 133, FB + 134, FB + 155,
        FB + 156, FB + 157, FB + 158, FB + 159, FB + 252, FB + 162,
        
        /* 13(#9)       [39] [44] */
        FB + 163, FB + 165, FB + 166, FB + 167, FB + 168, FB + 171,
        FB + 163, FB + 165, FB + 166, FB + 167, FB + 168, FB + 171,
        
        /* 13(#11)      [40] [45] */
        FB + 104, FB + 105, FC +  91, FC +  92, FB + 173, FB + 179,
        FB + 180, FB + 196, FB + 197, FB + 222, FB + 223, FB + 224,
        
        /* +7           [41] [21] */
        FB + 189, FB + 190, FB + 187, FB + 188, FB + 189, FB + 190,
        FB + 187, FB + 188, FB + 189, FB + 190, FB + 187, FB + 188,
        
        /* +7(b9)       [42] [23] */
        FA + 137, FA + 138, FA + 139, FA + 214, FA + 215, FA + 108,
        FA + 109, FA + 110, FA + 111, FA + 112, FA + 216, FC +  60,
        
        /* +7(#9)       [43] [41] */
        FB + 226, FB + 227, FB + 228, FB + 229, FB + 230, FB + 231,
        FB + 232, FB + 233, FB + 234, FB + 235, FB + 236, FB + 237,
        
        /* +9		   [44] [22] */
        FB + 238, FB + 239, FB + 240, FB + 241, FB + 242, FB + 243,
        FB + 238, FB + 239, FB + 240, FB + 241, FB + 242, FB + 243,
        
        /* sus2		 [45] [61] */
        FC +  48, FA + 202, FA + 253, FC +  53, FC +  61, FC +  62,
        FC +  63, FC +  64, FC +  65, FC +  66, FC +  67, FC +  68,
        
        /* sus2,sus4    [46] [62] */
        FC +  78, FA + 254, FA + 255, FC +  83, FC + 95,  FC +  96,
        FC +  97, FB +  64, FB + 202, FB + 253, FB + 254, FB + 255,
        
        /* sus4		 [47] [1] */
        FA + 152, FA + 153, FA + 154, FA + 155, FA + 156, FA + 157,
        FA + 158, FA + 159, FA + 252, FA + 162, FA + 163, FA + 164,
        
        /* 7sus4		[48] [5] */
        FA + 122, FA + 123, FA + 124, FA + 125, FA + 126, FA + 128,
        FA + 131, FA + 132, FA + 133, FA + 134, FA + 135, FA + 136,
        
        /* 9sus4		[49] [52] */
        FB +  45, FB +  46, FB +  47, FB +  48, FB +  49, FB +  50,
        FB +  51, FB +  52, FB +  53, FB +  54, FB +  55, FB +  56,
        
        /* 13sus4       [50] [43] */
        FC +  84, FC +  85, FC +  86, FC +  87, FB + 251, FB +  60,
        FB +  61, FB +  62, FB +  63, FB +  76, FB +  77, FB +  78,
        
        /* m(add2)      [51] [51] */
        FA + 248, FA + 249, FC +  55, FC +  70, FC +  71, FB + 126,
        FC +  93, FB + 129, FB + 130, FC +  72, FC +  73, FC +  74,
        
        /* m(add9)      [52] [57] */
        FA + 248, FA + 249, FC +  55, FC +  70, FC +  71, FB + 126,
        FC +  93, FB + 129, FB + 130, FC +  72, FC +  73, FC +  74,
        
        /* m6		   [53] [7] */ 
        FA +  43, FA +  44, FA +  45, FA + 105, FA +  47, FA +  80,     
        FA +  86, FA +  87, FA +  88, FA +  89, FA +  90, FA +  91,
        
        /* m(6/9)       [54] [16] */
        FC +  54, FC +  56, FC +  57, FA +  46, FA +  64, FC +  40,
        FA + 238, FA + 239, FA + 240, FA + 242, FA + 246, FA + 247,
        
        /* minor 7      [55] [3] */
        FA + 141, FA + 207, FA + 182, FA + 168, FA + 171, FA + 196,     
        FA + 167, FA + 169, FC +  59, FA + 140, FA + 172, FA + 186,
        
        /* m(maj7)      [56] [28] */
        FA +  63, FA +  76, FA +  92, FA +  62, FA +  63, FA +  93, 
        FA +  94, FA +  95, FA +  96, FA + 104, FA +  61, FA +  62,
        
        /* m7(b5)       [57] [18] */
        FA + 105, FA + 106, FA + 107, FA + 108, FA + 109, FA + 110, 
        FA + 111, FA + 112, FA + 118, FA + 119, FA + 120, FA + 121,
        
        /* m7(add4)     [58] [38] */
        FB +  60, FB +  61, FB +  62, FB +  63, FB + 135, FB + 136, 
        FB + 137, FB + 138, FB + 139, FB + 140, FB + 141, FB + 142,
        
        /* m7(add11)    [59] [37] */
        FB +  60, FB +  61, FB +  62, FB +  63, FB + 135, FB + 136, 
        FB + 137, FB + 138, FB + 139, FB + 140, FB + 141, FB + 142,
        
        /* m9		   [60] [9] */
        FA + 212, FA + 213, FA +  53, FA +  54, FA +  55, FA + 217,     
        FA + 218, FA + 219, FA + 220, FA + 221, FA + 222, FA + 223,
        
        /* m9(maj7)     [61] [40] */
        FB +  97, FB +  98, FB +  99, FB + 100, FB + 101, FB + 102, 
        FB + 161, FB + 104, FB + 105, FB + 164, FB +  95, FB +  96,
        
        /* m11		  [62] [25] */
        FB +  60, FB +  61, FB +  62, FB +  63, FB + 135, FB + 136, 
        FB + 137, FB + 138, FB + 139, FB + 140, FB + 141, FB + 142,
        
        /* m13		  [63] [49] */
        FB + 143, FB + 144, FB + 145, FB + 146, FB + 147, FB + 148, 
        FB + 149, FB + 150, FB + 151, FB + 152, FB + 153, FB + 154,
    }; 
    
    // Default voice for each hand
    int DefaultVoice[MAX_HANDS] =
    {
        0,
        4,
        6,
        7
    };
    
} // namespace

// Semitone Inc. Confidential Information.
// TM and (c) 2016-present Semitone Inc.  All Rights Reserved.
// Reproduction in whole or in part without prior written permission of a
// duly authorized representative is prohibited.
