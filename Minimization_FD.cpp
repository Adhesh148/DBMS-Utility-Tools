/******************************************
* AUTHOR : AdheshR*
******************************************/
#include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
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

struct FD
{
	char a[MAX];
	char b[MAX];
};

int main()
{
	int n,m;	// n is the number of elements in the relation, m is the number of FDS.
	cin>>n>>m;

	vector<struct FD> S(MAX); //declaring m FD structures 

	for(int i=0;i<m;++i)
		scanf("%s -> %s",S[i].a,S[i].b);

	Decompose(S,m);

	RemoveRedundant(S,m);

	RemoveExtraneousLHS(S,m);

	RemoveExtraneousRHS(S,m);

	printf("The minimal cover is:\n");
	for(int i=0;i<m;++i)
	{
		cout << S[i].a<<" "<<S[i].b<<"\n";
	}

	//RemoveRedundant(S,m);
	
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
			if(strcmp(S[i].a,S[j].a) == 0 && strcmp(S[i].b,S[j].b) ==0)
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
		{
			temp_closure.push_back(S[i].b[0]);
			GenerateClosure(S,m,S[i].b,temp_closure);
		}
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

int DoesContain(vector<char> &temp_closure,char b[])
{
	for(int i=0;i<temp_closure.size();++i)
		if(b[0] == temp_closure[i])
			return 1;

	return 0;
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

