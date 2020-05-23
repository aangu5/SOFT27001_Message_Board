#include "Message.h"
#include <iostream>
#include <ctime>
#include "sqlite3.h"
#include <regex>
#include "Dashboard.h"
#include "Database.h"
using namespace std;
Database messageDatabase;

//prompts the user for message details 
void Message::composeMessage(User newMessageUser, int messageIDToBe)
{
	bool validInput = false;
	bool validSubject = false;
	bool validContent = false;
	string userInput = "";
	messageID = messageIDToBe;
	posterID = newMessageUser.getUserID();
	posterUsername = newMessageUser.getUsername();
	postedTime = time(NULL);
	const regex allowedSubject("^[-!£$%^&*()@~#,.<>?/|\\_+={}\' A-Za-z0-9]+$");
	const regex allowedContent("^[-!£$%^&*()@~#,.<>?/|\\_+={}\' A-Za-z0-9]+$");

	system("CLS");
	cout << "This message will be ID " << messageID << endl;
	cout << "Please enter the following details to upload your message to the board:" << endl;
	while (validSubject != true) {
		cout << "Please enter the subject of the message: " << endl << "Subject: ";
		getline(cin, subject, '\n');
		if (regex_match(subject, allowedSubject)) {
			validSubject = true;
		}
		else {
			cout << "There was an invalid character inputted...";
			validSubject = false;
		}
	}

	while (validContent != true) {
		cout << "Please enter the body of the message: " << endl << "Content: ";
		getline(cin, content, '\n');
		if (regex_match(content, allowedContent)) {
			validContent = true;
		}
		else {
			cout << "There was an invalid character inputted...";
			validContent = false;
		}
	}

	cout << "Thanks for the information, your post currently looks like this:" << endl << endl;
	cout << "Message " << messageID << " from " << posterUsername << ", user ID " << posterID << " posted " << postedTimeString(postedTime) << endl;
	cout << "Subject: " << subject << endl;
	cout << "Content: " << content << endl << endl;
	cout << "Would you like to post, edit or cancel?" << endl;

	while (validInput != true) {
		getline(cin, userInput, '\n');
		userInput = newMessageUser.inputFormatting(userInput);
		if (userInput == "post") {
			messageDatabase.saveNewMessage(*this);
			validInput = true;
		}
		else if (userInput == "edit") {
			composeMessage(newMessageUser, messageID);
			validInput = true;
		}
		else if (userInput == "cancel") {
			cout << endl;
			return;
		}
		else {
			cout << "That wasn't a valid input, please try again." << endl;
		}
	}
}

//converts a time_t value to a user readable string 
string Message::postedTimeString(time_t unformattedTime)
{
	char str[26];
	postedTime = unformattedTime;
	ctime_s(str, sizeof str, &postedTime);
	return str;
}

//prompts the user to enter edited message details
void Message::editMessage(User editMessageUser, int inputMessageID)
{
	bool validInput = false;
	bool validSubject = false;
	bool validContent = false;
	string userInput = "";
	string inputSubject = "";
	string inputContent = "";
	messageID = inputMessageID;
	posterID = editMessageUser.getUserID();
	posterUsername = editMessageUser.getUsername();
	postedTime = time(NULL);
	const regex allowedSubject("^[-!/\\+.,?:@#()& A-Za-z0-9]+$");
	const regex allowedContent("^[-!/\\+.,?:@#()& \t A-Za-z0-9]+$");

	system("CLS");
	cout << "Here is the message that you would like to edit: " << endl;
	loadMessage(inputMessageID);
		
	cout << "This message will be ID " << messageID << endl;
	cout << "Currently the subject is '" << subject << "'. Type a new one or press enter to keep the same subject:" << endl;
	while (validSubject != true) {
		cout << "Please enter the subject of the message: " << endl << "Subject: ";
		getline(cin, inputSubject, '\n');
		if (regex_match(inputSubject, allowedSubject)) {
			subject = inputSubject;
			validSubject = true;
		}
		else if (inputSubject.empty()) {
			validSubject = true;
		}
		else {
			cout << "There was an invalid character inputted...";
			validSubject = false;
		}
	}
	cout << "Currently the content is '" << content << "'. Type a new one or press enter to keep the same subject: " << endl;
	while (validContent != true) {
		cout << "Please enter the body of the message: " << endl << "Content: ";
		getline(cin, inputContent, '\n');
		if (regex_match(inputContent, allowedContent)) {
			content = inputContent;
			validContent = true;
		}
		else if (inputSubject.empty()) {
			validContent = true;
		}
		else {
			cout << "There was an invalid character inputted...";
			validContent = false;
		}
	}

	cout << "Thanks for the information, your updated post looks like this:" << endl << endl;
	cout << "Message " << messageID << " from " << posterUsername << ", user ID " << posterID << " posted " << postedTimeString(postedTime) << endl;
	cout << "Subject: " << subject << endl;
	cout << "Content: " << content << endl << endl;
	cout << "Would you like to post, edit or cancel?" << endl;

	while (validInput != true) {
		getline(cin, userInput, '\n');
		userInput = editMessageUser.inputFormatting(userInput);
		if (userInput == "post") {
			messageDatabase.saveEditedMessage(*this);
			validInput = true;
		}
		else if (userInput == "edit") {
			composeMessage(editMessageUser, messageID);
			validInput = true;
		}
		else if (userInput == "cancel") {
			cout << endl;
			return;
		}
		else {
			cout << "That wasn't a valid input, please try again." << endl;
		}
	}
}

//returns message id
int Message::getMessageID()
{
	return messageID;
}

//returns poster id
int Message::getPosterID()
{
	return posterID;
}

//returns view count
int Message::getViewCount()
{
	return viewCount;
}

//returns subject
string Message::getSubject()
{
	return subject;
}

//returns content
string Message::getContent()
{
	return content;
}

//returns poster username
string Message::getPosterUsername()
{
	return posterUsername;
}

//returns posted time
time_t Message::getPostedTime()
{
	return postedTime;
}

//loads a message's information to the current object
bool Message::loadMessage(int id)
{
	vector<string> tempVector = messageDatabase.loadMessage(id);
	
	messageID = stoi(tempVector[0]);
	posterID = stoi(tempVector[1]);
	subject = tempVector[2];
	content = tempVector[3];
	posterUsername = tempVector[4];
	postedTime = stoi(tempVector[5]);
	return false;
}
