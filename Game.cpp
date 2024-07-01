#include "Game.h"
#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h>
using namespace std;
// Global variables
int n1 = 0, n2 = 0, botwins = 0, playerwins = 0, BotArrNums = 0, CheckNums = 0;
static string prevVarL = "0", prevVarR = "0", peretvor, points = "0-0", firstCourse = "player", NickName =""; // points left - Bot, right - User
bool isrestart = false, run = true, firstTime = true, firstChoose = true, bazarFull = false, isPass, menu = true;
const int row = 3, col = 57, rows= 7, cols = 4;
string gameField[row][col], Botarr[7], Botarr01[7] = {"1","1","1","1","1","1","1"};
const string arrStick[rows][cols] = {   {"0|0","1|0","1|1","2|0"},
										{"2|1","2|2","3|0","3|1"},
										{"3|2","3|3","4|0","4|1"},
										{"4|2","4|3","4|4","5|0"},
										{"5|1","5|2","5|3","5|4"},
										{"5|5","6|0","6|1","6|2"},
										{"6|3","6|4","6|5","6|6"} };
int arrFill[rows][cols];
// methods(functions)

// Game Menu
void Game::Menu() 
{
	if (menu) 
	{
		toMain:
		string menuChoice, underChoice, leadersChoice;
		cout << "*------------DOMINO-------------*" << endl;
		cout << "|\t1.Start game\t\t|\n";
		cout << "|\t2.Leaderboard\t\t|\n";
		cout << "|\t3.Rule and instructions\t|\n";
		cout << "|\t4.Exit\t\t\t|\n";
		cout << "*-------------------------------*";
		cout << "\nChoose variant(1-4): "; cin >> menuChoice;
		switch (menuChoice[0])
		{
		case '1':
			clear();
			cout << "Enter your Nickname(game name) to display in the Leaderboard: "; cin >> NickName;	
			menu = false;
			clear();
			break;
		case '2':
			clear();
			toTry:
			FileSort("save.txt");
			cout << "For exit q: "; cin >> leadersChoice;
			if (leadersChoice[0] == 'q') { clear(); goto toMain; }
			else { cout << "Try again!"; Sleep(700); clear(); goto toTry; }
			break;
		case '3':
			clear();
			toBegin:
			cout << """RULE: The game starts with 7 chips from the player and the computer.\nThe winner is the one who either has no chips left, or whose sum of numbers from the chips is less, as well as a tie,\nif the sum is the same (the game counts automatically).\nThere is also the bazaar, from where you will take chips into your \"hands\" (Maximum 7).\nINSTRUCTIONS: To exit the game, enter q.\nTo choose a chip, enter the numbers from 1 to 7.\nTo take chips from the bazaar, enter the b key and choose the position of the chip.\nTo skip a move, enter the p key.\nPoints are placed at the top of the game, where the computer's score is on the left, and yours is on the right.\n\t !!!IF THE WRONG LETTER OR NUMBER IS ENTERED, THE GAME WILL NOTIFY YOU ABOUT IT BE WARNED!!!\n""";
			cout << "For exit q: "; cin >> underChoice;
			if (underChoice[0] == 'q') { clear(); goto toMain; }
			else { cout << "Try again"; Sleep(700); clear(); goto toBegin; }
			break;
		case '4':
			run = false;
			break;
		default:
			cout << "Error! Try again!"; 
			Sleep(600); 
			clear();
			goto toMain;
		break;
		}
	}
}
// Saving
void Game::FileSaving() 
{
	ofstream file("save.txt", std::ios::app);
	string full;
	if (!file) {
		cerr << "Can`t open the file!" << endl;
	}
	full = NickName + ": " + points + "\n";
	file << full;
	file.close();
}
// Sort the file
struct Record {
	string name;
	double score = 0;
};
void Game::FileSort(string filename) {
	ifstream file(filename);
	if (!file.is_open()) {
		cout << "Can`t open the file!" << endl;
		return;
	}
	const int MAX_RECORDS = 100;
	Record* records = new Record[MAX_RECORDS];
	int numRecords = 0;
	string line;
	while (getline(file, line)) {
		size_t pos = line.find(":");
		if (pos != string::npos) {
			// string apart
			string Name = line.substr(0, pos);
			// get number after dash
			double balance = stod(line.substr(line.find("-", pos) + 1));
			// find similar Nicknames
			int index = -1;
			for (int i = 0; i < numRecords; ++i) {
				if (records[i].name == Name) {
					index = i;
					break;
				}
			}
			// write score
			if (index != -1) records[index].score += balance;
			else {
				records[numRecords].name = Name;
				records[numRecords].score = balance;
				numRecords++;
			}
		}
	}
	// sorting
	for (int i = 0; i < numRecords - 1; ++i) {
		for (int j = 0; j < numRecords - i - 1; ++j) {
			if (records[j].score < records[j + 1].score) {
				swap(records[j], records[j + 1]);
			}
		}
	}
	// output
	for (int i = 0; i < numRecords; ++i) {
		cout << records[i].name << ": " << records[i].score << endl;
	}
	delete[] records;
}
// Who won
void Game::Score() 
{
	int bot = 0, player = 0;
	for (int i = 0; i < 7; ++i)
		if (Botarr[i] == "0") ++bot;
	for (int j = 12; j < 19; ++j)
		if (gameField[2][j] == "0") ++player;
	if (bot == 7) 
	{
		if (playerwins >= 10) 
		{
			botwins += 1;
			points.erase(0, 2); points.insert(0, to_string(botwins));
			restart(); clear(); firstCourse = "bot";
		}
		else { botwins += 1; points.erase(points.begin()); points.insert(0, to_string(botwins)); restart(); clear(); firstCourse = "bot"; }
	}
	else if (player == 7) 
	{
		if (botwins >= 10)
		{
			playerwins += 1;
			points.erase(3); points.insert(3, to_string(playerwins));
			restart(); clear(); firstCourse = "player";
		}
		else { playerwins += 1; points.erase(2); points.insert(2, to_string(playerwins)); restart(); clear(); firstCourse = "player"; }
	}
	int labelcount = 0;
	for (int l = 0; l < 7; l++)
		if (Botarr[l] == "0" or gameField[2][(12 + l)] == "0") ++labelcount;
	if (bazarFull or labelcount == 0)
	{
		int Canput = 0, BotsSum = 0, PlayersSum = 0; string BotsSticks,PlayersSticks, peretBotSt, peretPlayerSt;
		for (int h = 0; h < 7; h++)
		{
			if (Botarr[h] != "0" and gameField[2][(12 + h)] != "0")
			{
				BotsSticks = Botarr[h]; PlayersSticks = gameField[2][(12 + h)];
				peretBotSt = BotsSticks; peretPlayerSt = PlayersSticks;
				swap(peretBotSt[0], peretBotSt[2]); swap(peretPlayerSt[0], peretPlayerSt[2]);
				if ((BotsSticks[2] == prevVarL[0] or peretBotSt[0] == prevVarR[2]) or (BotsSticks[0] == prevVarR[2] or peretBotSt[2] == prevVarL[0]))
					++Canput;
				if ((PlayersSticks[2] == prevVarL[0] or peretPlayerSt[0] == prevVarR[2]) or (PlayersSticks[0] == prevVarR[2] or peretPlayerSt[2] == prevVarL[0]))
					++Canput;
			}
		}
		if (Canput == 0) 
		{
			for (int b = 0; b < 7; b++)
			{
				if (Botarr[b] != "0" and gameField[2][(12 + b)] != "0")
				{
					BotsSticks = Botarr[b]; PlayersSticks = gameField[2][(12 + b)];
					BotsSum = (int)BotsSticks[0] + (int)BotsSticks[2];
					PlayersSum = (int)PlayersSticks[0] + (int)PlayersSticks[2];
				}
			}
			if (BotsSum < PlayersSum)
			{
				if (playerwins >= 10)
				{
					botwins += 1;
					points.erase(0, 2); points.insert(0, to_string(botwins));
					restart(); clear(); firstCourse = "bot";
				}
				else { botwins += 1; points.erase(points.begin()); points.insert(0, to_string(botwins)); restart(); clear(); firstCourse = "bot"; }

			}
			else if (BotsSum > PlayersSum)
			{
				if (botwins >= 10)
				{
					playerwins += 1;
					points.erase(3); points.insert(3, to_string(playerwins));
					restart(); clear(); firstCourse = "player";
				}
				else { playerwins += 1; points.erase(2); points.insert(2, to_string(playerwins)); restart(); clear(); firstCourse = "player"; }

			}
			else if (BotsSum == PlayersSum)
			{
				if (playerwins >= 10)
				{
					botwins += 1;
					points.erase(0, 2); points.insert(0, to_string(botwins));
					restart(); clear(); firstCourse = "bot";
				}
				else { botwins += 1; points.erase(points.begin()); points.insert(0, to_string(botwins)); restart(); clear(); firstCourse = "bot"; }
				//+
				if (botwins >= 10)
				{
					playerwins += 1;
					points.erase(3); points.insert(3, to_string(playerwins));
					restart(); clear(); firstCourse = "player";
				}
				else { playerwins += 1; points.erase(2); points.insert(2, to_string(playerwins)); restart(); clear(); firstCourse = "player"; }
			}
		}
	}
	// end
}
// Run
bool Game::Run()
{
	return run;
}
// Screen clearing
void Game::clear() 
{
	system("cls");
} // for Windows, dor Unix systems(Linux) system("clear");
// Exit from game
void Game::quit() {
	FileSaving();
	run = false;
}
// Game restart
void Game::restart()
{
	firstChoose = true; firstTime = true; CheckNums = 0; BotArrNums = 0; bazarFull = false; isrestart = true;
	prevVarL = "0"; prevVarR = "0";
	for (int i = 0; i < 7; ++i)
		for (int j = 0; j < 4; ++j)
			arrFill[i][j] = 0;
	for (int iF = 0; iF < 57; iF++) 
		gameField[1][iF] = "";
	for (int iP = 12; iP < 19; iP++)
		gameField[2][iP] = "0";
	for (int iB = 0; iB < 7; iB++)
	{
		Botarr[iB] = "0";
		Botarr01[iB] = "1";
	}
}
// random numbers
void Game::GetRandNums(int* n1, int* n2) 
{ 
	Sleep(1);
	*n1 = 0 + rand() % 7;
	*n2 = 0 + rand() % 4;
}
// Give chips for bazaar
void Game::bazgiveStick(int numPlace)
{
	int a = 0;
	GetRandNums(&n1, &n2);
	while (a != 1)
	{
		if (arrFill[n1][n2] == 1)
			GetRandNums(&n1, &n2);
		else
		{ arrFill[n1][n2] = 1; gameField[2][numPlace] = arrStick[n1][n2]; a = 1;}
	}
}
// bazaar
void Game::bazar()
{
	int l = 0;
	string corr, peretv;
	int n = 0;
	// checking
	while (l != 1)
	{
		for (int i = 12; i <= 18; i++)
		{
			if (gameField[2][i] != "0")
			{
				corr = gameField[2][i];
				// var transformation
				{
					peretv = corr;
					char temp = peretv[0];
					peretv[0] = peretv[2];
					peretv[2] = temp;
				}
				if ((corr[2] == prevVarL[0] or peretv[0] == prevVarR[2]) or (corr[0] == prevVarR[2] or peretv[2] == prevVarL[0]))
					++n;
			}
		}
		l = 1;
	}
	// release
	if (n > 0)
		cout << "You have: " << n << " moves";
	else
	{
		if (!bazarFull)
		{
			string bazChoose;
			int numPlace = 12;
			cout << "Choose chips` place in \"hands\"(1-7): "; cin >> bazChoose;
			switch (bazChoose[0])
			{
			case '1':
				if (gameField[2][numPlace] == "0") bazgiveStick(numPlace);
				else cout << "You have chip in this place!";
				break;
			case '2':
				if (gameField[2][(numPlace + 1)] == "0") bazgiveStick((numPlace + 1));
				else cout << "You have chip in this place!";
				break;
			case '3':
				if (gameField[2][(numPlace + 2)] == "0") bazgiveStick((numPlace + 2));
				else cout << "You have chip in this place!";
				break;
			case '4':
				if (gameField[2][(numPlace + 3)] == "0") bazgiveStick((numPlace + 3));
				else cout << "You have chip in this place!";
				break;
			case '5':
				if (gameField[2][(numPlace + 4)] == "0") bazgiveStick((numPlace + 4));
				else cout << "You have chip in this place!";
				break;
			case '6':
				if (gameField[2][(numPlace + 5)] == "0") bazgiveStick((numPlace + 5));
				else cout << "You have chip in this place!";
				break;
			case '7':
				if (gameField[2][(numPlace + 6)] == "0") bazgiveStick((numPlace + 6));
				else cout << "You have chip in this place!";
				break;
			default:
				cout << "Error! Try again!";
				break;
			}
		}
	}
}
// bazaar for Bot
void Game::Botbazar() 
{
	CheckArrAvailableSticks();
	AgainLoop:
	string stockstick, changedstick;
	int a = 0, i = 0;
	GetRandNums(&n1, &n2);
	if (!bazarFull) 
	{
		while (a != 1)
		{
			if (arrFill[n1][n2] == 1)
				GetRandNums(&n1, &n2);
			if (arrFill[n1][n2] != 1)
			{
				if (bazarFull) { a = 1; goto Ais1; }
				if (Botarr[i] == "0")
				{
					arrFill[n1][n2] = 1;
					Botarr[i] = arrStick[n1][n2];
					Botarr01[i] = "1";
					a = 1;
				}
				++i;
			}
			GetRandNums(&n1, &n2);
		Ais1:
			if (i == 7) { a = 1; }
		}
	}
	if (!bazarFull)
	{
		for (int j = 0; j < 7; j++)
		{
			stockstick = Botarr[j];
			if (stockstick != "0")
			{
				changedstick = stockstick;
				swap(changedstick[0], changedstick[2]);
				if ((stockstick[2] == prevVarL[0] or changedstick[2] == prevVarL[0]) or (stockstick[0] == prevVarR[2] or changedstick[0] == prevVarR[2]))
					++BotArrNums;
			}
		}
		if (BotArrNums == 0 and CheckNums != 0)
		{
			int numbersinBotArr = 0;
			for (int k = 0; k < 7; k++)
				if (Botarr[k] != "0") ++numbersinBotArr;
			if (numbersinBotArr != 7 and CheckNums != 0) goto AgainLoop;
			BotArrNums = 0;
		}
	}
	else BotArrNums = 0;
}
// Game Field
void Game::Fieldgame()
{
	for (int i = 0; i < row; i++, cout << endl)
		for (int j = 0; j < col; j++)
		{
			if (i == 0)
			{
				if (j >= 12 and j <= 18)
				{
					cout << gameField[i][j] << "   ";
				} else cout << gameField[i][j] << " ";
			}
			if (i == 2)
			{
				cout << gameField[i][j] << " ";
			}
			if (i == 1)
				cout << gameField[i][j];
		}
}
// Bot
void Game::Bot() 
{
	for (int i = 0; i < 7; ++i)
		gameField[0][(12+i)] = Botarr01[i];
	GetBestStick();

}
// Get chips on start
void Game::giveSticks(string gamer)
{
	if (gamer == "player")
	{
		GetRandNums(&n1, &n2);
		bool loop = true;
		int i = 12;
		while (loop)
		{
			if (arrFill[n1][n2] == 1)
				GetRandNums(&n1, &n2);
			if (arrFill[n1][n2] != 1)
			{
				gameField[2][i] = arrStick[n1][n2];
				arrFill[n1][n2] = 1;
				GetRandNums(&n1, &n2);
				++i;
			}
			if (i == 19)
				loop = false;
		}
	}
	if (gamer == "bot")
	{
		GetRandNums(&n1, &n2);
		bool loop = true;
		int i = 12;
		int n = 0;
		while (loop)
		{
			if (arrFill[n1][n2] == 1)
				GetRandNums(&n1, &n2);
			if (arrFill[n1][n2] != 1)
			{
				Botarr[n] = arrStick[n1][n2];
				arrFill[n1][n2] = 1;
				GetRandNums(&n1, &n2);
				++i;
				++n;
			}
			if (i == 19) loop = false;
		}
	}
}
// Choose ships for move(Bot)
void Game::GetBestStick()
{
	if (!isrestart or firstChoose) 
	{
		if (firstChoose)
		{
			BotSideLeft(Botarr[0], 12);
			Botarr[0] = "0";

		}
		else if (!firstChoose)
		{
		tryAgain:
			int arrN[7], ArrWithNums[7] = { 12,13,14,15,16,17,18 }, prevN1 = 0, N_1 = 0, prevN2 = 0, N_2 = 0, index = 0;
			string a, peretv, place1, place2, Side, arrSide[7], arrChangeStick[7]; // arrChangeStick - перетворена або звичайна(chganged or usual), place(1-ліва, 2-права)(перетворена чи ні(usual or chnaged))
			bool loop = true;
			while (loop)
			{
				prevN1 = 0, N_1 = 0, prevN2 = 0, N_2 = 0;
				a = Botarr[index];
				if (a == "0")
				{
					arrN[index] = 0;
					arrSide[index] = "0";
					arrChangeStick[index] = "0";
					++index;
					goto Bottom;
				}
				{
					peretv = a;
					if (peretv != "0|0")
					{
						char temp = peretv[0];
						peretv[0] = peretv[2];
						peretv[2] = temp;
					}
				}
				if ((a[2] == prevVarL[0] or peretv[0] == prevVarR[2]) or (a[0] == prevVarR[2] or peretv[2] == prevVarL[0]))
				{
					//left side
					if ((a[2] == prevVarL[0] or peretv[2] == prevVarL[0]))
					{
						string now, nowChanged;
						// usual chip
						if (a[2] == prevVarL[0])
						{
							for (int i = 0; i < 7;)
							{
								now = Botarr[i];
								// transformation choosed chip
								if (now != "0")
								{
									{
										nowChanged = now;
										char temp = nowChanged[0];
										nowChanged[0] = nowChanged[2];
										nowChanged[2] = temp;
									}
									if (a != now)
									{
										if (a[0] == now[2] or a[0] == nowChanged[2]) { ++N_1; ++i; }
										else ++i;
									}
									else ++i;
								}
								else ++i;
							}
						}
						// transformated
						if (peretv[2] == prevVarL[0])
						{
							for (int i = 0; i < 7;)
							{
								now = Botarr[i];
								// transformation choosed chip
								if (now != "0")
								{
									{
										nowChanged = now;
										char temp = nowChanged[0];
										nowChanged[0] = nowChanged[2];
										nowChanged[2] = temp;
									}
									if (a != now)
									{
										if (peretv[0] == now[2] or peretv[0] == nowChanged[2]) { ++prevN1; ++i; }
										else ++i;
									}
									else ++i;
								}
								else ++i;
							}
						}
						if (prevN1 >= N_1) { N_1 = prevN1; place1 = "changed"; }
						else place1 = "usual";
					}
					// right side
					if ((a[0] == prevVarR[2] or peretv[0] == prevVarR[2]))
					{
						string now2, nowChanged2;
						// usual chip
						if (a[0] == prevVarR[2])
						{
							for (int i = 0; i < 7;)
							{
								now2 = Botarr[i];
								// transformation choosed chip
								if (now2 != "0")
								{
									{
										nowChanged2 = now2;
										char temp = nowChanged2[0];
										nowChanged2[0] = nowChanged2[2];
										nowChanged2[2] = temp;
									}
									if (a != now2)
									{
										if (a[2] == now2[0] or a[2] == nowChanged2[0]) { ++N_2; ++i; }
										else ++i;
									}
									else ++i;
								}
								else ++i;
							}
						}
						// transformed
						if (peretv[0] == prevVarR[2])
						{
							for (int i = 0; i < 7;)
							{
								now2 = Botarr[i];
								// transformation choosed chip
								if (now2 != "0")
								{
									{
										nowChanged2 = now2;
										char temp = nowChanged2[0];
										nowChanged2[0] = nowChanged2[2];
										nowChanged2[2] = temp;
									}
									if (a != now2)
									{
										if (peretv[2] == now2[0] or peretv[2] == nowChanged2[0]) { ++prevN2; ++i; }
										else ++i;
									}
									else ++i;
								}
								else ++i;
							}
						}
						if (prevN2 >= N_2) { N_2 = prevN2; place2 = "changed"; }
						else place2 = "usual";
					}
					if (N_1 >= N_2)
					{
						Side = "L"; arrSide[index] = Side; arrChangeStick[index] = place1; arrN[index] = N_1; ++index;
					}
					else if (N_1 <= N_2) { Side = "R"; arrSide[index] = Side; arrChangeStick[index] = place2; arrN[index] = N_2; ++index; }
					if (N_1 == 0 or N_2 == 0)
					{
						if (arrSide[index] == "R" or arrSide[index] == "L")
						{
							arrN[index] = 1; ++index;
						}
					}
				}
				else
				{
					arrN[index] = 0;
					arrSide[index] = "0";
					arrChangeStick[index] = "0";
					++index;
				}
			Bottom:
				if (index == 7) loop = false;
			}
			// analysing and give chip into place functions
		Again:
			int arrindex = 0, arrmax;
			arrmax = arrN[0];
			for (int i = 0; i < 7; i++)
			{
				if (arrN[i] >= arrmax) { arrmax = arrN[i]; arrindex = i; }
			}
			if (arrmax == 0)
			{
				string stock, peretvorena; int countes = 0;
				for (int i = 0; i < 7; i++)
				{
					if (arrSide[i] != "0")
					{
						stock = Botarr[i];
						{ peretvorena = stock; swap(peretvorena[0], peretvorena[2]); }
						if ((stock[2] == prevVarL[0] or peretvorena[2] == prevVarL[0]))
						{
							arrSide[i] = "L";
							arrN[i] = 1;
							if (stock[2] == prevVarL[0]) arrChangeStick[i] = "usual";
							if (peretvorena[2] == prevVarL[0]) arrChangeStick[i] = "changed";
							countes++;
						}
						else if ((stock[0] == prevVarR[2] or peretvorena[0] == prevVarR[2]))
						{
							arrSide[i] = "R";
							arrN[i] = 1;
							if (stock[0] == prevVarR[2]) arrChangeStick[i] = "usual";
							if (peretvorena[0] == prevVarR[2]) arrChangeStick[i] = "changed";
							countes++;
						}
					}
				}
				if (countes != 0) goto Again;
				else if (countes == 0)
				{
					Botbazar();
					if (!bazarFull)
					{
						/*Botbazar();*/
						if (BotArrNums > 0) { BotArrNums = 0; goto tryAgain; }
						else if (BotArrNums == 0) { cout << "Pass! User turn!"; Sleep(600);}
					}
					else { cout << "\n Bazaar is empty!"; Sleep(600);}
				}
			}
			else if (arrmax != 0)
			{
				if (arrSide[arrindex] == "R")
				{
					if (arrChangeStick[arrindex] == "usual")
					{
						BotSideRight(Botarr[arrindex], ArrWithNums[arrindex]); 
						Botarr[arrindex] = "0"; Botarr01[arrindex] = "0";
					}
					else if (arrChangeStick[arrindex] == "changed")
					{
						peretv = Botarr[arrindex];
						char temp;
						temp = peretv[0];
						peretv[0] = peretv[2];
						peretv[2] = temp;
						BotSideRight(peretv, ArrWithNums[arrindex]);
						Botarr[arrindex] = "0"; Botarr01[arrindex] = "0";
					}
				}
				else if (arrSide[arrindex] == "L")
				{
					if (arrChangeStick[arrindex] == "usual")
					{
						BotSideLeft(Botarr[arrindex], ArrWithNums[arrindex]); 
						Botarr[arrindex] = "0"; Botarr01[arrindex] = "0";
					}
					else if (arrChangeStick[arrindex] == "changed")
					{
						peretv = Botarr[arrindex];
						char temp;
						temp = peretv[0];
						peretv[0] = peretv[2];
						peretv[2] = temp;
						BotSideLeft(peretv, ArrWithNums[arrindex]);
						Botarr[arrindex] = "0"; Botarr01[arrindex] = "0";
					}
				}
			}
		}
		firstChoose = false;
	}
	// end
}
// Check available chips in bazaar
void Game::CheckArrAvailableSticks() 
{
	CheckNums = 0;
	for (int i = 0; i < 7; ++i) {
		for (int j = 0; j < 4; ++j) {
			{ if (arrFill[i][j] != 1) ++CheckNums; }
		}
	}
	if (CheckNums == 0) { bazarFull = true; }
}
// Pass check
void Game::passAvailable() 
{
	int l = 0;
	string corr, peretv;
	int n = 0;
	// check
	while (l != 1)
	{
		for (int i = 12; i <= 18; i++)
		{
			if (gameField[2][i] != "0")
			{
				corr = gameField[2][i];
				// transformed
				{
					peretv = corr;
					char temp = peretv[0];
					peretv[0] = peretv[2];
					peretv[2] = temp;
				}
				if ((corr[2] == prevVarL[0] or peretv[0] == prevVarR[2]) or (corr[0] == prevVarR[2] or peretv[2] == prevVarL[0]))
					++n;
			}
		}
		l = 1;
	}
	// release
	if (n > 0)
	{
		cout << "You have " << n << " moves!";
		isPass = false;
	}
	else if (n == 0) isPass = true;
}
//Initialization give chips
void Game::Initialize()
{
	if (firstTime) {
		giveSticks("player");
		giveSticks("bot");
		firstTime = false;}
}
//Left side move for player
void SideLeft(string a, int ncol, const int nrow = 2)
{
	gameField[nrow][ncol] = "0";
	for (int i = 1; i <= 1; i++) {
		for (int j = 28; j >= 0;)
		{
			if (gameField[i][j] == "")
			{
				a.insert(0, "-");
				gameField[i][j] = a;
				j = -1;
			}
			else if (gameField[i][j] != "") j--;
		}
	}
}
// Right side move for player
void SideRight(string a, int ncol, const int nrow = 2) 
{
	gameField[nrow][ncol] = "0";
	for (int i = 1; i <= 1; i++) {
		for (int j = 29; j < 57;) {
			if (gameField[i][j] == "")
			{
				a.append("-");
				gameField[i][j] = a;
				j = 58;
			}
			else if (gameField[i][j] != "")
				j++;
		}
	}
}
// Left bot`s move
void Game::BotSideLeft(string stick, int sticknum)
{
	gameField[0][sticknum] = "0";
	if (firstChoose) { prevVarL = stick; prevVarR = stick; stick.append("-");}
	else prevVarL = stick;
	for (int i = 1; i <= 1; i++) {
		for (int j = 28; j >= 0;)
		{
			if (gameField[i][j] == "")
			{
				stick.insert(0, "-");
				gameField[i][j] = stick;
				j = -1;
			}
			else if (gameField[i][j] != "") j--;
		}
	}
}
// Right bot`s move 
void Game::BotSideRight(string stick, int sticknum) 
{
	gameField[0][sticknum] = "0";
	if (firstChoose) { prevVarL = stick; prevVarR = stick; }
	else prevVarR = stick;
	for (int i = 1; i <= 1; i++) {
		for (int j = 29; j < 57;) {
			if (gameField[i][j] == "")
			{
				stick.append("-");
				gameField[i][j] = stick;
				j = 57;
			}
			else if (gameField[i][j] != "")
				j++;
		}
	}
}
// Choose place for chip
void Game::choosePlace(string a, int ncol, const int nrow = 2)
{
	//First move
	if (firstChoose) 
	{
		prevVarL = a; prevVarR = a;
		a.insert(0, "-");
		a.append("-");
		gameField[1][28] = a;
		gameField[nrow][ncol] = "0";
		firstChoose = false;
	}
	else if (!firstChoose) // інші хода 
	{
		// transformation chip
		{
			peretvor = a;
			char temp = peretvor[0];
			peretvor[0] = peretvor[2];
			peretvor[2] = temp;
		}
		// Check available to move
		if ((a[2] == prevVarL[0] or peretvor[0] == prevVarR[2]) or (a[0] == prevVarR[2] or peretvor[2] == prevVarL[0]))
		{
			string hidplace;
			cout << "Choose side (1-left, 2-right): "; cin >> hidplace;
			switch (hidplace[0])
			{
			case '1':
				if (a[2] == prevVarL[0] or peretvor[2] == prevVarL[0])
				{
					cout << "Left!";
					if (a[2] == prevVarL[0])
					{
						prevVarL = a;
						SideLeft(a, ncol);
					}
					else { prevVarL = peretvor; SideLeft(peretvor, ncol); }
				}
				else { cout << "You can`t move this chip on left!\n"; Input(); }
				break;
			case '2':
				if (a[0] == prevVarR[2] or peretvor[0] == prevVarR[2]) 
				{
					cout << "Right!";
					if (a[0] == prevVarR[2]) 
					{
						prevVarR = a;
						SideRight(a, ncol);
					}
					else { prevVarR = peretvor; SideRight(peretvor, ncol); }
				}
				else { cout << "You can`t move this chip on right!\n"; Input(); }
				break;
			default:
				cout << "Error! Try again!";
				Sleep(600);
				clear();
				Score();
				cout << "\t\t\t" << points << endl;
				Fieldgame();
				Input();
				break;
			}
		}
		else { cout << "Available move is missing!\n"; Sleep(600); clear(); cout << "\t\t\t" << points << endl; Fieldgame(); Input(); }
	}
}
// User choose
void Game::Input()
{
	string choice;
	choice.reserve(1000);
	tryAgains:
	cout << "Choose variant(q-exit, p-pass, b-bazaar, (1-7) - choose chip for move): "; cin >> choice;
	if (!choice.empty()) 
	{
		switch (choice[0])
		{
		case '1':
			cout << "Choosed 1st chip!";
			if (gameField[2][12] != "0")
			{
				choosePlace(gameField[2][12], 12);
			}
			else if (gameField[2][12] == "0") { cout << "There is no chip!\n"; goto tryAgains; }
			break;
		case '2':
			cout << "Choosed 2nd chip!";
			if (gameField[2][13] != "0")
			{
				choosePlace(gameField[2][13], 13);
			}
			else if (gameField[2][13] == "0") { cout << "There is no chip!\n"; goto tryAgains; }
			break;
		case '3':
			cout << "Choosed 3rd chip!";
			if (gameField[2][14] != "0")
			{
				choosePlace(gameField[2][14], 14);
			}
			else if (gameField[2][14] == "0") { cout << "There is no chip!\n"; goto tryAgains; }
			break;
		case '4':
			cout << "Choosed 4th chip!";
			if (gameField[2][15] != "0")
			{
				choosePlace(gameField[2][15], 15);
			}
			else if (gameField[2][15] == "0") { cout << "There is no chip!\n"; goto tryAgains; }
			break;
		case '5':
			cout << "Choosed 5th chip!";
			if (gameField[2][16] != "0")
			{
				choosePlace(gameField[2][16], 16);
			}
			else if (gameField[2][16] == "0") { cout << "There is no chip!\n"; goto tryAgains; }
			break;
		case '6':
			cout << "Choosed 6th chip";
			if (gameField[2][17] != "0")
			{
				choosePlace(gameField[2][17], 17);
			}
			else if (gameField[2][17] == "0") { cout << "There is no chip\n"; goto tryAgains; }
			break;
		case '7':
			cout << "Choosed 7th chip";
			if (gameField[2][18] != "0")
			{
				choosePlace(gameField[2][18], 18);
			}
			else if (gameField[2][18] == "0") { cout << "There is no chip\n"; goto tryAgains; }
			break;
		case 'b':
			CheckArrAvailableSticks();
			if (bazarFull) cout << "\nBazaar is empty!";
			else 
			{
				bazar();
				Sleep(600);
				clear();
				Score();
				cout << "\t\t\t" << points << endl;
				Fieldgame();
				goto tryAgains;
			}
			break;
		case 'p':
			passAvailable();
			if (!isPass) goto tryAgains;
			else cout << "Pass!";
			break;
		case 'q':
			quit();
			cout << "You exit from game!";
			firstCourse = "player";
			break;
		default:
			cout << "Error! Try again!";
			goto tryAgains;
			break;
		}
	}
}
// Who move first?
void Game::whofirst()
{
	CheckArrAvailableSticks();
	if (firstCourse == "player") {
		cout << "\t\t\t" << points << endl;
		Initialize();
		for (int i = 0; i < 7; ++i)
			gameField[0][(12 + i)] = Botarr01[i];
		Fieldgame();
		Input();
		if (CheckNums != 0) {cout << "\nBot turn! Wait...." << endl; }
		isrestart = false;
		firstCourse = "bot";
		Sleep(1000);
		clear();
	}
	else if (firstCourse == "bot")
	{
		Initialize();
		Bot();
		Score();
		clear();
		if (isrestart) { clear();}
		else {
			cout << "\t\t\t" << points << endl;
			Fieldgame(); Input();
			if (CheckNums != 0 and firstCourse == "bot") cout << " Bot turn! Wait...." << endl;
			Score();
		}
	}
}
void Game::Draw()
{
	Menu();
	if(!menu)whofirst();
	isrestart = false;
	Sleep(800);
}
