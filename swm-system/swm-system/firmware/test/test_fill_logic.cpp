#include <cassert>
#include "../include/fill_logic.h"
int main(){assert(swm::pulseToCm(0)==-1);assert(swm::validRange(20));assert(!swm::validRange(10));float a[]={80,82,300,81,83};assert(swm::medianOf(a,5)==82);assert(swm::fillPercent(60,100)>49&&swm::fillPercent(60,100)<51);assert(swm::isCritical(80));assert(swm::bufferHasRoom(49));assert(!swm::bufferHasRoom(50));}
