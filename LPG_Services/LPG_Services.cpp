// LPG_Services.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>

using namespace std;


int main()
{
	// All declarations here:
	char a;																						// Random char Input to Freeze the Terminal
	char option;																				// Yes/No option char
	char option_UP;																				// Upper case option
	char customer_type;																			// Household or Establishment option

	cout << "Welcome to UNO LPG Services!" << endl;
	cout << "Would you like to proceed? Yes(Y/y) or No(N/n): ";									// Option Display to proceed
	cin >> option;																				// User Input

	option_UP = toupper(option);																// Converting option to Uppercase

	while (1)																					// Infinite Loop for Verification
	{	
		if (option_UP == 'Y' || option_UP == 'N')												// Entering option Y or N will break the loop
		{
			break;																				// Ends the verification Loop
		}

		cout << "Invalid option! Please Try again: ";											// Ask user to re-enter a valid option
		cin >> option;																			// User Input
		option_UP = toupper(option);															// Converting option to Uppercase
	}

	if (option_UP == 'N') 
	{
		cout << "Thank you, see you again! " << endl;
		cout << "Press any key to exit. . ." << endl;
		cin >> a;																				// Random input to Freeze the Terminal

		return 0;
	}

	if (option_UP == 'Y')
	{
		cout << "\nDo you want to get LPG for your Household(H/h) or Establisment(E/e)? : ";	// Household or Establishment option display
		cin >> customer_type;
	}
	

	cin >> a;																					// Random input to Freeze the Terminal

	return 0;
}

