#include <iostream>
#include <string>
#include <cstring> 
#include <cctype>  // For isalpha, isdigit, tolower
#include <fstream>
#include <cstdlib> // For system("cls"), system("pause")
#include <iomanip> 

using namespace std;

// Forward declarations
class Voter;
class Admin;
void adminMenu(Admin& admin);
void voterMenu(Voter& loggedInVoter);
void displayHelp();
void displayResults(const string& electionType);
bool isElectionActive(const string& electionType); // Added
void clearInputBuffer(); // Modified prototype

class User {
protected:
    string username;
    string password;
public:
    User() : username(""), password("") {}
    User(string u, string p) : username(u), password(p) {}
    void setusername(string u) { username = u; }
    void setuserpassword(string p) { password = p; }
    string getpassword() const { return password; }
    string getusername() const { return username; }
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

class Voter : public User {
private:
    string cnic;
    int age;
public:
    Voter() : User(), cnic(""), age(0) {}
    Voter(string u, string p, string c, int a = 0) : User(u, p), cnic(c), age(a) {}

    string getcnic() const { return cnic; }
    int getage() const { return age; }

    void setage(int a) {
        if (a < 18) {
            // cout << "Age must be 18 or older for voting eligibility.\n"; // Warning can be given at point of voting
        }
        this->age = a;
    }

    bool validatecnic(const string& c) const {
        if (c.length() != 13) return false;
        for (char ch : c) {
            if (!isdigit(ch)) return false;
        }
        return true;
    }

    void displayProvinceAndGender() const {
        if (cnic.length() != 13) {
            cout << "Invalid CNIC format. Cannot determine province and gender.\n";
            return;
        }
        char firstDigit = cnic[0];
        string province;
        switch (firstDigit) {
        case '1': province = "Khyber Pakhtunkhwa"; break;
        case '2': province = "Fata (Merged Districts)"; break;
        case '3': province = "Punjab"; break;
        case '4': province = "Sindh"; break;
        case '5': province = "Balochistan"; break;
        case '6': province = "Islamabad Capital Territory"; break;
        case '7': province = "Gilgit-Baltistan"; break;
        default: province = "Unknown or AJ&K"; break;
        }
        char lastDigit = cnic[12];
        string gender = "Unknown Gender";
        if (isdigit(lastDigit)) {
            int digit = lastDigit - '0';
            gender = (digit % 2 == 0) ? "Female" : "Male";
        }
        cout << "Province: " << province << endl;
        cout << "Gender: " << gender << endl;
    }
};

class Candidate {
private:
    string candidate_name;
    string party_name;
public:
    Candidate(string n, string p) : candidate_name(n), party_name(p) {}
    string getcandidatename() const { return candidate_name; }
    string getpartyname() const { return party_name; }
    void display_candidateinfo() const {
        cout << "Candidate name:     " << candidate_name << endl;
        cout << "Party affiliation:   " << party_name << endl;
    }
};

string toLower(string str) {
    for (char& c : str) {
        c = tolower(c);
    }
    return str;
}

// Modified clearInputBuffer to not use <limits>
void clearInputBuffer() {
    cin.clear(); // Clear error flags
    char c;
    while (cin.get(c) && c != '\n'); // Read characters until newline or EOF
}

void displayResults(const string& electionType) {
    string candidatesFilename = "candidates_" + electionType + ".txt";
    string votesFilename = "votes_" + electionType + ".txt";

    ifstream candFile(candidatesFilename);
    if (!candFile) {
        cout << "Candidates file for " << electionType << " elections ('" << candidatesFilename << "') not found.\n";
        return;
    }

    cout << "\n========== " << (electionType == "local" ? "Local" : "National") << " Election Results ==========\n";
    string candName, candParty;
    int maxVotes = -1;
    bool anyCandidatesListed = false;

    // Rewind file or re-open if necessary. Since it's just opened, it's at the beginning.
    // Changed: Direct reading from file stream, removed stringstream
    while (candFile >> quoted(candName) >> quoted(candParty)) {
        anyCandidatesListed = true;
        int currentCandidateVotes = 0;
        ifstream voteFile_check(votesFilename);
        if (voteFile_check) {
            string voterCnic_check, votedFor_check;
            // Changed: Direct reading from file stream
            while (voteFile_check >> voterCnic_check >> quoted(votedFor_check)) {
                if (votedFor_check == candName) {
                    currentCandidateVotes++;
                }
            }
            voteFile_check.close();
        }

        cout << "Candidate: " << left << setw(25) << candName
            << "Party: " << left << setw(20) << candParty
            << "Votes: " << currentCandidateVotes << endl;

        if (currentCandidateVotes > maxVotes) {
            maxVotes = currentCandidateVotes;
        }
    }
    candFile.close();

    if (!anyCandidatesListed) {
        cout << "No candidates registered for " << electionType << " elections.\n";
        return;
    }

    int totalVotesCastedOverall = 0;
    ifstream countVoteFile(votesFilename);
    if (countVoteFile) {
        string line_count; // Still use getline here to count lines robustly
        while (getline(countVoteFile, line_count)) {
            if (!line_count.empty()) totalVotesCastedOverall++;
        }
        countVoteFile.close();
    }

    cout << "--------------------------------------\n";
    cout << "Total Votes Cast in " << electionType << " election: " << totalVotesCastedOverall << endl;

    if (maxVotes >= 0 && totalVotesCastedOverall > 0) {
        cout << "Leading Candidate(s): ";
        bool firstLeader = true;

        ifstream candFileAgain(candidatesFilename); // Re-open for second pass
        if (!candFileAgain) {
            // Should not happen if first read worked, but good practice
            return;
        }

        string tempCandNameRead, tempCandPartyRead;
        // Changed: Direct reading from file stream
        while (candFileAgain >> quoted(tempCandNameRead) >> quoted(tempCandPartyRead)) {
            int tempVotes = 0;
            ifstream voteFileAgain_check(votesFilename);
            if (voteFileAgain_check) {
                string vCnic_check, vFor_check;
                // Changed: Direct reading from file stream
                while (voteFileAgain_check >> vCnic_check >> quoted(vFor_check)) {
                    if (vFor_check == tempCandNameRead) tempVotes++;
                }
                voteFileAgain_check.close();
            }
            if (tempVotes == maxVotes) {
                if (!firstLeader) cout << ", ";
                cout << tempCandNameRead;
                firstLeader = false;
            }
        }
        candFileAgain.close();
        cout << " with " << maxVotes << " votes.\n";
    }
    else if (anyCandidatesListed) {
        cout << "No votes have been cast yet for registered candidates in the " << electionType << " election.\n";
    }
    cout << "=====================================\n";
}

class Admin : public User {
public:
    Admin() {}
    Admin(string u, string p) : User(u, p) {}

    void addCandidate(const string& electionType) {
        string name, party;
        string filename = "candidates_" + electionType + ".txt";

        cout << "Enter candidate name for " << electionType << " election: ";
        getline(cin >> ws, name);

        ifstream readFile(filename);
        string existingName, existingParty;
        bool duplicate = false;
        if (readFile) {
            // Changed: Direct reading from file stream
            while (readFile >> quoted(existingName) >> quoted(existingParty)) {
                if (toLower(existingName) == toLower(name)) {
                    duplicate = true;
                    break;
                }
            }
            readFile.close();
        }

        if (duplicate) {
            cout << "Candidate " << name << " is already registered for " << electionType << " elections.\n";
            return;
        }

        cout << "Enter party name for " << name << ": ";
        getline(cin >> ws, party);

        ofstream file(filename, ios::app);
        if (!file) {
            cerr << "Error: Could not open " << filename << " for writing.\n";
            return;
        }
        file << quoted(name) << " " << quoted(party) << endl;
        file.close();
        cout << "Candidate added successfully to " << electionType << " elections!\n";
    }

    void viewCandidates(const string& electionType) {
        string filename = "candidates_" + electionType + ".txt";
        ifstream file(filename);
        if (!file) {
            cout << "File '" << filename << "' not found or no candidates for " << electionType << " elections.\n";
            return;
        }
        string name, party;
        int count = 1;
        cout << "\n--- Candidate List for " << electionType << " Elections ---\n";
        bool foundAny = false;
        // Changed: Direct reading from file stream
        while (file >> quoted(name) >> quoted(party)) {
            cout << count++ << ". " << name << " (" << party << ")\n";
            foundAny = true;
        }
        if (!foundAny) {
            cout << "No candidates are currently registered for " << electionType << " elections.\n";
        }
        file.close();
        cout << "------------------------------------------------\n";
    }

    void viewVoters() {
        ifstream file("voters.txt");
        if (!file) {
            cout << "Voters file ('voters.txt') not found or no voters registered.\n";
            return;
        }
        string username, password, cnic;
        int count = 1;
        cout << "\n--- Registered Voter List (Usernames only) ---\n";
        bool foundAny = false;
        while (file >> username >> password >> cnic) { // CNIC & password not displayed, just used for parsing
            cout << count++ << ". " << username << endl;
            foundAny = true;
        }
        if (!foundAny) {
            cout << "No voters are currently registered.\n";
        }
        file.close();
        cout << "---------------------------------------------\n";
    }

    void toggleElectionStatus(const string& electionType) {
        string statusFilename = "status_" + electionType + ".txt";
        string currentStatus = "inactive"; // Default if file doesn't exist or content is not "active"

        ifstream statusFileIn(statusFilename);
        if (statusFileIn) {
            string fileStatus;
            if (statusFileIn >> fileStatus) { // Read status from file
                if (fileStatus == "active") {
                    currentStatus = "active";
                }
            }
            statusFileIn.close();
        }

        string newStatus = (currentStatus == "active") ? "inactive" : "active";
        ofstream statusFileOut(statusFilename); // Overwrite
        if (!statusFileOut) {
            cerr << "Error: Could not open status file " << statusFilename << " for writing.\n";
            return;
        }
        statusFileOut << newStatus;
        statusFileOut.close();
        cout << electionType << " election status changed from " << currentStatus << " to " << newStatus << ".\n";
    }
};


bool isElectionActive(const string& electionType) {
    string statusFilename = "status_" + electionType + ".txt";
    ifstream statusFile(statusFilename);
    string statusRead;
    if (statusFile && (statusFile >> statusRead)) {
        statusFile.close();
        return statusRead == "active";
    }
    statusFile.close(); // Ensure closed even if read failed or file not found
    return false; // Default to inactive if file not found or unreadable/empty or not "active"
}


void adminMenu(Admin& admin) {
    int option;
    string electionTypeChoice;
    do {
        system("cls");
        cout << "\n------ Admin Menu ------";
        cout << "\nLogged in as: " << admin.getusername();
        cout << "\n1. Add Candidate";
        cout << "\n2. View Candidates";
        cout << "\n3. View All Registered Voters";
        cout << "\n4. View Election Results";
        cout << "\n5. Toggle Election Status (Active/Inactive)";
        cout << "\n6. Back to Startup Menu"; // Shifted option
        cout << "\nEnter choice: ";
        cin >> option;

        if (cin.fail()) {
            cout << "Invalid input. Please enter a number.\n";
            clearInputBuffer();
            system("pause");
            continue;
        }

        switch (option) {
        case 1:
        case 2:
        case 4:
        case 5: // Toggle also needs election type
            cout << "For which election type (local/national)? ";
            cin >> electionTypeChoice;
            if (electionTypeChoice != "local" && electionTypeChoice != "national") {
                cout << "Invalid election type. Please enter 'local' or 'national'.\n";
            }
            else {
                if (option == 1) admin.addCandidate(electionTypeChoice);
                else if (option == 2) admin.viewCandidates(electionTypeChoice);
                else if (option == 4) displayResults(electionTypeChoice);
                else if (option == 5) admin.toggleElectionStatus(electionTypeChoice);
            }
            break;
        case 3:
            admin.viewVoters();
            break;
        case 6: // Changed option number
            cout << "Returning to Startup Menu...\n";
            break;
        default:
            cout << "Invalid choice, try again.\n";
        }
        if (option != 6) system("pause"); // Changed option number
    } while (option != 6); // Changed option number
}

void voterMenu(Voter& loggedInVoter) {
    int option;
    string electionTypeChoice;
    do {
        system("cls");
        cout << "\n------ Voter Menu ------";
        cout << "\nLogged in as: " << loggedInVoter.getusername() << " (CNIC: " << loggedInVoter.getcnic() << ")";
        cout << "\n1. Cast Vote";
        cout << "\n2. Check My Voting Status";
        cout << "\n3. View Public Election Results";
        cout << "\n4. Back to Startup Menu";
        cout << "\nEnter choice: ";
        cin >> option;

        if (cin.fail()) {
            cout << "Invalid input. Please enter a number.\n";
            clearInputBuffer();
            system("pause");
            continue;
        }

        switch (option) {
        case 1: // Cast Vote
        case 2: // Check Status
            cout << "For which election type (local/national)? ";
            cin >> electionTypeChoice;
            if (electionTypeChoice != "local" && electionTypeChoice != "national") {
                cout << "Invalid election type. Please enter 'local' or 'national'.\n";
                break;
            }

            if (option == 1) { // Cast Vote
                if (!isElectionActive(electionTypeChoice)) {
                    cout << "The " << electionTypeChoice << " election is currently NOT ACTIVE. Voting is disabled.\n";
                    break;
                }

                string voterCnic = loggedInVoter.getcnic();
                int currentAge;
                string votesFilename = "votes_" + electionTypeChoice + ".txt";
                string candidatesFilename = "candidates_" + electionTypeChoice + ".txt";

                bool alreadyVoted = false;
                ifstream voteCheckFile(votesFilename);
                if (voteCheckFile) {
                    string cnicFromFile, votedCandidateFromFile_check;
                    // Changed: Direct reading
                    while (voteCheckFile >> cnicFromFile >> quoted(votedCandidateFromFile_check)) {
                        if (cnicFromFile == voterCnic) {
                            alreadyVoted = true;
                            cout << "You (CNIC: " << voterCnic << ") have already voted in the "
                                << electionTypeChoice << " election for: " << votedCandidateFromFile_check << ".\n";
                            break;
                        }
                    }
                    voteCheckFile.close();
                }

                if (alreadyVoted) {
                    cout << "You cannot vote again in this election type.\n";
                    break;
                }

                cout << "To cast your vote, please confirm your current age: ";
                cin >> currentAge;
                if (cin.fail() || currentAge <= 0) {
                    cout << "Invalid age input.\n";
                    if (cin.fail()) clearInputBuffer();
                    break;
                }
                loggedInVoter.setage(currentAge);

                if (loggedInVoter.getage() < 18) {
                    cout << "Voting failed: Voter must be 18 or older.\n";
                    break;
                }

                loggedInVoter.displayProvinceAndGender();

                ifstream candFile_list(candidatesFilename);
                if (!candFile_list) {
                    cout << "Candidate list for " << electionTypeChoice << " elections ('" << candidatesFilename << "') not available.\n";
                    break;
                }
                cout << "\n--- Candidates for " << electionTypeChoice << " Election ---\n";
                string cName_list, cParty_list;
                int candCount = 1;
                bool anyCandListed = false;
                // Changed: Direct reading
                while (candFile_list >> quoted(cName_list) >> quoted(cParty_list)) {
                    cout << candCount++ << ". " << cName_list << " (" << cParty_list << ")" << endl;
                    anyCandListed = true;
                }
                candFile_list.close();
                if (!anyCandListed) {
                    cout << "No candidates are registered for the " << electionTypeChoice << " election.\n";
                    break;
                }
                cout << "-----------------------------------------------\n";

                string candidateToVoteFor;
                cout << "Enter the full name of the candidate you want to vote for: ";
                getline(cin >> ws, candidateToVoteFor);

                bool candidateExists = false;
                string officialCandidateName_vote;
                ifstream candFile_validate(candidatesFilename);
                if (candFile_validate) {
                    string nameRead_val, partyRead_val;
                    // Changed: Direct reading
                    while (candFile_validate >> quoted(nameRead_val) >> quoted(partyRead_val)) {
                        if (toLower(nameRead_val) == toLower(candidateToVoteFor)) {
                            candidateExists = true;
                            officialCandidateName_vote = nameRead_val;
                            break;
                        }
                    }
                    candFile_validate.close();
                }

                if (!candidateExists) {
                    cout << "Sorry, candidate \"" << candidateToVoteFor << "\" is not registered for "
                        << electionTypeChoice << " elections or name mistyped.\n";
                    break;
                }

                ofstream voteFileOut(votesFilename, ios::app);
                if (!voteFileOut) {
                    cerr << "Error: Could not open " << votesFilename << " for writing. Vote not recorded.\n";
                    break;
                }
                voteFileOut << voterCnic << " " << quoted(officialCandidateName_vote) << endl;
                voteFileOut.close();

                cout << "[+] You have successfully voted for candidate: " << officialCandidateName_vote
                    << " in the " << electionTypeChoice << " election.\n";
                cout << "Your vote has been recorded.\n";

            }
            else if (option == 2) { // Check Status
                string voterCnic_status = loggedInVoter.getcnic();
                string votesFilename_status = "votes_" + electionTypeChoice + ".txt";
                ifstream voteCheckFile_status(votesFilename_status);
                string cnicFromFile_status, votedCandFromFile_status;
                bool hasVoted_status = false;
                if (voteCheckFile_status) {
                    // Changed: Direct reading
                    while (voteCheckFile_status >> cnicFromFile_status >> quoted(votedCandFromFile_status)) {
                        if (cnicFromFile_status == voterCnic_status) {
                            hasVoted_status = true;
                            break;
                        }
                    }
                    voteCheckFile_status.close();
                }

                if (hasVoted_status) {
                    cout << "You (CNIC: " << voterCnic_status << ") have ALREADY VOTED in the "
                        << electionTypeChoice << " election for: " << votedCandFromFile_status << endl;
                }
                else {
                    cout << "You (CNIC: " << voterCnic_status << ") have NOT YET VOTED in the "
                        << electionTypeChoice << " election.\n";
                }
            }
            break;
        case 3: // View Results
            cout << "View results for which election type (local/national)? ";
            cin >> electionTypeChoice;
            if (electionTypeChoice != "local" && electionTypeChoice != "national") {
                cout << "Invalid election type. Please enter 'local' or 'national'.\n";
            }
            else {
                displayResults(electionTypeChoice);
            }
            break;
        case 4:
            cout << "Returning to Startup Menu...\n";
            break;
        default:
            cout << "Invalid choice, try again.\n";
        }
        if (option != 4) system("pause");
    } while (option != 4);
}

void registerAdmin() {
    string newUsername, newPassword;
    cout << "\n--- Admin Registration ---\n";
    do {
        cout << "Enter new admin username: ";
        getline(cin >> ws, newUsername);
        if (newUsername.empty()) {
            cout << "Username cannot be empty. Try again.\n";
        }
        else {
            ifstream infile("admin.txt");
            string existingUsername_reg, existingPassword_reg;
            bool usernameTaken = false;
            if (infile) {
                while (infile >> existingUsername_reg >> existingPassword_reg) {
                    if (existingUsername_reg == newUsername) {
                        usernameTaken = true;
                        break;
                    }
                }
                infile.close();
            }
            if (usernameTaken) {
                cout << "Username \"" << newUsername << "\" already exists! Please choose a different one.\n";
                newUsername = ""; // Force loop to repeat for username
            }
        }
    } while (newUsername.empty());

    do {
        cout << "Enter password (5 characters, at least one letter and one digit): ";
        cin >> newPassword; // Keep it simple with cin >>
        if (!isValidPassword(newPassword)) {
            cout << "Invalid password format. Password must be 5 characters long and include at least one letter and one digit. Try again.\n";
        }
    } while (!isValidPassword(newPassword));

    ofstream outfile("admin.txt", ios::app);
    if (!outfile) {
        cerr << "Error: Could not open admin.txt for writing.\n";
        return;
    }
    outfile << newUsername << " " << newPassword << endl;
    outfile.close();
    cout << "[+] Admin \"" << newUsername << "\" registered successfully!\n";
}

void registerVoter() {
    string newUsername, newPassword, newCnic;
    cout << "\n--- Voter Registration ---\n";

    // Username registration loop
    do {
        cout << "Enter new voter username: ";
        getline(cin >> ws, newUsername); // Use getline to handle potential spaces if allowed, ws to consume leading whitespace
        if (newUsername.empty()) {
            cout << "Username cannot be empty. Try again.\n";
        }
        else {
            ifstream readCheckFile("voters.txt");
            string existingUsername_check, tempPass, tempCnic;
            bool usernameExists = false;
            if (readCheckFile) {
                while (readCheckFile >> existingUsername_check >> tempPass >> tempCnic) {
                    if (existingUsername_check == newUsername) {
                        usernameExists = true;
                        break;
                    }
                }
                readCheckFile.close();
            }
            if (usernameExists) {
                cout << "Username \"" << newUsername << "\" already exists! Please choose a different one.\n";
                newUsername = ""; // Invalidate username to force loop repetition
            }
        }
    } while (newUsername.empty());

    Voter tempVoterValidator; // Used for CNIC validation
    bool cnicValidAndUnique = false; // Flag to control the CNIC input loop

    // CNIC registration loop
    do {
        cout << "Enter your CNIC (13 digits, no spaces): ";
        cin >> newCnic;

        if (!tempVoterValidator.validatecnic(newCnic)) {
            cout << "Invalid CNIC format. Must be 13 digits and contain only numbers. Try again.\n";
            cnicValidAndUnique = false; // CNIC format is invalid, loop must continue
        }
        else {
            // CNIC format is valid, now check for uniqueness
            ifstream readCheckFile("voters.txt");
            string tempUser_cnicCheck, tempPass_cnicCheck, existingCnic_check;
            bool cnicExists = false;
            if (readCheckFile) {
                while (readCheckFile >> tempUser_cnicCheck >> tempPass_cnicCheck >> existingCnic_check) {
                    if (existingCnic_check == newCnic) {
                        cnicExists = true;
                        break;
                    }
                }
                readCheckFile.close();
            }

            if (cnicExists) {
                cout << "This CNIC (" << newCnic << ") is already registered. Please use a different CNIC.\n";
                cnicValidAndUnique = false; // CNIC is not unique, loop must continue
            }
            else {
                cnicValidAndUnique = true; // CNIC format is good and it's unique
            }
        }
    } while (!cnicValidAndUnique); // Loop until CNIC is both valid in format and unique

    // Password registration loop
    do {
        cout << "Enter password (5 characters, at least one letter and one digit): ";
        cin >> newPassword;
        if (!isValidPassword(newPassword)) {
            cout << "Invalid password format. Password must be 5 characters long and include at least one letter and one digit. Try again.\n";
        }
    } while (!isValidPassword(newPassword));

    // Age input
    int ageInput;
    cout << "Enter your age: ";
    cin >> ageInput;
    // Loop to ensure valid age is entered
    while (cin.fail() || ageInput < 18) {
        if (cin.fail()) {
            cout << "Invalid age input. Please enter a number.\n";
            clearInputBuffer(); // Clear error flags and buffer
        }
        else { // ageInput < 18 but not a cin.fail()
            cout << "Age must be 18 or older to register. Please enter a valid age.\n";
        }
        cout << "Enter your age: "; // Re-prompt
        cin >> ageInput;
    }

    // Write to file
    ofstream outfile("voters.txt", ios::app);
    if (!outfile) {
        cerr << "Error: Could not open voters.txt for writing.\n";
        return;
    }
    // Store the validated and unique CNIC, along with other details
    outfile << newUsername << " " << newPassword << " " << newCnic << endl;
    outfile.close();

    cout << "[+] Voter \"" << newUsername << "\" registered successfully with CNIC: " << newCnic << "!\n";
}

void displayHelp() {
    system("cls");
    cout << "-------------------------------------------------------------------------------------------------------------------------------\n";
    cout << "\n================================================ HELP & GUIDELINES ===========================================================\n";
    cout << "-------------------------------------------------------------------------------------------------------------------------------\n";
    cout << "Welcome to the Online Voting System!\n";
    cout << "----------------------------------------\n";
    cout << "\n1. Startup menu:\n";
    cout << "   - Login as Admin or Voter (up to 3 attempts).\n";
    cout << "   - Register new Admin or Voter accounts. System checks for unique usernames/CNICs.\n";
    cout << "   - View public election results for 'local' or 'national' elections.\n";

    cout << "\n2. Election Type:\n";
    cout << "   - System supports 'local' and 'national' elections separately.\n";
    cout << "   - Voters can vote ONCE in each election type (local AND national).\n";
    cout << "   - Elections (local/national) must be 'active' for voting (Admin controlled).\n";


    cout << "\n3. Voter Action (after login & selecting election type):\n";
    cout << "   - Cast Vote: Choose a candidate. Age must be 18+. Province/Gender estimated from CNIC.\n";
    cout << "     (Voting only possible if Admin has set the specific election to 'active').\n";
    cout << "   - Check Status: See if you have voted in the selected election type.\n";

    cout << "\n4. Admin Action (after login & selecting election type where applicable):\n";
    cout << "   - Manage Candidates: Add/View candidates for 'local' or 'national' elections.\n";
    cout << "   - View Voters: List all registered voter accounts (usernames only).\n";
    cout << "   - View Results: See vote counts for a specific election type.\n";
    cout << "   - Toggle Election Status: Set 'local' or 'national' elections to 'active' or 'inactive'.\n";

    cout << "\n5. Data:\n";
    cout << "   - Password: Exactly 5 characters, must contain at least one letter and one digit.\n";
    cout << "   - CNIC: Exactly 13 digits, no spaces. Must be unique for each voter.\n";
    cout << "   - Usernames: Must be unique for Admins and unique for Voters respectively.\n";
    cout << "   - Data Files: admin.txt, voters.txt, candidates_local.txt, candidates_national.txt,\n";
    cout << "                 votes_local.txt, votes_national.txt, status_local.txt, status_national.txt.\n";
    cout << "-------------------------------------------------------------------------------------------------------------------------------\n";
}

// (Previous code remains the same: includes, class definitions, functions etc.)


int main() {
    int choice;


    while (true) {
        system("cls");
        cout << "\n";
        cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "===========================================[  Election Management System  ]=============================================" << endl;
        cout << "-----------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "\n\t\t\t\t\t\t    STARTUP MENU";
        cout << "\n-----------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "\n\t\t\t\t\t\t1. Admin Login";
        cout << "\n\t\t\t\t\t\t2. Voter Login";
        cout << "\n\t\t\t\t\t\t3. Register New Admin";
        cout << "\n\t\t\t\t\t\t4. Register New Voter";
        cout << "\n\t\t\t\t\t\t5. View Public Election Results";
        cout << "\n\t\t\t\t\t\t6. Help & Guidelines";
        cout << "\n\t\t\t\t\t\t7. Exit Application";
        cout << "\n\t\t\t\t\t\tEnter option: ";
        cin >> choice;

        if (cin.fail()) {
            cout << "Invalid input. Please enter a number.\n";
            clearInputBuffer();
            system("pause");
            continue;
        }

        switch (choice) {
        case 1: { // Admin Login
            Admin adminUser; // To store the logged-in admin
            bool loginSuccess = false;

            for (int overallAttempt = 0; overallAttempt < 3 && !loginSuccess; ++overallAttempt) {
                cout << "\n--- Admin Login Attempt " << overallAttempt + 1 << " of 3 ---" << endl;
                cout << "Enter Admin Username: ";
                string inputUsername;
                cin >> inputUsername;

                ifstream infile("admin.txt");
                if (!infile) {
                    cout << "Admin file ('admin.txt') not found. Register an admin first.\n";
                    // No system("pause") here, main loop's pause will catch it later.
                    break; // Break from the overallAttempt loop for admin login
                }

                string storedUsername, storedPassword;
                bool usernameFound = false;
                string correctPasswordForUser;

                while (infile >> storedUsername >> storedPassword) {
                    if (inputUsername == storedUsername) {
                        usernameFound = true;
                        correctPasswordForUser = storedPassword;
                        break;
                    }
                }
                infile.close();

                if (!usernameFound) {
                    cout << "[!] Username \"" << inputUsername << "\" not found.\n";
                    if (overallAttempt == 2) { // Last overall attempt
                        cout << "Maximum login attempts reached.\n";
                    }
                    else {
                        cout << "Please try again.\n";
                    }
                    continue; // Go to the next overallAttempt (will re-ask username)
                }

                // Username exists, now try password up to 3 times for this specific user
                for (int passAttempt = 0; passAttempt < 3; ++passAttempt) {
                    cout << "Enter Password for admin \"" << inputUsername << "\" (password attempt " << passAttempt + 1 << "/3): ";
                    string inputPassword;
                    cin >> inputPassword;

                    if (inputPassword == correctPasswordForUser) {
                        loginSuccess = true;
                        adminUser = Admin(inputUsername, inputPassword);
                        break; // Correct password, break from passAttempt loop
                    }
                    else {
                        cout << "[!] Incorrect password.\n";
                        if (passAttempt == 2) { // Last password attempt for this user
                            cout << "Maximum password attempts for user \"" << inputUsername << "\" reached.\n";
                            // This overallAttempt is now considered failed due to password failure
                        }
                    }
                } // End of password attempt loop

                if (loginSuccess) {
                    break; // Break from overallAttempt loop as login is successful
                }
                else if (overallAttempt == 2 && !usernameFound) {
                    // This condition should have been caught by usernameFound check already. Redundant here.
                }
                else if (overallAttempt == 2) { // If it was the last overall attempt and password failed
                    cout << "Maximum login attempts reached (due to password failures).\n";
                }
                else if (!loginSuccess) { // If password attempts failed but overall attempts remain
                    cout << "Returning to username prompt.\n"; // Inform user they will be asked for username again
                }

            } // End of overallAttempt loop

            if (loginSuccess) {
                cout << "\n[+] Admin Login Successful!\n";
                system("pause");
                adminMenu(adminUser);
            }
            else {
                // Message about login failure overall already shown within the loop.
                // cout << "Admin login failed after all attempts.\n";
            }
            system("pause"); // Pause after the entire admin login process completes or fails
            break;
        }
        case 2: { // Voter Login
            Voter loggedInVoter;
            bool loginSuccess = false;

            for (int overallAttempt = 0; overallAttempt < 3 && !loginSuccess; ++overallAttempt) {
                cout << "\n--- Voter Login Attempt " << overallAttempt + 1 << " of 3 ---" << endl;
                cout << "Enter Voter Username: ";
                string inputUsername;
                cin >> inputUsername;

                ifstream infile("voters.txt");
                if (!infile) {
                    cout << "Voter file ('voters.txt') not found. Register as a voter first.\n";
                    break; // Break from overallAttempt loop for voter login
                }

                string storedUsername, storedPassword, storedCnic;
                bool usernameFound = false;
                string correctPasswordForUser;
                string cnicForUser;

                while (infile >> storedUsername >> storedPassword >> storedCnic) {
                    if (inputUsername == storedUsername) {
                        usernameFound = true;
                        correctPasswordForUser = storedPassword;
                        cnicForUser = storedCnic;
                        break;
                    }
                }
                infile.close();

                if (!usernameFound) {
                    cout << "[!] Voter username \"" << inputUsername << "\" not found.\n";
                    if (overallAttempt == 2) {
                        cout << "Maximum login attempts reached.\n";
                    }
                    else {
                        cout << "Please try again.\n";
                    }
                    continue; // Go to next overallAttempt (will re-ask username)
                }

                // Username exists, now try password
                for (int passAttempt = 0; passAttempt < 3; ++passAttempt) {
                    cout << "Enter Password for voter \"" << inputUsername << "\" (password attempt " << passAttempt + 1 << "/3): ";
                    string inputPassword;
                    cin >> inputPassword;

                    if (inputPassword == correctPasswordForUser) {
                        loginSuccess = true;
                        loggedInVoter = Voter(inputUsername, inputPassword, cnicForUser);
                        break; // Correct password, break from passAttempt loop
                    }
                    else {
                        cout << "[!] Incorrect password.\n";
                        if (passAttempt == 2) {
                            cout << "Maximum password attempts for user \"" << inputUsername << "\" reached.\n";
                        }
                    }
                } // End password attempt loop

                if (loginSuccess) {
                    break; // Break from overallAttempt loop
                }
                else if (overallAttempt == 2) {
                    cout << "Maximum login attempts reached (due to password failures).\n";
                }
                else if (!loginSuccess) {
                    cout << "Returning to username prompt.\n";
                }
            } // End overallAttempt loop

            if (loginSuccess) {
                cout << "\n[+] Voter Login Successful!\n";
                system("pause");
                voterMenu(loggedInVoter);
            }
            else {
                // Message already given
            }
            system("pause"); // Pause after voter login process
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
        case 5: {
            string type_results;
            cout << "View results for which election type (local/national)? ";
            cin >> type_results;
            if (type_results == "local" || type_results == "national") {
                displayResults(type_results);
            }
            else {
                cout << "Invalid election type. Please enter 'local' or 'national'.\n";
            }
            system("pause");
            break;
        }
        case 6:
            displayHelp();
            system("pause");
            break;
        case 7:
            cout << "Exiting application...\n";
            return 0;
        default:
            cout << "Invalid choice, please try again.\n";
            system("pause");
            break;
        }
    }
    return 0;
}
