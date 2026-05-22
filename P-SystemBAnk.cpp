#include<iostream>
#include<string>
#include<fstream>
#include<iomanip>
#include<vector>
using namespace std;


const string ClientsFileName = "Clients";

const string UsersFileName = "Users";

const string Seperater = "#//#";

void MainMenue();

void Transactions();

void ManageUsers();

void Login();

enum eUserOptions{enShowClientList=1,enAddNewClient=2,enDeleteClient=3,enUpdateClientInfo=4,
	enFindClient=5,enTransaction=6,enManageUsers=7,enLogout=8};

enum eTransactions{eDeposit=1,eWithdraw=2,eTotalBalance=3,eMainMenue=4};

enum eManageUsers{enListUsers=1,enAddNewUers=2,enDeleteUser=3,
	enUpdateUser=4,enFindUser=5,enMainMenue=6};

enum enPermissions{eAll=-1,pShowClientsList=1,pAddNewClients=2,pDeleteClient=4,pUpdateClient=8,
pFindClient=16,pTransaction=32,pManageUsers=64};

struct stClients
{
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	double AccountBalance;
	bool DeleteforClient = false;
};

struct stUsers
{
	string UserName;
	string Password;
	short Permissions;
	bool MarkforDelete = false;
};

stUsers UserNow;

string Longlineprinting(short HowMuch,string Line)
{
	string Longline;
		for (short i = 0; i < HowMuch; i++)
		{
			Longline +=Line;
		}
	
	return Longline;
}

void MainMenuScreen()
{
	system("cls");
	cout << Longlineprinting(45,"=") << endl;
	cout << "\t\t Main Menue Screen" << endl;
	cout << Longlineprinting(45,"=") << endl;
	cout << "\t [1] Show Client List." << endl;
	cout << "\t [2] Add New Client." << endl;
	cout << "\t [3] Delete Client." << endl;
	cout << "\t [4] Update Client Info." << endl;
	cout << "\t [5] Find Client." << endl;
	cout << "\t [6] Transactions." << endl;
	cout << "\t [7] Manage Users." << endl;
	cout << "\t [8] Logout." << endl;
	cout << Longlineprinting(45,"=") << endl;
}

void HeaderAndColmns(vector<stClients>Iteams)
{
	system("cls");
	cout << endl;
	cout << "\t\t\t\t\t Client List ("<<Iteams.size() << ") Clients(s)" << endl;
	cout << Longlineprinting(110, "-") << endl;
	cout << "| " << left << setw(15) << "Account Number";
	cout << "| " << left << setw(10) << "Pin Code";
	cout << "| " << left << setw(50) << "Client Name";
	cout << "| " << left << setw(15) << "Phone";
	cout << "| " << left << setw(15) << "Balance";
	cout << endl;
	cout << Longlineprinting(110, "-") << endl;
}

void PrintListClients(stClients Client)
{
	cout << "| " << left << setw(15) << Client.AccountNumber;
	cout << "| " << left << setw(10) << Client.PinCode;
	cout << "| " << left << setw(50) << Client.Name;
	cout << "| " << left << setw(15) << Client.Phone;
	cout << "| " << left << setw(15) << Client.AccountBalance;
}

vector<string> Splitstring(string Line,string Seperater)
{
	short Pos = 0;
	vector<string>S2;
	string Sword;
	while ((Pos = Line.find(Seperater)) != ::string::npos)
	{
		Sword = Line.substr(0, Pos);
		if (Sword != "")
		{
			S2.push_back(Sword);
		}
		Line.erase(0, Pos + Seperater.length());
	}

	if (Line != "")
	{
		S2.push_back(Line);
	}

	return S2;
}

stClients Convertstringtostruct(string Line)
{
	vector<string>Data;
	Data = Splitstring(Line,Seperater);
	stClients Client;
	Client.AccountNumber=Data[0];
	Client.PinCode=Data[1];
	Client.Name=Data[2];
	Client.Phone=Data[3];
	Client.AccountBalance = stod(Data[4]);
	return Client;
}

stUsers Converttostruct(string word)
{
	vector<string>Line;
	Line = Splitstring(word, Seperater);
	stUsers User;
	User.UserName = Line[0];
	User.Password = Line[1];
	User.Permissions = stoi(Line[2]);
	return User;
}

vector<stClients>LoadClientDatafromFile(string FileName)
{
	fstream Clients;
	vector<stClients> vClient;
	Clients.open(FileName, ios::in);
	if (Clients.is_open())
	{
		stClients Client;
		string Line;
		while (getline(Clients, Line))
		{
			Client = Convertstringtostruct(Line);
			vClient.push_back(Client);
		}
		Clients.close();
	}
	return vClient;
}

vector<stUsers>LoadUserDatafromFile(string FileName)
{
	fstream User;
	vector<stUsers> vUser;
	User.open(FileName, ios::in);
	if (User.is_open())
	{
		stUsers User1;
		string Line;
		while (getline(User, Line))
		{
			User1 =  Converttostruct(Line);
			vUser.push_back(User1);
		}
		User.close();
	}
	return vUser;
}

bool FindClientByAccountNumber(stClients &Clients,vector<stClients>&vClient, string AccountNumber)
{
	for (stClients& C : vClient)
	{
		if (C.AccountNumber == AccountNumber)
		{
			Clients = C;
			return true;
		}
	}

	return false;
}

bool FindUserByUserName(stUsers& User, vector<stUsers>& vUser, string UserName)
{
	for (stUsers& U : vUser)
	{
		if (U.UserName == UserName)
		{
			User = U;
			return true;
		}
	}

	return false;
}

bool FindUserByUserNameAndPassword(string userName,string password, stUsers& User)
{
	vector<stUsers> vUser;
	vUser = LoadUserDatafromFile(UsersFileName);
	for (stUsers& U : vUser)
	{
		if ((U.UserName == userName)&&(U.Password==password))
		{
			User = U;
			return true;
		}
	}

	return false;
}

void PrintErorePermissins()
{
	cout << endl;
	cout << Longlineprinting(40, "-") << endl;
	cout << "Access Denied,\nYou dont have Permission to do this,\nPlease Conact your admin." << endl;
	cout << Longlineprinting(40, "-") << endl;
}

bool CheckPermission(enPermissions Permission)
{
	return ((UserNow.Permissions & Permission) == Permission);
		
}

short ReadUser(string Msg,short from,short to)
{
	
	short UserOp=0;
	do
	{		
		cout << Msg;
		cin >> UserOp;
	} while (UserOp < from || UserOp>to);
	return UserOp;
}

void AddNewClientsScreen()
{
	cout << endl << endl;
	cout << Longlineprinting(40, "-") << endl;
	cout << "\t Add New Clients Screen" << endl;
	cout << Longlineprinting(40, "-") << endl;
	cout << "Adding New Client:" << endl << endl;
}

void AddNewUsersScreen()
{
	cout << endl << endl;
	cout << Longlineprinting(40, "-") << endl;
	cout << "\t Add New Users Screen" << endl;
	cout << Longlineprinting(40, "-") << endl;
	cout << "Adding New User:" << endl << endl;
}

void DeleteUserScreen()
{
	cout << endl << endl;
	cout << Longlineprinting(40, "-") << endl;
	cout << "\t Delete Users Screen" << endl;
	cout << Longlineprinting(40, "-") << endl << endl;
}

void FindUserScreen()
{
	cout << endl << endl;
	cout << Longlineprinting(40, "-") << endl;
	cout << "\t Find Users Screen" << endl;
	cout << Longlineprinting(40, "-") << endl << endl;
}

void UpdateUserScreen()
{
	cout << endl << endl;
	cout << Longlineprinting(40, "-") << endl;
	cout << "\t Update Users Screen" << endl;
	cout << Longlineprinting(40, "-") << endl << endl;
}

stClients ReadClient(vector<stClients>vClients)
{
	stClients Client;

	cout << "Enter Account Number : ";
	cin >> Client.AccountNumber;
	while (FindClientByAccountNumber(Client,vClients, Client.AccountNumber))
	{
		cout << "\nClient with [" << Client.AccountNumber << "] already exists,";
		cout << "\nEnter Another Account Number : ";
		cin >> Client.AccountNumber;
	}
	cout << "Enter Pin Code : ";
	cin >> Client.PinCode;
	cout << "Enter Name : ";
	getline(cin>>ws,Client.Name);
	cout << "Enter Phone : ";
	cin >> Client.Phone;
	cout << "Enter Account Balance : ";
	cin >> Client.AccountBalance;
	return Client;
}

string ConvertRecordtoLine(stClients Client,string Seperter)
{
	string Line;
	Line += Client.AccountNumber + Seperter;
	Line += Client.PinCode + Seperter;
	Line += Client.Name + Seperter;
	Line += Client.Phone + Seperter;
	Line +=to_string( Client.AccountBalance);
	return Line;
}

string ConvertRecordUserstoLine(stUsers User, string Seperater)
{
	string Line;
	Line = User.UserName + Seperater;
	Line += User.Password + Seperater;
	Line += to_string(User.Permissions);
	return Line;
}

void SaveDataToFile(string FileName,string Line)
{
	fstream Client;
	Client.open(FileName, ios::out | ios::app);
	if (Client.is_open())
	{
		Client << Line << endl;
		Client.close();
	}
}

void SavedataClientstoFileAftermodification(string FileName,vector<stClients>&vClient)
{
	fstream Client;
	Client.open(FileName, ios::out);
	if (Client.is_open())
	{
		string Line;
		for (stClients &C:vClient)
		{
			if (C.DeleteforClient == false)
			{
				Line = ConvertRecordtoLine(C, Seperater);
				Client << Line << endl;
			}
		}

		Client.close();
	}
}

void SavedataUserstoFileAftermodification(string FileName, vector<stUsers>& vUser)
{
	fstream User;
	User.open(FileName, ios::out);
	if (User.is_open())
	{
		string Line;
		for (stUsers &U : vUser)
		{
			if (U.MarkforDelete == false)
			{
				Line = ConvertRecordUserstoLine(U, Seperater);
				User << Line << endl;
			}
		}

		User.close();
	}
}

string ReadAccountNumber(string Msg)
{
	string Account;
	cout << Msg;
	cin >>Account;
	return Account;
}

void Head(string TypeofService)
{
	system("cls");
	cout << endl;
	cout << Longlineprinting(40, "-") << endl;
	cout << TypeofService << endl;
	cout << Longlineprinting(40, "-") << endl;
}

stClients AddUpdate(string AccountNumber)
{
	stClients Client;
	Client.AccountNumber = AccountNumber;
	cout << "\nEnter Pin Code : ";
	cin >> Client.PinCode;
	cout << "Enter Name : ";
	getline(cin >> ws, Client.Name);
	cout << "Enter Phone : ";
	cin >> Client.Phone;
	cout << "Enter Account Balance : ";
	cin >> Client.AccountBalance;
	return Client;
}

void UpdateClientNewInformation(vector<stClients>&vClient,string AccountNumber)
{

	for (stClients& C : vClient)
	{
		if (C.AccountNumber == AccountNumber)
		{
			C = AddUpdate(AccountNumber);
		}
	}
}

void PrintClientCard(stClients Client)
{
	cout << endl << endl;
	cout << "The following are the Client Details:" << endl;
	cout << Longlineprinting(40, "-") << endl;
	cout << "Account Number : " << Client.AccountNumber << endl;
	cout << "Pin Code       :" << Client.PinCode << endl;
	cout << "Name           :" << Client.Name << endl;
	cout << "Phone          :" << Client.Phone << endl;
	cout << "Account Balance:" << Client.AccountBalance << endl;
	cout << Longlineprinting(40, "-") << endl;
}

void MarkForDeleteClient(vector<stClients>&vClient,string AccountNumber)
{
	for (stClients& C : vClient)
	{
		if (C.AccountNumber == AccountNumber)
		{
			C.DeleteforClient = true;
		}
	}
}

void GoBacktoMainMenue()
{
	cout << "\nPress any key to go back Main Menue . . .";
	system("pause>0");
	MainMenue();
}

void GoBacktoTransactionsMain()
{
	cout << "\nPress any key to go back Main Transactions . . .";
	system("pause>0");
	Transactions();

}

void GoBackManageUsersMenue()
{
	cout << "\nPress any key to go back Mangae Users Menue . . .";
	system("pause>0");
	ManageUsers();
}

void ShowClients(vector<stClients>&vClients)
{
	if (!CheckPermission(pShowClientsList))
	{
		PrintErorePermissins();
		return;
	}
	HeaderAndColmns(vClients);

	for (stClients& C : vClients)
	{
		PrintListClients(C);
		cout << endl;
	}
	cout << Longlineprinting(110, "-") << endl;
}

void AddClients(vector<stClients>&vClients)
{
	if (!CheckPermission(pAddNewClients))
	{
		PrintErorePermissins();
		return;
	}
	stClients Client;
	string Line;
	char addmore = 'y';

	do 
	{
		system("cls");
		AddNewClientsScreen();
		 Client = ReadClient(vClients);
		 Line = ConvertRecordtoLine(Client,Seperater);
		 SaveDataToFile(ClientsFileName,Line);
		 cout << endl;
		 cout << "Client Added Successfully.." << endl << endl;
		 cout << "Do you want to Add More Client? y/n : ";
		 cin >> addmore;

	} while (addmore == 'y' || addmore == 'Y');
}

void DeleteClients(vector<stClients>&vClients)
{
	if (!CheckPermission(pDeleteClient))
	{
		PrintErorePermissins();
		return;
	}
	Head("\t Delete Client Screen ");
	stClients Client;
	string AccountNumber = ReadAccountNumber("\nPlease Enter Account Number : ");

	if (FindClientByAccountNumber(Client,vClients, AccountNumber))
	{
		PrintClientCard(Client);
		char SureDelete = 'Y';
		cout << "\n\nAre You Sure You Want Delete This Client? y/n : ";
		cin >> SureDelete;
		if (SureDelete == 'y' || SureDelete == 'Y')
		{
			MarkForDeleteClient(vClients, AccountNumber);
			SavedataClientstoFileAftermodification(ClientsFileName, vClients);
			cout << "\n\nClient Deleted Successfully.";
		}
		
	}
	else
	{
		cout << "\nThe " << AccountNumber << " is Not Found.";
	}
}

void UpdateClients(vector<stClients>& vClients)
{
	if (!CheckPermission(pUpdateClient))
	{
		PrintErorePermissins();
		return;
	}
	Head("\t Update Client Screen ");
	stClients Client;
	string AccountNumber = ReadAccountNumber("\nPlease Enter Account Number : ");
	
	if (FindClientByAccountNumber(Client, vClients, AccountNumber))
	{
		PrintClientCard(Client);
		char SureUpdate = 'Y';
		cout << "\n\nAre You Sure You Want Update This Client? y/n : ";
		cin >> SureUpdate;
		if (SureUpdate == 'y' || SureUpdate == 'Y')
		{
			UpdateClientNewInformation(vClients,AccountNumber);
			SavedataClientstoFileAftermodification(ClientsFileName, vClients);
			cout << "\n\nClient Update Successfully.";
		}
	}
	else
	{
		cout << "\nThe " << AccountNumber << " is Not Found.";
	}
}

void FindClients(vector<stClients>& vClient)
{
	if (!CheckPermission(pFindClient))
	{
		PrintErorePermissins();
		return;
	}
	Head("\t Profile Client Screen ");
	stClients Client;
	string AccountNumber = ReadAccountNumber("\nPlease Enter Account Number : ");
	if (FindClientByAccountNumber(Client, vClient, AccountNumber))
	{
		PrintClientCard(Client);
	}
	else
	{
		cout << "\nThe " << AccountNumber << " is Not Found.";
	}
}

void TransactonsMenueScreen()
{
	cout << Longlineprinting(45, "=") << endl;
	cout << "\tTransactions Menue Screen" << endl;
	cout << Longlineprinting(45, "=") << endl;
	cout << "\t [1] Deposit." << endl;
	cout << "\t [2] Withdraw." << endl;
	cout << "\t [3] Total Balance." << endl;
	cout << "\t [4] Main Menue." << endl;
	cout << Longlineprinting(45, "=") << endl;
}

void ManageUsersMenueScreen()
{
	cout << Longlineprinting(45, "=") << endl;
	cout << "\tManage Users Menue Screen" << endl;
	cout << Longlineprinting(45, "=") << endl;
	cout << "\t [1] List Users." << endl;
	cout << "\t [2] Add New User." << endl;
	cout << "\t [3] Delete User." << endl;
	cout << "\t [4] Update User." << endl;
	cout << "\t [5] Find User." << endl;
	cout << "\t [6] Main Menue." << endl;
	cout << Longlineprinting(45, "=") << endl;
}

double ReadNumber(string Msg)
{
	double ReadAmounts;
	cout << Msg;
	cin >> ReadAmounts;
	return ReadAmounts;
}

short ReadPermissios()
{
	short Permissions = 0;
	char answer = 'n';
	
	cout << "\nDo you want to give full access? y/n : ";
	cin >> answer;
	if (toupper(answer) == 'Y')
	{
		Permissions = eAll;
		return Permissions;
	}

	cout << "\nDo you want to give access to : \n";
	cout << "\nShow Clients List y/n :";
	cin >> answer;
	if (toupper(answer) == 'Y')
	{
		Permissions = pShowClientsList;
	}
	cout << "\nAdd New Client y/n :";
	cin >> answer;
	if (toupper(answer) == 'Y')
	{
		Permissions += pAddNewClients;
	}
	cout << "\nDelete Clients y/n :";
	cin >> answer;
	if (toupper(answer) == 'Y')
	{
		Permissions += pDeleteClient;
	}

	cout << "\nUpdate Clients y/n :";
	cin >> answer;
	if (toupper(answer) == 'Y')
	{
		Permissions +=pUpdateClient;
	}
	cout << "\nFind Clients y/n :";
	cin >> answer;
	if (toupper(answer) == 'Y')
	{
		Permissions +=pFindClient;
	}
	cout << "\nTransactions y/n :";
	cin >> answer;
	if (toupper(answer) == 'Y')
	{
		Permissions +=pTransaction;
	}
	cout << "\nManage Users y/n :";
	cin >> answer;
	if (toupper(answer) == 'Y')
	{
		Permissions +=pManageUsers;
	}
	return Permissions;


}

stUsers ReadNewuser(vector<stUsers>&vUser)
{
	stUsers User;
	cout << "Enter User Name : ";
	getline(cin >> ws, User.UserName);
	while (FindUserByUserName(User, vUser, User.UserName))
	{
		cout << "\nUser with [" << User.UserName << "] already exists,";
		cout << "\nEnter Another User Name : ";
		getline(cin >>ws, User.UserName);
	}
	cout << "Enter Password : ";
	getline(cin >> ws, User.Password);
	
	User.Permissions = ReadPermissios();
		
	return User;
}

stUsers ReadUpdatedUser(stUsers oldUser, vector<stUsers>& vUser)
{
	stUsers User = oldUser;

	string NewName;

	cout << "Enter New User Name (or press Enter to keep it): ";
	getline(cin >> ws, NewName);

	
	if (NewName != "" && NewName != oldUser.UserName)
	{
		stUsers temp;
		while (FindUserByUserName(temp, vUser, NewName))
		{
			cout << "\nUser with [" << NewName << "] already exists, enter another: ";
			getline(cin >> ws, NewName);
		}
		User.UserName = NewName;
	}

	cout << "Enter New Password : ";
	getline(cin >> ws, User.Password);

	cout << "\nUpdate Permissions:\n";
	User.Permissions = ReadPermissios();

	return User;
}


void AddNewUsers()
{
	vector<stUsers> vUser;
	vUser = LoadUserDatafromFile(UsersFileName);
	stUsers User;
	string Line;
	char addmore = 'y';

	do
	{
		system("cls");
		AddNewUsersScreen();
		User = ReadNewuser(vUser);
		Line = ConvertRecordUserstoLine(User, Seperater);
		SaveDataToFile(UsersFileName, Line);
		cout << endl;
		cout << "User Added Successfully.." << endl << endl;
		cout << "Do you want to Add More User? y/n : ";
		cin >> addmore;

	} while (addmore == 'y' || addmore == 'Y');
}

void PrintHeaderMangaeUsers(vector<stUsers>& Iteams)
{
	cout << endl;
	cout << "\t\t\t\t\t Users List (" << Iteams.size() << ") User(s)" << endl;
	cout << Longlineprinting(110, "-") << endl;
	cout << "| " << left << setw(30) << "User Name";
	cout << "| " << left << setw(15) << "Password";
	cout << "| " << left << setw(50) << "Promissions";
	cout << endl;
	cout << Longlineprinting(110, "-") << endl;
}

void PrintUser(stUsers User)
{
	cout << "| " << left << setw(30) << User.UserName;
	cout << "| " << left << setw(15) << User.Password;
	cout << "| " << left << setw(50) << User.Permissions;
	cout << endl;
	cout << Longlineprinting(110, "-") << endl;
}

void ShowListUsers()
{
	vector<stUsers>vUser;
	vUser = LoadUserDatafromFile(UsersFileName);
	PrintHeaderMangaeUsers(vUser);

	for (stUsers& User : vUser)
	{
		PrintUser(User);
	}
}

void DepositBalanceToClientByAccountNumber(string AccountNumber,
	double Amount, vector<stClients>& vClients)
{
	char Sure = 'Y';
	cout << "\nAre You Sure you want perfrom this transaction? y/n : ";
	cin >> Sure;
	if (Sure == 'y' || Sure == 'Y')
	{
		for (stClients& C : vClients)
		{
			if (C.AccountNumber == AccountNumber)
			{
				C.AccountBalance += Amount;
				cout << "\n\n Done successfully , New amount is : " << C.AccountBalance;
				break;
			}
		}
		SavedataClientstoFileAftermodification(ClientsFileName, vClients);
	}
}

void Deposit(vector<stClients>& vClients)
{
	Head("\t Deposit Screen");
	string AccountNumber = ReadAccountNumber("Please Enter Account Number : ");
	stClients Client;
	if (FindClientByAccountNumber(Client, vClients, AccountNumber))
	{
		PrintClientCard(Client);
		double Amount = ReadNumber("\n Please Enter Deposit Amount ? ");
		DepositBalanceToClientByAccountNumber(AccountNumber, Amount, vClients);
	}
	else
	{
		cout << "\n Client with [" << AccountNumber << "] does not exist.";
	}
}

void HeaderandColmnsBalnceList(vector<stClients>& vClients)
{
	cout << endl << endl;
	cout << "\t\t\t Balance List(" << vClients.size() << ") Clients(s)." << endl;
	cout << Longlineprinting(110, "-") << endl;
	cout << "| " << left << setw(20) << "Accunt Number";
	cout << "| " << left << setw(45) << "Client Name";
	cout << "| " << left << setw(15) << "Balance" << endl;
	cout << Longlineprinting(110, "-") << endl;
}

void Printinfobalance(stClients Client)
{
	cout << "| " << left << setw(20) << Client.AccountNumber;
	cout << "| " << left << setw(45) << Client.Name;
	cout << "| " << left << setw(15) << Client.AccountBalance;
}

void Withdraw(vector<stClients>& vClients)
{
	Head("\t Deposit Screen");
	string AccountNumber = ReadAccountNumber("Please Enter Account Number : ");
	stClients Client;
	if (FindClientByAccountNumber(Client, vClients, AccountNumber))
	{
		PrintClientCard(Client);
		double Amount = ReadNumber("\n Please Enter Withdraw Amount ? ");
		while (Client.AccountBalance < Amount)
		{
			cout << "\nAmount Exceeds the balance, you can withdraw up to : " << Client.AccountBalance;
			Amount = ReadNumber("\n Please Enter Withdraw Amount ? ");
		}

		DepositBalanceToClientByAccountNumber(AccountNumber, Amount * -1, vClients);
	}
	else
	{
		cout << "\n Client with [" << AccountNumber << "] does not exist.";
	}
}

void BalanceList(vector<stClients>& vClients) 
{
	HeaderandColmnsBalnceList(vClients);
	int Sumbalance = 0;
	for (stClients& C : vClients)
	{
		Sumbalance += C.AccountBalance;
		Printinfobalance(C);
		cout << endl;
	}
	cout << endl;
	cout << Longlineprinting(110, "-") << endl;
	cout << "\t\t\t\t\t Total Balances = " << Sumbalance << endl;
}

void Transactions()
{
	if (!CheckPermission(pTransaction))
	{
		PrintErorePermissins();
		return;
	}
	system("cls");
	TransactonsMenueScreen();
	vector<stClients> vClients;
	vClients = LoadClientDatafromFile(ClientsFileName);
	short Choiseuser =ReadUser("Choose what do you want to do ? [1 to 4]? ", 1, 4);
	switch ((eTransactions)Choiseuser)
	{
	case eTransactions::eDeposit:
		system("cls");
		Deposit(vClients);
		GoBacktoTransactionsMain();
		break;
	case eTransactions::eWithdraw:
		system("cls");
		Withdraw(vClients);
		GoBacktoTransactionsMain();
		break;
	case eTransactions::eTotalBalance:
		system("cls");
		BalanceList(vClients);
		GoBacktoTransactionsMain();
		break;
	default:
			MainMenue();
	}
}

void PrintinfoUser(stUsers User)
{
	cout << endl;
	cout << Longlineprinting(30, "-") << endl;
	cout << "User Name :   " << User.UserName << endl;
	cout << "Password :    " << User.Password << endl;
	cout << "Permissions : " << User.Permissions << endl;
	cout << Longlineprinting(30, "-") << endl;

}

bool Checkadmin(string UserName, vector<stUsers>& vUser)
{
	return ((UserName == vUser[0].UserName));
}

bool Confirm(string Msg)
{
	char Sure = 'n';
	cout << Msg;
	cin >> Sure;
	if (toupper(Sure) == 'Y')
	{
		return true;
	}
	return false;
}

void MarkfordeleteUser(vector<stUsers>& vUser, string UserName)
{
	for (stUsers& U : vUser)
	{
		if (U.UserName == UserName)
		{
			U.MarkforDelete = true;
		}
	}
}

void PerformanceDeleteingUser(vector<stUsers>&vUser)
{
	string UserName;
	stUsers User;
	bool Again,Again2;
	Again= false;
	Again2 = false;
	do {
		system("cls");
		DeleteUserScreen();
		if (Again2)
		{
			cout << "You cannot delete this user.\n";
		}
		else if (Again)
		{
			cout << "Is[" << UserName << "] Not Found.\n";
		}
		cout << "\nEnter User Name : ";
		cin >> UserName;
		Again2 = Checkadmin(UserName, vUser);
		Again = !FindUserByUserName(User, vUser, UserName);
		
	} while (Again2||Again);

	PrintinfoUser(User);
	if (Confirm("\nAre Sure You Want Delete This User ? y/n : "))
	{
		MarkfordeleteUser(vUser, UserName);
		cout << "\n\nDeleted User Successfully..";
	}

}

void DeleteUser()
{
	vector<stUsers> vUser;
	vUser = LoadUserDatafromFile(UsersFileName);
	PerformanceDeleteingUser(vUser);
	SavedataUserstoFileAftermodification(UsersFileName, vUser);
}

void UpdateingUserNewinfo(vector<stUsers>& vUser, string UserName,stUsers User)
{
	for (stUsers& U : vUser)
	{
		if (U.UserName == UserName)
		{
			U = User;
			break;
		}
	}
}

void PerformanceUpdateUser(vector<stUsers>& vUser)
{
	string UserName = "";
	stUsers User;
	bool Again = false;
	do
	{
		system("cls");
		UpdateUserScreen();
		if (Again)
		{
			cout << "Is[" << UserName << "] Not Found.\n";
		}
		cout << "\nEnter User Name : ";
		cin >> UserName;
		Again = !FindUserByUserName(User, vUser, UserName);

	} while (Again);
	PrintinfoUser(User);
	if (Confirm("\nAre You Sure want this User to Update ? y/n : "))
	{
		User = ReadUpdatedUser(User,vUser);
		UpdateingUserNewinfo(vUser, UserName, User);
		cout << "\n\nUpdateing User Successfully..";
	}
}

void UpdateUsers()
{
	vector<stUsers> vUser;
	vUser = LoadUserDatafromFile(UsersFileName);
	PerformanceUpdateUser(vUser);
	SavedataUserstoFileAftermodification(UsersFileName, vUser);

}

void PerformanceFindUser(vector<stUsers>& vUser)
{
	string UserName = "";
	stUsers User;
	bool Again = false;
	do
	{
		system("cls");
		FindUserScreen();
		if (Again)
		{
			cout << "Is[" << UserName << "] Not Found.\n";
		}
		cout << "\nEnter User Name : ";
		cin >> UserName;
		Again = !FindUserByUserName(User, vUser, UserName);

	} while (Again);
	PrintinfoUser(User);
}

void FindUsers()
{
	vector<stUsers> vUser;
	vUser = LoadUserDatafromFile(UsersFileName);
	PerformanceFindUser(vUser);
}

void ManageUsers()
{
	if (!CheckPermission(pManageUsers))
	{
		PrintErorePermissins();
		return;
	}
	system("cls");
	ManageUsersMenueScreen();
	short ChoiceUser = ReadUser("Choose whate do you want to do? [1 to 6] : ", 1, 6);

	switch ((eManageUsers)ChoiceUser)
	{
	case eManageUsers::enListUsers:
		system("cls");
		ShowListUsers();
		GoBackManageUsersMenue();
		break;
	case eManageUsers::enAddNewUers:
		system("cls");
		AddNewUsers();
		GoBackManageUsersMenue();
		break;
	case eManageUsers::enDeleteUser:
		system("cls");
		DeleteUser();
		GoBackManageUsersMenue();
		break;
	case eManageUsers::enUpdateUser:
		system("cls");
		UpdateUsers();
		GoBackManageUsersMenue();
		break;
	case eManageUsers::enFindUser:
		system("cls");
		FindUsers();
		GoBackManageUsersMenue();
		break;
	default:
		MainMenue();
	}

}

void MainMenue()
{
	vector<stClients> vClients;
	vClients = LoadClientDatafromFile(ClientsFileName);
	MainMenuScreen();
	short ChoicesUser =ReadUser("Choose what do you want to do? [1 to 8]? ",1,8);

	switch ((eUserOptions)ChoicesUser)
	{
	case eUserOptions::enShowClientList:
		system("cls");
		ShowClients(vClients);
		GoBacktoMainMenue();
		break;
	case eUserOptions::enAddNewClient:
		system("cls");
		AddClients(vClients);
		GoBacktoMainMenue();
		break;
	case eUserOptions::enDeleteClient:
		system("cls");
		DeleteClients(vClients);
		GoBacktoMainMenue();
		break;
	case eUserOptions::enUpdateClientInfo:
		system("cls");
		UpdateClients(vClients);
		GoBacktoMainMenue();
		break;
	case eUserOptions::enFindClient:
		system("cls");
		FindClients(vClients);
		GoBacktoMainMenue();
		break;
	case eUserOptions::enTransaction:
		system("cls");
		Transactions();
		break;
	case eUserOptions::enManageUsers:
		system("cls");
		ManageUsers();
		GoBacktoMainMenue();
		break;
	default:
		Login();
	}
}

bool FindUser(string UserName,string Password)
{
	
	if (FindUserByUserNameAndPassword(UserName, Password, UserNow))
		return true;
	
	return false;
}

void ShowHeaderLoginScreen()
{
	system("cls");
	cout << Longlineprinting(40, "-") << endl;
	cout << "\t\tLogin Screen" << endl;
	cout << Longlineprinting(40, "-") << endl;
}

void Login()
{
	bool Again = false;
	string UserName, Password;
	do {
		ShowHeaderLoginScreen();

		if (Again)
		{
			cout << "Invalid UserName/Password!" << endl;
		}
		cout << "\nPlease Enter User Name : ";
		cin >> UserName;
		cout << "\nPlease Enter Password : ";
		cin >> Password;
		Again = !FindUser(UserName, Password);
	} while (Again);
	MainMenue();

}

int main()
{
	//Start();
	Login();
	
	return 0;
}