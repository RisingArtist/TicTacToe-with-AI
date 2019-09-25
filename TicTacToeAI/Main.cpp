#include <iostream>
#include <vector>
#include <time.h> 

using namespace std;

#define MAX_TURNS 9
#define BOARD_SIZE 9 

void AiTurn(char  gameBoard[BOARD_SIZE], char& playerMark, char& aiMark);
void PlayerTurn(char gameBoard[BOARD_SIZE], char& playerMark);
bool const BeginGame(char& playerMark, char& aiMark);
void const RefreshBoard(const char gameBoard[BOARD_SIZE]);
bool const GameIsOver(char gameBoard[BOARD_SIZE], char& mark);
void RNG(char gameBoard[BOARD_SIZE], char& aiMark);

int main()
{
	//Initialize the game with a clean slate
	char gameBoard[BOARD_SIZE] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
	char playerMark;
	char aiMark;
	short currentTurn = 1;

	//Begin the game with the player or AI going first.
	bool isPlayerTurn;
	isPlayerTurn = BeginGame(playerMark, aiMark);

	//Game Loop
	RefreshBoard(gameBoard);
	bool isWin = false; //Is there a winner?
	while (currentTurn <= MAX_TURNS)
	{
		if (isPlayerTurn) //...Player's Turn
		{
			PlayerTurn(gameBoard, playerMark);
			if (GameIsOver(gameBoard, playerMark))
			{
				isWin = true;
				std::cout << "You win!" << endl;
				break;
			}
			isPlayerTurn = false;
		}
		else //...AI's Turn
		{
			AiTurn(gameBoard, playerMark, aiMark);
			if (GameIsOver(gameBoard, aiMark))
			{
				isWin = true;
				std::cout << "Computer wins!" << endl;
				break;
			}
			isPlayerTurn = true;
		}

		//Increment turn count
		currentTurn++;
	} //End of the Game Loop

	//Tie Game!
	if (currentTurn == MAX_TURNS && !isWin)
		std::cout << "CAT GAME! Nobody wins!" << endl;

	std::cout << " Thanks for playing!" << endl;
	return 0;
}

//BeginGame() - To start the game, we need to determine who goes first, the player or AI?
//Assign the two players with the appropiate symbol. 'X' goes first.
bool const BeginGame(char& playerMark, char& aiMark)
{
	bool startingPlayer;
	do {
		std::cout << "Who shall go first (will be 'X'): \n 0 - AI \n 1 - Player \n  Please enter here: ";
		cin >> startingPlayer;

		if (!cin)
		{
			cin.clear();
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
			std::cout << "Invalid input --- Please enter either 0 or 1 to start" << endl;
			continue;
		}
		else
		{
			break;
		}
	} while (true);

	if (startingPlayer)
	{
		playerMark = 'X';
		aiMark = 'O';
	}
	else
	{
		playerMark = 'O';
		aiMark = 'X';
	}
	return startingPlayer;
}

//AiTurn() - The AI knows where the vacant spots are then 
// 1) Determine which spot they place down will make it win the game...
// 2) Determine which spot will block the player from winning...
// 3) Otherwise, place the mark anywhere on the board with RNG
void AiTurn(char gameBoard[BOARD_SIZE], char& playerMark, char& aiMark)
{
	//Computer's Turn
	std::cout << "Computer's Turn!" << endl;
	char aiGameBoard[BOARD_SIZE];

	//Deep copy array for the AI
	for (int i = 0; i <= BOARD_SIZE; i++)
	{
		aiGameBoard[i] = gameBoard[i];
	}

	//Find all the vacant spots
	vector<int> vacantSpotIDs;
	for (int i = 0; i <= BOARD_SIZE; i++)
	{
		if (aiGameBoard[i] == ' ')
			vacantSpotIDs.push_back(i);
	}

	//Debug - what is inside the vacant spots vector?
	for (std::vector<int>::iterator it = vacantSpotIDs.begin(); it != vacantSpotIDs.end(); it++)
	{
		cout << *it << endl;
	}

	bool alreadyMove = false;
	for (std::vector<int>::iterator it = vacantSpotIDs.begin(); it != vacantSpotIDs.end(); it++)
	{
		int testSpot = *it;

		//Figure out where it can place the piece to win
		aiGameBoard[testSpot] = aiMark;
		if (GameIsOver(aiGameBoard, aiMark))
		{
			gameBoard[testSpot] = aiMark;
			RefreshBoard(gameBoard);
			std::cout << "Computer chose: " << testSpot << "!" << endl;
			alreadyMove = true;
			break;
		}
		//Figure out where it can place to block the player
		aiGameBoard[testSpot] = playerMark;
		if (GameIsOver(aiGameBoard, playerMark))
		{
			gameBoard[testSpot] = aiMark;
			RefreshBoard(gameBoard);
			alreadyMove = true;
			std::cout << "Computer chose: " << testSpot << "!" << endl;
			break;
		}
		aiGameBoard[testSpot] = ' ';
	}

	if (!alreadyMove)
	{
		//RNG, if everything is ok. 
		cout << "RNG method" << endl;
		RNG(gameBoard, aiMark);
	}
	vacantSpotIDs.clear();
}

//PlayerTurn() - Makes sure that the spot the player selects is valid
// if so, the player's mark will occupied the spot. 
// Otherwise, inform the player with the appropriate message and try again.
void PlayerTurn(char  gameBoard[BOARD_SIZE], char& playerMark)
{
	//If valid spot
	bool valid = false;
	do
	{
		int chosenSpot;
		std::cout << "Pick a number (0 - 8): ";
		cin >> chosenSpot;

		//Makes sure that this is a number.
		if (!cin)
		{
			std::cout << "NOT a digit" << endl;
			cin.clear();
			cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
			continue;
		}

		//Makes sure that number is in range
		if (chosenSpot < 0 || chosenSpot > 8)
		{
			std::cout << "Invalid number --- Please pick a number BETWEEN 0 - 8!" << endl;
			continue;
		}

		//Makes sure the spot isn't already taken
		if (gameBoard[chosenSpot] == ' ')
		{
			gameBoard[chosenSpot] = playerMark;
			RefreshBoard(gameBoard);
			valid = true;
		}
		else
		{
			std::cout << "The space is occupied by: " << gameBoard[chosenSpot] << endl;
			std::cout << "Invalid spot --- Please select a free spot on the board!" << endl;
		}
	} while (!valid);
}

//GameIsOver() - Checks to see if the game has a winner.
bool const GameIsOver(char gameBoard[BOARD_SIZE], char& mark)
{
	//Check if the game is over
	//Horizontal Check
	if (gameBoard[0] == mark && gameBoard[1] == mark && gameBoard[2] == mark ||
		gameBoard[3] == mark && gameBoard[4] == mark && gameBoard[5] == mark ||
		gameBoard[6] == mark && gameBoard[7] == mark && gameBoard[8] == mark)
	{
		return true;
	}

	//Vertical Check
	if (gameBoard[0] == mark && gameBoard[3] == mark && gameBoard[6] == mark ||
		gameBoard[1] == mark && gameBoard[4] == mark && gameBoard[7] == mark ||
		gameBoard[2] == mark && gameBoard[5] == mark && gameBoard[8] == mark)
	{
		return true;
	}

	//Diagonal Check
	if (gameBoard[0] == mark && gameBoard[4] == mark && gameBoard[8] == mark ||
		gameBoard[2] == mark && gameBoard[4] == mark && gameBoard[6] == mark)
	{
		return true;
	}

	return false;
}

//RefreshBoard() - Updates the board with current status of the game
void const RefreshBoard(const char gameBoard[BOARD_SIZE])
{
	system("CLS");
	std::cout << "Welcome to TicTacToe with AI. A computer will face off against you in a simple game of Tic Tac Toe!\nLet's see if you can win!" << endl;
	std::cout << gameBoard[0] << " | " << gameBoard[1] << " | " << gameBoard[2] << endl;
	std::cout << "---------" << endl;
	std::cout << gameBoard[3] << " | " << gameBoard[4] << " | " << gameBoard[5] << endl;
	std::cout << "---------" << endl;
	std::cout << gameBoard[6] << " | " << gameBoard[7] << " | " << gameBoard[8] << endl;

	std::cout << endl;

	std::cout << "How to pick a spot on the board:" << endl;
	std::cout << "1. Use the board below as a reference to pick your number." << endl;
	std::cout << "2. Use the numpad or number row to select a number when prompt" << endl;

	std::cout << 0 << " | " << 1 << " | " << 2 << endl;
	std::cout << "---------" << endl;
	std::cout << 3 << " | " << 4 << " | " << 5 << endl;
	std::cout << "---------" << endl;
	std::cout << 6 << " | " << 7 << " | " << 8 << endl;

	std::cout << endl;
}
 
//RNG() - Random Number Generator. A method for the AI if there isn't a space to win or block the player
void RNG(char gameBoard[BOARD_SIZE], char& aiMark)
{
	//RNG a spot
	srand(time(NULL));
	do {
		int randomSpot = rand() % BOARD_SIZE;
		if (gameBoard[randomSpot] == ' ')
		{
			gameBoard[randomSpot] = aiMark;
			RefreshBoard(gameBoard);
			std::cout << "Computer chose: " << randomSpot << endl;
			break;
		}
	} while (true);
}