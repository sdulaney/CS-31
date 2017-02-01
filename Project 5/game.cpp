//Stewart Dulaney
//Code for Project 5
//Mystery Word Game

#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <algorithm> 
#include <ctime>
using namespace std;

//Declare Global Constants
const int MINWORDLENGTH = 4;
const int MAXWORDLENGTH = 6;
const int MAXWORDS = 10000;

//Declare External Functions
int loadWords(char words[][MAXWORDLENGTH+1], int maxWords);
int manageOneRound(char words[][MAXWORDLENGTH + 1], int num, int wordnum);
int randInt(int lowest, int highest);
bool isValidInput(char trialWord[], int size);

int main()
{
	char wordList[MAXWORDS][MAXWORDLENGTH + 1];
	int nWords = loadWords(wordList, 10000);
	int numberRounds;
	int wordNumber;
	double average;
	int total = 0;
	int min = 0;
	int max = 0;
	int score;

	std::srand(std::time(0));			

	if(nWords < 1)
	{
		cout << "No words were loaded, so I can't play the game" << endl;
		return 0;
	}

	cout << "How many rounds do you want to play? ";
	cin >> numberRounds;
	if(numberRounds <= 0)
	{
		cout << "The number of rounds must be positive" << endl;
		return 0;
	}
	cin.ignore(1000, '\n');
	for(int k = 1; k <= numberRounds; k++)
	{
		wordNumber= randInt(0, nWords - 1);

		cout << endl << "Round " << k << endl;
		cout << "The mystery word is " << strlen(wordList[wordNumber]) << " letters long" << endl;
		score = manageOneRound(wordList, nWords, wordNumber);
		cout << "You got it in " << score << " tries" << endl;
		if(score < min || min == 0)
			min = score;
		if(score > max || max == 0)
			max = score;
		total += score;
		cout.setf(ios::fixed);
		cout.precision(2);
		average = (total/static_cast<double>(k));
		cout << "Average: " << average << ", minimum: " << min << ", maximum: " << max << endl;
	}

}

//Puts words into the "words" array and returns the number of words put into the array
int loadWords(char words[][MAXWORDLENGTH+1], int maxWords)
{
	const char filename[] = "C:/Users/sdulaney/Desktop/words.txt";

    ifstream wordfile(filename);
    if ( ! wordfile)
	{
        cout << "Cannot open " << filename << endl;
        return -1;
	}
    const int LINELIMIT = 10000;
    char line[LINELIMIT];
    int numWords = 0;
    while (wordfile.getline(line, LINELIMIT))
	{
        if (numWords == maxWords)
        {
            cout << "Using only the first " << numWords
                 << " words from " << filename << endl;
            break;
        }
        int k;
        for (k = 0; islower(line[k]); k++)
            ;
	if (line[k] == '\0'  &&  k >= MINWORDLENGTH  &&  k <= MAXWORDLENGTH)
        {
            strcpy(words[numWords], line);
            numWords++;
        }
    }
    return numWords;
}

//Plays one round of the game and returns the score for that round
int manageOneRound(char words[][MAXWORDLENGTH + 1], int num, int wordnum)
{
	if(num <= 0 || wordnum < 0 || wordnum >= num)
		return -1;
	char trialWord[100];
	int numberAttempts = 0;
	int numberOverlaps = 0;

	while(strcmp(trialWord, words[wordnum]) != 0)
	{
		cout << "Trial word: ";
		cin.getline(trialWord, 100);
		numberAttempts++;

		if(strcmp(trialWord, words[wordnum]) == 0)
			return numberAttempts;

		if(strlen(trialWord) < 4 || strlen(trialWord) > 6 || isValidInput(trialWord, strlen(trialWord)) == false)
		{
			cout << "Your trial word must be a word of 4 to 6 lower case letters" << endl;
		}

		if(strlen(trialWord) >= 4 && strlen(trialWord) <= 6 && isValidInput(trialWord, strlen(trialWord)) == true)
		{
			bool isFound = true;
			for(int k = 0; k <= num; k++)
			{
				if(strcmp(trialWord, words[k]) == 0)
					break;
				else if(k == num)
					isFound = false;
			}
			if(isFound == false)
				cout << "I don't know that word" << endl;
			if(isFound == true)
			{	
				numberOverlaps = 0;
				for(int k = 0; k != strlen(words[wordnum]); k++)
				{
					for(int i = 0; i != strlen(trialWord); i++)
					{
						if(words[wordnum][k] == trialWord[i])
						{
							trialWord[i] = 1;
							numberOverlaps++;
							break;
						}
					}
				}
				cout << numberOverlaps << endl;
			}
		}
	}
}

//Produces a sequence of numbers, each of which is almost independent of the others
int randInt(int lowest, int highest)
	{
	    if (highest < lowest)
	        swap(highest, lowest);
	    return lowest + (std::rand() % (highest - lowest + 1));
	}

//Determines is the C string is all letters and all lowercase
bool isValidInput(char trialWord[], int size)
{
	for(int k = 0; k < size; k++)
	{
		if(isalpha(trialWord[k]) && islower(trialWord[k]))
			continue;
		else
			return false;
	}
	return true;
}
