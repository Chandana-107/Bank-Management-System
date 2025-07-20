#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <string>
using namespace std;

struct Account {
    int accountNumber;
    string name;
    float balance;
    string accountType; 
};

void logTransaction(int accNo, string type, float amount, float prevBalance, float newBalance) {
    ofstream log("transactions.log", ios::app);
    time_t now = time(0);
    string dt = ctime(&now);
    dt.pop_back(); 

    log << "Account: " << accNo << " | " << type
        << " | Amount: " << amount
        << " | PrevBalance: " << prevBalance
        << " | NewBalance: " << newBalance
        << " | Time: " << dt << "\n";
    log.close();
}

void writeAccount(ofstream& file, const Account& acc) {
    file.write(reinterpret_cast<const char*>(&acc.accountNumber), sizeof(acc.accountNumber));
    size_t nameLen = acc.name.size();
    file.write(reinterpret_cast<const char*>(&nameLen), sizeof(nameLen));
    file.write(acc.name.c_str(), nameLen);

    file.write(reinterpret_cast<const char*>(&acc.balance), sizeof(acc.balance));
    size_t typeLen = acc.accountType.size();
    file.write(reinterpret_cast<const char*>(&typeLen), sizeof(typeLen));
    file.write(acc.accountType.c_str(), typeLen);
}

void writeAccount(fstream& file, const Account& acc) {
    file.write(reinterpret_cast<const char*>(&acc.accountNumber), sizeof(acc.accountNumber));
    size_t nameLen = acc.name.size();
    file.write(reinterpret_cast<const char*>(&nameLen), sizeof(nameLen));
    file.write(acc.name.c_str(), nameLen);

    file.write(reinterpret_cast<const char*>(&acc.balance), sizeof(acc.balance));
    size_t typeLen = acc.accountType.size();
    file.write(reinterpret_cast<const char*>(&typeLen), sizeof(typeLen));
    file.write(acc.accountType.c_str(), typeLen);
}

bool readAccount(ifstream& file, Account& acc) {
    if (!file.read(reinterpret_cast<char*>(&acc.accountNumber), sizeof(acc.accountNumber)))
        return false;
    size_t nameLen;
    file.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));
    acc.name.resize(nameLen);
    file.read(&acc.name[0], nameLen);

    file.read(reinterpret_cast<char*>(&acc.balance), sizeof(acc.balance));
    size_t typeLen;
    file.read(reinterpret_cast<char*>(&typeLen), sizeof(typeLen));
    acc.accountType.resize(typeLen);
    file.read(&acc.accountType[0], typeLen);

    return true;
}

bool readAccount(fstream& file, Account& acc) {
    if (!file.read(reinterpret_cast<char*>(&acc.accountNumber), sizeof(acc.accountNumber)))
        return false;
    size_t nameLen;
    file.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));
    acc.name.resize(nameLen);
    file.read(&acc.name[0], nameLen);

    file.read(reinterpret_cast<char*>(&acc.balance), sizeof(acc.balance));
    size_t typeLen;
    file.read(reinterpret_cast<char*>(&typeLen), sizeof(typeLen));
    acc.accountType.resize(typeLen);
    file.read(&acc.accountType[0], typeLen);

    return true;
}

void createAccount() {
    Account acc;
    cout << "\nEnter Account Number: ";
    while (!(cin >> acc.accountNumber) || acc.accountNumber <= 0) {
        cout << "Invalid input. Enter a positive integer for Account Number: ";
        cin.clear();
        cin.ignore(10000, '\n');
    }

    ifstream fileCheck("accounts.dat", ios::binary);
    Account temp;
    bool exists = false;
    while (readAccount(fileCheck, temp)) {
        if (temp.accountNumber == acc.accountNumber) {
            exists = true;
            break;
        }
    }
    fileCheck.close();
    if (exists) {
        cout << "Account number already exists. Try again.\n";
        return;
    }

    cout << "Enter Name: ";
    cin.ignore(); 
    getline(cin, acc.name);

    cout << "Enter Initial Balance: ";
    while (!(cin >> acc.balance) || acc.balance < 0) {
        cout << "Invalid input. Enter a non-negative number for Balance: ";
        cin.clear();
        cin.ignore(10000, '\n');
    }

    cout << "Enter Account Type (Savings/Current): ";
    cin.ignore();
    getline(cin, acc.accountType);

    ofstream file("accounts.dat", ios::binary | ios::app);
    writeAccount(file, acc);
    file.close();
    cout << "Account Created Successfully.\n";
}

void displayAccount(int accNo) {
    ifstream file("accounts.dat", ios::binary);
    Account acc;
    bool found = false;
    while (readAccount(file, acc)) {
        if (acc.accountNumber == accNo) {
            cout << "\nAccount Number: " << acc.accountNumber;
            cout << "\nName: " << acc.name;
            cout << "\nBalance: " << acc.balance << "\n";
            cout << "Account Type: " << acc.accountType << "\n";
            found = true;
            break;
        }
    }
    file.close();
    if (!found)
        cout << "Account Not Found.\n";
}

void updateAccount(int accNo) {
    fstream file("accounts.dat", ios::binary | ios::in | ios::out);
    Account acc;
    bool found = false;
    streampos prevPos;
    while (true) {
        prevPos = file.tellg();
        if (!readAccount(file, acc)) break;
        if (acc.accountNumber == accNo) {
            cout << "Enter New Name: ";
            cin.ignore();
            getline(cin, acc.name);
            cout << "Enter New Balance: ";
            while (!(cin >> acc.balance) || acc.balance < 0) {
                cout << "Invalid input. Enter a non-negative number for Balance: ";
                cin.clear();
                cin.ignore(10000, '\n');
            }
            cout << "Enter New Account Type (Savings/Current): ";
            cin.ignore();
            getline(cin, acc.accountType);

            file.seekp(prevPos);
            writeAccount(file, acc);
            found = true;
            cout << "Account Updated Successfully.\n";
            break;
        }
    }
    file.close();
    if (!found)
        cout << "Account Not Found.\n";
}

void deleteAccount(int accNo) {
    ifstream inFile("accounts.dat", ios::binary);
    ofstream outFile("temp.dat", ios::binary);
    Account acc;
    bool found = false;

    while (readAccount(inFile, acc)) {
        if (acc.accountNumber != accNo) {
            writeAccount(outFile, acc);
        } else {
            found = true;
        }
    }

    inFile.close();
    outFile.close();
    remove("accounts.dat");
    rename("temp.dat", "accounts.dat");

    if (found)
        cout << "Account Deleted Successfully.\n";
    else
        cout << "Account Not Found.\n";
}

void depositOrWithdraw(int accNo, bool isDeposit) {
    fstream file("accounts.dat", ios::binary | ios::in | ios::out);
    Account acc;
    bool found = false;
    float amount;
    streampos prevPos;

    while (true) {
        prevPos = file.tellg();
        if (!readAccount(file, acc)) break;
        if (acc.accountNumber == accNo) {
            cout << "Enter amount to " << (isDeposit ? "deposit: " : "withdraw: ");
            cin >> amount;

            if (!isDeposit && acc.balance < amount) {
                cout << "Insufficient balance.\n";
                break;
            }

            float prevBalance = acc.balance;
            acc.balance += (isDeposit ? amount : -amount);
            file.seekp(prevPos);
            writeAccount(file, acc);
            found = true;

            logTransaction(acc.accountNumber, isDeposit ? "Deposit" : "Withdraw", amount, prevBalance, acc.balance);
            cout << "Transaction Successful. New Balance: " << acc.balance << "\n";
            break;
        }
    }
    file.close();

    if (!found)
        cout << "Account Not Found.\n";
}

void showAllAccounts() {
    ifstream file("accounts.dat", ios::binary);
    Account acc;
    cout << "\nAll Accounts:\n";
    cout << "---------------------------------------------------\n";
    while (readAccount(file, acc)) {
        cout << "Acc No: " << acc.accountNumber
             << " | Name: " << acc.name
             << " | Balance: " << acc.balance
             << " | Type: " << acc.accountType << "\n";
    }
    file.close();
}

void showTransactionLog() {
    ifstream log("transactions.log");
    string line;
    cout << "\nTransaction Logs:\n";
    cout << "-----------------------------\n";
    while (getline(log, line)) {
        cout << line << "\n";
    }
    log.close();
}

int main() {
    int choice, accNo;

    do {
        cout << "\n------ Bank Management System ------\n";
        cout << "1. Create Account\n";
        cout << "2. Display Account\n";
        cout << "3. Update Account\n";
        cout << "4. Delete Account\n";
        cout << "5. Deposit\n";
        cout << "6. Withdraw\n";
        cout << "7. Show All Accounts\n";
        cout << "8. View Transaction Logs\n";
        cout << "9. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: createAccount(); break;
            case 2:
                cout << "Enter Account Number: ";
                cin >> accNo;
                displayAccount(accNo); break;
            case 3:
                cout << "Enter Account Number: ";
                cin >> accNo;
                updateAccount(accNo); break;
            case 4:
                cout << "Enter Account Number: ";
                cin >> accNo;
                deleteAccount(accNo); break;
            case 5:
                cout << "Enter Account Number: ";
                cin >> accNo;
                depositOrWithdraw(accNo, true); break;
            case 6:
                cout << "Enter Account Number: ";
                cin >> accNo;
                depositOrWithdraw(accNo, false); break;
            case 7: showAllAccounts(); break;
            case 8: showTransactionLog(); break;
            case 9: cout << "Exiting...\n"; break;
            default: cout << "Invalid Choice!\n";
        }
    } while (choice != 9);

    return 0;
}
