#pragma once
#include "pch.h"
#include "helperFunctions.h"
#include "closure.h"

void findCandidateKey(vector<struct FD> S,int m,int n)
{
	vector<vector<char>> ck(MAX);
	int len_ck=0;

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

	cout << "CANDIDATE KEY(S): \n";
	if(closure.size() == n)	// closure contains the n elements of the relation Rs
	{
		for(int i=0;i<len_ckA;++i)
			ck[len_ck].push_back(ckAttributes[i]);
		++len_ck;
	}		
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
		sortSubsetsLength(ckSet_subs,pow(2,len_ckSet));

		char tempCk[MAX];
		int len_tempCk=0,i=1;
		for(;i<pow(2,len_ckSet);++i)
		{
			vector<char> temp_closure;
			temp_closure.clear();
			len_tempCk=0;
			for(int j=0;j<ckSet_subs[i].size();++j)
				tempCk[len_tempCk++] = ckSet_subs[i][j];
			for(int j=0;j<strlen(ckAttributes);++j)
				tempCk[len_tempCk++] = ckAttributes[j];

			tempCk[len_tempCk]= '\0';
			Sort(tempCk,len_tempCk);
			findClosure(S,m,tempCk,temp_closure);
			vector<char>::iterator it; 
			int pos;

			if(temp_closure.size() == n)
			{
				// Remove the attributes in the current CK from the remaining subsets
				for(int j=i+1;j<pow(2,len_ckSet);++j)
				{
					for(int l=0;l<len_tempCk;++l)
					{
						it = find(ckSet_subs[j].begin(),ckSet_subs[j].end(),tempCk[l]);
						pos = it - ckSet_subs[j].begin();
						if(it!=ckSet_subs[j].end())
							ckSet_subs[j].erase(ckSet_subs[j].begin()+pos);
					}
				}

				// Copy the CK into the vector ck.
				for(int j=0;j<len_tempCk;++j)
					ck[len_ck].push_back(tempCk[j]);
				++len_ck;
			}
		}
	}

	for(int i=0;i<len_ck;++i)
	{
		for(int j=0;j<ck[i].size();++j)
			cout << ck[i][j];
		printf("\n");
	}
}