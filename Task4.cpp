#include <iostream>
#include <vector>
#include <ctime>
#include <iomanip>
using namespace std;

string currentDateTime() {
    time_t now = time(0);
    char buf[80];
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", localtime(&now));
    return buf;
}

class Transaction {
public:
    string type;
    double amount;
    string dateTime;

    Transaction(string t, double a) {
        type = t;
        amount = a;
        dateTime = currentDateTime();
    }

    void display() const {
        cout << setw(12) << type << " | "
             << setw(8) << amount << " | "
             << dateTime << endl;
    }
};

class Account {
private:
    int accountNumber;
    double balance;
    vector<Transaction> transactions;

public:
    Account(int accNo) {
        accountNumber = accNo;
        balance = 0.0;
    }

    int getAccountNumber() const {
        return accountNumber;
    }

    double getBalance() const {
        return balance;
    }

    void deposit(double amount) {
        balance += amount;
        transactions.push_back(Transaction("Deposit", amount));
        cout << "Deposited successfully.\n";
    }

    void withdraw(double amount) {
        if (amount > balance) {
            cout << "Insufficient balance.\n";
        } else {
            balance -= amount;
            transactions.push_back(Transaction("Withdraw", amount));
            cout << "Withdrawal successful.\n";
        }
    }

    void transfer(Account& toAccount, double amount) {
        if (amount > balance) {
            cout << "Insufficient balance.\n";
        } else {
            balance -= amount;
            toAccount.balance += amount;
            transactions.push_back(Transaction("Transfer Out", amount));
            toAccount.transactions.push_back(Transaction("Transfer In", amount));
            cout << "Transfer successful.\n";
        }
    }

    void showTransactions() const {
        if (transactions.empty()) {
            cout << "No transactions available.\n";
            return;
        }
        cout << "Type        | Amount   | Date\n";
        cout << "--------------------------------------------\n";
        for (const auto& t : transactions)
            t.display();
    }

    void showAccountInfo() const {
        cout << "Account Number: " << accountNumber << "\n";
        cout << "Balance: $" << fixed << setprecision(2) << balance << "\n";
        showTransactions();
    }
};

class Customer {
private:
    string name;
    string email;
    int customerID;
    vector<Account> accounts;

public:
    Customer(string n, string e, int id) {
        name = n;
        email = e;
        customerID = id;
    }

    void createAccount(int accNo) {
        accounts.push_back(Account(accNo));
        cout << "Account #" << accNo << " created successfully.\n";
    }

    Account* getAccount(int accNo) {
        for (auto& acc : accounts)
            if (acc.getAccountNumber() == accNo)
                return &acc;
        return nullptr;
    }

    void showCustomerInfo() const {
        cout << "Customer ID: " << customerID << "\n";
        cout << "Name: " << name << ", Email: " << email << "\n";
        cout << "Accounts:\n";
        for (const auto& acc : accounts) {
            cout << "  - Account #" << acc.getAccountNumber()
                 << " | Balance: $" << fixed << setprecision(2) << acc.getBalance() << "\n";
        }
    }
};

int main() {
    string name, email;
    int customerID, numAccounts;

    cout << "Enter Customer Name: ";
    getline(cin, name);
    cout << "Enter Email: ";
    getline(cin, email);
    cout << "Enter Customer ID: ";
    cin >> customerID;

    Customer customer(name, email, customerID);

    cout << "\nHow many accounts to create? ";
    cin >> numAccounts;
    for (int i = 0; i < numAccounts; ++i) {
        int accNo;
        cout << "Enter Account Number " << i + 1 << ": ";
        cin >> accNo;
        customer.createAccount(accNo);
    }

    int choice;
    do {
        cout << "\n========= MENU =========\n";
        cout << "1. Deposit\n2. Withdraw\n3. Transfer\n4. Show Account Info\n5. Show Customer Info\n0. Exit\n";
        cout << "Choose an option: ";
        cin >> choice;

        if (choice == 1 || choice == 2 || choice == 3) {
            int accNo;
            cout << "Enter your Account Number: ";
            cin >> accNo;
            Account* acc = customer.getAccount(accNo);

            if (!acc) {
                cout << "Account not found!\n";
                continue;
            }

            if (choice == 1) {
                double amt;
                cout << "Enter amount to deposit: ";
                cin >> amt;
                acc->deposit(amt);
            } else if (choice == 2) {
                double amt;
                cout << "Enter amount to withdraw: ";
                cin >> amt;
                acc->withdraw(amt);
            } else if (choice == 3) {
                int toAccNo;
                double amt;
                cout << "Enter recipient Account Number: ";
                cin >> toAccNo;
                Account* toAcc = customer.getAccount(toAccNo);
                if (!toAcc) {
                    cout << "Target account not found.\n";
                    continue;
                }
                cout << "Enter amount to transfer: ";
                cin >> amt;
                acc->transfer(*toAcc, amt);
            }
        } else if (choice == 4) {
            int accNo;
            cout << "Enter Account Number: ";
            cin >> accNo;
            Account* acc = customer.getAccount(accNo);
            if (acc)
                acc->showAccountInfo();
            else
                cout << "Account not found!\n";
        } else if (choice == 5) {
            customer.showCustomerInfo();
        }

    } while (choice != 0);

    cout << "\n Thank you for using our banking system!\n";
    return 0;
}
