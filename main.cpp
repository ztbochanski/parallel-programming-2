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

// global variables
int NowYear;  // 2023 - 2028
int NowMonth; // 0 - 11
int MonthNumber;

float NowPrecip;   // inches of rain per month
float NowTemp;     // temperature this month
float NowHeight;   // rye grass height in inches
int NowNumRabbits; // number of rabbits in the current population
bool NowMonkey;    // is the monkey awake?
bool user_agent_mode = true;

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

// done computing barrier
#pragma omp barrier
    // copy the next state into the now variables
    NowNumRabbits = nextNumRabbits;

// done assigning variables barrier
#pragma omp barrier

// done printing barrier
#pragma omp barrier
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

// done assigning variables barrier
#pragma omp barrier

// done printing barrier
#pragma omp barrier
  }
}

void Watcher()
{
  while (NowYear < 2029)
  {

// done computing barrier
#pragma omp barrier

// done assigning variables barrier
#pragma omp barrier
    // print results from computations of other threads

    int graphMonkeyRepresentation = 1;
    if (user_agent_mode)
    {
      if (NowMonkey == 1)
      {
        graphMonkeyRepresentation = 10;
      }
      fprintf(stdout, "%d, %d, %f,%f,%f,%d, %d\n", NowYear, MonthNumber + 1, NowPrecip, NowTemp, NowHeight, NowNumRabbits, graphMonkeyRepresentation);
    }
    else
    {
      fprintf(stdout, "%d, %d, %f,%f,%f,%d\n", NowYear, MonthNumber + 1, NowPrecip, NowTemp, NowHeight, NowNumRabbits);
    }

    // increment month and year
    NowMonth++;
    MonthNumber++;
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
  }
}

void InstantGratificationMonkey()
{
  while (NowYear < 2029)
  {
    // set the monkey's sleep schedule
    if (NowMonth % 12 == 0 || NowMonth % 12 == 1)
    {
      NowMonkey = true;
    }
    else
    {
      NowMonkey = false;
    }

    // if the monkey is awake it will convince the rabbits that reproducing is better than eating grass, therefore the rabbit population will grow faster
    if (NowMonkey)
    {
      NowNumRabbits += 10;
    }

    // if the monkey is awake it will jump for joy, which is unfortunate for the grass, therefore the grass will grow slower
    if (NowMonkey)
    {
      NowHeight -= 1;
    }

    // if the monkey if awake it will convince the clouds that they should do something else, therefore less precipitation
    if (NowMonkey)
    {
      NowPrecip -= 5;
    }

    // if the monkey is awake it will convince the sun that it should do something else, therefore it gets colder
    if (NowMonkey)
    {
      NowTemp -= 10;
    }

    // done computing barrier
#pragma omp barrier

// done assigning variables barrier
#pragma omp barrier

// done printing barrier
#pragma omp barrier
  }
}

int main()
{
  int num_threads;

  // starting date and time
  NowMonth = 0;
  NowYear = 2023;

  // starting state
  NowNumRabbits = 1;
  NowHeight = 5.;
  NowPrecip = 40.;
  NowTemp = 8.;

  if (user_agent_mode)
  {
    num_threads = 4;
    fprintf(stdout, "Year,Month,Precipitation,Temperature,RyeGrassHeight,NumberRabbits,MonkeyAwake\n");
  }
  else
  {
    num_threads = 3;
    fprintf(stdout, "Year,Month,Precipitation,Temperature,RyeGrassHeight,NumberRabbits\n");
  }
  omp_set_num_threads(num_threads);

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

#pragma omp section
    {
      // https://www.youtube.com/watch?v=arj7oStGLkU
      InstantGratificationMonkey();
    }
  }

  return 0;
}
