#pragma once
#include<iostream>
#include<string>
#include<iomanip>
#include<cstdlib>  //For system("CLS")
#include<ctime>
#include<mysql/jdbc.h>

using namespace std;

class colors {
public:
    string ColorSet(const string& code)
    {
        return "\033[" + code + "m";

        // 0 = Default
        // 31 = Red
        // 32 = Green
        // 33 = Yellow
        // 37 = Gray
        // 91 = Bright red
        // 94 = Bright blue
        // 95 = Bright magenta
        // 96 = Bright cyan
        // 97 = Light gray
    }

    string ColorReset()
    {
        return "\033[0m";
    }
};

extern colors Colors;

class UserInterface {
public:
    void PrintHeader(const string& title, const string& role = "", const string& username = "")
    {
        system("CLS");
        cout << string(155, '=') << endl;
        cout << "|" << setw(153) << " " << "|" << endl;
        cout << "|" << setw(60) << " " << Colors.ColorSet("96") << "Computer Components Trading System" << Colors.ColorReset() << setw(59) << " " << "|" << endl;
        cout << "|" << setw(153) << " " << "|" << endl;
        cout << string(155, '=') << endl;

        if (!role.empty() && !username.empty()) {
            cout << left << setw(15) << role << right << setw(140) << username << endl;
        }
    }

    void PrintTopLine()
    {
        cout << "______________________________________________________" << endl;
        cout << "|" << setw(52) << " " << "|" << endl;
    }

    void PrintFooter(int min_choice, int max_choice)
    {
        cout << "|____________________________________________________|" << endl;
        cout << "\nYour choice (" << min_choice << " - " << max_choice << "): ";
    }

    void MainMenu(string& scene, int& min_choice, int& max_choice)
    {
        scene = "MainMenu";
        PrintHeader("Main Menu");
        PrintTopLine();
        cout << "|   " << Colors.ColorSet("33") << "[1] " << Colors.ColorReset() << left << setw(45) << "Login to Existing Account" << "|" << endl;
        cout << "|   " << Colors.ColorSet("33") << "[2] " << Colors.ColorReset() << left << setw(45) << "Register New Account" << "|" << endl;
        cout << "|   " << Colors.ColorSet("33") << "[3] " << Colors.ColorReset() << left << setw(45) << "Exit System" << "|" << endl;
        PrintFooter(1, 3);
        min_choice = 1;
        max_choice = 3;
    }

    void AdminUI(string& scene, int& min_choice, int& max_choice, const string& username)
    {
        scene = "AdminUI";
        PrintHeader("Admin Menu", "Admin", username);
        PrintTopLine();
        cout << "|   " << Colors.ColorSet("33") << "[1] " << Colors.ColorReset() << left << setw(45) << "Manage Users" << "|" << endl;
        cout << "|   " << Colors.ColorSet("33") << "[2] " << Colors.ColorReset() << left << setw(45) << "Manage Computer Components Inventory" << "|" << endl;
        cout << "|   " << Colors.ColorSet("33") << "[3] " << Colors.ColorReset() << left << setw(45) << "Order Processing" << "|" << endl;
        cout << "|   " << Colors.ColorSet("33") << "[4] " << Colors.ColorReset() << left << setw(45) << "Monthly Order Trend Analysis" << "|" << endl;
        cout << "|   " << Colors.ColorSet("33") << "[5] " << Colors.ColorReset() << left << setw(45) << "Monthly Sales Analysis" << "|" << endl;
        cout << "|   " << Colors.ColorSet("33") << "[6] " << Colors.ColorReset() << left << setw(45) << "Annual Sales Report" << "|" << endl;
        cout << "|   " << Colors.ColorSet("33") << "[0] " << Colors.ColorReset() << left << setw(45) << "Return to Main Menu" << "|" << endl;
        PrintFooter(0, 6);
        min_choice = 0;
        max_choice = 6;
    }

    void CustomerUI(sql::Connection* con, string& scene, int& min_choice, int& max_choice, const string& username, int& current_cc_count,
        int& max_cc_count, bool& next_page, bool& previous_page, string search_string, string search_type)
    {
        scene = "CustomerUI";
        PrintHeader("Customer Menu", "Customer", username);
        cout << endl;
        ShowComponentsTable_UsersView(con, current_cc_count, max_cc_count, next_page, previous_page, search_string, search_type);

        PrintTopLine();
        cout << "|   " << Colors.ColorSet("33") << "[1] " << Colors.ColorReset() << left << setw(45) << "Search Computer Components" << "|" << endl;
        cout << "|   " << Colors.ColorSet("33") << "[2] " << Colors.ColorReset() << left << setw(45) << "Place Order" << "|" << endl;
        cout << "|   " << Colors.ColorSet("33") << "[3] " << Colors.ColorReset() << left << setw(45) << "View Orders History" << "|" << endl;
        int optionIndex = 4;
        if (next_page)
            cout << "|   " << Colors.ColorSet("33") << "[" << optionIndex++ << "]" << Colors.ColorReset() << left << setw(46) << " Next Page" << "|" << endl;
        if (previous_page)
            cout << "|   " << Colors.ColorSet("33") << "[" << optionIndex++ << "]" << Colors.ColorReset() << left << setw(46) << " Previous Page" << "|" << endl;
        cout << "|   " << Colors.ColorSet("33") << "[0] " << Colors.ColorReset() << left << setw(45) << "Return to Main Menu" << "|" << endl;
        PrintFooter(0, 3);
        min_choice = 0;
        max_choice = optionIndex - 1;
    }

    void ShowUsersTable(sql::Connection* con, int& current_users_count, int& max_users_count, bool& next_page, bool& previous_page, string temp_statement)
    {
        cout << left << Colors.ColorSet("1;33")
            << setw(10) << "UserID"
            << setw(45) << "Name"
            << setw(35) << "Email"
            << setw(25) << "Password"
            << setw(12) << "Role"
            << setw(10) << "Status"
            << setw(15) << "DateRegistered"
            << Colors.ColorReset() << endl;
        cout << string(155, '-') << endl;
        try {
            sql::PreparedStatement* pstmt;
            sql::ResultSet* res;
            
            string query = "SELECT * FROM users";

            if (temp_statement != "")
            {
                query += " WHERE Name LIKE ?";
            }

            query += " ORDER BY UserID ASC";

            pstmt = con->prepareStatement(query);

            if (temp_statement != "")
            {
                pstmt->setString(1, "%" + temp_statement + "%");
            }

            res = pstmt->executeQuery();

            current_users_count -= (current_users_count % 10);  //Reset the pointer to the start of every 10

            if (current_users_count == max_users_count)
                current_users_count -= 10;

            for (int i = 0; i < current_users_count; i++)
                res->next();

            if (!res->next())
            {
                for (int i = 0; i < 10; i++)
                {
                    cout << endl << endl;
                    next_page = false;
                    previous_page = false;
                }
            }
            res->previous();

            bool swap_colors = false;
            while (res->next() && current_users_count < max_users_count) {
                if (swap_colors)
                    cout << Colors.ColorSet("37");
                else if (!swap_colors)
                    cout << Colors.ColorSet("97");
                cout << left
                    << setw(10) << res->getString("UserID")
                    << setw(45) << res->getString("Name")
                    << setw(35) << res->getString("Email")
                    << setw(25) << res->getString("Password")
                    << setw(12) << res->getString("Role")
                    << setw(10) << res->getString("Status")
                    << setw(15) << res->getString("DateRegistered")
                    << endl << endl << Colors.ColorReset();
                swap_colors = !swap_colors;

                current_users_count++;

                if (res->next() && current_users_count == max_users_count)
                    next_page = true;
                else
                    next_page = false;

                res->previous();
            }
            if (max_users_count > 10)
                previous_page = true;
            else
                previous_page = false;

            delete res;
            delete pstmt;
        }
        catch (sql::SQLException& e) {
            cout << "\nError retrieving users: " << e.what() << endl;
        }
        if (current_users_count % 10 != 0)
        {
            int empty_line = 10 - (current_users_count % 10);
            for (int i = 0; i < empty_line; i++)
                cout << endl << endl;
        }
        cout << string(155, '-') << endl;
    }

    void ManageUsersUI(sql::Connection* con, string& scene, int& min_choice, int& max_choice, const string& username,
        int& current_users_count, int& max_users_count, bool& next_page, bool& previous_page, string temp_statement)
    {
        scene = "ManageUsersUI";
        PrintHeader("Users Management Menu", "Admin", username);
        cout << endl;
        ShowUsersTable(con, current_users_count, max_users_count, next_page, previous_page, temp_statement);
        PrintTopLine();
        cout << "|   " << Colors.ColorSet("33") << "[1] " << Colors.ColorReset() << left << setw(45) << "Search User" << "|" << endl;
        cout << "|   " << Colors.ColorSet("33") << "[2] " << Colors.ColorReset() << left << setw(45) << "Add User" << "|" << endl;
        cout << "|   " << Colors.ColorSet("33") << "[3] " << Colors.ColorReset() << left << setw(45) << "Delete User" << "|" << endl;
        cout << "|   " << Colors.ColorSet("33") << "[4] " << Colors.ColorReset() << left << setw(45) << "Edit User" << "|" << endl;
        if (next_page)
        {
            cout << "|   " << Colors.ColorSet("33") << "[5] " << Colors.ColorReset() << left << setw(45) << "Next Page" << "|" << endl;
            max_choice = 5;
            if (previous_page)
            {
                cout << "|   " << Colors.ColorSet("33") << "[6] " << Colors.ColorReset() << left << setw(45) << "Previous Page" << "|" << endl;
                max_choice = 6;
            }
        }
        else if (previous_page)
        {
            cout << "|   " << Colors.ColorSet("33") << "[5] " << Colors.ColorReset() << left << setw(45) << "Previous Page" << "|" << endl;
            max_choice = 5;
        }
        else
            max_choice = 4;
        cout << "|   " << Colors.ColorSet("33") << "[0] " << Colors.ColorReset() << left << setw(45) << "Return" << "|" << endl;
        PrintFooter(0, max_choice);
        min_choice = 0;
    }

    string TruncateWithEllipsis(const string& text, size_t width)
    {
        if (text.length() <= width)
            return text;

        if (width <= 3)
            return text.substr(0, width);

        return text.substr(0, width - 3) + "...";
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

    void ShowComponentsTable(sql::Connection* con, int& current_cc_count, int& max_cc_count, bool& next_page, bool& previous_page,
        string search_string, string search_type)
    {
        cout << left << Colors.ColorSet("1;33")
            << setw(12) << "ComponentID"
            << setw(55) << "Name"
            << setw(15) << "Category"
            << setw(23) << "Description"
            << setw(15) << "CostPrice(RM)"
            << setw(15) << "UnitPrice(RM)"
            << setw(8) << "Stock"
            << setw(12) << "DateAdded"
            << Colors.ColorReset() << endl;
        cout << string(155, '-') << endl;

        try {
            sql::PreparedStatement* pstmt;
            sql::ResultSet* res;

            string query = "SELECT * FROM components";

            // Add search filter
            if (search_type == "Name")
                query += " WHERE Name LIKE ?";
            else if (search_type == "Category")
                query += " WHERE Category = ?";

            query += " ORDER BY ComponentID ASC";

            pstmt = con->prepareStatement(query);

            if (search_string != "")
            {
                if (search_type == "Name")
                    pstmt->setString(1, "%" + search_string + "%");
                else if (search_type == "Category")
                    pstmt->setString(1, search_string);
            }

            res = pstmt->executeQuery();

            current_cc_count -= (current_cc_count % 10);

            if (current_cc_count == max_cc_count)
                current_cc_count -= 10;

            for (int i = 0; i < current_cc_count; i++)
                res->next();

            if (!res->next())
            {
                for (int i = 0; i < 10; i++)
                {
                    cout << endl << endl;
                    next_page = false;
                    previous_page = false;
                }
            }
            res->previous();

            bool swap_colors = false;
            while (res->next() && current_cc_count < max_cc_count) {
                string name = TruncateWithEllipsis(res->getString("Name"), 53);
                string desc = TruncateWithEllipsis(res->getString("Description"), 20);
                if (swap_colors)
                    cout << Colors.ColorSet("37");
                else if (!swap_colors)
                    cout << Colors.ColorSet("97");
                cout << left
                    << setw(12) << res->getString("ComponentID")
                    << setw(55) << name
                    << setw(15) << res->getString("Category")
                    << setw(23) << desc
                    << setw(15) << res->getString("CostPrice")
                    << setw(15) << res->getString("UnitPrice")
                    << setw(8) << res->getString("QuantityInStock")
                    << setw(12) << res->getString("DateAdded")
                    << endl << endl << Colors.ColorReset();

                swap_colors = !swap_colors;
                current_cc_count++;

                if (res->next() && current_cc_count == max_cc_count)
                    next_page = true;
                else
                    next_page = false;

                res->previous();
            }

            previous_page = (max_cc_count > 10);

            delete res;
            delete pstmt;
        }
        catch (sql::SQLException& e) {
            cout << "\nError retrieving components: " << e.what() << endl;
        }

        if (current_cc_count % 10 != 0) {
            int empty_line = 10 - (current_cc_count % 10);
            for (int i = 0; i < empty_line; i++)
                cout << endl << endl;
        }

        cout << string(155, '-') << endl;
    }

    void ShowComponentsTable_UsersView(sql::Connection* con, int& current_cc_count, int& max_cc_count, bool& next_page, bool& previous_page,
        string search_string, string search_type)
    {
        cout << left << Colors.ColorSet("1;33")
            << setw(12) << "ComponentID"
            << setw(55) << "Name"
            << setw(15) << "Category"
            << setw(50) << "Description"
            << setw(15) << "UnitPrice(RM)"
            << setw(8) << "Stock"
            << Colors.ColorReset() << endl;
        cout << string(155, '-') << endl;

        try {
            sql::PreparedStatement* pstmt;
            sql::ResultSet* res;

            string query = "SELECT * FROM components";

            // Add search filter
            if (search_type == "Name")
                query += " WHERE Name LIKE ?";
            else if (search_type == "Category")
                query += " WHERE Category = ?";

            query += " ORDER BY ComponentID ASC";

            pstmt = con->prepareStatement(query);

            if (search_string != "")
            {
                if (search_type == "Name")
                    pstmt->setString(1, "%" + search_string + "%");
                else if (search_type == "Category")
                    pstmt->setString(1, search_string);
            }

            res = pstmt->executeQuery();

            current_cc_count -= (current_cc_count % 10);

            if (current_cc_count == max_cc_count)
                current_cc_count -= 10;

            for (int i = 0; i < current_cc_count; i++)
                res->next();

            if (!res->next())
            {
                for (int i = 0; i < 10; i++)
                {
                    cout << endl << endl;
                    next_page = false;
                    previous_page = false;
                }
            }
            res->previous();

            bool swap_colors = false;
            while (res->next() && current_cc_count < max_cc_count) {
                string name = TruncateWithEllipsis(res->getString("Name"), 53);
                string desc = TruncateWithEllipsis(res->getString("Description"), 47);
                if (swap_colors)
                    cout << Colors.ColorSet("37");
                else if (!swap_colors)
                    cout << Colors.ColorSet("97");
                cout << left
                    << setw(12) << res->getString("ComponentID")
                    << setw(55) << name
                    << setw(15) << res->getString("Category")
                    << setw(50) << desc
                    << setw(15) << res->getString("UnitPrice")
                    << setw(8) << res->getString("QuantityInStock")
                    << endl << endl << Colors.ColorReset();

                swap_colors = !swap_colors;
                current_cc_count++;

                if (res->next() && current_cc_count == max_cc_count)
                    next_page = true;
                else
                    next_page = false;

                res->previous();
            }

            previous_page = (max_cc_count > 10);

            delete res;
            delete pstmt;
        }
        catch (sql::SQLException& e)
        {
            cout << "\nError retrieving components: " << e.what() << endl;
        }

        if (current_cc_count % 10 != 0) {
            int empty_line = 10 - (current_cc_count % 10);
            for (int i = 0; i < empty_line; i++)
                cout << endl << endl;
        }

        cout << string(155, '-') << endl;
    }

    void ManageCCInventoryUI
    (sql::Connection* con, string& scene, int& min_choice, int& max_choice, const string& username, int& current_cc_count,
        int& max_cc_count, bool& next_page, bool& previous_page, string search_string, string search_type)
    {
        scene = "ManageCCInventoryUI";
        PrintHeader("Components Inventory Management", "Admin", username);
        cout << endl;

        ShowComponentsTable(con, current_cc_count, max_cc_count, next_page, previous_page, search_string, search_type);

        PrintTopLine();
        cout << "|   " << Colors.ColorSet("33") << "[1] " << Colors.ColorReset() << left << setw(45) << "Search Components" << "|" << endl;
        cout << "|   " << Colors.ColorSet("33") << "[2] " << Colors.ColorReset() << left << setw(45) << "Add Component" << "|" << endl;
        cout << "|   " << Colors.ColorSet("33") << "[3] " << Colors.ColorReset() << left << setw(45) << "Delete Component" << "|" << endl;
        cout << "|   " << Colors.ColorSet("33") << "[4] " << Colors.ColorReset() << left << setw(45) << "Edit Component" << "|" << endl;

        if (next_page) {
            cout << "|   " << Colors.ColorSet("33") << "[5] " << Colors.ColorReset() << left << setw(45) << "Next Page" << "|" << endl;
            max_choice = 5;

            if (previous_page) {
                cout << Colors.ColorSet("33") << "[6] " << Colors.ColorReset() << left << setw(45) << "Previous Page" << "|" << endl;
                max_choice = 6;
            }
        }
        else if (previous_page) {
            cout << "|   " << Colors.ColorSet("33") << "[5] " << Colors.ColorReset() << left << setw(45) << "Previous Page" << "|" << endl;
            max_choice = 5;
        }
        else max_choice = 4;

        cout << "|   " << Colors.ColorSet("33") << "[0] " << Colors.ColorReset() << left << setw(45) << "Return" << "|" << endl;

        PrintFooter(0, max_choice);
        min_choice = 0;
    }

    void ShowOrdersTable(sql::Connection* con, int& current_order_count, int& max_order_count,
        bool& next_page, bool& previous_page, string search_order)
    {
        cout << left << Colors.ColorSet("1;33")
            << setw(10) << "OrderID"
            << setw(35) << "Customer"
            << setw(60) << "Component Name"
            << setw(10) << "Qty"
            << setw(14) << "Total(RM)"
            << setw(15) << "Date"
            << setw(12) << "Status"
            << Colors.ColorReset() << endl;

        cout << string(155, '-') << endl;

        try
        {
            sql::PreparedStatement* pstmt;
            sql::ResultSet* res;

            string query =
                "SELECT orders.OrderID, users.Name AS CustomerName, components.Name AS ComponentName, orders.Quantity, orders.TotalAmount, orders.DateOrdered, orders.Status "
                "FROM orders "
                "JOIN users ON orders.UserID = users.UserID "
                "JOIN components ON orders.ComponentID = components.ComponentID "
                "WHERE 1=1 ";

            if (search_order != "")
                query += "AND (orders.OrderID LIKE ? OR users.Name LIKE ? OR components.Name LIKE ?) ";

            query += "ORDER BY (orders.Status='Pending') DESC, orders.OrderID ASC";

            pstmt = con->prepareStatement(query);

            if (search_order != "")
            {
                pstmt->setString(1, "%" + search_order + "%");
                pstmt->setString(2, "%" + search_order + "%");
                pstmt->setString(3, "%" + search_order + "%");
            }

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
                    next_page = false;
                    previous_page = false;
                }
            }
            res->previous();

            bool swap_colors = false;
            while (res->next() && current_order_count < max_order_count) {
                if (swap_colors)
                    cout << Colors.ColorSet("37");
                else if (!swap_colors)
                    cout << Colors.ColorSet("97");

                cout << left
                    << setw(10) << res->getString("OrderID")
                    << setw(35) << TruncateWithEllipsis(res->getString("CustomerName"), 33)
                    << setw(60) << TruncateWithEllipsis(res->getString("ComponentName"), 58)
                    << setw(10) << res->getString("Quantity")
                    << setw(14) << res->getString("TotalAmount")
                    << setw(15) << res->getString("DateOrdered");
                if (res->getString("Status") == "Delivered")
                    cout << Colors.ColorSet("92");
                else if (res->getString("Status") == "Pending")
                    cout << Colors.ColorSet("93");
                cout << setw(12) << res->getString("Status")
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
        catch (sql::SQLException& e) {
            cout << "\nError retrieving orders: " << e.what() << endl;
        }

        if (current_order_count % 10 != 0) {
            int empty_line = 10 - (current_order_count % 10);
            for (int i = 0; i < empty_line; i++)
                cout << endl << endl;
        }

        cout << string(155, '-') << endl << endl;
    }

    void OrderProcessingUI(sql::Connection* con, string& scene, int& min_choice, int& max_choice, const string& username, int& current_order_count,
        int& max_order_count, bool& next_page, bool& previous_page, string search_order)
    {
        PrintHeader("Order Processing", "Admin", username);
        cout << endl;

        ShowOrdersTable(con, current_order_count, max_order_count, next_page, previous_page, search_order);
        PrintTopLine();
        cout << "|   " << Colors.ColorSet("33") << "[1] " << Colors.ColorReset() << left << setw(45) << "Search Orders" << "|" << endl;
        cout << "|   " << Colors.ColorSet("33") << "[2] " << Colors.ColorReset() << left << setw(45) << "Delivery Orders" << "|" << endl;
        cout << "|   " << Colors.ColorSet("33") << "[3] " << Colors.ColorReset() << left << setw(45) << "Delete Orders" << "|" << endl;

        int optionIndex = 4;
        if (next_page)
            cout << "|   " << Colors.ColorSet("33") << "[" << optionIndex++ << "]" << Colors.ColorReset() << left << setw(46) << " Next Page" << "|" << endl;

        if (previous_page)
            cout << "|   " << Colors.ColorSet("33") << "[" << optionIndex++ << "]" << Colors.ColorReset() << left << setw(46) << " Previous Page" << "|" << endl;

        cout << "|   " << Colors.ColorSet("33") << "[0] " << Colors.ColorReset() << left << setw(45) << "Exit" << "|" << endl;

        min_choice = 0;
        max_choice = optionIndex - 1;

        PrintFooter(min_choice, max_choice);

        scene = "OrderProcessingUI";
    }

    void ShowReceipt(string receiptID, string customerName, string componentID, string componentName, string category, double unitPrice, int quantity)
    {
        double total = unitPrice * quantity;

        time_t now = time(0);
        char dt[26];
        ctime_s(dt, sizeof(dt), &now);

        vector<string> lines;
        lines = WrapText(componentName, 49);

        system("CLS");
        cout << string(70, '=') << endl;
        cout << "|" << setw(68) << " " << "|" << endl;
        cout << "|" << setw(21) << " " << "Component Purchase Receipt" << setw(21) << " " << "|" << endl;
        cout << "|" << setw(68) << " " << "|" << endl;
        cout << string(70, '=') << endl << endl;

        cout << left << setw(20) << "Receipt ID" << ": " << receiptID << endl;
        cout << left << setw(20) << "Customer Name" << ": " << customerName << endl;
        cout << left << setw(20) << "Date & Time" << ": " << dt << endl;

        cout << string(70, '-') << endl;
        cout << left << setw(20) << "Component ID" << ": " << componentID << endl;
        if (!lines.empty())
        {
            cout << left << setw(20) << "Name" << ": " << lines[0] << endl;
            for (int i = 1; i < lines.size(); i++)
            {
                cout << setw(22) << "" << lines[i] << endl;
            }
        }
        else
            cout << left << setw(20) << "Name" << ": " << componentName << endl;
        cout << left << setw(20) << "Category" << ": " << category << endl;
        cout << left << setw(20) << "Unit Price" << ": RM " << fixed << setprecision(2) << unitPrice << endl;
        cout << left << setw(20) << "Quantity" << ": " << quantity << endl;
        cout << string(70, '-') << endl;
        cout << left << setw(20) << "Total Payment" << ": RM " << fixed << setprecision(2) << total << endl << endl;

        cout << string(70, '=') << endl;
        cout << left << setw(20) << "" << "Thank you for shopping with us!" << endl;
        cout << string(70, '=') << endl;
    }

    void ShowOrderTrends(sql::Connection* con, int month, int year)
    {
        try
        {
            static const string categories[5] = { "CPU", "RAM", "Motherboard", "GPU", "Storage" };
            static const string Months[] = { "", "January","February","March","April","May","June","July","August","September", "October","November","December" };

            struct TrendData
            {
                string topName = "N/A";
                int topSold = 0;
                string worstName = "N/A";
                int worstSold = 0;
            };

            TrendData trend[5];

            sql::PreparedStatement* pstmt;
            sql::ResultSet* res;

            for (int i = 0; i < 5; i++)
            {
                pstmt = con->prepareStatement(
                    "SELECT c.Name, IFNULL(SUM(o.Quantity),0) AS Sold "
                    "FROM components c "
                    "LEFT JOIN orders o ON c.ComponentID=o.ComponentID "
                    "AND MONTH(o.DateOrdered)=? AND YEAR(o.DateOrdered)=? "
                    "WHERE c.Category=? "
                    "GROUP BY c.ComponentID "
                    "ORDER BY Sold DESC"
                );

                pstmt->setInt(1, month);
                pstmt->setInt(2, year);
                pstmt->setString(3, categories[i]);

                res = pstmt->executeQuery();

                bool first = true;
                int rank = 0;
                int lastSold = 99999999;

                while (res->next())
                {
                    int sold = res->getInt("Sold");
                    string name = res->getString("Name");

                    if (first)
                    {
                        trend[i].topName = name;
                        trend[i].topSold = sold;
                        first = false;
                    }

                    if (sold <= lastSold)
                    {
                        trend[i].worstName = name;
                        trend[i].worstSold = sold;
                        lastSold = sold;
                    }

                    rank++;
                }

                delete res;
                delete pstmt;
            }

            cout << endl << string(145, '=') << endl;
            cout << left << setw(144) << "|" << "|" << endl;
            cout << left << setw(62) << "|" << "Order Trends for " << Months[month] << " " << year << right << setw(62 - Months[month].length()) << "|\n";
            cout << left << setw(144) << "|" << "|" << endl;
            cout << string(145, '=') << endl << endl;

            cout << Colors.ColorSet("93")
                << left << setw(20) << "Category"
                << setw(50) << "Top Selling Component"
                << right << setw(10) << "Sold"
                << "     "
                << left << setw(50) << "Worst Selling Component"
                << right << setw(10) << "Sold"
                << Colors.ColorReset() << endl;

            cout << string(145, '-') << endl;

            for (int i = 0; i < 5; i++)
            {
                trend[i].topName = TruncateWithEllipsis(trend[i].topName, 48);
                trend[i].worstName = TruncateWithEllipsis(trend[i].worstName, 48);
                cout << left << setw(20) << categories[i]
                    << Colors.ColorSet("92")
                    << setw(50) << trend[i].topName
                    << right << setw(10) << trend[i].topSold
                    << Colors.ColorReset()
                    << "     "
                    << Colors.ColorSet("91")
                    << left << setw(50) << trend[i].worstName
                    << right << setw(10) << trend[i].worstSold
                    << Colors.ColorReset()
                    << endl << endl;
            }

            cout << string(145, '-') << endl;
        }
        catch (sql::SQLException& e)
        {
            cout << Colors.ColorSet("91") << "\nSQL ERROR: " << e.what() << Colors.ColorReset();
        }
    }

    void ShowCategoryDetails(sql::Connection* con, const string& category, int month, int year, int startIndex, bool& next_page, bool& previous_page)
    {
        try
        {
            sql::PreparedStatement* pstmt;
            sql::ResultSet* res;

            pstmt = con->prepareStatement(
                "SELECT c.Name, IFNULL(SUM(o.Quantity),0) AS Sold "
                "FROM components c "
                "LEFT JOIN orders o ON c.ComponentID = o.ComponentID "
                "AND MONTH(o.DateOrdered)=? AND YEAR(o.DateOrdered)=? "
                "WHERE c.Category=? "
                "GROUP BY c.ComponentID "
                "ORDER BY Sold DESC"
            );

            pstmt->setInt(1, month);
            pstmt->setInt(2, year);
            pstmt->setString(3, category);

            res = pstmt->executeQuery();

            // Skip rows before startIndex (pagination)
            int skip = startIndex;
            while (skip-- > 0 && res->next());

            cout << endl << Colors.ColorSet("93")
                << left << setw(80) << "     Component Name"
                << right << setw(15) << "Sold"
                << Colors.ColorReset() << endl;

            cout << string(95, '-') << endl;

            if (!res->next())
            {
                for (int i = 0; i < 10; i++)
                {
                    cout << endl << endl;
                    next_page = false;
                    previous_page = false;
                }
            }
            res->previous();

            int shown = 0;

            while (res->next() && shown < 10)
            {
                string name = TruncateWithEllipsis(res->getString("Name"), 78);

                cout << right << "("  << setw(2) << shown + 1 << ") "
                    << left << setw(80) << name
                    << right << setw(10) << res->getInt("Sold")
                    << endl << endl;

                shown++;
            }
            res->previous();

            // Fill blank rows if fewer than 10
            for (int i = shown; i < 10; i++)
                cout << endl << endl;

            cout << string(95, '-') << endl;

            if (res->next() && shown == 10)
            {
                next_page = true;
            }
            res->previous();

            previous_page = (startIndex > 0);

            delete res;
            delete pstmt;
        }
        catch (sql::SQLException& e)
        {
            cout << Colors.ColorSet("91") << "\nSQL Error: " << e.what() << Colors.ColorReset();
        }
    }

    void OrderTrendAnalysisUI(sql::Connection* con, string& scene, int& min_choice, int& max_choice, const string& username, int month, int year)
    {
        PrintHeader("Order Trend Analysis", "Admin", username);

        ShowOrderTrends(con, month, year);

        PrintTopLine();
        cout << "|   " << Colors.ColorSet("33") << "[1] " << Colors.ColorReset() << left << setw(45) << "View Details" << "|\n";
        cout << "|   " << Colors.ColorSet("33") << "[2] " << Colors.ColorReset() << left << setw(45) << "Choose Another Month & Year" << "|\n";
        cout << "|   " << Colors.ColorSet("33") << "[0] " << Colors.ColorReset() << left << setw(45) << "Exit" << "|\n";

        min_choice = 0;
        max_choice = 2;

        PrintFooter(min_choice, max_choice);

        scene = "OrderTrendAnalysisUI";
    }

    void ShowSalesAnalysis(sql::Connection* con, int month, int year, int& current_cc_count, int& max_cc_count,
        bool& next_page, bool& previous_page, string search_string, string search_type)
    {
        string Months[] = { "", "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };

        cout << endl << string(153, '=') << endl;
        cout << left << setw(152) << "|" << "|" << endl;
        cout << left << setw(60) << "|"
            << "Sales Analysis of " << setw(10) << Months[month] << year
            << right << setw(61) << "|" << endl;
        cout << left << setw(152) << "|" << "|" << endl;
        cout << string(153, '=') << endl; 

        cout << left << Colors.ColorSet("93") << endl
            << setw(12) << "ID"
            << setw(60) << "Component Name"
            << setw(15) << "Category"
            << right
            << setw(8) << "Sold"
            << setw(16) << "Revenue(RM)"
            << setw(16) << "Cost(RM)"
            << setw(16) << "Profit(RM)"
            << setw(10) << "Margin%"
            << Colors.ColorReset() << endl;

        cout << string(153, '-') << endl;

        try {
            sql::PreparedStatement* pstmt;
            sql::ResultSet* res;

            string query =
                "SELECT c.ComponentID, c.Name, c.Category, "
                "SUM(o.Quantity) AS Sold, "
                "SUM(o.Quantity * c.UnitPrice) AS Revenue, "
                "SUM(o.Quantity * c.CostPrice) AS Cost "
                "FROM components c "
                "LEFT JOIN orders o ON c.ComponentID = o.ComponentID "
                "AND MONTH(o.DateOrdered)=? AND YEAR(o.DateOrdered)=? ";

            if (search_type == "Name")
                query += "WHERE c.Name LIKE ? ";
            else if (search_type == "Category")
                query += "WHERE c.Category = ? ";

            query += "GROUP BY c.ComponentID "
                "ORDER BY Revenue DESC";

            pstmt = con->prepareStatement(query);
            pstmt->setInt(1, month);
            pstmt->setInt(2, year);

            if (search_string != "")
                pstmt->setString(3, search_type == "Name" ? "%" + search_string + "%" : search_string);

            res = pstmt->executeQuery();

            current_cc_count -= current_cc_count % 10;

            if (current_cc_count == max_cc_count)
                current_cc_count -= 10;

            for (int i = 0; i < current_cc_count; i++)
                res->next();

            bool swap = false;
            int shown = 0;

            if (!res->next())
            {
                for (int i = 0; i < 10; i++)
                    cout << endl << endl;
                next_page = false;
                previous_page = false;
            }
            res->previous();

            while (res->next() && shown < 10) {
                double revenue = res->getDouble("Revenue");
                double cost = res->getDouble("Cost");
                int sold = res->getInt("Sold");

                double profit = revenue - cost;
                double margin = (revenue == 0) ? 0 : (profit / revenue * 100);

                cout << (swap ? Colors.ColorSet("37") : Colors.ColorSet("97"))
                    << left
                    << setw(12) << res->getString("ComponentID")
                    << setw(60) << TruncateWithEllipsis(res->getString("Name"), 58)
                    << setw(15) << res->getString("Category")
                    << right
                    << setw(8) << sold
                    << setw(16) << fixed << setprecision(2) << revenue
                    << setw(16) << cost
                    << setw(16) << profit;
                if (margin >= 40)
                    cout << Colors.ColorSet("92");      // Green = VERY HIGH  
                else if (margin >= 30)
                    cout << Colors.ColorSet("32");      // Light green = HIGH  
                else if (margin >= 15)
                    cout << Colors.ColorSet("33");      // Yellow = MEDIUM  
                else
                    cout << Colors.ColorSet("91");      // Red = LOW
                cout << setw(10) << fixed << setprecision(2) << margin
                    << Colors.ColorReset() << endl << endl;

                swap = !swap;
                current_cc_count++;
                shown++;

                if (res->next() && shown == 10) next_page = true;
                else next_page = false;
                res->previous();
            }

            previous_page = (current_cc_count > 10);

            delete res;
            delete pstmt;
        }
        catch (sql::SQLException& e) {
            cout << Colors.ColorSet("91")
                << "\nSQL Error: " << e.what()
                << Colors.ColorReset();
        }

        if (current_cc_count % 10 != 0) {
            int empty_line = 10 - (current_cc_count % 10);
            for (int i = 0; i < empty_line; i++)
                cout << endl << endl;
        }

        cout << string(153, '-') << endl;
    }

    void MonthlySalesAnalysisUI(sql::Connection* con, string& scene, int& min_choice, int& max_choice, const string& username,
        int month, int year, int& current_cc_count, int& max_cc_count, bool& next_page, bool& previous_page, string search_string, string search_type)
    {
        scene = "MonthlySalesAnalysisUI";

        PrintHeader("Profit Margin Report", "Admin", username);

        ShowSalesAnalysis(con, month, year, current_cc_count, max_cc_count, next_page, previous_page, search_string, search_type);

        PrintTopLine();

        cout << "|   " << Colors.ColorSet("33") << "[1] " << Colors.ColorReset() << left << setw(45) << "Search Components" << "|\n";

        cout << "|   " << Colors.ColorSet("33") << "[2] " << Colors.ColorReset() << left << setw(45) << "Search Another Month & Year" << "|\n";

        int optionIndex = 3;

        if (next_page)
        {
            cout << "|   " << Colors.ColorSet("33") << "[" << optionIndex++ << "] " << Colors.ColorReset() << left << setw(45) << "Next Page" << "|\n";
        }

        if (previous_page)
        {
            cout << "|   " << Colors.ColorSet("33") << "[" << optionIndex++ << "] " << Colors.ColorReset() << left << setw(45) << "Previous Page" << "|\n";
        }


        cout << "|   " << Colors.ColorSet("33") << "[0] " << Colors.ColorReset() << left << setw(45) << "Exit" << "|\n";

        min_choice = 0;
        max_choice = optionIndex - 1;
        PrintFooter(0, max_choice);
    }

    void ShowSalesReport(sql::Connection* con, int year)
    {
        try
        {
            sql::PreparedStatement* pstmt;
            sql::ResultSet* res;

            int salesArr[13] = { 0 };
            double revenueArr[13] = { 0 };
            double costArr[13] = { 0 };
            double profitArr[13] = { 0 };
            double marginArr[13] = { 0 };

            static const char* months[] = {
                "", "January","February","March","April","May","June",
                "July","August","September","October","November","December"
            };

            // ========== FIRST PASS: GATHER ALL MONTH DATA ==========
            for (int m = 1; m <= 12; m++)
            {
                int totalOrders = 0;
                double revenue = 0, cost = 0, profit = 0, margin = 0;

                // Get orders count
                pstmt = con->prepareStatement(
                    "SELECT COUNT(*) AS Orders FROM orders "
                    "WHERE MONTH(DateOrdered)=? AND YEAR(DateOrdered)=?");
                pstmt->setInt(1, m);
                pstmt->setInt(2, year);
                res = pstmt->executeQuery();
                if (res->next())
                    totalOrders = res->getInt("Orders");
                delete res; delete pstmt;

                // Get revenue
                pstmt = con->prepareStatement(
                    "SELECT SUM(TotalAmount) AS Revenue FROM orders "
                    "WHERE MONTH(DateOrdered)=? AND YEAR(DateOrdered)=?");
                pstmt->setInt(1, m);
                pstmt->setInt(2, year);
                res = pstmt->executeQuery();
                if (res->next())
                    revenue = res->getDouble("Revenue");
                delete res; delete pstmt;

                // Get cost
                pstmt = con->prepareStatement(
                    "SELECT SUM(o.Quantity * c.CostPrice) AS TotalCost "
                    "FROM orders o JOIN components c ON o.ComponentID = c.ComponentID "
                    "WHERE MONTH(o.DateOrdered)=? AND YEAR(o.DateOrdered)=?");
                pstmt->setInt(1, m);
                pstmt->setInt(2, year);
                res = pstmt->executeQuery();
                if (res->next())
                    cost = res->getDouble("TotalCost");
                delete res; delete pstmt;

                // Compute profit & margin
                profit = revenue - cost;
                if (revenue == 0)
                    margin = 0;
                else
                    margin = (profit / revenue) * 100;

                // Save for analysis panel
                salesArr[m] = totalOrders;
                revenueArr[m] = revenue;
                costArr[m] = cost;
                profitArr[m] = profit;
                marginArr[m] = margin;
            }

            // ========== FIND BEST & WORST MONTHS ==========
            auto findMaxIndex_int = [&](int arr[])
                {
                    int idx = 1;
                    for (int i = 2; i <= 12; i++)
                        if (arr[i] > arr[idx]) idx = i;
                    return idx;
                };

            auto findMinIndex_int = [&](int arr[])
                {
                    int idx = 1;
                    for (int i = 2; i <= 12; i++)
                        if (arr[i] < arr[idx]) idx = i;
                    return idx;
                };

            auto findMaxIndex = [&](double arr[])
                {
                    int idx = 1;
                    for (int i = 2; i <= 12; i++)
                        if (arr[i] > arr[idx]) idx = i;
                    return idx;
                };

            auto findMinIndex = [&](double arr[])
                {
                    int idx = 1;
                    for (int i = 2; i <= 12; i++)
                        if (arr[i] < arr[idx]) idx = i;
                    return idx;
                };

            int topSales = findMaxIndex_int((int*)salesArr);
            int worstSales = findMinIndex_int((int*)salesArr);

            int topRevenue = findMaxIndex(revenueArr);
            int worstRevenue = findMinIndex(revenueArr);

            int topCost = findMaxIndex(costArr);
            int worstCost = findMinIndex(costArr);

            int topProfit = findMaxIndex(profitArr);
            int worstProfit = findMinIndex(profitArr);

            int topMargin = findMaxIndex(marginArr);
            int worstMargin = findMinIndex(marginArr);

            // ========== PRINT MAIN TABLE ==========
            cout << endl << string(122, '=') << endl;
            cout << left << setw(121) << "|" << "|" << endl;
            cout << left << setw(44) << "|" << "Annual Sales Report for Year " << year << right << setw(45) << "|" << endl;
            cout << left << setw(121) << "|" << "|" << endl;
            cout << string(122, '=') << endl;

            cout << endl << Colors.ColorSet("93")
                << left
                << setw(10) << "Month"
                << right
                << setw(10) << "Sales"
                << setw(19) << "Revenue(RM)"
                << setw(19) << "Cost(RM)"
                << setw(19) << "Profit(RM)"
                << setw(12) << "Margin %"
                << "         Analysis"
                << Colors.ColorReset() << endl;

            cout << string(122, '-') << endl;

            // Color helpers
            auto printDouble = [&](double value, int width, int precision, bool isBest, bool isWorst)
            {
                cout << right;
                if (isBest)  cout << Colors.ColorSet("92");
                if (isWorst) cout << Colors.ColorSet("91");
                cout << setw(width) << fixed << setprecision(precision) << value << Colors.ColorReset();
            };

            auto printInt = [&](int value, int width, bool isBest, bool isWorst)
            {
                cout << right;
                if (isBest)  cout << Colors.ColorSet("92");
                if (isWorst) cout << Colors.ColorSet("91");
                cout << setw(width) << value << Colors.ColorReset();
            };

            // ========== PRINT MONTH ROWS + RIGHT PANEL ==========
            for (int m = 1; m <= 12; m++)
            {
                cout << left << setw(10) << months[m];

                printInt(salesArr[m], 10, m == topSales, m == worstSales);

                printDouble(revenueArr[m], 19, 2, m == topRevenue, m == worstRevenue);

                printDouble(costArr[m], 19, 2, m == topCost, m == worstCost);

                printDouble(profitArr[m], 19, 2, m == topProfit, m == worstProfit);

                printDouble(marginArr[m], 12, 1, m == topMargin, m == worstMargin);

                // ========= Right Analysis Panel =========
                if (m == 1)
                {
                    cout << left << setw(22) << "    |    Top Sales" << ": " << Colors.ColorSet("92") << months[topSales] << Colors.ColorReset();
                    cout << endl << setw(93) << "" << "|" << endl;
                }
                else if (m == 2)
                {
                    cout << left << setw(22) << "    |    Worst Sales" << ": " << Colors.ColorSet("91") << months[worstSales] << Colors.ColorReset();
                    cout << endl << setw(93) << "" << "|" << endl;

                }
                else if (m == 3)
                {
                    cout << left << setw(22) << "    |    Top Revenue" << ": " << Colors.ColorSet("92") << months[topRevenue] << Colors.ColorReset();
                    cout << endl << setw(93) << "" << "|" << endl;

                }
                else if (m == 4)
                {
                    cout << left << setw(22) << "    |    Worst Revenue" << ": " << Colors.ColorSet("91") << months[worstRevenue] << Colors.ColorReset();
                    cout << endl << setw(93) << "" << "|" << endl;

                }
                else if (m == 5)
                {
                    cout << left << setw(22) << "    |    Most Cost" << ": " << Colors.ColorSet("92") << months[topCost] << Colors.ColorReset();
                    cout << endl << setw(93) << "" << "|" << endl;

                }
                else if (m == 6)
                {
                    cout << left << setw(22) << "    |    Least Cost" << ": " << Colors.ColorSet("91") << months[worstCost] << Colors.ColorReset();
                    cout << endl << setw(93) << "" << "|" << endl;

                }
                else if (m == 7)
                {
                    cout << left << setw(22) << "    |    Top Profit" << ": " << Colors.ColorSet("92") << months[topProfit] << Colors.ColorReset();
                    cout << endl << setw(93) << "" << "|" << endl;

                }
                else if (m == 8)
                {
                    cout << left << setw(22) << "    |    Worst Profit" << ": " << Colors.ColorSet("91") << months[worstProfit] << Colors.ColorReset();
                    cout << endl << setw(93) << "" << "|" << endl;

                }
                else if (m == 9)
                {
                    cout << left << setw(22) << "    |    Top Margin" << ": " << Colors.ColorSet("92") << months[topMargin] << Colors.ColorReset();
                    cout << endl << setw(93) << "" << "|" << endl;

                }
                else if (m == 10)
                {
                    cout << left << setw(22) << "    |    Worst Margin" << ": " << Colors.ColorSet("91") << months[worstMargin] << Colors.ColorReset();
                    cout << endl << setw(93) << "" << "|" << endl;

                }
                else
                {
                    cout << left << setw(22) << "    |";
                    cout << endl << setw(93) << "" << "|" << endl;
                }
            }

            cout << string(122, '-') << endl;

        }
        catch (sql::SQLException& e)
        {
            cout << Colors.ColorSet("91") << "\nError: " << e.what() << Colors.ColorReset();
        }
    }

    void AnnualSalesReportUI(sql::Connection* con, string& scene, int& min_choice, int& max_choice, const string& username, int year)
    {
        system("CLS");
        PrintHeader("Sales Analysis", "Admin", username);

        ShowSalesReport(con, year);

        PrintTopLine();
        cout << "|   " << Colors.ColorSet("33") << "[1] " << Colors.ColorReset() << left << setw(45) << "Choose Another Year" << "|" << endl;
        cout << "|   " << Colors.ColorSet("33") << "[0] " << Colors.ColorReset() << left << setw(45) << "Exit" << "|" << endl;
        min_choice = 0;
        max_choice = 1;
        PrintFooter(min_choice, max_choice);

        scene = "SalesReportUI";
    }
};
