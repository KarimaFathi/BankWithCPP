#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <vector>
#include <cstdlib>
using namespace std;



void showMainMenu();
void showTransactionsMenu();
void showManageUsersMenu();

const string fileN = "clients.txt";
const string usersFile = "users.txt";

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


struct stUser {
	string userName;
	int password;
	int permission;
	bool isMarkedToDelete = false;
	bool isMarkedToUpdate = false;
};

stUser currentUser;

enum enMenu { showClients = 1, addClient = 2, deleteClient = 3, updateClient = 4, findClient = 5, transactions = 6, manageUsers = 7, exitMenu = 8};
enum enTransactionsMenu { deposite = 1, withdraw = 2, totalBalance = 3, mainMenu = 4 };
enum enManageUsersMenu { showUsers = 1, addUser = 2, deleteUser = 3, updateUser = 4, findUser = 5, exitManageUsers = 6};
enum enPermissions { allPermissions = -1, showAllClients = 1, addNewClient = 2, deleteClients = 4, updateClients = 8, findClients = 16, manageTransactions = 32, AdministerUsers = 64 };



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

void getClientWithdrawInfo(vector<stClient> &vStClient, string accountNumber, stClient &client) {
	int withdrawAmount;
	for (stClient& clientInVec : vStClient) {
		if (clientInVec.AccountNumber == accountNumber) {
			cout << "\nPlease enter withdraw amount ?\n";
			cin >> withdrawAmount;
		    while (withdrawAmount > clientInVec.AccountBalance) {
				cout << "Amount exceeds the balance, you can withdraw up to :" << clientInVec.AccountBalance;
				cout << "Please enter another amount ? ";
				cin >> withdrawAmount;
			} 
			clientInVec.AccountBalance -= withdrawAmount;
			client = clientInVec;
		}
	}
}

void showWithdrawScreen() {
	cout << "\n-----------------------------------\n";
	cout << "\twithdraw Screen";
	cout << "\n-----------------------------------\n";
	string accountNumber = readAccountNumber();
	vector<stClient> vStClient = readFileContent(fileN);
	stClient client;
	char answer;
	while (isAccountNumberExists(vStClient, client, accountNumber) == false) {
		cout << "\nClient with [" << accountNumber << "] does not exist. \n";
		accountNumber = readAccountNumber();
	}
	printClientRecord(client);
	getClientWithdrawInfo(vStClient, accountNumber, client);
	cout << "Are you sure you want to perform this transaction? (y/n) ";
	cin >> answer;
	if (answer == 'y' || answer == 'Y') {
		saveVectorToFile(fileN, vStClient);
		cout << "Done successfully, the new balance is : " << client.AccountBalance;
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
	vector<stClient> vStClient = readFileContent(usersFile);
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
	case enTransactionsMenu::withdraw:
		system("cls");
		showWithdrawScreen();
		goBackToTransactionMenu();
		break;
	case enTransactionsMenu::mainMenu:
		system("cls");
		showMainMenu();
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

stUser convertUserLineToRecord(string line, string delim) {
	stUser user;
	vector<string> vString;
	vString = splitString(line, "#//#");
	user.userName = vString[0];
	user.password = stoi(vString[1]);
	user.permission = stoi(vString[2]);
	return user;
}

vector<stUser>  readUsersFileContent(string fileName) {

	fstream myFile;
	short counter = 0;
	stUser user;
	vector<stUser> vStUser;
	myFile.open(fileName, ios::in);
	if (myFile.is_open()) {
		string line;
		while (getline(myFile, line)) {
			user = convertUserLineToRecord(line, "#//#");
			vStUser.push_back(user);
			counter++;
		}
		myFile.close();
	}
	return vStUser;

}

string readUsername() {
	string username;
	cout << "\nEnter the username : ";
	cin >> username;
	return username;
}

int readPassword() {
	int password;
	cout << "\nEnter the password : ";
	cin >> password;
	return password;
}

bool isUserExists(vector<stUser> vStUsers, string username, int password) {
	for (stUser& user : vStUsers) {
		if (user.password == password && user.userName == username) {
			currentUser = user;
			return true;
		}
	}
	return false;
}

void showLoginScreen() {
	system("cls");
	cout << "\n-----------------------------------\n";
	cout << "\tLogin Screen";
	cout << "\n-----------------------------------\n";
}

void findUserByCredentials(string username, int password, vector<stUser> vStUsers) {
	stUser currentUser;
	while (isUserExists(vStUsers, username, password) == false) {
		showLoginScreen();
		cout << "\nInvalid Username/Password";
		username = readUsername();
		password = readPassword();
	}
	system("cls");
	showMainMenu();
}


void ShowAllUsersScreen() {
	vector<stUser> vStUsers = readUsersFileContent(usersFile);
	char col1[] = "Username";
	char col2[] = "Password";
	char col3[] = "Permissions";

	cout << "\t\t\t\t\t Users List (" << vStUsers.size() << ") User(s).\n";
	cout << "___________________________________________________________________________________________________\n\n";

	// Use 'left' to align text to the left in each column
	cout << "| " << left << setw(20) << col1
		<< "| " << left << setw(15) << col2
		<< "| " << left << setw(40) << col3 << endl;

	cout << "___________________________________________________________________________________________________\n\n";
	for (const stUser& data : vStUsers) {
		// Apply 'left' to align text to the left in each column
		cout << "| " << left << setw(20) << data.userName
			<< "| " << left << setw(15) << data.password
			<< "| " << left << setw(40) << data.permission << endl;
	}
	cout << "___________________________________________________________________________________________________\n";
}


void goBackToManageUsersMenu() {
	cout << "\n\nPress any key to go back to Main Menu...";
	system("pause>0");
	system("cls");
	showManageUsersMenu();
}

string convertUserRecordToLine(stUser userInfo, string delim) {
	string userRecord = "";
	userRecord += userInfo.userName + delim;
	userRecord += to_string(userInfo.password) + delim;
	userRecord += to_string(userInfo.permission);
	return userRecord;
}

void addUserToFile(string fileName, string record) {
	fstream MyFile;
	MyFile.open(fileName, ios::app);
	if (MyFile.is_open()) {
		MyFile << record << endl;
		MyFile.close();
	}
}

stUser readNewUser() {
	stUser user;
	short userPermission = 0;
	char answer;
	vector<stUser> vStUsers = readUsersFileContent(usersFile);
	cout << "Enter the username :";
	cin >> user.userName;
	cout << "Enter the password :";
	cin >> user.password;
	while (isUserExists(vStUsers, user.userName, user.password)) {
		cout << "\nUser with [" << user.userName << "]already exists, Enter another username ? ";
		getline(cin >> ws, user.userName);
		cout << "Enter the password : ";
		cin >> user.password;
	}
	cout << "Do you want to give full access ? ";
	cin >> answer;
	if (tolower(answer) == 'y') {
		user.permission = allPermissions;
		return user;
	}
	else {
		cout << "Do you want to give access to : \n";
		cout << "Show clients list ? (y/n)";
		cin >> answer;
		if (tolower(answer) == 'y') {
			userPermission += showAllClients;
		}
		cout << "Add New Client? (y/n)";
		cin >> answer;
		if (tolower(answer) == 'y') {
			userPermission += addNewClient;
		}
		cout << "Delete Client? (y/n)";
		cin >> answer;
		if (tolower(answer) == 'y') {
			userPermission += deleteClients;
		}
		cout << "Update Client? (y/n)";
		cin >> answer;
		if (tolower(answer) == 'y') {
			userPermission += updateClients;
		}
		cout << "Find Client? (y/n)";
		cin >> answer;
		if (tolower(answer) == 'y') {
			userPermission += findClients;
		}
		cout << "Manage transactions? (y/n)";
		cin >> answer;
		if (tolower(answer) == 'y') {
			userPermission += manageTransactions;
		}
		cout << "Manage Users? (y/n)";
		cin >> answer;
		if (tolower(answer) == 'y') {
			userPermission += AdministerUsers;
		}
	}
	user.permission = userPermission;
	return user;
}

void AddNewUser()
{
	stUser userInfo;
	userInfo = readNewUser();
	addUserToFile(usersFile, convertUserRecordToLine(userInfo, "#//#"));
}

void addUsers() {
	char AddMore = 'Y';
	do
	{
		cout << "Adding New User:\n\n";
		AddNewUser();
		cout << "\nUser Added Successfully, do you want to add more users ? Y / N ? ";
		cin >> AddMore;
	} while (toupper(AddMore) == 'Y');
}

void ShowAddNewUsersScreen() {
	cout << "\n-----------------------------------\n";
	cout << "\tAdd New Users Screen";
	cout << "\n-----------------------------------\n";
	addUsers();
}


bool isUserNameExists(vector<stUser> vStUsers, stUser& userToFind, string username) {
	for (stUser& user : vStUsers) {
		if (user.userName == username) {
			userToFind = user;
			return true;
		}
	}
	return false;
}

void printUserRecord(stUser user) {
	cout << "\nThe following are the user details: \n";
	cout << "Username    : " << user.userName<< endl;
	cout << "Password    : " << user.password << endl;
	cout << "Permission  : " << user.permission << endl;
}


void markUserForDeleteByUsername(vector<stUser>& vStUsers, string username) {
	for (stUser& user : vStUsers) {
		if (user.userName == username) {
			user.isMarkedToDelete = true;
		}
	}
}


void saveUsersVectorToFile(string fileName, vector<stUser> vStUsers) {
	fstream myFile;
	string line;
	myFile.open(fileName, ios::out); //overwrite
	if (myFile.is_open()) {
		for (stUser& user : vStUsers) {
			if (user.isMarkedToDelete == false) {
				line = convertUserRecordToLine(user, "#//#");
				myFile << line << endl;
			}
		}
		myFile.close();
	}
}

void deleteUserFromFile(vector<stUser> &vStUsers, string username) {
	stUser user;
	char answer = 'n';
	if (username == "admin") {
		cout << "\n\nYou cannot Delete This User.";
	}
	else if (isUserNameExists(vStUsers, user, username) == true && username != "admin") {
		printUserRecord(user);
		cout << endl << "Are you sure you want to delete this user ? (y/n)\n";
		cin >> answer;
		if ((answer == 'y' || answer == 'Y')) {
			markUserForDeleteByUsername(vStUsers, username);
			saveUsersVectorToFile(usersFile, vStUsers);
			cout << "User Deleted Successfully.\n";
		}
	}
	else {
		cout << "\nUser with username (" << username << ") not found !\n";
	}
}

void ShowDeleteNewUsersScreen() {
	cout << "\n-----------------------------------\n";
	cout << "\tDelete User Screen";
	cout << "\n-----------------------------------\n";
	vector<stUser> vStUsers = readUsersFileContent(usersFile);
	string username = readUsername();
	deleteUserFromFile(vStUsers, username);
}

void markUserForUpdateByUsername(vector<stUser>& vStUsers, string username) {
	for (stUser& user : vStUsers) {
		if (user.userName == username) {
			user.isMarkedToUpdate = true;
		}
	}
}

void getUserUpdatedInfo(vector<stUser> &vStUsers) {
	short userPermission = 0;
	char answer;
		for (stUser& user : vStUsers) {
			if (user.isMarkedToUpdate == true) {
				cout << "Enter the password :";
				cin >> user.password;
				cout << "Do you want to give full access ? ";
				cin >> answer;
				if (tolower(answer) == 'y') {
					user.permission = allPermissions;
				}
				else {
					cout << "Do you want to give access to : \n";
					cout << "Show clients list ? (y/n)";
					cin >> answer;
					if (tolower(answer) == 'y') {
						userPermission += showAllClients;
					}
					cout << "Add New Client? (y/n)";
					cin >> answer;
					if (tolower(answer) == 'y') {
						userPermission += addNewClient;
					}
					cout << "Delete Client? (y/n)";
					cin >> answer;
					if (tolower(answer) == 'y') {
						userPermission += deleteClients;
					}
					cout << "Update Client? (y/n)";
					cin >> answer;
					if (tolower(answer) == 'y') {
						userPermission += updateClients;
					}
					cout << "Find Client? (y/n)";
					cin >> answer;
					if (tolower(answer) == 'y') {
						userPermission += findClients;
					}
					cout << "Manage transactions? (y/n)";
					cin >> answer;
					if (tolower(answer) == 'y') {
						userPermission += manageTransactions;
					}
					cout << "Manage Users? (y/n)";
					cin >> answer;
					if (tolower(answer) == 'y') {
						userPermission += AdministerUsers;
					}
				}
				user.permission = userPermission;
			}
		}
}


void updateUserInfo(vector<stUser> &vStUsers, string username) {
	stUser user;
	char answer = 'n';

	if ((isUserNameExists(vStUsers, user, username) == true)) {
		printUserRecord(user);
		cout << endl << "Are you sure you want to update this user ? (y/n)\n";
		cin >> answer;
		if (answer == 'y' || answer == 'Y') {
			markUserForUpdateByUsername(vStUsers, username);
			getUserUpdatedInfo(vStUsers);
			saveUsersVectorToFile(usersFile, vStUsers);
			cout << "\nuser Updated Successfully.\n";
		}
	}
	else {
		cout << "\nUser with username (" << username << ") not found !\n";
	}
}

void ShowUpdateUsersScreen() {
	cout << "\n-----------------------------------\n";
	cout << "\tUpdate User Info Screen";
	cout << "\n-----------------------------------\n";
	vector<stUser> vStUsers = readUsersFileContent(usersFile);
	string username = readUsername();
	updateUserInfo(vStUsers, username);
}

void findUserByUsername(string username, vector<stUser> vStUsers)  {
	char answer;
	stUser user;
	if ((isUserNameExists(vStUsers, user, username) == true)) {
		printUserRecord(user);
	}
	else {
		cout << "\nUser with username (" << username << ") not found !\n";
	}
}


void ShowFindUsersScreen() {
	cout << "\n-----------------------------------\n";
	cout << "\tFind User Screen";
	cout << "\n-----------------------------------\n";
	vector<stUser> vStUsers = readUsersFileContent(usersFile);
	string username = readUsername();
	findUserByUsername(username, vStUsers);
}


void performManageUsersMenuOptions(enManageUsersMenu choice) {
	switch (choice) {
	case enManageUsersMenu::showUsers:
		system("cls");
		ShowAllUsersScreen();
		goBackToManageUsersMenu();
		break;
	case enManageUsersMenu::addUser:
		system("cls");
		ShowAddNewUsersScreen();
		goBackToManageUsersMenu();
		break;
	case enManageUsersMenu::deleteUser:
		system("cls");
		ShowDeleteNewUsersScreen();
		goBackToManageUsersMenu();
		break;
	case enManageUsersMenu::updateUser:
		system("cls");
		ShowUpdateUsersScreen();
		goBackToManageUsersMenu();
	case enManageUsersMenu::findUser:
		system("cls");
		ShowFindUsersScreen();
		goBackToManageUsersMenu();
	case enManageUsersMenu::exitManageUsers:
		system("cls");
		showMainMenu();
	}

}

void showManageUsersMenu() {
	cout << "=========================================================\n";
	cout << "\t\tManage Users Menu Screen\n";
	cout << "=========================================================\n";
	cout << "\t[1] List Users.\n";
	cout << "\t[2] Add New User.\n";
	cout << "\t[3] Delete User.\n";
	cout << "\t[4] Update User.\n";
	cout << "\t[5] Find User.\n";
	cout << "\t[6] Main Menu.\n";
	cout << "=========================================================\n";
	performManageUsersMenuOptions((enManageUsersMenu)readMenuChoice("Please select an option from the menu above (1-6): ", 1, 6));
}

bool verifyPermission(enPermissions choicePermission) {
	if (currentUser.permission == allPermissions || (currentUser.permission & choicePermission)) {
		return true;
	}
	else return false;
}

void showAccessDenyScreen() {
	cout << "---------------------------------------------\n";
	cout << "Access denied,\n";
	cout << "You don't have permission to do this,\n";
	cout << "Please contact your admin.\n";
	cout << "---------------------------------------------\n";
}

void perfomMainMenuOptions(enMenu choice) {
		switch (choice) {
		case enMenu::showClients:
			system("cls");
			if (verifyPermission(showAllClients) == true)
				ShowAllClientsScreen();
			else showAccessDenyScreen();
			goBackToMainMenu();
			break;
		case enMenu::addClient:
			system("cls");
			if (verifyPermission(addNewClient) == true)
			    showAddNewClientsScreen();
			else showAccessDenyScreen();
			goBackToMainMenu();
			break;
		case enMenu::deleteClient:
			system("cls");
			if (verifyPermission(deleteClients) == true)
			showDeleteClientScreen();
			else showAccessDenyScreen();
			goBackToMainMenu();
			break;
		case enMenu::updateClient:
			system("cls");
			if (verifyPermission(updateClients) == true)
			showUpdateClientScreen();
			else showAccessDenyScreen();
			goBackToMainMenu();
			break;
		case enMenu::findClient:
			system("cls");
			if (verifyPermission(findClients) == true)
			ShowFindClientScreen();
			else showAccessDenyScreen();
			goBackToMainMenu();
			break;
		case enMenu::transactions:
			system("cls");
			if (verifyPermission(manageTransactions) == true)
			showTransactionsMenu();
			else showAccessDenyScreen();
			goBackToMainMenu();
			break;
		case enMenu::manageUsers:
			system("cls");
			if (verifyPermission(AdministerUsers) == true)
				showManageUsersMenu();
			else showAccessDenyScreen();
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
	cout << "\t[7] Manage Users.\n";
	cout << "\t[8] Logout.\n";
	cout << "=========================================================\n";
	perfomMainMenuOptions((enMenu)readMenuChoice("Please select an option from the menu above (1-8): ", 1, 8));
}

void Login() {
	showLoginScreen();
	vector<stUser> vStUsers = readUsersFileContent(usersFile);
	string userName = readUsername();
	int password = readPassword();
	findUserByCredentials(userName, password, vStUsers);
}

int main()
{
	Login();
	return 0;
}
