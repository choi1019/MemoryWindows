#include "Aspect.h"

#define COUNT_TAB 20
#define SIZE_TAB 4
#define SPACE ' '

unsigned Aspect::s_uCountTab = 1;
char Aspect::s_pcTab[COUNT_TAB * SIZE_TAB];

char* Aspect::GetTab() {
    for (unsigned i = 0; i < Aspect::s_uCountTab * SIZE_TAB; i++) {
        Aspect::s_pcTab[i] = SPACE;
    }
    Aspect::s_pcTab[Aspect::s_uCountTab * SIZE_TAB] = '\0';
    return Aspect::s_pcTab;
}
void Aspect::AddTab() { Aspect::s_uCountTab++; }
void Aspect::RemoveTab() { Aspect::s_uCountTab--; }