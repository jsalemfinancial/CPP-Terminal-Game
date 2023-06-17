#pragma once
#include <iostream>
#include <string>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>

#define STD_OUTPUT_HANDLE ((DWORD)-12)
#define FOREGROUND_RED 0x0004
#define FOREGROUND_BLUE 0x0001
#define FOREGROUND_GREEN 0x0002
#define FOREGROUND_INTENSITY 0x0008
#define FOREGROUND_BLACK 0x0000

#define VK_RETURN 0x0D

using namespace std;
HANDLE stdHandle;
HWND consoleControl = GetConsoleWindow(); //For launching the console in a certain size.

										  //Standard colorizer.
void colorPicker(int colorNumber) {

	stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	if (colorNumber == 1) {
		SetConsoleTextAttribute(stdHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	}

	else if (colorNumber == 2) {
		SetConsoleTextAttribute(stdHandle, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	}
	else if (colorNumber == 3) {
		SetConsoleTextAttribute(stdHandle, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
	}
	else if (colorNumber == 4) {
		SetConsoleTextAttribute(stdHandle, FOREGROUND_BLACK);
	}
	else {
		SetConsoleTextAttribute(stdHandle, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
	}

}
//Niffty buffer positioning function.
void pos(SHORT x, SHORT y) {
	SetConsoleCursorPosition(stdHandle, { x, y });
}
//My wanna-be true RNG.
int RNG(int limit) {
	MEMORYSTATUSEX memInfo;
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memInfo);
	DWORDLONG physMemUsed = memInfo.ullTotalPhys - memInfo.ullAvailPhys;

	//Code gained from: 
	/*https://stackoverflow.com/questions/63166/
	how-to-determine-cpu-and-memory-consumption-
	from-inside-a-process */

	int rnNum = (physMemUsed % limit);
	return rnNum;
}
//Keeps keystrokes from entering the buffer.
int keyPress() {
	int keyPress = _getwch();
	keyPress = toupper(keyPress);

	return keyPress;
}
//interprets the result and matches it with a key to avoid _getwch() repetition.
bool keyResult(int keyPress, int key) {
	if (toupper(key) == toupper(keyPress)) {
		return true;
	}
	else {
		return false;
	}
}
//Type and delayed display of file content.
void coolFileTextEffect(string chosenFile, float lineInterval, float letterInterval, int colorOne, int colorTwo) {
	ifstream inFS;
	string readerString;

	inFS.open(chosenFile);
	int i = 0;
	while (!inFS.eof()) {
		i = i % 2;
		if (i == 1) {
			colorPicker(colorOne);
		}
		else {
			colorPicker(colorTwo);
		}
		getline(inFS, readerString);
		Sleep(lineInterval * 1000);
		for (int n = 0; n < readerString.size(); n++) {
			Sleep(letterInterval * 1000);
			cout << readerString.at(n);
		}
		cout << endl;
		i++;
	}
	inFS.close();
}
//Displays the contents of files normally.
void fileTextDisplay(string chosenFile, int colorOne, int colorTwo) {
	ifstream inFS;
	string readerString;

	inFS.open(chosenFile);
	int i = 0;
	while (!inFS.eof()) {
		i = i % 2;
		if (i == 1) {
			colorPicker(colorOne);
		}
		else {
			colorPicker(colorTwo);
		}
		getline(inFS, readerString);
		cout << readerString << endl;
		i++;
	}
	inFS.close();
}
//Type-like text display.
void textWriter(string theString, float interval, int color) { //still need to call SetConsoleCursorPos beforehand.
	int i = 0;
	colorPicker(color);

	for (int n = 0; n < theString.size(); n++) {
		Sleep(interval * 1000);
		cout << theString.at(n);

	}
}
//Cooly flashy effect in the beginning (unstable.)
void textFlasherEnter(string theText, float interval, SHORT xPos, SHORT yPos, int colorOne, int colorTwo) {
	while (!GetAsyncKeyState(VK_RETURN)) {
		colorPicker(colorOne);
		pos(xPos, yPos);
		cout << theText;
		Sleep(interval * 1000);
		colorPicker(colorTwo);
		pos(xPos, yPos);
		cout << theText;
		Sleep(interval * 1000);

		if (GetAsyncKeyState(VK_RETURN)) {
			colorPicker(4);
			pos(xPos, yPos);
			cout << theText;
			Sleep(200);
			break;
		}

		cin.sync();
		cin.clear();

	}
}
//Replaces strings in a file with another string.
void fileTextReplacer(string fileName, int lineNum, int chunkIndex, int chunkEnd, string edit) {
	ifstream inFS;
	ofstream outFS;
	string readerString;

	int n = 0;
	string s;

	inFS.clear();
	inFS.sync();

	inFS.open(fileName);
	while (getline(inFS, s)) {
		n++;
	}
	inFS.close();

	vector<string> fileLines(n);

	inFS.open(fileName);
	int i = 0;
	while (i < n) {
		getline(inFS, readerString);
		fileLines.at(i) = readerString;
		i++;
	}
	inFS.close();

	fileLines.at(lineNum - 1).replace(chunkIndex, chunkEnd, edit);

	outFS.open(fileName);
	for (int i = 0; i < n; i++) {
		if (i < n && i != n - 1) {
			outFS << fileLines.at(i) << endl;
		}
		else if (i = n - 1) {
			outFS << fileLines.at(i);
		}
	}
	outFS.close();
}
//Attaches strings to the ends of strings in a file.
void fileTextAppender(string fileName, int lineNum, string edit) {
	ifstream inFS;
	ofstream outFS;
	string readerString;

	int n = 0;
	string s;
	inFS.open(fileName);
	while (getline(inFS, s)) {
		n++;
	}
	inFS.close();


	vector<string> fileLines(n);

	inFS.open(fileName);
	int i = 0;
	while (i < n) {
		getline(inFS, readerString);
		fileLines.at(i) = readerString;
		i++;
	}
	inFS.close();

	fileLines.at(lineNum - 1).append(edit);

	outFS.open(fileName);
	for (int i = 0; i < n; i++) {
		if (i < n && i != n - 1) {
			outFS << fileLines.at(i) << endl;
		}
		else {
			outFS << fileLines.at(i);
		}
	}
	outFS.close();
}
//Merges text files for easier horizontal drawing.
void fileMerger(string fileNameToCopy, string fileNameToWriteOver, string resultFile) {
	ifstream inFS;
	ofstream outFS;
	string readerString;

	int n = 0;
	string s;
	inFS.open(fileNameToCopy);
	while (getline(inFS, s)) {
		n++;
	}
	inFS.close();

	vector<string> fileTCLines(n);

	inFS.open(fileNameToCopy);
	int i = 0;
	while (i < n) {
		getline(inFS, readerString);
		fileTCLines.at(i) = readerString;
		i++;
	}
	inFS.close();

	/// phase 2:

	int q = 0;
	inFS.open(fileNameToWriteOver);
	while (getline(inFS, s)) {
		q++;
	}
	inFS.close();

	vector<string> fileTWOLines(q);

	inFS.open(fileNameToWriteOver);
	int f = 0;
	while (f < q) {
		getline(inFS, readerString);
		fileTWOLines.at(f) = readerString;
		f++;
	}
	inFS.close();

	for (int i = 0; i < q; i++) {
		fileTCLines.at(fileTCLines.size() - 1 - i) += fileTWOLines.at(fileTWOLines.size() - 1 - i);
	}

	outFS.open(resultFile);
	for (int i = 0; i < n; i++) {
		if (i < n && i != n - 1) {
			outFS << fileTCLines.at(i) << endl;
		}
		else {
			outFS << fileTCLines.at(i);
		}
	}
	outFS.close();
}
//gameplay functions:

//Keeps the main screen orderly, so things appear spaced and not stacked.
void kingdomLayoutKeeper(int castle, int church, int houses, float interval) {
	if (castle > 0) {
		coolFileTextEffect("Castle.txt", interval, 0, 2, 2);
	}
	if (church > 0 && houses > 0) {
		coolFileTextEffect("ChurchandHousesMix.txt", interval, 0, 2, 2);
	}
	else {
		if (church > 0) {
			coolFileTextEffect("Church.txt", interval, 0, 2, 2);
		}
		if (houses > 0) {
			coolFileTextEffect("House.txt", interval, 0, 2, 2);
		}
	}
}
//Church Events.
int churchEvents(int ducats) {
	int random = RNG(100);

	if (0 <= random && random < 15) {
		textWriter("You enter the church and notice the donation coffer unattended.", 0.07, 2); cout << endl;
		textWriter("Your kingly intuition tells you that it holds a nice sum of ducats.", 0.07, 2); cout << endl;
		textWriter("Attempt to steal them? (Y/N)", 0.07, 1); cout << endl;

		while (2 > 1) {
			int x = keyPress();
			if (keyResult(x, 'Y')) {
				int random = RNG(100);

				if (0 <= random && random < 65) {
					textWriter("You make away with 20 ducats without any of the bishops noticing.", 0.07, 2); cout << endl;
					textWriter("No one was going to miss these anyways. . .", 0.07, 2);
					return 1;
				}
				if (65 <= random && random < 100) {
					textWriter("A bishop catches your greedy hands in the coffer. He takes the nearest candlestick and beats you out of the church. You lose one health.", 0.07, 2); cout << endl;
					textWriter("Another tax free day for the church.", 0.07, 2);
					return 2;
				}
			}
			else if (keyResult(x, 'N')) {
				textWriter("You say to yourself \"Greed is a sin, you know?\"", 0.07, 2); cout << endl;
				textWriter("You walk out of the church after a short prayer.", 0.07, 2);
				Sleep(2000);
				return 3;
			}
		}
	}
	else if (15 <= random && random < 75) {
		textWriter("You step into the church and notice a congregation praying. You decide to join them in prayer. Afterall, the people must think you a pious leader!", 0.07, 2); cout << endl;
		textWriter("The time has come to make a donation. Will you spare 30 ducats to the good lord? (Y/N)", 0.07, 1); cout << endl;

		while (2 > 1) {
			int x = keyPress();
			if (keyResult(x, 'Y') && ducats >= 30) {
				textWriter("You begrudingly hand over 30 ducats to one of the deacons.", 0.07, 2); cout << endl;
				textWriter("At least you made a good example of yourself to the church.", 0.07, 2);
				return 4;
			}
			else if (keyResult(x, 'Y') && ducats < 30) {
				textWriter("You don't seem to have the funds the church is after.", 0.07, 2); cout << endl;

				int random = RNG(100);
				if (0 <= random && random < 15) {
					textWriter("A slight feeling of shame washes over you after the blunder.", 0.07, 2); cout << endl;
					textWriter("You leave after what seems like hours. Your knees are in pain.", 0.07, 2);
					Sleep(2000);
					return 5;
				}
				if (15 <= random && random < 100) {
					textWriter("The bishop shrugs and continues preaching.", 0.07, 2); cout << endl;
					textWriter("You leave after what seems like hours. Your knees are in pain.", 0.07, 2);
					Sleep(2000);
					return 6;
				}
			}

			if (keyResult(x, 'N')) {
				textWriter("The bishop leers at you from behind the altar. You can't help but feel uneasy.", 0.07, 2); cout << endl;
				textWriter("Maybe you should not have outright refused. . .", 0.07, 2); cout << endl;
				textWriter("You leave after what seems like hours. Your knees are in pain.", 0.07, 2);
				Sleep(2000);
				return 7;
			}
		}
	}
	else if (75 <= random && random < 100) {
		textWriter("After getting your fill of incense smoke, you exit the church.", 0.07, 2); cout << endl;
		textWriter("A group of peasants loiter outside the church doors.", 0.07, 2); cout << endl;
		textWriter("They see you and scream \"You're no king of mine!\"", 0.07, 2); cout << endl;
		textWriter("Suddenly, there is a volley of feces and junk coming your way, hitting both you and the church walls. They must be heretics. . .", 0.07, 2); cout << endl;
		textWriter("Execute them? (Y/N)", 0.07, 1); cout << endl;

		int x = keyPress();
		while (2 > 1) {
			if (keyResult(x, 'Y')) {
				textWriter("You order the execution of the peasants. The Church is pleased.", 0.07, 2); cout << endl;
				textWriter("However, this will probably not go unoticed by the village. . .", 0.07, 2);
				Sleep(2000);
				return 8;
			}
			if (keyResult(x, 'N') && ducats >= 18) {
				textWriter("You decide to be merciful, at least this time.", 0.07, 2); cout << endl;
				textWriter("Being such a wealthy king, you think you can please the group with some money.", 0.07, 2); cout << endl;
				textWriter("Pay the peasants about 18 ducats? (Y/N)", 0.07, 1); cout << endl;
				while (2 > 1) {
					int x = keyPress();
					if (keyResult(x, 'Y')) {
						textWriter("You hand over the sum of money to the peasants.", 0.07, 2); cout << endl;
						textWriter("One of them is so delighted, he kisses your hand with his blistened and pus-filled lips.", 0.07, 2); cout << endl;
						textWriter("You depart while furiously cleaning your hands.", 0.07, 2);
						Sleep(2000);
						return 9;
					}
					if (keyResult(x, 'N')) {
						textWriter("You depart from the church grounds. Leaving the peasants behind.", 0.07, 2);
						Sleep(2000);
						return 10;
					}
				}
			}
		}
	}
}
//Village events
int houseEvents(int ducats) {
	int random = RNG(100);

	if (0 <= random && random < 15) {
		textWriter("You decide to stroll through the village and interact with the commoners.", 0.07, 2); cout << endl;
		textWriter("A child runs up to you, exclaiming in joy. He tells everybody to have gifts at the ready!", 0.07, 2); cout << endl;
		textWriter("Do you accept them? (Y/N)", 0.07, 1); cout << endl;

		while (2 > 1) {
			int x = keyPress();
			if (keyResult(x, 'Y')) {
				int random = RNG(100);

				if (0 <= random && random < 65) {
					textWriter("You happily accept the gifts, and manage to sell them after everyone disperses.", 0.07, 2); cout << endl;
					textWriter("You gain 14 ducats!", 0.07, 2);
					Sleep(2000);
					return 1;
				}
				if (65 <= random && random < 100) {
					textWriter(" A peasant shouts \"Never can have enough, can ya mi'lord? Always stealing from the poor!\"", 0.07, 2); cout << endl;
					textWriter("He riles a crowd to attack you, and you barely manage to escape. Lose one health", 0.07, 2);
					Sleep(2000);
					return 2;
				}
			}
			else if (keyResult(x, 'N')) {
				textWriter("The child has a disappointed look in his eye as he manages to choke back a tear.", 0.07, 2); cout << endl;
				textWriter("The villagers understand that you refuse having their interests at heart.", 0.07, 2);
				Sleep(2000);
				return 3;
			}
		}
	}
	else if (15 <= random && random < 75) {
		textWriter("You roam the dilapidated village, and almost feel a sense of disgust from the atmosphere.", 0.07, 2); cout << endl;
		textWriter("As you maneuver around the mud and waste, you happen upon an elevated platform upon which you can make a speech.", 0.07, 2); cout << endl;
		textWriter("Do you address your people ? (Y/N)", 0.07, 1); cout << endl;

		while (2 > 1) {
			int x = keyPress();
			if (keyResult(x, 'Y')) {

				int random = RNG(100);
				if (0 <= random && random < 60) {
					textWriter("You rile the village people, but not in the way that you were hoping you might.", 0.07, 2); cout << endl;
					textWriter("You see three people rise up on the platform shouting", 0.07, 2); cout << endl;
					textWriter("\"Down with the oppressors!\"", 0.07, 2); cout << endl;
					textWriter("Realizing the severity of the situation, you begin to run away as fast as you can. Lose one health.", 0.07, 2);
					Sleep(2000);
					return 4;
				}
				if (60 <= random && random < 100) {
					textWriter("Luckily, they don't remember it was you who put them in this situation in the first place!", 0.07, 2); cout << endl;
					textWriter("The villagers seem to admire you more. You then ride away with a proud smile. . .", 0.07, 2);
					Sleep(2000);
					return 5;
				}
			}
			if (keyResult(x, 'N')) {
				textWriter(" Why would you want to waste your breath on useless peasants anyway?", 0.07, 2); cout << endl;
				textWriter("You continue walking through the street.", 0.07, 2); cout << endl;
				textWriter("As if you do not hear the agony of the workers. . .", 0.07, 2);
				Sleep(2000);
				return 6;
			}
		}
	}
	else if (75 <= random && random < 100) {
		textWriter("A priest, a merchant, and a peasant walk into the pub.", 0.07, 2); cout << endl;
		textWriter("You follow shortly after. Everybody inside is either too drunk", 0.07, 2); cout << endl;
		textWriter("or too distracted to notice you.", 0.07, 2); cout << endl;
		textWriter("As you approach the bar, the bartender's eyes widen.", 0.07, 2); cout << endl;
		textWriter("\"My Lord! What can I get you tonight? On the house, of course.\"", 0.07, 2); cout << endl;
		textWriter("Do you grab a drink ? (Y/N)", 0.07, 1); cout << endl;
		while (2 > 1) {
			int x = keyPress();
			if (keyResult(x, 'Y')) {
				int random = RNG(100);
				if (0 <= random < 55) {
					textWriter("After a night of boozing and schmoozing with the local harlot", 0.07, 2); cout << endl;
					textWriter("you think it may be time to go upstairs to the brothel for a night of fun.", 0.07, 2); cout << endl;
					textWriter("Do you take her hand to go up to one of the bedrooms for 5 ducats? (Y/N)", 0.07, 1); cout << endl;
					while (2 > 1) {
						int x = keyPress();
						if (keyResult(x, 'Y') && ducats >= 5) {
							int random = RNG(100);
							if (0 <= random && random < 70) {
								textWriter("You have a wonderful night with the girl of your dreams.", 0.07, 2); cout << endl;
								textWriter("At least, it sure did feel like a dream.", 0.07, 2); cout << endl;
								textWriter("In fact, when you go to the bathroom, you notice a spot of red in your urine.", 0.07, 2); cout << endl;
								textWriter("You run back into the room and see that the harlot you were talking to was actually a disease-ridden wench.", 0.07, 2); cout << endl;
								textWriter("You storm back to the palace to find a doctor. Lose 1 health and 5 ducats.", 0.07, 2);
								Sleep(2000);
								return 7;
							}
							if (70 <= random && random < 100) {
								textWriter("You have the time of your life!", 0.07, 2); cout << endl;
								textWriter("Everything was amazing. She even gave you a special surprise at the end.", 0.07, 2); cout << endl;
								textWriter("Free-of-charge as well!", 0.07, 2); cout << endl;
								textWriter("It's good to be king. Gain 1 health, lose 5 ducats.", 0.07, 2); cout << endl;
								textWriter("You become more respected in the village.", 0.07, 2);
								Sleep(2000);
								return 8;
							}
						}
						else if ((keyResult(x, 'Y') && ducats < 5)) {
							textWriter("\"What? You, the king, don't have a mere 5 ducats?\"", 0.07, 2); cout << endl;
							textWriter("\"Ugh! Even royalty has to pay!\"", 0.07, 2); cout << endl;
							textWriter("As the prostitute storms off, you reflect on your masculinity.", 0.07, 2); cout << endl;
							textWriter("Lose 1 health.", 0.07, 2);
							Sleep(2000);
							return 9;
						}
						if (keyResult(x, 'N')) {
							textWriter("Why would you sleep with the village harlot?", 0.07, 2); cout << endl;
							textWriter("God knows what kind of diseases she might have!", 0.07, 2); cout << endl;
							textWriter("You ride off your drunken stupor on your walk back to the palace.", 0.07, 2);
							Sleep(2000);
							return 10;
						}
					}
				}
				if (55 <= random && random < 100) {
					textWriter("You become angry at the lazy villagers, slacking off in the pub!", 0.07, 2); cout << endl;
					textWriter("You throw a ruckus, that can only be halfway understood through all of the alcohol-induced mumbling.", 0.07, 2); cout << endl;
					textWriter("No matter, the vilagers don't like your tone, and begin to fight you.", 0.07, 2); cout << endl;
					textWriter("You fool-heartedly swing right, but you hit a chair. Then a wild left fist meets your cheek.", 0.07, 2);
					textWriter("Oww! Maybe you should have thought that one through!", 0.07, 2);
					textWriter("Lose 1 health.", 0.07, 2);
					Sleep(2000);
					return 11;
				}
			}
			if (keyResult(x, 'N')) {
				textWriter("The filthy plebeians can enjoy their ales and booze", 0.07, 2); cout << endl;
				textWriter("you have a fine wine waiting to meet your lips in the palace.", 0.07, 2); cout << endl;
				textWriter("You leave the bar to go enjoy a night in your cozy bed alone.", 0.07, 2);
				textWriter("The group heading into the bar notices, and news spreads.", 0.07, 2); cout << endl;
				textWriter("you become less respected for being so lonesome. . .", 0.07, 2);
				Sleep(2000);
				return 12;
			}
		}
	}
}
//Displays the money, turns, and health on the main screen.
void overviewHUD(int ducats, int turns, int health) {
	pos(0, 0);

	colorPicker(2);
	cout << "Ducats: ";
	colorPicker(1);
	cout << ducats << endl;
	colorPicker(2);
	cout << "Turns: ";
	colorPicker(1);
	cout << turns << endl;
	colorPicker(2);
	cout << "Health: ";
	colorPicker(1);
	cout << health << endl;
}
//Displays the castle HUD!
int castleHUD(int ducats, int church, int houses) {
	ifstream inFS;
	string readerString;
	coolFileTextEffect("CastleHUD.txt", 0.07, 0, 2, 2);

	inFS.open("Kingdom.txt");
	int kingdomLine = 0;
	while (kingdomLine < 2) {
		getline(inFS, readerString);
		if (kingdomLine == 0) {
			pos(62, 7);
			textWriter(readerString.substr(14, string::npos), 0, 1);
		}

		if (kingdomLine == 1) {
			pos(3, 7);
			textWriter(readerString.substr(11, string::npos), 0, 1);
		}
		kingdomLine++;
	}
	inFS.close();

	pos(13, 16);
	colorPicker(1);
	cout << church;

	pos(14, 18);
	colorPicker(1);
	cout << houses;

	pos(4, 20);
	textWriter("Press B to invest in the village or C in the church. Press X to exit.", 0.07, 1);
	pos(0, 23);
	while (2 > 1) {
		int x = keyPress();

		if (keyResult(x, 'B') && ducats >= 45) {
			return 1;
		}
		else if (keyResult(x, 'B') && ducats < 45) {
			cout << "You do not have enough funds!" << endl;
		}

		if (keyResult(x, 'C') && ducats >= 70) {
			return 2;
		}
		else if (keyResult(x, 'C') && ducats < 70) {
			cout << "You do not have enough funds!" << endl;
		}

		if (keyResult(x, 'X')) {
			return 3;
		}
	}
}
//Resets user input into the file.
void kingdomReset() {
	fileTextReplacer("Kingdom.txt", 1, 0, string::npos, "Kingdom_Name: ");
	fileTextReplacer("Kingdom.txt", 2, 0, string::npos, "King_Name: ");
}
//Events interpreter / what was supposed to be in main.
void mainFunction(int ducats, int turns, int health, int castle, int houses, int church) {
	bool alive = true;
	bool win = false;
	bool dayone = true;

	while (alive != false && win != true) {
		turns = 7;
		health = 3;
		if (dayone == false) {
			ducats += (6 * houses);
		}
		if (houses == 0 && church == 0) {
			alive = false;
		}

		while (turns != 0 && alive == true && win != true) {
			kingdomLayoutKeeper(castle, church, houses, 0);
			overviewHUD(ducats, turns, health);
			dayone = false;

			pos(0, 28);
			textWriter("Press one of the corresponding buttons to visit an area. . . ", 0.01, 2);

			int x = keyPress();
			//Checking Castle HUD.
			if (keyResult(x, 'A') && castle > 0) {
				system("cls");
				int a = castleHUD(ducats, church, houses);
				if (a == 1) {
					ducats -= 45;
					houses++;
				}
				else if (a == 2) {
					ducats -= 70;
					church++;
				}
			}
			//Village Events.
			else if (keyResult(x, 'B') && houses > 0) {
				turns--;
				system("cls");

				int b = houseEvents(ducats);
				if (b == 1) {
					ducats += 7;
				}
				else if (b == 2) {
					health--;
				}
				else if (b == 4) {
					health--;
				}
				else if (b == 5) {
					houses++;
				}
				else if (b == 7) {
					health--;
					ducats -= 5;
				}
				else if (b == 8) {
					health++;
					houses++;
					ducats -= 5;
				}
				else if (b == 9) {
					health--;
				}
				else if (b == 11) {
					health--;
				}
				else if (b == 12) {
					houses--;
				}
			}
			//Church Events.
			else if (keyResult(x, 'C') && church > 0) {
				turns--;
				system("cls");

				int c = churchEvents(ducats);
				if (c == 1) {
					ducats += 14;
				}
				else if (c == 2) {
					health--;
				}
				else if (c == 4) {
					ducats -= 30;
					church += 1;
				}
				else if (c == 5) {
					church -= 1;
				}
				else if (c == 7) {
					church -= 1;
				}
				else if (c == 8) {
					church += 1;
					houses -= 1;
				}
				else if (c == 9) {
					houses += 1;
					ducats -= 19;
				}
			}
			//Secret instant win.
			else if (keyResult(x, 'W')) {
				int x = keyPress();
				if (keyResult(x, 'I')) {
					int x = keyPress();
					if (keyResult(x, 'N')) {
						win = true;
					}
				}
			}

			//Checking Death.
			if (health == 0) {
				alive = false;
			}

			//Checking victory conditions.
			if (houses >= 10 && church >= 10) {
				win = true;
			}
			system("cls");
		}
	}

	if (win) {
		coolFileTextEffect("Victory.txt", 0.11, 0.001, 3, 1);
		kingdomReset();
		Sleep(2000);
	}
	else {
		colorPicker(3);
		cout << "Gameover!" << endl;

		do {
			cout << "Press some buttons to quit. . ." << endl;
			//Quitting sequence.
			int x = keyPress();
			if (keyResult(x, 'Q')) {
				int x = keyPress();
				if (keyResult(x, 'U')) {
					int x = keyPress();
					if (keyResult(x, 'I')) {
						int x = keyPress();
						if (keyResult(x, 'T')) {
							kingdomReset();
							break;
						}
					}
				}
			}
		} while (2 > 1);
	}
}
