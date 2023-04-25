#include <omp.h>
#include <math.h>
#include <stdio.h>

// simulation parameters
const float RYEGRASS_GROWS_PER_MONTH = 20.0;
const float ONE_RABBITS_EATS_PER_MONTH = 1.0;

const float AVG_PRECIP_PER_MONTH = 12.0; // average
const float AMP_PRECIP_PER_MONTH = 4.0;  // plus or minus
const float RANDOM_PRECIP = 2.0;         // plus or minus noise

const float AVG_TEMP = 60.0;    // average
const float AMP_TEMP = 20.0;    // plus or minus
const float RANDOM_TEMP = 10.0; // plus or minus noise

const float MIDTEMP = 60.0;
const float MIDPRECIP = 14.0;

// number of threads
const int NUM_THREADS = 3;

// global variables
int NowYear;  // 2023 - 2028
int NowMonth; // 0 - 11

float NowPrecip;   // inches of rain per month
float NowTemp;     // temperature this month
float NowHeight;   // rye grass height in inches
int NowNumRabbits; // number of rabbits in the current population

unsigned int seed = 0; // random seed

float Sqr(float x)
{
  return x * x;
}

float Ranf(unsigned int *seedp, float low, float high)
{
  float r = (float)rand_r(seedp); // 0 - RAND_MAX

  return (low + r * (high - low) / (float)RAND_MAX);
}

void Rabbits()
{
  while (NowYear < 2029)
  {
    // rabbit population
    int nextNumRabbits = NowNumRabbits;
    int carryingCapacity = (int)(NowHeight);
    if (nextNumRabbits < carryingCapacity)
      nextNumRabbits++;
    else if (nextNumRabbits > carryingCapacity)
      nextNumRabbits--;

    // ensure rabbit population is not negative
    if (nextNumRabbits < 0)
      nextNumRabbits = 0;

    printf("done computing rabbits thread\n");
// done computing barrier
#pragma omp barrier
    // copy the next state into the now variables
    NowNumRabbits = nextNumRabbits;
    printf("done assigning rabbits thread\n");

// done assigning variables barrier
#pragma omp barrier
    printf("%d,%d,%f,%f,%f,%d\n", NowYear, NowMonth, NowPrecip, NowTemp, NowHeight, NowNumRabbits);
    printf("done printing rabbits thread\n");

// done printing barrier
#pragma omp barrier
    printf("done with rabbits thread\n");
  }
}

void RyeGrass()
{
  while (NowYear < 2029)
  {
    // temp and precip factors
    float tempFactor = exp(-Sqr((NowTemp - MIDTEMP) / 10.));
    float precipFactor = exp(-Sqr((NowPrecip - MIDPRECIP) / 10.));

    // grass height
    float nextHeight = NowHeight;
    nextHeight += tempFactor * precipFactor * RYEGRASS_GROWS_PER_MONTH;
    nextHeight -= (float)NowNumRabbits * ONE_RABBITS_EATS_PER_MONTH;

    // ensure grass height is not negative
    if (nextHeight < 0.)
      nextHeight = 0.;

// done computing barrier
#pragma omp barrier
    // copy the next state into the now variables
    NowHeight = nextHeight;
    printf("done assigning rye grass thread\n");

// done assigning variables barrier
#pragma omp barrier
    printf("%d,%d,%f,%f,%f,%d\n", NowYear, NowMonth, NowPrecip, NowTemp, NowHeight, NowNumRabbits);
    printf("done printing rye grass thread\n");

// done printing barrier
#pragma omp barrier
    printf("done with rye grass thread\n");
  }
}

void Watcher()
{
  while (NowYear < 2029)
  {
    // print initial state
    printf("%d,%d,%f,%f,%f,%d\n", NowYear, NowMonth, NowPrecip, NowTemp, NowHeight, NowNumRabbits);

// done computing barrier
#pragma omp barrier
    printf("done printing initial state\n");

// done assigning variables barrier
#pragma omp barrier
    // print results from computations of other threads

    // increment month and year
    NowMonth++;
    if (NowMonth > 11)
    {
      NowMonth = 0;
      NowYear++;
    }

    // calculate new environment parameters
    float ang = (30. * (float)NowMonth + 15.) * (M_PI / 180.);

    float temp = AVG_TEMP - AMP_TEMP * cos(ang);
    NowTemp = temp + Ranf(&seed, -RANDOM_TEMP, RANDOM_TEMP);

    float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin(ang);
    NowPrecip = precip + Ranf(&seed, -RANDOM_PRECIP, RANDOM_PRECIP);
    if (NowPrecip < 0.)
      NowPrecip = 0.;

// done printing barrier
#pragma omp barrier
    printf("done with watcher thread\n");
  }
}

int main()
{
  // starting date and time
  NowMonth = 0;
  NowYear = 2023;

  // starting state
  NowNumRabbits = 1;
  NowHeight = 5.;

  omp_set_num_threads(NUM_THREADS);

#pragma omp parallel sections
  {
#pragma omp section
    {
      Rabbits();
    }

#pragma omp section
    {
      RyeGrass();
    }

#pragma omp section
    {
      Watcher();
    }

    // #pragma omp section
    //     {
    //       // Agent();
    //     }
  }

  return 0;
}
