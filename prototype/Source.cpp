#include <iostream>
#include <string>
#include <cstring>
#include <cctype>
#include <fstream>
#include <cstdlib> 
#include <iomanip>
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
    void displayProvinceAndGender() {
        if (cnic.length() != 13) {
            cout << "Invalid CNIC. Cannot determine province and gender.\n";
            return;
        }


        char firstDigit = cnic[0];
        string province;
        switch (firstDigit) {
        case '3': province = "Punjab"; break;
        case '4': province = "Sindh"; break;
        case '5': province = "Balochistan"; break;
        case '6': province = "Khyber Pakhtunkhwa"; break;
        default: province = "Unknown Province"; break;
        }


        char lastDigit = cnic[12];
        string gender;
        if (isdigit(lastDigit)) {
            int digit = lastDigit - '0';
            gender = (digit % 2 == 0) ? "Female" : "Male";
        }
        else {
            gender = "Unknown Gender";
        }

        cout << "Province: " << province << endl;
        cout << "Gender: " << gender << endl;
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
void displayResults() {
    ifstream file("candidates.txt");
    if (!file) {
        cout << "No candidates found.\n";
        return;
    }

    string name, party;
    int totalVotes = 0;
    int maxVotes = -1;
    string leadingCandidate;

    cout << "\n========== Election Results ==========\n";
    while (file >> quoted(name) >> quoted(party)) {
        int vote = 0;


        ifstream votesFile("votes.txt");
        string voter, votedName;
        while (votesFile >> voter >> votedName) {
            if (votedName == name) {
                vote++;
            }
        }
        votesFile.close();

        cout << "Candidate: " << name << "        Party: " << party << "      Votes: " << vote << endl;

        totalVotes += vote;

        if (vote > maxVotes) {
            maxVotes = vote;
            leadingCandidate = name;
        }
    }

    file.close();

    cout << "--------------------------------------\n";
    cout << "Total Votes Cast: " << totalVotes << endl;

    if (maxVotes > 0) {
        cout << "🏆 Leading Candidate: " << leadingCandidate << " with " << maxVotes << " votes.\n";
    }
    else {
        cout << "No votes have been cast yet.\n";
    }
}

class Admin : public User {
public:
    Admin() {}
    Admin(string u, string p) : User(u, p) {}

    void addCandidate() {
        string name, party;
        cout << "Enter candidate name: ";
        cin.ignore();
        getline(cin, name);


        ifstream readFile("candidates.txt");
        string existingName, existingParty;
        bool duplicate = false;
        while (readFile >> quoted(existingName) >> quoted(existingParty)) {
            if (existingName == name) {
                duplicate = true;
                break;
            }
        }
        readFile.close();

        if (duplicate) {
            cout << "❌ Candidate \"" << name << "\" is already registered!\n";
            return;
        }

        cout << "Enter party name: ";
        getline(cin, party);

        ofstream file("candidates.txt", ios::app);
        file << quoted(name) << " " << quoted(party) << endl;
        file.close();
        cout << "✅ Candidate added successfully!\n";
    }

    void viewCandidates() {
        ifstream file("candidates.txt");
        string name, party;
        int count = 1;

        cout << "\n--- Candidate List ---\n";
        while (file >> quoted(name) >> quoted(party)) {
            cout << count << ". " << name << " (" << party << ")\n";
            count++;
        }
        file.close();
    }





    void viewElectionResults();

    void viewVoters() {
        ifstream file("voters.txt");
        string name, pass;
        int count = 1;

        cout << "\n--- Voter List ---\n";
        while (file >> name >> pass) {
            cout << count << ". " << name << endl;
            count++;
        }

        file.close();
    }

    void viewResults() {
        ifstream file("candidates.txt");
        if (!file) {
            cout << "No candidates available.\n";
            return;
        }

        string name, party;
        int vote;
        int totalVotes = 0;
        int maxVotes = -1;
        string leadingCandidate;

        cout << "\n--- Election Results ---\n";
        while (file >> quoted(name) >> quoted(party)) {
            vote = 0;


            ifstream votesFile("votes.txt");
            string votedName;
            while (votesFile >> quoted(votedName)) {
                if (votedName == name) {
                    vote++;
                }
            }
            votesFile.close();

            cout << name << " received " << vote << " votes.\n";
            totalVotes += vote;

            if (vote > maxVotes) {
                maxVotes = vote;
                leadingCandidate = name;
            }
        }

        file.close();
        cout << "Total Votes: " << totalVotes << endl;

        if (maxVotes > 0) {
            cout << "Candidate in lead: " << leadingCandidate << " with " << maxVotes << " votes.\n";
        }
        else {
            cout << "No votes have been cast yet.\n";
        }
    }



};
void Admin::viewElectionResults() {

    ifstream candFile("candidates.txt");
    string cname;
    string candidateNames[100];
    int candidateVotes[100] = { 0 };
    int candidateCount = 0;

    while (candFile >> cname) {
        candidateNames[candidateCount] = cname;
        candidateVotes[candidateCount] = 0;
        candidateCount++;
    }
    candFile.close();


    ifstream voteFile("votes.txt");
    string voter, votedFor;
    int totalVotes = 0;
    while (voteFile >> voter >> votedFor) {

        for (int i = 0; i < candidateCount; i++) {
            if (candidateNames[i] == votedFor) {
                candidateVotes[i]++;
                break;
            }
        }
        totalVotes++;
    }
    voteFile.close();


    for (int i = 0; i < candidateCount; i++) {
        cout << "Candidate " << candidateNames[i] << " received " << candidateVotes[i] << " votes." << endl;
    }

    cout << "Total Votes Cast: " << totalVotes << endl;
}

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



void adminMenu(Admin& admin);
void voterMenu();
void displayHelp();

void mainMenu() {
    int choice;
    Admin a;
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
            adminMenu(a);
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

void adminMenu(Admin& admin) {
    int option;
    do {
        system("cls");
        cout << "\n------ Admin Menu ------";
        cout << "\n1. Add Candidate";
        cout << "\n2. View All Candidates";
        cout << "\n3. View All Voters";
        cout << "\n4. View Election Results";
        cout << "\n5. Back to Main Menu";
        cout << "\nEnter choice: ";
        cin >> option;

        switch (option) {
        case 1:
            admin.addCandidate();
            break;
        case 2:
            admin.viewCandidates();
            break;
        case 3:
            admin.viewVoters();
            break;
        case 4:
            admin.viewResults();
            break;
        case 5:
            cout << "Returning to Main Menu...\n";
            break;
        default:
            cout << "Invalid choice, try again.\n";
        }
        system("pause");
    } while (option != 5);
}
string toLower(string str) {
    for (int i = 0; i < str.length(); i++)
        str[i] = tolower(str[i]);
    return str;
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

        case 1: {
            string username, password, candidate;
            cout << "Enter your username: ";
            cin >> username;
            cout << "Enter your password: ";
            cin >> password;

            ifstream infile("voters.txt");
            string storedUsername, storedPassword;
            bool found = false;

            while (infile >> storedUsername >> storedPassword) {
                if (username == storedUsername && password == storedPassword) {
                    found = true;
                    break;
                }
            }
            infile.close();

            if (found) {
                string cnic;
                int age;
                cout << "Enter your CNIC (13 digits): ";
                cin >> cnic;
                cout << "Enter your age: ";
                cin >> age;

                Voter v(username, password, cnic, age);
                if (age < 18 || !v.validatecnic(cnic)) {
                    cout << "Invalid age or CNIC. Cannot vote.\n";
                    break;
                }


                ifstream voteCheck("votes.txt");
                string votedCnic, votedCandidate;
                bool alreadyVoted = false;

                while (voteCheck >> votedCnic >> votedCandidate) {
                    if (votedCnic == cnic) {
                        alreadyVoted = true;
                        break;
                    }
                }
                voteCheck.close();

                if (alreadyVoted) {
                    cout << "CNIC already used to vote. You cannot vote again.\n";
                    break;
                }


                v.displayProvinceAndGender();

                cout << "Enter candidate name you want to vote for: ";
                cin.ignore();
                getline(cin, candidate);

                bool candidateExists = false;
                ifstream candidatesFile("candidates.txt");
                string fileCandidate;

                while (getline(candidatesFile, fileCandidate)) {
                    if (toLower(fileCandidate) == toLower(candidate)) {
                        candidateExists = true;
                        break;
                    }
                }

                candidatesFile.close();

                if (!candidateExists) {
                    cout << "Sorry, that candidate is not registered.\n";
                    break;
                }

                v.castvote(candidate);
                ofstream voteFile("votes.txt", ios::app);
                voteFile << cnic << " " << candidate << endl;
                voteFile.close();

                cout << "Your vote has been recorded successfully!\n";
            }
            else {
                cout << "Invalid credentials.\n";
            }
            break;
        }

        case 2: {
            string username, password;
            cout << "Enter your username: ";
            cin >> username;
            cout << "Enter your password: ";
            cin >> password;

            ifstream infile("voters.txt");
            string storedUsername, storedPassword, storedCnic;
            bool found = false;

            while (infile >> storedUsername >> storedPassword >> storedCnic) {
                if (username == storedUsername && password == storedPassword) {
                    found = true;
                    break;
                }
            }
            infile.close();

            if (!found) {
                cout << "Invalid credentials.\n";
                break;
            }


            ifstream voteCheck("votes.txt");
            string votedCnic, votedCandidate;
            bool hasVoted = false;

            while (voteCheck >> votedCnic >> votedCandidate) {
                if (votedCnic == storedCnic) {
                    cout << "You have already voted for: " << votedCandidate << endl;
                    hasVoted = true;
                    break;
                }
            }
            voteCheck.close();

            if (!hasVoted) {
                cout << "You have not voted yet.\n";
            }
            break;
        }

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
    cout << "-------------------------------------------------------------------------------------------------------------------------------\n";
    cout <<"\n================================================ HELP & GUIDELINES ===========================================================\n";
    cout << "-------------------------------------------------------------------------------------------------------------------------------\n";

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
    

    while (true) {
        system("cls");
        cout << "\n";
        cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "===========================================[  Welcome to Election 2025  ]==============================================" << endl;
		cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "\n";
        cout << "\t\t\t\t\t\t       Menu";
		cout << "\n-----------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "\n\t\t\t\t\t\t1. Admin Login\n\t\t\t\t\t\t2. Voter Login\n\t\t\t\t\t\t3. Register Admin\n\t\t\t\t\t\t4. Register Voter\n\t\t\t\t\t\t5. Exit\n\t\t\t\t\t\tEnter option: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1: {

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

            if (loginSuccess) {
                cout << " Admin Login Successful!\n";
                Admin admin(inputUsername, inputPassword);
                system("pause");
                adminMenu(admin);
            }
            else {
                char retry;
                cout << "Login failed! Incorrect username or password.\n";
                cout << "Would you like to retry? (y/n): ";
                cin >> retry;
                cin.ignore();
                if (retry == 'n' || retry == 'N') break;
            }
            break;
        }

        case 2: {
            string username, password;
            cout << "Enter your username: ";
            cin >> username;
            cout << "Enter your password: ";
            cin >> password;

            ifstream infile("voters.txt");
            string storedUsername, storedPassword, storedCnic;
            bool found = false;

            while (infile >> storedUsername >> storedPassword >> storedCnic) {
                if (username == storedUsername && password == storedPassword) {
                    found = true;
                    break;
                }
            }
            infile.close();

            if (!found) {
                cout << "Invalid information.\n";
                system("pause");
                break;
            }


            ifstream voteCheck("votes.txt");
            string votedCnic, votedCandidate;
            bool hasVoted = false;

            while (voteCheck >> votedCnic >> votedCandidate) {
                if (votedCnic == storedCnic) {
                    cout << "You have already voted for: " << votedCandidate << endl;
                    hasVoted = true;
                    break;
                }
            }
            voteCheck.close();

            if (!hasVoted) {
                cout << "You have not voted yet.\n";
            }


            voterMenu();
            break;
        }

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
