#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <vector>
#include <cstdlib>
using namespace std;


const string fileN = "clients.txt";

struct stClient
{
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	double AccountBalance = 0;
};

enum enMenu { showClients = 1, addClient = 2, deleteClient = 3, updateClient = 4, findClient = 5, exitMenu = 6};


void printMenu() {
    cout << "=========================================================\n";
    cout << "\t\tMain Menu Screen\n";
    cout << "=========================================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Exit.\n";
    cout << "=========================================================\n";
}

short readMenuChoice() {
    short choice;
    do {
        cout << "Please select an option from the menu above (1-6): ";
        cin >> choice;
    } while (choice > 6 || choice < 1);
    return choice;
}

enMenu convertChoiceToEnum(short choice) {
    switch (choice) {
    case 1:
        return enMenu::showClients;
    case 2:
        return enMenu::addClient;
    case 3: 
        return enMenu::deleteClient;
    case 4:
        return enMenu::updateClient;
    case 5:
        return enMenu::findClient;
    case 6:
        return enMenu::exitMenu;
    }
}

vector<string> splitString(string& clientRecord, string delim) {
	string word = "";
	vector<string> vString;
	short pos;
	while ((pos = clientRecord.find(delim)) != std::string::npos) {
		word = clientRecord.substr(0, pos);
		vString.push_back(word);
		clientRecord.erase(0, pos + delim.length());
	}
	if (clientRecord != "") {
		vString.push_back(clientRecord);
	}
	return vString;

}


stClient convertLineToRecord(string line, string delim) {
	stClient client;
	vector<string> vString;
	vString = splitString(line, "#//#");
	client.AccountNumber = vString[0];
	client.PinCode = vString[1];
	client.Name = vString[2];
	client.Phone = vString[3];
	client.AccountBalance = stod(vString[4]);
	return client;
}


vector<stClient> readFileContent(string fileName) {
	fstream myFile;
	short counter = 0;
	stClient client;
	vector<stClient> vStClient;
	myFile.open(fileName, ios::in);
	if (myFile.is_open()) {
		string line;
		while (getline(myFile, line)) {
			client = convertLineToRecord(line, "#//#");
			vStClient.push_back(client);
			counter++;
		}
		myFile.close();
	}
	return vStClient;
}

void printTableHeader(short counter) {
	char col1[] = "Account Number";
	char col2[] = "Pin Code";
	char col3[] = "Client Name";
	char col4[] = "Phone";
	char col5[] = "Balance";

	cout << "\t\t\t\t\t Client List (" << counter << ") Client(s).\n";
	cout << "_________________________________________________________________________________________________\n\n";

	// Use 'left' to align text to the left in each column
	cout << "| " << left << setw(16) << col1
		<< "| " << left << setw(11) << col2
		<< "| " << left << setw(40) << col3
		<< "| " << left << setw(12) << col4
		<< "| " << left << setw(10) << col5 << endl;

	cout << "_________________________________________________________________________________________________\n\n";
}


void printClientData(vector<stClient> vStClient) {
	for (const stClient& data : vStClient) {
		// Apply 'left' to align text to the left in each column
		cout << "| " << left << setw(16) << data.AccountNumber
			<< "| " << left << setw(11) << data.PinCode
			<< "| " << left << setw(40) << data.Name
			<< "| " << left << setw(12) << data.Phone
			<< "| " << left << setw(10) << data.AccountBalance << endl;
	}
	cout << "_________________________________________________________________________________________________\n";
}

int main()
{
    enMenu choice;
    
    do {
		system("cls");
        printMenu();
        vector<stClient> vStClient = readFileContent(fileN);
        choice = convertChoiceToEnum(readMenuChoice());
        switch (choice) {
            case enMenu::showClients:
				system("cls");
                printTableHeader(vStClient.size());
                printClientData(vStClient);
                system("pause");
                break;
                }
    } while (choice != enMenu::exitMenu);
   
}


