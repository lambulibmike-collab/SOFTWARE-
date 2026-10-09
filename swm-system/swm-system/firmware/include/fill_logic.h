#pragma once
#include <algorithm>
namespace swm {
constexpr float MIN_RANGE_CM=20.0f, MAX_RANGE_CM=600.0f, CRITICAL_PCT=80.0f;
constexpr int BUFFER_MAX=50;
inline float pulseToCm(unsigned long us){return us==0?-1.0f:us*0.0343f/2.0f;}
inline bool validRange(float d){return d>=MIN_RANGE_CM&&d<=MAX_RANGE_CM;}
inline float medianOf(float* s,int n){if(n<=0)return -1.0f;std::sort(s,s+n);return n%2?s[n/2]:(s[n/2-1]+s[n/2])/2.0f;}
inline float fillPercent(float d,float h){if(h<=MIN_RANGE_CM||d<0)return -1.0f;float p=(h-d)/(h-MIN_RANGE_CM)*100.0f;return p<0?0:(p>100?100:p);}
inline bool isCritical(float p,float t=CRITICAL_PCT){return p>=t;}
inline bool bufferHasRoom(int stored){return stored<BUFFER_MAX;}
}
