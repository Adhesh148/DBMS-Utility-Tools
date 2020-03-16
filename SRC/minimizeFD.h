#pragma once
#include "pch.h"
#include "helperFunctions.h"
#include "closure.h"

void Decompose(vector<struct FD> &S,int& m);
void RemoveRedundant(vector<struct FD> &S,int& m);
void RemoveExtraneousLHS(vector<struct FD> &S,int& m);
void RemoveExtraneousRHS(vector<struct FD> &S,int& m);

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