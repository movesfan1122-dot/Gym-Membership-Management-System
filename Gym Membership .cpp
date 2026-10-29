#include <iostream>
#include <string>
#include <limits>
#include <cctype>
using namespace std;

const int MAX_MEMBERS = 100;
const int MAX_MONTHS = 120;
const double BASIC_PRICE = 1000;
const double STANDARD_PRICE = 1500;
const double PREMIUM_PRICE = 2000;

// Safe integer input: returns false if input is not a valid integer
bool readInt(int &value)
{
    if (!(cin >> value))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return true;
}

// Safe double input
bool readDouble(double &value)
{
    if (!(cin >> value))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return true;
}

bool isValidPhone(const string &phone)
{
    if (phone.length() != 10)
        return false;
    for (char c : phone)
    {
        if (!isdigit(static_cast<unsigned char>(c)))
            return false;
    }
    return true;
}

double getPlanPrice(const string &plan)
{
    if (plan == "Basic")
        return BASIC_PRICE;
    if (plan == "Standard")
        return STANDARD_PRICE;
    return PREMIUM_PRICE;
}

class Member
{
public:
    int id = 0;
    string name;
    string phone;
    string plan = "Not selected";
    int months = 0;
    int daysLeft = 0;
    double paid = 0;
};

class Gym
{
    Member m[MAX_MEMBERS];
    int total = 0;
    double income = 0;

    // Returns index of member, or -1 if not found
    int findMember(int id) const
    {
        for (int i = 0; i < total; i++)
        {
            if (m[i].id == id)
                return i;
        }
        return -1;
    }

    // Asks for ID and returns index, or -1 (with message) if invalid/not found
    int askForMember()
    {
        int id;
        cout << "Enter member ID: ";
        if (!readInt(id))
        {
            cout << "Invalid input! Please enter a number.\n";
            return -1;
        }
        int pos = findMember(id);
        if (pos == -1)
            cout << "Member not found!\n";
        return pos;
    }

    string readName(const string &prompt)
    {
        string name;
        do
        {
            cout << prompt;
            getline(cin, name);
            if (name.empty())
                cout << "Name cannot be empty!\n";
        } while (name.empty());
        return name;
    }

    string readPhone(const string &prompt)
    {
        string phone;
        do
        {
            cout << prompt;
            getline(cin, phone);
            if (!isValidPhone(phone))
                cout << "Phone number must be exactly 10 digits!\n";
        } while (!isValidPhone(phone));
        return phone;
    }

public:
    void registerMember()
    {
        if (total >= MAX_MEMBERS)
        {
            cout << "Gym member limit reached!\n";
            return;
        }

        int newId;
        cout << "Enter member ID: ";
        if (!readInt(newId))
        {
            cout << "Invalid input! Please enter a number.\n";
            return;
        }

        if (newId <= 0)
        {
            cout << "Invalid member ID!\n";
            return;
        }

        if (findMember(newId) != -1)
        {
            cout << "Member ID already exists!\n";
            return;
        }

        m[total].id = newId;
        m[total].name = readName("Enter member name: ");
        m[total].phone = readPhone("Enter phone number (10 digits): ");
        m[total].plan = "Not selected";
        m[total].months = 0;
        m[total].daysLeft = 0;
        m[total].paid = 0;

        total++;

        cout << "Member registered successfully!\n";
    }

    void showMembers()
    {
        if (total == 0)
        {
            cout << "No members registered yet.\n";
            return;
        }

        cout << "\n===== GYM MEMBERS =====\n";

        for (int i = 0; i < total; i++)
        {
            cout << "\nMember ID: " << m[i].id;
            cout << "\nName: " << m[i].name;
            cout << "\nPhone: " << m[i].phone;
            cout << "\nPlan: " << m[i].plan;
            cout << "\nMonths: " << m[i].months;
            cout << "\nDays left: " << m[i].daysLeft;
            cout << "\nTotal paid: Rs. " << m[i].paid << endl;
        }
    }

    void updateMember()
    {
        cout << "(Update) ";
        int pos = askForMember();
        if (pos == -1)
            return;

        m[pos].name = readName("Enter new name: ");
        m[pos].phone = readPhone("Enter new phone number (10 digits): ");

        cout << "Member details updated successfully!\n";
    }

    void selectPlan()
    {
        int pos = askForMember();
        if (pos == -1)
            return;

        if (m[pos].daysLeft > 0)
        {
            cout << "Member already has an active plan (" << m[pos].plan
                 << ", " << m[pos].daysLeft << " days left).\n";
            cout << "Please use 'Renew Membership' instead.\n";
            return;
        }

        int choice;
        cout << "\nChoose Membership Plan\n";
        cout << "1. Basic    - Rs. " << BASIC_PRICE << "/month\n";
        cout << "2. Standard - Rs. " << STANDARD_PRICE << "/month\n";
        cout << "3. Premium  - Rs. " << PREMIUM_PRICE << "/month\n";
        cout << "Enter choice: ";
        if (!readInt(choice))
        {
            cout << "Invalid input! Please enter a number.\n";
            return;
        }

        string selectedPlan;
        if (choice == 1)
            selectedPlan = "Basic";
        else if (choice == 2)
            selectedPlan = "Standard";
        else if (choice == 3)
            selectedPlan = "Premium";
        else
        {
            cout << "Invalid plan choice!\n";
            return;
        }

        int months;
        cout << "Enter membership duration in months: ";
        if (!readInt(months) || months <= 0 || months > MAX_MONTHS)
        {
            cout << "Enter a valid duration (1-" << MAX_MONTHS << " months)!\n";
            return;
        }

        double amount = getPlanPrice(selectedPlan) * months;

        cout << "Membership cost: Rs. " << amount << endl;
        cout << "Pay now? (1 = Yes, 0 = No): ";
        int payChoice;
        if (!readInt(payChoice) || payChoice != 1)
        {
            cout << "Plan not activated. Payment pending.\n";
            return;
        }

        // Plan is assigned only after payment is confirmed
        m[pos].plan = selectedPlan;
        m[pos].months = months;
        m[pos].daysLeft = months * 30;
        m[pos].paid += amount;
        income += amount;

        cout << "Plan activated successfully!\n";
        cout << "Plan: " << m[pos].plan << endl;
        cout << "Validity: " << m[pos].daysLeft << " days\n";
        cout << "Payment received: Rs. " << amount << endl;
    }

    void renewMembership()
    {
        int pos = askForMember();
        if (pos == -1)
            return;

        if (m[pos].plan == "Not selected")
        {
            cout << "Select a plan before renewal!\n";
            return;
        }

        double price = getPlanPrice(m[pos].plan);

        int months;
        cout << "Enter extra months: ";
        if (!readInt(months) || months <= 0 || months > MAX_MONTHS)
        {
            cout << "Invalid number of months!\n";
            return;
        }

        double amount = price * months;

        cout << "Renewal cost: Rs. " << amount << endl;
        cout << "Pay now? (1 = Yes, 0 = No): ";
        int payChoice;
        if (!readInt(payChoice) || payChoice != 1)
        {
            cout << "Renewal not completed. Payment pending.\n";
            return;
        }

        m[pos].months += months;
        m[pos].daysLeft += months * 30;
        m[pos].paid += amount;
        income += amount;

        cout << "Membership renewed successfully!\n";
        cout << "New validity: " << m[pos].daysLeft << " days\n";
    }

    void checkValidity()
    {
        int pos = askForMember();
        if (pos == -1)
            return;

        cout << "Member: " << m[pos].name << endl;
        cout << "Plan: " << m[pos].plan << endl;
        cout << "Days remaining: " << m[pos].daysLeft << endl;

        if (m[pos].daysLeft > 0)
            cout << "Status: Active\n";
        else
            cout << "Status: Expired or not activated\n";
    }

    void recordPayment()
    {
        int pos = askForMember();
        if (pos == -1)
            return;

        double amount;
        cout << "Enter additional payment amount: Rs. ";
        if (!readDouble(amount) || amount <= 0)
        {
            cout << "Invalid payment amount!\n";
            return;
        }

        m[pos].paid += amount;
        income += amount;

        cout << "Payment recorded successfully!\n";
        cout << "Member's total recorded payments: Rs. "
             << m[pos].paid << endl;
    }

    void gymReport()
    {
        cout << "\n===== GYM REPORT =====\n";
        cout << "Total registered members: " << total << endl;
        cout << "Total payment recorded: Rs. " << income << endl;

        int active = 0, expired = 0;

        for (int i = 0; i < total; i++)
        {
            if (m[i].daysLeft > 0)
                active++;
            else
                expired++;
        }

        cout << "Active memberships: " << active << endl;
        cout << "Expired/unactivated memberships: " << expired << endl;
    }
};

int main()
{
    Gym g;
    int choice;

    do
    {
        cout << "\n====== GYM MANAGEMENT SYSTEM ======\n";
        cout << "1. Register Member\n";
        cout << "2. Display All Members\n";
        cout << "3. Update Member Details\n";
        cout << "4. Select Membership Plan\n";
        cout << "5. Renew Membership\n";
        cout << "6. Check Membership Validity\n";
        cout << "7. Record Additional Payment\n";
        cout << "8. Generate Gym Report\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";

        if (!readInt(choice))
            choice = -1;

        switch (choice)
        {
        case 1:
            g.registerMember();
            break;
        case 2:
            g.showMembers();
            break;
        case 3:
            g.updateMember();
            break;
        case 4:
            g.selectPlan();
            break;
        case 5:
            g.renewMembership();
            break;
        case 6:
            g.checkValidity();
            break;
        case 7:
            g.recordPayment();
            break;
        case 8:
            g.gymReport();
            break;
        case 0:
            cout << "Gym system closed successfully!\n";
            break;
        default:
            cout << "Invalid choice! Try again.\n";
        }

    } while (choice != 0);

    return 0;
}