#include <iostream>
#include "Client.h"
#include <string>
#include "Player.h"
#include "Ai.h"
#include <vector>

void Client::WaitEnter()
{
	cin.ignore();
	cout << "\nPress ENTER to end turn...";
	cin.get();
	for (int i = 0; i <= 60; i++) cout << endl;
}

void Client::RunSetup(){

	while(end_program == false) {

		int userChoice = 0;

		std::cout << "\n========================\n---------{Menu}---------\n1) Start New Game of Battleship\n2) Edit Number of Ships\n3) Exit Battleship\n========================\nEnter your choice: ";
		std::cin >> userChoice;

		if (std::cin.fail()) {

			std::cin.clear();
			std::cin.ignore();
			std::cout << "\n\n\n\nPlease type the number to which function you would like to do\n\n";

		}

		if(userChoice == 1){	//###Start Game
			//Connor—adding option to choose vs Player or AI
			std::cout << "\n\n========================\n1) Player-vs-Player " << "\n2) Player-vs-AI\n========================\n";
			std::cout << "Enter your choice: ";
			std::cin >> userChoice;
			if (userChoice == 1)
			{
				std::cout << "\nStarting the game with " << ship_count << " ships! \n\n";
				PlayerVsPlayer(ship_count);
			}

			else if (userChoice == 2)
			{
				SelectMode:
				int difficulty;
				std::cout << "\n\n========================\nSelect Difficulty\n";
				std::cout << "1) Easy \n2) Medium \n3) Hard \n";
				std::cout << "========================\nEnter your choice: ";
				std::cin >> difficulty;

				std::cout << "\nStarting the game with " << ship_count << " ships! \n\n";
				PlayerVsAI(ship_count, difficulty);
			}
			else
			{
				std::cout << "Not a valid option. Try again.\n";
 				goto SelectMode;
			}

  		}
		else if (userChoice == 2){	//###Edit Ship Count -- This checks for the correct num of ships

			int userShips = 0;

			std::cout << "\nEnter how many ships you would like next game to be played with (1 - 5): ";

			while (true)
			{
				bool valid_num_of_ships = true;
				std::cin >> userShips;

				if(std::cin.fail()) {
					valid_num_of_ships = false;
					std::cin.clear();
					std::cin.ignore();
					std::cout << "\n\nPlease enter the number of ships to play with\n\n";

				}
				if(userShips > 5) {
					valid_num_of_ships = false;
					std::cin.clear();
					std::cin.ignore();
					std::cout << "\nPlease enter less than 6 ships to play with: ";

				}
				if(userShips <= 0) {
					valid_num_of_ships = false;
					std::cin.clear();
					std::cin.ignore();
					std::cout << "\nPlease enter at least 1 ship to play with: ";
				}

				if(valid_num_of_ships == true) break;
	  		}
			ship_count = userShips;
			std::cout << "\nGame settings updated to be played with " << ship_count << " ship(s)!\n";
   		}
		else if (userChoice == 3){	//###Exit Program
			end_program = true;
		}
}	//end of run conidition
}	//end of Client::Run

//Connor—Changed name to PlayerVsPlayer as I am adding a PlayerVsAI function
void Client::PlayerVsPlayer(int num_ships)
{
	Player* player1 = new Player;		//create each player
	Player* player2 = new Player;
	std::string shot;
	int shotType = 0;
	std::vector<std::string> shotVector;
	std::string shipSunk;

	player1->placeShips(num_ships, 1);	//let both players place ships
	player1->replaceShip(num_ships, 1);
	WaitEnter();
	std::cout << "\n";
	player2->placeShips(num_ships, 2);
	player2->replaceShip(num_ships, 2);
	WaitEnter();
	end_game = false;

	while (end_game == false)
	{
		if(turn == false)
		{
			std::cout << "\nPlayer 1, its your turn!\n";
			std::cout << "YOUR CURRENT SHIP STATUS\n";
			player1->printShipBoard(); //prints ship board
			std::cout << "\nWHERE YOU'VE SHOT\n";
			std::cout << "Enemy Ships Remaining: " << player2->shipsRemaining() << "\n";
			player1->printShootBoard(); //prints shoot board
			std::cout << "X = hit, * = miss\n\n";

			bool valid_input = false; //Makes sure that user input is good before advancing
			while (valid_input == false) //start input loop
			{
				shotType = player1->selectShot();
				shotVector = player1->coordinateShot(shotType);
				valid_input = player1->validateShot(shotVector);
			} //end input loop

			player1->depleteSpecialShot(shotType);

			//iterate through shotVector
			for(auto& shot: shotVector)
			{
				std::cout << "\nFIRE!!!\n";

				if (player2->isHit(shot) == true) //Is it a hit?
				{
					player1->markShot(shot, true);
					std::cout << "BANG!!!";

					if (player2->isSunk(shot) == true) //Is it a sunk?
					{
						std::cout << "\nYou have sunk their ship with that shot!\n";
						shipSunk = player2->getShipSunk();
						player2->resetShipSunk();

						player1->acquireSpecialShot(shipSunk);
					}
					else
					{
						std::cout << "\nThats a hit!\n";
					}
				}
				else
				{
					if(!player1->uniqueShot(shot))
					{
						std::cout << "SPLASH!!!\n";
						std::cout << "You only hit debris!\n";
					}
					else
					{
						player1->markShot(shot, false);
						player2->markEnemyMiss(shot);
						std::cout << "SPLASH!!!\n";
						std::cout << "You hit empty waters.\n";
					}
				}
				if (player2->shipsRemaining() == 0) //Game win condition
				{
					std::cout << "\n##########- PLAYER 1 HAS WON THE GAME!!! -##########\n";
					player1->printShootBoard();
					std::cout << "##########- PLAYER 1 HAS WON THE GAME!!! -##########\n";
					end_game = true;
					break;
				}
			}
			shotVector.clear();
			if (!end_game) WaitEnter();
		//------------------------------------------------------------------------------------------
		}
		else //Player 2 turn
		{
			std::cout << "\nPlayer 2, its your turn!\n";
			std::cout << "YOUR CURRENT SHIP STATUS\n";
			player2->printShipBoard(); //prints ship board
			std::cout << "\nWHERE YOU'VE SHOT\n";
			std::cout << "Enemy Ships Remaining: " << player1->shipsRemaining() << "\n";
			player2->printShootBoard(); //prints shoot board
			std::cout << "X = hit, * = miss\n\n";

			bool valid_input = false; //Makes sure that user input is good before advancing
			while (valid_input == false) //start input loop
			{
				shotType = player2->selectShot();
				shotVector = player2->coordinateShot(shotType);
				valid_input = player2->validateShot(shotVector);
			} //end input loop

			player2->depleteSpecialShot(shotType);

			//iterate through shotVector
			for(auto& shot: shotVector)
			{
				std::cout << "\nFIRE!!!\n";

				if (player1->isHit(shot) == true) //Is it a hit?
				{
					player2->markShot(shot, true);
					std::cout << "BANG!!!";

					if (player1->isSunk(shot) == true) //Is it a sunk?
					{
						std::cout << "\nYou have sunk their ship with that shot!\n";
						shipSunk = player1->getShipSunk();
						player1->resetShipSunk();

						player2->acquireSpecialShot(shipSunk);
					}
					else
					{
						std::cout << "\nThats a hit!\n";
					}
				}
				else
				{
					if(!player2->uniqueShot(shot))
					{
						std::cout << "SPLASH!!!\n";
						std::cout << "You only hit debris!\n";
					}
					else
					{
						player2->markShot(shot, false);
						player1->markEnemyMiss(shot);
						std::cout << "SPLASH!!!\n";
						std::cout << "You hit empty waters.\n";
					}
				}
				if (player1->shipsRemaining() == 0) //Game win condition
				{
					std::cout << "\n##########- PLAYER 2 HAS WON THE GAME!!! -##########\n";
					player2->printShootBoard();
					std::cout << "##########- PLAYER 2 HAS WON THE GAME!!! -##########\n";
					end_game = true;
					break;
				}
			}
			shotVector.clear();
			if (!end_game) WaitEnter();
		} //end of turn selection
		if (turn == false) //Switch turns
		{
			turn = true;
		}
		else
		{
			turn = false;
		}
	}

	delete player1;	//free heap
	delete player2;
	player1 = nullptr;
	player2 = nullptr;
} // end_game loop


//Connor—Single player mode
void Client::PlayerVsAI(int num_ships, int difficulty)
{
	Player* player = new Player;		//create each player
	//Player* playerAI = new Player;
	std::vector<std::string> shotVector;
	int shotType;
	std::string shipSunk;
	player->placeShips(num_ships, 1);	//let both players place ships
	player->replaceShip(num_ships, 1);
	std::cout << "\n";

	AI ai(difficulty, num_ships);
	// std::cout << "AI board\n";

	// ai.printShipBoard(); // for check now
	ai.getCoords(player->getCoords()); // Gets the coordinates from the player

	end_game = false;
	turn = false;

	while (end_game == false)
	{
		if(turn == false)
		{
			std::cout << "\nPlayer, its your turn!\n";
			std::cout << "YOUR CURRENT SHIP STATUS\n";
			player->printShipBoard(); //prints ship board
			std::cout << "\nWHERE YOU'VE SHOT\n";
			std::cout << "AI Ships Remaining: " << ai.getShipsRemaining() << "\n";
			player->printShootBoard(); //prints shoot board
			std::cout << "X = hit, * = miss\n\n";

			std::string shot;
			bool valid_input = false; //Makes sure that user input is good before advancing
			while (valid_input == false) //start input loop
			{
				shotType = player->selectShot();
				shotVector = player->coordinateShot(shotType);
				valid_input = player->validateShot(shotVector);
			} //end input loop

			player->depleteSpecialShot(shotType);

			for(auto& shot: shotVector)
			{
				std::cout << "\nFIRE!!!\n";

				if (ai.isHit(shot) == true) //Is it a hit?
				{
					player->markShot(shot, true);
					std::cout << "BANG!!!";

					if(ai.getShipsRemaining() == 0) //Is it a sunk?
					{
						std::cout<< "\nYou have sunk their ship with that shot!\n";
						std::cout << "\n##########- PLAYER HAS WON THE GAME!!! -##########\n";
						player->printShootBoard();
						std::cout << "##########- PLAYER HAS WON THE GAME!!! -##########\n";
						end_game = true;
						break;
					}
					else if (ai.isSunk(shot))
					{
						std::cout<< "\nYou have sunk their ship with that shot!\n";
						shipSunk = ai.getShipSunk(shot);
						player->acquireSpecialShot(shipSunk);
					}
					else std::cout << "\nThat's a hit! \n";

					ai.markHit(shot);
				}
				else
				{
					if(!player->uniqueShot(shot))
					{
						std::cout << "SPLASH!!!\n";
						std::cout << "You only hit debris!\n";
					}
					else
					{
						player->markShot(shot, false);
						//ai.markEnemyMiss(shot);
						std::cout << "SPLASH!!!\n";
						std::cout << "You hit empty waters.\n";
					}
				}
			}
			shotVector.clear();
				cin.ignore();
				cout << "\nPress ENTER to end turn...";
				cin.get();
		}
		else{
			std::cout << "\nIts AI's turn!\n";
			std::string shot = ai.Move();
			std::cout << "\nFIRE!!! The AI shot at "<< shot << "\n";
			if (player->isHit(shot) == true) //Is it a hit?
			{
				ai.markShot(shot, true);
				std::cout << "BANG!!! You've been hit! \n";
				if (player->isSunk(shot))
				{
					std::cout << "One of your ship's was destroyed! \n";
					player->resetShipSunk();
				}
			}
			else
			{
				ai.markShot(shot, false);
				player->markEnemyMiss(shot);
				std::cout << "bloooop.....the missile was off-target.\n";
			}
			if (player->shipsRemaining() == 0) //Is it a sunk?
			{
				std::cout << "\n##########- AI HAS WON THE GAME!!! -##########\n";
				ai.printShootBoard();
				std::cout << "##########- AI HAS WON THE GAME!!! -##########\n";
				end_game = true;
			}
				//cin.ignore();
				cout << "\nPress ENTER to continue...";
				cin.get();
		}
		turn = !turn; // switch turns
	}

	delete player;
} // end_game loop




bool Client::CheckShotInput(std::string shot_check)
{
	if (shot_check.length() != 2) //Is it exactly two letter long?
	{
		return false;
	}

	char letter = toupper(shot_check[0]);
	char number = shot_check[1];

	if (( number >= '1' ) && ( number <= '9' ))
	{
		//compares ascii values
		if ( (letter >= 'A' ) && ( letter <= 'I' )) //Is the alpha within range?
		{
			return true; //Then the input is good!
		}
	}
	return false;
}

Client::Client(){ // constructor that defaults to the ship count being 3

	end_program = false;
	end_game = false;
	ship_count = 3;
	turn = false;
}

Client::~Client(){	//destructor

}	//end destructor
