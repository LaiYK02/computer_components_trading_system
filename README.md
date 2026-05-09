\# Computer Components Trading System



A C++ and MySQL based trading system designed for computer hardware businesses to manage inventory, customer orders, and sales analysis efficiently.



\---



\## Overview



The Computer Components Trading System helps computer hardware merchants transition from traditional physical stores to an online trading environment. The system simplifies inventory management, customer order processing, and sales reporting while reducing dependency on third-party e-commerce platforms.



This project was developed as a university workshop project using C++ and MySQL.



\---



\## Features



\### Admin Module

\- User Management

&#x20; - Add users

&#x20; - Edit users

&#x20; - Delete users

&#x20; - Search users



\- Inventory Management

&#x20; - Add computer components

&#x20; - Edit component details

&#x20; - Delete components

&#x20; - Search components



\- Order Management

&#x20; - Search customer orders

&#x20; - Update delivery status

&#x20; - Delete orders



\- Sales \& Analysis

&#x20; - Monthly sales analysis

&#x20; - Annual sales reports

&#x20; - Order trend analysis

&#x20; - Profit margin calculation



\---



\### Customer Module

\- Register account

\- Login system

\- Browse computer components

\- Search and filter components

\- Place orders

\- View order history



\---



\## Technologies Used



\- C++

\- MySQL

\- MySQL Connector/C++

\- Visual Studio

\- Console-based User Interface



\---



\## Database Structure



The system uses three main tables:



\### Users

\- UserID

\- Name

\- Email

\- Password

\- Role

\- Status

\- DateRegistered



\### Components

\- ComponentID

\- Name

\- Category

\- Description

\- CostPrice

\- UnitPrice

\- QuantityInStock

\- DateAdded



\### Orders

\- OrderID

\- UserID

\- ComponentID

\- Quantity

\- TotalAmount

\- DateOrdered

\- Status



\---



\## System Modules



\### 1. User Management

Handles customer/admin registration and login authentication.



\### 2. Inventory Management

Allows admin to manage computer hardware inventory efficiently.



\### 3. Order Processing

Handles customer purchasing workflow and delivery status updates.



\### 4. Sales Analysis \& Reporting

Generates monthly and annual sales reports with profit calculations and trend analysis.



\---



\## Screenshots



\## Main Menu

!\[Main Menu](screenshots/main-menu.png)



\## Admin Menu

!\[Admin Menu](screenshots/admin-menu.png)



\## Customer Menu

!\[Customer Menu](screenshots/customer-menu.png)



\## Monthly Sales Analysis

!\[Monthly Sales Analysis](screenshots/monthly-sales-analysis.png)



\---



\## Installation



\### 1. Clone the repository



```bash

git clone https://github.com/LaiYK02/computer\_components\_trading\_system.git

