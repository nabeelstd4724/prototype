#include <iostream>
#include <string>
#include<cstring>
#include <cctype>
#include <fstream>
#include <cstdlib> 
using namespace std;

class User {
protected:
    string username;
    string password;
public:
    User() {
        username = "";
        password = "";
    }
    User(string u, string p) :username(u), password(p) {}
    void setusername(string u) { username = u; }
    void setuserpassword(string p) { password = p; }
    string getpassword() { return password; }
    string getusername() { return username; }
    bool login(string inputusername, string inputpassword) {
        return (username == inputusername && password == inputpassword);
    }
};
bool isValidPassword(const string& password) {
    if (password.length() != 5) return false;

    bool hasLetter = false, hasDigit = false;

    for (char ch : password) {
        if (isalpha(ch)) hasLetter = true;
        if (isdigit(ch)) hasDigit = true;
    }

    return hasLetter && hasDigit;
}


class Voter :public User {
private:
    string cnic;
    int age;
    string cast_vote_to;
public:
    Voter() : age(0), cast_vote_to("") {}
    Voter(string u, string p, string c, int a) :User(u, p), cnic(c), age(a), cast_vote_to("") {}
    void castvote(string candidate_name) {
        cast_vote_to = candidate_name;
        cout << "You have successfully voted for candidate: " << candidate_name << endl;
    }
    void setage(int a) {
        if (a < 18) {
            cout << "For register as a voter you must be older than 18 years\n";
        }
        else {
            age = a;
        }
    }
    void setcnic(string c) {
        if (validatecnic(c)) {
            cnic = c;
        }
        else {
            cout << "CNIC must be 13 digits only without spaces\n";
        }
    }
    bool validatecnic(string c) {
        if (c.length() != 13) return false;
        for (char ch : c) {
            if (!isdigit(ch)) return false;
        }
        return true;
    }
    string votestatus() {
        if (cast_vote_to == "") {
            return "You have not voted yet.\n";
        }
        else {
            return "You have already voted to " + cast_vote_to + "\n";
        }
    }
};

class Candidate {
private:
    string candidate_name;
    string party_name;
    int vote_count;
    static int total_vote;
public:
    Candidate(string n, string p) :candidate_name(n), party_name(p), vote_count(0) {}
    void increment_vote() {
        vote_count++;
        total_vote++;
    }
    string getcandidatename() { return candidate_name; }
    string getpartyname() { return party_name; }
    int get_votereceived() { return vote_count; }
    static int get_totalvote() { return total_vote; }
    void display_candidateinfo() {
        cout << "Candidate name:     " << candidate_name << endl;
        cout << "Party affiliation:   " << party_name << endl;
        cout << "Vote received:       " << vote_count << endl;
    }
};
int Candidate::total_vote = 0;

class Election {
protected:
    string electionType;
public:
    Election(string type) : electionType(type) {}
    virtual void startElection() = 0;
    virtual void endElection() = 0;
};

class LocalElection : public Election {
public:
    LocalElection() : Election("Local") {}
    void startElection() override { cout << "Local Election Started.\n"; }
    void endElection() override { cout << "Local Election Ended.\n"; }
};

class NationalElection : public Election {
public:
    NationalElection() : Election("National") {}
    void startElection() override { cout << "National Election Started.\n"; }
    void endElection() override { cout << "National Election Ended.\n"; }
};

void displayResults() {
    system("cls");
    cout << "========== Election Results ==========\n";
    cout << "Total Votes: " << Candidate::get_totalvote() << endl;
}

void adminMenu();
void voterMenu();
void displayHelp();

void mainMenu() {
    int choice;
    do {
        system("cls");
        cout << "\n===================================";
        cout << "\n    Election Management System";
        cout << "\n===================================";
        cout << "\n1. Admin";
        cout << "\n2. Voter";
        cout << "\n3. View Election Result";
        cout << "\n4. Help & Guidelines";
        cout << "\n5. Exit";
        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            adminMenu();
            break;
        case 2:
            voterMenu();
            break;
        case 3:
            displayResults();
            system("pause");
            break;
        case 4:
            displayHelp();
            system("pause");
            break;
        case 5:
            cout << "Exiting program...\n";
            break;
        default:
            cout << "Invalid choice! Please try again.\n";
            system("pause");
        }
    } while (choice != 5);
}

void adminMenu() {
    int option;
    do {
        system("cls");
        cout << "\n------ Admin Menu ------";
        cout << "\n1. Add Candidate";
        cout << "\n2. Add Voter";
        cout << "\n3. View All Candidates";
        cout << "\n4. View All Voters";
        cout << "\n5. View Election Results";
        cout << "\n6. Back to Main Menu";
        cout << "\nEnter choice: ";
        cin >> option;

        switch (option) {
        case 1:
            cout << "Add candidate\n";
            break;
        case 2:
            cout << "Add Voter Logic Here...\n";
            break;
        case 3:
            cout << "Displaying all candidates...\n";
            break;
        case 4:
            cout << "Displaying all voters...\n";
            break;
        case 5:
            displayResults();
            break;
        case 6:
            cout << "Returning to Main Menu...\n";
            break;
        default:
            cout << "Invalid choice, try again.\n";
        }
        system("pause");
    } while (option != 6);
}

void voterMenu() {
    int option;
    do {
        system("cls");
        cout << "\n------ Voter Menu ------";
        cout << "\n1. Cast Vote";
        cout << "\n2. Check Voting Status";
        cout << "\n3. View Election Results";
        cout << "\n4. Back to Main Menu";
        cout << "\nEnter choice: ";
        cin >> option;

        switch (option) {
        case 1:
            cout << "Casting vote logic here...\n";
            break;
        case 2:
            cout << "Checking voting status...\n";
            break;
        case 3:
            displayResults();
            break;
        case 4:
            cout << "Returning to Main Menu...\n";
            break;
        default:
            cout << "Invalid choice, try again.\n";
        }
        system("pause");
    } while (option != 4);
}

void registerAdmin() {
    ifstream infile("admin.txt");
    string existingUsername, existingPassword;
    string newUsername, newPassword;


    do {
        cout << "Enter username: ";
        getline(cin >> ws, newUsername);
        if (newUsername.empty()) {
            cout << " Username cannot be empty. Try again.\n";
        }
    } while (newUsername.empty());


    while (infile >> existingUsername >> existingPassword) {
        if (existingUsername == newUsername) {
            cout << "Username already exists! Please log in.\n";
            infile.close();
            return;
        }
    }
    infile.close();

    ofstream outfile("admin.txt", ios::app);
    do {
        cout << "Enter password (5 characters, at least one letter and one digit): ";
        cin >> newPassword;
        if (!isValidPassword(newPassword)) {
            cout << " Invalid password format. Try again.\n";
        }
    } while (!isValidPassword(newPassword));

    outfile << newUsername << " " << newPassword << endl;

    outfile.close();

    cout << "Admin registered successfully!\n";
}

bool loginAdmin() {
    string inputUsername, inputPassword;
    cout << "Enter Admin Username: ";
    cin >> inputUsername;
    cout << "Enter Admin Password: ";
    cin >> inputPassword;

    ifstream infile("admin.txt");
    string storedUsername, storedPassword;
    bool loginSuccess = false;

    while (infile >> storedUsername >> storedPassword) {
        if (inputUsername == storedUsername && inputPassword == storedPassword) {
            loginSuccess = true;
            break;
        }
    }
    infile.close();
    return loginSuccess;
}

void registerVoter() {
    ifstream infile("voters.txt");
    string existingUsername, existingPassword;
    string newUsername, newPassword;

    do {
        cout << "Enter username: ";
        getline(cin >> ws, newUsername);
        if (newUsername.empty()) {
            cout << " Username cannot be empty. Try again.\n";
        }
    } while (newUsername.empty());


    while (infile >> existingUsername >> existingPassword) {
        if (existingUsername == newUsername) {
            cout << "Username already exists! Please log in.\n";
            infile.close();
            return;
        }
    }
    infile.close();

    ofstream outfile("voters.txt", ios::app);
    do {
        cout << "Enter password (5 characters, at least one letter and one digit): ";
        cin >> newPassword;
        if (!isValidPassword(newPassword)) {
            cout << " Invalid password format. Try again.\n";
        }
    } while (!isValidPassword(newPassword));

    outfile << newUsername << " " << newPassword << endl;

    outfile.close();

    cout << "Voter registered successfully!\n";
}

bool loginVoter() {
    string inputUsername, inputPassword;
    cout << "Enter Voter Username: ";
    cin >> inputUsername;
    cout << "Enter Voter Password: ";
    cin >> inputPassword;

    ifstream infile("voters.txt");
    string storedUsername, storedPassword;
    bool loginSuccess = false;

    while (infile >> storedUsername >> storedPassword) {
        if (inputUsername == storedUsername && inputPassword == storedPassword) {
            loginSuccess = true;
            break;
        }
    }
    infile.close();
    return loginSuccess;
}

void displayHelp() {
    system("cls");
    cout << "\n========== HELP & GUIDELINES ==========\n";
    cout << "Welcome to the Online Voting System!\n";
    cout << "----------------------------------------\n";

    cout << "\n1. User Authentication:\n";
    cout << "- Voters and Admins must log in.\n";
    cout << "- New users must register first.\n";

    cout << "\n2. Voter Guidelines:\n";
    cout << "- You can only vote once.\n";
    cout << "- Votes cannot be changed.\n";
    cout << "- Check your vote status anytime.\n";

    cout << "\n3. Admin Privileges:\n";
    cout << "- Manage elections, voters, and candidates.\n";

    cout << "\n4. Security:\n";
    cout << "- CNIC must be 13 digits without spaces.\n";
    cout << "- Keep your password secure.\n";

    cout << "\n----------------------------------------\n";
}

int main() {
    int choice;
    cout << "Welcome to the Election Management System\n";

    while (true) {
        system("cls");
        cout << "\n1. Admin Login\n2. Voter Login\n3. Register Admin\n4. Register Voter\n5. Exit\nEnter option: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1:
            if (loginAdmin()) {
                cout << " Admin Login Successful!\n";
                system("pause");
                adminMenu();
            }
            else {
                char retry;
                cout << " Login failed! Incorrect username or password.\n";
                cout << "Would you like to retry? (y/n): ";
                cin >> retry;
                cin.ignore();
                if (retry == 'n' || retry == 'N') continue;
            }
            break;

        case 2:
            if (loginVoter()) {
                cout << " Voter Login Successful!\n";
                system("pause");
                voterMenu();
            }
            else {
                char retry;
                cout << " Login failed! Incorrect username or password.\n";
                cout << "Would you like to retry? (y/n): ";
                cin >> retry;
                cin.ignore();
                if (retry == 'n' || retry == 'N') continue;
            }
            break;

        case 3:
            registerAdmin();
            system("pause");
            break;

        case 4:
            registerVoter();
            system("pause");
            break;

        case 5:
            cout << "Exiting program...\n";
            return 0;

        default:
            cout << "Invalid choice, please try again.\n";
            system("pause");
            break;
        }
    }











}
