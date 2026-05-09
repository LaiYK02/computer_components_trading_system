#include<iostream>
#include<iomanip>
#include<mysql/jdbc.h>	//For link database
#include<stdlib.h>
#include<thread>		//For waiting
#include<chrono>
#include<string>
#include<cstdlib>		//For system("CLS")
#include<sstream>
#include<conio.h>		//For input *
#include<vector>
#include"UserInterface.h"

using namespace std;

int choice, min_choice, max_choice, counts, current_users_count, max_users_count, current_cc_count, max_cc_count, current_order_count, max_order_count, month, year;
string scene, dot = ".", user_id, email, email_password, email_username, email_role, date_registered, temp_statement = "", search_string = "", search_type = "", search_order = "";
bool user_password, next_page, previous_page;
UserInterface UI;
colors Colors;

const string cctdb = "computercomponentstradingdb";
const string server = "tcp://127.0.0.1:3306";
const string username = "root";
const string password = "";

sql::Driver* driver;
sql::Connection* con;

int main();

void InputValidation();

void LoginUser();
bool ReadEmail();
string InputPassword();
void ReadPassword();
void RegisterUser();
string GetCurrentDate();
bool isAllDigits(const string& s);

void ManageUsers();
void ManageCCInventory();
string GenerateComponentID(string& category);
void OrderProcessing();
void OrderTrendAnalysis();
void MonthlySalesAnalysis();
void AnnualSalesReport();

void SearchItems();
void PlaceOrder();
void ViewOrderHistory();

class systems {
public:
	void AdminSystem()
	{
		bool exit_AdminSystem = false;
		while (!exit_AdminSystem)
		{
			UI.AdminUI(scene, min_choice, max_choice, email_username);
			InputValidation();
			scene = "";
			switch (choice)
			{
			case 0:
				exit_AdminSystem = true;
				main();
				break;
			case 1:
				ManageUsers();
				break;
			case 2:
				ManageCCInventory();
				break;
			case 3:
				OrderProcessing();
				break;
			case 4:
				OrderTrendAnalysis();
				break;
			case 5:
				MonthlySalesAnalysis();
				break;
			case 6:
				AnnualSalesReport();
				break;
			}
		}
	}

	void CustomerSystem()
	{
		bool exit_CustomerSystem = false;

		current_cc_count = 0;
		max_cc_count = 10;
		search_string = "";
		search_type = "";
		next_page = false;
		previous_page = false;

		while (!exit_CustomerSystem)
		{
			UI.CustomerUI(con, scene, min_choice, max_choice, email_username,
				current_cc_count, max_cc_count, next_page, previous_page, search_string, search_type);

			InputValidation();
			scene = "";

			switch (choice)
			{
			case 0:
				exit_CustomerSystem = true;
				main();
				break;

			case 1:
				current_cc_count = 0;
				SearchItems();
				break;

			case 2:
				PlaceOrder();
				break;

			case 3:
				ViewOrderHistory();
				break;

			case 4:
				if (next_page)
					max_cc_count += 10;
				else
				{
					current_cc_count -= 10;
					max_cc_count -= 10;
				}
				break;

			case 5:
				if (previous_page)
				{
					current_cc_count -= 10;
					max_cc_count -= 10;
				}
				break;
			}
		}
	}
};

systems System;

int main()
{
	email = "";
	email_password = "";
	user_password = false;
	////Connection////

	try
	{
		driver = get_driver_instance();
		con = driver->connect(server, username, password); // Create connection
		con->setSchema(cctdb); // Set the schema
		//cout << "Database connection successful!" << endl;
	}
	catch (sql::SQLException& e)
	{
		system("CLS");
		cout << "Connecting to " << cctdb << dot << endl;
		dot = dot + ".";
		if (dot.length() > 3)
		{
			dot = ".";
			cout << Colors.ColorSet("91") << "\nError message: " << e.what() << Colors.ColorReset();
		}
		this_thread::sleep_for(chrono::milliseconds(400));
		main();	
		exit(1);
	}
	////Connection////

	bool exit_MainMenu = false;
	while (!exit_MainMenu)
	{
		UI.MainMenu(scene, min_choice, max_choice);
		InputValidation();
		scene = "";
		switch (choice)
		{
		case 1:
			LoginUser();
			break;
		case 2:
			RegisterUser();
			break;
		case 3:
			UI.PrintHeader("Goodbye");
			cout << endl;
			cout << "See you next time!" << endl << endl;
			this_thread::sleep_for(chrono::seconds(2));

			exit_MainMenu = true;
			delete con;
			exit(1);
		}
	}

	delete con;

	return 0;
}

void InputValidation()
{
	string temp;
	bool exit = false;
	while (!exit)
	{
		getline(cin, temp);
		while (temp.length() != 1 || !isdigit(temp[0]))
		{
			cout << Colors.ColorSet("91") << "\nError: Invalid choice!" << Colors.ColorReset();
			this_thread::sleep_for(chrono::seconds(2));
			//Get the answer again
			if (scene == "MainMenu")
				UI.MainMenu(scene, min_choice, max_choice);
			else if(scene == "AdminUI")
				UI.AdminUI(scene, min_choice, max_choice, email_username);
			else if (scene == "CustomerUI")
				UI.CustomerUI(con, scene, min_choice, max_choice, email_username, current_cc_count, max_cc_count, next_page, previous_page, search_string, search_type);
			else if (scene == "ManageUsersUI")
				UI.ManageUsersUI(con, scene, min_choice, max_choice, email_username, current_users_count, max_users_count, next_page, previous_page, temp_statement);
			else if (scene == "ManageCCInventoryUI")
				UI.ManageCCInventoryUI(con, scene, min_choice, max_choice, email_username, current_cc_count, max_cc_count, next_page, previous_page, search_string, search_type);
			else if (scene == "OrderProcessingUI")
				UI.OrderProcessingUI(con, scene, min_choice, max_choice, email_username, current_order_count,
					max_order_count, next_page, previous_page, search_order);
			else if (scene == "OrderTrendAnalysisUI")
				UI.OrderTrendAnalysisUI(con, scene, min_choice, max_choice, email_username, month, year);
			else if (scene == "AnnualSalesReportUI")
				UI.AnnualSalesReportUI(con, scene, min_choice, max_choice, email_username, year);

			getline(cin, temp);
		}
		choice = stoi(temp);
		if (choice < min_choice || choice > max_choice)
		{
			cout << Colors.ColorSet("91") << "\nError: Invalid choice!" << Colors.ColorReset();
			this_thread::sleep_for(chrono::seconds(2));
			//Get the answer again
			if (scene == "MainMenu")
				UI.MainMenu(scene, min_choice, max_choice);
			else if (scene == "AdminUI")
				UI.AdminUI(scene, min_choice, max_choice, email_username);
			else if (scene == "CustomerUI")
				UI.CustomerUI(con, scene, min_choice, max_choice, email_username, current_cc_count, max_cc_count, next_page, previous_page, search_string, search_type);
			else if (scene == "ManageUsersUI")
				UI.ManageUsersUI(con, scene, min_choice, max_choice, email_username, current_users_count, max_users_count, next_page, previous_page, temp_statement);
			else if (scene == "ManageCCInventoryUI")
				UI.ManageCCInventoryUI(con, scene, min_choice, max_choice, email_username, current_cc_count, max_cc_count, next_page, previous_page, search_string, search_type);
			else if (scene == "OrderProcessingUI")
				UI.OrderProcessingUI(con, scene, min_choice, max_choice, email_username, current_order_count,
					max_order_count, next_page, previous_page, search_order);
			else if (scene == "OrderTrendAnalysisUI")
				UI.OrderTrendAnalysisUI(con, scene, min_choice, max_choice, email_username, month, year);

		}
		else
			exit = true;
	}
}

void LoginUser()
{
	while(!ReadEmail())
	{
		UI.PrintHeader("");
		cout << "\nEnter your email (0 for Exit): ";
		getline(cin, email);
		if (email != "0")
		{
			if (!ReadEmail())
			{
				cout << Colors.ColorSet("91") << "\nError: Account doesn't exist!" << Colors.ColorReset();
				this_thread::sleep_for(chrono::seconds(2));
			}
		}
		else
			main();
	}

	while (!user_password)
	{
		UI.PrintHeader("");
		cout << "\nEnter your password (0 for Exit): ";
		email_password = InputPassword();
		if (email_password != "0")
		{
			ReadPassword();
			if (!user_password)
			{
				cout << Colors.ColorSet("91") << "\nError: Password incorrect!" << Colors.ColorReset();
				this_thread::sleep_for(chrono::seconds(2));
			}
		}
		else
			main();
	}

	UI.PrintHeader("");
	cout << "\nLogin successful! Welcome, " << email_username << "!";
	this_thread::sleep_for(chrono::seconds(2));

	if (email_role == "Admin")
		System.AdminSystem();
	else if (email_role == "Customer")
		System.CustomerSystem();
}

bool ReadEmail()
{
	try
	{
		sql::Statement* stmt;
		sql::ResultSet* res;
		stmt = con->createStatement();
		res = stmt->executeQuery("SELECT * FROM users");
		while (res->next()) {
			if (email == res->getString("Email"))
				return true;
		}
		delete res;
		delete stmt;
		return false;
	}
	catch (sql::SQLException& e)
	{
		cout << Colors.ColorSet("91") << "\nError retrieving users: " << e.what() << endl << Colors.ColorReset();
	}
}

string InputPassword()
{
	string input;
	char ch;

	while ((ch = _getch()) != '\r')		// '\r' = Enter/Return key
	{
		if (ch == '\b') {				// '\b' = Backspace
			if (!input.empty()) {
				cout << "\b \b";		// erase last '*'
				input.pop_back();
			}
		}
		else {
			input.push_back(ch);
			cout << '*';				// show '*' instead of the real character
		}
	}
	cout << endl;
	return input;
}

void ReadPassword()
{
	try
	{
		sql::Statement* stmt;
		sql::ResultSet* res;
		stmt = con->createStatement();
		res = stmt->executeQuery("SELECT * FROM users");
		while (res->next()) {
			if (email == res->getString("Email"))
			{
				if (email_password == res->getString("Password"))
				{
					user_password = true;
					user_id = res->getString("UserID");
					email_username = res->getString("Name");
					email_role = res->getString("Role");
				}
			}
		}
		delete res;
		delete stmt;
	}
	catch (sql::SQLException& e)
	{
		cout << Colors.ColorSet("91") << "\nError retrieving users: " << e.what() << endl << Colors.ColorReset();
	}
}

void RegisterUser()
{
	bool email_register = false;
	while(!email_register)
	{
		UI.PrintHeader("");	//Enter email
		cout << endl;
		cout << "Enter your email (0 for Exit): ";
		getline(cin, email);
		while (email.find('@') == string::npos && email != "0")
		{
			cout << Colors.ColorSet("91") << "\nError: Invalid email format." << Colors.ColorReset();
			this_thread::sleep_for(chrono::seconds(2));
			UI.PrintHeader("");
			cout << endl;
			cout << "Enter your email (0 for Exit): ";
			getline(cin, email);
		}

		if (email == "0")
			main();

		if (!ReadEmail())
			email_register = true;
		else
		{
			cout << Colors.ColorSet("91") << "\nError: Email already registered!" << Colors.ColorReset();
			this_thread::sleep_for(chrono::seconds(2));
		}
	}

	bool password_enter = false;
	while(!password_enter)
	{
		system("CLS");	//Enter password
		cout << "======================================================" << endl;
		cout << "|                                                    |" << endl;
		cout << "|         " << Colors.ColorSet("96") << "Computer Components Trading System" << Colors.ColorReset() << "         |" << endl;
		cout << "|                                                    |" << endl;
		cout << "======================================================" << endl;
		cout << endl;
		cout << "Enter your password: ";
		getline(cin, email_password);
		while (email_password.length() < 6)
		{
			cout << Colors.ColorSet("91") << "\nError: The password must be a least 6 characters." << Colors.ColorReset();
			this_thread::sleep_for(chrono::seconds(2));
			system("CLS");
			cout << "======================================================" << endl;
			cout << "|                                                    |" << endl;
			cout << "|         " << Colors.ColorSet("96") << "Computer Components Trading System" << Colors.ColorReset() << "         |" << endl;
			cout << "|                                                    |" << endl;
			cout << "======================================================" << endl;
			cout << endl;
			cout << "Enter your password: ";
			getline(cin, email_password);
		}
		string confirm_password;
		cout << "Confirm your password (0 for Return): ";
		getline(cin, confirm_password);

		while (confirm_password != email_password && confirm_password != "0")
		{
			cout << Colors.ColorSet("91") << "\nError: Make sure the passwords are same!" << Colors.ColorReset();
			this_thread::sleep_for(chrono::seconds(2));
			system("CLS");
			cout << "======================================================" << endl;
			cout << "|                                                    |" << endl;
			cout << "|         " << Colors.ColorSet("96") << "Computer Components Trading System" << Colors.ColorReset() << "         |" << endl;
			cout << "|                                                    |" << endl;
			cout << "======================================================" << endl;
			cout << endl;
			cout << "Enter your password: " << email_password << endl;
			cout << "Confirm your password (0 for Return): ";
			getline(cin, confirm_password);
		}
		if(confirm_password != "0") password_enter = true;
	}

	UI.PrintHeader("");
	cout << endl;
	cout << "Enter your name: ";
	getline(cin, email_username);
	while (email_username == "")
	{
		cout << Colors.ColorSet("91") << "\nError: The name cannot be empty!" << Colors.ColorReset();
		this_thread::sleep_for(chrono::seconds(2));
		UI.PrintHeader("");
		cout << endl;
		cout << "Enter your name: ";
		getline(cin, email_username);
	}

	email_role = "Customer";

	//Get the User ID//
	counts = 1;
	string temp_str;
	int temp_num;
	bool exit = false;
	try
	{
		sql::Statement* stmt;
		sql::ResultSet* res;
		stmt = con->createStatement();
		res = stmt->executeQuery("SELECT UserID FROM users WHERE UserID LIKE 'C%' ORDER BY UserID ASC");
		while (res->next()) {
			temp_str = res->getString("UserID");
			temp_str = temp_str.substr(1);
			temp_num = stoi(temp_str);

			if (temp_num != counts)
			{
				break;
			}
			counts++;
		}
		stringstream ss;
		ss << "C" << setfill('0') << setw(4) << counts;		//Generate the user id like C0001
		user_id = ss.str();

		delete res;
		delete stmt;
	}
	catch (sql::SQLException& e)
	{
		cout << Colors.ColorSet("91") << "\nError retrieving users: " << e.what() << endl << Colors.ColorReset();
	}
	//Get the User ID//

	date_registered = GetCurrentDate();	//Save current date of register

	try {	//Inserting all the data
		sql::PreparedStatement* pstmt;
		pstmt = con->prepareStatement("INSERT INTO users (UserID, Email, Password, Name, Role, DateRegistered) VALUES (?, ?, ?, ?, ?, ?)");
		pstmt->setString(1, user_id);
		pstmt->setString(2, email);
		pstmt->setString(3, email_password);
		pstmt->setString(4, email_username);
		pstmt->setString(5, email_role);
		pstmt->setString(6, date_registered);
		pstmt->executeUpdate();

		UI.PrintHeader("");
		cout << "\nAccount successfully registered!";
		this_thread::sleep_for(chrono::seconds(3));
		delete pstmt;
		main();
	}
	catch (sql::SQLException& e) {
		cout << Colors.ColorSet("91") << "Error: " << e.what() << endl << Colors.ColorReset();
	}
}

string GetCurrentDate() {
	// Get current time from system clock
	auto now = chrono::system_clock::now();
	// Convert to time_t (calendar time)
	time_t t = chrono::system_clock::to_time_t(now);
	// Convert to tm structure for local time
	tm localTime;
#ifdef _WIN32
	localtime_s(&localTime, &t);  // Windows
#else
	localtime_r(&t, &localTime);  // Linux/Mac
#endif
	// Format as yyyy-mm-dd
	stringstream ss;
	ss << put_time(&localTime, "%Y-%m-%d");
	return ss.str();
}

vector<string> WrapText(const string& text, size_t width)
{
	vector<string> lines;

	if (text.empty()) {
		lines.push_back("");
		return lines;
	}

	string word;
	string line = "";
	stringstream ss(text);

	while (ss >> word) {
		if (line.length() + word.length() + 1 > width)
		{
			lines.push_back(line);
			line = word;
		}
		else
		{
			if (line.empty())
				line = word;
			else
				line += " " + word;
		}
	}

	if (!line.empty())
		lines.push_back(line);

	return lines;
}

bool isAllDigits(const string& s)
{
	if (s.empty())
		return false;

	for (char c : s)
	{
		if (!isdigit(c))
			return false;
	}
	return true;
}

void ManageUsers()
{
	current_users_count = 0, max_users_count = 10;
	temp_statement = "";
	bool exit_ManageUsers = false;
	next_page = false, previous_page = false;
	while(!exit_ManageUsers)
	{
		UI.ManageUsersUI(con, scene, min_choice, max_choice, email_username, current_users_count, max_users_count, next_page, previous_page, temp_statement);
		InputValidation();
		scene = "";
		if (choice == 1)	//Search
		{
			UI.PrintHeader("Users Management Menu", "Admin", email_username);
			cout << endl;
			string temp_searchName;
			cout << "Enter the User's Name to search (0 for Reset): ";
			getline(cin, temp_searchName);
			if (temp_searchName != "0")
			{
				temp_statement = temp_searchName;
				current_users_count = 0;
				max_users_count = 10;
				next_page = false;
				previous_page = false;
			}
			else
				temp_statement = "";
		}
		else if (choice == 2)	//Add user
		{
			bool email_register = false;
			while (!email_register)
			{
				UI.PrintHeader("");	//Enter email
				cout << endl;
				cout << "Enter email (0 for Exit): ";
				getline(cin, email);
				if (email.find('@') != string::npos)
				{
					if (!ReadEmail())
					{
						bool password_enter = false;
						while (!password_enter)
						{
							UI.PrintHeader("");	//Enter password
							cout << endl;
							cout << "Create password (0 to Return): ";
							getline(cin, email_password);
							if (email_password == "0")
								password_enter = true;
							else if (email_password.length() < 6 && email_password != "0")
							{
								cout << Colors.ColorSet("91") << "\nError: The password must be a least 6 characters." << Colors.ColorReset();
								this_thread::sleep_for(chrono::seconds(2));
							}
							else if (email_password.length() >= 6 && email_password != "0")
							{
								bool exit_confirm = false;
								string confirm_password;
								while (!exit_confirm)
								{
									UI.PrintHeader("");
									cout << endl;
									cout << "Create password: " << email_password << endl;
									cout << "\nConfirm password (0 to Return): ";
									getline(cin, confirm_password);

									if (confirm_password == email_password && confirm_password != "0")
									{
										UI.PrintHeader("");
										cout << endl;
										string temp_email_username;
										cout << "Enter name: ";
										getline(cin, temp_email_username);
										while (temp_email_username == "")
										{
											cout << Colors.ColorSet("91") << "\nError: The name cannot be empty!" << Colors.ColorReset();
											this_thread::sleep_for(chrono::seconds(2));
											UI.PrintHeader("");
											cout << endl;
											cout << "Enter name: ";
											getline(cin, temp_email_username);
										}

										bool role_enter = false;
										while (!role_enter)
										{
											UI.PrintHeader("");
											cout << endl;
											cout << "Enter the role (Admin/Customer): ";
											getline(cin, email_role);
											if (email_role == "Admin" || email_role == "Customer")
												role_enter = true;
											else
											{
												cout << Colors.ColorSet("91") << "\nError: Please enter 'Admin' or 'Customer'!" << Colors.ColorReset();
												this_thread::sleep_for(chrono::seconds(2));
											}
										}

										//Get the User ID//
										counts = 1;
										string temp_str;
										int temp_num;
										bool exit = false;
										try
										{
											sql::Statement* stmt;
											sql::ResultSet* res;
											stmt = con->createStatement();
											res = stmt->executeQuery("SELECT UserID FROM users");
											if (email_role == "Admin")
												res = stmt->executeQuery("SELECT UserID FROM users WHERE UserID LIKE 'A%' ORDER BY UserID ASC");
											else if (email_role == "Customer")
												res = stmt->executeQuery("SELECT UserID FROM users WHERE UserID LIKE 'C%' ORDER BY UserID ASC");
											while (res->next()) {
												temp_str = res->getString("UserID");
												temp_str = temp_str.substr(1);
												temp_num = stoi(temp_str);

												if (temp_num != counts)
												{
													break;
												}
												counts++;
											}
											stringstream ss;
											if (email_role == "Admin")
												ss << "A" << setfill('0') << setw(4) << counts;		//Generate the user id like A0001
											else if (email_role == "Customer")
												ss << "C" << setfill('0') << setw(4) << counts;		//Generate the user id like C0001
											user_id = ss.str();

											delete res;
											delete stmt;
										}
										catch (sql::SQLException& e)
										{
											cout << Colors.ColorSet("91") << "\nError retrieving users: " << e.what() << endl << Colors.ColorReset();
											system("pause");
										}
										//Get the User ID//

										date_registered = GetCurrentDate();	//Save current date of register

										try
										{	//Inserting all the data
											sql::PreparedStatement* pstmt;
											pstmt = con->prepareStatement("INSERT INTO users (UserID, Email, Password, Name, Role, DateRegistered) VALUES (?, ?, ?, ?, ?, ?)");
											pstmt->setString(1, user_id);
											pstmt->setString(2, email);
											pstmt->setString(3, email_password);
											pstmt->setString(4, temp_email_username);
											pstmt->setString(5, email_role);
											pstmt->setString(6, date_registered);
											pstmt->executeUpdate();

											UI.PrintHeader("");
											cout << endl;
											cout << "Account successfully added!";
											this_thread::sleep_for(chrono::seconds(3));
											delete pstmt;
										}
										catch (sql::SQLException& e)
										{
											cout << Colors.ColorSet("91") << "Error: " << e.what() << endl << Colors.ColorReset();
											system("pause");
										}

										exit_confirm = true;
										password_enter = true;
										email_register = true;
									}
									else if (confirm_password != email_password && confirm_password != "0")
									{
										cout << Colors.ColorSet("91") << "\nError: Make sure the passwords are same!" << Colors.ColorReset();
										this_thread::sleep_for(chrono::seconds(2));
									}
									else if (confirm_password == "0")
										exit_confirm = true;
								}
							}
						}
					}
					else
					{
						cout << Colors.ColorSet("91") << "\nError: Email already registered!" << Colors.ColorReset();
						this_thread::sleep_for(chrono::seconds(2));
					}
				}
				else if (email == "0")
					email_register = true;
				else
				{
					cout << Colors.ColorSet("91") << "\nError: Invalid email format." << Colors.ColorReset();
					this_thread::sleep_for(chrono::seconds(2));
				}
			}
		}
		else if (choice == 3)	//Delete user
		{
			bool exit = false;
			while(!exit)
			{
				UI.PrintHeader("Users Management Menu", "Admin", email_username);
				cout << endl;
				UI.ShowUsersTable(con, current_users_count, max_users_count, next_page, previous_page, temp_statement);
				string temp_UserID;
				cout << "\nEnter the User ID to delete (0 for Exit): ";
				getline(cin, temp_UserID);
				if (temp_UserID != "0")
				{
					try
					{
						sql::PreparedStatement* pstmt = con->prepareStatement("SELECT * FROM users WHERE UserID = ?");
						pstmt->setString(1, temp_UserID);
						sql::ResultSet* res = pstmt->executeQuery();

						if (res->next())
						{
							bool exit_delete = false;
							while (!exit_delete)
							{
								UI.PrintHeader("Users Management Menu", "Admin", email_username);
								cout << endl;
								cout << string(155, '-') << endl << endl;
								cout << left << setw(15) << "UserID" << ": " << res->getString("UserID") << endl << endl;
								cout << left << setw(15) << "Name" << ": " << res->getString("Name") << endl << endl;
								cout << left << setw(15) << "Email" << ": " << res->getString("Email") << endl << endl;
								cout << left << setw(15) << "Password" << ": " << res->getString("Password") << endl << endl;
								cout << left << setw(15) << "Role" << ": " << res->getString("Role") << endl << endl;
								cout << left << setw(15) << "Status" << ": " << res->getString("Status") << endl << endl;
								cout << left << setw(15) << "Date Registered" << ": " << res->getString("DateRegistered") << endl << endl;
								cout << string(155, '-') << endl << endl;
								string answer;
								if (email == res->getString("Email"))
								{
									cout << Colors.ColorSet("91") << "WARNING: You are the owner of this account!" << endl << endl;
								}
								cout << Colors.ColorSet("91") << "WARNING" << Colors.ColorReset() << ": Do you want to delete everything of this user? (Y/N): ";
								getline(cin, answer);
								if (answer == "Y" || answer == "y")
								{
									try
									{
										// Delete related orders
										sql::PreparedStatement* delOrders = con->prepareStatement("DELETE FROM orders WHERE UserID = ?");
										delOrders->setString(1, temp_UserID);
										delOrders->executeUpdate();
										delete delOrders;

										// Now delete the user
										sql::PreparedStatement* delUser = con->prepareStatement("DELETE FROM users WHERE UserID = ?");
										delUser->setString(1, temp_UserID);
										delUser->executeUpdate();
										delete delUser;

										cout << "\nUser with ID " << temp_UserID << " has been deleted successfully.";
										this_thread::sleep_for(chrono::seconds(2));
										if (email == res->getString("Email"))
										{
											main();
										}
									}
									catch (sql::SQLException& e) {
										cout << Colors.ColorSet("91") << "\nError deleting user: " << e.what() << endl << Colors.ColorReset();
									}
									exit_delete = true;
								}
								else if (answer == "N" || answer == "n")
									exit_delete = true;
								else
								{
									cout << Colors.ColorSet("91") << "\nError: Invalid choice!" << Colors.ColorReset();
									this_thread::sleep_for(chrono::seconds(2));
								}
							}
						}
						else
						{
							cout << Colors.ColorSet("91") << "\nError: User ID not found." << Colors.ColorReset();
							this_thread::sleep_for(chrono::seconds(2));
						}
						delete res;
						delete pstmt;
					}
					catch (sql::SQLException& e)
					{
						cout << Colors.ColorSet("91") << "\nError retrieving users: " << e.what() << endl << Colors.ColorReset();
					}
				}
				else
					exit = true;
			}
		}
		else if (choice == 4)	//Edit user
		{
			bool exit = false;
			while (!exit)
			{
				UI.PrintHeader("Users Management Menu", "Admin", email_username);
				cout << endl;
				UI.ShowUsersTable(con, current_users_count, max_users_count, next_page, previous_page, temp_statement);
				string temp_UserID;
				cout << "\nEnter the User ID to edit (0 for Exit): ";
				getline(cin, temp_UserID);
				if (temp_UserID != "0")
				{
					try
					{
						sql::PreparedStatement* check_user = con->prepareStatement("SELECT * FROM users WHERE UserID = ?");
						check_user->setString(1, temp_UserID);
						sql::ResultSet* temp_res = check_user->executeQuery();

						if (temp_res->next())
						{
							bool exit_edit = false;
							while (!exit_edit)
							{
								sql::PreparedStatement* pstmt = con->prepareStatement("SELECT * FROM users WHERE UserID = ?");
								pstmt->setString(1, temp_UserID);
								sql::ResultSet* res = pstmt->executeQuery();
								res->next();

								UI.PrintHeader("Users Management Menu", "Admin", email_username);
								cout << endl;
								cout << string(155, '-') << endl << endl;
								cout << Colors.ColorSet("33") << "[1] " << Colors.ColorReset() << left << setw(15) << "UserID" << ": " << res->getString("UserID") << endl << endl;
								cout << Colors.ColorSet("33") << "[2] " << Colors.ColorReset() << left << setw(15) << "Name" << ": " << res->getString("Name") << endl << endl;
								cout << Colors.ColorSet("33") << "[3] " << Colors.ColorReset() << left << setw(15) << "Email" << ": " << res->getString("Email") << endl << endl;
								cout << Colors.ColorSet("33") << "[4] " << Colors.ColorReset() << left << setw(15) << "Password" << ": " << res->getString("Password") << endl << endl;
								cout << Colors.ColorSet("33") << "[5] " << Colors.ColorReset() << left << setw(15) << "Role" << ": " << res->getString("Role") << endl << endl;
								cout << Colors.ColorSet("33") << "[6] " << Colors.ColorReset() << left << setw(15) << "Status" << ": " << res->getString("Status") << endl << endl;
								cout << Colors.ColorSet("33") << "[7] " << Colors.ColorReset() << left << setw(15) << "Date Registered" << ": " << res->getString("DateRegistered") << endl << endl;
								cout << string(155, '-') << endl << endl;
								cout << "Your choice (0 for Exit): ";
								min_choice = 0;
								max_choice = 7;

								string temp;
								bool exit_validation = false;
								while (!exit_validation)
								{
									getline(cin, temp);
									while (temp.length() != 1 || !isdigit(temp[0]))
									{
										cout << Colors.ColorSet("91") << "\nError: Invalid choice!" << Colors.ColorReset();
										this_thread::sleep_for(chrono::seconds(2));
										//Get the answer again
										UI.PrintHeader("Users Management Menu", "Admin", email_username);
										cout << endl;
										cout << string(155, '-') << endl << endl;
										cout << Colors.ColorSet("33") << "[1] " << Colors.ColorReset() << left << setw(15) << "UserID" << ": " << res->getString("UserID") << endl << endl;
										cout << Colors.ColorSet("33") << "[2] " << Colors.ColorReset() << left << setw(15) << "Name" << ": " << res->getString("Name") << endl << endl;
										cout << Colors.ColorSet("33") << "[3] " << Colors.ColorReset() << left << setw(15) << "Email" << ": " << res->getString("Email") << endl << endl;
										cout << Colors.ColorSet("33") << "[4] " << Colors.ColorReset() << left << setw(15) << "Password" << ": " << res->getString("Password") << endl << endl;
										cout << Colors.ColorSet("33") << "[5] " << Colors.ColorReset() << left << setw(15) << "Role" << ": " << res->getString("Role") << endl << endl;
										cout << Colors.ColorSet("33") << "[6] " << Colors.ColorReset() << left << setw(15) << "Status" << ": " << res->getString("Status") << endl << endl;
										cout << Colors.ColorSet("33") << "[7] " << Colors.ColorReset() << left << setw(15) << "Date Registered" << ": " << res->getString("DateRegistered") << endl << endl;
										cout << string(155, '-') << endl << endl;
										cout << "Your choice (0 for Exit): ";
										getline(cin, temp);
									}
									choice = stoi(temp);
									if (choice < min_choice || choice > max_choice)
									{
										cout << Colors.ColorSet("91") << "\nError: Invalid choice!" << Colors.ColorReset();
										this_thread::sleep_for(chrono::seconds(2));
										//Get the answer again
										UI.PrintHeader("Users Management Menu", "Admin", email_username);
										cout << endl;
										cout << string(155, '-') << endl << endl;
										cout << Colors.ColorSet("33") << "[1] " << Colors.ColorReset() << left << setw(15) << "UserID" << ": " << res->getString("UserID") << endl << endl;
										cout << Colors.ColorSet("33") << "[2] " << Colors.ColorReset() << left << setw(15) << "Name" << ": " << res->getString("Name") << endl << endl;
										cout << Colors.ColorSet("33") << "[3] " << Colors.ColorReset() << left << setw(15) << "Email" << ": " << res->getString("Email") << endl << endl;
										cout << Colors.ColorSet("33") << "[4] " << Colors.ColorReset() << left << setw(15) << "Password" << ": " << res->getString("Password") << endl << endl;
										cout << Colors.ColorSet("33") << "[5] " << Colors.ColorReset() << left << setw(15) << "Role" << ": " << res->getString("Role") << endl << endl;
										cout << Colors.ColorSet("33") << "[6] " << Colors.ColorReset() << left << setw(15) << "Status" << ": " << res->getString("Status") << endl << endl;
										cout << Colors.ColorSet("33") << "[7] " << Colors.ColorReset() << left << setw(15) << "Date Registered" << ": " << res->getString("DateRegistered") << endl << endl;
										cout << string(155, '-') << endl << endl;
										cout << "Your choice (0 for Exit): ";
									}
									else
										exit_validation = true;
								}

								if (choice == 1)	//Edit UserID
								{
									cout << Colors.ColorSet("91") << "\nError: UserID cannot be edited because it is Primary Key!" << Colors.ColorReset();
									this_thread::sleep_for(chrono::seconds(2));
								}
								else if (choice == 2)	//Edit Name
								{
									bool exit_name = false;
									while(!exit_name)
									{
										UI.PrintHeader("Users Management Menu", "Admin", email_username);
										cout << endl;
										cout << string(155, '-') << endl << endl;
										cout << left << setw(19) << "[1] UserID" << ": " << res->getString("UserID") << endl << endl;
										cout << left << setw(19) << "[2] Name" << ": " << res->getString("Name") << endl << endl;
										cout << left << setw(19) << "[3] Email" << ": " << res->getString("Email") << endl << endl;
										cout << left << setw(19) << "[4] Password" << ": " << res->getString("Password") << endl << endl;
										cout << left << setw(19) << "[5] Role" << ": " << res->getString("Role") << endl << endl;
										cout << left << setw(19) << "[6] Status" << ": " << res->getString("Status") << endl << endl;
										cout << left << setw(19) << "[7] Date Registered" << ": " << res->getString("DateRegistered") << endl << endl;
										cout << string(155, '-') << endl << endl;
										cout << "Enter new name (0 for Return): ";
										string temp_name;
										getline(cin, temp_name);
										if (temp_name.length() != 0)
										{
											if (temp_name != "0")
											{
												try
												{
													sql::PreparedStatement* pstmt = con->prepareStatement("UPDATE users SET Name = ? WHERE UserID = ?");
													pstmt->setString(1, temp_name);
													pstmt->setString(2, temp_UserID);
													pstmt->executeUpdate();
													delete pstmt;

													cout << "\nUser's name has been updated successfully.";
													this_thread::sleep_for(chrono::seconds(2));
												}
												catch (sql::SQLException& e) {
													cout << Colors.ColorSet("91") << "\nError editing user: " << e.what() << endl << Colors.ColorReset();
												}
											}
											exit_name = true;
										}
										else
										{
											cout << Colors.ColorSet("91") << "\nError: Name cannot be empty!" << Colors.ColorReset();
											this_thread::sleep_for(chrono::seconds(2));
										}
									}
								}
								else if (choice == 3)	//Edit Email
								{
									bool exit_email = false;
									while (!exit_email)
									{
										UI.PrintHeader("Users Management Menu", "Admin", email_username);
										cout << endl;
										cout << string(155, '-') << endl << endl;
										cout << left << setw(19) << "[1] UserID" << ": " << res->getString("UserID") << endl << endl;
										cout << left << setw(19) << "[2] Name" << ": " << res->getString("Name") << endl << endl;
										cout << left << setw(19) << "[3] Email" << ": " << res->getString("Email") << endl << endl;
										cout << left << setw(19) << "[4] Password" << ": " << res->getString("Password") << endl << endl;
										cout << left << setw(19) << "[5] Role" << ": " << res->getString("Role") << endl << endl;
										cout << left << setw(19) << "[6] Status" << ": " << res->getString("Status") << endl << endl;
										cout << left << setw(19) << "[7] Date Registered" << ": " << res->getString("DateRegistered") << endl << endl;
										cout << string(155, '-') << endl << endl;
										cout << "Enter new email (0 for Return): ";
										string temp_email;
										getline(cin, temp_email);

										if (temp_email != "0")
										{
											if (temp_email.find('@') == string::npos)
											{
												cout << Colors.ColorSet("91") << "\nError: Invalid email format." << Colors.ColorReset();
												this_thread::sleep_for(chrono::seconds(2));
											}
											else
											{
												try
												{
													sql::PreparedStatement* temp_stmt = con->prepareStatement("SELECT * FROM users WHERE Email = ?");
													temp_stmt->setString(1, temp_email);
													sql::ResultSet* temp_res2 = temp_stmt->executeQuery();

													if (temp_res2->next())
													{
														cout << Colors.ColorSet("91") << "\nError: Email already registered!" << Colors.ColorReset();
														this_thread::sleep_for(chrono::seconds(2));
													}
													else
													{
														try
														{
															sql::PreparedStatement* pstmt = con->prepareStatement("UPDATE users SET Email = ? WHERE UserID = ?");
															pstmt->setString(1, temp_email);
															pstmt->setString(2, temp_UserID);
															pstmt->executeUpdate();
															delete pstmt;

															cout << "\nUser's email has been updated successfully.";
															this_thread::sleep_for(chrono::seconds(2));
															exit_email = true;
														}
														catch (sql::SQLException& e) {
															cout << Colors.ColorSet("91") << "\nError editing user: " << e.what() << endl << Colors.ColorReset();
														}
													}
													delete temp_stmt;
													delete temp_res2;
												}
												catch (sql::SQLException& e)
												{
													cout << Colors.ColorSet("91") << "\nError retrieving users: " << e.what() << endl << Colors.ColorReset();
												}
											}
										}
										else
											exit_email = true;
									}
								}
								else if (choice == 4)	//Edit Password
								{
									bool exit_password = false;
									while (!exit_password)
									{
										UI.PrintHeader("Users Management Menu", "Admin", email_username);
										cout << endl;
										cout << string(155, '-') << endl << endl;
										cout << left << setw(19) << "[1] UserID" << ": " << res->getString("UserID") << endl << endl;
										cout << left << setw(19) << "[2] Name" << ": " << res->getString("Name") << endl << endl;
										cout << left << setw(19) << "[3] Email" << ": " << res->getString("Email") << endl << endl;
										cout << left << setw(19) << "[4] Password" << ": " << res->getString("Password") << endl << endl;
										cout << left << setw(19) << "[5] Role" << ": " << res->getString("Role") << endl << endl;
										cout << left << setw(19) << "[6] Status" << ": " << res->getString("Status") << endl << endl;
										cout << left << setw(19) << "[7] Date Registered" << ": " << res->getString("DateRegistered") << endl << endl;
										cout << string(155, '-') << endl << endl;
										cout << "Enter new password (0 for Return): ";
										string temp_password;
										getline(cin, temp_password);

										if (temp_password != "0")
										{
											if (temp_password != res->getString("Password"))
											{
												if (temp_password.length() < 6)
												{
													cout << Colors.ColorSet("91") << "\nError: The password must be a least 6 characters." << Colors.ColorReset();
													this_thread::sleep_for(chrono::seconds(2));
												}
												else
												{
													string confirm_password;
													cout << " " << "Confirm your password (0 for Return): ";
													getline(cin, confirm_password);

													while (confirm_password != temp_password && confirm_password != "0")
													{
														cout << Colors.ColorSet("91") << "\nError: Make sure the passwords are same!" << Colors.ColorReset();
														this_thread::sleep_for(chrono::seconds(2));
														UI.PrintHeader("Users Management Menu", "Admin", email_username);
														cout << endl;
														cout << string(155, '-') << endl << endl;
														cout << left << setw(19) << "[1] UserID" << ": " << res->getString("UserID") << endl << endl;
														cout << left << setw(19) << "[2] Name" << ": " << res->getString("Name") << endl << endl;
														cout << left << setw(19) << "[3] Email" << ": " << res->getString("Email") << endl << endl;
														cout << left << setw(19) << "[4] Password" << ": " << res->getString("Password") << endl << endl;
														cout << left << setw(19) << "[5] Role" << ": " << res->getString("Role") << endl << endl;
														cout << left << setw(19) << "[6] Status" << ": " << res->getString("Status") << endl << endl;
														cout << left << setw(19) << "[7] Date Registered" << ": " << res->getString("DateRegistered") << endl << endl;
														cout << string(155, '-') << endl << endl;
														cout << "Enter new password (0 for Return): " << temp_password << endl;
														cout << "Confirm your password (0 for Return): ";
														getline(cin, confirm_password);
													}
													if (confirm_password != "0")
													{
														try
														{
															sql::PreparedStatement* pstmt = con->prepareStatement("UPDATE users SET Password = ? WHERE UserID = ?");
															pstmt->setString(1, temp_password);
															pstmt->setString(2, temp_UserID);
															pstmt->executeUpdate();
															delete pstmt;

															cout << "\nUser's password has been updated successfully.";
															this_thread::sleep_for(chrono::seconds(2));
															exit_password = true;
														}
														catch (sql::SQLException& e) {
															cout << Colors.ColorSet("91") << "\nError editing user: " << e.what() << endl << Colors.ColorReset();
														}
													}
												}
											}
											else
											{
												cout << Colors.ColorSet("91") << "\nError: The password must be different!" << Colors.ColorReset();
												this_thread::sleep_for(chrono::seconds(2));
											}
										}
										else
											exit_password = true;
									}
								}
								else if (choice == 5)	//Edit Role
								{
									bool exit_role = false;
									while (!exit_role)
									{
										UI.PrintHeader("Users Management Menu", "Admin", email_username);
										cout << endl;
										cout << string(155, '-') << endl << endl;
										cout << left << setw(19) << "[1] UserID" << ": " << res->getString("UserID") << endl << endl;
										cout << left << setw(19) << "[2] Name" << ": " << res->getString("Name") << endl << endl;
										cout << left << setw(19) << "[3] Email" << ": " << res->getString("Email") << endl << endl;
										cout << left << setw(19) << "[4] Password" << ": " << res->getString("Password") << endl << endl;
										cout << left << setw(19) << "[5] Role" << ": " << res->getString("Role") << endl << endl;
										cout << left << setw(19) << "[6] Status" << ": " << res->getString("Status") << endl << endl;
										cout << left << setw(19) << "[7] Date Registered" << ": " << res->getString("DateRegistered") << endl << endl;
										cout << string(155, '-') << endl << endl;
										cout << "Enter role ('Admin' / 'Customer', 0 for Return): ";
										string temp_role;
										getline(cin, temp_role);
										if (temp_role != "0")
										{
											if (temp_role == "Admin" || temp_role == "Customer")
											{
												try
												{
													sql::PreparedStatement* pstmt = con->prepareStatement("UPDATE users SET Role = ? WHERE UserID = ?");
													pstmt->setString(1, temp_role);
													pstmt->setString(2, temp_UserID);
													pstmt->executeUpdate();
													delete pstmt;

													cout << "\nUser's role has been updated successfully.";
													this_thread::sleep_for(chrono::seconds(2));
													exit_role = true;
												}
												catch (sql::SQLException& e) {
													cout << Colors.ColorSet("91") << "\nError editing user: " << e.what() << endl << Colors.ColorReset();
												}
											}
											else
											{
												cout << Colors.ColorSet("91") << "\nError: The role should be Admin or Customer!" << Colors.ColorReset();
												this_thread::sleep_for(chrono::seconds(2));
											}
										}
										else
											exit_role = true;
									}
								}
								else if (choice == 6)	//Edit Status
								{
									bool exit_status = false;
									while (!exit_status)
									{
										UI.PrintHeader("Users Management Menu", "Admin", email_username);
										cout << endl;
										cout << string(155, '-') << endl << endl;
										cout << left << setw(19) << "[1] UserID" << ": " << res->getString("UserID") << endl << endl;
										cout << left << setw(19) << "[2] Name" << ": " << res->getString("Name") << endl << endl;
										cout << left << setw(19) << "[3] Email" << ": " << res->getString("Email") << endl << endl;
										cout << left << setw(19) << "[4] Password" << ": " << res->getString("Password") << endl << endl;
										cout << left << setw(19) << "[5] Role" << ": " << res->getString("Role") << endl << endl;
										cout << left << setw(19) << "[6] Status" << ": " << res->getString("Status") << endl << endl;
										cout << left << setw(19) << "[7] Date Registered" << ": " << res->getString("DateRegistered") << endl << endl;
										cout << string(155, '-') << endl << endl;
										cout << "Enter status ('Active' / 'Deactivated', 0 for Return): ";
										string temp_status;
										getline(cin, temp_status);
										if (temp_status != "0")
										{
											if (temp_status == "Active" || temp_status == "Deactivated")
											{
												try
												{
													sql::PreparedStatement* pstmt = con->prepareStatement("UPDATE users SET Status = ? WHERE UserID = ?");
													pstmt->setString(1, temp_status);
													pstmt->setString(2, temp_UserID);
													pstmt->executeUpdate();
													delete pstmt;

													cout << "\nUser's status has been updated successfully.";
													this_thread::sleep_for(chrono::seconds(2));
													exit_status = true;
												}
												catch (sql::SQLException& e) {
													cout << Colors.ColorSet("91") << "\nError editing user: " << e.what() << endl << Colors.ColorReset();
												}
											}
											else
											{
												cout << Colors.ColorSet("91") << "\nError: The status should be Active or Deactivated!" << Colors.ColorReset();
												this_thread::sleep_for(chrono::seconds(2));
											}
										}
										else
											exit_status = true;
									}
								}
								else if (choice == 7)	//Edit DateRegistered
								{
									cout << Colors.ColorSet("91") << "\nError: Date Registered cannot be edited because it is fixed value!" << Colors.ColorReset();
									this_thread::sleep_for(chrono::seconds(2));
								}
								else if (choice == 0)
									exit_edit = true;

								delete res;
								delete pstmt;
							}
						}
						else
						{
							cout << Colors.ColorSet("91") << "\nError: User ID not found." << Colors.ColorReset();
							this_thread::sleep_for(chrono::seconds(2));
						}
						delete temp_res;
						delete check_user;
					}
					catch (sql::SQLException& e)
					{
						cout << Colors.ColorSet("91") << "\nError retrieving users: " << e.what() << endl << Colors.ColorReset();
					}
				}
				else
					exit = true;
			}
		}
		else if (choice == 5)	//Next page or Previous page
		{
			if (next_page)
				max_users_count += 10;
			else
			{
				current_users_count -= 10;
				max_users_count -= 10;
			}
		}
		else if (choice == 6)	//Previous page
		{
			current_users_count -= 10;
			max_users_count -= 10;
		}
		else if (choice == 0)	//Exit
			exit_ManageUsers = true;
	}
}

void ManageCCInventory()
{
	current_cc_count = 0, max_cc_count = 10;
	search_string = "";
	search_type = "";
	next_page = false, previous_page = false;
	bool exit_CC = false;

	while (!exit_CC)
	{
		UI.ManageCCInventoryUI(con, scene, min_choice, max_choice, email_username,
			current_cc_count, max_cc_count, next_page, previous_page, search_string, search_type);

		InputValidation();
		scene = "";

		if (choice == 1)  // Search component
		{
			UI.PrintHeader("Components Inventory Management", "Admin", email_username);
			cout << endl;

			cout << "Enter Computer Component's Name or Category to search (0 to reset): ";
			getline(cin, search_string);

			if (search_string == "0" || search_string == "")
			{
				search_string = "";
				search_type = "";
			}
			else if (search_string == "CPU" || search_string == "cpu" || search_string == "RAM" || search_string == "ram"
				|| search_string == "Motherboard" || search_string == "motherboard" || search_string == "GPU"
				|| search_string == "gpu" || search_string == "Storage" || search_string == "storage")
			{
				search_type = "Category";
			}
			else
				search_type = "Name";
			current_cc_count = 0;
		}
		else if (choice == 2)  // Add component
		{
			string component_id, component_name, category, description,cost_price, unit_price, stock_quantity;
			int stock;
			double CostPrice, UnitPrice;
			bool exit_add_component = false;

			while (!exit_add_component)
			{
				UI.PrintHeader("Add New Component", "Admin", email_username);
				cout << endl;

				cout << "Component Name (0 to cancel): ";
				getline(cin, component_name);
				if (component_name == "")
				{
					cout << Colors.ColorSet("91") << "\nError: The name cannot be empty!" << Colors.ColorReset();
					this_thread::sleep_for(chrono::seconds(2));
				}
				else if (component_name == "0")
					exit_add_component = true;
				else
				{
					cout << "\nCategory (CPU/RAM/Motherboard/GPU/Storage): ";
					getline(cin, category);
					while (category != "CPU" && category != "RAM" && category != "Motherboard" && category != "GPU" && category != "Storage")
					{
						cout << Colors.ColorSet("91") << "\nError: The category is wrong!" << Colors.ColorReset();
						this_thread::sleep_for(chrono::seconds(2));
						UI.PrintHeader("Add New Component", "Admin", email_username);
						cout << endl;
						cout << "Component Name (0 to cancel): " << component_name << endl;
						cout << "\nCategory (CPU/RAM/Motherboard/GPU/Storage): ";
						getline(cin, category);
					}

					component_id = GenerateComponentID(category);
					if (component_id == "") {
						cout << Colors.ColorSet("91") << "\nError: Cannot generate ComponentID. Category range might be full or invalid." << Colors.ColorReset();
						this_thread::sleep_for(chrono::seconds(2));
					}
					else
					{
						cout << "\nGenerated ComponentID: " << component_id << endl;

						cout << "\nDescription (optional): ";
						getline(cin, description);
						if (description.length() == 0)
							description = " ";

						cout << "\nCost Price (RM): ";
						getline(cin, cost_price);
						while (cost_price.find(' ') != string::npos || !isdigit(cost_price[0]))
						{
							cout << Colors.ColorSet("91") << "\nError: Cost Price should be just only numbers!" << Colors.ColorReset();
							this_thread::sleep_for(chrono::seconds(2));

							UI.PrintHeader("Add New Component", "Admin", email_username);
							cout << endl;
							cout << "Component Name (0 to cancel): " << component_name << endl;
							cout << "\nCategory (CPU/RAM/Motherboard/GPU/Storage): " << category << endl;
							cout << "\nGenerated ComponentID: " << component_id << endl;
							cout << "\nDescription (optional): " << description << endl;
							cout << "\nCost Price (RM): ";
							getline(cin, cost_price);
						}
						CostPrice = stoi(cost_price);

						cout << "\nUnit Price (RM): ";
						getline(cin, unit_price);
						while (unit_price.find(' ') != string::npos || !isdigit(unit_price[0]))
						{
							cout << Colors.ColorSet("91") << "\nError: Unit Price should be just only numbers!" << Colors.ColorReset();
							this_thread::sleep_for(chrono::seconds(2));

							UI.PrintHeader("Add New Component", "Admin", email_username);
							cout << endl;
							cout << "Component Name (0 to cancel): " << component_name << endl;
							cout << "\nCategory (CPU/RAM/Motherboard/GPU/Storage): " << category << endl;
							cout << "\nGenerated ComponentID: " << component_id << endl;
							cout << "\nDescription (optional): " << description << endl;
							cout << "\nCost Price (RM): " << cost_price << endl;
							cout << "\nUnit Price (RM): ";
							getline(cin, unit_price);
						}
						UnitPrice = stoi(unit_price);

						cout << "\nQuantity in Stock: ";
						getline(cin, stock_quantity);
						while (stock_quantity.find(' ') != string::npos || !isdigit(stock_quantity[0]))
						{
							cout << Colors.ColorSet("91") << "\nError: Quantity in Stock should be just only numbers!" << Colors.ColorReset();
							this_thread::sleep_for(chrono::seconds(2));

							UI.PrintHeader("Add New Component", "Admin", email_username);
							cout << endl;
							cout << "Component Name (0 to cancel): " << component_name << endl;
							cout << "\nCategory (CPU/RAM/Motherboard/GPU/Storage): " << category << endl;
							cout << "\nGenerated ComponentID: " << component_id << endl;
							cout << "\nDescription (optional): " << description << endl;
							cout << "\nCost Price (RM): " << cost_price << endl;
							cout << "\nUnit Price (RM): " << unit_price << endl;
							cout << "\nQuantity in Stock: ";
							getline(cin, stock_quantity);
						}
						stock = stoi(stock_quantity);

						string dateAdded = GetCurrentDate();

						try
						{
							sql::PreparedStatement* pstmt = con->prepareStatement("INSERT INTO components (ComponentID, Name, Category, Description, CostPrice, UnitPrice, QuantityInStock, DateAdded) VALUES (?, ?, ?, ?, ?, ?, ?, ?)");

							pstmt->setString(1, component_id);
							pstmt->setString(2, component_name);
							pstmt->setString(3, category);
							pstmt->setString(4, description);
							pstmt->setInt(5, CostPrice);
							pstmt->setInt(6, UnitPrice);
							pstmt->setInt(7, stock);
							pstmt->setString(8, dateAdded);

							pstmt->executeUpdate();
							delete pstmt;

							cout << "\nComponent added successfully!";
							this_thread::sleep_for(chrono::seconds(2));
							exit_add_component = true;
						}
						catch (sql::SQLException& e) {
							cout << Colors.ColorSet("91") << "\nError adding component: " << e.what() << endl << Colors.ColorReset();
							this_thread::sleep_for(chrono::seconds(2));
						}
					}
				}
			}
		}
		else if (choice == 3)  // Delete Component
		{
			bool exit_delete = false;
			while (!exit_delete)
			{
				UI.PrintHeader("Delete Component", "Admin", email_username);
				cout << endl;
				UI.ShowComponentsTable(con, current_cc_count, max_cc_count, next_page, previous_page, search_string, search_type);

				string cid;
				cout << "\nEnter ComponentID to delete (0 to cancel): ";
				getline(cin, cid);
				if (cid != "0")
				{
					try
					{
						cid[0] = toupper(cid[0]);
						sql::PreparedStatement* pstmt = con->prepareStatement("SELECT * FROM components WHERE ComponentID = ?");
						pstmt->setString(1, cid);
						sql::ResultSet* res = pstmt->executeQuery();

						if (res->next())
						{
							bool exitDelete = false;
							while (!exitDelete)
							{
								string desc = res->getString("Description");
								vector<string> lines = WrapText(desc, 135);

								UI.PrintHeader("Users Management Menu", "Admin", email_username);
								cout << endl;
								cout << string(155, '-') << endl << endl;
								cout << left << setw(15) << "ComponentID" << ": " << res->getString("ComponentID") << endl << endl;
								cout << left << setw(15) << "Name" << ": " << res->getString("Name") << endl << endl;
								cout << left << setw(15) << "Category" << ": " << res->getString("Category") << endl << endl;
								if (!lines.empty())
								{
									cout << left << setw(15) << "Description" << ": " << lines[0] << endl;
									for (int i = 1; i < lines.size(); i++)
									{
										cout << setw(17) << "" << lines[i] << endl;
									}
									cout << endl;
								}
								else
									cout << left << setw(15) << "Description" << ": " << endl << endl;
								cout << left << setw(15) << "CostPrice (RM)" << ": " << res->getString("CostPrice") << endl << endl;
								cout << left << setw(15) << "UnitPrice (RM)" << ": " << res->getString("UnitPrice") << endl << endl;
								cout << left << setw(15) << "Stock" << ": " << res->getString("QuantityInStock") << endl << endl;
								cout << left << setw(15) << "DateAdded" << ": " << res->getString("DateAdded") << endl << endl;
								cout << string(155, '-') << endl << endl;
								string answer;
								cout << Colors.ColorSet("91") << "WARNING" << Colors.ColorReset() << ": Do you want to delete everything of this component? (Y/N): ";
								getline(cin, answer);
								if (answer == "Y" || answer == "y")
								{
									try
									{
										// Delete related orders
										sql::PreparedStatement* delOrders = con->prepareStatement("DELETE FROM orders WHERE ComponentID = ?");
										delOrders->setString(1, cid);
										delOrders->executeUpdate();
										delete delOrders;

										// Now delete the component
										sql::PreparedStatement* delComponent = con->prepareStatement("DELETE FROM components WHERE ComponentID = ?");
										delComponent->setString(1, cid);
										delComponent->executeUpdate();
										delete delComponent;

										cout << "\nComponent deleted successfully!";
										this_thread::sleep_for(chrono::seconds(2));
									}
									catch (sql::SQLException& e)
									{
										cout << Colors.ColorSet("91") << "\nError deleting component: " << e.what() << endl << Colors.ColorReset();
										system("PAUSE");
									}
									exitDelete = true;
								}
								else if (answer == "N" || answer == "n")
								{
									exitDelete = true;
								}
								else
								{
									cout << Colors.ColorSet("91") << "\nError: Invalid choice!" << Colors.ColorReset();
									this_thread::sleep_for(chrono::seconds(2));
								}
							}
						}
						else
						{
							cout << Colors.ColorSet("91") << "\nError: Component ID not found!" << Colors.ColorReset();
							this_thread::sleep_for(chrono::seconds(2));
						}

						delete pstmt;
						delete res;
					}
					catch (sql::SQLException& e)
					{
						cout << Colors.ColorSet("91") << "\nError retrieving components: " << e.what() << endl << Colors.ColorReset();
					}
					
				}
				else
					exit_delete = true;
			}
		}
		else if (choice == 4)  // Edit Component
		{
			bool exit_edit = false;
			while (!exit_edit)
			{
				UI.PrintHeader("Edit Component", "Admin", email_username);
				cout << endl;
				UI.ShowComponentsTable(con, current_cc_count, max_cc_count, next_page, previous_page, search_string, search_type);

				string cid;
				cout << "\nEnter ComponentID to edit (0 to cancel): ";
				getline(cin, cid);
				if (cid != "0")
				{
					try
					{
						sql::PreparedStatement* pstmt = con->prepareStatement("SELECT * FROM components WHERE ComponentID = ?");
						pstmt->setString(1, cid);
						sql::ResultSet* res = pstmt->executeQuery();

						if (res->next())
						{
							bool exitEdit = false;
							while (!exitEdit)
							{
								sql::PreparedStatement* pstmt = con->prepareStatement("SELECT * FROM components WHERE ComponentID = ?");
								pstmt->setString(1, cid);
								sql::ResultSet* res = pstmt->executeQuery();
								res->next();

								string desc = res->getString("Description");
								vector<string> lines;
								lines = WrapText(desc, 130);

								UI.PrintHeader("Users Management Menu", "Admin", email_username);
								cout << endl;
								cout << string(155, '-') << endl << endl;
								cout << Colors.ColorSet("33") << "[1] " << Colors.ColorReset() << left << setw(15) << "ComponentID" << ": " << res->getString("ComponentID") << endl << endl;
								cout << Colors.ColorSet("33") << "[2] " << Colors.ColorReset() << left << setw(15) << "Name" << ": " << res->getString("Name") << endl << endl;
								cout << Colors.ColorSet("33") << "[3] " << Colors.ColorReset() << left << setw(15) << "Category" << ": " << res->getString("Category") << endl << endl;
								if (!lines.empty())
								{
									cout << Colors.ColorSet("33") << "[4] " << Colors.ColorReset() << left << setw(15) << "Description" << ": " << lines[0] << endl;
									for (int i = 1; i < lines.size(); i++)
									{
										cout << setw(21) << "" << lines[i] << endl;
									}
									cout << endl;
								}
								else
									cout << Colors.ColorSet("33") << "[4] " << Colors.ColorReset() << left << setw(15) << "Description" << ": " << endl << endl;
								cout << Colors.ColorSet("33") << "[5] " << Colors.ColorReset() << left << setw(15) << "CostPrice (RM)" << ": " << res->getString("CostPrice") << endl << endl;
								cout << Colors.ColorSet("33") << "[6] " << Colors.ColorReset() << left << setw(15) << "UnitPrice (RM)" << ": " << res->getString("UnitPrice") << endl << endl;
								cout << Colors.ColorSet("33") << "[7] " << Colors.ColorReset() << left << setw(15) << "Stock" << ": " << res->getString("QuantityInStock") << endl << endl;
								cout << Colors.ColorSet("33") << "[8] " << Colors.ColorReset() << left << setw(15) << "DateAdded" << ": " << res->getString("DateAdded") << endl << endl;
								cout << string(155, '-') << endl << endl;

								cout << "Your choice (0 for Exit): ";
								min_choice = 0;
								max_choice = 8;

								string temp;
								bool exit_validation = false;
								while (!exit_validation)
								{
									getline(cin, temp);
									while (temp.length() != 1 || !isdigit(temp[0]))
									{
										cout << Colors.ColorSet("91") << "\nError: Invalid choice!" << Colors.ColorReset();
										this_thread::sleep_for(chrono::seconds(2));
										//Get the answer again
										UI.PrintHeader("Users Management Menu", "Admin", email_username);
										cout << endl;
										cout << string(155, '-') << endl << endl;
										cout << Colors.ColorSet("33") << "[1] " << Colors.ColorReset() << left << setw(15) << "ComponentID" << ": " << res->getString("ComponentID") << endl << endl;
										cout << Colors.ColorSet("33") << "[2] " << Colors.ColorReset() << left << setw(15) << "Name" << ": " << res->getString("Name") << endl << endl;
										cout << Colors.ColorSet("33") << "[3] " << Colors.ColorReset() << left << setw(15) << "Category" << ": " << res->getString("Category") << endl << endl;
										if (!lines.empty())
										{
											cout << Colors.ColorSet("33") << "[4] " << Colors.ColorReset() << left << setw(15) << "Description" << ": " << lines[0] << endl;
											for (int i = 1; i < lines.size(); i++)
											{
												cout << setw(21) << "" << lines[i] << endl;
											}
											cout << endl;
										}
										else
											cout << Colors.ColorSet("33") << "[4] " << Colors.ColorReset() << left << setw(15) << "Description" << ": " << endl << endl;
										cout << Colors.ColorSet("33") << "[5] " << Colors.ColorReset() << left << setw(15) << "CostPrice (RM)" << ": " << res->getString("CostPrice") << endl << endl;
										cout << Colors.ColorSet("33") << "[6] " << Colors.ColorReset() << left << setw(15) << "UnitPrice (RM)" << ": " << res->getString("UnitPrice") << endl << endl;
										cout << Colors.ColorSet("33") << "[7] " << Colors.ColorReset() << left << setw(15) << "Stock" << ": " << res->getString("QuantityInStock") << endl << endl;
										cout << Colors.ColorSet("33") << "[8] " << Colors.ColorReset() << left << setw(15) << "DateAdded" << ": " << res->getString("DateAdded") << endl << endl;
										cout << string(155, '-') << endl << endl;

										cout << "Your choice (0 for Exit): ";
										getline(cin, temp);
									}
									choice = stoi(temp);
									if (choice < min_choice || choice > max_choice)
									{
										cout << Colors.ColorSet("91") << "\nError: Invalid choice!" << Colors.ColorReset();
										this_thread::sleep_for(chrono::seconds(2));
										//Get the answer again
										UI.PrintHeader("Users Management Menu", "Admin", email_username);
										cout << endl;
										cout << string(155, '-') << endl << endl;
										cout << Colors.ColorSet("33") << "[1] " << Colors.ColorReset() << left << setw(15) << "ComponentID" << ": " << res->getString("ComponentID") << endl << endl;
										cout << Colors.ColorSet("33") << "[2] " << Colors.ColorReset() << left << setw(15) << "Name" << ": " << res->getString("Name") << endl << endl;
										cout << Colors.ColorSet("33") << "[3] " << Colors.ColorReset() << left << setw(15) << "Category" << ": " << res->getString("Category") << endl << endl;
										if (!lines.empty())
										{
											cout << Colors.ColorSet("33") << "[4] " << Colors.ColorReset() << left << setw(15) << "Description" << ": " << lines[0] << endl;
											for (int i = 1; i < lines.size(); i++)
											{
												cout << setw(21) << "" << lines[i] << endl;
											}
											cout << endl;
										}
										else
											cout << Colors.ColorSet("33") << "[4] " << Colors.ColorReset() << left << setw(15) << "Description" << ": " << endl << endl;
										cout << Colors.ColorSet("33") << "[5] " << Colors.ColorReset() << left << setw(15) << "CostPrice (RM)" << ": " << res->getString("CostPrice") << endl << endl;
										cout << Colors.ColorSet("33") << "[6] " << Colors.ColorReset() << left << setw(15) << "UnitPrice (RM)" << ": " << res->getString("UnitPrice") << endl << endl;
										cout << Colors.ColorSet("33") << "[7] " << Colors.ColorReset() << left << setw(15) << "Stock" << ": " << res->getString("QuantityInStock") << endl << endl;
										cout << Colors.ColorSet("33") << "[8] " << Colors.ColorReset() << left << setw(15) << "DateAdded" << ": " << res->getString("DateAdded") << endl << endl;
										cout << string(155, '-') << endl << endl;

										cout << "Your choice (0 for Exit): ";
									}
									else
										exit_validation = true;
								}
								if (choice == 1)	//Edit ComponentID
								{
									cout << Colors.ColorSet("91") << "\nError: ComponentID cannot be edited because it is Primary Key!" << Colors.ColorReset();
									this_thread::sleep_for(chrono::seconds(2));
								}
								else if (choice == 2)	//Edit Name
								{
									bool exit_name = false;
									while (!exit_name)
									{
										UI.PrintHeader("Users Management Menu", "Admin", email_username);
										cout << endl;
										cout << string(155, '-') << endl << endl;
										cout << left << setw(19) << "[1] ComponentID" << ": " << res->getString("ComponentID") << endl << endl;
										cout << left << setw(19) << "[2] Name" << ": " << res->getString("Name") << endl << endl;
										cout << left << setw(19) << "[3] Category" << ": " << res->getString("Category") << endl << endl;
										if (!lines.empty())
										{
											cout << left << setw(19) << "[4] Description" << ": " << lines[0] << endl;
											for (int i = 1; i < lines.size(); i++)
											{
												cout << setw(21) << "" << lines[i] << endl;
											}
											cout << endl;
										}
										else
											cout << left << setw(19) << "[4] Description" << ": " << endl << endl;
										cout << left << setw(19) << "[5] CostPrice (RM)" << ": " << res->getString("CostPrice") << endl << endl;
										cout << left << setw(19) << "[6] UnitPrice (RM)" << ": " << res->getString("UnitPrice") << endl << endl;
										cout << left << setw(19) << "[7] Stock" << ": " << res->getString("QuantityInStock") << endl << endl;
										cout << left << setw(19) << "[8] DateAdded" << ": " << res->getString("DateAdded") << endl << endl;
										cout << string(155, '-') << endl << endl;
										cout << "Enter new name (0 for Return): ";
										string temp_name;
										getline(cin, temp_name);
										if (temp_name.length() != 0)
										{
											if (temp_name != "0")
											{
												try
												{
													sql::PreparedStatement* pstmt = con->prepareStatement("UPDATE components SET Name = ? WHERE ComponentID = ?");
													pstmt->setString(1, temp_name);
													pstmt->setString(2, cid);
													pstmt->executeUpdate();
													delete pstmt;

													cout << "\nComponent's name has been updated successfully.";
													this_thread::sleep_for(chrono::seconds(2));
												}
												catch (sql::SQLException& e) {
													cout << Colors.ColorSet("91") << "\nError editing component: " << e.what() << endl << Colors.ColorReset();
												}
											}
											exit_name = true;
										}
										else
										{
											cout << Colors.ColorSet("91") << "\nError: Name cannot be empty!" << Colors.ColorReset();
											this_thread::sleep_for(chrono::seconds(2));
										}
									}
								}
								else if (choice == 3)	//Edit Category
								{
									bool exit_cat = false;
									while (!exit_cat)
									{
										UI.PrintHeader("Users Management Menu", "Admin", email_username);
										cout << endl;
										cout << string(155, '-') << endl << endl;
										cout << left << setw(19) << "[1] ComponentID" << ": " << res->getString("ComponentID") << endl << endl;
										cout << left << setw(19) << "[2] Name" << ": " << res->getString("Name") << endl << endl;
										cout << left << setw(19) << "[3] Category" << ": " << res->getString("Category") << endl << endl;
										if (!lines.empty())
										{
											cout << left << setw(19) << "[4] Description" << ": " << lines[0] << endl;
											for (int i = 1; i < lines.size(); i++)
											{
												cout << setw(21) << "" << lines[i] << endl;
											}
											cout << endl;
										}
										else
											cout << left << setw(19) << "[4] Description" << ": " << endl << endl;
										cout << left << setw(19) << "[5] CostPrice (RM)" << ": " << res->getString("CostPrice") << endl << endl;
										cout << left << setw(19) << "[6] UnitPrice (RM)" << ": " << res->getString("UnitPrice") << endl << endl;
										cout << left << setw(19) << "[7] Stock" << ": " << res->getString("QuantityInStock") << endl << endl;
										cout << left << setw(19) << "[8] DateAdded" << ": " << res->getString("DateAdded") << endl << endl;
										cout << string(155, '-') << endl << endl;
										cout << "Enter Category (CPU/RAM/Motherboard/GPU/Storage, 0 for Return): ";
										string temp_cat;
										getline(cin, temp_cat);
										if (temp_cat != "0")
										{
											if (temp_cat == "CPU" || temp_cat == "RAM" || temp_cat == "Motherboard" || temp_cat == "GPU" || temp_cat == "Storage")
											{
												try
												{
													sql::PreparedStatement* pstmt = con->prepareStatement("UPDATE components SET Category = ? WHERE ComponentID = ?");
													pstmt->setString(1, temp_cat);
													pstmt->setString(2, cid);
													pstmt->executeUpdate();
													delete pstmt;

													cout << "\nComponent's category has been updated successfully.";
													this_thread::sleep_for(chrono::seconds(2));
													exit_cat = true;
												}
												catch (sql::SQLException& e) {
													cout << Colors.ColorSet("91") << "\nError editing component: " << e.what() << endl << Colors.ColorReset();
												}
											}
											else
											{
												cout << Colors.ColorSet("91") << "\nError: The category is incorrect!" << Colors.ColorReset();
												this_thread::sleep_for(chrono::seconds(2));
											}
										}
										else
											exit_cat = true;
									}
								}
								else if (choice == 4)	//Edit Description
								{
									bool exit_desc = false;
									while (!exit_desc)
									{
										UI.PrintHeader("Users Management Menu", "Admin", email_username);
										cout << endl;
										cout << string(155, '-') << endl << endl;
										cout << left << setw(19) << "[1] ComponentID" << ": " << res->getString("ComponentID") << endl << endl;
										cout << left << setw(19) << "[2] Name" << ": " << res->getString("Name") << endl << endl;
										cout << left << setw(19) << "[3] Category" << ": " << res->getString("Category") << endl << endl;
										if (!lines.empty())
										{
											cout << left << setw(19) << "[4] Description" << ": " << lines[0] << endl;
											for (int i = 1; i < lines.size(); i++)
											{
												cout << setw(21) << "" << lines[i] << endl;
											}
											cout << endl;
										}
										else
											cout << left << setw(19) << "[4] Description" << ": " << endl << endl;
										cout << left << setw(19) << "[5] CostPrice (RM)" << ": " << res->getString("CostPrice") << endl << endl;
										cout << left << setw(19) << "[6] UnitPrice (RM)" << ": " << res->getString("UnitPrice") << endl << endl;
										cout << left << setw(19) << "[7] Stock" << ": " << res->getString("QuantityInStock") << endl << endl;
										cout << left << setw(19) << "[8] DateAdded" << ": " << res->getString("DateAdded") << endl << endl;
										cout << string(155, '-') << endl << endl;
										cout << "Enter new description (0 for Return): ";
										string temp_desc;
										getline(cin, temp_desc);
										if (temp_desc != "0")
										{
											if (temp_desc.length() == 0)
												temp_desc = " ";
											try
											{
												sql::PreparedStatement* pstmt = con->prepareStatement("UPDATE components SET Description = ? WHERE ComponentID = ?");
												pstmt->setString(1, temp_desc);
												pstmt->setString(2, cid);
												pstmt->executeUpdate();
												delete pstmt;

												cout << "\nComponent's description has been updated successfully.";
												this_thread::sleep_for(chrono::seconds(2));
											}
											catch (sql::SQLException& e) {
												cout << Colors.ColorSet("91") << "\nError editing component: " << e.what() << endl << Colors.ColorReset();
											}
										}
										exit_desc = true;
									}
								}
								else if (choice == 5)	//Edit CostPrice
								{
									bool exit_price = false;
									while (!exit_price)
									{
										UI.PrintHeader("Users Management Menu", "Admin", email_username);
										cout << endl;
										cout << string(155, '-') << endl << endl;
										cout << left << setw(19) << "[1] ComponentID" << ": " << res->getString("ComponentID") << endl << endl;
										cout << left << setw(19) << "[2] Name" << ": " << res->getString("Name") << endl << endl;
										cout << left << setw(19) << "[3] Category" << ": " << res->getString("Category") << endl << endl;
										if (!lines.empty())
										{
											cout << left << setw(19) << "[4] Description" << ": " << lines[0] << endl;
											for (int i = 1; i < lines.size(); i++)
											{
												cout << setw(21) << "" << lines[i] << endl;
											}
											cout << endl;
										}
										else
											cout << left << setw(19) << "[4] Description" << ": " << endl << endl;
										cout << left << setw(19) << "[5] CostPrice (RM)" << ": " << res->getString("CostPrice") << endl << endl;
										cout << left << setw(19) << "[6] UnitPrice (RM)" << ": " << res->getString("UnitPrice") << endl << endl;
										cout << left << setw(19) << "[7] Stock" << ": " << res->getString("QuantityInStock") << endl << endl;
										cout << left << setw(19) << "[8] DateAdded" << ": " << res->getString("DateAdded") << endl << endl;
										cout << string(155, '-') << endl << endl;
										cout << "Enter new CostPrice (RM) (0 for Return): ";
										string temp_price;
										getline(cin, temp_price);
										if (temp_price.length() != 0)
										{
											if (temp_price != "0")
											{
												try
												{
													sql::PreparedStatement* pstmt = con->prepareStatement("UPDATE components SET CostPrice = ? WHERE ComponentID = ?");
													pstmt->setString(1, temp_price);
													pstmt->setString(2, cid);
													pstmt->executeUpdate();
													delete pstmt;

													cout << "\nComponent's CostPrice has been updated successfully.";
													this_thread::sleep_for(chrono::seconds(2));
												}
												catch (sql::SQLException& e) {
													cout << Colors.ColorSet("91") << "\nError editing component: " << e.what() << endl << Colors.ColorReset();
												}
											}
											exit_price = true;
										}
										else
										{
											cout << Colors.ColorSet("91") << "\nError: CostPrice cannot be empty!" << Colors.ColorReset();
											this_thread::sleep_for(chrono::seconds(2));
										}
									}
								}
								else if (choice == 6)	//Edit UnitPrice
								{
									bool exit_price = false;
									while (!exit_price)
									{
										UI.PrintHeader("Users Management Menu", "Admin", email_username);
										cout << endl;
										cout << string(155, '-') << endl << endl;
										cout << left << setw(19) << "[1] ComponentID" << ": " << res->getString("ComponentID") << endl << endl;
										cout << left << setw(19) << "[2] Name" << ": " << res->getString("Name") << endl << endl;
										cout << left << setw(19) << "[3] Category" << ": " << res->getString("Category") << endl << endl;
										if (!lines.empty())
										{
											cout << left << setw(19) << "[4] Description" << ": " << lines[0] << endl;
											for (int i = 1; i < lines.size(); i++)
											{
												cout << setw(21) << "" << lines[i] << endl;
											}
											cout << endl;
										}
										else
											cout << left << setw(19) << "[4] Description" << ": " << endl << endl;
										cout << left << setw(19) << "[5] CostPrice (RM)" << ": " << res->getString("CostPrice") << endl << endl;
										cout << left << setw(19) << "[6] UnitPrice (RM)" << ": " << res->getString("UnitPrice") << endl << endl;
										cout << left << setw(19) << "[7] Stock" << ": " << res->getString("QuantityInStock") << endl << endl;
										cout << left << setw(19) << "[8] DateAdded" << ": " << res->getString("DateAdded") << endl << endl;
										cout << string(155, '-') << endl << endl;
										cout << "Enter new UnitPrice (RM) (0 for Return): ";
										string temp_price;
										getline(cin, temp_price);
										if (temp_price.length() != 0)
										{
											if (temp_price != "0")
											{
												try
												{
													sql::PreparedStatement* pstmt = con->prepareStatement("UPDATE components SET UnitPrice = ? WHERE ComponentID = ?");
													pstmt->setString(1, temp_price);
													pstmt->setString(2, cid);
													pstmt->executeUpdate();
													delete pstmt;

													cout << "\nComponent's UnitPrice has been updated successfully.";
													this_thread::sleep_for(chrono::seconds(2));
												}
												catch (sql::SQLException& e) {
													cout << Colors.ColorSet("91") << "\nError editing component: " << e.what() << endl << Colors.ColorReset();
												}
											}
											exit_price = true;
										}
										else
										{
											cout << Colors.ColorSet("91") << "\nError: UnitPrice cannot be empty!" << Colors.ColorReset();
											this_thread::sleep_for(chrono::seconds(2));
										}
									}
								}
								else if (choice == 7)	//Edit Stock
								{
									bool exit_stock = false;
									while (!exit_stock)
									{
										UI.PrintHeader("Users Management Menu", "Admin", email_username);
										cout << endl;
										cout << string(155, '-') << endl << endl;
										cout << left << setw(19) << "[1] ComponentID" << ": " << res->getString("ComponentID") << endl << endl;
										cout << left << setw(19) << "[2] Name" << ": " << res->getString("Name") << endl << endl;
										cout << left << setw(19) << "[3] Category" << ": " << res->getString("Category") << endl << endl;
										if (!lines.empty())
										{
											cout << left << setw(19) << "[4] Description" << ": " << lines[0] << endl;
											for (int i = 1; i < lines.size(); i++)
											{
												cout << setw(21) << "" << lines[i] << endl;
											}
											cout << endl;
										}
										else
											cout << left << setw(19) << "[4] Description" << ": " << endl << endl;
										cout << left << setw(19) << "[5] CostPrice (RM)" << ": " << res->getString("CostPrice") << endl << endl;
										cout << left << setw(19) << "[6] UnitPrice (RM)" << ": " << res->getString("UnitPrice") << endl << endl;
										cout << left << setw(19) << "[7] Stock" << ": " << res->getString("QuantityInStock") << endl << endl;
										cout << left << setw(19) << "[8] DateAdded" << ": " << res->getString("DateAdded") << endl << endl;
										cout << string(155, '-') << endl << endl;
										cout << "Enter new quantity of stock (E to exit): ";
										string temp_quantity;
										getline(cin, temp_quantity);
										if (temp_quantity.length() != 0)
										{
											if (temp_quantity != "E" && temp_quantity != "e")
											{
												if (isAllDigits(temp_quantity))
												{
													try
													{
														sql::PreparedStatement* pstmt = con->prepareStatement("UPDATE components SET QuantityInStock = ? WHERE ComponentID = ?");
														pstmt->setString(1, temp_quantity);
														pstmt->setString(2, cid);
														pstmt->executeUpdate();
														delete pstmt;

														cout << "\nComponent's quantity has been updated successfully.";
														this_thread::sleep_for(chrono::seconds(2));
														exit_stock = true;
													}
													catch (sql::SQLException& e) {
														cout << Colors.ColorSet("91") << "\nError editing component: " << e.what() << endl << Colors.ColorReset();
													}
												}
												else
												{
													cout << Colors.ColorSet("91") << "\nError: Invalid quantity!" << Colors.ColorReset();
													this_thread::sleep_for(chrono::seconds(2));
												}
											}
											else
												exit_stock = true;
										}
										else
										{
											cout << Colors.ColorSet("91") << "\nError: Quantity cannot be empty!" << Colors.ColorReset();
											this_thread::sleep_for(chrono::seconds(2));
										}
									}
								}
								else if (choice == 8)
								{
									cout << Colors.ColorSet("91") << "\nError: DateAdded cannot be edited because it is a fixed value!" << Colors.ColorReset();
									this_thread::sleep_for(chrono::seconds(2));
								}
								else if (choice == 0)
									exitEdit = true;
							}
						}
						else
						{
							cout << Colors.ColorSet("91") << "\nError: Component ID not found!" << Colors.ColorReset();
							this_thread::sleep_for(chrono::seconds(2));
						}

						delete pstmt;
						delete res;
					}
					catch (sql::SQLException& e)
					{
						cout << Colors.ColorSet("91") << "\nError retrieving components: " << e.what() << endl << Colors.ColorReset();
					}

				}
				else
					exit_edit = true;
			}	
		}
		else if (choice == 5) // Next page or previous page
		{
			if (next_page)
				max_cc_count += 10;
			else {
				current_cc_count -= 10;
				max_cc_count -= 10;
			}
		}
		else if (choice == 6) // Previous page
		{
			current_cc_count -= 10;
			max_cc_count -= 10;
		}
		else if (choice == 0)
			exit_CC = true;
	}
}

string GenerateComponentID(string& category)
{
	int startRange = 0, endRange = 0;

	if (category == "CPU")
	{ 
		startRange = 1;
		endRange = 999;
	}
	else if (category == "RAM")
	{
		startRange = 1001;
		endRange = 1999;
	}
	else if (category == "Motherboard")
	{
		startRange = 2001;
		endRange = 2999;
	}
	else if (category == "GPU")
	{
		startRange = 3001;
		endRange = 3999;
	}
	else if (category == "Storage")
	{
		startRange = 4001;
		endRange = 4999;
	}
	else
	{
		return ""; // Invalid category
	}

	try {
		sql::PreparedStatement* pstmt = con->prepareStatement("SELECT ComponentID FROM components WHERE ComponentID LIKE 'P%' ORDER BY ComponentID ASC");
		sql::ResultSet* res = pstmt->executeQuery();

		int nextNum = startRange;

		while (res->next())
		{
			string id = res->getString("ComponentID");
			int num = stoi(id.substr(1));

			if (num >= startRange && num <= endRange)
			{
				if (num == nextNum)
					nextNum++;
				else if (num > nextNum)
					break;
			}
		}

		delete res;
		delete pstmt;

		if (nextNum > endRange)
		{
			return "";
		}

		stringstream ss;
		ss << "P" << setw(4) << setfill('0') << nextNum;
		return ss.str();
	}
	catch (sql::SQLException& e) {
		cout << Colors.ColorSet("91") << "\nError generating ComponentID: " << e.what() << endl << Colors.ColorReset();
		return "";
	}
}

void OrderProcessing()
{
	current_order_count = 0;
	max_order_count = 10;
	next_page = false, previous_page = false;
	search_order = "";
	bool exit_OrderProcessing = false;

	while (!exit_OrderProcessing)
	{
		UI.OrderProcessingUI(con, scene, min_choice, max_choice, email_username, current_order_count,
			max_order_count, next_page, previous_page, search_order);
		InputValidation();
		scene = "";

		if (choice == 0)
			exit_OrderProcessing = true;

		else if (choice == 1)	//Search Order
		{
			UI.PrintHeader("Order Processing", "Admin", email_username);
			cout << endl;
			cout << "\nEnter OrderID / Customer Name / Component Name (0 to reset): ";
			getline(cin, search_order);
			if (search_order == "0") search_order = "";
			current_order_count = 0;
			max_order_count = 10;
		}
		else if (choice == 2)	//Delivery Order
		{
			string oid;
			bool exit_deliveryOrder = false;

			while (!exit_deliveryOrder)
			{
				UI.PrintHeader("Order Processing", "Admin", email_username);
				cout << endl;

				UI.ShowOrdersTable(con, current_order_count, max_order_count, next_page, previous_page, search_order);

				cout << "\nEnter OrderID to mark as Delivered (0 to Exit): ";
				getline(cin, oid);

				if (oid != "0")
				{
					try
					{
						sql::PreparedStatement* oid_stmt = con->prepareStatement("SELECT * FROM orders WHERE OrderID = ?");
						oid_stmt->setString(1, oid);
						sql::ResultSet* oid_res = oid_stmt->executeQuery();

						if (oid_res->next())
						{
							bool exit_mark = false;

							while (!exit_mark)
							{
								UI.PrintHeader("Order Processing", "Admin", email_username);
								cout << endl;
								cout << string(155, '-') << endl;
								cout << left << setw(15) << "OrderID" << ": " << oid_res->getString("OrderID") << endl << endl;
								cout << left << setw(15) << "UserID" << ": " << oid_res->getString("UserID") << endl << endl;
								cout << left << setw(15) << "ComponentID" << ": " << oid_res->getString("ComponentID") << endl << endl;
								cout << left << setw(15) << "Quantity" << ": " << oid_res->getString("Quantity") << endl << endl;
								cout << left << setw(15) << "Total (RM)" << ": " << oid_res->getString("TotalAmount") << endl << endl;
								cout << left << setw(15) << "Date Ordered" << ": " << oid_res->getString("DateOrdered") << endl << endl;
								cout << left << setw(15) << "Status" << ": ";
								if (oid_res->getString("Status") == "Delivered")
									cout << Colors.ColorSet("92");
								else if (oid_res->getString("Status") == "Pending")
									cout << Colors.ColorSet("93");
								cout << oid_res->getString("Status") << endl << endl << Colors.ColorReset();
								cout << string(155, '-') << endl << endl;
								string answer;
								cout << "Do you want to mark this order as Delivered? (Y/N): ";
								getline(cin, answer);
								if (answer == "Y" || answer == "y")
								{
									try
									{
										sql::PreparedStatement* pstmt = con->prepareStatement("UPDATE orders SET Status = 'Delivered' WHERE OrderID = ? AND Status = 'Pending'");
										pstmt->setString(1, oid);

										int affected = pstmt->executeUpdate();
										delete pstmt;

										if (affected > 0)
										{
											cout << "\nOrder marked as Delivered!";
											this_thread::sleep_for(chrono::seconds(2));
										}
										else
										{
											cout << Colors.ColorSet("91") << "\nError: Order already delivered!" << Colors.ColorReset();
											this_thread::sleep_for(chrono::seconds(2));
										}
										exit_mark = true;
									}
									catch (sql::SQLException& e) {
										cout << Colors.ColorSet("91") << "\nError updating order: " << e.what() << endl << Colors.ColorReset();
									}
								}
								else if (answer == "N" || answer == "n")
								{
									exit_mark = true;
								}
								else
								{
									cout << Colors.ColorSet("91") << "\nError: Invalid choice!" << Colors.ColorReset();
									this_thread::sleep_for(chrono::seconds(2));
								}
							}
						}
						else
						{
							cout << Colors.ColorSet("91") << "\nError: OrderID not found!" << Colors.ColorReset();
							this_thread::sleep_for(chrono::seconds(2));
						}

						delete oid_stmt;
					}
					catch (sql::SQLException& e) {
						cout << Colors.ColorSet("91") << "\nError updating order: " << e.what() << endl << Colors.ColorReset();
					}
				}
				else
					exit_deliveryOrder = true;
			}
		}
		else if (choice == 3) // Delete order
		{
			string oid;
			bool exit_delete = false;
			while (!exit_delete)
			{
				UI.PrintHeader("Order Processing", "Admin", email_username);
				cout << endl;

				UI.ShowOrdersTable(con, current_order_count, max_order_count, next_page, previous_page, search_order);

				cout << "\nEnter OrderID to delete (0 to Exit): ";
				getline(cin, oid);

				if (oid != "0")
				{
					try
					{
						sql::PreparedStatement* oid_stmt;
						sql::ResultSet* oid_res;

						string query =
							"SELECT orders.OrderID, orders.UserID, users.Name AS CustomerName, orders.ComponentID, components.Name AS ComponentName, orders.Quantity, orders.TotalAmount, orders.DateOrdered, orders.Status "
							"FROM orders "
							"JOIN users ON orders.UserID = users.UserID "
							"JOIN components ON orders.ComponentID = components.ComponentID "
							"WHERE orders.OrderID = ?";

						oid_stmt = con->prepareStatement(query);

						oid_stmt->setString(1, oid);

						oid_res = oid_stmt->executeQuery();

						if (oid_res->next())
						{
							bool exit_confirm = false;

							while (!exit_confirm)
							{
								UI.PrintHeader("Order Processing", "Admin", email_username);
								cout << endl;
								cout << string(155, '-') << endl << endl;
								cout << left << setw(15) << "OrderID" << ": " << oid_res->getString("OrderID") << endl << endl;
								cout << left << setw(15) << "UserID" << ": " << oid_res->getString("UserID") << endl << endl;
								cout << left << setw(15) << "Customer Name" << ": " << oid_res->getString("CustomerName") << endl << endl;
								cout << left << setw(15) << "ComponentID" << ": " << oid_res->getString("ComponentID") << endl << endl;
								cout << left << setw(15) << "Component Name" << ": " << oid_res->getString("ComponentName") << endl << endl;
								cout << left << setw(15) << "Quantity" << ": " << oid_res->getString("Quantity") << endl << endl;
								cout << left << setw(15) << "Total (RM)" << ": " << oid_res->getString("TotalAmount") << endl << endl;
								cout << left << setw(15) << "Date Ordered" << ": " << oid_res->getString("DateOrdered") << endl << endl;
								cout << left << setw(15) << "Status" << ": ";
								if (oid_res->getString("Status") == "Delivered")
									cout << Colors.ColorSet("92");
								else if (oid_res->getString("Status") == "Pending")
									cout << Colors.ColorSet("93");
								cout << oid_res->getString("Status") << endl << endl << Colors.ColorReset();
								cout << string(155, '-') << endl << endl;
								string answer;
								cout << "Do you want to delete this order? (Y/N): ";
								getline(cin, answer);
								if (answer == "Y" || answer == "y")
								{
									try
									{
										sql::PreparedStatement* pstmt = con->prepareStatement("DELETE FROM orders WHERE OrderID = ?");
										pstmt->setString(1, oid);
										int affected = pstmt->executeUpdate();
										delete pstmt;

										if (affected > 0)
										{
											cout << "\nOrder deleted successfully!";
											this_thread::sleep_for(chrono::seconds(2));
										}
									}
									catch (sql::SQLException& e) {
										cout << Colors.ColorSet("91") << "\nError deleting order: " << e.what() << endl << Colors.ColorReset();
									}
									exit_confirm = true;
								}
								else if (answer == "N" || answer == "n")
								{
									exit_confirm = true;
								}
								else
								{
									cout << Colors.ColorSet("91") << "\nError: Invalid choice!" << Colors.ColorReset();
									this_thread::sleep_for(chrono::seconds(2));
								}
							}
						}
						else
						{
							cout << Colors.ColorSet("91") << "\nError: OrderID not found!" << Colors.ColorReset();
							this_thread::sleep_for(chrono::seconds(2));
						}

						delete oid_stmt;
					}
					catch (sql::SQLException& e) {
						cout << Colors.ColorSet("91") << "\nError deleting order: " << e.what() << endl << Colors.ColorReset();
					}					
				}
				else
					exit_delete = true;
			}
		}
		else if (choice == 4 && next_page) // Next page
		{
			max_order_count += 10;
		}
		else if ((choice == 5 && previous_page) || (choice == 4 && !next_page && previous_page)) // Previous page
		{
			current_order_count -= 10;
			max_order_count -= 10;
		}
	}
}

void OrderTrendAnalysis()
{
	auto now = chrono::system_clock::now();
	time_t t = chrono::system_clock::to_time_t(now);
	tm localTime;
	localtime_s(&localTime, &t);

	month = localTime.tm_mon + 1;		//Current month
	year = localTime.tm_year + 1900;	//Current year

	bool exitTrend = false;

	while (!exitTrend)
	{
		UI.OrderTrendAnalysisUI(con, scene, min_choice, max_choice, email_username, month, year);

		InputValidation();
		scene = "";

		if (choice == 1)	//View Details
		{
			bool back = false;
			while (!back)
			{
				UI.PrintHeader("Order Trend Analysis", "Admin", email_username);
				UI.ShowOrderTrends(con, month, year);

				UI.PrintTopLine();
				cout << "|   " << Colors.ColorSet("33") << "[1] " << Colors.ColorReset() << left << setw(45) << "CPU" << "|\n";
				cout << "|   " << Colors.ColorSet("33") << "[2] " << Colors.ColorReset() << left << setw(45) << "RAM" << "|\n";
				cout << "|   " << Colors.ColorSet("33") << "[3] " << Colors.ColorReset() << left << setw(45) << "Motherboard" << "|\n";
				cout << "|   " << Colors.ColorSet("33") << "[4] " << Colors.ColorReset() << left << setw(45) << "GPU" << "|\n";
				cout << "|   " << Colors.ColorSet("33") << "[5] " << Colors.ColorReset() << left << setw(45) << "Storage" << "|\n";
				cout << "|   " << Colors.ColorSet("33") << "[0] " << Colors.ColorReset() << left << setw(45) << "Return" << "|\n";

				min_choice = 0;
				max_choice = 5;

				UI.PrintFooter(min_choice, max_choice);

				cout << "Your choice: ";

				string input;
				string categories[5] = { "CPU", "RAM", "Motherboard", "GPU", "Storage" };
				int cat;
				getline(cin, input);

				if (input.length() == 1 && isdigit(input[0]))
				{
					cat = stoi(input);
					if (cat >= min_choice && cat <= max_choice)
					{
						if (cat != 0)
						{
							int start = 0;
							next_page = false;
							previous_page = false;

							bool detailBack = false;

							while (!detailBack)
							{
								UI.PrintHeader("Details - " + categories[cat - 1], "Admin", email_username);

								UI.ShowCategoryDetails(con, categories[cat - 1], month, year, start, next_page, previous_page);

								int optionIndex = 1;

								UI.PrintTopLine();
								if (next_page)
									cout << "|   " << Colors.ColorSet("33") << "[" << optionIndex++ << "] " << Colors.ColorReset() << left << setw(45) << "Next Page" << "|\n";
								if (previous_page)
									cout << "|   " << Colors.ColorSet("33") << "[" << optionIndex++ << "] " << Colors.ColorReset() << left << setw(45) << "Previous Page" << "|\n";
								cout << "|   " << Colors.ColorSet("33") << "[0] " << Colors.ColorReset() << left << setw(45) << "Return" << "|\n";
								min_choice = 0;
								max_choice = optionIndex - 1;

								UI.PrintFooter(min_choice, max_choice);

								string c;
								int temp_c;
								getline(cin, c);

								if (c.length() == 1 && isdigit(c[0]))
								{
									temp_c = stoi(c);
									if (temp_c >= min_choice && temp_c <= max_choice)
									{
										if (temp_c == 1 && next_page)
											start += 10;
										else if (temp_c == 2 && previous_page && start >= 10)
											start -= 10;
										else if (temp_c == 0)
											detailBack = true;
									}
									else
									{
										cout << Colors.ColorSet("91") << "\nError: Invalid choice!" << Colors.ColorReset();
										this_thread::sleep_for(chrono::seconds(2));
									}
								}
								else
								{
									cout << Colors.ColorSet("91") << "\nError: Invalid choice!" << Colors.ColorReset();
									this_thread::sleep_for(chrono::seconds(2));
								}
							}
						}
						else
							back = true;
					}
					else
					{
						cout << Colors.ColorSet("91") << "\nError: Invalid choice!" << Colors.ColorReset();
						this_thread::sleep_for(chrono::seconds(2));
					}
				}
				else
				{
					cout << Colors.ColorSet("91") << "\nError: Invalid choice!" << Colors.ColorReset();
					this_thread::sleep_for(chrono::seconds(2));
				}
			}
		}
		else if (choice == 2)	//Choose Another Month/Year
		{
			bool valid_input = false;

			while (!valid_input)
			{
				UI.PrintHeader("Order Trend Analysis", "Admin", email_username);
				UI.ShowOrderTrends(con, month, year);

				string temp_month;
				cout << "\nEnter Month (1-12): ";
				getline(cin, temp_month);

				if (temp_month == "")
				{
					month = localTime.tm_mon + 1;		//Current month
					year = localTime.tm_year + 1900;	//Current year
					valid_input = true;
				}
				else if (isAllDigits(temp_month) && stoi(temp_month) >= 1 && stoi(temp_month) <= 12)
				{
					bool valid_input_year = false;
					while (!valid_input_year)
					{
						UI.PrintHeader("Order Trend Analysis", "Admin", email_username);
						UI.ShowOrderTrends(con, month, year);
						cout << "\nEnter Month (1-12): " << temp_month << endl;

						string temp_year;
						cout << "Enter Year (YYYY): ";
						getline(cin, temp_year);

						if (temp_year == "")
						{
							month = stoi(temp_month);
							year = localTime.tm_year + 1900;	//Current year
							valid_input_year = true;
							valid_input = true;
						}
						else if (isAllDigits(temp_year) && temp_year.length() == 4)
						{
							month = stoi(temp_month);
							year = stoi(temp_year);
							valid_input_year = true;
							valid_input = true;
						}
						else
						{
							cout << Colors.ColorSet("91") << "\nError: Invalid year!" << Colors.ColorReset();
							this_thread::sleep_for(chrono::seconds(2));
						}
					}
				}
				else
				{
					cout << Colors.ColorSet("91") << "\nError: Invalid month!" << Colors.ColorReset();
					this_thread::sleep_for(chrono::seconds(2));
				}
			}
		}
		else if (choice == 0)
			exitTrend = true;
	}
}

void MonthlySalesAnalysis()
{
	current_cc_count = 0;
	max_cc_count = 10;
	next_page = false;
	previous_page = false;
	search_string = "";
	search_type = "";

	auto now = chrono::system_clock::now();
	time_t t = chrono::system_clock::to_time_t(now);
	tm localTime;
	localtime_s(&localTime, &t);

	month = localTime.tm_mon + 1;		//Current month
	year = localTime.tm_year + 1900;	//Current year

	bool exit = false;
	while (!exit)
	{
		UI.MonthlySalesAnalysisUI(con, scene, min_choice, max_choice, email_username, month, year, current_cc_count,
			max_cc_count, next_page, previous_page, search_string, search_type);

		InputValidation();
		scene = "";

		if (choice == 1)	// Search Components
		{
			UI.PrintHeader("Profit Margin Report", "Admin", email_username);
			UI.ShowSalesAnalysis(con, month, year, current_cc_count, max_cc_count, next_page, previous_page, search_string, search_type);

			cout << "\nEnter the name/category to search (0 for Reset): ";
			getline(cin, search_string);

			if (search_string == "0" || search_string == "")
			{
				search_string = "";
				search_type = "";
			}
			else if (search_string == "CPU" || search_string == "cpu" || search_string == "RAM" || search_string == "ram"
				|| search_string == "Motherboard" || search_string == "motherboard" || search_string == "GPU"
				|| search_string == "gpu" || search_string == "Storage" || search_string == "storage")
			{
				search_type = "Category";
			}
			else
				search_type = "Name";
			current_cc_count = 0;
		}
		else if (choice == 2)	// Search Another Month & Year
		{
			current_cc_count = 0;
			search_string = "";
			search_type = "";

			bool valid_input = false;

			while (!valid_input)
			{
				UI.PrintHeader("Profit Margin Report", "Admin", email_username);
				UI.ShowSalesAnalysis(con, month, year, current_cc_count, max_cc_count, next_page, previous_page, search_string, search_type);

				string temp_month;
				cout << "\nEnter Month (1-12): ";
				getline(cin, temp_month);

				if (temp_month == "")
				{
					month = localTime.tm_mon + 1;		//Current month
					year = localTime.tm_year + 1900;	//Current year
					valid_input = true;
				}
				else if (isAllDigits(temp_month) && stoi(temp_month) >= 1 && stoi(temp_month) <= 12)
				{
					bool valid_input_year = false;
					while (!valid_input_year)
					{
						UI.PrintHeader("Profit Margin Report", "Admin", email_username);
						UI.ShowSalesAnalysis(con, month, year, current_cc_count, max_cc_count, next_page, previous_page, search_string, search_type);
						cout << "\nEnter Month (1-12): " << temp_month << endl;

						string temp_year;
						cout << "Enter Year (YYYY): ";
						getline(cin, temp_year);

						if (temp_year == "")
						{
							month = stoi(temp_month);
							year = localTime.tm_year + 1900;	//Current year
							valid_input_year = true;
							valid_input = true;
						}
						else if (isAllDigits(temp_year) && temp_year.length() == 4)
						{
							month = stoi(temp_month);
							year = stoi(temp_year);
							valid_input_year = true;
							valid_input = true;
						}
						else
						{
							cout << Colors.ColorSet("91") << "\nError: Invalid year!" << Colors.ColorReset();
							this_thread::sleep_for(chrono::seconds(2));
						}
					}
				}
				else
				{
					cout << Colors.ColorSet("91") << "\nError: Invalid month!" << Colors.ColorReset();
					this_thread::sleep_for(chrono::seconds(2));
				}
			}
		}
		else if (choice == 3)
		{
			if (next_page)
				max_cc_count += 10;
			else if (previous_page)
			{
				current_cc_count -= 10;
				max_cc_count -= 10;
			}
		}
		else if (choice == 4)
		{
			current_cc_count -= 10;
			max_cc_count -= 10;
		}
		else if (choice == 0)
			exit = true;
	}
}

void AnnualSalesReport()
{
	auto now = chrono::system_clock::now();
	time_t t = chrono::system_clock::to_time_t(now);
	tm localTime;
	localtime_s(&localTime, &t);

	year = localTime.tm_year + 1900;	//Current year

	bool exit_SalesAnalysis = false;

	while (!exit_SalesAnalysis)
	{
		UI.AnnualSalesReportUI(con, scene, min_choice, max_choice, email_username, year);
		InputValidation();

		if (choice == 0)
			exit_SalesAnalysis = true;
		else if (choice == 1)
		{
			string temp_year;
			bool valid_input = false;

			while (!valid_input)
			{
				UI.PrintHeader("Sales Analysis", "Admin", email_username);
				UI.ShowSalesReport(con, year);
				cout << "\nEnter year (YYYY): ";
				getline(cin, temp_year);

				if (temp_year == "")
				{
					year = localTime.tm_year + 1900;	//Current year
					valid_input = true;
				}
				else if (isAllDigits(temp_year) && temp_year.length() == 4)
				{
					year = stoi(temp_year);
					valid_input = true;
				}
				else
				{
					cout << Colors.ColorSet("91") << "\nError: Invalid year!" << Colors.ColorReset();
					this_thread::sleep_for(chrono::seconds(2));
				}
			}
		}
	}
}

void SearchItems()
{
	UI.PrintHeader("Search Component", "Customer", email_username);
	cout << endl;
	UI.ShowComponentsTable(con, current_cc_count, max_cc_count, next_page, previous_page, search_string, search_type);

	cout << "\nEnter the name/category to search (0 for Reset): ";
	getline(cin, search_string);

	if (search_string == "0" || search_string == "")
	{
		search_string = "";
		search_type = "";
	}
	else if (search_string == "CPU" || search_string == "cpu" || search_string == "RAM" || search_string == "ram"
		|| search_string == "Motherboard" || search_string == "motherboard" || search_string == "GPU"
		|| search_string == "gpu" || search_string == "Storage" || search_string == "storage")
	{
		search_type = "Category";
	}
	else
		search_type = "Name";
	current_cc_count = 0;
}

void PlaceOrder()
{
	string temp_cid;
	bool exit_PlaceOrder = false;

	while (!exit_PlaceOrder)
	{
		UI.PrintHeader("Place Order", "Customer", email_username);
		cout << endl;
		UI.ShowComponentsTable_UsersView(con, current_cc_count, max_cc_count, next_page, previous_page, search_string, search_type);

		cout << "\nEnter the Component's ID to place order (0 for Exit): ";
		getline(cin, temp_cid);
		if (temp_cid != "0")
		{
			try
			{
				sql::PreparedStatement* pstmt = con->prepareStatement("SELECT * FROM components WHERE ComponentID = ?");
				pstmt->setString(1, temp_cid);
				sql::ResultSet* res = pstmt->executeQuery();

				if (res->next())
				{
					bool exitOrder = false;
					temp_cid[0] = toupper(temp_cid[0]);
					while (!exitOrder)
					{
						string desc = res->getString("Description");
						vector<string> lines = WrapText(desc, 80);

						UI.PrintHeader("Place Order", "Customer", email_username);
						cout << endl;
						cout << string(155, '-') << endl << endl;
						cout << left << setw(15) << "ComponentID" << ": " << res->getString("ComponentID") << endl << endl;
						cout << left << setw(15) << "Name" << ": " << res->getString("Name") << endl << endl;
						cout << left << setw(15) << "Category" << ": " << res->getString("Category") << endl << endl;
						if (!lines.empty())
						{
							cout << left << setw(15) << "Description" << ": " << lines[0] << endl;
							for (int i = 1; i < lines.size(); i++)
							{
								cout << setw(17) << "" << lines[i] << endl;
							}
							cout << endl;
						}
						else
							cout << left << setw(15) << "Description" << ": " << endl << endl;
						cout << left << setw(15) << "UnitPrice (RM)" << ": " << res->getString("UnitPrice") << endl << endl;
						cout << left << setw(15) << "Stock" << ": " << res->getString("QuantityInStock") << endl << endl;
						cout << string(155, '-') << endl << endl;
						string answer;
						cout << "Do you want to purchase this item? (Y/N): ";
						getline(cin, answer);
						if (answer == "Y" || answer == "y")
						{
							bool exitQuantity = false;

							while (!exitQuantity)
							{
								UI.PrintHeader("Place Order", "Customer", email_username);
								cout << endl;
								cout << string(155, '-') << endl << endl;
								cout << left << setw(15) << "ComponentID" << ": " << res->getString("ComponentID") << endl << endl;
								cout << left << setw(15) << "Name" << ": " << res->getString("Name") << endl << endl;
								cout << left << setw(15) << "Category" << ": " << res->getString("Category") << endl << endl;
								if (!lines.empty())
								{
									cout << left << setw(15) << "Description" << ": " << lines[0] << endl;
									for (int i = 1; i < lines.size(); i++)
									{
										cout << setw(17) << "" << lines[i] << endl;
									}
									cout << endl;
								}
								else
									cout << left << setw(15) << "Description" << ": " << endl << endl;
								cout << left << setw(15) << "UnitPrice (RM)" << ": " << res->getString("UnitPrice") << endl << endl;
								cout << left << setw(15) << "Stock" << ": " << res->getString("QuantityInStock") << endl << endl;
								cout << string(155, '-') << endl << endl;
								cout << "Enter the quantity (0 for Exit): ";
								getline(cin, answer);
								if (isAllDigits(answer))
								{
									if (answer != "0")
									{
										int temp_quantity;

										temp_quantity = stoi(answer);
										if (res->getInt("QuantityInStock") < temp_quantity)
										{
											cout << Colors.ColorSet("91") << "\nError: Insufficient inventory!" << Colors.ColorReset();
											this_thread::sleep_for(chrono::seconds(2));
										}
										else
										{
											double price;
											bool exitConfirm = false;
											price = temp_quantity * res->getDouble("UnitPrice");

											while (!exitConfirm)
											{
												UI.PrintHeader("Place Order", "Customer", email_username);
												cout << endl;
												cout << string(155, '-') << endl << endl;
												cout << left << setw(15) << "ComponentID" << ": " << res->getString("ComponentID") << endl << endl;
												cout << left << setw(15) << "Name" << ": " << res->getString("Name") << endl << endl;
												cout << left << setw(15) << "Category" << ": " << res->getString("Category") << endl << endl;
												if (!lines.empty())
												{
													cout << left << setw(15) << "Description" << ": " << lines[0] << endl;
													for (int i = 1; i < lines.size(); i++)
													{
														cout << setw(17) << "" << lines[i] << endl;
													}
													cout << endl;
												}
												else
													cout << left << setw(15) << "Description" << ": " << endl << endl;
												cout << left << setw(15) << "UnitPrice (RM)" << ": " << res->getString("UnitPrice") << endl << endl;
												cout << left << setw(15) << "Stock" << ": " << res->getString("QuantityInStock") << endl << endl;
												cout << string(155, '-') << endl << endl;
												cout << "Enter the quantity (0 for Exit): " << answer << endl;

												cout << "\nTotal price: RM" << price << endl;
												cout << "Confirm your purchase (Y/N): ";

												string confirm;
												getline(cin, confirm);
												if (confirm == "Y" || confirm == "y")
												{
													//Get the Order ID//
													counts = 1;
													string temp_str, order_id;
													int temp_num;

													try
													{
														sql::Statement* temp_stmt;
														sql::ResultSet* temp_res;
														temp_stmt = con->createStatement();
														temp_res = temp_stmt->executeQuery("SELECT OrderID FROM orders ORDER BY OrderID ASC");
														while (temp_res->next())
														{
															temp_str = temp_res->getString("OrderID");
															temp_str = temp_str.substr(1);
															temp_num = stoi(temp_str);

															if (temp_num != counts)
															{
																break;
															}
															counts++;
														}
														stringstream ss;
														ss << "O" << setfill('0') << setw(4) << counts;		//Generate the order id like O0001
														order_id = ss.str();

														delete temp_res;
														delete temp_stmt;
													}
													catch (sql::SQLException& e)
													{
														cout << Colors.ColorSet("91") << "\nError retrieving orders: " << e.what() << endl << Colors.ColorReset();
													}
													//Get the Order ID//

													string date_ordered = GetCurrentDate();

													try
													{
														sql::PreparedStatement* insertOrder = con->prepareStatement
														(
															"INSERT INTO orders (OrderID, UserID, ComponentID, Quantity, TotalAmount, DateOrdered, Status) VALUES (?, ?, ?, ?, ?, ?, ?)"
														);
														insertOrder->setString(1, order_id);
														insertOrder->setString(2, user_id);
														insertOrder->setString(3, res->getString("ComponentID"));
														insertOrder->setInt(4, temp_quantity);
														insertOrder->setDouble(5, price);
														insertOrder->setString(6, date_ordered);
														insertOrder->setString(7, "Pending");
														insertOrder->executeUpdate();
														delete insertOrder;

														try
														{
															sql::PreparedStatement* update = con->prepareStatement("UPDATE components SET QuantityInStock = ? WHERE ComponentID = ?");
															update->setInt(1, res->getInt("QuantityInStock") - temp_quantity);
															update->setString(2, temp_cid);
															update->executeUpdate();
															delete update;
														}
														catch (sql::SQLException& e) {
															cout << Colors.ColorSet("91") << "\nError editing component: " << e.what() << endl << Colors.ColorReset();
														}

														UI.ShowReceipt(order_id, email_username, temp_cid, res->getString("Name"), res->getString("Category"), res->getDouble("UnitPrice"), temp_quantity);
														cout << "\nPress Enter to continue...";
														string dummy;
														getline(cin, dummy);
													}
													catch (sql::SQLException& e) {
														cout << Colors.ColorSet("91") << "\nError ordering component: " << e.what() << endl << Colors.ColorReset();
													}
													exitConfirm = true;
													exitQuantity = true;
													exitOrder = true;
												}
												else if (confirm == "N" || confirm == "n")
												{
													exitConfirm = true;
													exitQuantity = true;
													exitOrder = true;
												}
												else
												{
													cout << Colors.ColorSet("91") << "\nError: Invalid choice!" << Colors.ColorReset();
													this_thread::sleep_for(chrono::seconds(2));
												}
											}
										}
									}
									else
									{
										exitQuantity = true;
										exitOrder = true;
									}
								}
								else
								{
									cout << Colors.ColorSet("91") << "\nError: Quantity must be numbers!" << Colors.ColorReset();
									this_thread::sleep_for(chrono::seconds(2));
								}
							}
						}
						else if (answer == "N" || answer == "n")
						{
							exitOrder = true;
						}
						else
						{
							cout << Colors.ColorSet("91") << "\nError: Invalid choice!" << Colors.ColorReset();
							this_thread::sleep_for(chrono::seconds(2));
						}
					}
				}
				else
				{
					cout << Colors.ColorSet("91") << "\nError: Component ID not found!" << Colors.ColorReset();
					this_thread::sleep_for(chrono::seconds(2));
				}

				delete pstmt;
				delete res;
			}
			catch (sql::SQLException& e)
			{
				cout << Colors.ColorSet("91") << "\nError retrieving components: " << e.what() << endl << Colors.ColorReset();
			}
		}
		else
			exit_PlaceOrder = true;
	}
}

void ViewOrderHistory()
{
	int current_order_count = 0;
	int max_order_count = 10;
	next_page = false, previous_page = false;
	string search_string = "";
	bool exit_ViewOrderHistory = false;

	while (!exit_ViewOrderHistory)
	{
		UI.PrintHeader("View Order", "Customer", email_username);
		cout << endl;
		cout << left << Colors.ColorSet("93")
			<< setw(12) << "OrderID"
			<< setw(80) << "Component Name"
			<< setw(10) << "Quantity"
			<< setw(15) << "Total (RM)"
			<< setw(15) << "Date"
			<< setw(15) << "Status"
			<< Colors.ColorReset() << endl;
		cout << string(155, '-') << endl;

		try
		{
			sql::PreparedStatement* pstmt;
			sql::ResultSet* res;

			string query =
				"SELECT orders.OrderID, components.Name AS ComponentName, orders.Quantity, orders.TotalAmount, orders.DateOrdered, orders.Status "
				"FROM orders "
				"JOIN components ON orders.ComponentID = components.ComponentID "
				"WHERE orders.UserID = ?";

			if (search_string != "")
				query += " AND components.Name LIKE ?";

			query += " ORDER BY orders.OrderID ASC";

			pstmt = con->prepareStatement(query);
			pstmt->setString(1, user_id);

			if (search_string != "")
				pstmt->setString(2, "%" + search_string + "%");

			res = pstmt->executeQuery();

			current_order_count -= (current_order_count % 10);

			if (current_order_count == max_order_count)
				current_order_count -= 10;

			for (int i = 0; i < current_order_count; i++)
				res->next();

			if (!res->next())
			{
				for (int i = 0; i < 10; i++)
				{
					cout << endl << endl;
				}
			}
			res->previous();

			bool swap_colors = false;
			while (res->next() && current_order_count < max_order_count)
			{
				if (swap_colors)
					cout << Colors.ColorSet("37");
				else if (!swap_colors)
					cout << Colors.ColorSet("97");

				cout << left
					<< setw(12) << res->getString("OrderID")
					<< setw(80) << UI.TruncateWithEllipsis(res->getString("ComponentName"), 78)
					<< setw(10) << res->getString("Quantity")
					<< setw(15) << res->getString("TotalAmount")
					<< setw(15) << res->getString("DateOrdered");
				if (res->getString("Status") == "Delivered")
					cout << Colors.ColorSet("92");
				else if (res->getString("Status") == "Pending")
					cout << Colors.ColorSet("93");
				cout << setw(15) << res->getString("Status")
					<< Colors.ColorReset() << endl << endl;

				swap_colors = !swap_colors;
				current_order_count++;

				if (res->next() && current_order_count == max_order_count)
					next_page = true;
				else
					next_page = false;

				res->previous();
			}

			previous_page = (max_order_count > 10);

			delete res;
			delete pstmt;
		}
		catch (sql::SQLException& e)
		{
			cout << Colors.ColorSet("91") << "\nError retrieving order history: " << e.what() << endl << Colors.ColorReset();
		}

		if (current_order_count % 10 != 0)
		{
			int empty_line = 10 - (current_order_count % 10);
			for (int i = 0; i < empty_line; i++)
				cout << endl << endl;
		}

		cout << string(155, '-') << endl;

		int optionIndex = 2;

		UI.PrintTopLine();
		cout << "|   " << Colors.ColorSet("33") << "[1] " << Colors.ColorReset() << left << setw(45) << "Search Component Name" << "|" << endl;
		if (next_page)
			cout << "|   " << Colors.ColorSet("33") << "[" << optionIndex++ << "] " << Colors.ColorReset() << left << setw(45) << "Next Page" << "|" << endl;
		if (previous_page)
			cout << "|   " << Colors.ColorSet("33") << "[" << optionIndex++ << "] " << Colors.ColorReset() << left << setw(45) << "Previous Page" << "|" << endl;
		cout << "|   " << Colors.ColorSet("33") << "[0] " << Colors.ColorReset() << left << setw(45) << "Return" << "|" << endl;
		
		min_choice = 0;
		max_choice = optionIndex - 1;
		UI.PrintFooter(min_choice, max_choice);
		string temp;
		getline(cin, temp);
		if (temp.length() == 1 && isdigit(temp[0]))
		{
			choice = stoi(temp);
			if (choice >= min_choice && choice <= max_choice)
			{
				if (choice == 0)
				{
					exit_ViewOrderHistory = true;
				}
				else if (choice == 1)    // Search order
				{
					UI.PrintHeader("View Order", "Customer", email_username);
					cout << endl;
					cout << left << setw(12) << "OrderID"
						<< setw(80) << "Component Name"
						<< setw(10) << "Quantity"
						<< setw(15) << "Total (RM)"
						<< setw(15) << "Date"
						<< setw(15) << "Status"
						<< endl;
					cout << string(155, '-') << endl;

					try
					{
						sql::PreparedStatement* pstmt;
						sql::ResultSet* res;

						string query =
							"SELECT orders.OrderID, components.Name AS ComponentName, orders.Quantity, orders.TotalAmount, orders.DateOrdered, orders.Status "
							"FROM orders "
							"JOIN components ON orders.ComponentID = components.ComponentID "
							"WHERE orders.UserID = ?";

						if (search_string != "")
							query += " AND components.Name LIKE ?";

						query += " ORDER BY orders.OrderID ASC";

						pstmt = con->prepareStatement(query);
						pstmt->setString(1, user_id);

						if (search_string != "")
							pstmt->setString(2, "%" + search_string + "%");

						res = pstmt->executeQuery();

						current_order_count -= (current_order_count % 10);

						if (current_order_count == max_order_count)
							current_order_count -= 10;

						for (int i = 0; i < current_order_count; i++)
							res->next();

						bool swap_colors = false;
						while (res->next() && current_order_count < max_order_count)
						{
							if (swap_colors)
								cout << Colors.ColorSet("94");
							else if (!swap_colors)
								cout << Colors.ColorSet("93");

							cout << left
								<< setw(12) << res->getString("OrderID")
								<< setw(80) << UI.TruncateWithEllipsis(res->getString("ComponentName"), 78)
								<< setw(10) << res->getString("Quantity")
								<< setw(15) << res->getString("TotalAmount")
								<< setw(15) << res->getString("DateOrdered")
								<< setw(15) << res->getString("Status")
								<< Colors.ColorReset() << endl << endl;

							swap_colors = !swap_colors;
							current_order_count++;

							if (res->next() && current_order_count == max_order_count)
								next_page = true;
							else
								next_page = false;

							res->previous();
						}

						previous_page = (max_order_count > 10);

						delete res;
						delete pstmt;
					}
					catch (sql::SQLException& e)
					{
						cout << Colors.ColorSet("91") << "\nError retrieving order history: " << e.what() << endl << Colors.ColorReset();
					}

					if (current_order_count % 10 != 0)
					{
						int empty_line = 10 - (current_order_count % 10);
						for (int i = 0; i < empty_line; i++)
							cout << endl << endl;
					}

					cout << string(155, '-') << endl;
					
					cout << "\nEnter Component Name to search (0 to reset): ";
					getline(cin, search_string);

					if (search_string == "0")
						search_string = "";

					current_order_count = 0;
					max_order_count = 10;
					next_page = false;
					previous_page = false;
				}
				else if (choice == 2 && next_page)
				{
					max_order_count += 10;
				}
				else if ((choice == 3 && previous_page) || (choice == 2 && !next_page && previous_page))
				{
					current_order_count -= 10;
					max_order_count -= 10;
				}
			}
			else
			{
				cout << Colors.ColorSet("91") << "\nError: Invalid choice!" << Colors.ColorReset();
				this_thread::sleep_for(chrono::seconds(2));
			}
		}
		else
		{
			cout << Colors.ColorSet("91") << "\nError: Invalid choice!" << Colors.ColorReset();
			this_thread::sleep_for(chrono::seconds(2));
		}
		
	}
}
