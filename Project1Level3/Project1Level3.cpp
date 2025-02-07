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
	bool isMarkedToDelete = false;
	bool isMarkedToUpdate = false;
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

string convertRecordToLine(stClient clientInfo, string delim) {
	string clientRecord = "";
	clientRecord += clientInfo.AccountNumber + delim;
	clientRecord += clientInfo.PinCode + delim;
	clientRecord += clientInfo.Name + delim;
	clientRecord += clientInfo.Phone + delim;
	clientRecord += to_string(clientInfo.AccountBalance);
	return clientRecord;
}


string readAccountNumber() {
	string AccountNumber;
	cout << "Please enter the account number : ";
	cin >> AccountNumber;
	return AccountNumber;

}

bool clientExistsByAccountNumber(string AccountNumber, string
	FileName)
{
	vector <stClient> vClients;
	fstream MyFile;
	MyFile.open(FileName, ios::in);//read Mode
	if (MyFile.is_open())
	{
		string Line;
		stClient Client;
		while (getline(MyFile, Line))
		{
			Client = convertLineToRecord(Line, "#//#");
			if (Client.AccountNumber == AccountNumber)
			{
				MyFile.close();
				return true;
			}
			vClients.push_back(Client);
		}
		MyFile.close();
	}
	return false;
}




stClient readNewClient() {
	stClient client;
	cout << "Please enter your account number ?";
	cin >> client.AccountNumber;
	while (clientExistsByAccountNumber(client.AccountNumber, fileN) == true) {
		cout << "\nClient with [" << client.AccountNumber << "]already exists, Enter another Account Number ? ";
		getline(cin >> ws, client.AccountNumber);
	}
	cout << "Please enter your pin code ? ";
	cin >> client.PinCode;
	cin.ignore();
	cout << "Please enter your name ? ";
	getline(cin, client.Name);
	cout << "Please enter your phone number ? ";
	cin >> client.Phone;
	cout << "Please enter your account balance ? ";
	cin >> client.AccountBalance;
	return client;
}


void addClientToFile(string fileName, string record) {
	fstream MyFile;
	MyFile.open(fileName, ios::app);
	if (MyFile.is_open()) {
		MyFile << record << endl;
		MyFile.close();
	}
}

void AddNewClient()
{
	stClient clientInfo;
	clientInfo = readNewClient();
	addClientToFile(fileN, convertRecordToLine(clientInfo, "#//#"));
}

void addClients() {
	char AddMore = 'Y';
	do
	{
		system("cls");
		cout << "Adding New Client:\n\n";
		AddNewClient();
		cout << "\nClient Added Successfully, do you want to add more clients ? Y / N ? ";
		cin >> AddMore;
	} while (toupper(AddMore) == 'Y');
}

vector<stClient> loadFileContentToVector(string fileName) {
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

bool isAccountNumberExists(vector<stClient> vStClient, stClient& clientToFind, string accountNumber) {
	for (stClient& client : vStClient) {
		if (client.AccountNumber == accountNumber) {
			clientToFind = client;
			return true;
		}
	}
	return false;
}

void printClientRecord(stClient client) {
	cout << "\nThe following are the client details: \n";
	cout << "Account Number    : " << client.AccountNumber << endl;
	cout << "Pin Code          : " << client.PinCode << endl;
	cout << "Name              : " << client.Name << endl;
	cout << "Phone             : " << client.Phone << endl;
	cout << "Account Balance   : " << client.AccountBalance << endl;
}


string convertRecordToStringWithDelim(stClient clientInfo, string delim) {
	string clientRecord = "";
	clientRecord += clientInfo.AccountNumber + delim;
	clientRecord += clientInfo.PinCode + delim;
	clientRecord += clientInfo.Name + delim;
	clientRecord += clientInfo.Phone + delim;
	clientRecord += to_string(clientInfo.AccountBalance);
	return clientRecord;
}

void markClientForDeleteByAccountNum(vector<stClient>& vStClient, string accountNumber) {
	for (stClient& client : vStClient) {
		if (client.AccountNumber == accountNumber) {
			client.isMarkedToDelete = true;
		}
	}
}

void saveVectorToFile(string fileName, vector<stClient> vStClient) {
	fstream myFile;
	string line;
	myFile.open(fileName, ios::out); //overwrite
	if (myFile.is_open()) {
		for (stClient& client : vStClient) {
			if (client.isMarkedToDelete == false) {
				line = convertRecordToStringWithDelim(client, "#//#");
				myFile << line << endl;
			}
		}
		myFile.close();
	}
}

void deleteClientFromFile(vector<stClient>  &vStClient, string accountNumber) {
	stClient client;
	char answer = 'n';
	if (isAccountNumberExists(vStClient, client, accountNumber) == true) {
		printClientRecord(client);
		cout << endl << "Are you sure you want to delete this client ? (y/n)\n";
		cin >> answer;
		if (answer == 'y' || answer == 'Y') {
			markClientForDeleteByAccountNum(vStClient, accountNumber);
			saveVectorToFile(fileN, vStClient);
			//refresh clients
			vStClient = loadFileContentToVector(fileN);
			cout << "Client Deleted Successfully.\n";
		}
	}
	else {
		cout << "\nClient with account number (" << accountNumber << ") not found !\n";
	}
}

void markClientForUpdateByAccountNum(vector<stClient>& vStClient, string accountNumber) {
	for (stClient& client : vStClient) {
		if (client.AccountNumber == accountNumber) {
			client.isMarkedToUpdate = true;
		}
	}
}
void getClientUpdatedInfo(vector<stClient>& vStClient) {

	for (stClient& client : vStClient) {
		if (client.isMarkedToUpdate == true) {
			cout << "\nPlease enter the new pin code ?";
			cin >> client.PinCode;
			cout << "Please enter the new name ?";
			cin.ignore(); //cin >> leaves a newline character (\n) in the input buffer, which causes the subsequent getline(cin, client.Name) to read an empty string.
			getline(cin, client.Name);
			cout << "Please enter the new phone number ?";
			cin >> client.Phone;
			cout << "Please enter the new account balance ?";
			cin >> client.AccountBalance;
		}
	}

}


void updateClientInfo(vector<stClient>& vStClient, string accountNumber) {
	stClient client;
	char answer = 'n';

	if (isAccountNumberExists(vStClient, client, accountNumber) == true) {
		printClientRecord(client);
		cout << endl << "Are you sure you want to update this client ? (y/n)\n";
		cin >> answer;
		if (answer == 'y' || answer == 'Y') {
			markClientForUpdateByAccountNum(vStClient, accountNumber);
			getClientUpdatedInfo(vStClient);
			saveVectorToFile(fileN, vStClient);
			//refresh clients
			/*vStClient = loadFileContentToVector(fileName);*/
			cout << "\nClient Updated Successfully.\n";
		}
	}
	else {
		cout << "\nClient with account number (" << accountNumber << ") not found !\n";
	}
}

int main()
{
    enMenu choice;
    
    do {
		system("cls");
        printMenu();
        vector<stClient> vStClient = readFileContent(fileN);
		string accountNumber;
		stClient client;
        choice = convertChoiceToEnum(readMenuChoice());
		switch (choice) {
		case enMenu::showClients:
			system("cls");
			printTableHeader(vStClient.size());
			printClientData(vStClient);
			system("pause");
			break;
		case enMenu::addClient:
			system("cls");
			cout << "====================================================\n";
			cout << "\t\tAdd Client(s) Screen\n";
			cout << "====================================================\n\n";
			addClients();
			system("pause");
			break;
		case enMenu::deleteClient:
			system("cls");
			cout << "====================================================\n";
			cout << "\t\tUpdate Client(s) Screen\n";
			cout << "====================================================\n\n";
			accountNumber = readAccountNumber();
			deleteClientFromFile(vStClient, accountNumber);
		case enMenu::updateClient:
			system("cls");
			cout << "====================================================\n";
			cout << "\t\tUpdate Client Info Screen\n";
			cout << "====================================================\n\n";
			accountNumber = readAccountNumber();
			updateClientInfo(vStClient, accountNumber);

		}
    } while (choice != enMenu::exitMenu);
   
}


