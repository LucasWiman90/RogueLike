#include <ncurses.h>
#include <stdlib.h>

#include <time.h>

typedef struct Position
{
    int y;
    int x;
    // TILE_TYPE tile;
} Position;

typedef struct Room
{
    Position position;
    int height;
    int width;
    Position ** doors;
    //Monster ** monsters;
    //Item ** items;
} Room;

typedef struct Player
{
    Position position;
    int health;
    // Room * room;
} Player;

//Utility functions
void *safe_malloc(size_t size);
int screenSetup();

//Player functions
Player *playerSetup();
void handleInput(int input, Player *player);
void playerMove(int y, int x, Player *player);
void checkPosition(int newY, int newX, Player *player);

//Room functions
Room ** mapSetup();
Room * createRoom(int y, int x, int height, int width);
void drawRoom(Room *room);
void freeRoom(Room *room);
void freeMap(Room **map, int size);

int main() 
{
    Player *player;
    Room ** rooms;
    int ch;

    //Setup the screen
    screenSetup();

    //Setup the map
    rooms = mapSetup(6);

    //Setup the player
    player = playerSetup();

    //Main game loop
    while((ch = getch()) != 'q')
    {
        handleInput(ch, player);
    }

    //Free player memory
    free(player);

    //Free the map memory
    freeMap(rooms, 6);

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

    srand(time(NULL));

    return 0;
}

/**
 * @brief Initializes and sets up the map with rooms.
 * @param size How many rooms the map should have.
 * @return Returns a pointer to an array of Room pointers when succesful
 * */
Room ** mapSetup(int size)
{
    Room ** rooms;
    rooms = safe_malloc(sizeof(Room) * size);

    // Initialize all elements to NULL
    for (int i = 0; i < 6; i++) {
        rooms[i] = NULL;
    }

    rooms[0] = createRoom(13, 13, 6, 8);
    drawRoom(rooms[0]);

    rooms[1] = createRoom(2, 40, 6, 8);
    drawRoom(rooms[1]);

    rooms[2] = createRoom(10, 40, 6, 12);
    drawRoom(rooms[2]);

    return rooms;
}

/**
 * @brief Allocates memory with error checking.
 *
 * This function allocates memory using malloc and checks if the allocation
 * was successful. If malloc fails and the requested size is greater than 0,
 * it prints an error message using perror and exits the program with failure.
 *
 * @param size The size of the memory block to allocate.
 * @return A pointer to the allocated memory block, or NULL if the allocation fails.
 */
void *safe_malloc(size_t size)
{
    void *ptr = malloc(size);

    if (!ptr && (size > 0)) {
      perror("malloc failed!");
      exit(EXIT_FAILURE);
    }

    return ptr;
}

/**
 * @brief Creates a room.
 *
 * This function creates a room with x,y coordinates (top left corner).
 * And with a defined width and height.
 *
 * @param size The size of the memory block to allocate.
 * @return A pointer to the newly created room.
 */
Room * createRoom(int y, int x, int height, int width)
{
    Room * newRoom;
    newRoom = safe_malloc(sizeof(Room));

    newRoom->position.y = y;
    newRoom->position.x = x;
    newRoom->height = height;
    newRoom->width = width;

    newRoom->doors = safe_malloc(sizeof(Position) * 4);

    //Top door
    newRoom->doors[0] = safe_malloc(sizeof(Position));
    newRoom->doors[0]->x = rand() % (width - 2) + newRoom->position.x + 1;
    newRoom->doors[0]->y = newRoom->position.y;

    //Bottom door
    newRoom->doors[1] = safe_malloc(sizeof(Position));
    newRoom->doors[1]->x = rand() % (width - 2) + newRoom->position.x + 1;
    newRoom->doors[1]->y = newRoom->position.y + newRoom->height - 1;

    //Left door
    newRoom->doors[2] = safe_malloc(sizeof(Position));
    newRoom->doors[2]->y = rand() % (height - 2) + newRoom->position.y + 1;
    newRoom->doors[2]->x = newRoom->position.x;

    //Right door
    newRoom->doors[3] = safe_malloc(sizeof(Position));
    newRoom->doors[3]->y = rand() % (height - 2) + newRoom->position.y + 1;
    newRoom->doors[3]->x = newRoom->position.x + width - 1;

    return newRoom;
}

/**
 * @brief Draws a room.
 *
 * This function draws a room
 *
 * @param room A pointer to a room to be drawn.
 */
void drawRoom(Room *room)
{
    int y;
    int x;

    //Draw top and bottom of room
    for (x = room->position.x; x < room->position.x + room->width; x++)
    {
        mvprintw(room->position.y, x, "-"); /*top*/
        mvprintw(room->position.y + room->height - 1, x, "-"); /*bottom*/
    }

    //Draw the floors and sidewalls
    for(y = room->position.y + 1; y < room->position.y + room->height - 1; y++)
    {
        /*draw side walls*/
        mvprintw(y, room->position.x, "|");
        mvprintw(y, room->position.x + room->width - 1, "|");

        /*draw floors*/
        for(x = room->position.x + 1; x < room->position.x + room->width - 1; x++)
        {
            mvprintw(y, x, ".");
        }
    }

    //Draw doors
    mvprintw(room->doors[0]->y, room->doors[0]->x, "+");
    mvprintw(room->doors[1]->y, room->doors[1]->x, "+");
    mvprintw(room->doors[2]->y, room->doors[2]->x, "+");
    mvprintw(room->doors[3]->y, room->doors[3]->x, "+");
}

/**
 * @brief Frees memory allocated for a single room.
 * @param room: Pointer to the room to be freed.
 */
void freeRoom(Room *room) 
{
    if (room != NULL && room->doors != NULL) {
        //Loop through each door position and free it
        for(int i = 0; i < 4; i++)
        {
            free(room->doors[i]);
        }

        //Free the doors array itself.
        free(room->doors);
    }
    //Free the room structure
    free(room);
}

/**
 * @brief Frees memory allocated for an array of rooms.
 * @param map: Pointer to the array of room pointers.
 * @param size: The number of rooms in the array.
 */
void freeMap(Room **map, int size) 
{
    if (map != NULL) 
    {
        for (int i = 0; i < size; i++) 
        {
            if (map[i] != NULL) {
                freeRoom(map[i]);
            }
        }
        free(map);
    }
}

/**
 * @brief Sets up the player
 * @param None
 * @return Returns a Player pointer when succesful
 * */
Player* playerSetup()
{
    Player *newPlayer;
    newPlayer = safe_malloc(sizeof(Player));

    newPlayer->position.x = 14;
    newPlayer->position.y = 14;
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
            newY = player->position.y-1;
            newX = player->position.x;
            break;

        //Move down
        case 's':
        case 'S':
            newY = player->position.y+1;
            newX = player->position.x;
            break;

        //Move left
        case 'a':
        case 'A':
            newY = player->position.y;
            newX = player->position.x-1;
            break;

        //Move right
        case 'd':
        case 'D':
            newY = player->position.y;
            newX = player->position.x+1;
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
            move(player->position.y, player->position.x);
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
    mvprintw(player->position.y, player->position.x, ".");

    player->position.y = y;
    player->position.x = x;

    mvprintw(player->position.y, player->position.x, "@");
    move(player->position.y,player->position.x);
}