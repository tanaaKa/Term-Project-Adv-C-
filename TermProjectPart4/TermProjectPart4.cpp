// Term Project - Part 4
// By Troy Billsen
// Adv C++ - CIS3340

#include "sqlite3.h"
#include <iostream>

using namespace std;

// Prototypes
void doSetup();
static int addProperty(const char* s);
static int callback(void* NotUsed, int argc, char** argv, char** azColName);
static int createDB(const char* s);
static int createTable(const char* s);
static int selectData(const char* s);
static int insertData(const char* s, char propType[50], char tenant[50], char owner[50], char value[10]);
// Database
const char* db = "properties.db";
bool created; // checks if db is created

// ------------------------------------------------------------------------
//
void createMenu()
{
	int choice;

	do
	{
		// Title
		cout << "----------------------------" << endl;
		cout << "PROPERTY MANAGER - MAIN MENU" << endl;
		cout << "----------------------------" << endl;

		// Display options
		cout << "1.) Setup database" << endl
			<< "2.) Add property" << endl
			<< "3.) Generate Property List" << endl
			<< "4.) Exit" << endl << endl
			<< "Choose an option:";

		// Grab user choice
		cin >> choice;

		// Call based off-of choices
		if (choice == 1)
			doSetup();
		if (choice == 2)
			if (!created)
				cout << "ERROR: The database needs to be created first!" << endl << endl;
			else
				addProperty(db);
		if (choice == 3)
			if (!created)
				cout << "ERROR: The database needs to be created first!" << endl << endl;
			else
				selectData(db);
	} while (choice != 4);
}

void doSetup()
{
	char answer;
	// Send warning
	cout << "------------------------" << endl;
	cout << "\tWARNING" << endl;
	cout << "------------------------" << endl;
	cout << "Running this setup will attempt to wipe and recreate the database" << endl;
	cout << "Are you sure you wish to continue? (y/n):" << endl;
	cin >> answer;

	if (answer == 'y' || answer == 'Y')
	{
		createDB(db);
		createTable(db);
		created = true;
	}
	else 
		cout << "Returning to main menu" << endl << endl;	
}

// Select data from default table
static int selectData(const char* s)
{
	sqlite3* DB;

	int exit = sqlite3_open(s, &DB);

	string sql = "SELECT * FROM PROPERTY;";

	sqlite3_exec(DB, sql.c_str(), callback, NULL, NULL);

	return 0;
}

// Creates the database as needed
static int createDB(const char* s)
{
	sqlite3* DB;
	int exit = 0;

	exit = sqlite3_open(s, &DB);

	sqlite3_close(DB);

	cout << "Database created successfully in source directory!" << endl;

	return 0;
}

// Display table information
static int callback(void* NotUsed, int argc, char** argv, char** azColname)
{
	for (int i = 0; i < argc; i++)
	{
		cout << azColname[i] << ": " << argv[i] << endl;
	}
	cout << endl;

	return 0;
}

// Creates the property table
static int createTable(const char* s)
{
	sqlite3* DB;

	string sql = "CREATE TABLE IF NOT EXISTS PROPERTY("
		"ID INTEGER PRIMARY KEY AUTOINCREMENT, "
		"PTYPE	TEXT NOT NULL, "
		"TENANT	TEXT NOT NULL, "
		"OWNER	INT NOT NULL, "
		"RENT	REAL NOT NULL);";

	try
	{
		int exit = 0;
		exit = sqlite3_open(s, &DB);

		char* messageError;
		exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);

		if (exit != SQLITE_OK) {
			cerr << "Error creating table within properties.db!" << endl;
			sqlite3_free(messageError);
		}
		else
			cout << "Table created successfully in properties.db!" << endl;
		sqlite3_close(DB);
	}
	catch (const exception & e)
	{
		cerr << e.what();
	}
	return 0;
}

// Insert data passed from the addProperty function
static int insertData(const char* s, char propType[50], char tenant[50], char owner[50], char value[10])
{
	sqlite3* DB;
	char* messageError;
	string p = propType;
	string t = tenant;
	string o = owner;
	string v = value;

	int exit = sqlite3_open(s, &DB);

	// Pass values
	string sql("INSERT INTO PROPERTY (PTYPE, TENANT, OWNER, RENT) VALUES('" + p + "', '" + t + "', '" + o +"', '" + v + "');");

	exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
	if (exit != SQLITE_OK) {
		cerr << "ERROR: Error on inserting into table!" << endl;
		sqlite3_free(messageError);
	}
	else
		cout << endl << "Added to property table successfully!" << endl;

	return 0;
}

static int addProperty(const char* s)
{
	int amount;

	// Values in db
	// Index, property type, tenant, owner, rent amount
	char propType[50];
	char tenant[50];
	char owner[50];
	char value[10];

	cout << "How many properties do you wish to add to the database?" << endl;
	cin >> amount;

	// Loop to enter in info
	for (int i = 0; i < amount; i++)
	{
		cin.ignore();
		// Get property type
		// House, apt, other...
		cout << "Enter in the property type: ";
		cin.getline(propType, sizeof(propType));
		// Get tenant name
		cout << "Enter in the tenant name: ";
		cin.getline(tenant, sizeof(tenant));
		// Get owner name
		cout << "Enter in the owner name: ";
		cin.getline(owner, sizeof(owner));
		// Get value
		cout << "Enter in the monthly rent rate: ";
		cin >> value;
		// Function to add info to DB
		insertData(db, propType, tenant, owner, value);
	}

	// Return to main menu
	cout << "Returning to main menu..." << endl << endl;
	return 0;
}

int main()
{
	sqlite3* DB;

	// Menu loop
	createMenu();

    return 0;
}

