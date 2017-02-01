//Stewart Dulaney
//Code for Project 2
//Calculate phone bill

#include <iostream>
#include <string>
using namespace std;

int main ()
{
	//Collect data
	int minutesUsed;
	int messagesUsed;
	string customerName;
	int monthNumber;

	cout << "Minutes used: ";							
	cin >> minutesUsed;
	if (minutesUsed < 0)				//Check for negative minutes
		{
		cout << "---" << endl;
		cout << "The number of minutes used must be nonnegative." << endl;
		return 1;
		}

	cout << "Text messages: ";
	cin >> messagesUsed;
	if (messagesUsed < 0)				//Check for negative text messages
		{
		cout << "---" << endl;
		cout << "The number of text messages must be nonnegative." << endl;
		return 1;
		}

	cin.ignore(10000, '\n');
	cout << "Customer name: ";
	getline (cin, customerName);
	if (customerName == "")				//Check for valid customer name
		{
		cout << "---" << endl;
		cout << "You must enter a customer name." << endl;
		return 1;
		}

	cout << "Month number (1=Jan, 2=Feb, etc.): ";				
	cin >> monthNumber;
	if (monthNumber < 1 || monthNumber > 12)		//Check for valid month number
		{
		cout << "---" << endl;
		cout << "The month number must be in the range 1 through 12." << endl;
		return 1;
		}
	
	
	
	
	
	//Calculate bill
	double balance = 45.00;											//Basic Charge
	cout.setf(ios::fixed);
	cout.precision(2);				
	if (minutesUsed > 400)											//Account for minutes not included in basic charge	
		balance = balance + (minutesUsed - 400) * 0.40;
	if (messagesUsed > 250 && messagesUsed <= 500)					//Account for messages not included in basic charge	
		{
		if (monthNumber >= 10 || monthNumber <= 5)
			balance = balance + (messagesUsed - 250) * 0.02;
		if (monthNumber >= 6 && monthNumber <= 9)
			balance = balance + (messagesUsed - 250) * 0.01;
		}
	if (messagesUsed > 500)
		{	
		if (monthNumber >= 10 || monthNumber <= 5)	
			balance = balance + (250 * 0.02) + (messagesUsed - 500) * 0.11;
		if (monthNumber >= 6 && monthNumber <= 9)
			balance = balance + (250 * 0.01) + (messagesUsed - 500) * 0.11;
		}
	cout << "---" << endl;
	cout << "The bill for " << customerName << " is $" << balance << endl;
	

}
