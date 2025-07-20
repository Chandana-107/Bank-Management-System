# Bank Management System

A simple C++ console application for managing bank accounts using binary file storage.This project allows users to create, view, update, and delete bank accounts, as well as perform deposit and withdrawal operations.All account data is stored persistently in a binary file, and every transaction is logged with a timestamp for auditing purposes.

## Features

- Create new accounts (with unique account numbers)
- Display, update, and delete accounts
- Deposit and withdraw money
- View all accounts
- Transaction log with details (timestamp, previous and new balance)
- Data stored in binary files for persistence

## File Structure

- `bank_system.cpp` — Main source code
- `accounts.dat` — Binary file storing account data (auto-created)
- `transactions.log` — Text file storing transaction logs (auto-created)

## How to Build & Run

Make sure you have a C++ compiler installed (like g++):
g++ bank_system.cpp -o bank_system
./bank_system

## Sample Menu UI
========== BANK MANAGEMENT SYSTEM ==========
1. Create New Account
2. Display Account
3. Update Account
4. Delete Account
5. Deposit Money
6. Withdraw Money
7. Display All Accounts
8. View Transaction Logs
9. Exit
===========================================

## Concepts Used
- File handling in binary mode (fstream)
- Struct-based record management
- Input validation
- Menu-driven user interface
- Logging with timestamps (<ctime>)

##  License
This project is licensed under the [MIT License](LICENSE).
