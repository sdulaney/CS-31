//Stewart Dulaney
//Code for Project 7


// robots.cpp

// Portions you are to complete are marked with a TODO: comment.
// We've provided some incorrect return statements (so indicated) just
// to allow this skeleton program to compile and run, albeit incorrectly.
// The first thing you probably want to do is implement the utterly trivial
// functions (marked TRIVIAL).  Then get Arena::display going.  That gives
// you more flexibility in the order you tackle the rest of the functionality.
// As you finish implementing each TODO: item, remove its TODO: comment.

#include <iostream>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <cctype>
#include <cassert>
using namespace std;

///////////////////////////////////////////////////////////////////////////
// Manifest constants
///////////////////////////////////////////////////////////////////////////

const int MAXROWS = 20;             // max number of rows in the arena
const int MAXCOLS = 20;             // max number of columns in the arena
const int MAXROBOTS = 100;          // max number of robots allowed
const int MAXCHANNELS = 3;          // max number of channels
const double WALL_DENSITY = 0.11;   // density of walls

const int NORTH = 0;
const int EAST  = 1;
const int SOUTH = 2;
const int WEST  = 3;
const int NUMDIRS = 4;

const int EMPTY = 0;
const int WALL  = 1;

///////////////////////////////////////////////////////////////////////////
// Type definitions
///////////////////////////////////////////////////////////////////////////

class Arena;  // This is needed to let the compiler know that Arena is a
              // type name, since it's mentioned in the Robot declaration.

class Robot
{
  public:
        // Constructor
    Robot(Arena* ap, int r, int c, int channel);

        // Accessors
    int  row() const;
    int  col() const;
    int  channel() const;
    bool isDead() const;

        // Mutators
    void forceMove(int dir);
    void move();

  private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    int    m_channel;
	int    m_damage;
};

class Player
{
  public:
        // Constructor
    Player(Arena *ap, int r, int c);

        // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;

        // Mutators
    string stand();
    string move(int dir);
    void   setDead();

  private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    bool   m_dead;
};

class Arena
{
  public:
        // Constructor/destructor
    Arena(int nRows, int nCols);
    ~Arena();

        // Accessors
    int     rows() const;
    int     cols() const;
    Player* player() const;
    int     robotCount() const;
    int     getCellStatus(int r, int c) const;
    int     numberOfRobotsAt(int r, int c) const;
    void    display(string msg) const;

        // Mutators
    void   setCellStatus(int r, int c, int status);
    bool   addRobot(int r, int c, int channel);
    bool   addPlayer(int r, int c);
    string moveRobots(int channel, int dir);

  private:
    int     m_grid[MAXROWS][MAXCOLS];
    int     m_rows;
    int     m_cols;
    Player* m_player;
    Robot*  m_robots[MAXROBOTS];
    int     m_nRobots;

        // Helper functions
    void checkPos(int r, int c) const;
};

class Game
{
  public:
        // Constructor/destructor
    Game(int rows, int cols, int nRobots);
    ~Game();

        // Mutators
    void play();

  private:
    Arena* m_arena;

        // Helper functions
    string takePlayerTurn();
    string takeRobotsTurn();
};

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function declarations
///////////////////////////////////////////////////////////////////////////

int randInt(int lowest, int highest);
bool charToDir(char ch, int& dir);
bool attemptMove(const Arena& a, int dir, int& r, int& c);
bool recommendMove(const Arena& a, int r, int c, int& bestDir);
void clearScreen();
char intToChar(int n);

///////////////////////////////////////////////////////////////////////////
//  Robot implementation
///////////////////////////////////////////////////////////////////////////

Robot::Robot(Arena* ap, int r, int c, int channel)
{
    if (ap == NULL)
    {
        cout << "***** A robot must be created in some Arena!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
    {
        cout << "***** Robot created with invalid coordinates (" << r << ","
             << c << ")!" << endl;
        exit(1);
    }
    if (channel < 1  ||  channel > MAXCHANNELS)
    {
        cout << "***** Robot created with invalid channel " << channel << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    m_channel = channel;
	m_damage = 0;
}

int Robot::row() const
{
    return m_row;
}

int Robot::col() const
{
    return m_col;
}

int Robot::channel() const
{
    return m_channel;
}

bool Robot::isDead() const
{
	if(m_damage == 3)
		return true;
	else
		return false;  
}

void Robot::forceMove(int dir)
{
	if(!isDead())
	{
		if(!attemptMove(*m_arena, dir, m_row, m_col))
			m_damage += 1;
	}
}

void Robot::move()
{
      // Attempt to move in a random direction; if we can't move, don't move
    if (!isDead())
        attemptMove(*m_arena, randInt(0, NUMDIRS-1), m_row, m_col);
}

///////////////////////////////////////////////////////////////////////////
//  Player implementation
///////////////////////////////////////////////////////////////////////////

Player::Player(Arena* ap, int r, int c)
{
    if (ap == NULL)
    {
        cout << "***** The player must be created in some Arena!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
    {
        cout << "**** Player created with invalid coordinates (" << r
             << "," << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    m_dead = false;
}

int Player::row() const
{
    return m_row;
}

int Player::col() const
{
    return m_col;
}

string Player::stand()
{
    return "Player stands.";
}

string Player::move(int dir)
{
	if(attemptMove(*m_arena, dir, m_row, m_col))
	{
		if(m_arena->numberOfRobotsAt(m_row, m_col) > 0)
		{
			setDead();
			return "Player walked into a robot and died.";
		}
		if(dir == NORTH)
			return "Player moved north.";
		else if(dir == SOUTH)
			return "Player moved south.";
		else if(dir == EAST)
			return "Player moved east.";
		else if(dir == WEST)
			return "Player moved west.";
		else
			exit(1);
	}
	else
		return "Player couldn't move; player stands.";
}

bool Player::isDead() const
{
	if(m_dead == true)
		return true;
	else
		return false;  
}

void Player::setDead()
{
    m_dead = true;
}

///////////////////////////////////////////////////////////////////////////
//  Arena implementation
///////////////////////////////////////////////////////////////////////////

Arena::Arena(int nRows, int nCols)
{
    if (nRows <= 0  ||  nCols <= 0  ||  nRows > MAXROWS  ||  nCols > MAXCOLS)
    {
        cout << "***** Arena created with invalid size " << nRows << " by "
             << nCols << "!" << endl;
        exit(1);
    }
    m_rows = nRows;
    m_cols = nCols;
    m_player = NULL;
    m_nRobots = 0;
    for (int r = 1; r <= m_rows; r++)
        for (int c = 1; c <= m_cols; c++)
            setCellStatus(r, c, EMPTY);
}

Arena::~Arena()
{
	delete m_player;
	for(int k = 0; k < m_nRobots; k++)
		delete m_robots[k];
}

int Arena::rows() const
{
    return m_rows;
}

int Arena::cols() const
{
    return m_cols;
}

Player* Arena::player() const
{
    return m_player;
}

int Arena::robotCount() const
{
    return m_nRobots;
}

int Arena::getCellStatus(int r, int c) const
{
    checkPos(r, c);
    return m_grid[r-1][c-1];
}

int Arena::numberOfRobotsAt(int r, int c) const
{
	int numberRobots = 0;
	for(int k = 0; k < m_nRobots; k++)
	{
		if(m_robots[k]->row() == r && m_robots[k]->col() == c)
			numberRobots++;
		else
			continue;
	}
    return numberRobots;
}

void Arena::display(string msg) const
{
    char displayGrid[MAXROWS][MAXCOLS];
    int r, c;
    
      // Fill displayGrid with dots (empty) and stars (wall)
    for (r = 1; r <= rows(); r++)
        for (c = 1; c <= cols(); c++)
            displayGrid[r-1][c-1] = (getCellStatus(r,c) == EMPTY ? '.' : '*');

      // Indicate robot positions by their channels.  If more than one robot
      // occupies a cell, show just one (any one will do).
	for(int k = 0; k < m_nRobots; k++)
	{
		displayGrid[m_robots[k]->row()-1][m_robots[k]->col()-1] = intToChar(m_robots[k]->channel());
	}

      // Indicate player's position
    if (m_player != NULL)
      displayGrid[m_player->row()-1][m_player->col()-1] = (m_player->isDead() ? 'X' : '@');

      // Draw the grid
    clearScreen();
    for (r = 1; r <= rows(); r++)
    {
        for (c = 1; c <= cols(); c++)
            cout << displayGrid[r-1][c-1];
        cout << endl;
    }
    cout << endl;

      // Write message, robot, and player info
    if (msg != "")
        cout << msg << endl;
    cout << "There are " << robotCount() << " robots remaining." << endl;
    if (m_player == NULL)
        cout << "There is no player!" << endl;
    else if (m_player->isDead())
        cout << "The player is dead." << endl;
}

void Arena::setCellStatus(int r, int c, int status)
{
    checkPos(r, c);
    m_grid[r-1][c-1] = status;
}

bool Arena::addRobot(int r, int c, int channel)
{
    if (m_nRobots == MAXROBOTS)
         return false;
    m_robots[m_nRobots] = new Robot(this, r, c, channel);
    m_nRobots++;
    return true;
}

bool Arena::addPlayer(int r, int c)
{
    if (m_player != NULL)
        return false;
    m_player = new Player(this, r, c);
    return true;
}

string Arena::moveRobots(int channel, int dir)
{
      // Robots on the channel will respond with probability 1/2
    bool willRespond = (randInt(0, 1) == 0);

      // Move all robots
    int nRobotsOriginally = m_nRobots;

	if(willRespond)
	{
		for(int k = 0; k < m_nRobots; k++)
		{
			if(m_robots[k]->channel() == channel)
			{
				m_robots[k]->forceMove(dir);
			}
			else
				m_robots[k]->move();
			if(m_robots[k]->isDead())
			{
				delete m_robots[k];
				for(int i = k; i < m_nRobots - 1 ; i++)
					m_robots[k] = m_robots[k + 1];
				m_robots[m_nRobots-1] = NULL;
				m_nRobots--;
			}
		}
	}
	else
		for(int k = 0; k < m_nRobots; k++)
		{
			m_robots[k]->move();
		}

	for(int k = 0; k < m_nRobots; k++)
	{
		if(m_player->row() == m_robots[k]->row() && m_player->col() == m_robots[k]->col())
		{
			m_player->setDead();
			break;
		}
	}

    if (m_nRobots < nRobotsOriginally)
        return "Some robots have been destroyed.";
    else
        return "No robots were destroyed.";
}

void Arena::checkPos(int r, int c) const
{
    if (r < 1  ||  r > m_rows  ||  c < 1  ||  c > m_cols)
    {
        cout << "***** " << "Invalid arena position (" << r << ","
             << c << ")" << endl;
        exit(1);
    }
}

///////////////////////////////////////////////////////////////////////////
//  Game implementation
///////////////////////////////////////////////////////////////////////////

Game::Game(int rows, int cols, int nRobots)
{
    if (nRobots < 0  ||  nRobots > MAXROBOTS)
    {
        cout << "***** Game created with invalid number of robots:  "
             << nRobots << endl;
        exit(1);
    }
    int nEmpty = rows * cols - nRobots - 1;  // 1 for Player
    if (nEmpty < 0)
    {
        cout << "***** Game created with a " << rows << " by "
             << cols << " arena, which is too small too hold a player and "
             << nRobots << " robots!" << endl;
        exit(1);
    }

        // Create arena
    m_arena = new Arena(rows, cols);

        // Add some walls in WALL_DENSITY of the empty spots
    assert(WALL_DENSITY >= 0  &&  WALL_DENSITY <= 1);
    int nWalls = static_cast<int>(WALL_DENSITY * nEmpty);
    while (nWalls > 0)
    {
        int r = randInt(1, rows);
        int c = randInt(1, cols);
        if (m_arena->getCellStatus(r, c) == WALL)
            continue;
        m_arena->setCellStatus(r, c, WALL);
        nWalls--;
    }

        // Add player
    int rPlayer;
    int cPlayer;
    do
    {
        rPlayer = randInt(1, rows);
        cPlayer = randInt(1, cols);
    } while (m_arena->getCellStatus(rPlayer, cPlayer) != EMPTY);
    m_arena->addPlayer(rPlayer, cPlayer);

      // Populate with robots
    while (nRobots > 0)
    {
        int r = randInt(1, rows);
        int c = randInt(1, cols);
        if (m_arena->getCellStatus(r,c) != EMPTY  ||  (r == rPlayer && c == cPlayer))
            continue;
        m_arena->addRobot(r, c, randInt(1, MAXCHANNELS));
        nRobots--;
    }
}

Game::~Game()
{
    delete m_arena;
}

string Game::takePlayerTurn()
{
    for (;;)
    {
        cout << "Your move (n/e/s/w/x or nothing): ";
        string playerMove;
        getline(cin, playerMove);

        Player* player = m_arena->player();
        int dir;

        if (playerMove.size() == 0)
        {
            if (recommendMove(*m_arena, player->row(), player->col(), dir))
                return player->move(dir);
            else
                return player->stand();
        }
        else if (playerMove.size() == 1)
        {
            if (tolower(playerMove[0]) == 'x')
                return player->stand();
            else if (charToDir(playerMove[0], dir))
                return player->move(dir);
        }
        cout << "Player move must be nothing, or 1 character n/e/s/w/x." << endl;
    }
}

string Game::takeRobotsTurn()
{
    for (;;)
    {
        cout << "Broadcast (e.g., 2n): ";
        string broadcast;
        getline(cin, broadcast);
        if (broadcast.size() != 2)
            cout << "Broadcast must be channel followed by direction." << endl;
        else if (broadcast[0] < '1'  ||  broadcast[0] > '0'+MAXCHANNELS)
            cout << "Channel must be a valid digit." << endl;
        else
        {
            int dir;
            if (charToDir(broadcast[1], dir))
                return m_arena->moveRobots(broadcast[0]-'0', dir);
            else
                cout << "Direction must be n, e, s, or w." << endl;
        }
    }
}

void Game::play()
{
    m_arena->display("");
    while ( ! m_arena->player()->isDead()  &&  m_arena->robotCount() > 0)
    {
        string msg = takePlayerTurn();
        m_arena->display(msg);
        Player* player = m_arena->player();
        if (player->isDead())
            break;
        msg = takeRobotsTurn();
        m_arena->display(msg);
    }
    if (m_arena->player()->isDead())
        cout << "You lose." << endl;
    else
        cout << "You win." << endl;
}

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function implementation
///////////////////////////////////////////////////////////////////////////

  // Return a uniformly distributed random int from lowest to highest, inclusive
int randInt(int lowest, int highest)
{
    if (highest < lowest)
        swap(highest, lowest);
    return lowest + (rand() % (highest - lowest + 1));
}

bool charToDir(char ch, int& dir)
{
    switch (tolower(ch))
    {
      default:  return false;
      case 'n': dir = NORTH; break;
      case 'e': dir = EAST;  break;
      case 's': dir = SOUTH; break;
      case 'w': dir = WEST;  break;
    }
    return true;
}

bool attemptMove(const Arena& a, int dir, int& r, int& c)
{
	if(dir == NORTH)
	{
		if( (r - 1) <= 0 || a.getCellStatus((r - 1), c) == WALL)
			return false;
		else
		{
			r -= 1;
			return true;
		}
	}
	else if(dir == SOUTH)
	{
		if( (r + 1) > a.rows() || a.getCellStatus((r + 1), c) == WALL)
			return false;
		else
		{
			r += 1;
			return true;
		}
	}
	else if(dir == EAST)
	{
		if( (c + 1) > a.cols() || a.getCellStatus(r, (c + 1)) == WALL)
			return false;
		else
		{
			c += 1;
			return true;
		}
	}
	else if(dir == WEST)
	{
		if( (c - 1) <= 0 || a.getCellStatus(r, (c - 1)) == WALL)
			return false;
		else
		{
			c -= 1;
			return true;
		}
	}
	else 
		exit(1);
}

  // Recommend a move for a player at (r,c):  A false return means the
  // recommendation is that the player should stand; otherwise, bestDir is
  // set to the recommended direction to move.
bool recommendMove(const Arena& a, int r, int c, int& bestDir)
{
	int surrRobots = 0;
	int surrRobotsN = 0;
	int surrRobotsS = 0;
	int surrRobotsE = 0;
	int surrRobotsW = 0;

	//Count how many robots player is next to if moves x/n/s/e/w
	if(a.numberOfRobotsAt((r-1), c) > 0)
		surrRobots += a.numberOfRobotsAt((r-1), c);
	if(a.numberOfRobotsAt((r+1), c) > 0)
		surrRobots += a.numberOfRobotsAt((r+1), c);
	if(a.numberOfRobotsAt(r, (c+1)) > 0)
		surrRobots += a.numberOfRobotsAt(r, (c+1));
	if(a.numberOfRobotsAt(r, (c-1)) > 0)
		surrRobots += a.numberOfRobotsAt(r, (c-1));

	if( (r - 1) > 0)
	{
		if((c-1) > 0 && a.numberOfRobotsAt((r-1), (c-1)) > 0)
			surrRobots += a.numberOfRobotsAt((r-1), (c-1));
		if((r-2) > 0 && a.numberOfRobotsAt((r-2), c) > 0)
			surrRobotsN += a.numberOfRobotsAt((r-2), c);
		if((c+1) <= a.cols() && a.numberOfRobotsAt((r-1), (c+1)) > 0)
			surrRobotsN += a.numberOfRobotsAt((r-1), (c+1));
	}

	if( (r + 1) <= a.rows())
	{
		if((c-1) > 0 && a.numberOfRobotsAt((r+1), (c-1)) > 0)
			surrRobotsS += a.numberOfRobotsAt((r+1), (c-1));
		if((r+2) <= a.rows() && a.numberOfRobotsAt((r+2), c) > 0)
			surrRobotsS += a.numberOfRobotsAt((r+2), c);
		if((c+1) <= a.cols() && a.numberOfRobotsAt((r+1), (c+1)) > 0)
			surrRobotsS += a.numberOfRobotsAt((r+1), (c+1));
	}

	if( (c+1) <= a.cols())
	{
		if((r+1) <= a.rows() && a.numberOfRobotsAt((r+1), (c+1)) > 0)
			surrRobotsE += a.numberOfRobotsAt((r+1), (c+1));
		if((c+2) <= a.cols() && a.numberOfRobotsAt(r, (c+2)) > 0)
			surrRobotsE += a.numberOfRobotsAt(r, (c+2));
		if((r-1) > 0 && a.numberOfRobotsAt((r-1), (c+1)) > 0)
			surrRobotsE += a.numberOfRobotsAt((r-1), (c+1));
	}

	if( (c-1) > 0)
	{
		if((r+1) <= a.rows() && a.numberOfRobotsAt((r+1), (c-1)) > 0)
			surrRobotsW += a.numberOfRobotsAt((r+1), (c-1));
		if((c-2) > 0 && a.numberOfRobotsAt(r, (c-2)) > 0)
			surrRobotsW += a.numberOfRobotsAt(r, (c-2));
		if((r-1) > 0 && a.numberOfRobotsAt((r-1), (c-1)) > 0)
			surrRobotsW += a.numberOfRobotsAt((r-1), (c-1));
	}

	//Determine which move will put player next to least number of robots and return value accordingly
	if(surrRobots <= surrRobotsN && surrRobots <= surrRobotsS && surrRobots <= surrRobotsE && surrRobots <= surrRobotsW)
		return false;

	if((r-1) > 0 &&a.numberOfRobotsAt((r - 1), c) == 0  && a.getCellStatus((r - 1), c) != WALL)
	{
		if(surrRobotsN <= surrRobotsS && surrRobotsN <= surrRobotsE && surrRobotsN <= surrRobotsW)
		{
			bestDir = NORTH;
			return true;
		}
	}
	if((r+1) <= a.rows() && a.numberOfRobotsAt((r + 1), c) == 0 && a.getCellStatus((r + 1), c) != WALL)
	{
		if(surrRobotsS <= surrRobotsN && surrRobotsS <= surrRobotsE && surrRobotsS <= surrRobotsW)
		{
			bestDir = SOUTH;
			return true;
		}
	}
	if((c+1) <= a.cols() && a.numberOfRobotsAt(r, (c+1)) == 0 && a.getCellStatus(r, (c+1)) != WALL)
	{
		if(surrRobotsE <= surrRobotsN && surrRobotsE <= surrRobotsS && surrRobotsE <= surrRobotsW)
		{
			bestDir = EAST;
			return true;
		}
	}
	if((c-1) > 0 && a.numberOfRobotsAt(r, (c-1)) == 0 && a.getCellStatus(r, (c-1)) != WALL)
	{
		if(surrRobotsW <= surrRobotsN && surrRobotsW <= surrRobotsS && surrRobotsW <= surrRobotsE)
		{
		bestDir = WEST;
		return true;
		}
	}
	return false; 
}

//Function that is passed a channel number of type int and returns the digit 
//character of type char that represents that channel number
char intToChar(int n)
{
	char c;
	switch(n)
	{
	case 1:
		c = '1';
		break;
	case 2:
		c = '2';
		break;
	case 3:
		c = '3';
		break;
	default:
		break;
	}
	return c;
}

#ifdef _MSC_VER  //  Microsoft Visual C++

#include <windows.h>

void clearScreen()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    COORD upperLeft = { 0, 0 };
    DWORD dwCharsWritten;
    FillConsoleOutputCharacter(hConsole, TCHAR(' '), dwConSize, upperLeft,
							&dwCharsWritten);
    SetConsoleCursorPosition(hConsole, upperLeft);
}

#else  // not Microsoft Visual C++, so assume UNIX interface

#include <cstring>

void clearScreen()
{
    static const char* term = getenv("TERM");
    static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal esc seq:  ESC [
    if (term == NULL  ||  strcmp(term, "dumb") == 0)
        cout << endl;
     else
        cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << flush;
}

#endif

///////////////////////////////////////////////////////////////////////////
// main()
///////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <ctime>
using namespace std;

void doBasicTests();

int main()
{
	//doBasicTests();

		// Initialize the random number generator
    srand(static_cast<unsigned int>(time(0)));

      // Create a game
      // Use this instead to create a mini-game:   Game g(3, 5, 2);
    Game g(10, 12, 50);

      // Play the game
    g.play();
}

#include <cassert>
#include <cstdlib>

using namespace std;

void thisFunctionWillNeverBeCalled()
{
      // If the student deleted or changed the manifest constants, this
      // won't compile.

    const bool b1 = (MAXROWS == 20 && MAXCOLS == 20 && MAXROBOTS == 100 &&
                     MAXCHANNELS == 3 && EMPTY == 0 && WALL == 1 &&
                     NORTH == 0 && EAST == 1 && SOUTH == 2 && WEST == 3);
    char a1[b1 ? 1 : -1];  // illegal negative size array if b1 is false

      // If the student deleted or changed the interfaces to the public
      // functions, this won't compile.  (This uses magic beyond the scope
      // of CS 31.)

    Robot r(static_cast<Arena*>(0), 1, 1, 1);
    int  (Robot::*pr1)() const = &Robot::row;
    int  (Robot::*pr2)() const = &Robot::col;
    int  (Robot::*pr3)() const = &Robot::channel;
    bool (Robot::*pr4)() const = &Robot::isDead;
    void (Robot::*pr5)(int)    = &Robot::forceMove;
    void (Robot::*pr6)()       = &Robot::move;

    Player p(static_cast<Arena*>(0), 1, 1);
    int  (Player::*pp1)() const = &Player::row;
    int  (Player::*pp2)() const = &Player::col;
    bool (Player::*pp3)() const = &Player::isDead;
    string (Player::*pp4)()     = &Player::stand;
    string (Player::*pp5)(int)  = &Player::move;
    void (Player::*pp6)()       = &Player::setDead;

    Arena a(1, 1);
    int  (Arena::*pa1)() const    = &Arena::rows;
    int  (Arena::*pa2)() const    = &Arena::cols;
    Player* (Arena::*pa3)() const = &Arena::player;
    int (Arena::*pa4)() const     = &Arena::robotCount;
    int (Arena::*pa5)(int,int) const = &Arena::getCellStatus;
    int (Arena::*pa6)(int,int) const = &Arena::numberOfRobotsAt;
    void (Arena::*pa7)(string) const = &Arena::display;
    void (Arena::*pa8)(int,int,int)  = &Arena::setCellStatus;
    bool (Arena::*pa9)(int,int,int)  = &Arena::addRobot;
    bool (Arena::*pa10)(int,int)     = &Arena::addPlayer;
    string (Arena::*pa11)(int,int)   = &Arena::moveRobots;

    Game g(1,1,1);
    void (Game::*pg1)() = &Game::play;
}

void doBasicTests()
{
    {
        Arena a(10, 20);
        a.addPlayer(2, 5);
        Player* pp = a.player();
        assert(pp->row() == 2  &&  pp->col() == 5  && ! pp->isDead());
        assert(pp->move(NORTH) == "Player moved north.");
        assert(pp->row() == 1  &&  pp->col() == 5  && ! pp->isDead());
        assert(pp->move(NORTH) == "Player couldn't move; player stands.");
        assert(pp->row() == 1  &&  pp->col() == 5  && ! pp->isDead());
        pp->setDead();
        assert(pp->row() == 1  &&  pp->col() == 5  && pp->isDead());
    }
    {
        Arena a(10, 20);
        a.setCellStatus(1, 4, WALL);
        a.setCellStatus(1, 5, WALL);
        a.setCellStatus(3, 4, WALL);
        a.setCellStatus(3, 5, WALL);
        a.setCellStatus(2, 3, WALL);
        a.setCellStatus(2, 6, WALL);
        a.addRobot(2, 4, 3);
        a.addPlayer(7, 7);
        assert(a.robotCount() == 1  &&  a.numberOfRobotsAt(2, 4) == 1);
        for (int k = 0; k < 100  &&  a.numberOfRobotsAt(2, 4) == 1; k++)
            a.moveRobots(3, EAST);
        assert(a.numberOfRobotsAt(2, 4) == 0 && a.numberOfRobotsAt(2, 5) == 1);
        for (int k = 0; k < 100  &&  a.robotCount() == 1; k++)
            a.moveRobots(2, EAST);
        assert(a.robotCount() == 1);
        assert(a.numberOfRobotsAt(2, 4) == 1 || a.numberOfRobotsAt(2, 5) == 1);
        for (int k = 0; k < 100  &&  a.robotCount() == 1; k++)
            a.moveRobots(3, EAST);
        assert(a.robotCount() == 0);
        assert(a.numberOfRobotsAt(2, 4) == 0 && a.numberOfRobotsAt(2, 5) == 0);
        for (int k = 0; k < MAXROBOTS/4; k++)
        {
            a.addRobot(6, 7, 1);
            a.addRobot(8, 7, 1);
            a.addRobot(7, 6, 1);
            a.addRobot(7, 8, 1);
        }
        assert(! a.player()->isDead());
        a.moveRobots(2, NORTH);
        assert(a.player()->isDead());
    }
    cout << "Passed all basic tests" << endl;
    exit(0);
}
