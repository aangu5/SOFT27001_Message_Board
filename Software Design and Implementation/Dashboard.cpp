#include <stdio.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <regex>
#include "sqlite3.h"
#include "Dashboard.h"
#include "Message.h"
#include "Database.h"
using namespace std;

Message message;
bool initialOpen = true;
Database database;

//opens the database and initialises the tables
bool Dashboard::openDashboard()
{
    if (database.openDatabase() && database.initialDatabase()) {
        return true;
    }
    else {
        return false;
    }
}

//offers the user the unauthenticated options 
void Dashboard::mainMenu()
{
    User currentUser;
    string userInput;
    bool validInput = false;

    cout << "Welcome to the Atlantic Railway Corporation's Message Board. Here is a list of available commands." << endl << endl;
    cout << "register \t Creates a new user." << endl;
    cout << "login \t\t Opens the login page." << endl;
    cout << "help \t\t Shows the list of available commands" << endl;
    cout << "exit \t\t Closes the program." << endl << endl;
    cout << "Would you like to login or register? " << endl;

    while (validInput != true) {
        cout << "ARCMB>";
        getline(cin, userInput, '\n');
        userInput = currentUser.inputFormatting(userInput);
        if (userInput == "register") {
            validInput = true;
            currentUser.newUser(database.getNumberOfUsers());
            homepage(currentUser);
        }
        else if (userInput == "login") {
            validInput = true;
            if (currentUser.userLogin()) {
                homepage(currentUser);
            }
            else {
                mainMenu();
            }
        }
        else if (userInput == "exit") {
            exitProgram();
        }
        else if (userInput.empty()) {
        }
        else if (userInput == "help") {
            validInput = false;
            cout << "Here are the available commands: " << endl << endl;
            cout << "register \t Creates a new user." << endl;
            cout << "login \t\t Opens the login page." << endl;
            cout << "help \t\t Shows the list of available commands" << endl;
            cout << "exit \t\t Closes the program." << endl << endl;
        }
        else {
            validInput = false;
            cout << "'" << userInput << "' is not recognised as an internal or external command." << endl;
        }
    }
}

//offers the user the homepage menu
void Dashboard::homepage(User homepageUser)
{
    Message newMessage;
    string userInput = "";
    bool exitInput = false;
    const regex messages("^messages$|^m$");
    const regex messagesNew("^messages /new$|^m /n$");
    const regex messagesMine("^messages /mine$|^m /m$");
    const regex messagesNewest("^messages /newest$|^m /new$");
    const regex messagesOldest("^messages /oldest$|^m /old$");
    const regex messagesShowLong("^messages /show [0-9]+$");
    const regex messagesShowShort("^m /sh [0-9]+$");
    const regex messagesUserLong("^messages /user [0-9]+$");
    const regex messagesUserShort("^m /u [0-9]+$");
    const regex messagesSearchContentLong("^messages /searchcontent [a-zA-z0-9 ]+$");
    const regex messagesSearchContentShort("^m /sc [a-zA-z0-9 ]+$");
    const regex messagesSearchSubjectLong("^messages /searchsubject [a-zA-z0-9 ]+$");
    const regex messagesSearchSubjectShort("^m /ss [a-zA-z0-9 ]+$");
    const regex messagesSearchAllLong("^messages /searchall [a-zA-z0-9 ]+$");
    const regex messagesSearchAllShort("^m /sa [a-zA-z0-9 ]+$");
    const regex messagesEditLong("^messages /edit [0-9]+$");
    const regex messagesEditShort("^m /e [0-9]+$");
    const regex messagesDeleteLong("^messages /delete [0-9]+$");
    const regex messagesDeleteShort("^m /d [0-9]+$");
    const regex messagesStatistics("^messages /statistics$|^m /stat$");
    const regex home("^home$|^ho$");
    const regex help("^help$|^he$");
    const regex helpMessages("^help /messages$|^he /m$");
    const regex logout("^logout$|^l$");
    const regex exit("^exit$|^e$");

    cout << "HOMEPAGE - " << newMessage.postedTimeString(time(NULL)) << homepageUser.getUsername() << " - Welcome to the homepage! Type help for a list of available commands " << endl << endl;

    if (initialOpen) {
        cin.ignore();
        initialOpen = false;
    }
    while (exitInput != true) {
        cout << "ARCMB\\User\\" << homepageUser.getUsername() << ">";
        getline(cin, userInput, '\n');
        userInput = homepageUser.inputFormatting(userInput);

        if (regex_match(userInput, messagesNew)) {
            newMessage.composeMessage(homepageUser, database.getNumberOfMessages() + 1);
            database.messagePosted(homepageUser);
            homepage(homepageUser);
        }
        else if (regex_match(userInput, messages)) {
            database.getAllMessages("MessageID ASC");
        }
        else if (regex_match(userInput, messagesMine)) {
            database.getUsersMessages(homepageUser.getUserID());
        }
        else if (regex_match(userInput, messagesOldest)) {
            database.getAllMessages("Time DESC");
        }
        else if (regex_match(userInput, messagesNewest)) {
            database.getAllMessages("Time ASC");
        }
        else if (regex_match(userInput, messagesUserLong)) {
            selectedAuthor = 0;
            selectedAuthor = stoi(userInput.substr(15, userInput.length()));
            database.getUsersMessages(selectedAuthor);
        }
        else if (regex_match(userInput, messagesUserShort)) {
            selectedAuthor = 0;
            selectedAuthor = stoi(userInput.substr(5, userInput.length()));
            database.getUsersMessages(selectedAuthor);
        }
        else if (regex_match(userInput, messagesShowLong)) {
            selectedMessage = 0;
            selectedMessage = stoi(userInput.substr(15, userInput.length()));
            database.getSelectedMessage(selectedMessage);
        }
        else if (regex_match(userInput, messagesShowShort)) {
            selectedMessage = 0;
            selectedMessage = stoi(userInput.substr(6, userInput.length()));
            database.getSelectedMessage(selectedMessage);
        }
        else if (regex_match(userInput, messagesSearchContentLong)) {
            searchTerm = "";
            searchTerm = userInput.substr(25, userInput.length());
            database.searchByContent(searchTerm);
        }
        else if (regex_match(userInput, messagesSearchContentShort)) {
            searchTerm = "";
            searchTerm = userInput.substr(6, userInput.length());
            database.searchByContent(searchTerm);
        }
        else if (regex_match(userInput, messagesSearchSubjectLong)) {
            searchTerm = "";
            searchTerm = userInput.substr(25, userInput.length());
            database.searchBySubject(searchTerm);
        }
        else if (regex_match(userInput, messagesSearchSubjectShort)) {
            searchTerm = "";
            searchTerm = userInput.substr(6, userInput.length());
            database.searchBySubject(searchTerm);
        }
        else if (regex_match(userInput, messagesSearchAllLong)) {
            searchTerm = "";
            searchTerm = userInput.substr(21, userInput.length());
            database.searchAll(searchTerm);
        }
        else if (regex_match(userInput, messagesSearchAllShort)) {
            searchTerm = "";
            searchTerm = userInput.substr(6, userInput.length());
            database.searchAll(searchTerm);
        }
        else if (regex_match(userInput, messagesEditLong)) {
            messageToEdit = 0;
            messageToEdit = stoi(userInput.substr(15, userInput.length()));

            if (database.getMessageAuthor(messageToEdit) == homepageUser.getUserID()) {
                message.editMessage(homepageUser, messageToEdit);
            }
            else {
                cout << "You are only able to edit messages that you have posted." << endl;
            }
        }
        else if (regex_match(userInput, messagesEditShort)) {
            messageToEdit = 0;
            messageToEdit = stoi(userInput.substr(5, userInput.length()));

            if (database.getMessageAuthor(messageToEdit) == homepageUser.getUserID()) {
                message.editMessage(homepageUser, messageToEdit);
            }
            else {
                cout << "You are only able to edit messages that you have posted." << endl;
            }
        }
        else if (regex_match(userInput, messagesDeleteLong)) {
            messageToDelete = 0;
            messageToDelete = stoi(userInput.substr(17, userInput.length()));

            if (database.getMessageAuthor(messageToDelete) == homepageUser.getUserID()) {
                string deletionInput = "";
                cout << "Are you sure that you would like to delete this message? " << endl;
                cin >> deletionInput;
                deletionInput = homepageUser.inputFormatting(deletionInput);
                if (deletionInput == "yes") {
                    database.deleteMessage(homepageUser.getUserID(), messageToDelete);
                    homepage(homepageUser);
                }
            }
            else {
                cout << "You are only able to delete messages that you have posted." << endl;
            }
        }
        else if (regex_match(userInput, messagesDeleteShort)) {
            messageToDelete = 0;
            messageToDelete = stoi(userInput.substr(5, userInput.length()));

            if (database.getMessageAuthor(messageToDelete) == homepageUser.getUserID()) {
                string deletionInput = "";
                cout << "Are you sure that you would like to delete this message? " << endl;
                cin >> deletionInput;
                deletionInput = homepageUser.inputFormatting(deletionInput);
                if (deletionInput == "yes") {
                    database.deleteMessage(homepageUser.getUserID(), messageToDelete);
                    homepage(homepageUser);
                }
            }
            else {
                cout << "You are only able to delete messages that you have posted." << endl;
            }
        }
        else if (regex_match(userInput, home)) {
            system("CLS");
            homepage(homepageUser);
        }
        else if (regex_match(userInput, help)) {
            cout << "Here are the available commands: " << endl << endl <<
                "Command \t Shortcut \t Description" << endl <<
                "messages \t m \t\t View all messages. Parameters available under \"help /messages\"." << endl <<
                "home \t\t ho \t\t Returns to the Homepage. " << endl <<
                "help \t\t he \t\t Shows the Help Menu. " << endl <<
                "help /messages \t he /m \t\t Shows help for the messages command." << endl <<
                "logout \t\t l \t\t Logs the current user out. " << endl <<
                "exit \t\t e \t\t Exits the program. " << endl << endl;
        }
        else if (regex_match(userInput, helpMessages)) {
        cout << "Here are the optional parameters " << endl << endl <<
            "Command                        Shorthand               Description" << endl <<
            "messages                           m                   view all messages" << endl <<
            "messages /new                      m /n                new message" << endl <<
            "messages /mine                     m /m                my messages" << endl <<
            "messages /oldest                   m /old              gets oldest messages" << endl <<
            "messages /newest                   m /new              gets newest messages" << endl <<
            "messages /show [messageID]         m /sh [messageID]   shows message with messageID" << endl <<
            "messages /user [userID]            m /u [userID]       shows userID's messages" << endl <<
            "messages /searchContent [string]   m /sc [string]      search content for string" << endl <<
            "messages /searchSubject [string]   m /ss [string]      search subject for string" << endl <<
            "messages /searchAll [string]       m /sa [string]      search all for string" << endl <<
            "messages /edit [messageID]         m /e [messageID]    edit message with messageID" << endl <<
            "messages /delete [messageID]       m /d [messageID]    deletes message with messageID" << endl <<
            "messages /statistics               m /stat             shows statistics" << endl << endl;
        }
        else if (regex_match(userInput, messagesStatistics)) {
            getStatisticsDashboard();
        }
        else if (regex_match(userInput, logout)) {
            database.logOut();
            mainMenu();
            exitInput = true;
            return;
        }
        else if (regex_match(userInput, exit)) {
        exitInput = true;
        exitProgram();
        }
        else if (userInput.empty()) {

        }
        else {
            exitInput = false;
            cout << "'" << userInput << "' is not recognised as an internal or external command." << endl;
        }
    }
}

//shows the user the statistics about the dashboard
void Dashboard::getStatisticsDashboard()
{
    system("CLS");
    cout << "Here are the available statistics for this message board: " << endl << endl;
    cout << "Current time: " << message.postedTimeString(time(NULL));
    cout << "Number of messages: " << database.getNumberOfMessages() << endl;
    cout << "Number of users: " << database.getNumberOfUsers() << endl;
    cout << "Most recent message: " << database.getMostRecentMessage() << endl;
    cout << "Most active author: " << database.getMostActiveAuthor() << endl;
    cout << "Most viewed message: " << database.getMostViewedMessage() << endl << endl;
    return;
}

//closes the program if the logout functionality is successful
void Dashboard::exitProgram()
{
    if (database.logOut()) {
        database.closeDatabase();
        exit(0);
    }
}



