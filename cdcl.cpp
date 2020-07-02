#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std ;


enum assigns { one = 1 , zero = 0 , unass = 2};

enum status { unresolved = 0,  satisfied = 1, unit =2 } ;

// MAXIMUM ARRAY SIZE

/************************************************
   NOTE : For inputs larger than these adjust 
          the CMAX and DMAX values accordingly.
************************************************/

#define CMAX 3000
#define DMAX 200

struct variable
{
   int assignment= unass ;
   int antecedent=-1 ;
   int dlevel=-1 ;
} ;

// GLOBALS

int num_of_clauses ;
int num_of_vars ;
int decision_level =0;
int num_of_learnt_clauses = 0;


//FUNCTIONS FOR DEBUGGING
void printClauses(int array[CMAX][DMAX],int learntArray[CMAX][DMAX])
{


   for(int i =0 ; i<num_of_clauses ; i++)
   {
      for(int j= 1  ; j<=num_of_vars ; j++)
      {
        if(array[i][j]!=0)
        	cout<<array[i][j]<<" ";
      }
      
      cout<<"\n";
    }

   for(int i =0 ; i<num_of_learnt_clauses ; i++)
   {
      for(int j= 1  ; j<=num_of_vars ; j++)
      {
        if(learntArray[i][j]!=0)
        	cout<<learntArray[i][j]<<" ";
      }
      
      cout<<"\n";
    }
}

void printStatus(int clause_status[CMAX])
{

	for(int i=0 ; i<num_of_clauses ; i++)
	{
		switch(clause_status[i])
		{
            case 0 : cout<<i<<" : Unresolved\n"; break;
            case 1 : cout<<i<<" : Satisfied\n"; break;
            case 2 : cout<<i<<" : Unit\n"; break;

            default: cout<<"Something's Wrong\n";
		}
	}
}

void printVariables(variable varray[DMAX])
{
  for(int i = 0 ; i<=num_of_vars ; i++)
  {
    cout<<"VARIABLE : "<<i<<endl;
    cout<<"   Decision level :"<<varray[i].dlevel<<endl;
    cout<<"   Antecedent :"<<varray[i].antecedent<<endl ;
    cout<<"   Assignment :" ;
    switch(varray[i].assignment)
    {
      case one : cout<<"1"<<endl; break;
      case zero : cout<<"0"<<endl; break;
      case unass : cout<<"Unassigned"<<endl; break;
    }
  }
}


// Function to count the number of UNASSIGNED variables in a given clause
int countUnresolved(int array[CMAX][DMAX], variable varray[DMAX], int clause_no )
{
 int count=0 ;

   for(int i = 1 ; i<=num_of_vars ; i++)
   {
   	  if(array[clause_no][i]!= 0)
   	  {	
   	  	if(varray[i].assignment == unass)
          { 
             count++ ;
          }	 
      }
    }
   return count ;
}

//UNIT RESOLUTION
int unit_resolution (int array[CMAX][DMAX], int clause_status[CMAX], 
	                 int learntArray[CMAX][DMAX], int clause_status_learnt[CMAX], variable varray[DMAX])
{

   bool flag = false;

   for(int i=0 ; i<num_of_clauses ; i++)
   {
     if(clause_status[i] == unit)
     {
     	int unit_element ;

     	//Finding Unit Element
     	for(int k = 1 ; k<=num_of_vars ; k++)
     	{
     		if(array[i][k]!=0)
     		{	
     			int temp = array[i][k];
     			if(varray[abs(temp)].assignment == unass)
     			{
     				unit_element = array[i][k];
     				break;
     			}
     	    }		
     	}


        //Setting Value of unit element
        
        clause_status[i] = satisfied ;
        varray[abs(unit_element)].antecedent = i ;
        varray[abs(unit_element)].dlevel = decision_level;

        if(unit_element<0)
           varray[abs(unit_element)].assignment = zero ;
        else
           varray[abs(unit_element)].assignment = one ;

     	//Check all clauses for this element
     	for(int j = 0 ; j<num_of_clauses ; j++)
     	{
     	  if(clause_status[j]!=satisfied)
          {
     	   // If same sign as the unit element	
           if(array[j][abs(unit_element)] == unit_element)
           {
 
              clause_status[j]= satisfied;          
           }

           //If oppsite sign
           else if(array[j][abs(unit_element)] == (-unit_element))
           {
           	  if(clause_status[j]== unit)
           	  {
           	  	varray[0].antecedent = j ;           // Variable array's 0th element represents
                varray[0].dlevel = decision_level;   // the contradictory node of Implication Graph
                       
                return 2; 
           	  }

           	  int count_left = countUnresolved(array, varray, j);
           	  if(count_left==1)
           	  {
           	  	 clause_status[j]=unit;
           	  	 flag = true ;
           	  }
            }
           } 

        }

        // Unit resolution in the array of learnt clauses
        if(num_of_learnt_clauses>0)
        {
        	for(int l=0 ; l<num_of_learnt_clauses ; l++)
        	{
               if(clause_status_learnt[l]!= satisfied)
               {
                 if(learntArray[l][abs(unit_element)] == unit_element)
           		{
 
              		clause_status_learnt[l]= satisfied;          
          	    }

           	    //If oppsite sign
           		else if(learntArray[l][abs(unit_element)] == (-unit_element))
           		{
           	  		if(clause_status_learnt[l]== unit)
           	  		{
           	  			varray[0].antecedent = l+num_of_clauses ; // Variable array's 0th element represents
                		varray[0].dlevel = decision_level;   // the contradictory node of Implication Graph
                       
                		return 2; 
           	  		} 

           	  		int count_left = countUnresolved(learntArray, varray, l);

           	  		if(count_left==1)
           	  		{
           	  	 		clause_status_learnt[l]=unit;
           	  	 		flag = true ;
           	 	 	}              	

                }
        	   }
            }
        } 

      }

    }

    if(num_of_learnt_clauses>0)
    {
       for(int i = 0  ; i< num_of_learnt_clauses ; i++)
       {
       	 if(clause_status_learnt[i]==unit)
       	 {
       	 	int unit_element_l ;

         	for(int k = 1 ; k<=num_of_vars ; k++)
     		{
     			if(learntArray[i][k]!=0)
     			{	
     				int temp = learntArray[i][k];
     				if(varray[abs(temp)].assignment == unass)
     				{
     					unit_element_l = learntArray[i][k];
     				}
     	    	}		
     		}

        	//Setting Value of unit element

        	clause_status_learnt[i] = satisfied ;
        	varray[abs(unit_element_l)].antecedent = i + num_of_clauses;
        	varray[abs(unit_element_l)].dlevel = decision_level;

        	if(unit_element_l<0)
           		varray[abs(unit_element_l)].assignment = zero ;
        	else
          	 	varray[abs(unit_element_l)].assignment = one ;     		

     		//Check all clauses for this element
     		for(int j = 0 ; j<num_of_clauses ; j++)
     		{
     	  		if(clause_status[j]!=satisfied)
          		{
     	   			// If same sign as the unit element	
           			if(array[j][abs(unit_element_l)] == unit_element_l)
           			{
 
              			clause_status[j]= satisfied;          
           			}

           			//If oppsite sign
           			else if(array[j][abs(unit_element_l)] == (-unit_element_l))
           			{
           	  			if(clause_status[j]== unit)
           	  			{
           	  				varray[0].antecedent = j ;           // Variable array's 0th element represents
                			varray[0].dlevel = decision_level;   // the contradictory node of Implication Graph
                       
                			return 2; 
           	  			}

           	  			int count_left = countUnresolved(array, varray, j);
            
           	  			if(count_left==1)
           	  			{
           	  	 			clause_status[j]=unit;
           	  	 			flag = true ;
           	  			}
            		}
           		} 

        	}

        	for(int l=0 ; l<num_of_learnt_clauses ; l++)
        	{
               if(clause_status_learnt[l]!= satisfied)
               {
                 if(learntArray[l][abs(unit_element_l)] == unit_element_l)
           		{
 
              		clause_status_learnt[l]= satisfied;          
          	    }

           	    //If oppsite sign
           		else if(learntArray[l][abs(unit_element_l)] == (-unit_element_l))
           		{
           	  		if(clause_status_learnt[l]== unit)
           	  		{
           	  			varray[0].antecedent = l+num_of_clauses ; // Variable array's 0th element represents
                		varray[0].dlevel = decision_level;   // the contradictory node of Implication Graph
                       
                		return 2; 
           	  		} 

           	  		int count_left = countUnresolved(learntArray, varray, l);

           	  		if(count_left==1)
           	  		{
           	  	 		clause_status_learnt[l]=unit;
           	  	 		flag = true ;
           	 	 	}              	

                }
        	   }
            }

       	 }

       }
    }

 
    //Recursing if some another unit clause was formed during this call
    if(flag==true)
    {
       int r = unit_resolution(array,clause_status,learntArray, clause_status_learnt, varray);
       return r ;
    }	
    return 1;
}


// Returns the unassigned variable to condition
int findConditionVariable(variable varray[])
{
  int i ;
  for(i=1; i<=num_of_vars ; i++)
  {

    if(varray[i].assignment==unass)
      break;
  }

  return i ;
}

//Function to Condition the clauses on the condition variable passed
// Only handles the original set of clauses, learned clauses are handles separately
void condition (int array[CMAX][DMAX], variable varray[DMAX], int clause_status[DMAX]  , int cv )
{

   // Value of condition variable = 1 

   varray[cv].assignment = one ;
   varray[cv].dlevel = decision_level;
   varray[cv].antecedent = -1 ;

   // Conditioning the Clauses

   for(int i = 0 ; i<num_of_clauses ; i++)
   {

   	if(clause_status[i]!= satisfied)
   	{	
      if(array[i][cv] < 0)
      {
          int count_left = countUnresolved(array,varray,i);
          if(count_left==1)
          {
            clause_status[i]= unit ;
          }
          else if(count_left>1)
          {
          	clause_status[i]=unresolved;
          }	
      }

      else if (array[i][cv]>0)
      {

         clause_status[i] = satisfied;

      }

   }

  }
} 

// Function called by during conflict analysis, to find the implied variable in the learned clause
int findImpliedVariable(vector<int>* learning_clause, variable varray[])
{

     int result=0;
     int i =0 ;
     vector<int>::iterator it = (*learning_clause).begin();

     for(; it!=(*learning_clause).end() ; it++)
      {
        int element =  (*learning_clause)[i];

        ++i;

        if(varray[abs(element)].dlevel==decision_level && varray[abs(element)].antecedent>=0)
        {
          result = element ;
          break ;
        }
      }

   return result ;
}

//Called by Conflict Analyser, to check whether a variable is present in the learned clause
bool isPresent(vector<int>* learning_clause, int temp)
{
	vector<int>::iterator it = (*learning_clause).begin();
	int i=0 ;
   for(; it!=(*learning_clause).end() ; it++)
   {
        if((*learning_clause)[i] == temp)
          return true ;
      i++;
   } 


   return false ;
}

//Resolution function used by Conflict Analyser 
void resolve(int alpha, int element, vector<int>* learning_clause, 
			int array[CMAX][DMAX], int learntArray[CMAX][DMAX])
{

  int temp ;
  vector<int>::iterator it = (*learning_clause).begin();
  int pos = 0;


  for( ; it!= (*learning_clause).end() ; it++)
  {
    if((*learning_clause)[pos] == element)
    {
         (*learning_clause).erase(it);
         break;
    }
    pos++;
  }

  if(alpha<num_of_clauses)
  {
    for(int i = 1 ; i <= num_of_vars ; i++)
    {
        temp = array[alpha][i] ;
      if(temp!=0)  
      {	
        if(temp != (-element) && !isPresent(learning_clause, temp))
        {
           (*learning_clause).push_back(temp) ;
        }
      } 
    }
  }

  else
  {
    alpha = alpha - num_of_clauses ;
    for(int i =1 ; i <= num_of_vars ; i++)
    {
        temp = learntArray[alpha][i] ;
      if(temp!=0)
      {  
        if(temp != (-element) && !isPresent(learning_clause, temp))
        {
           (*learning_clause).push_back(temp) ;
        }
      }  
    }

  }
}


// Conflict Analyser
int conflictAnalysis(int array[CMAX][DMAX], variable varray[DMAX], int learntArray [CMAX][DMAX], int clause_status_learnt[CMAX])
{
    vector<int> learning_clause  ;
    int alpha = varray[0].antecedent ;
    int element ;
    

   //Checks whether the antecedent of NULL node is in learnt array or the original clause array
   //Initializing the intial learned clause 
    if(alpha< num_of_clauses)
    {
    	for(int i=1 ; i<=num_of_vars ; i++)
    	{
    	  if(array[alpha][i]!=0)
    	  {	
    		learning_clause.push_back(array[alpha][i]) ;
          }	
    	}
    }

    else
    {    
    	alpha = alpha - num_of_clauses;

        for(int i=1 ; i<= num_of_vars  ; i++)
      	{
      	  if(learntArray[alpha][i]!=0)
    	  {	
        	learning_clause.push_back(learntArray[alpha][i]) ;
          }	
      	}
    }	

    //Printing Learned Clause
    // int k=0;
    // for(vector<int>::iterator pit=learning_clause.begin(); pit!=learning_clause.end(); pit++)
    // {
    // 	cout<<learning_clause[k]<<" ";
    // 	k++;
    // }
    // cout<<endl;

      element = findImpliedVariable(&learning_clause,varray);
      alpha = varray[abs(element)].antecedent;
  
    // Clause learning Process
    while(element!=0)
    {

    	resolve(alpha, element, &learning_clause, array,learntArray);
        element = findImpliedVariable(&learning_clause,varray);
        alpha = varray[abs(element)].antecedent;
    }


    vector<int>::iterator it = learning_clause.begin() ;
    int i=0;
    //Finding max1 = the highest decision level so far
    int max1 = varray[abs(learning_clause[0])].dlevel ;
  
    int vcount =0;
    //Adding the learned clause to the learntArray
    for(; it!=learning_clause.end() ; it++)
    {
    	int temp = learning_clause[i];
    	learntArray[num_of_learnt_clauses][abs(temp)] = temp;
        vcount++;
    	if(varray[abs(learning_clause[i])].dlevel>max1)
    		max1 = varray[abs(learning_clause[i])].dlevel;
    	i++;
    }	

    if(vcount>1)
   { 	
    clause_status_learnt[num_of_learnt_clauses] = unresolved;
   }

   else if(vcount==1)
   {
   	clause_status_learnt[num_of_learnt_clauses] = unresolved;
   }

    num_of_learnt_clauses++;

    //Finding the second highest decision level
    it = learning_clause.begin();
    int max2 = 0;
    i=0;
    for(; it!=learning_clause.end() ; it++)
    {
    	if(varray[abs(learning_clause[i])].dlevel>max2 && varray[abs(learning_clause[i])].dlevel<max1)
    		max2 = varray[abs(learning_clause[i])].dlevel;
    	i++;
    }


    if(decision_level==0 && max2 == 0)
    	return -1 ;
    else
    	return max2;

}

// Returns true if all have been assigned
bool allAssigned(int clause_status[CMAX], int clause_status_learnt[CMAX])
{
	bool retval = true ;

	for(int i = 0 ; i<num_of_clauses ; i++)
	{
		if(clause_status[i]!=satisfied)
		{
			retval = false;
			break;
		}
	}

	if(retval)
	{
		for(int j=0 ; j<num_of_learnt_clauses ; j++)
		{
			if(clause_status_learnt[j]!=satisfied)
			{
				retval = false;
				break;
			}
		}
	}

	return retval;
}
    
// Handles the learnt clauses before unit resolution    
void handleLearntClauses(int learntArray[CMAX][DMAX], variable varray[DMAX], int clause_status_learnt[CMAX])
{
	for(int i=1 ; i<=num_of_vars ; i++)
	{
	
	   if(varray[i].assignment == zero)
	   {

	   		
		 for(int j=0 ; j<num_of_learnt_clauses ; j++)
		 {
		   if(clause_status_learnt[j]!= satisfied)
	       {
			if(learntArray[j][i]<0)
			{
				clause_status_learnt[j] = satisfied;
			}
			else if(learntArray[j][i]>0)
			{
				int other_count = countUnresolved(learntArray, varray, j);
            
                if(other_count==1)
                	clause_status_learnt[j]=unit;
                else
                	clause_status_learnt[j]=unresolved;

			}
		   }	
		 }
	    }

	    else if(varray[i].assignment==one)
	    {
	      for(int j=0 ; j<num_of_learnt_clauses ; j++)
		 {
		   if(clause_status_learnt[j]!= satisfied)
	       {
			if(learntArray[j][i]>0)
			{
				clause_status_learnt[j] = satisfied;
			}
			else if(learntArray[j][i]<0)
			{
				int other_count = countUnresolved(learntArray, varray, j);
            
                if(other_count==1)
                	clause_status_learnt[j]=unit;
                else
                	clause_status_learnt[j]=unresolved;

			}

		   }

	    }
      }
    }  
}   

// Used during backtracking to restore the state of the clauses
void restoreState (int array[CMAX][DMAX], variable varray[DMAX], int clause_status[CMAX])
{
	for(int i=1 ; i<=num_of_vars ; i++)
	{
		if(varray[i].assignment == one)
		{

			for(int j =0 ; j<num_of_clauses ; j++)
			{
			

			 if(clause_status[j]!=satisfied)
			 {	
				if(array[j][i]>0)
				{
                   clause_status[j]= satisfied;
				}
				else if(array[j][i]<0)
				{

					int other_count = countUnresolved(array, varray, j);

					if(other_count == 1)
						clause_status[j]= unit;

					else if(other_count>1)
						clause_status[j]= unresolved;

				}
			  }
		    }
	    }

	    else if(varray[i].assignment == zero)
		{

			for(int j =0 ; j<num_of_clauses ; j++)
			{
				
			 if(clause_status[j]!=satisfied)
			 {	
				if(array[j][i]<0)
				{
                   clause_status[j]= satisfied;
				}
				else if(array[j][i]>0)
				{
					int other_count = countUnresolved(array, varray, j);
					if(other_count == 1)
						clause_status[j]= unit;
					else if(other_count>1)
						clause_status[j]= unresolved;

				}
			  }
		    }
	    }
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
 
     
       {
          
          stringstream ss(str) ;

          ss>>c ; 
          ss>>c ;
          ss>>v ;
          ss>>c ;

          num_of_clauses = stoi(c);
          num_of_vars = stoi(v);
       }
    
    // Defining the Structures   

    int array [CMAX][DMAX] = {0};
    int clause_status [CMAX] = {-1};
    int learntArray[CMAX][DMAX] = {0};
    int clause_status_learnt[CMAX]={-1};

    variable varray[DMAX];

    int num;
    int count = 0;

  //Reading and Initializing
    
  for(int c=0; c<num_of_clauses ; c++)
  { 
  	   count = 0;

       while(cin>>num)
       {
           if(num==0)
               break ;
           else
           {
              array[c][abs(num)] = num ;
              count++ ;
           }

       }

       if(count == 1)
            clause_status[c]= unit;
       else
           clause_status[c]= unresolved; 
    }   



 
     int runcount = 0;
     bool UNSATFLAG = false;
     int result ;
     int cv ;
     bool after_backtrack = false ;

     result = unit_resolution(array,clause_status, learntArray, clause_status_learnt, varray) ;

     if(result == 2)
     	cout<<"\nUNSAT\n";
     else
     {
     	while(!allAssigned(clause_status,clause_status_learnt))
     	{
     		if(after_backtrack==false)
     		{
     			cv = findConditionVariable(varray);

     			decision_level++;
     			condition(array,varray,clause_status,cv);
     		}
     		else
     		{
     			after_backtrack = false;
     		}


     		result = unit_resolution(array,clause_status, learntArray, clause_status_learnt,varray);
           
            if(result == 2)
            {
            	int backtrack_level = conflictAnalysis(array,varray,learntArray,clause_status_learnt);

                decision_level = backtrack_level ;

                after_backtrack = true ;

            	if(backtrack_level<0)
            	{
            	  UNSATFLAG = true ;
            	  cout<<"UNSAT\n";
            	  break;
            	}

            	else
            	{

            		runcount++;

            		//Resetting Decision Variables
            		for(int i=0; i<=num_of_vars ; i++)
            		{
            			if(varray[i].dlevel>decision_level || (varray[i].dlevel==decision_level && varray[i].antecedent>-1))
            			{
            				varray[i].assignment= unass;
            				varray[i].antecedent = -1;
            				varray[i].dlevel = -1;
            			}

            		}

            		for(int i=0 ; i<num_of_clauses ; i++)
            		{
            			clause_status[i] = unresolved;
            		}

            		for(int j=0 ; j<num_of_learnt_clauses ; j++)
            		{
            			int count_u = countUnresolved(learntArray,varray,j);
            			if(count_u==1)
            			    clause_status_learnt[j]=unit;
            			else
                            clause_status_learnt[j]=unresolved;
            		}
                  

            		//Update Status Array
            		restoreState(array, varray, clause_status);
            		handleLearntClauses(learntArray,varray, clause_status_learnt);

            	}	

            }
          
     	}
     }

     if(!UNSATFLAG)
     {	
     	cout<<"SAT\n";
     	for(int i =1 ; i<=num_of_vars; i++)
     	{
     		if(varray[i].assignment==one)
     			cout<<i<<" ";
     		else if(varray[i].assignment==zero)
     			cout<<-i<<" ";
     		else
     			cout<<i<<" ";
     	}

     	cout<<"0\n";
     } 	

     return 0 ;
}

   