//Stewart Dulaney
//Code for Project 4

#include <iostream>
#include <string>
#include <cassert>
using namespace std;

int appendToAll(string a[], int n, string value);
int lookup(const string a[], int n, string target);
int positionOfMax(const string a[], int n);
int rotateLeft(string a[], int n, int pos);
int rotateRight(string a[], int n, int pos);
int flip(string a[], int n);
int differ(const string a1[], int n1, const string a2[], int n2);
int subsequence(const string a1[], int n1, const string a2[], int n2);
int lookupAny(const string a1[], int n1, const string a2[], int n2);
int split(string a[], int n, string splitter);

int main()
{
	string folks[6] = {"george", "lucille", "gob", "michael", "lindsay", "buster"};
	assert(appendToAll(folks, 6, "!!!") == 6);
	assert(folks[0] == "george!!!" && folks[5] == "buster!!!");
	const string names[5] = {"bob", "john", "mary", "ellen", "mary"};
	assert(lookup(names, 5, "mary") == 2);
	assert(lookup(names, 5, "ellen") == 3);
	const string letters[6] = {"a", "b", "c", "d", "e", "f"};
	assert(positionOfMax(letters, 6) == 5);
	const string persons[6] = {"george", "lucille", "gob", "michael", "lindsay", "buster"};
	assert(positionOfMax(persons, 6) == 3);
	string humans[5] = {"homer", "marge", "bart", "lisa", "maggie"};
	assert(rotateLeft(humans, 5, 0) == 0);
	assert(humans[1] == "bart" && humans[4] == "homer");
	string simpsons[5] = {"homer", "marge", "bart", "lisa", "maggie"};
	assert(rotateRight(simpsons, 5, 2) == 2);
	assert(simpsons[0] == "bart" && simpsons[1] == "homer" && simpsons[2] == "marge");
	string beings[6] = {"claire", "phil", "", "haley", "alex", "luke"};
	assert(flip(beings, 4) == 4);
	assert(beings[0] == "haley" && beings[1] == "" && beings[2] == "phil" && beings[3] == "claire");
	string group1[6] = {"claire", "phil", "", "haley", "alex", "luke"};
	string group2[4] = {"claire", "phil", "mitchell", "luke"};
	assert(differ(group1, 6, group2, 4) == 2);
	assert(differ(group1, 2, group2, 1) == 1);
	cerr << "All tests succeeded." << endl;
	
}

int appendToAll(string a[], int n, string value)
{
	if(n < 0)
		return -1;
	for(int k = 0; k < n; k++)
		a[k] += value;
	return n;
}

int lookup(const string a[], int n, string target)
{
	if(n < 0)
		return -1;
	for(int k = 0; k < n; k++)
	{
		if(a[k] == target)
			return k;
	}
	return -1;
}

int positionOfMax(const string a[], int n)
{
	if(n < 0)
		return -1;
	string maxString = "";
	int indexMax = 0;
	for(int k = 0; k < n; k++)
	{
		if (a[k] > maxString)
		{
			maxString = a[k];
			indexMax = k;
		}
		else
			continue;
	}
	return indexMax;
}

int rotateLeft(string a[], int n, int pos)
{
	if( n < 0 || pos < 0)
		return -1;
	string s;
	s = a[pos];
	for(int k = pos; k < (n-1); k++)
	{
		a[k] = a[k+1];
	}
	a[n-1] = s;
	return pos;
}

int rotateRight(string a[], int n, int pos)
{
	if( n < 0 || pos < 0)
		return -1;
	string t;
	t = a[pos];
	for(int k = pos; k >= 1; k--)
	{
		a[k] = a[k-1];
	}
	a[0] = t;
	return pos;
}

int flip(string a[], int n)
{
	if(n < 0)
		return -1;
	int x = 0;
	string y[10000];
	for(int k = (n-1); k >= 0; k--)
	{
		y[x] = a[k];
		x++;
	}
	for(int k = 0; k < n; k++)
	{
		a[k] = y[k];
	}
	return n;
}

int differ(const string a1[], int n1, const string a2[], int n2)
{
	if(n1 < 0 || n2 < 0)
		return -1;
	int n;
	if(n1 < n2)
		n = n1;
	if(n2 < n1)
		n = n2;
	for(int k = 0; k <= n1 && k <= n2; k++)
	{
		if(a1[k] != a2[k])
			return k;
	}
	return n;
}

int subsequence(const string a1[], int n1, const string a2[], int n2)
{
	for(int k = 0; k <= n1; k++)
	{

	}
	return -1;
}

int lookupAny(const string a1[], int n1, const string a2[], int n2)
{
	return -1;
}

int split(string a[], int n, string splitter)
{
	return -1;
}
