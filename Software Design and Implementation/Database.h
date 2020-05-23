#pragma once
#include "sqlite3.h"
#include "Dashboard.h"
#include "Message.h"
#include <string>

class Database
{
private:
	sqlite3* db;
	int open = sqlite3_open("ARCMBDB.db", &db);
	string SQL;
	int exec = 0;
	char* errorMessage = NULL;
	int userCount = 0;
	int messageCount = 0;
	int mostActiveAuthor = 0;
	int messageAuthor = 0;
	int mostViewedMessage = 0;

	static int callbackMessages(void* data, int argc, char** argv, char** azColName);
	static int callbackGetMessages(void* data, int argc, char** argv, char** azColName);
	static int callbackUsers(void* data, int argc, char** argv, char** azColName);
	static int callbackRecentMessage(void* data, int argc, char** argv, char** azColName);
	static int callbackMostActiveAuthor(void* data, int argc, char** argv, char** azColName);
	static int callbackGetAuthor(void* data, int argc, char** argv, char** azColName);
	static int callbackStandard(void* data, int argc, char** argv, char** azColName);
	static int callbackUsername(void* data, int argc, char** argv, char** azColName);
	static int callbackPassword(void* data, int argc, char** argv, char** azColName);
	static int callbackLoadDetails(void* data, int argc, char** argv, char** azColName);
	static int callbackLoadMessage(void* data, int argc, char** argv, char** azColName);
	static int callbackMostReadMessage(void* data, int argc, char** argv, char** azColName);

public:
	bool openDatabase();
	bool initialDatabase();
	int getNumberOfMessages();
	int getMostRecentMessage();
	int getMostActiveAuthor();
	void getAllMessages(string direction);
	void getUsersMessages(int userID);
	void getSelectedMessage(int messageID);
	int getMessageAuthor(int messageID);
	void searchByContent(string searchContentInput);
	void searchBySubject(string searchSubjectInput);
	void searchAll(string searchAllInput);
	void deleteMessage(int userID, int messageID);
	int getNumberOfUsers();
	bool checkUsername(string inputUsername);
	bool validCredentials(string inputUsername, unsigned int inputPassword);
	vector<string> loadUser(string tempUsername);
	vector<string> loadMessage(int messageID);
	bool saveUser(User userToSave);
	void saveNewMessage(Message messageToSave);
	void saveEditedMessage(Message messageToEdit);
	void messagePosted(User author);
	bool logOut();
	string messageFormatting(string inputString);
	int getMostViewedMessage();
	bool closeDatabase();
};

