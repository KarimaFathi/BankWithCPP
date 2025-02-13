#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <vector>
#include <cstdlib>
using namespace std;

void showMainMenu();
void showTransactionsMenu();

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

enum enMenu { showClients = 1, addClient = 2, deleteClient = 3, updateClient = 4, findClient = 5, transactions = 6, exitMenu = 7};
enum enTransactionsMenu { deposite = 1, withdraw = 2, totalBalance = 3, mainMenu = 4 };



short readMenuChoice(string message, short min, short max) {
    short choice;
    do {
        cout << message;
        cin >> choice;
    } while (choice > max || choice < min);
    return choice;
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


void findClientByAccountNumber(string accountNumber, vector<stClient> vStClient) {
	stClient client;
	if (isAccountNumberExists(vStClient, client, accountNumber)) {
		printClientRecord(client);
	}
	else {
		cout << "\nClient with account number (" << accountNumber << ") not found !\n";
	}

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

void ShowAllClientsScreen() {
	vector<stClient> vStClient = readFileContent(fileN);
	char col1[] = "Account Number";
	char col2[] = "Pin Code";
	char col3[] = "Client Name";
	char col4[] = "Phone";
	char col5[] = "Balance";

	cout << "\t\t\t\t\t Client List (" << vStClient.size() << ") Client(s).\n";
	cout << "___________________________________________________________________________________________________\n\n";

	// Use 'left' to align text to the left in each column
	cout << "| " << left << setw(16) << col1
		<< "| " << left << setw(11) << col2
		<< "| " << left << setw(40) << col3
		<< "| " << left << setw(15) << col4
		<< "| " << left << setw(10) << col5 << endl;

	cout << "___________________________________________________________________________________________________\n\n";
	for (const stClient& data : vStClient) {
		// Apply 'left' to align text to the left in each column
		cout << "| " << left << setw(16) << data.AccountNumber
			<< "| " << left << setw(11) << data.PinCode
			<< "| " << left << setw(40) << data.Name
			<< "| " << left << setw(15) << data.Phone
			<< "| " << left << setw(10) << data.AccountBalance << endl;
	}
	cout << "___________________________________________________________________________________________________\n";
}

void goBackToMainMenu()
{
	cout << "\n\nPress any key to go back to Main Menu...";
	system("pause>0");
	system("cls");
	showMainMenu();
}

void showAddNewClientsScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tAdd New Clients Screen";
	cout << "\n-----------------------------------\n";
	addClients();
}


void showDeleteClientScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tDelete Client Screen";
	cout << "\n-----------------------------------\n";
	vector<stClient> vStClient = readFileContent(fileN);
	string accountNumber = readAccountNumber();
	deleteClientFromFile(vStClient, accountNumber);
}

void showUpdateClientScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tUpdate Client Info Screen";
	cout << "\n-----------------------------------\n";
	vector<stClient> vStClient = readFileContent(fileN);
	string accountNumber = readAccountNumber();
	updateClientInfo(vStClient, accountNumber);
}

void ShowFindClientScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tFind Client Screen";
	cout << "\n-----------------------------------\n";
	vector<stClient> vStClient = readFileContent(fileN);
	string accountNumber = readAccountNumber();
	findClientByAccountNumber(accountNumber, vStClient);
}

void showEndScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tProgram Ends :-)";
	cout << "\n-----------------------------------\n";
}

void getClientDepositInfo(vector<stClient>& vStClient, string accountNumber, stClient &client) {
	int depositAmount;
	for (stClient& clientInVec : vStClient) {
		if (clientInVec.AccountNumber == accountNumber) {
			cout << "\nPlease enter deposit amount ?\n";
			cin >> depositAmount;
			clientInVec.AccountBalance += depositAmount;
			client = clientInVec;
		
		}
	}
}

void showDepositScreen() {
	cout << "\n-----------------------------------\n";
	cout << "\tDeposit Screen";
	cout << "\n-----------------------------------\n";
	string accountNumber = readAccountNumber();
	vector<stClient> vStClient = readFileContent(fileN);
	stClient client;
	int depositAmount;
	char answer;
	while (isAccountNumberExists(vStClient, client, accountNumber) == false) {
		cout << "\nClient with [" << accountNumber << "] does not exist. \n";
		accountNumber = readAccountNumber();
	}
	printClientRecord(client);
	getClientDepositInfo(vStClient, accountNumber, client);
	cout << "Are you sure you want to perform this transaction? (y/n) ";
	cin >> answer;
	if (answer == 'y' || answer == 'Y') {
		saveVectorToFile(fileN, vStClient);
		cout << "Done successfully, the new balance is : " << client.AccountBalance ;
	}
	else {
		cout << "No changes were made\n";
	}
}

void goBackToTransactionMenu() {
	cout << "\n\nPress any key to go back to Main Menu...";
	system("pause>0");
	system("cls");
	showTransactionsMenu();
}


void showBalanceScreen() {
	vector<stClient> vStClient = readFileContent(fileN);
	char col1[] = "Account Number";
	char col2[] = "Client Name";
	char col3[] = "Balance";
	int balanceSum = 0;

	cout << "\t\t\t\t\t Client List (" << vStClient.size() << ") Client(s).\n";
	cout << "___________________________________________________________________________________________________\n\n";

	// Use 'left' to align text to the left in each column
	cout << "| " << left << setw(16) << col1
		<< "| " << left << setw(40) << col2
		<< "| " << left << setw(10) << col3 << endl;

	cout << "___________________________________________________________________________________________________\n\n";
	for (const stClient& data : vStClient) {
		// Apply 'left' to align text to the left in each column
		cout << "| " << left << setw(16) << data.AccountNumber
			<< "| " << left << setw(40) << data.Name
			<< "| " << left << setw(10) << data.AccountBalance << endl;
		balanceSum += data.AccountBalance;
	}
	cout << "___________________________________________________________________________________________________\n\n";
	cout << "\t\t\t\t\t\t Total Balances = " << balanceSum;
}

void performTransactionsMenuOptions(enTransactionsMenu choice) {
	switch (choice) {
	case enTransactionsMenu::deposite:
		system("cls");
		showDepositScreen();
		goBackToTransactionMenu();
		break;
	case enTransactionsMenu::totalBalance:
		system("cls");
		showBalanceScreen();
		goBackToTransactionMenu();
		break;
	}
}

void showTransactionsMenu() {
	cout << "=========================================================\n";
	cout << "\t\tTransactions Menu Screen\n";
	cout << "=========================================================\n";
	cout << "\t[1] Deposit.\n";
	cout << "\t[2] Withdraw.\n";
	cout << "\t[3] Total Balances.\n";
	cout << "\t[4] Main Menu.\n";
	cout << "=========================================================\n";
	performTransactionsMenuOptions((enTransactionsMenu)readMenuChoice("Please select an option from the menu above (1-4): ", 1, 4));
}

void perfomMainMenuOptions(enMenu choice) {
		switch (choice) {
		case enMenu::showClients:
			system("cls");
			ShowAllClientsScreen();
			goBackToMainMenu();
			break;
		case enMenu::addClient:
			system("cls");
			showAddNewClientsScreen();
			goBackToMainMenu();
			break;
		case enMenu::deleteClient:
			system("cls");
			showDeleteClientScreen();
			goBackToMainMenu();
			break;
		case enMenu::updateClient:
			system("cls");
			showUpdateClientScreen();
			goBackToMainMenu();
			break;
		case enMenu::findClient:
			system("cls");
			ShowFindClientScreen();
			goBackToMainMenu();
			break;
		case enMenu::transactions:
			system("cls");
			showTransactionsMenu();
			goBackToMainMenu();
			break;
		case enMenu::exitMenu:
			system("cls");
			showEndScreen();
			break;
		}
}


void showMainMenu() {
	cout << "=========================================================\n";
	cout << "\t\tMain Menu Screen\n";
	cout << "=========================================================\n";
	cout << "\t[1] Show Client List.\n";
	cout << "\t[2] Add New Client.\n";
	cout << "\t[3] Delete Client.\n";
	cout << "\t[4] Update Client Info.\n";
	cout << "\t[5] Find Client.\n";
	cout << "\t[6] Transactions.\n";
	cout << "\t[7] Exit.\n";
	cout << "=========================================================\n";
	perfomMainMenuOptions((enMenu)readMenuChoice("Please select an option from the menu above (1-7): ", 1, 7));
}

int main()
{
	showMainMenu();
	return 0;
}
