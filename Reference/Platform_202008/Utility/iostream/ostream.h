#pragma once
#include "01Utility/string/string.h"

namespace nibble {
    class ostream
    {
        public:
            ostream()
            {
            }
            ostream& operator << (const int& rInt)
            {
                printf("%d", rInt);
                return *this;
            }
            ostream& operator << (const char* rCharArray)
            {
                printf("%s", rCharArray);
                return *this;
            }
            ostream& operator << (const string &rString)
            {
                printf("%s", rString.c_str());
                return *this;
            }
    };
}