/******************************************
* AUTHOR : AdheshR*
******************************************/

/*	
BUGS FOUND:
	1. aa -> b is not working		// allow only distinct inputs.
	2. sort the inputs   (Done)
	3. Repeat the LHS and RHS function once more.	(Had a justification)
MODIFICATIONS NEEDED:
	1. Sort function is BubbleSort for convienence. Should change to nlogn sort.
	2. Have to add another function called findMinimal and include Decompose, RemoveRedundant etc within it.
*/

#include "pch.h"
#include "closure.h"
#include "minimizeFD.h"
#include "findCK.h"
#include "helperFunctions.h"
#include "fileio.h"


void findMinimalCover(vector<struct FD> &S,int& m);

void findCandidateKey(vector<struct FD> S,int m,int n);

int main()
{
	int n,m;							// n is the number of elements in the relation, m is the number of FDS.
	vector<struct FD> S(MAX); 			//declaring m FD structures 

	fileRead(n,m,S);

	for(int i=0;i<m;++i)
	{
		Sort(S[i].a,strlen(S[i].a));
		Sort(S[i].b,strlen(S[i].b));
	}

	findMinimalCover(S,m);

	findCandidateKey(S,m,n);
	
}

void findMinimalCover(vector<struct FD> &S,int& m)
{
	Decompose(S,m);
	RemoveRedundant(S,m);
	RemoveExtraneousLHS(S,m);
	RemoveExtraneousRHS(S,m);

	printf("The minimal cover is:\n");
	for(int i=0;i<m;++i)
		cout << S[i].a<<" -> "<<S[i].b<<"\n";

	// Have to Run the RemoveExtraneous LHS and RHS twice because ----- ??
	// RemoveExtraneousLHS(S,m);
	// RemoveExtraneousRHS(S,m);
	//RemoveRedundant(S,m);
}

// COMMENTS
/*

		//Print THE CLOSURES...
		
		for(int i=0;i<closure.size();++i)
		{
			cout << closure[i];
		}
		printf("\n");

		
		// PRINT SUBSETS
		for(int i=1;i<pow(2,len_ckSet);++i)
		{
			for(int j=0;j<ckSet_subs[i].size();++j)
				cout << ckSet_subs[i][j];
			cout << "\n";
		}

*/

