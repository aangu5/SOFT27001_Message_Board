#include "Database.h"
#include "User.h"
#include <iostream>
#include <ctime>
#include <string>
#include <vector>
Dashboard tempDashboard;
using namespace std;

int tempCountOfMessages = 0;
int highestTime = 0;
int highestMessage;
int tempMostActiveAuthor = 0;
int mostMessages = 0;
int tempAuthor = 0;
int tempCountOfUsers = 0;
int i = 1;
const int maxUserCount = 100;
string existingNames[maxUserCount];
int userLoopCount = 0;
string storedPassword = "";
vector<string> userDetails;
vector<string> messageDetails;
vector<int> messageStatistics;
User tempUser;
Message tempMessage;

//opens the database and creates the tables necessary
bool Database::openDatabase()
{
    int exec;
    string SQL = "CREATE TABLE IF NOT EXISTs Users (UserID int PRIMARY KEY, Username varchar(255), Password varchar(255), EmailAddress varchar(255), MessageCount int, LoggedIn boolean); " \
        "CREATE TABLE IF NOT EXISTS Messages (MessageID int PRIMARY KEY, PosterID int, Subject varchar(255), Content varchar(255), PosterName varchar(255), Time bigint, ViewCount int); " \
        "CREATE TABLE IF NOT EXISTS Dashboard (StartupTime bigint PRIMARY KEY, NumberofPosts int, MostRecentPost int, MostActiveAuthor int, NumberofUsers int);" \
        "INSERT OR REPLACE INTO Users VALUES(1, 'admin', 'ChangeMe!', 'admin@arc.net', 1, 0);" \
        "INSERT OR REPLACE INTO Messages VALUES(1, 1, 'Welcome!', 'Hello and Welcome to ARCMB, Atlantic Railway Corporation''s new Message Board!', 'admin',  1586390400, 0);";

    exec = sqlite3_exec(db, SQL.c_str(), NULL, 0, &errorMessage);
    if (exec != SQLITE_OK) {
        cout << "There has been a SQL error: " << errorMessage;
        return false;
    }
    else {
        return true;
    }
}

//adds the admin user and initial welcome message to the database so the statistics functions work correctly, otherwise there are callback errors
bool Database::initialDatabase()
{
    SQL = "INSERT INTO Dashboard VALUES(" + to_string(time(NULL)) + ", " + to_string(getNumberOfMessages()) + ", " + to_string(getMostRecentMessage()) + ", " + to_string(getMostActiveAuthor()) + ", " + to_string(getNumberOfUsers()) + "); ";
    exec = sqlite3_exec(db, SQL.c_str(), NULL, 0, &errorMessage);
    if (exec != SQLITE_OK) {
        cout << "There has been a SQL error: " << errorMessage;
        return false;
    }
    else {
        return true;
    }
}

//callback function for the getNumberOfMessages() function
int Database::callbackMessages(void* data, int argc, char** argv, char** azColName)
{
    tempCountOfMessages = stoi(argv[0]);
    return 0;
}

//callback function which prints out messages to the console
int Database::callbackGetMessages(void* data, int argc, char** argv, char** azColName)
{
    cout << endl << "Message " << argv[0] << " from " << argv[4] << ", user ID " << argv[1] << " posted " << tempMessage.postedTimeString(atoi(argv[5])) << "Views: " << argv[6] << endl << endl;
    cout << "Subject: " << argv[2] << endl;
    cout << "Content: " << argv[3] << endl;
    return 0;

}

//callback function for the getNumberOfUsers() function
int Database::callbackUsers(void* data, int argc, char** argv, char** azColName)
{
    tempCountOfUsers = stoi(argv[0]);
    return 0;
}

//callback function for the getMostRecentMessage() function
int Database::callbackRecentMessage(void* data, int argc, char** argv, char** azColName)
{
    if (stoi(argv[1]) > highestTime) {
        highestMessage = stoi(argv[0]);
        highestTime = stoi(argv[1]);
    }
    return 0;
}

//callback function for the getMostActiveAuthor() function
int Database::callbackMostActiveAuthor(void* count, int argc, char** argv, char** azColName)
{
    tempMostActiveAuthor = stoi(argv[1]);
    return 0;
}

//callback function for the getAuthor() function
int Database::callbackGetAuthor(void* data, int argc, char** argv, char** azColName)
{
    tempAuthor = stoi(argv[0]);
    return 0;
}

//standard callback function
int Database::callbackStandard(void* data, int argc, char** argv, char** azColName)
{
    fprintf(stderr, "%s: ", (const char*)data);

    for (int i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

//callback function to get list of usernames 
int Database::callbackUsername(void* data, int argc, char** argv, char** azColName)
{
    for (int i = 0; i < argc; i++) {
        existingNames[userLoopCount] = (string)argv[i];
    }
    userLoopCount++;
    return 0;
}

//callback function to get a user's hashed password
int Database::callbackPassword(void* data, int argc, char** argv, char** azColName)
{
    storedPassword = (string)argv[0];
    return 0;
}

//callback function to load a user's details
int Database::callbackLoadDetails(void* data, int argc, char** argv, char** azColName)
{
    for (int i = 0; i < argc; i++) {
        if (i == 0) {
            userDetails.push_back(argv[0]);
        }
        if (i == 1) {
            userDetails.push_back(argv[1]);
        }
        else if (i == 3) {
            userDetails.push_back(argv[3]);
        }
        else if (i == 4) {
            userDetails.push_back(argv[4]);
        }
    }
    return 0;
}

//callback function to load a message's details
int Database::callbackLoadMessage(void* data, int argc, char** argv, char** azColName)
{
    for (int i = 0; i < argc; i++) {
        messageDetails.push_back(argv[i]);
    }
    return 0;
}

//callback function to get the most read message
int Database::callbackMostReadMessage(void* data, int argc, char** argv, char** azColName)
{
    for (int i = 0; i < argc; i++) {
        messageStatistics.push_back(stoi(argv[i]));
    }
    return 0;
}


//retrieves the number of messages from the database
int Database::getNumberOfMessages()
{
    SQL = "SELECT MAX(MessageID) from Messages;";
    sqlite3_exec(db, SQL.c_str(), callbackMessages, 0, 0);
    messageCount = tempCountOfMessages;
    tempCountOfMessages = 0;
    return messageCount;
}

//retrieves the most recent from the database
int Database::getMostRecentMessage()
{
    int table = 0;
    SQL = "SELECT MessageID,Time from Messages;";
    sqlite3_exec(db, SQL.c_str(), callbackRecentMessage, 0, 0);
    return highestMessage;
}

//retrieves the most active author from the database
int Database::getMostActiveAuthor()
{
    SQL = "SELECT UserID,MAX(MessageCount) FROM Users;";
    sqlite3_exec(db, SQL.c_str(), callbackMostActiveAuthor, 0, 0);
    mostActiveAuthor = tempMostActiveAuthor;
    tempMostActiveAuthor = 0;
    return mostActiveAuthor;
}

//retrieves all messages from the database
void Database::getAllMessages(string direction)
{

    system("CLS");
    cout << "Here are all the messages available:" << endl;
    SQL = "UPDATE Messages SET ViewCount = ViewCount +  1; SELECT * from Messages ORDER BY " + direction + ";";
    sqlite3_exec(db, SQL.c_str(), callbackGetMessages, 0, 0);
    return;
}

//retrieves a user's messages from the database
void Database::getUsersMessages(int userID)
{
    system("CLS");
    cout << "Here are all the messages posted by User " << userID << ": " << endl;
    SQL = "UPDATE Messages SET ViewCount = ViewCount +  1 WHERE PosterID = " + to_string(userID) + "; SELECT * from Messages WHERE PosterID = " + to_string(userID) + " ;";
    sqlite3_exec(db, SQL.c_str(), callbackGetMessages, 0, 0);
    return;
}

//retrieves the selected message from the database
void Database::getSelectedMessage(int messageID)
{
    system("CLS");
    cout << "Here is message " << messageID << ":" << endl;
    SQL = "UPDATE Messages SET ViewCount = ViewCount +  1 WHERE MessageID = " + to_string(messageID) + "; SELECT * from Messages WHERE MessageID = " + to_string(messageID) + " ;";
    sqlite3_exec(db, SQL.c_str(), callbackGetMessages, 0, 0);
    return;
}

//retrieves the author of a message from the database
int Database::getMessageAuthor(int messageID)
{
    system("CLS");
    SQL = "SELECT PosterID FROM Messages WHERE MessageID = " + to_string(messageID) + ";";
    sqlite3_exec(db, SQL.c_str(), callbackGetAuthor, 0, 0);
    messageAuthor = tempAuthor;
    tempAuthor = 0;
    return messageAuthor;
}

//searches message content for a string
void Database::searchByContent(string searchContentInput)
{
    system("CLS");
    searchContentInput = messageFormatting(searchContentInput);
    SQL = "UPDATE Messages SET ViewCount = ViewCount +  1 WHERE Content LIKE '%" + searchContentInput + "%'; SELECT * FROM Messages WHERE Content LIKE '%" + searchContentInput + "%';";
    sqlite3_exec(db, SQL.c_str(), callbackGetMessages, 0, 0);
    return;
}

//searches message subject for a string
void Database::searchBySubject(string searchSubjectInput)
{
    system("CLS");
    searchSubjectInput = messageFormatting(searchSubjectInput);
    SQL = "UPDATE Messages SET ViewCount = ViewCount +  1 WHERE Subject LIKE '%" + searchSubjectInput + "%'; SELECT * FROM Messages WHERE Subject LIKE '%" + searchSubjectInput + "%';";
    sqlite3_exec(db, SQL.c_str(), callbackGetMessages, 0, 0);
    return;
}

//searches message content, subject and user's names for a string
void Database::searchAll(string searchAllInput)
{
    system("CLS");
    searchAllInput = messageFormatting(searchAllInput);
    SQL = "UPDATE Messages SET ViewCount = ViewCount +  1 WHERE Content LIKE '%" + searchAllInput + "%' OR Subject LIKE '%" + searchAllInput + "%' OR PosterName Like '%" + searchAllInput + "%'; SELECT * FROM Messages WHERE Subject LIKE '%" + searchAllInput + "%' OR Content LIKE '%" + searchAllInput + "%'OR PosterName Like '%" + searchAllInput + "%';";
    sqlite3_exec(db, SQL.c_str(), callbackGetMessages, 0, 0);
    return;
}

//deletes a message from the database
void Database::deleteMessage(int userID, int messageID)
{
    system("CLS");
    cout << "Message deleted, here are your remaining messages:" << endl;
    SQL = "DELETE FROM Messages WHERE MessageID = " + to_string(messageID) + "; UPDATE MessageCount = MessageCount - 1 WHERE UserID = " + to_string(userID) + "; SELECT * FROM Messages WHERE PosterID = " + to_string(userID) + ";";
    sqlite3_exec(db, SQL.c_str(), callbackGetMessages, 0, 0);
    return;
}

//gets the number of users from the database
int Database::getNumberOfUsers()
{
    sqlite3_exec(db, "SELECT MAX(UserID) from Users", callbackUsers, 0, 0);
    userCount = tempCountOfUsers;
    tempCountOfUsers = 0;
    return userCount;
}

//checks that a user's input name does not match an existing username
bool Database::checkUsername(string inputUsername)
{
    SQL = "SELECT Username FROM Users";
    sqlite3_exec(db, SQL.c_str(), callbackUsername, 0, &errorMessage);
    for (int u = 0; u < userLoopCount; u++) {
        if (inputUsername == existingNames[u]) {
            cout << "This name is already taken..." << endl;
            return false;
        }
        else {
            return true;
        }
    }

    return false;
}

//checks that a user has entered a correct username and password combo
bool Database::validCredentials(string inputUsername, unsigned int inputPassword)
{
    SQL = "SELECT Password FROM Users WHERE Username = \"" + inputUsername + "\";";
    sqlite3_exec(db, SQL.c_str(), callbackPassword, 0, &errorMessage);
    if (storedPassword == to_string(inputPassword)) {
        cout << endl << "Your password was correct, welcome!" << endl;
        return true;
    }
    else {
        return false;
    }
}

//loads the user from the database through a vector
vector<string> Database::loadUser(string tempUsername)
{
    string SQL = "UPDATE Users set LoggedIn = true where Username = \"" + tempUsername + "\"; SELECT * FROM Users WHERE Username = \"" + tempUsername + "\";";
    sqlite3_exec(db, SQL.c_str(), callbackLoadDetails, 0, &errorMessage);
    return userDetails;
}

//loads the message from the database through a vector
vector<string> Database::loadMessage(int messageID)
{
    string SQL = "SELECT * FROM Messages WHERE MessageID = \"" + to_string(messageID) + "\";";
    sqlite3_exec(db, SQL.c_str(), callbackLoadMessage, 0, &errorMessage);
    return messageDetails;
}

//saves the user's information to the database
bool Database::saveUser(User userToSave)
{
    SQL = "INSERT INTO Users VALUES(" + to_string(userToSave.getUserID()) + ", '" + userToSave.getUsername() + "', '" + to_string(userToSave.getPassword()) + "', '" + userToSave.getUserEmail() + "', 0, 0)";
    int rc = sqlite3_exec(db, SQL.c_str(), NULL, 0, &errorMessage);
    if (rc != SQLITE_OK) {
        cout << "There has been a database error: " << errorMessage << endl;
        return false;
    }
    else {
        return true;
    }
}

//saves a new message to the database
void Database::saveNewMessage(Message messageToSave)
{
    string tempSubject = messageFormatting(messageToSave.getSubject());
    string tempContent = messageFormatting(messageToSave.getContent());

    SQL = "INSERT INTO Messages VALUES(" + to_string(messageToSave.getMessageID()) + ", " + to_string(messageToSave.getPosterID()) + ", '" + tempSubject + "', '" + tempContent + "', '" + messageToSave.getPosterUsername() + "', " + to_string(messageToSave.getPostedTime()) + ", " + to_string(messageToSave.getViewCount()) + ");";
    exec = sqlite3_exec(db, SQL.c_str(), 0, 0, &errorMessage);

    if (exec != SQLITE_OK) {
        cout << "There has been a database error: " << errorMessage << endl;
    }
    else {
        cout << "Message saved succesfully!" << endl;

        return;
    }
}

//saves an edited message to the database
void Database::saveEditedMessage(Message messageToEdit)
{
    string tempSubject = messageFormatting(messageToEdit.getSubject());
    string tempContent = messageFormatting(messageToEdit.getContent());

    SQL = "UPDATE Messages SET Subject = '" + tempSubject + "', Content = '" + tempContent + "', Time = " + to_string(messageToEdit.getPostedTime()) + " WHERE MessageID = " + to_string(messageToEdit.getMessageID()) + ";";
    exec = sqlite3_exec(db, SQL.c_str(), 0, 0, &errorMessage);
    if (exec != SQLITE_OK) {
        cout << "There has been a database error: " << errorMessage << endl;
    }
    else {
        cout << "Message saved succesfully!" << endl;

        return;
    }
}

//adds 1 to the user's MessageCount column in the database
void Database::messagePosted(User author)
{
    SQL = "UPDATE Users set MessageCount = MessageCount + 1 where Username = '" + author.getUsername() + "';";
    sqlite3_exec(db, SQL.c_str(), NULL, 0, 0);
    return;
}

//logs out all users
bool Database::logOut()
{
    SQL = "UPDATE Users set LoggedIn = false;";
    exec = sqlite3_exec(db, SQL.c_str(), callbackStandard, 0, &errorMessage);
    if (exec != SQLITE_OK) {
        cout << "There has been a database error: " << errorMessage << endl;
        return false;
    }
    else {
        system("CLS");
        return true;
    }
}

//formats input for text storage in database, escapes ' characters to prevent SQL injection
string Database::messageFormatting(string inputString)
{
    string outputString = "";

    for (char letter : inputString) {
        if (letter == '\'') {
            outputString = outputString + '\'' + letter;
        }
        else {
            outputString += letter;
        }
    }
    return outputString;
}

//gets the most viewed message from the databse
int Database::getMostViewedMessage()
{
    sqlite3_exec(db, "SELECT messageID,MAX(ViewCount) from Messages", callbackMostReadMessage, 0, 0);
    mostViewedMessage = messageStatistics[0];
    return mostViewedMessage;
}

//closes the database
bool Database::closeDatabase()
{
    exec = sqlite3_exec(db, SQL.c_str(), callbackStandard, 0, &errorMessage);
    if (exec != SQLITE_OK) {
        cout << "There has been a database error: " << errorMessage << endl;
        return false;
    }
    else {
        system("CLS");
        return true;
    }
}