#include "NTimer.h"

static	clock_t		start;
static	clock_t		stop;


void NTimer::Start() {
  start= clock();
} //eof NTimer::Initialize()


void NTimer::Stop() {
  stop= clock();
} //eof NTimer::Check()


float NTimer::Report(bool T) {
  if (T) {
    return((stop-start)*1.0)/CLOCKS_PER_SEC;
  } else {
    printf("Seconds: %f\n",((stop-start)*1.0)/CLOCKS_PER_SEC);
  } //fi
  return(0);
} //eof NTimer::Count()
