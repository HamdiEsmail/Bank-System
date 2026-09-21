#include <iostream>
#include<fstream>
#include<string>
#include<vector>
#include<iomanip>
using namespace std;
const string fileName = "clients.txt";
const string UserFile = "Users.txt";

enum userChoice { ShowClients = 1, AddClient = 2, DeleteClient = 3, UpdateClient = 4, FindClient = 5, transActionn = 6,ManageUser=7 ,logOut=8};
enum etransAction { eDeposit = 1, eWithDraw = 2, eTotalBalance = 3, eMainManue = 4 };
enum ePermission {allPermission=-1,pShowClient=1,pAddNewClient=2,pDeleteClient=4,pUpdateClient=8,pFinClient=16,pTransaction=32,pManageUser=64};
enum enManageUser {eListUser=1,eAddNewUser=2,eDeleteUser=3,eUpdateUser=4,eFindUser=5,eGoToMainManue=6};
struct stClient {
	string accNumber;
	string pinCode;
	string name;
	string phone;
	double balance;
	bool markToDelete = false;
};
struct stUser {
	string userName,userPass;
	int permission=0;
	bool markToDelete = false;
};
void AccessDenied() {
	system("cls");
	cout << "___________________________________________________________\n";
	cout << "Accsess Denied ,\n";
	cout << "You Don't Have Permission To Do This \n";
	cout << "Please Conact Your Admin \n";
	cout << "___________________________________________________________\n";


}
stUser CurrenUser;
string gDelim = "#//#";
void readPermission( stUser &user) {
	char userChoice;
	cout << "give user all permission y/n\n";
	cin >> userChoice;
	if (tolower(userChoice) == 'y') {
		user.permission = ePermission::allPermission;
	
	}
	else {
		cout << "Show Client List (y/n)\n";
		cin >> userChoice;
		if (tolower(userChoice) == 'y') {
			user.permission |= ePermission::pShowClient;
		
		}
		cout << "Add New Client (y/n)\n";
		cin >> userChoice;
		if (tolower(userChoice) == 'y') {
			user.permission |= ePermission::pAddNewClient;

		}
		cout << "Delete Client (y/n)\n";
		cin >> userChoice;
		if (tolower(userChoice) == 'y') {
			user.permission |= ePermission::pDeleteClient;

		}cout << "Update Client (y/n)\n";
		cin >> userChoice;
		if (tolower(userChoice) == 'y') {
			user.permission |= ePermission::pUpdateClient;

		}
		cout << "Find Client (y/n)\n";
		cin >> userChoice;
		if (tolower(userChoice) == 'y') {
			user.permission |= ePermission::pFinClient;

		}
		cout << "Transaction  (y/n)\n";
		cin >> userChoice;
		if (tolower(userChoice) == 'y') {
			user.permission |= ePermission::pTransaction;

		}cout << "Manage User  (y/n)\n";
		cin >> userChoice;
		if (tolower(userChoice) == 'y') {
			user.permission |= ePermission::pManageUser;

		}
	
	
	
	}











}
bool hasPermission( const stUser &user, ePermission userPermisson) {

	return (user.permission & userPermisson)==userPermisson;

}
vector <string> mySplit(string userString, string delim = "#//#") {
	vector<string>vUserData;
	size_t pos;
	string word;
	while ((pos = userString.find(delim)) != string::npos) {

		word = userString.substr(0, pos);
		if (word != "") {
			vUserData.push_back(word);


		}
		userString.erase(0, pos + delim.length());




	}
	if (userString != "") {

		vUserData.push_back(userString);

	}
	return vUserData;




}
stClient convertStringToRecord(string userString, string delim = "#//#") {

	stClient client;
	vector <string>vUserData = mySplit(userString);
	client.accNumber = vUserData[0];
	client.pinCode = vUserData[1];
	client.name = vUserData[2];
	client.phone = vUserData[3];
	client.balance = stod(vUserData[4]);

	return client;


}
string convertRecordToString(const stClient client, string delim = "#//#") {
	string myString = "";
	myString += client.accNumber + delim;
	myString += client.pinCode + delim;
	myString += client.name + delim;
	myString += client.phone + delim;
	myString += to_string(client.balance);

	return myString;



}
vector<stClient>fillVectorByFile(string fileName) {
	fstream myFile;
	vector<stClient> vAllClient;
	myFile.open(fileName, ios::in);
	if (myFile.is_open()) {
		string line;
		stClient client;
		while (getline(myFile, line)) {
			client = convertStringToRecord(line);
			vAllClient.push_back(client);


		}





		myFile.close();
	}

	return vAllClient;





}
void fillFileByVector(const vector<stClient>& vClientsData, string fileName) {
	fstream myFile;
	myFile.open(fileName, ios::out);
	if (myFile.is_open()) {
		string line;
		for (const stClient& c : vClientsData) {
			if (c.markToDelete != true) {

				line = convertRecordToString(c);
				myFile << line << endl;

			}

		}


		myFile.close();
	}




}
stClient readClient() {
	stClient client;
	cout << "Please Enter Account Number : ";
	getline(cin >> ws, client.accNumber);
	cout << "Please Enter Pin Code       : ";
	getline(cin, client.pinCode);
	cout << "Please Enter Name           : ";
	getline(cin, client.name);
	cout << "Please Enter Your Phone     : ";
	getline(cin, client.phone);
	cout << "Please Enter Your Balance   : ";
	cin >> client.balance;
	return client;


}
void printClient(const stClient& client) {
	cout << "Account Number : " << client.accNumber << endl;
	cout << "Pin Code       : " << client.pinCode << endl;
	cout << "Name           : " << client.name << endl;
	cout << "Phone          : " << client.phone << endl;
	cout << "Balance        : " << client.balance << endl;


}
void addClient(vector<stClient>& allClient, string fileName) {
	if (!hasPermission(CurrenUser, ePermission::pAddNewClient)) {
		system("cls");

		AccessDenied();
	
	}
	else {
	
		stClient client;
		client = readClient();
		allClient.push_back(client);
		fillFileByVector(allClient, fileName);
	
	}

	

}
void Bank(vector<stClient>& vAllclient, string fileName, userChoice choice);
string readAccNumber(string text) {
	string accNumber;
	cout << text;
	getline(cin >> ws, accNumber);
	return accNumber;

}
bool findClient(vector<stClient>& allClients, string accNumber) {
	if (!hasPermission(CurrenUser,ePermission::pFinClient)) {
		system("cls");

		AccessDenied();
		return false;
	
	}
	else {

		for (const stClient& c : allClients) {

			if (accNumber == c.accNumber) {

				printClient(c);
				return true;
			}


		}


		return false;
	
	}
	


}
void deleteClient(vector<stClient>& allClient, string filName) {

	if (!hasPermission(CurrenUser,ePermission::pDeleteClient)) {
		system("cls");

		AccessDenied();
	
	}
	else {
	
	

		char userInput;
		string accNumber = readAccNumber("Enter Account Number You Want To Deleted \n");
		findClient(allClient, accNumber);
		cout << "\nAre you sure you want to deleted ?\n";
		cin >> userInput;
		if (tolower(userInput) == 'y') {

			for (stClient& c : allClient) {
				if (accNumber == c.accNumber) {

					c.markToDelete = true;
					break;
				}


			}
			fillFileByVector(allClient, filName);
			allClient = fillVectorByFile(filName);

		}
	
	}




}
void updateData(stClient& client) {

	cout << "Please Enter Pin Code       : ";
	getline(cin >> ws, client.pinCode);
	cout << "Please Enter Name           : ";
	getline(cin, client.name);
	cout << "Please Enter Your Phone     : ";
	getline(cin, client.phone);
	cout << "Please Enter Your Balance   : ";
	cin >> client.balance;

}
void updateClient(vector<stClient>& allClients, string fileName) {
	if (!hasPermission(CurrenUser, ePermission::pUpdateClient)) {
		system("cls");

		AccessDenied();
	
	
	}
	else {
		system("cls");

		char isSuer;
		string accNumber = readAccNumber("Please enter an account Number you want to update\n");
		if (findClient(allClients, accNumber)) {
			cout << "Are you Sure To update this client [y or n]? \n";
			cin >> isSuer;
			if (tolower(isSuer) == 'y') {

				for (stClient& c : allClients) {

					if (c.accNumber == accNumber) {
						updateData(c);
						break;

					}


				}
				cout << "Client Updated \n";
				fillFileByVector(allClients, fileName);
			}
		}
		else {

			cout << "Client Not Found\n";
		}
	
	}








}
userChoice readUserChoice() {
	int num;
	cout << "Choice what you want to do [1 to 6] \n";
	cin >> num;
	return userChoice(num);

}
bool findClientAndReturnIt(vector <stClient>& vAllClient, string accNum, stClient& client) {
	for (stClient& c : vAllClient) {

		if (accNum == c.accNumber) {
			client = c;
			printClient(c);
			return true;

		}



	}
	return false;



}
void deposit(vector<stClient>& vAllClient) {
	system("cls");
	int deposit = 0;
	stClient client;
	string accNumber = readAccNumber("Please enter An a account you want ? : ");
	while (!findClientAndReturnIt(vAllClient, accNumber, client)) {
		accNumber = readAccNumber("Client With " + accNumber + " is not found !! ");

	}
	cout << "Please enter deposit amount \n";
	cin >> deposit;
	client.balance += deposit;
	for (stClient& c : vAllClient) {

		if (c.accNumber == client.accNumber) {

			c = client;
			break;
		}

	}

	cout << "\nDeposit Done !!\n";

	fillFileByVector(vAllClient, fileName);

}
void withDraw(vector<stClient>& vAllClient) {
	system("cls");
	stClient client;
	int withDraw;
	string accNumber = readAccNumber("Please enter your account Number ");
	while (!findClientAndReturnIt(vAllClient, accNumber, client)) {

		accNumber = readAccNumber("Client Not Found !! Please enter your account Number Again ");

	}
	cout << "Please enter with Draw amount \n";
	cin >> withDraw;
	while (withDraw > client.balance) {

		cout << "amount exceeds the balance You can draw up to :" << client.balance << "\n";
		cin >> withDraw;

	}
	client.balance = client.balance - withDraw;
	for (stClient& c : vAllClient) {
		if (client.accNumber == c.accNumber) {

			c = client;
			break;
		}


	}
	cout << "\nWithDraw Done !!\n";

	fillFileByVector(vAllClient, fileName);

}
void balanceScreen() {
	cout << "_____________________________________________________________________________________________\n\n";

	cout << "| " << setw(20) << left << "Account Number ";
	cout << "| " << setw(30) << left << "Client Name ";
	cout << "| " << setw(12) << left << "Balance " << endl;
	cout << "_____________________________________________________________________________________________\n\n";

}
void printEachClientBalance(const stClient& client) {
	cout << "| " << setw(20) << left << client.accNumber;
	cout << "| " << setw(30) << left << client.name;
	cout << "| " << setw(12) << left << client.balance;
	cout << "\n";


}
void calcAllBalance(const vector<stClient>& allClient) {
	system("cls");
	double allBalance = 0;
	cout << "============================================================================================\n";
	cout << "                                      Total Balance Screen                                  \n";
	cout << "============================================================================================\n\n";
	cout << "                                      Client Number (" << allClient.size() << ")              \n";
	balanceScreen();


	for (const stClient& c : allClient) {
		allBalance += c.balance;
		printEachClientBalance(c);

	}
	cout << "\nTotal Balance is : " << allBalance;



}
void transActionScreen(vector <stClient>& allClients);
void GoBackToTransMenu(vector<stClient>& vAllClient)
{
	cout << "\n\nPress any key to go back to Trans Menu...";
	system("pause>0");
	transActionScreen(vAllClient);
}
void mainManue(vector <stClient>& vAllclient);
void GoBackToMainMenu()
{
	vector <stClient> vAllclient = fillVectorByFile(fileName);
	cout << "\n\nPress any key to go back to Main Menu...";
	system("pause>0");
	mainManue(vAllclient);
}
void transAction(vector <stClient>& allClients, etransAction userChoice) {
	if (userChoice > 4 || userChoice < 1) {
		return;
	}
	switch (userChoice) {
	case etransAction::eDeposit:
		deposit(allClients);
		GoBackToTransMenu(allClients);

		break;
	case etransAction::eTotalBalance:
		calcAllBalance(allClients);
		GoBackToTransMenu(allClients);

		break;
	case etransAction::eWithDraw:
		withDraw(allClients);
		GoBackToTransMenu(allClients);
		break;

	case etransAction::eMainManue:

		mainManue(allClients);
		break;

	}



}
void transActionScreen(vector <stClient>& allClients) {
	if (!hasPermission(CurrenUser, ePermission::pTransaction)) {
		system("cls");
		AccessDenied();
	
	
	}
	else {
	
		system("cls");
		cout << "============================================================================================\n";
		cout << "                                       Main Menue Screen                                    \n";
		cout << "============================================================================================\n";
		cout << setw(30) << " " << "[1] Deposit  \n";
		cout << setw(30) << " " << "[2] WithDraw  \n";
		cout << setw(30) << " " << "[3] Total Balance \n";
		cout << setw(30) << " " << "[4] Main Manue \n";

		transAction(allClients, (etransAction)readUserChoice());
		cout << "\n============================================================================================\n";
	
	}
	


}
void mainManue(vector <stClient>& vAllclient) {
	system("cls");
	cout << "============================================================================================\n";
	cout << "                                       Main Menue Screen                                    \n";
	cout << "============================================================================================\n";
	cout << setw(30) << " " << "[1] Show Client List  \n";
	cout << setw(30) << " " << "[2] Add New Client  \n";
	cout << setw(30) << " " << "[3] Delete Client  \n";
	cout << setw(30) << " " << "[4] Update Client Info \n";
	cout << setw(30) << " " << "[5] Find Client  \n";
	cout << setw(30) << " " << "[6] Transactions  \n";
	cout << setw(30) << " " << "[7] Manage Users  \n";
	cout << setw(30) << " " << "[8] Log Out  \n";
	cout << "============================================================================================\n";
	Bank(vAllclient, fileName, readUserChoice());



}
void printOneClient(stClient& Client) {

	cout << "| " << setw(20) << left << Client.accNumber;
	cout << "| " << setw(12) << left << Client.pinCode;
	cout << "| " << setw(30) << left << Client.name;
	cout << "| " << setw(12) << left << Client.phone;
	cout << "| " << setw(12) << left << Client.balance << endl;


}
void printAllClients(vector<stClient>& vAllClients) {
	if (!hasPermission(CurrenUser, ePermission::pShowClient)) {
		system("cls");
		AccessDenied();
	
	}
	else {
	
	
		cout << "| " << setw(20) << left << "Account Number ";
		cout << "| " << setw(12) << left << "Pin Code ";
		cout << "| " << setw(30) << left << "Client Name ";
		cout << "| " << setw(12) << left << "Phone ";
		cout << "| " << setw(12) << left << "Balance " << endl;
		cout << "_____________________________________________________________________________________________\n\n";

		for (stClient& c : vAllClients) {

			printOneClient(c);

		}
	
	}








}
void showEndScreen() {

	cout << "==========================================================================\n";
	cout << setw(39) << "             program End (-:\n";
	cout << "==========================================================================\n";


}
void manageUserManue();
void LoginScreen();
void Bank(vector<stClient>& vAllclient, string fileName, userChoice choice) {
	system("cls");


	if (int(choice) > 8 || int(choice) < 1) { return; }
	switch (choice) {
	case userChoice::ShowClients:
		system("cls");
		printAllClients(vAllclient);
		GoBackToMainMenu();
		mainManue(vAllclient);
		break;
	case userChoice::AddClient:
		system("cls");
		addClient(vAllclient, fileName);
		GoBackToMainMenu();
		mainManue(vAllclient);

		break;
	case userChoice::DeleteClient:
		system("cls");
		deleteClient(vAllclient, fileName);
		GoBackToMainMenu();
		mainManue(vAllclient);

		break;
	case userChoice::UpdateClient:
		system("cls");

		updateClient(vAllclient, fileName);
		GoBackToMainMenu();
		mainManue(vAllclient);

		break;
	case userChoice::FindClient:
	{
		system("cls");
		string accNumber = readAccNumber("Please enter your Account Num you want it ");
		findClient(vAllclient, accNumber);
		GoBackToMainMenu();
		mainManue(vAllclient);

		break;
	}
	case userChoice::ManageUser:
		system("cls");
		manageUserManue();

		break;
	case userChoice::transActionn:
		system("cls");
		transActionScreen(vAllclient);
		break;
	case userChoice::logOut:
		LoginScreen();
		break;

	}



}
stUser convertLineToUser(string line) {
	stUser user;
	vector<string>vLineData = mySplit(line);
	user.userName = vLineData[0];
	user.userPass = vLineData[1];
	user.permission = stoi(vLineData[2]);
	return user;
}
vector<stUser>getAllUserFromFile() {
	fstream myFile;
	string line;
	stUser user;
	vector<stUser>vAllUser;
	myFile.open(UserFile,ios::in);
	if (myFile.is_open()) {
		while (getline(myFile, line)) {
		
		user = convertLineToUser(line);
		vAllUser.push_back(user);

		}
		myFile.close();
	}
	return vAllUser;

}
string convertUserToLine(const stUser &user) {

	string line="";
	line += user.userName + gDelim;
	line += user.userPass + gDelim;
	line += to_string(user.permission) ;
	return line;

}
void setAllUserToFile(const vector<stUser>&allUsers) {
	fstream myFile;
	string word;
	myFile.open(UserFile,ios::out);
	if (myFile.is_open()) {
		for (const stUser &user:allUsers) {
			if (!user.markToDelete) {
			
				word = convertUserToLine(user);

				myFile << word << endl;
			
			}
		
		
		
		}
	
	
	
		myFile.close();
	}



}
bool isUserHere(stUser user) {
	vector<stUser>vAllUser = getAllUserFromFile();
	for (stUser &u:vAllUser) {
		if (u.userName == user.userName && u.userPass == user.userPass) {
			CurrenUser = u;
			return true;
		
		}
	
	
	}

	return false;

}
void LoginScreen() {
	system("cls");
	stUser user;
	bool found;
	cout << "_________________________________________________________________________\n";
	cout << "\n\t\t\tLogin Screen\n ";
	cout << "_________________________________________________________________________\n";
	do {
		cout << "\n\nPlease enter user Name : ";
		getline(cin >> ws, user.userName);
		cout << "Please enter user Pass : ";
		getline(cin >> ws, user.userPass);
		found = isUserHere(user);

		if (!found)
			cout << "\nInvalid Username/Password!\n\n";
	
	} while (!found);



}
void printOneUser(const stUser &user) {

	cout << "| " << setw(20) << left << user.userName << "| " << setw(20) << left << user.userPass << "| " << setw(20) << left << user.permission << endl;

}
void listUser() {
	system("cls");
	vector<stUser>vAlluser = getAllUserFromFile();
	cout << "\t\t\t\tUsers List (" << vAlluser.size() << ") User(s)\n";
	cout << "___________________________________________________________________________________________________________\n\n";

	cout << "| " << setw(20) << left << "User Name" << "| " << setw(20) << left << "Password" << "| " << setw(20) << left << "Permission" << endl;

	cout << "___________________________________________________________________________________________________________\n";

	for (const stUser &u:vAlluser) {
	
		printOneUser(u);
	
	}
}
bool isUserNameHere(string userName,stUser &user) {
	vector <stUser>vAllUser = getAllUserFromFile();
	for (const stUser &u:vAllUser) {
		if (u.userName == userName)

		{
			user = u;
			return true;
		};
	
	}
	return false;


}
void addNewUser() {
	system("cls");
	cout << "______________________________________________________\n";
	cout << "\t\t Add User Screen\n";
	cout << "______________________________________________________\n";
	stUser user;
	vector<stUser> allUser= getAllUserFromFile();
		cout << "Please enter user Name : ";
		getline(cin >> ws, user.userName);
		
		while (isUserNameHere(user.userName,user)) {
			cout << "Sorry The username already exists\n";
			cout << "Please enter user Name : ";
			getline(cin >> ws, user.userName);
		
		}
		cout << "Please enter user Pass : ";
		cin >> user.userPass;
		readPermission(user);
		allUser.push_back(user);
		setAllUserToFile(allUser);


}
void smallPrint(const stUser &user) {
	cout << "user name  : " << user.userName << endl;
	cout << "user pass  : " << user.userPass << endl;
	cout << "permission : " << user.permission<<endl;

}
void deleteUser() {
	system("cls");
	cout << "______________________________________________________\n";
	cout << "\t\t Delete User Screen\n";
	cout << "______________________________________________________\n";
	stUser user;
	char userChoice;
	vector<stUser>allUser=getAllUserFromFile();
	cout << "Enter username you want to delete it \n";
	getline(cin >> ws, user.userName);
	while (!isUserNameHere(user.userName,user)) {
		cout << "Not Found This UserName !!\n";
		cout << "Enter username you want to delete it \n";
		getline(cin >> ws, user.userName);
	}
	smallPrint(user);
	cout << "Are you sure you want delete this user ? (y/n)";
	cin >> userChoice;
	if (tolower(userChoice) == 'y') {
	
	
		for (stUser& u : allUser) {
			if (u.userName == user.userName) {

				u.markToDelete = true;
				break;
			}

		}
	}
	setAllUserToFile(allUser);
	cout << "Sccessfuly Delete..";
	


}
void updateUser() {
	system("cls");
	cout << "\n____________________________________________________________________\n";
	cout << "\t\t\t\tUpdate User \n";
	cout << "\n____________________________________________________________________\n";
	vector<stUser>vAlluser = getAllUserFromFile();
	stUser user;
	char userChoice;
	cout << "Enter your user Name : ";
	getline(cin >> ws, user.userName);
	while (!isUserNameHere(user.userName,user)) {
	
		cout << "user is not found !!\n";
		cout << "Enter your user Name : ";
		getline(cin >> ws, user.userName);
	}
	smallPrint(user);
	cout << "Are you sure you want to update this user ?\n";
	cin >> userChoice;
	if (tolower(userChoice) == 'y') {
		for (stUser &u:vAlluser) {
			if (u.userName==user.userName) {
				cout << "Please enter your password \n";
				getline(cin>>ws,u.userPass);
				readPermission(u);
				break;
			
			}
		
		
		}
	
	
	}
	setAllUserToFile(vAlluser);


}
void findUser() {
	system("cls");
	vector<stUser>vAllUser=getAllUserFromFile();
	stUser user;

	cout << "\n____________________________________________________________________\n";
	cout << "\t\t\t\t Find User Screen \n";
	cout << "\n____________________________________________________________________\n";
	cout << "Enter user name : ";
	getline(cin >> ws, user.userName);
	while (!isUserNameHere(user.userName,user)) {
	
		cout << "user name is not found!!\n";
		cout << "Enter user name : ";
		getline(cin >> ws, user.userName);
	
	
	}
	smallPrint(user);


}
enManageUser readManageUserChoice() {
	int userChoice;
	cout << "Please enter your choice [1-6]\n";
	cin >> userChoice;
	return enManageUser(userChoice);

}

void GoToManageUserManue() {
	cout << "\n\nPress any key to go back to Manage User Menu...";
	system("pause>0");
	manageUserManue();

}
void manageUser(enManageUser userChoice ) {
	if (userChoice>6||userChoice<1) {
		cout << "Invalid Num";
	}
	switch (userChoice) {
	case enManageUser::eAddNewUser:
		addNewUser();
		GoToManageUserManue();
		break;
	case enManageUser::eDeleteUser:
		deleteUser();
		GoToManageUserManue();
		break;
	case enManageUser::eUpdateUser:
		updateUser();
		GoToManageUserManue();
		break;
	case enManageUser::eListUser:
		listUser();
		GoToManageUserManue();
		break;
	case enManageUser::eFindUser:
		findUser();
		GoToManageUserManue();
		break;
	case enManageUser::eGoToMainManue:
		GoBackToMainMenu();
	
	}




}
void manageUserManue() {
	system("cls");
	if (!hasPermission(CurrenUser, ePermission::pManageUser)) {
		AccessDenied();
	
	
	}
	else {

		cout << "=======================================================================\n";
		cout << "\t\t\t Mangae user Manue Screen \n";
		cout << "=======================================================================\n";
		cout << "\t\t[1]List Userers\n";
		cout << "\t\t[2]Add New User\n";
		cout << "\t\t[3]Delete User\n";
		cout << "\t\t[4]Update User\n";
		cout << "\t\t[5]Find User\n";
		cout << "\t\t[6]Go To Main Menue\n";
		manageUser(readManageUserChoice());
	
	}



}
void Login() {
	LoginScreen();

	vector <stClient>vAllClient = fillVectorByFile(fileName);
	mainManue(vAllClient);



}
int main()
{

	Login();

	return 0;
}
