#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<windows.h>
//defining colors
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

enum rank{ Ace, Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King };
enum Suit { Spades, Clubs, Diamonds, Hearts };
char *RankNames[] = { "Ace", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King" };
char *suitNames[] = { "Spades", "Clubs", "Diamonds", "Hearts" };

typedef struct cardStructure{
	enum rank rank;
	enum suit suit;
} card;
//method prototyping
void dealCards(card *player1[], card *player2[], card *player3[], card *player4[], card *deck[]);//creates 52 unique cards and assigns them to 4 players
void playGame(card *player1[], card *player2[], card *player3[], card *player4[], card *pile[]);//main function which runs the game
void clearPile(card *pile[]);
void clearPileOutput();
int checkwin(card *player1[], card *player2[], card *player3[], card *player4[]);
int higher(card *currentCard, card *previousCard);

void locate(int a, int b){
	COORD c;
	c.X = a;
	c.Y = b;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);//sets cursor to the entered coordinate
}
void design(){
	for (int i = 0; i < 102; i += 2){
		locate(i + 10, 7);
		printf(GREEN"* ");
	}
	for (int i = 0; i < 102; i += 2){
		locate(i + 10, 9);
		printf("* ");
	}
	for (int i = 0; i < 42; i += 2){
		locate(i + 10, 23);
		printf("*");
	}
	for (int i = 64; i < 102; i += 2){
		locate(i + 10, 23);
		printf("*");
	}
	for (int i = 0; i < 102; i += 2){
		locate(i + 10, 26);
		printf("*");
	}
	for (int i = 0; i < 18; i++){
		locate(10, i + 8);
		printf("*");
	}
	for (int i = 0; i < 18; i++){
		locate(30, i + 8);
		printf("*");
	}
	for (int i = 0; i < 18; i++){
		locate(51, i + 8);
		printf("*");
	}
	for (int i = 0; i < 18; i++){
		locate(73, i + 8);
		printf("*");
	}
	for (int i = 0; i < 18; i++){
		locate(92, i + 8);
		printf("*");
	}
	for (int i = 0; i < 20; i++){
		locate(112, i + 7);
		printf("*");
	}
	printf(RESET);
}
void instructions(){
	system("cls");
	locate(47, 1);
	printf(GREEN"***"CYAN"Instructions"GREEN"***\n"RESET);
	printf(GREEN"\n\t\tGeneral info:"CYAN"\n \t\t\t<>"YELLOW"This is a card game. There are a total of 52 unique cards with a rank and a suit."CYAN"\n\t\t\t<>"YELLOW"The ranks range from 0 to 12. The suits are Spades ,Clubs ,Diamonds ,Hearts.\n"RESET);
	printf(CYAN"\t\t\t<>"YELLOW"There are a total of 4 players. This game starts from player1.\n"RESET);
	printf(GREEN"\t\tRules:"CYAN"\n\t\t\t1:"YELLOW"The main goal of the player is to play all of his cards and make his deck empty.\n"RESET);
	printf(CYAN"\t\t\t2:"YELLOW"A player can either play(by entering a card) or pass to next player.\n\t\t\t     It means he does not puts any card it that turn if he passes.\n"RESET);
	printf(CYAN"\t\t\t3:"YELLOW"A Player can enter any card but only once. It means he cannot use that card again.\n\t\t\t     The entered cards must be unique.\n"RESET);
	printf(CYAN"\t\t\t4:"YELLOW"The card inserted by any player should be greater than previously placed card.\n\t\t\t     If first or new turn is started then card of any value can be placed.\n"RESET);
	printf(CYAN"\t\t\t5:"YELLOW"If any player passes his turn and others pass too and then turn comes back to him\n\t\t\t     Then the piled cards are cleared and the player is required to place new card of any value.\n");
	printf("\t\t\t     That card should be used first. Moreover the player cannot pass his turn in this case.\n"RESET);
	printf(CYAN"\t\t\t6:"YELLOW"If a player places a card with rank of two then:\n");
	printf("\t\t\t     Piled cards are cleared & turn is started again.\n");
	printf("\t\t\t     The player has to place new card with any rank\n\t\t\t     But he cannot place the card he used previously.\n"RESET);
	printf(MAGENTA"\n Press any key to exit instructions."RESET);
	getch();
}
void about(){
	system("cls");
	locate(47, 1);
	printf(GREEN"***"CYAN"About"GREEN"***"RESET);
	locate(10, 3);
	printf(CYAN"<>"YELLOW"This is an end semester project for the course of Fundamentals of Programming.");
	locate(10, 4);
	printf(CYAN"<>"YELLOW"The is a team work of:");
	locate(15, 5);
	printf(CYAN"->"YELLOW"Ahson");
	locate(15, 6);
	printf(CYAN"->"YELLOW"Ahsan Mushtaq Sial");
	locate(15, 7);
	printf(CYAN"->"YELLOW"Muhammad Uzair");
	locate(15, 8);
	printf(CYAN"->"YELLOW"Muhammad Uzair");
	printf(MAGENTA"\n\n Press any key to exit about page."RESET);
	getch();
}

void choice(int *ptrExit){
	system("cls");
	int userChoice = 0;
	printf("\n\t\t\t   (`\\ .-') /`   ('-.\t\t\t\t\t _   .-')       ('-.\n");
	printf("\t\t	    `.( OO ),' _(  OO)                                  ( '.( OO )_  _(  OO)\n");
	printf(GREEN"\t\t	,--."RESET"/"GREEN"  .--."RESET"   (,"GREEN"------..--.       .-----.  "RESET".-')"GREEN",-----.  ,--.   ,--."RESET")("GREEN",------.\n"RESET);
	printf(GREEN"\t\t	|      |  |    | .----'|  |"RESET".-')  "GREEN"'  .--./ "RESET"( OO"GREEN"'  .-.  ' |   `.'   |  |  .---'\n"RESET);
	printf(GREEN"\t\t	|  |   |  |"RESET","GREEN"   | |     |  |"RESET" OO )"GREEN" |  |"RESET"('-. /"GREEN"   |  | |  | |         |  |  |    \n"RESET);
	printf(GREEN"\t\t	|  |.'.|  |"RESET"_) ("GREEN"|  '--. |  |"RESET"`-' |/_) |OO  )\\_)"GREEN" |  |"RESET"\\"GREEN"|  | |  |'.'|  | "RESET"("GREEN"|  '--.\n"RESET);
	printf(GREEN"\t\t	|         |    |  .--'"RESET"("GREEN"|  '---."RESET"'|"GREEN"|  |"RESET"`-'|   \\"GREEN" |  | |  | |  |   |  |  |  .--' \n");
	printf(GREEN"\t\t	|   ,'.   |    |  `---.|      |"RESET"(_"GREEN"'  '--'\\    "RESET"`"GREEN"'  '-'  ' |  |   |  |  |  `---.\n");
	printf(GREEN"\t\t	'--'   '--'    `------''`-----'   `-----'      `-----'  `--'   `--'  `------'\n");
	locate(42, 10);
	printf(GREEN"Welcome to the the game."RESET);
	locate(42, 11);
	printf(CYAN"1:"YELLOW"Play Game"RESET);
	locate(42, 12);
	printf(CYAN"2:"YELLOW"Instructions"RESET);
	locate(42, 13);
	printf(CYAN"3:"YELLOW"About"RESET);
	locate(42, 14);
	printf(CYAN"4:"YELLOW"Exit."RESET);
	locate(42, 16);
	printf(MAGENTA"Your choice:"RESET);
	scanf("%d", &userChoice);
	if (userChoice != 1 && userChoice != 2 && userChoice != 3 && userChoice != 4){ printf("\a"); choice(ptrExit); }
	if (userChoice == 2){ instructions(); choice(ptrExit); }
	if (userChoice == 3){ about(); choice(ptrExit); }
	if (userChoice == 4){ *ptrExit = 1; }
}

int main(){
	card *player1[52 / 4];
	card *player2[52 / 4];
	card *player3[52 / 4];
	card *player4[52 / 4];
	card *deck[52];
	card *pile[52];
	int exit;
	int *ptrExit = &exit;
	//first page
	choice(ptrExit);
	if (*ptrExit == 1){ locate(21, 18); printf(YELLOW"Program is exited succcessfully. "RED); return 0; }//main will return zero i.e it will end. Thus program will end.
	//play game
	locate(0, 20);
	printf(MAGENTA"Dealing cards..."RESET);
	Sleep(2000);
	dealCards(player1, player2, player3, player4, deck);//this function generates random cards and  assigns them to players
	system("cls");
	printf(CYAN"\t\t\t         _____  _____  ______  _____ _____ _____  ______ _    _ _______\n");
	printf("\t\t\t	|  __ \\|  __ \\|  ____|/ ____|_   _|  __ \\|  ____| \\  | |__   __|\n");
	printf("\t\t\t	| |__) | |__) | |__  | (___   | | | |  | | |__  |  \\ | |  | |\n");
	printf("\t\t\t	| ___ /|  _  /|  __|  \\___ \\  | | | |  | |  __| | .  ` |  | |\n");
	printf("\t\t\t	| |    | | \\ \\| |____ ____) |_| |_| |__| | |____| | \\  |  | |\n");
	printf("\t\t\t	|_|    |_|  \\_\\______|_____/|_____|_____/|______|_|  \\_|  |_|\n"RESET);

	design();
	locate(15, 8);
	printf(MAGENTA"Player 1");
	locate(35, 8);
	printf("Player 2");
	locate(75, 8);
	printf("Player 3");
	locate(95, 8);
	printf("Player 4"RESET);
	for (int i = 0; i < 52 / 4; i++){
		locate(12, i + 10);
		int p = player1[i]->suit;
		printf(CYAN"%d)"YELLOW"%d of %s\n"RESET, i + 1, player1[i]->rank, suitNames[p]);
	}
	printf("\n");
	for (int i = 0; i < 52 / 4; i++){
		int p = player2[i]->suit;
		locate(32, i + 10);
		printf(CYAN"%d)"YELLOW"%d of %s\n"RESET, i + 1, player2[i]->rank, suitNames[p]);
	}
	printf("\n");
	for (int i = 0; i < 52 / 4; i++){
		int p = player3[i]->suit;
		locate(75, i + 10);
		printf(CYAN"%d)"YELLOW"%d of %s\n"RESET, i + 1, player3[i]->rank, suitNames[p]);
	}
	printf("\n");
	for (int i = 0; i < 52 / 4; i++){
		int p = player4[i]->suit;
		locate(94, i + 10);
		printf(CYAN"%d)"YELLOW"%d of %s\n"RESET, i + 1, player4[i]->rank, suitNames[p]);
	}
	locate(1, 23);
	printf(CYAN"Choose\n card:\n-1 to pass");
	locate(10, 27);
	printf("Status:");
	locate(10, 28);
	printf("Note:"YELLOW" Any player can end game by entering -2."RESET);
	//the main game starts here
	playGame(player1, player2, player3, player4, pile);
	//game ends here
	printf(RED" ");
	getch();
	return 0;
}//main ends here

void dealCards(card *player1[], card *player2[], card *player3[], card *player4[], card* deck[]){
	int cardsCreated = 0;
	int turn = 0;         // check turn to know which player will get the card
	card Card;            // Card template
	srand(time(NULL));
	while (cardsCreated < 52)
	{
		int cardFound = 0;
		Card.rank = rand() % 13;
		Card.suit = rand() % 4;

		for (int index = 0; index < cardsCreated; index++)
		{
			card *cardTaken = deck[index];  // Take a card from the deck...

			if (cardTaken->rank == Card.rank && cardTaken->suit == Card.suit) // ...and compare it to the newly made card
			{
				cardFound = 1;  // Card is a duplicate, exit loop and continue
				break;
			}

		}

		if (cardFound == 0)
		{
			if (turn == 0) {
				player1[cardsCreated / 4] = (card *)malloc(sizeof(card)); // Malloc the card and give player1 the card
				player1[cardsCreated / 4]->suit = Card.suit;
				player1[cardsCreated / 4]->rank = Card.rank;
				deck[cardsCreated] = player1[cardsCreated / 4];               // Add card to deck for comparison
			}
			else if (turn == 1) {
				player2[(cardsCreated - 1) / 4] = (card *)malloc(sizeof(card)); // Malloc the card and give player2 the card
				player2[(cardsCreated - 1) / 4]->suit = Card.suit;
				player2[(cardsCreated - 1) / 4]->rank = Card.rank;
				deck[cardsCreated] = player2[(cardsCreated - 1) / 4];               // Add card to deck for comparison
			}
			else if (turn == 2){
				player3[(cardsCreated - 2) / 4] = (card *)malloc(sizeof(card)); // Malloc the card and give player3 the card
				player3[(cardsCreated - 2) / 4]->suit = Card.suit;
				player3[(cardsCreated - 2) / 4]->rank = Card.rank;
				deck[cardsCreated] = player3[(cardsCreated - 2) / 4];               // Add card to deck for comparison
			}
			else if (turn == 3){
				player4[(cardsCreated - 3) / 4] = (card *)malloc(sizeof(card)); // Malloc the card and give player4 the card
				player4[(cardsCreated - 3) / 4]->suit = Card.suit;
				player4[(cardsCreated - 3) / 4]->rank = Card.rank;
				deck[cardsCreated] = player4[(cardsCreated - 3) / 4];               // Add card to deck for comparison
			}
			turn++;
			if (turn == 4){ turn = 0; }    // Restart turn
			cardsCreated++;
		}
	}
}

void playGame(card *player1[], card *player2[], card *player3[], card *player4[], card* pile[]){
	int turn = 1;
	int player = 1;
	int userInput = 0;
	int cardPiled = 0;
	int line = 10;
	int gameEnd = 0;
	card *selectedCard[52];
	card *currentCard;
	card *previousCard;
	int cardInserted = 0;
	int compareResult;
	int pass = 0;
	int cardLine = 0;
	int win = 0;
	int playerWhoEnds = 0;

	while (gameEnd == 0){
		while (player == 1){
			while (cardInserted == 0){
				if (pass == 4){
					clearPile(pile);
					cardPiled = 0;
					clearPileOutput();
					locate(17, 27);
					printf(YELLOW"You started pass and turn is back to you so restart the turn by putting new card.                   "RESET);
					turn = 1;
					pass = 0;
					line = 10;
				}
				locate(12, 24);
				scanf("%d", &userInput);
				//check if right input is inserted
				if (userInput > 13 || userInput <-2 || userInput == 0){
					locate(12, 24); printf("                  ");
					locate(17, 27); printf(YELLOW"Please enter valid input.                                                                          "RESET);
					cardInserted = 0;
					break;
				}
				if (userInput == -2){ playerWhoEnds = player; player = 5; cardInserted = 1; gameEnd = 1; break; }
				locate(17, 27);
				printf("                                                                                                   ");
				if (userInput == -1 && turn != 1){
					player++; pass++; locate(12, 24); printf("                  ");
					locate(17, 27); printf("                                                                                                   "); turn = 5; break;
				}
				if (userInput == -1 && turn == 1){
					locate(17, 27); printf(YELLOW"You cannot pass now.                                                                               "RESET);
					locate(12, 24); printf("                  "); cardInserted = 0; break;
				}
				locate(55, line);
				//check if card is already taken
				if (player1[userInput - 1] != NULL){
					//setting currentCard and previous
					currentCard = (card*)malloc(sizeof(card));
					previousCard = (card*)malloc(sizeof(card));
					currentCard->rank = player1[userInput - 1]->rank;
					currentCard->suit = player1[userInput - 1]->suit;
					if (turn != 1){
						previousCard->rank = pile[(cardPiled - 1)]->rank;
						previousCard->suit = pile[(cardPiled - 1)]->suit;
					}
					//check if card with rank of 2 is enterd
					if (currentCard->rank == 2){
						clearPile(pile);
						cardPiled = 0;
						clearPileOutput();
						locate(12, 24); printf("                  ");
						player1[userInput - 1] = NULL;
						cardLine = 9 + (userInput);
						locate(28, cardLine);
						printf("-");
						pass = 0;
						turn = 1;
						line = 10;
						locate(17, 27);
						printf(YELLOW"Enter one more card because you entered a card with rank of 2.                                      "RESET);
						break;
					}
					if (turn == 1){ compareResult = 1; }
					else{
						//Check if card has higher value
						compareResult = higher(currentCard, previousCard);
					}
					if (compareResult == 1){
						pile[cardPiled] = (card *)malloc(sizeof(card));
						pile[cardPiled]->rank = player1[userInput - 1]->rank;
						pile[cardPiled]->suit = player1[userInput - 1]->suit;
						int SuitName = pile[cardPiled]->suit;
						printf("%d of %s", pile[cardPiled]->rank, suitNames[SuitName]);
						cardLine = 9 + (userInput);
						locate(28, cardLine);
						printf("-");
						locate(12, 24);
						printf("                  ");
						locate(17, 27);
						printf("                                                                                                   ");
						player1[userInput - 1] = NULL;
						cardInserted = 1;
						cardPiled++;
						pass = 0;
						turn = 5;
						win = checkwin(player1, player2, player3, player4);
						if (win == 1){ player = 5;	gameEnd = 1; break; }
						player++;
						line++;
					}
					else{
						locate(17, 27);
						printf(YELLOW"Card has lower or equal value than one inserted previously. Insert card with higher value.         "YELLOW);
						locate(12, 24);
						printf("                  ");
					}
					//user has to input again coz while loop runs as cardInserted is still 0
				}
				else {//if player1[userInput]==NULL. It means card is already taken
					printf("\a");
					locate(17, 27);
					printf(YELLOW"Card is already taken.                                                                             "RESET);
					locate(12, 24);
					printf("                  ");
				}
			}//ending of while(cardInserted==0)
			cardInserted = 0;
		}
		while (player == 2){
			while (cardInserted == 0){
				if (pass == 4){
					clearPile(pile);
					cardPiled = 0;
					clearPileOutput();
					locate(17, 27);
					printf(YELLOW"You started pass and turn is back to you so restart the turn by putting new card.                  "RESET);
					turn = 222;
					pass = 0;
					line = 10;
				}
				locate(32, 24);
				scanf("%d", &userInput);
				//check if right input is inserted
				if (userInput > 13 || userInput <-2 || userInput == 0){
					locate(32, 24); printf("                   ");
					locate(17, 27); printf(YELLOW"Please enter valid input.                                                                          "RESET);
					cardInserted = 0;
					break;
				}
				if (userInput == -2){ playerWhoEnds = player; player = 5; cardInserted = 1; gameEnd = 1;  break; }
				locate(17, 27);
				printf("                                                                                                   ");
				if (userInput == -1 && turn != 222){
					player++; pass++; locate(32, 24); printf("                   "); 	locate(17, 27);
					printf("                                                                                                   "); turn = 5; break;
				}
				if (userInput == -1 && turn == 222){
					locate(17, 27); printf(YELLOW"You cannot pass now.                                                                               "RESET);
					locate(32, 24); printf("                   "); cardInserted = 0; break;
				}
				locate(55, line);
				//check if card is already taken
				if (player2[userInput - 1] != NULL){
					//setting currentCard and previoius
					currentCard = (card*)malloc(sizeof(card));
					previousCard = (card*)malloc(sizeof(card));
					if (turn != 222){
						previousCard->rank = pile[(cardPiled - 1)]->rank;
						previousCard->suit = pile[(cardPiled - 1)]->suit;
					}
					currentCard->rank = player2[userInput - 1]->rank;
					currentCard->suit = player2[userInput - 1]->suit;
					//Check if card with rank of 2 is entered
					if (currentCard->rank == 2){
						clearPile(pile);
						cardPiled = 0;
						clearPileOutput();
						locate(32, 24); printf("                   ");
						player2[userInput - 1] = NULL;
						cardLine = 9 + (userInput);
						locate(49, cardLine);
						printf("-");
						locate(17, 27);
						turn = 222;//i have just let it 222 coz i am using it as 222 for player 2
						line = 10;
						pass = 0;
						printf(YELLOW"Enter one more card because you entered a card with rank of 2.                                     "RESET);
						break;
					}
					if (turn == 222){ compareResult = 1; }
					else {
						//Check if card has higher value
						compareResult = higher(currentCard, previousCard);
					}
					if (compareResult == 1){
						pile[cardPiled] = (card *)malloc(sizeof(card));
						pile[cardPiled]->rank = player2[userInput - 1]->rank;
						pile[cardPiled]->suit = player2[userInput - 1]->suit;
						int SuitName = pile[cardPiled]->suit;
						printf("%d of %s", pile[cardPiled]->rank, suitNames[SuitName]);
						//printf("*%d*", pass);
						locate(32, 24);
						printf("                   ");
						cardLine = 9 + (userInput);
						locate(49, cardLine);
						printf("-");
						locate(17, 27);
						printf("                                                                                                   ");
						player2[userInput - 1] = NULL;
						cardInserted = 1;
						cardPiled++;
						turn = 5;
						win = checkwin(player1, player2, player3, player4);
						if (win == 2){ player = 5;	gameEnd = 1; break; }
						player++;
						pass = 0;
						line++;
					}
					else{
						locate(17, 27);
						printf(YELLOW"Card has lower or equal value than one inserted previously. Insert card with higher value.         "RESET);
						locate(32, 24);
						printf("                   ");
					}
				}
				else {
					printf("\a");
					locate(17, 27);
					printf(YELLOW"Card is already taken.                                                                             "RESET);
					locate(32, 24);
					printf("                   ");
				}
			}
			cardInserted = 0;
		}
		while (player == 3){
			while (cardInserted == 0){
				if (pass == 4){
					clearPile(pile);
					cardPiled = 0;
					clearPileOutput();
					locate(17, 27);
					printf(YELLOW"You started pass and turn is back to you so restart the turn by putting new card.                  "RESET);
					turn = 333;
					pass = 0;
					line = 10;
				}
				locate(75, 24);
				scanf("%d", &userInput);
				//check if right input is inserted
				if (userInput > 13 || userInput <-2 || userInput == 0){
					locate(75, 24); printf("                 ");
					locate(17, 27); printf(YELLOW"Please enter valid input.                                                                          "RESET);
					cardInserted = 0;
					break;
				}
				if (userInput == -2){ playerWhoEnds = player; player = 5; cardInserted = 1; gameEnd = 1; break; }
				locate(17, 27);
				printf("                                                                                                   ");
				if (userInput == -1 && turn != 333){
					player++; pass++; locate(75, 24); printf("                 "); locate(17, 27);
					printf("                                                                                                   "); turn = 5; break;
				}
				if (userInput == -1 && turn == 333){
					locate(17, 27); printf(YELLOW"You cannot pass now.                                                                               "RESET);
					locate(75, 24); printf("                 "); cardInserted = 0; break;
				}
				locate(55, line);
				//check if card is already taken
				if (player3[userInput - 1] != NULL){
					//setting currentCard and previoius
					currentCard = (card*)malloc(sizeof(card));
					previousCard = (card*)malloc(sizeof(card));
					if (turn != 333){
						previousCard->rank = pile[((cardPiled - 1))]->rank;
						previousCard->suit = pile[((cardPiled - 1))]->suit;
					}
					currentCard->rank = player3[userInput - 1]->rank;
					currentCard->suit = player3[userInput - 1]->suit;

					//Check if card with rank of 2 is entered
					if (currentCard->rank == 2){
						clearPile(pile);
						cardPiled = 0;
						clearPileOutput();
						locate(75, 24); printf("                 ");
						player3[userInput - 1] = NULL;
						cardLine = 9 + (userInput);
						locate(91, cardLine);
						printf("-");
						locate(17, 27);
						turn = 333;//i have just let it 333 coz i am using it as 333 for player 3
						line = 10;
						pass = 0;
						printf(YELLOW"Enter one more card because you entered a card with rank of 2.                                     "RESET);
						break;
					}
					if (turn == 333){ compareResult = 1; }
					else {
						//Check if card has higher value
						compareResult = higher(currentCard, previousCard);
					}
					if (compareResult == 1){
						pile[cardPiled] = (card *)malloc(sizeof(card));
						pile[cardPiled]->rank = player3[userInput - 1]->rank;
						pile[cardPiled]->suit = player3[userInput - 1]->suit;
						int SuitName = pile[cardPiled]->suit;
						printf("%d of %s", pile[cardPiled]->rank, suitNames[SuitName]);
						//printf("*%d*", pass);
						locate(75, 24);
						printf("                 ");
						cardLine = 9 + (userInput);
						locate(91, cardLine);
						printf("-");
						locate(17, 27);
						printf("                                                                                                   ");
						player3[userInput - 1] = NULL;
						cardInserted = 1;
						cardPiled++;
						turn = 5;
						win = checkwin(player1, player2, player3, player4);
						if (win == 3){ player = 5;	gameEnd = 1; break; }
						pass = 0;
						player++;
						line++;
					}
					else{
						locate(17, 27);
						printf(YELLOW"Card has lower or equal value than one inserted previously. Insert card with higher value.         "RESET);
						locate(75, 24);
						printf("                 ");
					}
				}
				else {
					printf("\a");
					locate(17, 27);
					printf(YELLOW"Card is already taken.                                                                             "RESET);
					locate(75, 24);
					printf("                 ");
				}
			}
			cardInserted = 0;
		}
		while (player == 4){
			while (cardInserted == 0){
				if (pass == 4){
					clearPile(pile);
					cardPiled = 0;
					clearPileOutput();
					locate(17, 27);
					printf(YELLOW"You started pass and turn is back to you so restart the turn by putting new card.                  "RESET);
					turn = 444;
					pass = 0;
					line = 10;
				}
				locate(94, 24);
				scanf("%d", &userInput);
				//check if right input is inserted
				if (userInput > 13 || userInput <-2 || userInput == 0){
					locate(94, 24); printf("                  ");
					locate(17, 27); printf(YELLOW"Please enter valid input.                                                                          "RESET);
					cardInserted = 0;
					break;
				}
				if (userInput == -2){ playerWhoEnds = player; player = 5;  cardInserted = 1; gameEnd = 1; break; }
				locate(17, 27);
				printf("                                                                                                   ");
				if (userInput == -1 && turn != 444){
					player = 1; pass++;	locate(94, 24); printf("                  "); 	locate(17, 27);
					printf("                                                                                                   "); turn = 5; break;
				}
				if (userInput == -1 && turn == 444){
					locate(17, 27); printf(YELLOW"You cannot pass now.                                                                               "RESET);
					locate(94, 24); printf("                  "); cardInserted = 0; break;
				}
				locate(55, line);
				//check if card is already taken
				if (player4[userInput - 1] != NULL){
					//setting currentCard and previoius
					currentCard = (card*)malloc(sizeof(card));
					previousCard = (card*)malloc(sizeof(card));
					if (turn != 444){
						previousCard->rank = pile[((cardPiled - 1))]->rank;
						previousCard->suit = pile[((cardPiled - 1))]->suit;
					}
					currentCard->rank = player4[userInput - 1]->rank;
					currentCard->suit = player4[userInput - 1]->suit;

					//Check if card with rank of 2 is entered
					if (currentCard->rank == 2){
						clearPile(pile);
						cardPiled = 0;
						clearPileOutput();
						locate(94, 24); printf("                  ");
						player4[userInput - 1] = NULL;
						cardLine = 9 + (userInput);
						locate(111, cardLine);
						printf("-");
						locate(17, 27);
						turn = 444;//i have just let it 444 coz i am using it as 444 for player 4
						line = 10;
						pass = 0;
						printf(YELLOW"Enter one more card because you entered a card with rank of 2.                                     "RESET);
						break;
					}
					if (turn == 444){ compareResult = 1; }
					else {
						//Check if card has higher value
						compareResult = higher(currentCard, previousCard);
					}
					if (compareResult == 1){
						pile[cardPiled] = (card *)malloc(sizeof(card));
						pile[cardPiled]->rank = player4[userInput - 1]->rank;
						pile[cardPiled]->suit = player4[userInput - 1]->suit;
						int SuitName = pile[cardPiled]->suit;
						printf("%d of %s", pile[cardPiled]->rank, suitNames[SuitName]);
						//printf("*%d*", pass);
						locate(94, 24);
						printf("                  ");
						cardLine = 9 + (userInput);
						locate(111, cardLine);
						printf("-");
						locate(17, 27);
						printf("                                                                                                   ");
						player4[userInput - 1] = NULL;
						cardInserted = 1;
						cardPiled++;
						win = checkwin(player1, player2, player3, player4);
						if (win == 4){ player = 5;	gameEnd = 1; break; }
						player = 1;
						line++;
						pass = 0;
						turn = 5;
					}
					else{
						locate(17, 27);
						printf(YELLOW"Card has lower or equal value than one inserted previously. Insert card with higher value.         "RESET);
						locate(94, 24);
						printf("                  ");
					}
				}
				else {
					printf("\a");
					locate(17, 27);
					printf(YELLOW"Card is already taken.                                                                             "RESET);
					locate(94, 24);
					printf("                  ");
				}
			}
			cardInserted = 0;
		}
	}//if winner is found then we come out of this loop. OR game is ended in between	
	if (playerWhoEnds == 0){
		locate(17, 27);
		printf(YELLOW"Game is Ended. player%d is winner.                                                                 "RESET, win);
		locate(40, 27);
	}
	else{
		locate(17, 27);
		printf(YELLOW"Game ended without completion, by player%d.                                                        "RESET, playerWhoEnds);
		locate(59, 27);
	}
}//playGame() function ends here

int higher(card *currentCard, card *previousCard){
	if (currentCard->rank > previousCard->rank){ return 1; }
	else { return 0; }
}

void clearPile(card *pile[]){
	for (int index = 0; index < 52; index++){
		pile[index] = NULL;
	}
}

void clearPileOutput(){
	for (int fline = 10; fline < 25; fline++){
		locate(55, fline);
		printf("               ");
	}
}
int checkwin(card *player1[], card *player2[], card *player3[], card *player4[]){
	int found = 0;
	int ret = 0;
	if (found == 0){
		for (int index = 0; index < 13; index++){ if (player1[index] != NULL){ found = 1; break; } }
		if (found == 0){ ret = 1; }
	}
	if (found == 1){
		found = 0;
		for (int index = 0; index < 13; index++){ if (player2[index] != NULL){ found = 1; break; } }
		if (found == 0){ ret = 2; }
	}
	if (found == 1){
		found = 0;
		for (int index = 0; index < 13; index++){ if (player2[index] != NULL){ found = 1; break; } }
		if (found == 0){ ret = 3; }
	}
	if (found == 1){
		found = 0;
		for (int index = 0; index < 13; index++){ if (player4[index] != NULL){ found = 1; break; } }
		if (found == 0){ ret = 4; }
	}

	return ret;
}