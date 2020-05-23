#include "User.h"
#include "sqlite3.h"
#include "Database.h"
#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <string>
#include <regex>
#include <vector>

using namespace std;
Database userDatabase;

//prompts the user for registration credentials, including username, email and password validation
bool User::getRegistrationCredentials()
{
	string inputEmail = "";
	string inputName = "";
	string inputPassword1 = "";
	string inputPassword2 = "";
	char passwordCharacters{};

	bool validUsername = false;
	bool validEmail = false;
	bool passwordMatch = false;

	const regex allowedPassword("^[-!£$%^&*()@~#,.<>?/|\\_+={}A-Za-z0-9]+$");
	const regex allowedUsername("^[a-zA-Z0-9]+$");
	const regex allowedEmail("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");


	cout << "Please enter your Username: " << endl;
	while (validUsername == false) {
		getline(cin, inputName, '\n');
		inputName = inputFormatting(inputName);
		if (inputName.length() > 5 && inputName.length() < 32 && regex_match(inputName, allowedUsername) && userDatabase.checkUsername(inputName)) {
			validUsername = true;
			name = inputFormatting(inputName);
		}
		else {
			cout << inputName << " is not a valid username. Usernames must have more than 6 characters and can only contain letters and numbers. Please type a new username:" << endl;

		};
	};


	cout << "Please enter your email address: " << endl;
	while (validEmail == false) {
		getline(cin, inputEmail, '\n');
		if (inputEmail.find_first_of('@') < 100 && inputEmail.find_first_of('.') < 100 && inputEmail.length() > 4 && regex_match(inputEmail, allowedEmail)) {
			validEmail = true;
			emailAddress = inputFormatting(inputEmail);
		}
		else { cout << "That is not a valid email address, please try again:" << endl; }
	};

	cout << "Please enter a password: " << endl;
	while (passwordMatch == false) {
		while (passwordCharacters != '\r') {
			passwordCharacters = _getch();
			if (passwordCharacters == '\b' && inputPassword1.length() > 0) {
				cout << "\b \b";
				inputPassword1.pop_back();
			}
			else if (passwordCharacters == '\b' && inputPassword1.length() == 0) {

			}
			else if (passwordCharacters == '\r') {

			}
			else {
				cout << '*';
				inputPassword1 += passwordCharacters;

			};
		};

		cout << endl << "Please re-enter your password:" << endl;
		passwordCharacters = ' ';
		while (passwordCharacters != '\r') {
			passwordCharacters = _getch();
			if (passwordCharacters == '\b' && inputPassword2.length() > 0) {
				cout << "\b \b";
				inputPassword2.pop_back();
			}
			else if (passwordCharacters == '\b' && inputPassword2.length() == 0) {

			}
			else if (passwordCharacters == '\r') {

			}
			else {
				cout << '*';
				inputPassword2 += passwordCharacters;

			}
		};

		if (inputPassword1 == inputPassword2 && inputPassword1.length() > 7 && inputPassword1.length() < 32 && regex_match(inputPassword1, allowedPassword)) {
			passwordMatch = true;
			userPassword = hashThis(inputPassword1);
			inputPassword1 = "";
			inputPassword2 = "";
			passwordCharacters = ' ';
			cout << endl << "You have successfully created an account, your username is " << name << " and your UserID is " << userID << "." << endl;
		}
		else {
			cout << endl << "There is an invalid entry, please try again. Passwords must be between 8 and 32 characters and cannot contain spaces." << endl << "Please enter a password" << endl;
			passwordMatch = false;
			inputPassword1 = "";
			inputPassword2 = "";
			passwordCharacters = ' ';
		};
	};
	if (validEmail && validUsername && passwordMatch) {
		return true;
	}
	else {
		return false;
	}
}

//prompts the user for credentials to log in with and checks them against the stored credentials
bool User::getLoginCredentials()
{
	string inputUsername;
	string inputPassword = "";
	char passwordCharacters{};
	int characterPosition = 0;
	bool validInput = false;
	int maxAttempts = 3;
	int attempt = 0;
	unsigned int hashedInput;

	while (validInput == false && attempt < maxAttempts) {
		cout << "Please enter your username: " << endl;
		cin >> inputUsername;
		cout << "Please enter your password: " << endl;
		while (passwordCharacters != '\r') {
			passwordCharacters = _getch();
			if (passwordCharacters == '\b' && inputPassword.length() > 0) {
				cout << "\b \b";
				inputPassword.pop_back();
			}
			else if (passwordCharacters == '\b' && inputPassword.length() == 0) {

			}
			else if (passwordCharacters == '\r') {

			}
			else {
				cout << '*';
				inputPassword += passwordCharacters;

			}
		};
		inputUsername = inputFormatting(inputUsername);
		hashedInput = hashThis(inputPassword);
		if (userDatabase.validCredentials(inputUsername, hashedInput)) {
			name = inputUsername;
			attempt++;
			validInput = true;
			return true;
		}
		else {
			cout << endl << "Username/Password is incorrect" << endl;
			inputUsername = "";
			inputPassword = "";
			passwordCharacters = ' ';
			attempt++;
			validInput = false;
			if (attempt == 3) {
				return false;
			}
		}
	}
	return false;
}

//formats user input to make it all lower case and remove leading and trailing spaces
string User::inputFormatting(string upperCase)
{
	string middleMan = "";
	string output = "";
	char ch;
	for (unsigned int i = 0; i < upperCase.length(); i++) {
		ch = upperCase.c_str()[i];
		ch = tolower(ch);
		middleMan = middleMan + ch;
	}

	size_t firstCharacter = middleMan.find_first_not_of(' ');
	if (string::npos == firstCharacter) {
		return output;
	}
	size_t lastCharacter = middleMan.find_last_not_of(' ');
	output = middleMan.substr(firstCharacter, (lastCharacter - firstCharacter + 1));
	return output;
}

//once user has provided valid login credentials, this will load their details into the current user object
bool User::userLogin() {

	if (getLoginCredentials()) {
		cout << endl;
		system("CLS");
		vector<string> tempVector = userDatabase.loadUser(name);

		userID = stoi(tempVector[0]);
		name = tempVector[1];
		emailAddress = tempVector[2];
		messageCount = stoi(tempVector[3]);
		return true;
	}
	else {
		return false;
	}
}

//returns the current user's username
string User::getUsername()
{
	return name;
}

//returns the current user's email
string User::getUserEmail()
{
	return emailAddress;
}

//returns the current user's ID
int User::getUserID()
{
	return userID;
}

//returns the current user's message count
int User::getMessageCount()
{
	return messageCount;
}

//returns the current user's hashed password
unsigned int User::getPassword()
{
	return userPassword;
}

//sets the current user's ID
void User::setUserID(int userIDToBe)
{
	userID = userIDToBe;
}

//sets the current user's username
void User::setUsername(string usernameToBe)
{
	name = usernameToBe;
}

//sets the current user's email
void User::setUserEmail(string emailToBe)
{
	emailAddress = emailToBe;
}

//sets the current user's message count
void User::setMessageCount(int messageCountToBe)
{
	messageCount = messageCountToBe;
}

//hashes any string input and returns the hash
unsigned int User::hashThis(string plaintext)
{
	unsigned int hashedText;
	hashedText = (unsigned int)hash<string>{}(plaintext);
	return hashedText;
}

//if the user inputs valid registration credentials and the database is able to save them, this will run the login procedure
void User::newUser(int id) {
	userID = id + 1;

	if (getRegistrationCredentials() && userDatabase.saveUser(*this)) {
		userLogin();
	}
	else {
		cout << "There was an error with saving your user account to the system, please try again." << endl;
		newUser(userID - 1);
	}

}