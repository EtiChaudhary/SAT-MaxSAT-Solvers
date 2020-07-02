#include <iostream>
#include <sstream>
#include <string>
#include "core/Solver.h"
#include "mtl/Vec.h"
#include "mtl/Heap.h"
#include "mtl/Alg.h"
#include "utils/Options.h"
#include "core/SolverTypes.h"

using namespace std ;

#define CMAX 1500
#define DMAX 500

int num_of_clauses = 0;
int num_of_vars = 0 ;


/************************************
Function to convert the CNF input to
format required for input to MiniSAT
for k=0
************************************/
void convert (int array[CMAX][DMAX], Minisat::Solver* solver)
{

	Minisat::vec<Minisat::Lit> newClause ;
	int relaxation_var = num_of_vars ;

	//Creating new variables
	for(int i=1 ; i<=num_of_vars; i++)
	{
		solver->newVar();
	}

	//Adding clauses to the solver	
	for(int i= 0 ; i<num_of_clauses ; i++)
	{
		for(int j=0  ; j<num_of_vars ; j++)
		{
			if(array[i][j]!=0)
			{
				if(array[i][j]>0)
					newClause.push(Minisat::mkLit(j));
				else
					newClause.push(~Minisat::mkLit(j));
			}
		}
		solver->addClause(newClause);
		newClause.clear();
	}

}

/******************************************
Function to convert the formula into
SEQUENTIAL (at most k) ENCODING
*******************************************/
void atMostkEncode (int array[CMAX][DMAX], int k , Minisat::Solver* solver)
{

	Minisat::vec<Minisat::Lit> newClause ;
	int relaxation_var = num_of_vars ;


	for(int i=1 ; i<=num_of_vars+num_of_clauses ; i++)
	{
		solver->newVar();
	}

	//k times more variables
	for(int i=1 ; i<=k*num_of_clauses ; i++)
	{
		solver->newVar();
	}

	//Encoding the orginal grammar with relaxation variable appended to each clause
	for(int i= 0 ; i<num_of_clauses ; i++)
	{
		for(int j=0  ; j<num_of_vars ; j++)
		{
			if(array[i][j]!=0)
			{
				if(array[i][j]>0)
					newClause.push(Minisat::mkLit(j));
				else
					newClause.push(~Minisat::mkLit(j));
			}
		}

		newClause.push(Minisat::mkLit(relaxation_var));
		solver->addClause(newClause);
		relaxation_var++ ;
		newClause.clear();
	}

	//*******************************
	//Adding the Extra Clauses
	//*******************************

	//Handling k = 1 case	
	int relaxing_variable = num_of_vars ;
	int start_var = num_of_vars + num_of_clauses ;

	for(int i=1 ; i<=num_of_clauses-1 ; i++)
	{	
		newClause.push(~Minisat::mkLit(relaxing_variable));
		newClause.push(Minisat::mkLit(start_var));
		solver->addClause(newClause);

		relaxing_variable++ ;
		start_var++ ;
		newClause.clear();
	}

	//Handling k=2 and above
	//Formula 2
	newClause.clear();
	start_var = num_of_vars+ 2*num_of_clauses ;
	for(int j=2 ; j<=k ; j++)
	{
		newClause.push(~Minisat::mkLit(start_var));
		solver->addClause(newClause);
		newClause.clear();
		start_var += num_of_clauses ;
	}	


	//Formula 3
	start_var = num_of_vars+num_of_clauses+1 ;
	int prev ;
	newClause.clear();
	for(int j = 1 ; j<=k ; j++)
	{
		for(int i=2 ; i<=num_of_clauses-1 ; i++)
		{
			prev = start_var-1 ;
			newClause.push(~Minisat::mkLit(prev));
			newClause.push(Minisat::mkLit(start_var));
			solver->addClause(newClause);
			newClause.clear();
			start_var++ ;
		}

		start_var = num_of_vars + (j+1)*num_of_clauses + 1;
	}

	//Formula 4
	int second_arg ;
	int current = num_of_vars + 2*num_of_clauses + 1;
	int first_arg = num_of_vars +1 ;

	newClause.clear();

	for(int j=2 ; j<=k ; j++)
	{
		for(int i=2 ; i<=num_of_clauses-1 ; i++)
		{
			second_arg = current - num_of_clauses - 1;
			newClause.push(~Minisat::mkLit(first_arg));
			newClause.push(~Minisat::mkLit(second_arg));
			newClause.push(Minisat::mkLit(current));
			solver->addClause(newClause);
			newClause.clear();
			current++ ;
			first_arg++ ;
		}	

		current = current+ 2;
		first_arg =  num_of_vars + 1;
	}

	// Formula 5
	first_arg = num_of_vars + 1;
	prev = num_of_vars + k*num_of_clauses ;
	newClause.clear();
	for(int i=2 ; i<=num_of_clauses ; i++)
	{
		newClause.push(~Minisat::mkLit(first_arg));
		newClause.push(~Minisat::mkLit(prev));
		solver->addClause(newClause);
		newClause.clear();
		first_arg++ ;
		prev++ ;
	}
}


int main()
{

	// Taking Input

    string str , c , v ;
    int ccount = 0 ;
    while(getline(cin, str, '\n'))
    {
       if(str[0] != 'c')
             break ;
    }     
 
     
    
	stringstream ss(str) ;

	ss>>c ; 
	ss>>c ;
	ss>>v ;
	ss>>c ;

	num_of_clauses = stoi(c);
	num_of_vars = stoi(v);

    
    // Defining the Structures   
    int array [CMAX][DMAX] = {0};
    int num;

  //Reading and Initializing
    int inp ;
  for(int c=0; c<num_of_clauses ; c++)
  { 
  	cin>>num ;
       while(num!= 0)
       {

              inp =  abs(num) - 1 ;
              array[c][inp] = num ;
              
              cin>>num;

       }
   } 


	Minisat::Solver* solver ;
	int k = 0;
	int result = false;
	solver = new Minisat::Solver();

	
	//**********************************
	// Inplementaing : Linear 
	//******************************** 
	convert(array, solver);

	// Check for case k=0
	if(solver->solve() == true)
		cout<<num_of_clauses<<"\n";
	else
	{
		//While UNSAT, keep on incrementing k
		// Once SAT found, break out of the loop
		while(result == false && k<=num_of_clauses)
		{
			k++ ;
			solver = new Minisat::Solver();

			atMostkEncode(array, k , solver);
			result = solver->solve() ;
		}

		cout<<num_of_clauses-k<<endl;
	}

	//Printing the output
	for(int i=0 ; i<num_of_vars ; i++)
	{
		if(toInt(solver->model[i])==0)
			cout<<i+1<<" ";
		else
			cout<<"-"<<i+1<<" ";
	}


   cout<<"0\n";
   return 0;
}
