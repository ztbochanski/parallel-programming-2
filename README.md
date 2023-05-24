# parallel-programming-functional-decomposition

### Abstract

This simulation studies the effects of various agents within an ecosystem. It uses a simple ecosystem concept as an exercise to use functional decomposition in practice. This is implemented by forcing 4 different threads to take responsibility for a different system aspect. In this simulation, each thread represents an agent in a simple four-agent system comprised of grass, rabbits, environment, and a fourth, user agent. The user agent in the case of this experiment is the instant gratification monkey from Tim Urban's Ted Talk: https://www.youtube.com/watch?v=arj7oStGLkU

### Simulation 1 -> Regular Ecosystem Observation

First, a system is represented by three simple agents. The system below shows how the number of rabbits changes over a 72-month period in relation to precipitation, temperature, and ryegrass height.

Figure 1. Ecoysytem Observed Without Monkey
![nomonkey 1.svg](https://github.com/ztbochanski/parallel-programming-functional-decomposition/blob/132048a405fda98ebe5ab05c64a72c66f8841125/nomonkey%201.svg)

Data:
![[0230426203635.png]]

### Simulation 2  -> Introduction of Instant Gratification 

Second, a system is represented by introducing a fourth user agent. The system below shows how the number of rabbits changes over a 72-month period in relation to precipitation, temperature, ryegrass height, and one instant gratification monkey.

> Note that the instant gratification monkey is represented by periods of awakeness or sleeping. We can see that it's asleep when its value is 0.

Figure 2. Ecosystem Observed with Instant Gratification Monkey
![monkey](https://github.com/ztbochanski/parallel-programming-functional-decomposition/blob/cc13677c9d4060ffc176591e8e41743941e999cd/monkey%201.svg)

Data:
![[20230426203542.png]]


### Commentary

From studying Tim Urban's Ted Talk we know that instant gratification monkeys are mostly concerned with distracting the agents in their environment from related to said agent. 

Notable instant gratification monkey behavior within this ecosystem are as follows:
1. If the monkey is awake it will convince the rabbits that reproducing is better than eating grass, therefore the rabbit population will grow faster
2. If the monkey is awake it will jump for joy, which is unfortunate for the grass as the monkey jumps on it; therefore the grass will grow slower.
3. If the monkey is awake it will convince the sun that it should do something else, therefore it gets colder

>It's been observed that the monkey sleeps most of the time during the warm months and comes out wide awake in January and February.

The first simulation in Figure 1 shows a stable cycle representing an in-balance system that seems to cycle approximately every year. The population doesn't grow too large and the grass doesn't get too low. When temperature and precipitation increase, the grass and rabbit population follow close suit. Over a five-year period, the state of the ecosystem stays relatively stable.

In the second simulation, when an instant gratification monkey is released into a stable environment the results show that much more variation is introduced into the parameters of the system. Temperature, grass height, and rabbit population are struggling to stay in balance. 

Temperature swings are much larger, the grass gets completely destroyed during certain drought periods when rainfall is lower and the rabbit population is higher than normal. The monkey increases the rabbit population, tramples grass by jumping for joy, and turns down the precipitation and temperature in the region where it's released.

A secondary effect of less precipitation and a lower-than-normal temperature during January and February is larger temperature swings and less grass than normal throughout the rest of the year; where the low grass problem is compounded by the introduction of even more rabbits.
