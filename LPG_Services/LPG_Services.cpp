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

int initial();

enum CylinderType {
	_12_7kg, _14kg, _50kg
};

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
	initial();
	return 0;
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
	// Establishment Variable
	string establishment;
	// Household Variable
	string household;
	// Establishment Cylinder Choice
	int eCylinder;


	// Option Display to proceed
	cout << "Welcome to UNO LPG Services!" << endl;
	cout << "\nWould you like to proceed? Yes(Y/y) or No(N/n): ";		
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
		cout << "Invalid option! Please Try Again: ";						
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

		// Address Input
		cout << "Enter your address: ";
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
			cout << "Invalid option! Please Try Again: ";
			// User Input
			cin >> customer_type;
			// Converting option to Uppercase
			customer_type_up = toupper(customer_type);
		}

		// If Customer is from an establishment
		if (customer_type_up == 'E') 
		{
			// Establishment Input
			cout << "\nEnter Establishment name: ";
			cin >> establishment;
			
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

				// Ask user to re-enter a valid option
				cout << "Invalid Cylinder Weight! Please Try Again: " << endl;
				// Input for Cylinder Option
				cin >> eCylinder;

			}

			if (eCylinder == 14) 
			{
				cout << "\nYou purchasing the 14 Kg Cylinder " << endl;
				// Verifying from our Database
				cout << "Checking Stocks..." << endl;
				cout << "Transaction done by " << name << ", from " << establishment<< " it will be delivered to" << address << endl;
				cout << "\nThank you, see you again!" << endl;
				cout << "Press any key to exit. . ." << endl;
				// Random input to Freeze the Terminal
				cin >> a;

				return 0;
			}

			if (eCylinder == 50)
			{
				cout << "\nYou are purchasing the 50 Kg Cylinder " << endl;
				// Verifying from our Database
				cout << "Checking Stocks..." << endl;
				cout << "Transaction done by " << name << ", from " << establishment << " it will be delivered to" << address << endl;
				cout << "\nThank you, see you again!" << endl;
				cout << "Press any key to exit. . ." << endl;
				// Random input to Freeze the Terminal
				cin >> a;

				return 0;
			}
		}

		// If Customer is from a household
		if (customer_type_up == 'H')
		{
			// Household Input
			cout << "\nEnter Household name: " << endl;
			cin >> household;
			cout << "\nYou are purchasing the 12.7 Kg Cylinder " << endl;
			// Verifying from our Database
			cout << "Checking Stocks..." << endl;
			cout << "Transaction done by " << name << ", from " << household << " it will be delivered to " << address << endl;
			cout << "\nThank you, see you again!" << endl;
			cout << "Press any key to exit. . ." << endl;
			// Random input to Freeze the Terminal
			cin >> a;

			return 0;
		}
	}
	
	// Random input to Freeze the Terminal
	cin >> a;																					

	return 0;
}
