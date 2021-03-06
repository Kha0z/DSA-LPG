// 1617517, 1617658
/**************************************************************
* FILE NAME: LPG_Services.cpp                                 *
*                                                             *
* FILE REFERENCES:                                            *
* Name       I/O            Description                       *
* ----       ---            -----------                       *
* None                                                        *
*                                                             *
* ABNORMAL TERMINATION CONDITIONS, ERRORS AND WARNINGS:       *
*  Abnormal termination may occur when:                       *
*  - customers.csv and/or inventory.csv does not exist        *
*  - Improper data type entry is made by the operator         *
*                                                             *
* ASSUMPTIONS, CONSTRAINTS, RESTRICTIONS:                     *
*  This code assumes that the databases exist. It will crash  *
*  if it does not find the two databases                      *
*                                                             *
* NOTES:                                                      *
*  This program requires manual entry of the CSV databases,   *
*  namely customers.csv and inventory.csv as per spec before  *
*  attempting to run this program.                            *
*															  *
*  This program is compiled with the highest level of         *
*  compiler warnings, W4 (one level down from Wall, which     *
*  displays warnings in dependencies, even STL) with zero     *
*  errors, zero warnings.                                     *
*                                                             *
* REQUIREMENTS/FUNCTIONAL SPECIFICATIONS REFERENCES:          *
*  none                                                       *
*															  *
**************************************************************/

#include "stdafx.h"

#pragma warning(disable: 4996)

using namespace std;
using json = nlohmann::json;

#pragma region Function Prototypes

void Command_Mode();
bool Init_Mode();
vector<string> split(const string& str, const string& delim);

void List_Customers(rapidcsv::Document &doc);
void Add_Customer(rapidcsv::Document &doc);
void Customer_History(rapidcsv::Document &doc, int customer_id_lookup);
void Add_Order(rapidcsv::Document &doc, int customer_id_lookup = -1);
void Print_Receipt(string customer_name, string customer_address, int order1, int order2, int order3, double price);
void Quantity();
void Help();

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

/***************************************************************
*                                                              *
* FUNCTION NAME: Command_Mode                                  *
*															   *
* DESCRIPTION: Initialises the interactive command line of the *
*              software. Handles arguments and exits the       *
*              program upon the EXIT command                   *
*                                                              *
* ARGUMENTS:                                                   *
*                                                              *
* Argument       Type       IO    Description                  *
* --------       ----       --    -----------                  *
* none                                                         *
*                                                              *
* RETURN VALUE: none                                           *
*                                                              *
***************************************************************/
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
				Customer_History(customers_db, stoi(arguments.at(1)));
			}
		}
		else if (arguments.at(0) == "NEW")
		{
			if (arguments.size() != 1)
				cout << "Invalid command, syntax: NEW" << endl;
			else
				Add_Customer(customers_db);
		}
		else if (arguments.at(0) == "ORDER")
		{
			if (arguments.size() != 2)
				cout << "Invalid command, syntax: ORDER <customerID>" << endl;
			else
				Add_Order(customers_db, stoi(arguments.at(1)));
		}
		else if (arguments.at(0) == "QUANTITY")
		{
			if (arguments.size() != 1)
				cout << "Invalid command, syntax: QUANTITY" << endl;
			else
				Quantity();
		}
		else if (arguments.at(0) == "HELP")
		{
			Help();
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
*															   *
* DESCRIPTION: Prints the welcome message and allows the user  *
*			   to log in or cancel the login.                  *
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
		input_char = (char)tolower(input_char);
		if (input_char != 'y' && input_char != 'n')
		{
			cout << "Invalid command" << endl;
		}
	}
	if (input_char == 'y')
		return true;
	return false;
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

/***************************************************************
*                                                              *
* FUNCTION NAME: List_Customers                                *
*															   *
* DESCRIPTION: Lists all customers in the database             *
*                                                              *
* ARGUMENTS:                                                   *
*                                                              *
* Argument       Type       IO    Description                  *
* --------       ----       --    -----------                  *
* &doc           Document   IO    Reference to the database    *
*                                                              *
* RETURN VALUE: none                                           *
*                                                              *
***************************************************************/
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

/***************************************************************
*                                                              *
* FUNCTION NAME: Add_Customer                                  *
*															   *
* DESCRIPTION: Adds a customer to the database                 *
*                                                              *
* ARGUMENTS:                                                   *
*                                                              *
* Argument       Type       IO    Description                  *
* --------       ----       --    -----------                  *
* &doc           Document   IO    Reference to the database    *
*                                                              *
* RETURN VALUE: none                                           *
*                                                              *
***************************************************************/
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
	row_data.push_back("[]"); // create empty history JSON object
	doc.SetRow(last_id, row_data);
	doc.Save();
	cout << "Saved customer" << endl;
}

/***************************************************************
*                                                              *
* FUNCTION NAME: Customer_History                              *
*															   *
* DESCRIPTION: Displays the order history of a customer, down  *
*              to the exact order placed, and order date       *
*                                                              *
* ARGUMENTS:                                                   *
*                                                              *
* Argument       Type       IO    Description                  *
* --------       ----       --    -----------                  *
* &doc           Document   IO    Reference to the database    *
* customer_id    int        I     The customer ID to lookup    *
* _lookup                                                      *
*                                                              *
* RETURN VALUE: none                                           *
*                                                              *
***************************************************************/
void Customer_History(rapidcsv::Document &doc, int customer_id_lookup)
{
	int customer_id = -1; // This is the actual customer ID, -1 if not found
	vector<int> customer_ids = doc.GetColumn<int>("id");
	
	for (vector<int>::iterator iter = customer_ids.begin(); iter != customer_ids.end(); ++iter)
	{
		if (*iter == customer_id_lookup)
			customer_id = *iter;
	}

	// If the customer ID was not found
	if (customer_id == -1)
	{
		cout << "Customer ID " << customer_id_lookup << " not found!" << endl;
		return;
	}

	// Parse JSON
	vector<string> customer_details = doc.GetRow<string>(customer_id);
	auto json_history = json::parse(customer_details.at(4));
	size_t history_size = json_history.size();

	if (history_size < 1)
	{
		cout << "No past order history" << endl;
		return;
	}
	
	cout << "--------------------------------------------------------------------------------" << endl;
	for (size_t i = 0; i < history_size; i++)
	{
		 cout << "12.7kg canisters: " << json_history[i]["12.7kg"] << endl;
		 cout << "14kg canisters: " << json_history[i]["14kg"] << endl;
		 cout << "50kg canisters: " << json_history[i]["50kg"] << endl;
		 // Decode time and print
		 time_t createdDate = json_history[i]["createdDate"];
		 struct tm *tm = localtime(&createdDate);
		 cout << "Date and time created: " << 
			 tm->tm_year+1900 << "-" << tm->tm_mon << "-" << tm->tm_mday <<
			 " " << setfill('0') << setw(2) << tm->tm_hour << ":" << setfill('0') << setw(2) << tm->tm_min << endl;
		 cout << "--------------------------------------------------------------------------------" << endl;
	}
}

/***************************************************************
*                                                              *
* FUNCTION NAME: Add_Order                                     *
*															   *
* DESCRIPTION: Add an order to an existing customer, and       *
*              decrements the inventory                        *
*                                                              *
* ARGUMENTS:                                                   *
*                                                              *
* Argument       Type       IO    Description                  *
* --------       ----       --    -----------                  *
* &doc           Document   IO    Reference to the database    *
* customer_id    int        I     The customer ID to lookup    *
* _lookup                                                      *
*                                                              *
* RETURN VALUE: none                                           *
*                                                              *
***************************************************************/
void Add_Order(rapidcsv::Document &doc, int customer_id_lookup)
{
	/*string customer_id_str;
	int customer_id_lookup; // This is the customer ID to lookup for
	
	cout << "Please key in the customer ID to place an order: ";
	getline(cin, customer_id_str);
	customer_id_lookup = stoi(customer_id_str);*/

	// Looking up customer ID to see if it's found
	int customer_id = -1; // This is the actual customer ID, -1 if not found
	vector<int> customer_ids = doc.GetColumn<int>("id");
	for (vector<int>::iterator iter = customer_ids.begin(); iter != customer_ids.end(); ++iter)
	{
		if (*iter == customer_id_lookup)
			customer_id = *iter;
	}
	// If the customer ID was not found
	if (customer_id == -1)
	{
		cout << "Customer ID " << customer_id_lookup << " not found!" << endl;
		return;
	}

	// Read customer details; previous history into JSON
	vector<string> customer_details = doc.GetRow<string>(customer_id);
	string customer_name = customer_details.at(2);
	string customer_address = customer_details.at(3);
	auto json_history = json::parse(customer_details.at(4));
	size_t history_size = json_history.size();

	// Create string and numerical buffers for user input
	string _12_7kg_qty_str, _14kg_qty_str, _50kg_qty_str;
	int _12_7kg_qty = 0, _14kg_qty = 0, _50kg_qty = 0;

	// Get inventory quantity just to make sure that the customer isn't ordering more
	rapidcsv::Document inventory(rapidcsv::Properties("inventory.csv", -1, -1, true, ';'));
	int inventory_12_7kg, inventory_14kg, inventory_50kg;
	double price_12_7kg, price_14kg, price_50kg;
	inventory_12_7kg = inventory.GetCell<int>(0, 0);
	inventory_14kg = inventory.GetCell<int>(1, 0);
	inventory_50kg = inventory.GetCell<int>(2, 0);

	price_12_7kg = inventory.GetCell<double>(0, 1);
	price_14kg = inventory.GetCell<double>(1, 1);
	price_50kg = inventory.GetCell<double>(2, 1);

	// Prompt user for order input
	cout << "Order mode initialised" << endl;
	cout << "Ordering for: " << customer_name << endl;
	cout << "12.7kg cylinder order qty. (" << inventory_12_7kg << " left in inventory): ";
	getline(cin, _12_7kg_qty_str);
	cout << "14kg cylinder order qty. (" << inventory_14kg << " left in inventory): ";
	getline(cin, _14kg_qty_str);
	cout << "50kg cylinder order qty. (" << inventory_50kg << " left in inventory): ";
	getline(cin, _50kg_qty_str);
	// Convert string to int
	_12_7kg_qty = stoi(_12_7kg_qty_str);
	_14kg_qty = stoi(_14kg_qty_str);
	_50kg_qty = stoi(_50kg_qty_str);

	// Check that the entry does not exceed inventory
	bool exceed1 = false, exceed2 = false, exceed3 = false;
	if (_12_7kg_qty > inventory_12_7kg)
		exceed1 = true;
	if (_14kg_qty > inventory_14kg)
		exceed2 = true;
	if (_50kg_qty > inventory_12_7kg)
		exceed3 = true;
	if (exceed1 || exceed2 || exceed3)
	{
		cout << "Order exceeded inventory! Terminating Order mode..." << endl;
		return;
	}

	// Get current UNIX timestamp
	time_t current_time = time(0);

	// Serialise JSON back into a string for insertion into DB
	json_history[history_size] = 
	{
		{"12.7kg", _12_7kg_qty},
		{"14kg", _14kg_qty},
		{"50kg", _50kg_qty},
		{"createdDate", current_time}
	};

	// Recreate row
	vector<string> row_data;
	row_data.push_back(to_string(customer_id));
	row_data.push_back(customer_details.at(1));
	row_data.push_back(customer_name);
	row_data.push_back(customer_address);
	row_data.push_back(json_history.dump()); // create empty history JSON object
	doc.SetRow(customer_id, row_data);
	doc.Save();
	// cout << "Saved order to database" << endl;

	// Sum price
	double total = 0.0;
	total += _12_7kg_qty * price_12_7kg;
	total += _14kg_qty * price_14kg;
	total += _50kg_qty * price_50kg;

	// Print receipt
	Print_Receipt(customer_name, customer_address, _12_7kg_qty, _14kg_qty, _50kg_qty, total);

	// Subtract inventory
	vector<int> inventory_new;
	inventory_new.push_back(inventory_12_7kg - _12_7kg_qty);
	inventory_new.push_back(inventory_14kg - _14kg_qty);
	inventory_new.push_back(inventory_50kg - _50kg_qty);
	inventory.SetRow(0, inventory_new);
	inventory.Save();
}

/***************************************************************
*                                                              *
* FUNCTION NAME: Print_Receipt                                 *
*															   *
* DESCRIPTION: Prints a receipt to screen for the customer     *
*                                                              *
* ARGUMENTS:                                                   *
*                                                              *
* Argument       Type       IO    Description                  *
* --------       ----       --    -----------                  *
* customer_name  string     I     The name of the customer     *
* customer       string     I     The address of the customer  *
* _address                                                     *
* order1         int        I     Number of orders for 12.7kg  *
* order2         int        I     Number of orders for 14kg    *
* order3         int        I     Number of orders for 50kg    *
* price          double     I     Precomputed price of orders  *
*                                                              *
* RETURN VALUE: none                                           *
*                                                              *
***************************************************************/
void Print_Receipt(string customer_name, string customer_address, int order1, int order2, int order3, double price)
{
	cout << "----------------------------------PRINT START-----------------------------------\n" << endl;

	cout << "--------------------------------------------------------------------------------" << endl;
	//cout << " Date: " << customer_name << endl;
	cout << " Invoice to: " << customer_name << endl;
	cout << " Billing/Delivery Address: " << customer_address << endl;
	cout << "--------------------------------------------------------------------------------" << endl;
	cout << " " << order1 << "x 12.7kg canisters" << endl;
	cout << " " << order2 << "x 14kg canisters" << endl;
	cout << " " << order3 << "x 50kg canisters" << endl;
	cout << "--------------------------------------------------------------------------------" << endl;
	cout << " Total price: $" << fixed << setprecision(2) << price << endl;

	cout << "\n-----------------------------------PRINT END------------------------------------\n" << endl;
}

/***************************************************************
*                                                              *
* FUNCTION NAME: Quantity                                      *
*															   *
* DESCRIPTION: Displays the inventory quantity of the LPG cans *
*              and the corresponding prices                    *
*                                                              *
* ARGUMENTS:                                                   *
*                                                              *
* Argument       Type       IO    Description                  *
* --------       ----       --    -----------                  *
* none														   *
*															   *
* RETURN VALUE: none                                           *
*                                                              *
***************************************************************/
void Quantity()
{
	rapidcsv::Document inventory(rapidcsv::Properties("inventory.csv", -1, -1, true, ';'));
	int qty_12_7kg, qty_14kg, qty_50kg;
	double price_12_7kg, price_14kg, price_50kg;
	qty_12_7kg = inventory.GetCell<int>(0, 0);
	qty_14kg = inventory.GetCell<int>(1, 0);
	qty_50kg = inventory.GetCell<int>(2, 0);

	price_12_7kg = inventory.GetCell<double>(0, 1);
	price_14kg = inventory.GetCell<double>(1, 1);
	price_50kg = inventory.GetCell<double>(2, 1);

	cout << "Quantity check: " << endl;
	cout << "12.7kg: " << qty_12_7kg << endl;
	cout << "14kg: " << qty_14kg << endl;
	cout << "50kg: " << qty_50kg << endl;

	cout << "Price list: " << endl;
	cout << "12.7kg: " << fixed << setprecision(2) << price_12_7kg << endl;
	cout << "14kg: " << fixed << setprecision(2) << price_14kg << endl;
	cout << "50kg: " << fixed << setprecision(2) << price_50kg << endl;
}

/***************************************************************
*                                                              *
* FUNCTION NAME: Help                                          *
*															   *
* DESCRIPTION: Displays the helpfile for the command line      *
*                                                              *
* ARGUMENTS:                                                   *
*                                                              *
* Argument       Type       IO    Description                  *
* --------       ----       --    -----------                  *
* none														   *
*															   *
* RETURN VALUE: none                                           *
*                                                              *
***************************************************************/
void Help()
{
	cout << " Helpfile" << endl;
	cout << "--------------------------------------------------------------------------------" << endl;
	cout << " LIST: Provides a list of all the customers and their information" << endl;
	cout << " HISTORY <customerID>: Displays order history for a specific <customerID>" << endl;
	cout << " NEW: Adds a new customer to the database. Requires some form filling" << endl;
	cout << " ORDER <customerID>: Place a new order for a <customerID>" << endl;
	cout << " QUANTITY: Displays the inventory left and the prices of all products" << endl;
	cout << " HELP: This command that you're running. Displays a helpfile" << endl;
	cout << " EXIT: Exits the console safely" << endl;
}

#pragma endregion
