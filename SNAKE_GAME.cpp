#include <Windows.h>
#include <iostream>
#include <conio.h>
#include <time.h>
#include <string>
using namespace std;
enum direction { Up, Down, Left, Right };
const int height = 50;
const int width = 50;
const int winscore = 10;
const int maxtime = 60;
void getRowColbyLeftClick(int& rpos, int& cpos)
{
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD Events;
    INPUT_RECORD InputRecord;
    SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
    do
    {
        ReadConsoleInput(hInput, &InputRecord, 1, &Events);
        if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
        {
            cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
            rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
            break;
        }
    } while (true);
}
void gotoRowCol(int rpos, int cpos)
{
    COORD scrn;
    HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
    scrn.X = cpos;
    scrn.Y = rpos;
    SetConsoleCursorPosition(hOuput, scrn);
}
void SetClr(int clr)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), clr);
}
struct position
{
    int ri, ci;
};
struct snake
{
    int snake_number;
    position* pos = new position[3];
    int len = 3;
    string name;
    direction dir;
    char keys[4];
    bool alive = false;
    int score = 0;
};
struct snakes_team
{
    int members;
    int team_num;
    int score = 0;
    snake* Snake;
    string name;
};
struct teams
{
    snakes_team* teams_list = new snakes_team[2];
};
struct food
{
    position loc;
    bool active = false;
    int score;
    int start_time = 0;
};
struct foods
{
    int types;
    food* food;
};
void init_snake_position(snake& snake, position head)
{
    if (snake.dir == Right)
    {
        for (int i = 0; i < 3; i++)
        {
            snake.pos[i] = head;
            snake.pos[i].ci -= i;
        }
    }
    else
    {
        for (int i = 0; i < 3; i++)
        {
            snake.pos[i] = head;
            snake.pos[i].ci += i;
        }
    }
}
void init_team(snakes_team& team, int members)
{
    team.members = members;
    team.Snake = new snake[members];

    //.....................................
    position a;

    int i = 0;
    team.Snake[0].snake_number = i;
    team.Snake[0].dir = Left;
    team.Snake[0].alive = true;
    a.ri = height / 4;
    a.ci = width - width / 4;
    init_snake_position(team.Snake[i], a);
    cout << "Enter Snake 1 name : ";
    cin >> team.Snake[0].name;
    team.Snake[0].keys[0] = 72;
    team.Snake[0].keys[1] = 80;
    team.Snake[0].keys[2] = 75;
    team.Snake[0].keys[3] = 77;
    i++;
    if (i < team.members)
    {
        team.Snake[i].snake_number = i;
        team.Snake[i].dir = Right;
        team.Snake[i].alive = true;
        a.ri = height / 4;
        a.ci = width / 4;
        init_snake_position(team.Snake[i], a);
        cout << "Enter Snake 2 name : ";
        cin >> team.Snake[1].name;
        team.Snake[i].keys[0] = 119;
        team.Snake[i].keys[1] = 115;
        team.Snake[i].keys[2] = 97;
        team.Snake[i].keys[3] = 100;
        i++;
    }

    if (i < team.members)
    {
        team.Snake[i].snake_number = i;
        team.Snake[i].dir = Right;
        team.Snake[i].alive = true;
        a.ri = height - height / 4;
        a.ci = width / 4;
        init_snake_position(team.Snake[i], a);
        cout << "Enter Snake 3 name : ";
        cin >> team.Snake[i].name;
        team.Snake[i].keys[0] = 105;
        team.Snake[i].keys[1] = 107;
        team.Snake[i].keys[2] = 106;
        team.Snake[i].keys[3] = 108;
        i++;
    }
    if (i < team.members)
    {
        team.Snake[i].snake_number = i;
        team.Snake[i].dir = Left;
        team.Snake[i].alive = true;
        a.ri = height - height / 4;
        a.ci = width - width / 4;
        init_snake_position(team.Snake[i], a);
        cout << "Enter Snake 4 name : ";
        cin >> team.Snake[0].name;
        team.Snake[i].keys[0] = 56;
        team.Snake[i].keys[1] = 50;
        team.Snake[i].keys[2] = 52;
        team.Snake[i].keys[3] = 54;
        i++;
    }
}
void init_teams(teams& teams)
{
    for (int i = 0; i < 2; i++)
    {
        teams.teams_list[i].team_num = i;
        teams.teams_list[i].score = 0;
        teams.teams_list[i].members = 2;
        teams.teams_list[i].Snake = new snake[2];
        cout << "Enter team " << i + 1 << " name :";
        cin >> teams.teams_list[i].name;

        position a;
        for (int j = 0; j < 2; j++)
        {
            teams.teams_list[i].Snake[j].snake_number = j;
            teams.teams_list[i].Snake[j].len = 3;
            teams.teams_list[i].Snake[j].alive = true;
            teams.teams_list[i].Snake[j].score = 0;
            //...............................
            if (teams.teams_list[i].team_num == 0)
            {
                teams.teams_list[i].Snake[j].dir = Right;
                if (teams.teams_list[i].Snake[j].snake_number == 0)
                {
                    teams.teams_list[i].Snake[j].keys[0] = 119;
                    teams.teams_list[i].Snake[j].keys[1] = 115;
                    teams.teams_list[i].Snake[j].keys[2] = 97;
                    teams.teams_list[i].Snake[j].keys[3] = 100;
                    a.ri = height / 4;
                    a.ci = width / 4;
                }
                else
                {
                    teams.teams_list[i].Snake[j].keys[0] = 105;
                    teams.teams_list[i].Snake[j].keys[1] = 107;
                    teams.teams_list[i].Snake[j].keys[2] = 106;
                    teams.teams_list[i].Snake[j].keys[3] = 108;
                    a.ri = height - height / 4;
                    a.ci = width / 4;
                }
            }
            else
            {
                teams.teams_list[i].Snake[j].dir = Left;
                if (teams.teams_list[i].Snake[j].snake_number == 0)
                {

                    teams.teams_list[i].Snake[j].keys[0] = 72;
                    teams.teams_list[i].Snake[j].keys[1] = 80;
                    teams.teams_list[i].Snake[j].keys[2] = 75;
                    teams.teams_list[i].Snake[j].keys[3] = 77;
                    a.ri = height / 4;
                    a.ci = width - width / 4;
                }
                else
                {
                    teams.teams_list[i].Snake[j].keys[0] = 56;
                    teams.teams_list[i].Snake[j].keys[1] = 50;
                    teams.teams_list[i].Snake[j].keys[2] = 52;
                    teams.teams_list[i].Snake[j].keys[3] = 54;
                    a.ri = height - height / 4;
                    a.ci = width - width / 4;
                }
            }
            //...............................
            init_snake_position(teams.teams_list[i].Snake[j], a);
        }
    }
}
void initgametype(bool& teammatch, int& members)
{
    cout << "You want to play inter_team(y/Y for yes)";
    char a;
    cin >> a;
    if (a == 'y' || a == 'Y')
        teammatch = true;
    if (teammatch)
    {
        members = 2;
    }
    else
    {
        cout << "num of players in team(max 4)";
        cin >> members;
    }

}

void display_snake(const snake& S)
{
    SetClr(4);
    for (int i = 0; i < S.len; i++)
    {
        gotoRowCol(S.pos[i].ri, S.pos[i].ci);
        cout << '*';
    }
}
void display_snakes(snakes_team team)
{
    for (int j = 0; j < team.members; j++)
    {
        if (team.Snake[j].alive)
        {
            display_snake(team.Snake[j]);
        }
    }
}
void display_snakes_for_teammatch(const teams& Teams)
{
    for (int i = 0; i < 2; i++)
    {
        display_snakes(Teams.teams_list[i]);
    }
}
void erase_snake(snake S)
{
    SetClr(2);
    for (int i = 0; i < S.len; i++)
    {
        gotoRowCol(S.pos[i].ri, S.pos[i].ci);
        cout << char(-37);
    }
}
void erase_snakes(snakes_team team)
{
    for (int i = 0; i < team.members; i++)
    {
        erase_snake(team.Snake[i]);
    }
}
void erase_snakes_for_teammatch(const teams& Teams)
{
    for (int i = 0; i < 2; i++)
    {
        erase_snakes(Teams.teams_list[i]);
    }
}

void move_snake(snake& S)
{
    for (int i = S.len - 1; i > 0; i--)
    {
        S.pos[i] = S.pos[i - 1];
    }

    if (S.dir == Up)
    {
        S.pos[0].ri--;
        if (S.pos[0].ri == -1)
        {
            S.pos[0].ri = height - 1;
        }
    }
    else if (S.dir == Down)
    {
        S.pos[0].ri++;
        if (S.pos[0].ri == height)
        {
            S.pos[0].ri = 0;
        }
    }
    else if (S.dir == Left)
    {
        S.pos[0].ci--;
        if (S.pos[0].ci == -1)
        {
            S.pos[0].ci = width - 1;
        }
    }
    else if (S.dir == Right)
    {
        S.pos[0].ci++;
        if (S.pos[0].ci == width)
        {
            S.pos[0].ci = 0;
        }
    }
}
void move_snakes(snakes_team& team)
{
    for (int j = 0; j < team.members; j++)
    {
        if (team.Snake[j].alive)
            move_snake(team.Snake[j]);
    }
}
void move_snakes_for_teammatch(const teams& Teams)
{
    for (int i = 0; i < 2; i++)
    {
        move_snakes(Teams.teams_list[i]);
    }
}

void change_dir_snake(snake& S, char key)
{
    if (key == S.keys[0] && S.dir != Down)
    {
        S.dir = Up;
    }
    else if (key == S.keys[1] && S.dir != Up)
    {
        S.dir = Down;
    }
    else if (key == S.keys[2] && S.dir != Right)
    {
        S.dir = Left;
    }
    else if (key == S.keys[3] && S.dir != Left)
    {
        S.dir = Right;
    }
}
void change_dir(snakes_team team, char key)
{

    for (int i = 0; i < team.members; i++)
    {
        change_dir_snake(team.Snake[i], key);
    }
}
void change_dir_for_teammatch(const teams& Teams, char key)
{
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            if (Teams.teams_list[i].Snake[j].alive)
            {
                change_dir_snake(Teams.teams_list[i].Snake[j], key);
            }
        }
    }
}

bool food_on_snake(food f, snake S)
{
    for (int i = 0; i < S.len; i++)
    {
        if (f.loc.ri == S.pos[i].ri && f.loc.ci == S.pos[i].ci)
            return true;
    }
    return false;
}

bool valid_food_loc(food f, snakes_team team)
{
    for (int j = 0; j < 2; j++)
    {
        if (food_on_snake(f, team.Snake[j]))
            return false;
    }
    return true;
}
bool valid_food_loc_for_teammatch(food f, teams Teams)
{
    for (int i = 0; i < 2; i++)
    {
        if (!valid_food_loc(f, Teams.teams_list[i]))
        {
            return false;
        }
    }
    return true;
}

void generate_food(food& f, snakes_team& team)
{
    do
    {
        f.loc.ri = (rand() % (height - 1)) + 1;
        f.loc.ci = (rand() % (width - 1)) + 1;
    } while (!valid_food_loc(f, team));
    f.active = true;
}
void generate_food_for_teammatch(food& f, teams Teams)
{
    do
    {
        f.loc.ri = (rand() % (height - 1)) + 1;
        f.loc.ci = (rand() % (width - 1)) + 1;
    } while (!valid_food_loc_for_teammatch(f, Teams));
    f.active = true;
}

void display_food(food f)
{
    if (f.score == 1)
        SetClr(6);
    else
        SetClr(7);
    gotoRowCol(f.loc.ri, f.loc.ci);
    cout << char(-37);
}
void undisplay_food(food f)
{
    SetClr(2);
    gotoRowCol(f.loc.ri, f.loc.ci);
    cout << char(-37);
}

bool should_be_eaten_by_snake(snake S, food f)
{
    if (S.pos[0].ri == f.loc.ri && S.pos[0].ci == f.loc.ci)
        return true;
    return false;
}

void eat_by_snake(snake& S, food& f)
{
    position* temp = new position[S.len + 1];
    temp[0] = f.loc;
    for (int i = 0; i < S.len; i++)
    {
        temp[i + 1] = S.pos[i];
    }
    delete[] S.pos;
    temp[S.len] = f.loc;
    S.pos = temp;

    S.len++;
    S.score += f.score;
    f.active = false;
}
void eat(snakes_team& team, food& f)
{
    for (int j = 0; j < team.members; j++)
    {
        if (should_be_eaten_by_snake(team.Snake[j], f))
        {
            eat_by_snake(team.Snake[j], f);
            team.score += f.score;
        }
    }
}
void eat_for_teammatch(teams& Teams, food& f)
{
    for (int i = 0; i < 2; i++)
    {
        eat(Teams.teams_list[i], f);

    }
}

void display_score(const snakes_team& team)
{
    for (int i = 0; i < team.members; i++)
    {
        gotoRowCol(5 * (i + 1), width + 5);
        cout << "Score of " << team.Snake[i].name << " is " << team.Snake[i].score << " .";
    }
}
void display_score_teammatch(const teams& Teams)
{
    for (int i = 0; i < 2; i++)
    {
        gotoRowCol(5 * (i + 1), width + 5);
        cout << "Score of Team " << Teams.teams_list[i].name << " is : ";
        for (int j = 0; j < 2; j++)
        {
            cout << Teams.teams_list[i].Snake[j].score << " + ";
        }
        cout << "\b\b= " << Teams.teams_list[i].score;
    }
}
void display_lawazmat(const snakes_team& team, int startgametime)
{
    gotoRowCol(5 * (team.members + 1), width + 5);
    cout << "Target score is " << winscore;

    gotoRowCol(5 * (team.members + 2), width + 5);
    cout << "TIME: " << maxtime - (time(0) - startgametime);
}
void display_lawazmat_for_teammatch(int startgametime)
{
    gotoRowCol(5 * 3, width + 5);
    cout << "Target score is " << winscore;

    gotoRowCol(5 * 4, width + 5);
    cout << "TIME: " << maxtime - (time(0) - startgametime);
}

bool on_boundry(snake S)
{
    if (S.pos[0].ri == 0 || S.pos[0].ri == height || S.pos[0].ci == 0 || S.pos[0].ci == width)
        return true;
    return false;
}
void killsnakes(snakes_team& team)
{
    for (int j = 0; j < team.members; j++)
    {
        if (on_boundry(team.Snake[j]))
            team.Snake[j].alive = false;
    }
}
void killforteammatch(teams& Teams)
{
    for (int i = 0; i < 2; i++)
    {
        killsnakes(Teams.teams_list[i]);
    }

}

void display_board()
{

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {

            SetClr(2);
            gotoRowCol(i, j);
            cout << char(-37);
        }
    }
}
void displayboundary()
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (i == 0 || i == height - 1 || j == 0 || j == width - 1)
            {
                SetClr(3);
                gotoRowCol(i, j);
                cout << char(-37);
            }

        }
    }
}

void win_with_highest_count_for_teammatch(teams Team)
{
    int highest = Team.teams_list[0].score;
    int highest_ind = 0;
    for (int i = 0; i < 2; i++)
    {
        if (Team.teams_list[i].score > highest)
        {
            highest = Team.teams_list[i].score;
            highest_ind = i;
        }
    }
    system("cls");
    cout << Team.teams_list[highest_ind].name << " WON .............";
    exit(0);
}
void win_with_highest_count(snakes_team team)
{
    int highest = team.Snake[0].score;
    int highest_ind = 0;
    for (int i = 0; i < team.members; i++)
    {
        if (team.Snake[i].score > highest)
        {
            highest = team.Snake[i].score;
            highest_ind = i;
        }
    }
    system("cls");
    cout << team.Snake[highest_ind].name << " WON .............";
    exit(0);
}
int onlysurvivor_for_teammatch(teams Team)
{
    for (int i = 0; i < 2; i++)
    {
        bool check = true;
        for (int j = 0; j < 2; j++)
        {
            if (Team.teams_list[i].Snake[j].alive)
            {
                check = false;
            }
            if (check)
            {
                return (i + 1) % 2;
            }
        }
    }
    return -1;
}
int onlysurvivorsnake(snakes_team team)
{
    int alive_ind;
    int alive = 0;
    for (int i = 0; i < team.members; i++)
    {

        if (team.Snake[i].alive)
        {
            alive++;
            alive_ind = i;
        }
    }
    if (alive == 1)
    {
        return alive_ind;
    }
    return -1;
}
void checkwin_for_teammatch(teams Team, int startgametime)
{
    for (int i = 0; i < 2; i++)
    {
        if (Team.teams_list[i].score >= winscore)
        {
            system("cls");
            cout << Team.teams_list[i].name << " WON .............";
            exit(0);
        }
    }
    if (time(0) - startgametime >= maxtime)
    {
        win_with_highest_count_for_teammatch(Team);
    }
    int x = onlysurvivor_for_teammatch(Team);
    if (x != -1)
    {
        system("cls");
        cout << Team.teams_list[x].name << " WON .............";
        exit(0);
    }
}
void checkwin(snakes_team team, int startgametime)
{
    for (int i = 0; i < team.members; i++)
    {
        if (team.Snake[i].score >= winscore)
        {
            system("cls");
            cout << team.Snake[i].name << " WON .............";
            exit(0);
        }
    }
    if (time(0) - startgametime >= maxtime)
    {
        win_with_highest_count(team);
    }
    int x = onlysurvivorsnake(team);
    if (x != -1)
    {
        system("cls");
        cout << team.Snake[x].name << " WON .............";
        exit(0);
    }



}
void touching_othersnake(snakes_team & team)
{
    for (int i = 0; i < team.members; i++)
    {
        for (int j = 0; j < team.members; j++)
        {
            if (i != j)
            {
                for (int k = 0; k < team.Snake[j].len; k++)
                {
                    if (team.Snake[i].pos[0].ri == team.Snake[j].pos[k].ri && team.Snake[i].pos[0].ci == team.Snake[j].pos[k].ci)
                    {
                        team.Snake[i].alive=false;
                    }
                }            
                
            }
        }
    }
}

//**/
void teamgame(int stage)
{


    food f;
    f.score = 1;

    food fbig;
    fbig.score = 3;

    int cycleoffood = 0;

    teams Teams;
    init_teams(Teams);
    display_board();
    generate_food_for_teammatch(f, Teams);


    if (stage == 2)
    {
        displayboundary();
    }
    int startgametime = time(0);
    while (true)
    {
        if (_kbhit())
        {
            char a = _getch();
            if (a == -32)
            {
                a = _getch();
            }
            change_dir_for_teammatch(Teams, a);
        }
        //...............
        if (f.active == false)
        {
            generate_food_for_teammatch(f, Teams);
        }
        else
        {
            display_food(f);
        }
        //...............
        if (fbig.active == false)
        {
            if (cycleoffood % 5 == 0 && cycleoffood != 0)
            {
                generate_food_for_teammatch(fbig, Teams);
                fbig.start_time = time(0);
                cycleoffood = 0;
            }
        }
        else
        {
            display_food(fbig);
        }

        //...............
        display_snakes_for_teammatch(Teams);
        Sleep(100);
        erase_snakes_for_teammatch(Teams);
        move_snakes_for_teammatch(Teams);
        //...............
        if (stage == 2)
        {
            killforteammatch(Teams);
        }
        //...............
        if (f.active)
            eat_for_teammatch(Teams, f);
        if (f.active == false)
            cycleoffood++;
        //...............
        if (fbig.active)
        {
            eat_for_teammatch(Teams, fbig);
            if (time(0) - fbig.start_time > 10)
            {
                fbig.active = false;
                undisplay_food(fbig);
            }
        }
        display_score_teammatch(Teams);
        display_lawazmat_for_teammatch(startgametime);
        checkwin_for_teammatch(Teams, startgametime);

    }
}
//...................................
void multiplayer(int members, int stage)
{

    //........................

    food f;
    f.score = 1;

    food fbig;
    fbig.score = 3;

    int cycleoffood = 0;
    //.......................
    snakes_team team;
    init_team(team, members);
    //.......................
    display_board();
    if (stage == 2)
    {
        displayboundary();
    }
    ///////////////////////////////////////
    int startgametime = time(0);
    while (true)
    {
        if (_kbhit())
        {
            char a = _getch();
            if (a == -32)
            {
                a = _getch();
            }
            change_dir(team, a);
        }

        if (stage == 2)
        {
            killsnakes(team);
        }
        //.....................
        if (f.active == false)
        {
            generate_food(f, team);
        }
        else
        {
            display_food(f);
        }
        //...............
        if (fbig.active == false)
        {
            if (cycleoffood % 5 == 0 && cycleoffood != 0)
            {
                generate_food(fbig, team);
                fbig.start_time = time(0);
                cycleoffood = 0;
            }
        }
        else
        {
            display_food(fbig);
        }
        //...............
        display_snakes(team);
        Sleep(100);
        erase_snakes(team);
        move_snakes(team);
        touching_othersnake(team);
        //............................
        if (stage == 2)
        {
            killsnakes(team);
        }
        //...............
        if (f.active)
            eat(team, f);
        if (f.active == false)
            cycleoffood++;
        //...............
        if (fbig.active)
        {
            eat(team, fbig);
            if (time(0) - fbig.start_time > 10)
            {
                fbig.active = false;
                undisplay_food(fbig);
            }
        }
        display_score(team);
        display_lawazmat(team, startgametime);
        //..........................................
        checkwin(team, startgametime);

    }
}
//**/
int main()
{
    bool  teammatch = false;
    int members;
    initgametype(teammatch, members);
    int stage;
    cout << "Enter stage:";
    cin >> stage;
    if (teammatch)
    {
        teamgame(stage);
    }
    else
    {
        multiplayer(members, stage);
    }
}