// C++ program to create target string, starting from
// random string using Genetic Algorithm
#include<vector>
#include <iostream>
#include<string>
#include<cmath>
#include<algorithm>
#include <ctime>
#include "VariadicTable.h"
#include <math.h>       /* ceil */
#include <stdlib.h>
#include<time.h>

using namespace std;

//Change this to false if you want to test minimizing the function
bool isMax = false;
  
// Number of individuals in each generation
#define POPULATION_SIZE 60
#define MAX_GENERATIONS 1000
int generation = 0;

// this is just the allowed scope of w,x,y,z
const int MAX_SIZE = 75;

//Percent of children that have mutation preformed on them.
const double MUTATE_RATE = .15;

//Percent of population that are randomly selected to move on
const double ELITE_PERCENT = 10;

//Only the top X percent are able to reproduce
const double PERCENTILE_TO_REPRODUCE = .35;

//Change scope of mutation from completely random to within range starting at generation
const int FLIP_MUTATION_TYPE = 25;

//This will narrow the scope by X times.
const double NARROW_DOWN_BY = 1.5;

//THis will narrow the mutation scope every x generations
const int NARROW_EVERY = 25;
//This is the divider for narrowing the scope it starts at 1 cause it shouldn't be too narrow to begin with.
double divider = 1;

// Function to generate random numbers in given range
int getRand(int start, int end)
{
    int range = (end-start)+1;
    int random_int = start+(rand()%range);
    return random_int;
}
  
vector<double> createChromosome()
{
    vector<double> newGene;
    for(int i = 0;i<4;i++){
        double f = (rand() / (double)RAND_MAX * MAX_SIZE);
        newGene.push_back(f);
    }
    return newGene;
}
  
class Individual
{
public:
    vector<double> chromosome;
    double fitness;
    int number;
    int parent1 = -1;
    int parent2 =-1;
    Individual(vector<double> chromosome);
    void mutate();
    double computeFitness();
};
  
Individual::Individual(vector<double> chromosome)
{
    this->chromosome = chromosome;
    this->fitness = computeFitness();
};

double getRanddouble(double M, double N)
{   double f = M + (rand() / (RAND_MAX / (N - M)));
    return f;
}

void Individual::mutate(){
    if(generation < FLIP_MUTATION_TYPE){
          double chance = getRand(0,100)/100.0;
          if(chance < MUTATE_RATE){
              chance = getRand(0,100)/100.0;
              if(chance < .25){
                  chromosome.at(0) = (rand() / (double)RAND_MAX * MAX_SIZE);
              }
              else if(chance < .5){
                  chromosome.at(1) = (rand() / (double)RAND_MAX * MAX_SIZE);
              }
              else if(chance < .75){
                  chromosome.at(2) = (rand() / (double)RAND_MAX * MAX_SIZE);
              }
              else{
                  chromosome.at(3) = (rand() / (double)RAND_MAX * MAX_SIZE);
              }
          }
    }

else{
    double chance = getRand(0,100)/100.0;
    double maxMutate;
    double negate = getRand(0,1);
    if(negate == 0){
        negate = 1.0;
    }
    else{
        negate = -1.0;
    }

  if(chance < MUTATE_RATE){
      double randAbout;
      int chrome;
      chance = getRand(0,100)/100.0;
      if(chance < .25){
          chrome = 0;
      }
      else if(chance < .5){
          chrome = 1;
      }
      else if(chance < .75){
          chrome = 2;
      }
      else{
          chrome = 3;
      }
      maxMutate = 75.0 - chromosome.at(chrome);
      if(chromosome.at(chrome) < maxMutate){
          maxMutate = chromosome.at(chrome);
      }
      randAbout = ((getRanddouble(0.0, abs(maxMutate)/divider)*negate));
      double wtf = randAbout + chromosome.at(chrome);

      chromosome.at(chrome) =  wtf;
  }
}
}
vector<vector<double>> crossover(Individual par1, Individual par2)
{
    vector<vector<double>> children;
    vector<double> child;
    vector<double> child2;
    int crossoverPoint = getRand(1, 3);
    int i = 0;
    while(i < crossoverPoint){
        child.push_back(par1.chromosome.at(i));
        i++;
    }
    while(i < 4){
        child.push_back(par2.chromosome.at(i));
        i++;
    }
    i--;
    while(i >= crossoverPoint){
        child2.push_back(par1.chromosome.at(i));
        i--;
    }
    while(i >= 0){
        child2.push_back(par2.chromosome.at(i));
        i--;
    }
    children.push_back(child);
    children.push_back(child2);
    return children;
};
  
double Individual::computeFitness()
{
    
    double omega = 2*M_PI;
    double w = chromosome.at(0);
    double x = chromosome.at(1);
    double y = chromosome.at(2);
    double z = chromosome.at(3);
    return (pow(sin(omega*(x+w)), 2))*(pow(sin(omega*(y+z)), 2))*(pow(2.7182818284590451,(-1*(w+x+y+z)/150)));
};
  
bool operator<(const Individual &one, const Individual &two)
{
    if(isMax)
        return one.fitness > two.fitness;
    else
        return one.fitness < two.fitness;
}

void printResults(vector<Individual> oldPop, int generation, vector<Individual> newPop){
    VariadicTable<string, string, double, int, int, string, string, string, double, int, int> t({"Num", "Chromosome  ", "Fitness     ", "Parent 1   ", "Parent 2  ", "@@@@", "Num", "Chromosome  ", "Fitness     ", "Parent 1   ", "Parent 2  "});
    cout << endl << "Generation: " << generation << "                                                                                             " << "Generation: " << generation +1 << endl;
    int count = 0;
    string countString;
    double min = 1000;
    double max = -1000;
    double average = 0;
    int bestIndividual = -90;
    for(Individual x: oldPop){
        if(newPop.at(count).fitness < min){
            min = newPop.at(count).fitness;
            if(!isMax){
                bestIndividual = count;
            }
        }
        if(newPop.at(count).fitness > max){
            max = newPop.at(count).fitness;
            if(isMax){
                bestIndividual = count;
            }
        }
        average += newPop.at(count).fitness;
        countString = to_string(count);
        countString.append(") ");
        string chromeString = "";
        chromeString.append(to_string(x.chromosome.at(0))).append(", ").append(to_string(x.chromosome.at(1))).append(", ").append(to_string(x.chromosome.at(2))).append(", ").append(to_string(x.chromosome.at(3)));
        string chromeString2 = "";
        chromeString2.append(to_string(newPop.at(count).chromosome.at(0))).append(", ").append(to_string(newPop.at(count).chromosome.at(1))).append(", ").append(to_string(newPop.at(count).chromosome.at(2))).append(", ").append(to_string(newPop.at(count).chromosome.at(3)));
        t.addRow({countString, chromeString, x.fitness, x.parent1, x.parent2, "@@@@", countString, chromeString2, newPop.at(count).fitness, newPop.at(count).parent1, newPop.at(count).parent2});
        count++;
    }
    t.print(std::cout);
    cout << "Note: Generation " << generation + 1 << " & Accumulated Statistics: max=" << max << ", min=" << min << ", avg=" << average/count << ", sum=" << average << " Best Individual to date is " << bestIndividual << " fitness: " << newPop.at(bestIndividual).fitness << " Average of chromesomes: " << (newPop.at(bestIndividual).chromosome.at(0) + newPop.at(bestIndividual).chromosome.at(1) +newPop.at(bestIndividual).chromosome.at(2) +newPop.at(bestIndividual).chromosome.at(3))/4.0  << endl;


}

vector<int> getElites(){
            vector<int> elites;
            int count = (ELITE_PERCENT*POPULATION_SIZE)/100;
            int i = 0;
            while(i < count){
            int randomIndex = rand() % POPULATION_SIZE;
                if (!(std::find(elites.begin(), elites.end(), randomIndex) != elites.end())){
                        elites.push_back(randomIndex);
                        i++;
                }
            }
            return elites;
}
int main()
{
      srand(time(0));
      vector<Individual> population;
      //initialize population
      for(int i = 0;i<POPULATION_SIZE;i++)
      {
          vector<double> chomesomes = createChromosome();
          population.push_back(Individual(chomesomes));
      }
      //
      while(generation < MAX_GENERATIONS)
      {
          sort(population.begin(), population.end());
          vector<Individual> newPop;

        //eliteism for 10% of the population

        vector<int> eliteIndexs = getElites();
        for(auto x: eliteIndexs){
            population.at(x).parent1 = -10;
            population.at(x).parent2 = -10;
            newPop.push_back(population.at(x));
        }
          unsigned long count;
          count = (POPULATION_SIZE - newPop.size())/2 + 1;
          if(generation%NARROW_EVERY == 1 && generation != 1 && generation > FLIP_MUTATION_TYPE){
            divider*= NARROW_DOWN_BY;
          }
          for(int i = 0;i<count;i++)
          {
              int r = getRand(0, POPULATION_SIZE * PERCENTILE_TO_REPRODUCE);
              Individual parent1 = population[r];
              int r2 = getRand(0, POPULATION_SIZE * PERCENTILE_TO_REPRODUCE);
              while(r2 == r){
                r2 = getRand(0, POPULATION_SIZE * PERCENTILE_TO_REPRODUCE);
              }
              Individual parent2 = population[r2];
            vector<vector<double>> reproductionresults = crossover(parent1, parent2);
                
              Individual childOne = Individual(reproductionresults.at(0));
              Individual childTwo = Individual(reproductionresults.at(1));

              childOne.mutate();
              childTwo.mutate();
              childOne.parent1 = r;
              childOne.parent2 = r2;
              childTwo.parent1 = r;
              childTwo.parent2 = r2;
              newPop.push_back(childOne);
              newPop.push_back(childTwo);
          }
          printResults(population, generation, newPop);
          population = newPop;
          generation++;
       }
}
