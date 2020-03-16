#pragma once
#include "pch.h"
#include "helperFunctions.h"

void findClosure(vector<struct FD> S,int m,char a[],vector<char> &temp_closure);
void GenerateClosure(vector<struct FD> S,int m,char a[],vector<char> &temp_closure);


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