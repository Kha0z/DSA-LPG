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

	//system("pause");
	//cin >> a;																					// Random input to Freeze the Terminal
}
