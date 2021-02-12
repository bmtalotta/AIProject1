//game.cpp
//Ben Talotta
//2/6/2021
//for use in cs405 5x5 tic tac toe
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <utility>
#include <unistd.h>
#include <algorithm>
#include "game.hpp"
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::getline;
using std::ofstream;
using std::ifstream;
using std::vector;
using std::pair;
using std::max;


void displayBoard(Board match);
int menu();
int checkRows(Board &match, char &curPiece,int row,int col);
bool checkForWinner(Board &match, char &player,int row,int col);
pair<int,int> placeBotChoice(Board &match, char &curTeam, int turnCounter);
int evaluateFunc(Board &match, int row,int col,char curTeam);
int minimax(Board &match, int depth,int row,int col,char curTeam, bool minimizing);


/*============GAME RULES============
	1. get 4 in a row on a 5x5 board to win
	2. in a row is counted for horizontal vertical or diagnal
	player type 1 is human
	player type 2 is random bot
	player type 3 is AI
*/
void game()
{
	int gameMode = menu();
	int playerOneType;
	int playerTwoType;
	switch (gameMode) {
	case 1:
		playerOneType = 1;
		playerTwoType = 1;
		break;
	case 2:
		playerOneType = 1;
		playerTwoType = 2;
		break;
	case 3:
	cout << "sleeping for 1 second between boards" << endl;
		playerOneType = 3;
		playerTwoType = 1;
		break;
	case 4:
		playerOneType = 1;
		playerTwoType = 3;
		break;
	case 5:
	
	cout << "sleeping for 1 second between boards" << endl;
		playerOneType = 3;
		playerTwoType = 2;
		break;
	case 6:
	
	cout << "sleeping for 1 second between boards" << endl;
		playerOneType = 3;
		playerTwoType = 3;
		break;
	case 7:
		replay();
		return;
		break;
		
	case 8:
		cout << "the number of boards is (depth + 1)!\n since we are looking at depth 3 the number of boards will be 4! which is 24" << endl;
		return;
		break;
	default:
		break;
	}

	Board match = { {' ', ' ', ' ', ' ',' '},
					{' ', ' ', ' ', ' ',' '},
					{' ', ' ', ' ', ' ',' '},
					{' ', ' ', ' ', ' ',' '},
					{' ', ' ', ' ', ' ',' '} };
	bool gameOver = false;
	bool validMove;
	bool check;
	string playerchoice;
	int row;
	int col;
	int turnCounter= 0;
	int currentPlayerType;
	char playersSymbol;
	srand(time(NULL));
	ofstream file("temp.txt");
	file.open("temp.txt");
	file << "";
	file.close();
	cout << "to enter a move enter x then y with the top left being 1,1 and bottom right being 5,5, or quit by entering q\n";
	displayBoard(match);
	while (!gameOver) {
		
		turnCounter += 1;
		if (turnCounter % 2 == 1) {
			currentPlayerType = playerOneType;
		}
		else {
			currentPlayerType = playerTwoType;
		}
		row = 100;
		col = 100;
		if (turnCounter % 2 == 1) {
			cout << "player ones turn" << std::endl;
		}
		else {
			cout << "player twos turn" << std::endl;
		}
		validMove = false;
		pair<int,int> botsMove;
		
		playersSymbol = 'x';
		if(turnCounter % 2 != 1){
			playersSymbol = 'O';
		}
		while (!validMove) {
			if (currentPlayerType == 1) {
				cout << "enter the x value\n";
				cin >> playerchoice;
				if(playerchoice == "q"){
					cout << "ok, game is over" << endl;
					return;
				}
				col = std::stoi(playerchoice)-1;
				cout << "enter the y value\n";
				cin >> playerchoice;
				if(playerchoice == "q"){
					cout << "ok, game is over" << endl;
					return;
				}
				row = std::stoi(playerchoice)-1;
			}
			else if (currentPlayerType == 2) {
				row = rand() % 5;
				col = rand() % 5;
			}
			else if (currentPlayerType == 3) {
				botsMove = placeBotChoice(match,playersSymbol,turnCounter);
				row = botsMove.first;
				col = botsMove.second;
			}
			if (match[row][col] == ' ') {
				validMove = true;
			}
		}
		match[row][col] = playersSymbol;
		displayBoard(match);
		if(playerOneType != 1 && playerTwoType != 1){
			sleep(1);
		}
		check = checkForWinner(match, playersSymbol, row, col);
		if(check){
			gameOver = true;
			cout << playersSymbol << "\'s has won" << std::endl;
		}
		else if(turnCounter == 25){
			gameOver = true;
			cout << "Game over\nIts a draw!" << std::endl;
		}
	}
	char answer;
	string name;
	bool decide = false;
	while(!decide){
		cout << "save the match history? y/n\n";
		cin >> answer;
		if(tolower(answer) == 'y'){
			cout << "okay, enter a name for the file" << std::endl;
			cin >> name;
			string filename = name + ".txt";
			ifstream in("temp.txt");
			ofstream out(filename);
			out << in.rdbuf();
			out.close();
			in.close();
			decide = true;
		}
		else if (tolower(answer) == 'n'){
			decide = true;
		}
	}


}

void replay()
{
	string name;
	cout <<" enter the match save name\n";
	cin >> name;
	name += ".txt";
	ifstream in(name);
	cout << in.rdbuf();
	in.close();
	return;
}
void displayBoard(Board currentBoard)
{
	cout << std::endl;
	string boardState;
	for(auto i =0; i < 5; i++ ){
		boardState += " ";
		for (int j = 0; j < 4; ++j){
			boardState += currentBoard[i][j];
			boardState += " | ";
		}
		boardState += currentBoard[i][4];
		boardState += "\n";
		if(i !=4){
		boardState += "------------------\n";
		}else{
			boardState += "\n";
		}
	}
	cout << boardState;
	ofstream file;
	file.open("temp.txt", std::ios_base::app);
	file << "\n\n" << boardState;
	file.close();

}
int menu()
{
	string choice;
	int numChoice;
	while (true) {
		numChoice = 999;
		cout << "Please Select GameMode:\n    1.Player vs Player\n    2. player vs random bot\n    3.AI vs player(ai goes first)\n    4.player vs AI(ai goes second)\n    5.AI vs random bot\n    6.AI vs AI\n    7.replay a match\n    8.number of boards generated by the AI" << endl;
		getline(cin, choice);
		numChoice = std::stoi(choice);
		if (numChoice >= 1 && numChoice <= 8) {
			return numChoice;
		}
		cout << "please enter a valid choice\n";
	}
	return 0;
}


int checkRows(Board &match, char &curPiece,int row,int col){
	int curRow = row;
	int curCol = col;
	bool valid = true;
	int inRowCountV = 0;
	int inRowCountH = 0;
	int inRowCountDa = 0;
	int inRowCountDb = 0;
	int tempRow = 0;
	//checkvertical
	for(int i = 0; i < 5; ++i){
		if(match[curRow][i] == curPiece){
			inRowCountV += 1;
		}
		else if(tempRow < inRowCountV){
			tempRow = inRowCountV;
			inRowCountV = 0;
		}
		else{
			inRowCountV = 0;
		}
	}
	if(tempRow > inRowCountV){
		inRowCountV = tempRow;
	}
	if(inRowCountV == 4){
		return 4;
	}

	//check horizontal
	tempRow = 0;
	for(int i = 0; i < 5; ++i){
		if(match[i][curCol] == curPiece){
			inRowCountH += 1;
		}else if(tempRow < inRowCountH){
			tempRow = inRowCountH;
			inRowCountH = 0;
		}
		else{
			inRowCountH = 0;
		}
	}
	if(tempRow > inRowCountH){
			inRowCountH = tempRow;
		}
	if(inRowCountH == 4){
		return 4;
	}
	while(valid){//diagonal go to top left from last placed
		if(curCol >= 0 && curRow >= 0){
			curCol -= 1;
			curRow -= 1;
		}
		else{
			valid = false;
		}
	}
	valid = true;
	tempRow = 0;
	while(valid){//check that diagonal
		if(curCol < 4 && curRow < 4){
			curCol += 1;
			curRow += 1;
			if (match[curRow][curCol] == curPiece){
				inRowCountDa += 1;
			}
			else if (tempRow < inRowCountDa){
				tempRow = inRowCountDa;
				inRowCountDa = 0;
			}
			else{
				inRowCountDa = 0;
			}
		}
		else{
			valid = false;
		}
	}
	if(tempRow > inRowCountDa){
		inRowCountDa = tempRow;
	}
	if(inRowCountDa == 4){
		return 4;
	}
	curRow = row;
	curCol = col;
	valid = true;
	while(valid){//diagonal to top right
		if(curRow  >= 0 && curCol <= 4 ){
			curCol += 1;
			curRow -= 1;
		}
		else{
			valid = false;
		}
	}
	valid = true;
	tempRow = 0;
	while(valid){//check that diagonal
		if(curRow < 4 && curCol > 0){
			curCol -= 1;
			curRow += 1;
			if (match[curRow][curCol] == curPiece){
				inRowCountDb += 1;
			}
			else if (tempRow < inRowCountDb){
				tempRow = inRowCountDb;
				inRowCountDb = 0;
			}
			else{
				inRowCountDb = 0;
			}
		}
		else{
			valid = false;
		}
	}
	if(tempRow > inRowCountDb){
		inRowCountDb = tempRow;
	}
	if(inRowCountDb == 4){
		return 4;
	}
	
	int dummyVal = max(inRowCountV,max(inRowCountH,max(inRowCountDa, inRowCountDb)));
	if(dummyVal >= 3){
	}
	return dummyVal;
}
bool checkForWinner(Board &match, char &player,int row,int col){
	
	int winner = checkRows(match,player,row,col);
	if(winner >= 4){
		//displayBoard(match);
		return true;
	}
	else{
		return false;
	}
}
pair<int,int> placeBotChoice(Board &match, char &curTeam, int turnCounter){
	int bestScore = -1000000000;
	int depth = 0;
	int score;
	pair<int,int> bestMove;
	int firstBlankFound = 0;
	for (int i = 0; i < 5; i++){
		for(int j = 0; j < 5; j++){
			if(match[i][j] == ' '){
				
				match[i][j] = curTeam;
				score = minimax(match,depth,i,j,curTeam, true);
				
				cout << j + 1 << ", " << i + 1 << "    " << bestScore << endl;
				match[i][j] = ' ';
				if(score > bestScore){
					bestScore = score;
					bestMove.first = i;
					bestMove.second = j;
				}
			}
		}
	}
	cout << bestMove.second + 1 << ", " << bestMove.first + 1 << "    " << bestScore << endl;
	return bestMove;
}
int evaluateFunc(Board &match,int row,int col,char curTeam){
	int score = 10;
	char otherTeam = 'x';
	bool middle = false;
	if(row > 0 && row < 4 && col > 0 && col < 4){
		middle = true;
	}
	if(otherTeam == curTeam){
		otherTeam = 'O';
	}
	match[row][col]=otherTeam;
	int enemy = checkRows(match,otherTeam,row,col);
	match[row][col]=curTeam;
	int friendly = checkRows(match,curTeam,row,col);
	//check middle
	if(row == 2 && col == 2){
		score += 10000;
	}
	int middleBlankCount = 0;
	//check if in middle 3x3
	for(int i = 1; i < 4; ++i){
		for(int j = 1; j < 4; ++j){
			if(match[i][j] == otherTeam){
				--score;
				if(((i==1)||(i==3))&&((j==1)||(j==3))){
					--score;
				}
			}
			if(match[i][j] == curTeam){
				++score;
			}
			
		}
	}
	if(middle){
		score += 10;
	}
	if(enemy >= 2 && middle){
		score += 150;
	}
	if(friendly >= 2 && middle){
		score += 49;
	}
	if(enemy == 3){
		score += 200;
	}
	if(enemy == 2){
		score += 10;
	}
	if (friendly == 4){
		score += 999;
	}
	if(friendly == 4){
		score += 200;
	}
	if(friendly == 3){
		score += 200;
	}
	if(friendly == 2){
		score += 20;
	}


	return score;
}
int minimax(Board &match, int depth,int row,int col,char lastTeam, bool minimizing){
	int bestScore = -100000000;
	int score;
	int evalScore;
	char  currentTeam = 'x';
	if(lastTeam == currentTeam){
		currentTeam = 'O';
	}
	
	if(minimizing){
		bestScore = 100000000;
	}
	bool winOnTurn = checkForWinner(match,lastTeam,row,col);
	if(winOnTurn){
		if(!minimizing){
			return -100000;
		}
		else{
			return 100000;
		}
	}
	if(depth  >= 1 && !minimizing){
		
		evalScore = -evaluateFunc(match, row, col, lastTeam) ;
		return evalScore;
	}
	else if(depth >= 1 && minimizing){
		evalScore = evaluateFunc(match, row, col, lastTeam);
		return evalScore;
	}
	for (int i = 0; i < 5; i++){
		for(int j = 0; j < 5; j++){
			if(match[i][j] == ' '){
				match[i][j] = currentTeam;
				score = minimax(match,depth + 1,i,j,currentTeam,!minimizing);
				match[i][j] = ' ';
				if(bestScore < score && !minimizing){
					bestScore = score;
				}
				else if(bestScore > score && minimizing){
					bestScore = score;
				}
				
			}
		}
	}
	return bestScore;
}