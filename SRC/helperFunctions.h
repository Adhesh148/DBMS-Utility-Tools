#pragma once
#include "pch.h"

void RemoveDuplicates(vector <char> &temp_closure);
void Subset(char a[],int b[],int i,int n,vector<vector<char>> &subs);
int DoesContain(vector<char> &temp_closure,char b[]);
int isIn(char a[],char b);
void Sort(char a[],int n);
void sortSubsetsLength(vector<vector<char>> &ckSet_subs,int n);

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

void sortSubsetsLength(vector<vector<char>> &ckSet_subs,int n)			// Give lengthwise sorting only (No lexographic)
{
	for(int i=1;i<n;++i)
		for(int j=1;j<n-i-1;++j)
			if(ckSet_subs[j].size()>ckSet_subs[j+1].size())
				ckSet_subs[j].swap(ckSet_subs[j+1]);
}