#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <math.h>
#include <stack>
#include <set>
using namespace std;

/***************************
DATA STRUCTURES being used
****************************/

enum r{SAT=1 , UNSAT=-1} ;           // Using SAT and UNSAT instead of 1 and -1 for readbility

stack<int> literals ;                // Stack of integers to store the literals being conditioned and resolved

struct clause                        // Each clause is represented by a set of integers and count of
{                                    // number of literals in that clause
    set<int> vars ;
   int count=0 ;
};

struct result                         // Structure returned by unit_prop function
{
  int pushcount=0 ;                  // Number of literals pushed onto the stack during unit resolution
  int status=0 ;                     //
};

/************************************
      UNIT RESOLUTION function
 Inputs : array[] -> array of clauses
          size -> number of clauses
*************************************/

result unit_prop(clause array[] , int size)
{
 
  bool unsat = false;                                             // bool for checking unsatisfiability

  int tempv ;
  set<int>::iterator it,pos;
  result res ;
  int i ;

  for(i=0 ; i<size ; i++)                                          // iterates over all clauses
  {
    if(array[i].count==1)                                          // If UNIT CLAUSE FOUND
     {
         it =  array[i].vars.begin();
         tempv = *it;
         array[i].count=0;
         array[i].vars.clear();
         literals.push(tempv) ;
         res.pushcount++ ;

   
         for(int j=0 ; j<size ; j++)                           // resolving all the clauses possible 
         {                                                     // depending on the unit clause found above
            if(array[j].count>0)
            {
                 pos = array[j].vars.find(tempv) ;             // searching for same polarity literal
       
                 if(pos!=array[j].vars.end())
                 {
                    array[j].vars.clear();
                    array[j].count=0 ;
                 }


                 else
                 {
                   pos = array[j].vars.find(-tempv);         //searching for opposite polarity literal
                   
                   if(pos!=array[j].vars.end())
                   {
                      if(array[j].count==1)
                      {
                         array[j].count = 0 ;
                         array[j].vars.clear();        
                         unsat = true ;                                 // Return if UNSATISFIABLE
                         break ;
                      }
                      
                      array[j].count--;
                      array[j].vars.erase(pos);

                      if(array[j].count == 1 && j<i)                // if some clause above the current
                      {                                             // unit clause becomes unit as well
                         i=-1;                                      // start the loop again  
                      
                      }
                    }
                  }
            }

        }
     
        if(unsat == true)                                        //if unsatisfiable ; break out of loop
        {
    
           res.status=UNSAT;
           break;
        }
     }
  }

//CHECKING FOR SATISFIABLILITY

   bool satflag = false ;
   
   if(!unsat)
   {
      for(int e=0; e<size ; e++)
      {
          if(array[e].count>1)
          { 
             satflag=true ;
          }
       }

       if(!satflag)
       res.status=SAT;
   }

        
return res ;
}

/******************************************
    CONDITION Function
 Input : array[] -> array of clauses
         cv -> condition variable
         size -> number of clauses
*******************************************/

void condition_on(clause array[], int cv, int size)
{
    set<int>::iterator it;

    for(int k=0 ; k<size ; k++)
    {
       it = array[k].vars.find(cv) ;                          // if same polarity literal appears in the
       if(it!= array[k].vars.end())                           // clause, empty that clause
         {
            array[k].count = 0;
            array[k].vars.clear();
         }
       else
        {
          it = array[k].vars.find(-cv) ;                     // if opposite polarity literal exists in that
                                                             // clause, delete only that literal
          if(it!=array[k].vars.end())
           {
               array[k].count-- ;
               array[k].vars.erase(it);
           }
         }
     }
}

/****************************************
 FUNCTION implementing the DPLL Algorithm
 Input : array[] -> array of cluases
         size -> number of clauses
 ****************************************/

int DPLL(clause array[] , int size)
{
   result res ;
    clause tempu[size];
    copy(array,array+size,tempu);
   res = unit_prop(tempu,size) ;                 // calling unit resolution

   if(res.status==UNSAT)                         
    {
       for(int i=0; i<res.pushcount ; i++)
      {
         literals.pop();                          // if returns unsat, pop off the variables pushed during
      }                                           // resolution
      return UNSAT ; 
     }

   else if(res.status == SAT)
    {
       return SAT ;                              // return SAT if satisfiable assignment found
    }

    else
    {

         clause tempcl[size];                      // creating a copy of the results after resolution
         copy(tempu,tempu+size,tempcl);
         int cv;
        
         set<int>::iterator it ;

         for(int k=0 ; k<size ; k++)                  // first literal found in a non empty cluase
         {                                            // is chosen as the condition variable
           it = tempcl[k].vars.begin();
           if(it!=tempcl[k].vars.end())
            {
                cv = *it ;
                break ;
            } 
         }

         literals.push(cv);                            // push condition variable onto the stack
      
         condition_on(tempcl,cv,size);                 // call condition function



         if(DPLL(tempcl,size)==SAT)
          {
            return SAT ;
          }

         else
          {
               literals.pop();  

               copy(tempu,tempu+size,tempcl) ;
              
               condition_on(tempcl,-cv,size);            //condtioning on negation of the condition variable
               literals.push(-cv);
              
               if(DPLL(tempcl,size)!=UNSAT)
               {

                        return SAT ;
               }
               else
               {
                    
                    for(int e=0 ; e<res.pushcount ; e++)
                     {
                         literals.pop();                    // if nothing worked, pop that many variables
                     }                                      // that were added during resolution and one more
                     literals.pop();                        // i.e. the negated condition variable

                     return UNSAT ;
               }
          }
     }
}

/*****************************************
    MAIN Function
 Usage : Inputs the values and converts to
         the desired data structure
         Ouputs the results
******************************************/  

int main()
{

  string var, clau ;
 
  string temp ;

  while(getline(cin,temp,'\n'))
  {
    if(temp[0]=='c')                           // skipping on comments
    {
        continue;
    }
    else
    {
      break ;
    }
     
  }
 
  int len = temp.size();
  int i=0;

   stringstream ss1(temp) ;
   ss1>>clau ;
   ss1>>clau;
   ss1>>var;
   ss1>>clau;
  
int c ,v ;
 
  c = stoi(clau);                   // NUMBER OF CLAUSES
  v = stoi(var);                    //NUMBER OF VARIABLES


  clause array[c] ;                // ARRAY OF CLAUSES


  string str ;
  int value ;

  for(int i=0 ; i<c ; i++)
   {
    
    while(cin>>value)
    { 
     if(value==0)
      {
        break ;
      }
      else
       {
          array[i].vars.insert(value);
          array[i].count++;
          
       }
    }
   }


clause tempcl[c];
copy(array,array+c,tempcl);

bool printval[v] = {false};              // boolean array to indicate sign of the literal in satisfying 
                                         // assignment
int outp = DPLL(tempcl,c);                   

if(outp==SAT)
 {
   cout<<"SAT\n" ;
    while(!literals.empty())
      {
          int popped =literals.top() ;

          if(popped<0)
          {
            printval[(abs(popped)-1)] = true ;
          }

          literals.pop();
      }

      for(int ctr=0 ; ctr<v ; ctr++)
      {
        if(printval[ctr]==true)
          cout<<"-" ;
        cout<<(ctr+1)<<" ";
      }

      cout<<"0"<<endl;
}
else if(outp==UNSAT)
{
  cout<<"UNSAT\n";
}

return 0;

}
 


