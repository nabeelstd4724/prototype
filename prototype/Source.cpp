#include <iostream>
#include<string>
#include<cctype>
#include<fstream>
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
	User(string u, string p) :username(u), password(p) {};
	void setusername(string u)
	{
		username = u;
	}
	void setuserpassword(string p) {
		password = p;
	}
	string getpassword() {
		return password;
	}
	string getusername() {
		return username;
	}
	bool login(string inputusername, string inputpassword) {
		return (username == inputusername && password == inputpassword);
	}

};
class Voter :public User {
private:
	string cnic;
	int age;
	string cast_vote_to;
public:
	Voter() {
		cnic = "";
		age = 0;
		cast_vote_to = "";
	}
	Voter(string u, string p, string c, int a) :User(u, p), cnic(c), age(a), cast_vote_to("") {};
	void castvote(string candidate_name) {
		cast_vote_to = candidate_name;
		cout << "You have successfully voted for candidate:  " << candidate_name << endl;
	}
	void setage(int a) {
		if (a < 18) {
			cout << "For register as a voter you must be older than 18 years";
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
			cout << "Cnic must be 13 digits only wihtout spaces";
		}
	}
	bool validatecnic(string c) {
		if (c.length() != 13)return false;
		for (char ch : c) {
			if (!isdigit(ch))return false;
		}

	}


	string votestatus() {
		if (cast_vote_to == "") {
			cout << "You have not voted yet";

		}
		else {
			cout << "You have already voted to" << cast_vote_to << endl;
		}



	}



};
class Candidate {
private:
	string 	candidate_name;
	string party_name;
	int vote_count;
	static int total_vote;
public:
	Candidate(string n, string p) :candidate_name(n), party_name(p), vote_count(0) {};
	void increment_vote() {
		vote_count++;
		total_vote++;
	}
	string getcandidatename() {
		return candidate_name;
	}
	string getpartyname() {
		return party_name;


	}
	int get_votereceived() {
		return vote_count;
	}
	static int get_totalvote() {
		return total_vote;
	}
	void display_candidateinfo() {
		cout << "Candidate name:     " << candidate_name << endl;
		cout << "Party affiliation:   " << party_name << endl;
		cout << " Vote received:         " << vote_count << endl;

	}





};
int Candidate::total_vote = 0;


void adminMenu();
void voterMenu();
void mainMenu() {
	int choice;
	do {
		cout << "\n===================================";
		cout << "\n    Election Management System";
		cout << "\n===================================";
		cout << "\n1. Admin";
		cout << "\n2. Voter";
		cout << "\n3. Exit";
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
			cout << "Exiting program...\n";
			break;
		default:
			cout << "Invalid choice! Please try again.\n";
		}
	} while (choice != 3);
}


void adminMenu() {
	int option;
	do {
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
			cout << "Add Candidate Logic Here...\n";
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
			cout << "Viewing election results...\n";
			break;
		case 6:
			cout << "Returning to Main Menu...\n";
			break;
		default:
			cout << "Invalid choice, try again.\n";
		}
	} while (option != 6);
}


void voterMenu() {
	int option;
	do {
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
			cout << "Viewing election results...\n";
			break;
		case 4:
			cout << "Returning to Main Menu...\n";
			break;
		default:
			cout << "Invalid choice, try again.\n";
		}
	} while (option != 4);
}
void registerUser() {
	ifstream infile("users.txt");
	string existingUsername, existingPassword;
	string newUsername, newPassword;

	cout << "Enter new username: ";
	cin >> newUsername;


	while (infile >> existingUsername >> existingPassword) {
		if (existingUsername == newUsername) {
			cout << "Username already registered! Please log in.\n";

			infile.close();
			return;
		}
	}
	infile.close();


	ofstream outfile("users.txt", ios::app);
	cout << "Enter password: ";
	cin >> newPassword;

	outfile << newUsername << " " << newPassword << endl;
	outfile.close();

	cout << "User registered successfully!\n";
}





int main() {
	int choice;
	cout << "Welcome to the Election Management System\n";
	cout << "1. Register New User\n2. Login\nEnter option: ";
	cin >> choice;

	if (choice == 1) {
		registerUser();
	}
	else if (choice == 2) {
		string inputUsername, inputPassword;
		cout << "Enter username: ";
		cin >> inputUsername;
		cout << "Enter password: ";
		cin >> inputPassword;

		ifstream infile("users.txt");
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
			cout << "Login successful!\n";
			mainMenu();
		}
		else {
			cout << "Login failed! Incorrect username or password.\n";
		}
	}
	else {
		cout << "Invalid choice, exiting...\n";
	}

	return 0;
}
