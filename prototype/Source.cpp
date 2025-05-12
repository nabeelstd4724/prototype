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
//void Admin ::viewElectionResults() {
//    //  Paste the fixed function here:
//    // Step 1: Read all candidates into an array
//    ifstream candFile("candidates.txt");
//    string cname;
//    string candidateNames[100]; // Assume max 100 candidates
//    int candidateVotes[100] = { 0 }; // To store votes
//    int candidateCount = 0;
//
//    while (candFile >> cname) {
//        candidateNames[candidateCount] = cname;
//        candidateVotes[candidateCount] = 0;
//        candidateCount++;
//    }
//    candFile.close();
//
//    // Step 2: Read votes and update vote count for each candidate
//    ifstream voteFile("votes.txt");
//    string voter, votedFor;
//    int totalVotes = 0;
//    while (voteFile >> voter >> votedFor) {
//        // Search which candidate this vote is for
//        for (int i = 0; i < candidateCount; i++) {
//            if (candidateNames[i] == votedFor) {
//                candidateVotes[i]++;
//                break;
//            }
//        }
//        totalVotes++;
//    }
//    voteFile.close();
//
//    // Step 3: Display results
//    for (int i = 0; i < candidateCount; i++) {
//        cout << "Candidate " << candidateNames[i] << " received " << candidateVotes[i] << " votes." << endl;
//    }
//
//    cout << "Total Votes Cast: " << totalVotes << endl;
//}






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
	cout << "----------------------------------------------------------------------------------------------------------------------------\n";
    cout << "===============================================[  Election Results  ]=======================================================\n";
	cout << "----------------------------------------------------------------------------------------------------------------------------\n";
    cout << "Total Votes: " << Candidate::get_totalvote() << endl;
}

void adminMenu();
void voterMenu();
void displayHelp();

void mainMenu() {
    int choice;
    do {
        cout << "-----------------------------------------------------------------------------------------------------------------------------\n";
        cout << "=====================================[  Election Management System  ]========================================================";
        cout << "----------------------------------------------------------------------------------------------------------------------------\n";
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
        cout << "\n======================================================================================================================";
        cout << "\n\t\t\t\t\t\t[ Admin Menu ]";
        cout << "\n======================================================================================================================";

        cout << "\n\t\t\t\t\t1. Add Candidate";
        cout << "\n\t\t\t\t\t2. Add Voter";
        cout << "\n\t\t\t\t\t3. View All Candidates";
        cout << "\n\t\t\t\t\t4. View All Voters";
        cout << "\n\t\t\t\t\t5. View Election Results";
        cout << "\n\t\t\t\t\t6. Back to Main Menu";
        cout << "\n\t\t\t\t\tEnter choice: ";
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
        cout << "\n======================================================================================================================";
        cout << "\n\t\t\t\t\t Voter Menu ";
		cout << "\n======================================================================================================================";
        cout << "\n1. Cast Vote";
        cout << "\n2. Check Voting Status";
        cout << "\n3. View Election Results";
        cout << "\n4. Back to Main Menu";
        cout << "\nEnter choice: ";
        cin >> option;

        switch (option) 
        {
           case 1:
           {
              cout << "Casting vote logic here...\n";
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
                      cout << " Invalid age or CNIC. Cannot vote.\n";
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
                      cout << " CNIC already used to vote. You cannot vote again.\n";
                      break;
                  }

                  /*v.displayProvinceAndGender();*/

                  cout << "Enter candidate name you want to vote for: ";
                  cin.ignore();
                  getline(cin, candidate);


                  bool candidateExists = false;
                  ifstream candidatesFile("candidates.txt");
                  string fileCandidate;

                  while (getline(candidatesFile, fileCandidate)) {
                      if (fileCandidate == candidate) {
                          candidateExists = true;
                          break;
                      }
                  }
                  candidatesFile.close();

                  if (!candidateExists) {
                      cout << " Sorry, that candidate is not registered.\n";
                      break;
                  }

                  v.castvote(candidate);
                  ofstream voteFile("votes.txt", ios::app); // store vote with CNIC
                  voteFile << cnic << " " << candidate << endl;
                  voteFile.close();

                  cout << " Your vote has been recorded successfully!\n";

              }
              else {
                  cout << " Invalid credentials.\n";
              }
              
              break;
           }
           case 2:
           {
              cout << "Checking voting status...\n";
              break;
           }
           case 3:
           {
              displayResults();
              break;
           }    
           case 4:
           {
              cout << "Returning to Main Menu...\n";
              break;
           }    
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
	cout << "--------------------------------------------------------------------------------------------------------------------------\n";
    cout << "\n=============================================== HELP & GUIDELINES ======================================================\n";
    cout << "Welcome to the Online Voting System!\n";
    cout << "--------------------------------------------------------------------------------------------------------------------------\n";

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

    cout << "\n------------------------------------------------------------------------------------------------------------------------\n";
}

int main() {
    int choice;
    cout << "Welcome to the Election Management System\n";

    while (true) {
        system("cls");
        cout << "---------------------------------------------------------------------------------------------------------------------\n";
        cout << "========================================[  Election Management System  ]=============================================\n";
        cout << "---------------------------------------------------------------------------------------------------------------------\n";
		cout << "\n\t\t\t\t\t\t   [ Main Menu ]";
        cout << "\n";
        cout << "\n\t\t\t\t\t\t1. Admin Login\n\t\t\t\t\t\t2. Voter Login\n\t\t\t\t\t\t3. Register Admin\n\t\t\t\t\t\t4. Register Voter\n\t\t\t\t\t\t5. Exit\nEnter option: ";
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
