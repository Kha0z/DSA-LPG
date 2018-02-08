// LPG_Services.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>

using namespace std;


int main()
{
	// All declarations here:
	// Random char Input to Freeze the Terminal
	char a;								
	// Yes/No option char
	char option;																				
	// Upper case option (Yes or No)
	char option_UP;		
	// Household or Establishment option
	char customer_type;		
	// Upper case option (Establishment)
	char customer_type_up;
	// Name Variable
	string name;
	// Address Variable
	string address;
	// Household/Company Variable
	string establishment;


	// Option Display to proceed
	cout << "Welcome to UNO LPG Services!" << endl;
	cout << "Would you like to proceed? Yes(Y/y) or No(N/n): ";		
	// User Input
	cin >> option;		


	// Converting option to Uppercase
	option_UP = toupper(option);		


	// Infinite Loop for Verification
	while (1)																					
	{	
		// Entering option Y or N will break the loop
		if (option_UP == 'Y' || option_UP == 'N')												
		{
			// Ends the verification Loop
			break;																				
		}

		// Ask user to re-enter a valid option
		cout << "Invalid option! Please Try again: ";						
		// User Input
		cin >> option;										
		// Converting option to Uppercase
		option_UP = toupper(option);															
	}

	if (option_UP == 'N') 
	{
		cout << "Thank you, see you again! " << endl;
		cout << "Press any key to exit. . ." << endl;
		// Random input to Freeze the Terminal
		cin >> a;																				

		return 0;
	}

	if (option_UP == 'Y')
	{
		// Name Input
		cout << "\nEnter your name: " ;
		cin >> name;

		// Establishment Input
		cout << "\nEnter Establishment name: ";
		cin >> establishment;

		// Address Input
		cout << "\nEnter your address: ";
		cin >> address;
		
		// Household or Establishment option display
		cout << "\nDo you want to get LPG for your Household(H/h) or Establisment(E/e)? : ";	
		cin >> customer_type;

		// Converting option to Uppercase
		customer_type_up = toupper(customer_type);

		// Infinite Loop for Verification
		while (1) 
		{
			if (customer_type_up == 'H' || customer_type_up == 'E')
			{
				// Ends the verification loop
				break;
			}

			// Ask user to re-enter a valid option
			cout << "Invalid option! Please Try again: ";
			// User Input
			cin >> customer_type_up;
			// Converting option to Uppercase
			customer_type_up = toupper(customer_type);
		}
	}
	
	// Random input to Freeze the Terminal
	cin >> a;																					

	return 0;
}

