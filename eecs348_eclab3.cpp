#include <iostream>
#include <iomanip>
#include <string>

class Account {
protected:
    std::string accountNumber;
    std::string accountHolder;
    double balance;

public:
    Account(const std::string& accNumber, const std::string& accHold, double accBalance)
        : accountNumber(accNumber), accountHolder(accHold), balance(accBalance) {}

    virtual void displayDetails() const {
        std::cout << "Account Details for Account (ID: " << accountNumber << "):" << std::endl;
        std::cout << "   Type: Account" << std::endl; // Default type is Account
        std::cout << "   Holder: " << accountHolder << std::endl;
        std::cout << "   Balance: $" << std::fixed << std::setprecision(2) << balance << std::endl;
    }

    void deposit(double amount) {
        balance += amount;
    }

    virtual void withdraw(double amount) = 0;

    virtual void transfer(Account& receiver, double amount) {
        if (balance >= amount) {
            withdraw(amount);
            receiver.deposit(amount);
            std::cout << "Transferred $" << amount << " from Account " << accountNumber
                      << " to Account " << receiver.accountNumber << std::endl;
        } else {
            std::cout << "Insufficient funds in Account " << accountNumber << " for transfer." << std::endl;
        }
    }

    friend Account& operator+(Account& sender, double amount) {
        sender.withdraw(amount);
        return sender;
    }

    friend std::ostream& operator<<(std::ostream& os, const Account& account) {
        account.displayDetails();
        return os;
    }
};

class SavingsAccount : public Account {
private:
    double interestRate;

public:
    SavingsAccount(const std::string& number, const std::string& holder, double initialBalance, double rate)
        : Account(number, holder, initialBalance), interestRate(rate) {}

    void withdraw(double amount) override {
        if (balance - amount >= 0) {
            balance -= amount;
            std::cout << "Withdrawn $" << amount << " from Savings Account." << std::endl;
        } else {
            std::cout << "Insufficient funds in Savings Account." << std::endl;
        }
    }

    void applyInterest() {
        balance += balance * interestRate;
    }

    double getInterestRate() const {
        return interestRate;
    }

    void displayDetails() const override {
        std::cout << "Account Details for Savings Account (ID: " << accountNumber << "):" << std::endl;
        std::cout << "   Type: Savings Account" << std::endl;
        std::cout << "   Holder: " << accountHolder << std::endl;
        std::cout << "   Balance: $" << std::fixed << std::setprecision(2) << balance << std::endl;
        std::cout << "   Interest Rate: " << std::fixed << std::setprecision(2) << interestRate * 100 << "%" << std::endl;
    }
};

class CurrentAccount : public Account {
private:
    double overdraftLimit;

public:
    CurrentAccount(const std::string& number, const std::string& holder, double initialBalance, double limit)
        : Account(number, holder, initialBalance), overdraftLimit(limit) {}

    void withdraw(double amount) override {
        if (balance + overdraftLimit - amount >= 0) {
            balance -= amount;
            std::cout << "Withdrawn $" << amount << " from Current Account." << std::endl;
        } else {
            std::cout << "Withdrawal exceeds overdraft limit in Current Account." << std::endl;
        }
    }

    double getOverdraftLimit() const {
        return overdraftLimit;
    }

    void displayDetails() const override {
        std::cout << "Account Details for Current Account (ID: " << accountNumber << "):" << std::endl;
        std::cout << "   Type: Current Account" << std::endl;
        std::cout << "   Holder: " << accountHolder << std::endl;
        std::cout << "   Balance: $" << std::fixed << std::setprecision(2) << balance << std::endl;
        std::cout << "   Overdraft Limit: $" << std::fixed << std::setprecision(2) << overdraftLimit << std::endl;
    }
};

int main() {
    SavingsAccount savings("S123", "Mark Ke", 100000, 0.02);
    CurrentAccount current("C456", "Ginny Ke", 500000, 500);

    std::cout << savings << std::endl;
    std::cout << current << std::endl;

    std::cout << std::endl;
    std::cout << "Account Details After Deposit and Withdrawal: " << std::endl;

    savings.deposit(500);
    current.withdraw(1000);

    std::cout << savings << std::endl;
    std::cout << current << std::endl;

    // Transfer 300 from savings to current
    savings + 300;
    current + 300;

    std::cout << std::endl;
    std::cout << "Account Details after Transfer: " << std::endl;
    std::cout << savings << std::endl;
    std::cout << current << std::endl;

    return 0;
}
