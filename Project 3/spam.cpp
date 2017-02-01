//Stewart Dulaney
//Code for Project 3
//Spam Detector

#include <iostream>
#include <string>
#include <cctype>
#include <assert.h>
using namespace std;

string subject;
string body;
int spamScore = 0;
int numberWords = 0;

string getFirstWord(string text);
string getLastWord(string text);
string extractWord(string& text);
bool isUppercase(string text);
string makeUppercase(string text);
bool hasMultipleExclamations(string text);
bool isGibberishWord(string text);
double wordCounter(string text);
void doUnitTests(); 


int main() 
{
	string response;

	cout << "Enter the subject line of the email: ";
	getline(cin, subject); 
	cout << "Enter the body of the email.  Press Enter on an empty line to finish." << endl;
	getline(cin, body);

	//Spam Rule 1
	double allUppercase = 0;
	for(size_t k = 0; k < subject.size(); k++)
	{
		if(isUppercase(extractWord(subject)))
			allUppercase++;
	}
	if(getFirstWord(subject) != "" && allUppercase/wordCounter(subject) > .90)
		spamScore += 30;

	//Spam Rule 2
	if(isGibberishWord(getLastWord(subject)))
		spamScore += 40;

	//Spam Rule 3
	if(hasMultipleExclamations(subject))
		spamScore +=20;

	//Spam Rule 4
	allUppercase = 0;
	for(size_t k = 0; k < subject.size(); k++)
	{
		if(isUppercase(extractWord(body)))
			allUppercase++;
	}
	if(extractWord(body) != "" && allUppercase/wordCounter(body) > .50)
		spamScore += 40;

	//Spam Rule 5 
	for(size_t k = 0; k < subject.size(); k++)
		if(extractWord(body) == "buy" || extractWord(body) == "cheap" || extractWord(body) == "click" ||extractWord(body) == "diploma" || extractWord(body) == "enlarge"
			|| extractWord(body) == "free" || extractWord(body) == "limited" || extractWord(body) == "money" || extractWord(body) == "now" || extractWord(body) == "offer"
			|| extractWord(body) == "only" || extractWord(body) == "pills" || extractWord(body) == "sex")
				spamScore += 5;
	
	if(spamScore > 100)
		cout << "This email is classified as spam, because its spam score is " << spamScore << "." << endl;
	else
		cout << "This email is classified as legitimate, because its spam score is " << spamScore << "." << endl;
	
	cout << "Would you like to classify another email (y or n)? ";
	getline(cin, response);
	while(response != "y" && response != "n")
	{
		cout << "Please enter y or n." << endl;
		cout << "Would you like to classify another email (y or n)? ";
		getline(cin, response);
	}
	
}


string getFirstWord(string text)
{
	for(size_t k = 0; k < text.size(); k++)
		{
			if(isalpha(text[k]))
			{	
				text = text.substr(k, text.size() - k);	
				break;
			}
			
		}	 
	for(size_t k = 0; k < text.size(); k++)
		{
			if(!isalpha(text[k]))
				{
				text = text.substr(0, k);
				break;
				}
		}
	return text;
}

string getLastWord(string text)
{
	for(size_t k = text.size() - 1; k >= 0; k--)
		{
			if(isalpha(text[k]))
			{	
				text = text.substr(0, k + 1);	
				break;
			}
		}
	for(size_t k = text.size() - 1; k >= 0; k--)
		{	
			if(!isalpha(text[k]))
			{
				text = text.substr(k + 1, text.size() - k);
				break;		
			}
		}	 
	return text;
}

string extractWord(string& text)
{
	string t;
	if(text == "")
		return "";
	for(size_t k = 0; k < text.size(); k++)
		{
			if(isalpha(text[k]))
			{	
				text = text.substr(k, text.size() - k);	
				break;
			}
			
		}	 
	for(size_t k = 0; k < text.size(); k++)
		{
			if(!isalpha(text[k]))
			{
				t = text.substr(0, k);
				text = text.substr(k, text.size() - k);
				return t;
			}
				
		}
	if(isalpha(text[text.size() - 1]))
		{
		t = text;
		text = "";
		}

	return t;
}

bool isUppercase(string text)
{
	for(size_t k = 0; k < text.size(); k++)
		{
		if(!isalpha(text[k]))
			continue;
		else if(islower(text[k]))
			return false;
		else if(isupper(text[k]))
			continue;
		}
	return true;
}

string makeUppercase(string text)
{
	string t;
	for(size_t k = 0; k < text.size(); k++)
	{
		t += toupper(text[k]);
	}
	return t;
}

bool hasMultipleExclamations(string text)
{
	int numberExclamations = 0;
	for(size_t k = 0; k < text.size(); k++)
	{
		if(text[k] == '!')
			numberExclamations++;
		else
		{
			numberExclamations = 0; 
		}	
		if(numberExclamations >= 3)
			return true;
	}
	return false;
	

}

bool isGibberishWord(string text)
{
	int numberConsonants = 0;
	for(size_t k = 0; k < text.size(); k++)
	{
		if(isalpha(text[k]) && text[k] != 'a' && text[k] != 'e' && text[k] != 'i' && text[k] != 'o' && text[k] != 'u' 
								&& text[k] != 'A' && text[k] != 'E' && text[k] != 'I' && text[k] != 'O' && text[k] != 'U')
		{
			numberConsonants++;
			if (numberConsonants > 3)	
				return true;
		}
		else if(!isalpha(text[k]) || text[k] == 'a' || text[k] == 'e' || text[k] == 'i' || text[k] == 'o' || text[k] == 'u' 
								|| text[k] == 'A' || text[k] == 'E' || text[k] == 'I' || text[k] == 'O' || text[k] == 'U')
			numberConsonants = 0;
		
	}
	return false;

}

double wordCounter(string text)
{
	for(;;)
		if(extractWord(text) != "")
			numberWords++;
		else
			return numberWords;
}

void doUnitTests()
{
	assert(getFirstWord("hello there") == "hello");
	assert(getFirstWord(" $@#%!!") == "");
	assert(getLastWord("MAKE MONEY FAST!!") == "FAST");
	string s = "***hello there";
	assert(extractWord(s) == "hello" && s == " there");
	assert(extractWord(s) == "there" && s == "");
	assert(isUppercase("WOW!!"));
	assert(!isUppercase("WoW!!"));
	assert(makeUppercase("Earn *big* MONEY at home!!") == "EARN *BIG* MONEY AT HOME!!");
	assert(hasMultipleExclamations("Wow!!!"));
	assert(hasMultipleExclamations("Congrats!!!! Good luck"));
	assert(!hasMultipleExclamations("W!I!N!"));
	assert(!hasMultipleExclamations("!! !"));
	assert(isGibberishWord("AGPQrxab"));
	assert(isGibberishWord("xxxxozzzz"));
	assert(!isGibberishWord("mortgage"));
	assert(!isGibberishWord("discount prescriptions"));
	cerr << "All tests succeeded" << endl;

}





