#include <iostream>
#include <fstream>   // for file I/O
#include <string>
#include <cstring>
#include <cstdlib>
#include <iomanip>    // for setw()
using namespace std;
//nothing has been changed
//=========================================================     Person class started     ========================================================================//
class person
{
protected:
    string name;
    string cnic;
    string voteto;
public:
    person()
    {
        name = "empty";
        cnic = "null";
        voteto = "none";
    }
    person(string name, string c, string v)
    {
        this->name = name;
        setcnic(c);
        voteto = v;
    }
    void setvote(string v) {
        voteto = v;
    }
    void setname(string n) {
        bool flag2 = true;
        for (char ch : n) {
            if (!isalpha(ch)) {
                flag2 = false;
                break;
            }
        }
        if (flag2 && n.length() <= 25) {
            name = n;
            cout << "\nYour NAME is saved.\n" << endl;
        }
        else {
            cout << "Invalid name!\nYour name should be in alphabets, and it should be less then 25 characters." << endl;
            cout << "\nRe-enter your name.\n\n" << endl;

        }

    }
    void setcnic(string c) {

        cnic = c;
        cout << "CNIC has entered " << endl;

    }

private:
    bool alldigits(string str) {
        for (char ch : str) {
            if (!isdigit(ch))
                return false;
        }
        return true;
    }
public:
    string getcnic() {
        return cnic;
    }
    string getname() {
        return name;
    }
    string getvoteto() {
        return voteto;
    }

};


//==============================================================     Candidate class started     ===================================================================//

class candidate :public person {
private:
    string party;
    string symbol;
    int votecount;
public:
    candidate() {
        votecount = 0;
    }
    void setparty(string p) {
        party = p;

    }
    void setsymbol(string s) {
        symbol = s;
    }
    void addvote() {
        votecount++;
    }
    int getvotecount() {
        return votecount;
    }
    string getsymbol() {
        return symbol;
    }
    string getparty() {
        return party;
    }



};



//========================================================     Admin class started     ========================================================================// 


class admin {
private:
    candidate** candidates;
    int maxcandidates;
    int count;
public:
    admin(int size) {
        maxcandidates = size;
        candidates = new candidate * [maxcandidates];
        count = 0;
    }
    void addcandidate() {
        if (count >= maxcandidates) {
            cout << "candidate limit reached\n";
            return;
        }
        string name, cnic, party, symbol;
        cout << "Enter candidate name:";cin >> name;
        cout << "Enter cnic:";cin >> cnic;
        cout << "Enter party name:";cin >> party;
        cout << "Enter party symbol:";cin >> symbol;
        candidate* c = new candidate();
        c->setname(name);
        c->setcnic(cnic);
        c->setparty(party);
        c->setsymbol(symbol);
        candidates[count++] = c;
        cout << "candidate added successfully\n";
    }
    void showcandidates() {
        cout << "\nList of candidates:\n";
        for (int i = 0;i < count;i++) {
            cout << i + 1 << "." << candidates[i]->getname() << "|";
            cout << candidates[i]->getparty();
            cout << "| Symbol:" << candidates[i]->getsymbol();
            cout << candidates[i]->getvotecount() << endl;

        }
    }
    void vote(person& voter) {
        if (count == 0) {
            cout << "NO candidates available to vote\n";
            return;

        }
        cout << "\n Available candidates\n";
        for (int i = 0;i < count;i++) {
            cout << i + 1 << "." << candidates[i]->getname() << "  " << candidates[i]->getparty() << endl;

        }
        int choice;
        cout << "Enter the number of the candidates you want to vote for :";
        cin >> choice;
        if (choice<1 || choice>count) {
            cout << "invalid candidates choice\n";
            return;

        }

        candidate* selected = candidates[choice - 1];
        selected->addvote();
        voter.setvote(selected->getname());
        cout << "Vote casted to:" << selected->getname() << endl;


    }
    ~admin() {
        for (int i = 0;i < count;i++) {

            delete candidates[i];
        }

        delete[]candidates;
    }

};



bool validateCNIC(const string& cnic)
{

    if (cnic.length() != 13)
    {
        cout << "Error: CNIC should be exactly 13 digits long!" << endl;
        return false;
    }


    for (char ch : cnic)
    {
        if (!isdigit(ch))
        {
            cout << " Error: CNIC should only contain digits (0-9)!" << endl;
            return false;
        }
    }

    // Agar dono test pass ho gaye to...
    cout << " CNIC verified." << endl;
    return true;
}




//==================================================          File writing info of voter           ==================================================//

void saveVoterInfo(person& p) {
    ofstream outFile("voters.txt", ios::app);  // Append mode
    if (outFile.is_open()) {
        outFile << p.getname() << " " << p.getcnic() << " " << p.getvoteto() << endl;
        outFile.close();
        cout << "\nVoter data saved successfully!\n";
    }
    else {
        cout << "\nError: Cannot open file to save voter info.\n";
    }
}
//=================================================           Search Voter function for info         ==================================================//

void searchVoterByCNIC(const string& searchCnic) {
    ifstream inFile("voters.txt");
    if (!inFile.is_open()) {
        cout << "\nError opening this file\n";
        return;
    }

    string name, cnic, voteto;
    bool found = false;

    while (inFile >> name >> cnic >> voteto) {
        if (cnic == searchCnic) {
            cout << "\nRecord Found:\n";
            cout << "------------------------------------------\n";
            cout << "Name     : " << name << endl;
            cout << "CNIC     : " << cnic << endl;
            cout << "Voted To : " << voteto << endl;
            cout << "------------------------------------------\n";
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "\nNo voter found with CNIC: " << searchCnic << endl;
    }

    inFile.close();
}

int main()
{
    int choice;
    bool flag1 = true;
    string name, cnic;
    person p;
    while (flag1) {
        cout << "\n========================================================================================================================";
        cout << "\n\t\t\t\t\t\tELECTION MANAGEMENT SYSTEM";
        cout << "\n========================================================================================================================";
        cout << "--------------------------------------------          (    MENU    )          ------------------------------------------\n\n";
        cout << "\n\n\t\t\t\t\t\t\t1. Admin\n\t\t\t\t\t\t\t2. Caste Vote\n\t\t\t\t\t\t\t3. Voter Info\n\t\t\t\t\t\t\t4. Exit\n\n\n";
        cout << "\n------------------------------------------------------------------------------------------------------------------------\n";
        cout << "Enter your choice: ";
        cin >> choice;


        cin.ignore();
        cout << "\n========================================================================================================================";
        switch (choice)
        {
        case 1:
        {
            int pass;
            system("cls");
            cout << "\n========================================================================================================================";
            cout << "\n\t\t\t\t\t\tWELCOME TO ADMIN PANEL";
            cout << "\n========================================================================================================================";
            cout << "Enter your password:";
            cin >> pass;
            if (pass == 1234) {
                cout << "\n\n\t\t\t\t\t\t\tWELCOME ADMIN";

                while (true) {
                    cout << "\n1. Add Candidate\n2. Show Candidates\n3. Vote\n4. Exit\n";
                    cout << "Enter your choice: ";
                    cin >> choice;
                    switch (choice) {
                    case 1:

                        cout << "1";
                        break;
                    case 2:
                        cout << "2";

                        break;
                    case 3:
                        cout << "3";

                        break;
                    case 4:
                        cout << "4";

                    default:
                        cout << "Invalid choice!.\nPlease Retry again.\n";
                    }
                }
            }
            else {
                cout << "\nWrong password!.\nPlease Retry again.\n";
            }
            break;
        }
        case 2:
        {
            cout << "\n========================================================================================================================";
            cout << "\n\t\t\t\t\t\t\tWELCOME TO Voting section";
            cout << "\n========================================================================================================================";
            cout << "Enter your name:;h ";
            getline(cin, name);
            p.setname(name);
            cout << "Your name is saved as: " << p.getname();
            cout << "\nEnter CNIC :";
            getline(cin, cnic);  // Use getline to read the CNIC
            while (true)
            {
                bool valid = true;

                // Check if CNIC has exactly 13 characters
                if (cnic.length() != 13)  //.length() is a function that checks the length
                {
                    valid = false;
                }

                // Check if all characters in CNIC are digits
                for (char ch : cnic)           // Assigning only one value of string at a time
                {
                    if (!isdigit(ch))
                    {  // If any character is not a digit
                        valid = false;
                        break;
                    }
                }

                if (valid)
                {
                    p.setcnic(cnic);
                    break;  // Exit loop if CNIC is valid
                }
                else
                {
                    cout << "Invalid CNIC. CNIC should contain exactly 13 digits: ";
                    getline(cin, cnic);  // Re-enter CNIC if invalid
                }
            }


            saveVoterInfo(p);
            cout << "\n\n\nYou can check your status in the voter info section\n\n\n";

            break;
        }
        case 3:
        {
            cout << "\n========================================================================================================================";
            cout << "\n\t\t\t\t\t\t\tWELCOME TO Voter info section";
            cout << "\n========================================================================================================================";
            string searchCnic;
            cin.ignore();
            cout << "\nEnter your CNIC to search: ";
            cin >> searchCnic;

            if (validateCNIC(searchCnic)) {   // Validate CNIC length 13 digits
                searchVoterByCNIC(searchCnic);   //  Call Search function
            }
            else {
                cout << "Invalid CNIC format! Must be 13 numeric digits only.\n";
            }
            break;
        }
        case 4:
            exit(0);
        default:
            cout << "Invalid choice!.\nPlease Retry again.\n";
        }
    }

}