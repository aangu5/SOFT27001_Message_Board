#pragma once
#include <iostream>
#include <string>
#include <ctime>
#include "User.h"

using namespace std;

class Message
{
private:
	int messageID = 1;
	int posterID = 0;
	int viewCount = 0;
	string subject = "";
	string content = "";
	string posterUsername = "";
	time_t postedTime = 0;
public:
	void composeMessage(User newMessageUser, int messageIDToBe);
	string postedTimeString(time_t unformattedTime);
	void editMessage(User editMessageUser, int inputMessageID);
	int getMessageID();
	int getPosterID();
	int getViewCount();
	string getSubject();
	string getContent();
	string getPosterUsername();
	time_t getPostedTime();
	bool loadMessage(int id);
};