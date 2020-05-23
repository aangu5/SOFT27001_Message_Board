#pragma once
#include <string>
#include <iostream>

using namespace std;

class User {
private:
	string name = "";
	string emailAddress = "";
	int userID = 0;
	unsigned int userPassword = 0;
	bool loggedIn = false;
	int messageCount = 0;

	bool getRegistrationCredentials();
	bool getLoginCredentials();
	unsigned int hashThis(string plaintext);

public:
	string getUsername();
	string getUserEmail();
	int getUserID();
	int getMessageCount();
	unsigned int getPassword();
	void setUserID(int userIDToBe);
	void setUsername(string usernameToBe);
	void setUserEmail(string emailToBe);
	void setMessageCount(int messageCountToBe);
	void newUser(int id);
	bool userLogin();
	string inputFormatting(string upperCase);
};

