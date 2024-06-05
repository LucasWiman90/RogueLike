#include <ncurses.h>
#include <stdlib.h>

typedef struct Player
{
    int xPosition;
    int yPosition;
    int health;
} Player;

int screenSetup();
int mapSetup();
Player *playerSetup();
void handleInput(int input, Player *player);
void playerMove(int y, int x, Player *player);
void checkPosition(int newY, int newX, Player *player);

int main() 
{
    Player *player;
    int ch;

    //Setup the screen
    screenSetup();

    //Setup the map
    mapSetup();

    //Setup the player
    player = playerSetup();

    //Main game loop
    while((ch = getch()) != 'q')
    {
        handleInput(ch, player);
    }

    endwin();

    return 0;
}

/**
 * @brief Draws the screen
 * @param None
 * @return Returns 0 when succesful
 * */
int screenSetup()
{
    initscr();
    printw("Hello world");
    noecho();
    refresh();

    return 0;
}

/**
 * @brief Draws the map
 * @param None
 * @return Returns 0 when succesful
 * */
int mapSetup()
{
    mvprintw(13, 13, "--------");
    mvprintw(14, 13, "|......|");
    mvprintw(15, 13, "|......|");
    mvprintw(16, 13, "|......|");
    mvprintw(17, 13, "|......|");
    mvprintw(18, 13, "--------");

    mvprintw(2, 40, "--------");
    mvprintw(3, 40, "|......|");
    mvprintw(4, 40, "|......|");
    mvprintw(5, 40, "|......|");
    mvprintw(6, 40, "|......|");
    mvprintw(7, 40, "--------");

    mvprintw(10, 40, "------------");
    mvprintw(11, 40, "|..........|");
    mvprintw(12, 40, "|..........|");
    mvprintw(13, 40, "|..........|");
    mvprintw(14, 40, "|..........|");
    mvprintw(15, 40, "------------");

    return 0;
}

/**
 * @brief Sets up the player
 * @param None
 * @return Returns a Player pointer when succesful
 * */
Player* playerSetup()
{
    Player *newPlayer;
    newPlayer = malloc(sizeof(Player));

    newPlayer->xPosition = 14;
    newPlayer->yPosition = 14;
    newPlayer->health = 20;

    playerMove(14, 14, newPlayer);

    return newPlayer;
}

/**
 * @brief Reads player input
 * @param input: The player key input
 * @param player: Pointer to the player
 * */
void handleInput(int input, Player *player)
{
    int newY;
    int newX;
    switch(input)
    {
        //Move up
        case 'w':
        case 'W':
            newY = player->yPosition-1;
            newX = player->xPosition;
            break;

        //Move down
        case 's':
        case 'S':
            newY = player->yPosition+1;
            newX = player->xPosition;
            break;

        //Move left
        case 'a':
        case 'A':
            newY = player->yPosition;
            newX = player->xPosition-1;
            break;

        //Move right
        case 'd':
        case 'D':
            newY = player->yPosition;
            newX = player->xPosition+1;
            break;

        default:
            break;
    }

    checkPosition(newY, newX, player);
}

/**
 * @brief Checks player position to determine collision
 * @param newY: New y-position
 * @param newX: New x-position
 * @param player: Pointer to the player
 * */
void checkPosition(int newY, int newX, Player *player)
{
    int space;
    switch(mvinch(newY, newX))
    {
        case '.':
            playerMove(newY, newX, player);
            break;
        default:
            move(player->yPosition, player->xPosition);
            break;
    }
}

/**
 * @brief Moves the player to a new position. Replace old position
 * with a floor tile '.', then draw the player again '@'
 * @param y: Y-coordinate of the player
 * @param x: X-coordinate of the player
 * @param player: Pointer to the player
 * */
void playerMove(int y, int x, Player *player)
{
    mvprintw(player->yPosition, player->xPosition, ".");

    player->yPosition = y;
    player->xPosition = x;

    mvprintw(player->yPosition, player->xPosition, "@");
    move(player->yPosition,player->xPosition);
}