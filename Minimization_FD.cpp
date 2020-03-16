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

#include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <string>
using namespace std;
#define LL long long
#define MAX 100
#define MOD 1000000007
#define dd double

int k=0;

void Decompose(vector<struct FD> &S,int& m);
void RemoveRedundant(vector<struct FD> &S,int& m);
void findClosure(vector<struct FD> S,int m,char a[],vector<char> &temp_closure);
void GenerateClosure(vector<struct FD> S,int m,char a[],vector<char> &temp_closure);
void Subset(char a[],int b[],int i,int n,vector<vector<char>> &subs);
void RemoveDuplicates(vector <char> &temp_closure);
int DoesContain(vector<char> &temp_closure,char b[]);
void RemoveExtraneousLHS(vector<struct FD> &S,int& m);
void RemoveExtraneousRHS(vector<struct FD> &S,int& m);
void Sort(char a[],int n);
int isIn(char a[],char b);

void findCandidateKey(vector<struct FD> S,int m,int n);

struct FD
{
	char a[MAX];
	char b[MAX];
};

int main()
{
	int n,m;							// n is the number of elements in the relation, m is the number of FDS.
	cin>>n>>m;

	vector<struct FD> S(MAX); 			//declaring m FD structures 

	for(int i=0;i<m;++i)
	{
		scanf("%s -> %s",S[i].a,S[i].b);
		Sort(S[i].a,strlen(S[i].a));
		Sort(S[i].b,strlen(S[i].b));
	}

	Decompose(S,m);

	RemoveRedundant(S,m);

	RemoveExtraneousLHS(S,m);

	RemoveExtraneousRHS(S,m);

	// Have to Run the RemoveExtraneous LHS and RHS twice because ----- ??

	// RemoveExtraneousLHS(S,m);

	// RemoveExtraneousRHS(S,m);

	printf("The minimal cover is:\n");
	for(int i=0;i<m;++i)
	{
		cout << S[i].a<<" "<<S[i].b<<"\n";
	}

	//RemoveRedundant(S,m);

	findCandidateKey(S,m,n);
	
}

void Decompose(vector<struct FD> &S,int& m)	//DECOMPOSITION
{
	int cnt =0;
	for(int i=0;i<m;++i)
	{
		int len = strlen(S[i].b);
		if(len>1)
		{
			for(int j=1;j<strlen(S[i].b);++j)
			{
				strcpy(S[m+cnt].a,S[i].a);
				S[m+cnt].b[0] =  S[i].b[j];
				++cnt;
			}
			S[i].b[1]= '\0';
		}	
	}
	m = m+cnt;
}

void RemoveRedundant(vector<struct FD> &S,int& m)
{
	int cnt =0;
	for(int i=0;i<m;++i)
	{
		for(int j=i+1;j<m;++j)
		{
			if(strcmp(S[i].a,S[j].a) == 0 && strcmp(S[i].b,S[j].b) ==0 && S[i].a[0]!='\0')
			{
				S.erase(S.begin()+j);
				++cnt;
			}
		}
	}
	m = m - cnt;
}

void RemoveExtraneousLHS(vector<struct FD> &S,int& m)
{
	for(int i=0;i<m;++i)
	{
		int len = strlen(S[i].a);
		if(len>1)
		{
			// For each character in the array, remove it from the LHS and run find closure. If closure gives all atrributes 
			for(int j=0;j<len;++j)		
			{
				char str[len],removed_char[1];
				int pos=0;
				for(int l=0;l<len;++l)
				{
					if(l!=j)
					{
						str[pos] = S[i].a[l];
						++pos;
					}
				}
				removed_char[0] = S[i].a[j];
				removed_char[1] = '\0';
				str[len-1] = '\0';
				vector<char> closure;

				closure.clear();


				findClosure(S,m,str,closure); // find the closure of str

				if(DoesContain(closure,S[i].b) == 1 || DoesContain(closure,removed_char) == 1 )	// Can we get b without attribute S[i].a[j] ?
				{
					// Remove S[i].a[j] from S[i].a because that attribute is extraneous
					for(int l=j;l<len-1;++l)
						S[i].a[l] = S[i].a[l+1];

					S[i].a[--len] = '\0';
				}
			}
		}
	}
}

void RemoveExtraneousRHS(vector<struct FD> &S,int& m) // Remove Transitive Relations
{
	int i=0;
	while(i<m)
	{
		vector<struct FD> P(MAX); // define new structure excluding ith element from S
		int pos =0;
		for(int j=0;j<m;++j)
		{
			if(i!=j)
			{
				strcpy(P[pos].a,S[j].a);
				strcpy(P[pos].b,S[j].b);
				++pos;
			}
		}
		
		vector<char> closure;
		closure.clear();

		char str[MAX];
		for(int j=0;j<strlen(S[i].a);++j)
		{
			str[j] = S[i].a[j];
		}
		str[strlen(S[i].a)] ='\0';

		int new_m = m-1;

		findClosure(P,new_m,str,closure);

		if(DoesContain(closure,S[i].b) == 1)
		{

			S.erase(S.begin()+i);
			--m;
		}
		else
			++i;
	}
}

void findClosure(vector<struct FD> S,int m,char a[],vector<char> &temp_closure)	// Find closure(a)
{
	//Generate all possible subss of the Attribute excluding empty set

	int n = strlen(a),b[MAX];
	fill(b,b+n,0);
	vector<vector<char>> subs(MAX);
	
	k=0;

	Subset(a,b,0,n,subs);

	char str[MAX];

	//Push all elements in character to temp_closure. Trivial Closures
	for(int i=0;i<subs[pow(2,n)-1].size();++i)
		temp_closure.push_back(subs[pow(2,n)-1][i]);


	for(int l =1;l<pow(2,n);++l)
	{
		for(int j=0;j<subs[l].size();++j)
			str[j] =  subs[l][j];
		str[subs[l].size()] = '\0';

		GenerateClosure(S,m,str,temp_closure);
	}

	RemoveDuplicates(temp_closure);
	sort(temp_closure.begin(),temp_closure.end());
	
	// Generate Subsets of the temp_closure and run GenerateClosure again. Run untill no more elements are added to the closure set.
	int iter =0;
	int len = temp_closure.size();
	int new_len = temp_closure.size();
	do
	{
	len = temp_closure.size();
	char new_substr[len];
	new_substr[len] = '\0';
	for(int i=0;i<len;++i)
		new_substr[i] = temp_closure[i];

	fill(b,b+n,0);
	k=0;
	vector<vector<char>> new_subs(MAX);

	Subset(new_substr,b,0,len,new_subs);

	char new_str[MAX];

	for(int i=1;i<pow(2,len);++i)
	{
		for(int j=0;j<new_subs[i].size();++j)
			new_str[j]  = new_subs[i][j];
		new_str[new_subs[i].size()] = '\0';

		GenerateClosure(S,m,new_str,temp_closure);

	}

	RemoveDuplicates(temp_closure);
	sort(temp_closure.begin(),temp_closure.end());
	new_len = temp_closure.size();
	}while(len<new_len);

}

void GenerateClosure(vector<struct FD> S,int m,char a[],vector<char> &temp_closure)
{
	for(int i=0;i<m;++i)
	{
		if(strcmp(S[i].a,a) == 0)
			temp_closure.push_back(S[i].b[0]);
	}
}

void RemoveDuplicates(vector <char> &temp_closure)
{
	int i=0;
	while(i<temp_closure.size())
	{
		if(count(temp_closure.begin(),temp_closure.end(),temp_closure[i]) >1)
			temp_closure.erase(temp_closure.begin()+i);
		else
			++i;
	}
}

void Subset(char a[],int b[],int i,int n,vector<vector<char>> &subs)
{
	if(i<n)
	{
		b[i] = 0;
		Subset(a,b,i+1,n,subs);
		b[i] = 1;
		Subset(a,b,i+1,n,subs);
	
	}
	else if(i == n)
	{
		for(int j=0;j<n;++j)
			if(b[j] == 1)
				subs[k].push_back(a[j]);
		++k;
	}
}

int DoesContain(vector<char> &temp_closure,char b[])	// returns 1 if contains and 0 otherwise
{
	for(int i=0;i<temp_closure.size();++i)
		if(b[0] == temp_closure[i])
			return 1;

	return 0;
}

int isIn(char a[],char b)
{
	for(int i=0;i<strlen(a);++i)
		if(b == a[i])
			return 1;
	return 0;
}

void Sort(char a[],int n)		// Sorted using Bubble Sort - Have to replace with merge sort
{
	char temp;
	for(int i=0;i<n-1;++i)
	{
		for(int j=0;j<n-i-1;++j)
		{
			if(a[j]>a[j+1])
			{
				temp = a[j];
				a[j] = a[j+1];
				a[j+1] = temp;
			}
		}
	}
}

void findCandidateKey(vector<struct FD> S,int m,int n)
{
	// get RHS Attributes
	vector<char> rhsAttributes;
	rhsAttributes.clear();
	for(int i=0;i<m;++i)
		rhsAttributes.push_back(S[i].b[0]);

	RemoveDuplicates(rhsAttributes);	// Remove duplicates from the vector

	char ckAttributes[MAX];	// contains attributes that must always be included in all CKs.
	int len_ckA=0;
	char check[2];	// single length character array. Helps to find non rhsAttributes
	check[1] = '\0';
	for(int i=0;i<n;++i)
	{
		check[0] = 'A'+i;
		if(DoesContain(rhsAttributes,check) == 0)
			ckAttributes[len_ckA++] = check[0];
	}
	ckAttributes[len_ckA] = '\0';


	//Compute closure of ckAttributes. if closure = R => ckAttributes are the only candidate key. else ....
	vector<char> closure;
	closure.clear();

	findClosure(S,m,ckAttributes,closure);

	if(closure.size() == n)			// closure contains the n elements of the relation Rs
		cout << "CANDIDATE KEY: "<<ckAttributes<<"\n";
	else
	{
		//get LHS Attributes
		vector<char> lhsAttributes;
		lhsAttributes.clear();
		for(int i=0;i<m;++i)
		{
			for(int j=0;j<strlen(S[i].a);++j)
				lhsAttributes.push_back(S[i].a[j]);
		}

		RemoveDuplicates(lhsAttributes);

		int len_notCkA =0,len_ckSet=0;
		char notCkAttributes[MAX]; 					// contains attributes that cannot be part of the CK.
		char ckSet[MAX];							// contains R - (notCK) // these attributes can be a part of CK
		for(int i=0;i<n;++i)
		{
			check[0] = 'A'+i;
			if(DoesContain(lhsAttributes,check) == 0)
				notCkAttributes[len_notCkA++] = check[0];
			else
				if(isIn(ckAttributes,check[0]) == 0)
					ckSet[len_ckSet++] = check[0];
		}
		ckSet[len_ckSet] = '\0';
		notCkAttributes[len_notCkA++] = '\0';

		// Generate all subsets of ckSet
		int b[MAX];
		fill(b,b+len_ckSet,0);
		vector<vector<char>> ckSet_subs(MAX);
		k=0;
		Subset(ckSet,b,0,len_ckSet,ckSet_subs);

		// After Generating the Subset. Check if (subset+ckAttributes) is SuperKey ??
		sortSubsets(ckSet_subs,pow(2,len_ckSet)-1);
		for(int i=1;i<pow(2,len_ckSet);++i)
		{
			for(int j=0;j<ckSet_subs[i].size();++j)
				cout << ckSet_subs[i][j];
			cout << "\n";
		}
		/*
		vector<vector<char>> superKey(MAX);
		char tempSuperKey[MAX];
		int len_tempSuperKey=0,len_superKey=0;
		
		for(int i=1;i<pow(2,len_ckSet);++i)
		{
			vector<char> temp_closure;
			temp_closure.clear();
			len_tempSuperKey =0;
			for(int j=0;j<ckSet_subs[i].size();++j)
				tempSuperKey[len_tempSuperKey++] = ckSet_subs[i][j];
			for(int j=0;j<strlen(ckAttributes);++j)
				tempSuperKey[len_tempSuperKey++] = ckAttributes[j];

			tempSuperKey[len_tempSuperKey] = '\0';
			Sort(tempSuperKey,len_tempSuperKey);
			findClosure(S,m,tempSuperKey,temp_closure);	

			if(temp_closure.size() ==n)
			{
				for(int j=0;j<strlen(tempSuperKey);++j)
					superKey[len_superKey].push_back(tempSuperKey[j]);
				++len_superKey;
			}
		}

		for(int i=0;i<len_superKey;++i)
		{
			for(int j=0;j<superKey[i].size();++j)
				cout << superKey[i][j];
			cout << "\n";
		}
		*/


	}


	//vector<vector<char>> ck(MAX); 	// stores candidate keys
}

void sortSubsets(vector<vector<char>> v,int n)
{
	for(int i=0;i<n;++i)
	{
		for(int j=0;j<n;++j)
		{
			
		}
	}
}


// COMMENTS
/*

		//Print THE CLOSURES...
		
		for(int i=0;i<closure.size();++i)
		{
			cout << closure[i];
		}
		printf("\n");

*/

