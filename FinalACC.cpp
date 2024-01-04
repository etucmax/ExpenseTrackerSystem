#include <bits/stdc++.h>
#include <stdio.h>
#include <windows.h>
#include <conio.h>
using namespace std;
HANDLE COLOR = GetStdHandle(STD_OUTPUT_HANDLE); // Used To Set Color Of The Text In The Console
const int MAX_TRANSACTIONS = 100;

struct Transaction {
    string type;
    double amount;
    string transactionType;  // Added field to indicate transaction type (income or expense)
};

struct ExpenseTracker {
    double totalBalance;
    double income;
    double expenses;
    string transactionsFilename; // Separate filename for transactions
    string summaryFilename; 
    Transaction transactions[MAX_TRANSACTIONS];
    int numTransactions;
};

/*** Function Prototypes ***/
void welcomePage();
void header();
void clearInputStream();
void exitPage();
void displayMenu(ExpenseTracker &tracker);
void displaySummary(const ExpenseTracker &tracker);
void addIncome(ExpenseTracker &tracker);
void addExpense(ExpenseTracker &tracker);
void displayTransactions(const ExpenseTracker &tracker);
void saveTransactionsToFile(const ExpenseTracker &tracker);
void saveSummaryToFile(const ExpenseTracker &tracker);
void loadTransactionsFromFile(ExpenseTracker &tracker);
void loadSummaryFromFile(ExpenseTracker &tracker);

/*** Global Variables ***/
ExpenseTracker expenseTracker;

/*** Main Function ***/
int main() {
	
    expenseTracker.totalBalance = 0;
    expenseTracker.income = 0;
    expenseTracker.expenses = 0;
    expenseTracker.transactionsFilename = "transactions.txt";
    expenseTracker.summaryFilename = "summary.txt";
    expenseTracker.numTransactions = 0;

    loadSummaryFromFile(expenseTracker);
    loadTransactionsFromFile(expenseTracker);
    welcomePage();

    return 0;
}

void welcomePage() {
	
    char title[] = "\n\n\t-----------------------------------------------\n\t       WELCOME TO EXPENSE TRACKER SYSTEM\n\t-----------------------------------------------\n\n\t\t Press Any Key To Continue...";
    char *ptr = &title[0];
	SetConsoleTextAttribute(COLOR, 1);
    for(int i = 0; i < sizeof(title); i++) {
        cout << *(ptr + i);
        Sleep(40);
    }
    SetConsoleTextAttribute(COLOR, 15);
    getch();
    system("CLS");
    displayMenu(expenseTracker);
}

void header() {
	
	SetConsoleTextAttribute(COLOR, 1);
    cout << "\n\n\t------------------------------------------------" << endl;
    cout << "\t\t      EXPENSE TRACKER SYSTEM" << endl;
    cout << "\t------------------------------------------------" << endl;
    SetConsoleTextAttribute(COLOR, 15);
}

void displayMenu(ExpenseTracker &tracker) {
    string input;
    int choice;

    header();
    do {
        cout << "\n\t1. Display Summary\n";
        cout << "\t2. Add Income\n";
        cout << "\t3. Add Expense\n";
        cout << "\t4. Display Transactions\n";
        cout << "\t5. Exit\n";
        cout << "\tEnter Your Choice: ";
        
        getline(cin, input);

        // Input validation for integer choice
        stringstream ss(input);
        if (!(ss >> choice) || ss.peek() != EOF || choice < 1 || choice > 5) {
            system("cls");
            SetConsoleTextAttribute(COLOR, 4);
            cout << "\t\tInvalid choice. Please try again.\n";
            SetConsoleTextAttribute(COLOR, 15);
            Sleep(400);
            header();
            continue;
        }

        switch (choice) {
            case 1:
                system("CLS");
                header();
                displaySummary(tracker);
                break;
            case 2:
                system("CLS");
                header();
                addIncome(tracker);
                break;
            case 3:
                system("CLS");
                header();
                addExpense(tracker);
                break;
            case 4:
                system("CLS");
                header();
                displayTransactions(tracker);
                break;
            case 5:
                system("CLS");
                saveSummaryToFile(tracker);
                saveTransactionsToFile(tracker);
                exitPage();
                break;
        }
    } while (choice != 5);
}

void displaySummary(const ExpenseTracker &tracker) {
	
	SetConsoleTextAttribute(COLOR, 5);
    cout << "\n\t\t---------- Summary ----------\n";
    cout << "\t\tTotal Balance: " << tracker.totalBalance << " PHP\n";
    cout << "\t\tIncome: " << tracker.income << " PHP\n";
    cout << "\t\tExpenses: " << tracker.expenses << " PHP\n";
    cout << "\t\t-----------------------------\n";
    SetConsoleTextAttribute(COLOR, 15);
}

void addIncome(ExpenseTracker &tracker) {
    double amount;
    string incomeType;

    cout << "\n\n\tEnter Income Amount: ";
    while (!(cin >> amount) || amount <= 0 || getchar() != 10) {
        SetConsoleTextAttribute(COLOR, 4);
        cout << "\tInvalid amount. Please enter a valid number.\n";
        SetConsoleTextAttribute(COLOR, 15);
        clearInputStream();
        cout << "\n\tEnter Income Amount: ";
    }

    cout << "\tEnter Income Type: ";
    getline(cin, incomeType);

    tracker.income += amount;
    tracker.totalBalance += amount;

    Transaction transaction;
    transaction.type = incomeType;
    transaction.amount = amount;
    transaction.transactionType = "Income"; // Set transaction type to "Income"
    tracker.transactions[tracker.numTransactions++] = transaction;

    // Display the transaction details
    SetConsoleTextAttribute(COLOR, 2);
    cout << "\n\t\tIncome added successfully!\n";
    SetConsoleTextAttribute(COLOR, 15);
    cout << "\n\tType: " << incomeType << "\n";
    cout << "\tAmount: " << amount << " PHP\n";
}

void addExpense(ExpenseTracker &tracker) {
    double amount;
    string expenseType;

    cout << "\n\n\tEnter expense amount: ";
    while (!(cin >> amount) || amount <= 0 || amount > tracker.totalBalance || getchar() != 10) {
        SetConsoleTextAttribute(COLOR, 4);
        if (amount <= 0) {
            cout << "\n\tInvalid amount. Please enter a valid positive number.\n";
        } else {
            cout << "\n\tInvalid amount. Expense must be less than or equal to the total balance.\n";
        }
        SetConsoleTextAttribute(COLOR, 15);
        clearInputStream();
        cout << "\n\tEnter expense amount: ";
    }

    cout << "\tEnter Expense Type: ";
    getline(cin, expenseType);

    tracker.expenses += amount;
    tracker.totalBalance -= amount;

    Transaction transaction;
    transaction.type = expenseType;
    transaction.amount = amount;
    transaction.transactionType = "Expense"; // Set transaction type to "Expense"
    tracker.transactions[tracker.numTransactions++] = transaction;

    // Display the transaction details
    SetConsoleTextAttribute(COLOR, 2);
    cout << "\n\t\tExpense added successfully!\n";
    SetConsoleTextAttribute(COLOR, 15);
    cout << "\n\tType: " << expenseType << "\n";
    cout << "\tAmount: " << amount << " PHP\n";
}

void displayTransactions(const ExpenseTracker &tracker) {
    
	if(tracker.numTransactions == 0) {
    	SetConsoleTextAttribute(COLOR, 4);
        cout << "\n\t\t   No transactions available.\n";
        SetConsoleTextAttribute(COLOR, 15);
        return;
    }
    
    SetConsoleTextAttribute(COLOR, 5);
    cout << "\n\t\t-------- Transactions --------\n";
	for(int i = 0; i < tracker.numTransactions; ++i) {
        cout << "\t\tType: " << tracker.transactions[i].type << "\n";
        cout << "\t\tAmount: " << tracker.transactions[i].amount << " PHP\n";
        cout << "\t\tTransaction Type: " << tracker.transactions[i].transactionType << "\n";
        cout << "\t\t-----------------------------\n";
    }
    SetConsoleTextAttribute(COLOR, 15);
}

void saveTransactionsToFile(const ExpenseTracker &tracker) {
    ofstream file(tracker.transactionsFilename);

    if(!file.is_open()) {
        return;
    }

    // Clear existing data in the file
    file.close();
    file.open(tracker.transactionsFilename, std::ofstream::out | std::ofstream::trunc);

    // Save new data
    for(int i = 0; i < tracker.numTransactions; ++i) {
        file << tracker.transactions[i].type << "§"
             << tracker.transactions[i].amount << "§"
             << tracker.transactions[i].transactionType << endl;
    }

    file.close();
}

void saveSummaryToFile(const ExpenseTracker &tracker) {
    ofstream file(tracker.summaryFilename);

    if(!file.is_open()) {
        return;
    }

    // Clear existing data in the file
    file.close();
    file.open(tracker.summaryFilename, std::ofstream::out | std::ofstream::trunc);

    // Save new data
    file << "Total: Balance: " << tracker.totalBalance << "\n";
    file << "Income: " << tracker.income << "\n";
    file << "Expenses: " << tracker.expenses << "\n";
    file << "-----------------\n";

    file.close();
}

void loadSummaryFromFile(ExpenseTracker &tracker) {
    ifstream file(tracker.summaryFilename);

    if(!file.is_open()) {
        return;
    }

    string line;

    while(getline(file, line)) {
        stringstream get(line);

        string type;

        get >> type;
        if(type == "Total:") {
            get >> type >> tracker.totalBalance;
        } 
		else if(type == "Income:") {
            get >> tracker.income;
        } 
		else if(type == "Expenses:") {
            get >> tracker.expenses;
        }
    }

    file.close();
}

void loadTransactionsFromFile(ExpenseTracker &tracker) {
    ifstream file(tracker.transactionsFilename);

    if(!file.is_open()) {
        return;
    }

    string line;

    // Clear existing data in the transactions array
    tracker.numTransactions = 0;

    while(getline(file, line)) {
        stringstream get(line);
        string trammount;

        Transaction transaction;

        getline(get, transaction.type, '§');
        getline(get, trammount, '§');
        transaction.amount = stoi(trammount);
        getline(get, transaction.transactionType, '§');

        tracker.transactions[tracker.numTransactions++] = transaction;
    }

    file.close();
}


void clearInputStream() { // This Used To Clear The Input Stream
	
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void exitPage() {
	SetConsoleTextAttribute(COLOR, 6);
	cout << "\n\n\n\n\t\t\t   ----------------------------------------------" << endl;
	cout << "\t\t\t\t   THANK YOU FOR USING THIS SYSTEM" << endl;
	cout << "\t\t\t   ----------------------------------------------" << endl;
	cout << "\t\tDEVELOPED BY JASTREL ACEBU, PRINCE KEITH TAGALICUD, AND GEWEL SACABIN\n\n\n\n\n\n\n" << endl;
	SetConsoleTextAttribute(COLOR, 15);
	exit(0);
}

