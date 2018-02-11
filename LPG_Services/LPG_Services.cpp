/**************************************************************
* FILE NAME: LPG_Services.cpp                                 *
*                                                             *
* PURPOSE: This file defines the starting point of the entire *
*          starting point of the project                      *
*                                                             *
* FILE REFERENCES:                                            *
* Name       I/O            Description                       *
* ----       ---            -----------                       *
* None                                                        *
*                                                             *
* ABNORMAL TERMINATION CONDITIONS, ERRORS AND WARNINGS:       *
*  none                                                       *
*                                                             *
* ASSUMPTIONS, CONSTRAINTS, RESTRICTIONS:                     *
*  This code assumes that correct calibration of the LDR      *
*  sensitivity potentiometer. CALIBRATE BEFORE USE            *
*                                                             *
* NOTES:                                                      *
*  This program utilises the following timers and interrupts: *
*  Interrupt0: pin 2 falling edge detection                   *
*  Timer0: millis(), as per normal                            *
*  Timer1: pin 10 servo, which means no PWM on pin 9 and 10   *
*                                                             *
* REQUIREMENTS/FUNCTIONAL SPECIFICATIONS REFERENCES:          *
*  none                                                       *
*                                                             *
* DEVELOPMENT HISTORY:                                        *
*                                                             *
* Date      Author     Change  Release  Description of Change *
*                      Id                                     *
* --------  ---------  ------  -------  --------------------- *
* 07-02-18  Josh                  1      Initial version      *
* 10-02-18  Ziyue                 1      Significant changes  *
*								         to code structure    *
*                                                             *
* ALGORITHM (PDL):   IGNORE THIS PSEUDOCODE FOR NOW           *
* 
*                                                             *
* CALL begin(9600) to start serial connection for debugging   *
* SET LDR_PIN's mode to INPUT                                 *
* CALL attachInterrupt to attach FALLING to INT0 (pin 2)      *
*                                                             *
* CALL sevseg.begin to configure 7 segment display            *
* CALL sevseg.setBrightness to predefined brightness level    *
*                                                             *
* CALL servo.attach to attach servo to SERVO_PIN              *
*                                                             *
* LOOP                                                        *
*     CALL sevseg.setNumber to display hit_count              *
*     IF current time passed cooloff time THEN                *
*         SET target_up to true                               *
*     ENDIF                                                   *
*     IF target_up is true THEN                               *
*         CALL servo.write with an angle of 90                *
*     ELSE                                                    *
*         CALL servo.write with an angle of 0                 *
*     ENDIF                                                   *
*     CALL sevseg.refreshDisplay to refresh the display       *
* ENDLOOP                                                     *
*                                                             *
* ISR0                                                        *
*     SET hit_millis to the result of CALL millis             *
*     IF more than COOLOFF has passed since last_hit THEN     *
*         SET last_hit to hit_millis                          *
*         SET target_up to false                              *
*         SET hit_count to increment by 1                     *
*         CALL Serial.println to print debug message          *
*     ENDIF                                                   *
* ENDISR0                                                     *
*                                                             *
**************************************************************/

#include "stdafx.h"

using namespace std;

#pragma region Function Prototypes

void Command_Mode();
bool Init_Mode();
int initial();
vector<string> split(const string& str, const string& delim);

// Adapter functions
void List_Customers(rapidcsv::Document &doc);
void Add_Customer(rapidcsv::Document &doc);

#pragma endregion

#pragma region Enumerations

enum CylinderType {
	_12_7kg, _14kg, _50kg
};

#pragma endregion

#pragma region Main function

/***************************************************************
*                                                              *
* FUNCTION NAME: main                                          *
*                                                              *
* ARGUMENTS:                                                   *
*                                                              *
* Argument       Type       IO    Description                  *
* --------       ----       --    -----------                  *
* none                                                         *
*                                                              *
* RETURN VALUE: int                                            *
*                                                              *
***************************************************************/
int main()
{
	bool proceed = Init_Mode();
	if (proceed)
	{
		Command_Mode();
	}
	else 
	{
		cout << "\nExiting..." << endl;
		system("pause");
		return 0;
	}
	system("pause");
	return 0;
}

#pragma endregion

#pragma region Functions

void Command_Mode()
{
	cout << "Logged in. Entering command mode..." << endl;
	Sleep(100);
	char input_buffer[100] = { 0x0 }; // char buffers are easier than string buffers
	string delimiter = " ";

	// Open databases
	rapidcsv::Document customers_db(rapidcsv::Properties("customers.csv", 0, -1, true, ';'));

	cout << "Database loaded" << endl;

	// Formally enter into a REPL environment
	cin.ignore(INT_MAX, '\n'); // unforunately, we need this to flush the buffer
	while (strcmp(input_buffer, "EXIT") != 0) // not equal
	{
		cout << "> ";
		cin.getline(input_buffer, 100, '\n');

		vector<string> arguments = split(input_buffer, delimiter);

		if (arguments.size() == 0) {
			// If the user accidentally pressed enter, we don't want the program to crash
			continue;
		}

		if (arguments.at(0) == "LIST")
		{
			List_Customers(customers_db);
		}
		else if (arguments.at(0) == "HISTORY")
		{
			if (arguments.size() != 2)
				cout << "Invalid command, syntax: HISTORY <customerID>" << endl;
			else
			{
				cout << arguments[1] << endl;
			}
		}
		else if (arguments.at(0) == "DELETE")
		{
			if (arguments.size() != 2)
				cout << "Invalid command, syntax: DELETE <customerID>" << endl;
			else
			{
				cout << arguments[1] << endl;
			}
		}
		else if (arguments.at(0) == "NEW")
		{
			if (arguments.size() != 1)
				cout << "Invalid command, syntax: NEW" << endl;
			else
			{
				Add_Customer(customers_db);
			}
		}
		else if (arguments.at(0) == "ORDER")
		{
			if (arguments.size() != 2)
				cout << "Invalid command, syntax: ORDER <customerID>" << endl;
			else
			{

			}
		}
		else if (arguments.at(0) == "HELP")
		{
			if (arguments.size() != 1)
				cout << "Invalid command, syntax: ORDER <customerID>" << endl;
			else
			{

			}
		}
		else if (arguments.at(0) != "EXIT")
		{
			// Final check to see if it's even a valid command
			cout << "Invalid command" << endl;
		}
	}
	cout << "EXIT command detected. Exiting";
	for (int i = 0; i < 3; i++)
	{
		cout << " .";
		Sleep(300);
	}
	cout << endl;
}

/***************************************************************
*                                                              *
* FUNCTION NAME: Init_Mode                                     *
*                                                              *
* ARGUMENTS:                                                   *
*                                                              *
* Argument       Type       IO    Description                  *
* --------       ----       --    -----------                  *
* none                                                         *
*                                                              *
* RETURN VALUE: bool                                           *
*                                                              *
***************************************************************/
bool Init_Mode()
{
	cout << "--------------------------------------------------------------------------------" << endl;
	cout << " UNO LPG Services Pte. Ltd. Customer and Orders Management Console" << endl;
	cout << " Version 0.1a" << endl;
	cout << "--------------------------------------------------------------------------------" << endl;
	char input_char = '\0';
	while (input_char != 'y' && input_char != 'n')
	{
		cout << "Do you want to log into the console (y/n): ";
		cin >> input_char;
		input_char = tolower(input_char);
		if (input_char != 'y' && input_char != 'n')
		{
			cout << "Invalid command" << endl;
		}
	}
	if (input_char == 'y')
		return true;
	return false;
}

int initial()
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
	// Establishment Cylinder Choice
	int eCylinder;


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
		cout << "Enter your address: ";
		cin >> address;
		
		// Household or Establishment option display
		cout << "Do you want to get LPG for your Household(H/h) or Establisment(E/e)? : ";	
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
			cin >> customer_type;
			// Converting option to Uppercase
			customer_type_up = toupper(customer_type);
		}

		if (customer_type_up == 'E') 
		{
			// Establishment Cylinder Choice
			cout << "Would you like to purchase the 14 Kg or the 50 Kg cylinder? : ";
			// Input for Cylinder Option
			cin >> eCylinder;

			while (1)
			{
				if (eCylinder == 14 || eCylinder == 50)
				{
					// Ends the verification loop
					break;
				}
			}


		}
	}
	
	// Random input to Freeze the Terminal
	cin >> a;																					

	return 0;
}

vector<string> split(const string& str, const string& delim)
{
	vector<string> tokens;
	size_t prev = 0, pos = 0;
	do
	{
		pos = str.find(delim, prev);
		if (pos == string::npos) pos = str.length();
		string token = str.substr(prev, pos - prev);
		if (!token.empty()) tokens.push_back(token);
		prev = pos + delim.length();
	} while (pos < str.length() && prev < str.length());
	return tokens;
}

#pragma endregion

#pragma region Adapter functions

void List_Customers(rapidcsv::Document &doc)
{
	vector<int> customer_ids = doc.GetColumn<int>("id");
	if (customer_ids.size() < 1)
		cout << "No customers" << endl;
	else {
		for (vector<int>::iterator it = customer_ids.begin(); it != customer_ids.end(); ++it) {
			vector<string> customer_details = doc.GetRow<string>(*it);
			cout << "Customer ID: " << customer_details.at(0) << endl;
			if (stoi(customer_details.at(1)) == 0)
				cout << "Type: Household" << endl;
			else
				cout << "Type: Establishment" << endl;
			cout << "Name: " << customer_details.at(2) << endl;
			cout << "Address: " << customer_details.at(3) << endl;
		}
	}
}

void Add_Customer(rapidcsv::Document &doc)
{
	vector<int> customer_ids = doc.GetColumn<int>("id");
	int last_id; // Get the last customer's ID
	if (customer_ids.size() == 0)
	{
		// If the database is empty
		last_id = -1;
	}
	else
	{
		last_id = *(customer_ids.end() -1); // As .end() is after the last elem, we must -1
	}
	
	string customer_name, customer_address;
	int customer_type = INT_MAX;
	cout << "--------------------------------------------------------------------------------" << endl;
	cout << " New customer submission form" << endl;
	cout << "--------------------------------------------------------------------------------" << endl;
	cout << "The customer will be created with ID " << ++last_id << endl;
	cout << "Customer Name: ";
	getline(cin, customer_name);
	while (customer_type != 0 && customer_type != 1)
	{
		cout << "Customer Type (0: Household, 1: Establishment): ";
		string customer_type_str;
		getline(cin, customer_type_str);
	 	customer_type = stoi(customer_type_str);
		if (customer_type != 0 && customer_type != 1)
			cout << "Invalid customer type. Please key in 0 or 1" << endl;
	}
	cout << "Customer Address: ";
	getline(cin, customer_address);

	// Save to database
	// Create row first
	vector<string> row_data;
	row_data.push_back(to_string(last_id));
	row_data.push_back(to_string(customer_type));
	row_data.push_back(customer_name);
	row_data.push_back(customer_address);
	doc.SetRow(last_id, row_data);
	doc.Save();
	cout << "Saved customer" << endl;
}

#pragma endregion
