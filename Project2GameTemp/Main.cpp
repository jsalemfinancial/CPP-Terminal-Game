#include "mainheader.h"

/*
Pseudo Code:
Phase 1: Developing functions for file manipulation.
Goal: Develop code for messing with text files, since this is a a text-based game.

- pos(X,Y); move the buffer cursor to different locations using SetConsoleCursorPosition.
- fileTextDisplay(...); Take a file and open it with ifstream. Use a .eof() while-loop with int i and getline to transfer the file content into strings and
	individually color the strings using i % 2 (with the outcomes being assinged to certain colors from a colorizer function.)
- textWriter(...); Take a string and read the characters. Display the characters with a Sleep() delay afterwards.
- coolFileTextEffect(...); Same structure as the fileTextDisplay(...) function and textWriter(...) function combined. Open a file's content, then create 
	a while-loop until the end of the file (.eof()) that delays the letter placement and line placement (probably a for-loop. The text 
	will be colorized using the same for-loop int (i) % 2 to alternate color.)
- textFlashEnter(...); Use pos(X,Y) and keep outputting the desired string while resetting the buffer to the origin. Alternate the colors
	to make a flashing effect. //This was only really used for "Press ENTER to play."
- EDITED: fileTextReplacer(...); Same opening sequence as fileTextDisplay(...) in addition to memorizing the file's content into a vector<string>
	using getline. A while-loop with int n and getline was used to open the file and add to n until no more lines were read; this was used
	isntead of .eof() because that gave me some problems. Afterwards, the string inputted will replace a chosen "line" from the vector using .replace()
	and overwrite the file using ofstream.
- fileTextAppender(...): Almost exactly like fileTextReplacer(...) with the exception of using .append() instead of .replace().
- fileMerger(...); essentially fileTextAppender with 2 vectors instead. the .append() is replaced with a "+=". The content of 2 files is memorized in
	vector<string> and then the values are added. the results overwrite the first vector. The first vector is then used with ofstream to input the
	newly merged lines into another text file. //This isn't directly implemented, but it was used for combining ASCII pictures, as I suck at making them.

Phase 2: Core game functions.
Goal: Develop functions specific to the game.

- keyPress(); Utilize <conio.h> _getwch() to instantly get keystrokes. Since characters have an int value, the function returns an int.
- keyResult(...); Uses the KeyPress() return value as one parameter and a desired character as the other. Both are altered with toupper()
	and then compared. If they match, a true is returned for use with if statements.
- RNG(...); takes at range limit as a parameter. I read somewhere that some RNG use atmospheric static from lightning to generate numbers. This
	functions doesn't use pseudo random RNG like rand(). It instead reads the current RAM usage of the computer (code for that not original, 
	see RNG(...) function for link) and then modules it by the limit. A DWORDLONG was used to get as big of a number as possible for variation.
- overviewHUD(...); takes ducats, turns, and health as parameters; it simply displays these values on the top-left using pos(X,Y) to overwrite the display.
- kingdomLayoutKeeper(...); takes the current levels of the kingdom as parameters. Displays the ASCII files in if-else branches to create the "terrain."
* - castleHUD(...); When the castle is entered, it greets the player with their chosen king and kingdom names read from the altered Kingdom.txt file. 
	Also displays the current levels for buildings and allows for purchasing upgrades with ducats.
* houseEvents(...); Uses RNG(...) to generate random events for the village! Only creates a story using textWriter. Based on the results, an int is returned
	to be interpreted by the main() function.
- churchEvents(...); Basically the same as houseEvents(...) except it gives church-related events.
- kingdomReset(); uses fileTextReplacer(...) to reset the kingdom name and king name in Kingdom.txt.

Phase 3: The game.
Goal: create a cool game!

- The game plays simply. Instead of inputting text into the console, keystrokes are retrieved instantly with _getwch(). A chain of while-loops is created,
	the first is meant to reset the player's health every 7 turns and check for the gameover condition. The second loop is nested inside the first and
	retrieves keystrokes as well as interpret them; the second loop also checks for death and victory conditions.

* The player can win instantly by entering the sequence W I N while in the main screen.
* After losing, the player is prompted to "Press some buttons to exit. . ." and can enter the seuqnce Q U I T to finally quit.
	Any other buttons will reset the sequence, failing to exit the game!


* means the line is project related.
PS: Event writing was mainly done by a friend of mine, I take little credit for it.
PPS: A lot of content was cut due to lack of time sadly. About 4 other buildings were not implemented.
*/
int main() {
	ShowCursor(FALSE);
	MoveWindow(consoleControl, 0, 0, 800, 800, TRUE);
	kingdomReset(); //Resets Kingdom.txt

	string kingName;
	string kingdomName;

	int ducats = 45; int turns = 7; int health = 3;
	int castle = 1; int houses = 3; int church = 0;

	coolFileTextEffect("MainMenu.txt", 0.002, 0.0012, 1, 3);
	textFlasherEnter("Press ENTER to play", 0.5, 30, 21, 3, 4);
	cin.ignore();
	system("cls");

	kingdomLayoutKeeper(castle, church, houses, 0.12);

	pos(0, 0);
	textWriter("In a land", 0.09, 1); cout << endl;
	textWriter("not so far away called. . .", 0.09, 1); cout << endl;

	colorPicker(2);
	cout << "Kingdom's Name: ";
	getline(cin, kingdomName); cout << endl;

	colorPicker(3);
	textWriter("There lived a king named. . .", 0.07, 1); cout << endl;

	colorPicker(2);
	cout << "Your Name: ";
	getline(cin, kingName);

	fileTextAppender("Kingdom.txt", 1, kingdomName);
	fileTextAppender("Kingdom.txt", 2, kingName);

	system("cls");
	mainFunction(ducats, turns, health, castle, houses, church);

	system("exit");
}