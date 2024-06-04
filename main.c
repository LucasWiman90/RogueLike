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
int handleInput(int input, Player *player);
int playerMove(int y, int x, Player *player);

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

int screenSetup()
{
    initscr();
    printw("Hello world");
    noecho();
    refresh();

    return 1;
}

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
}

Player *playerSetup()
{
    Player *newPlayer;
    newPlayer = malloc(sizeof(Player));

    newPlayer->xPosition = 14;
    newPlayer->yPosition = 14;
    newPlayer->health = 20;

    playerMove(14, 14, newPlayer);

    return newPlayer;
}

int handleInput(int input, Player *player)
{
    switch(input)
    {
        //Move up
        case 'w':
        case 'W':
            playerMove(player->yPosition-1, player->xPosition, player);
            break;

        //Move down
        case 's':
        case 'S':
            playerMove(player->yPosition+1, player->xPosition, player);
            break;

        //Move left
        case 'a':
        case 'A':
            playerMove(player->yPosition, player->xPosition-1, player);
            break;

        //Move right
        case 'd':
        case 'D':
            playerMove(player->yPosition, player->xPosition+1, player);
            break;

        default:
            break;
    }
}

//Move the player, replace old position with floor.
//Update position and draw the player in the new position and move the cursor
int playerMove(int y, int x, Player *player)
{
    mvprintw(player->yPosition, player->xPosition, ".");

    player->yPosition = y;
    player->xPosition = x;

    mvprintw(player->yPosition, player->xPosition, "@");
    move(player->yPosition,player->xPosition);
}