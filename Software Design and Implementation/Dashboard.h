#pragma once
#include "User.h"
#include <regex>
using namespace std;

class Dashboard
{
private:
	int selectedAuthor = 0;
	int messageToEdit = 0;
	int messageToDelete = 0;
	int selectedMessage = 0;
	int mostRecentMessage = 0;
	string searchTerm = "";
	void homepage(User homepageUser);
	void getStatisticsDashboard();
	void exitProgram();

public:
	bool openDashboard();
	void mainMenu();
};

