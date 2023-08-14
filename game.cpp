#include <iostream>
#include <random>
#include <vector>
#include <unistd.h>

using namespace std;

#define num_player 5

enum characters
{
  wolf = 1,
  minion = 2,
  seer = 3,
  robber = 4,
  clown = 5,
  drunk = 6,
  insomnia = 7,
  villager = 8
};

string to_str( int c )
{
  switch( c )
  {
  case 1:
    return "wolf 狼人";
  case 2:
    return "minion 爪牙";
  case 3:
    return "seer 預言家";
  case 4:
    return "robber 強盜";
  case 5:
    return "clown 搗蛋鬼";
  case 6:
    return "drunk 酒鬼";
  case 7:
    return "insomnia 失眠者";
  case 8:
    return "villager 村民";
  }
}

struct moves
{
  int robber_get = -1;
  int robber = -1;
  int clown_swap1 = -1;
  int clown_swap2 = -1;
  int drunk_get = -1;
  int drunk = -1;
};

int get_wolf( vector<int> chars )
{
  for ( auto i = 1; i < chars.size(); ++i )
  {
    if ( chars[i] == wolf )
      return i;
  }
}

char int2char( int i )
{
  switch( i )
  {
  case 1:
    return 'A';
  case 2:
    return 'B';
  case 3:
    return 'C';
  default:
    return 'X';
  }
}

void seer_sees_cards( vector<int>& chars, int not_see )
{
  for ( auto i = num_player + 1; i < chars.size(); ++i )
  {
    if ( i == not_see + num_player )
      continue;
    cout << "Card " << int2char(i - num_player) << " is " << to_str( chars[i] ) << "\n";
  }
}

void perform1( vector<int>& chars, moves& m, int id )
{
  switch( chars[id] )
  {
    case minion:
      if ( get_wolf(chars) > num_player )
        cout << "Wolf is not among players\n";
      else
        cout << "Wolf is player number " << get_wolf( chars ) << "\n";
      break;
    case seer:
      {
        while ( true )
        {
          char s;
          cout << "Do you want to see one player [p] or two cards in the sea [s]? ";
          cin >> s;
          if ( s == 'p' )
          {
            while ( true )
            {
              char s1;
              cout << "Which player do you want to see [1-" << num_player << "]? ";
              cin >> s1;
              bool input_ok = false, cont = false;
              for ( auto i = 1; i <= num_player; ++i )
              {
                if ( s1 == '0' + i )
                {
                  if ( chars[i] == seer )
                  {
                    cout << "\nCannot see yourself\n";
                    cont = true;
                  }
                  else
                    cout << "Player number " << i << " is " << to_str( chars[i] ) << "\n";
                  input_ok = true;
                  break;
                }
              }
              if ( !input_ok )
              {
                cout << "\nWrong input\n";
                continue;
              }
              if ( !cont )
                break;
            }
            break;
          }
          else if ( s == 's' )
          {
            while ( true )
            {
              char s2;
              cout << "Which card do you not want to see [A-C]? ";
              cin >> s2;
              if ( s2 == 'a' || s2 == 'A' )
              {
                seer_sees_cards( chars, 1 );
                break;
              }
              else if ( s2 == 'b' || s2 == 'B' )
              {
                seer_sees_cards( chars, 2 );
                break;
              }
              else if ( s2 == 'c' || s2 == 'C' )
              {
                seer_sees_cards( chars, 3 );
                break;
              }
              else
                cout << "\nWrong input\n";
            }
            break;
          }
          else
            cout << "\nWrong input\n";
        }
        break;
      }
    case clown:
      {
        while ( true )
        {
          char s;
          cout << "Which players do you want to swap? Enter the first player [1-" << num_player << "]: ";
          cin >> s;
          bool input_ok = false, cont = false;
          for ( auto i = 1; i <= num_player; ++i )
          {
            if ( s == '0' + i )
            {
              if ( i == id )
              {
                cout << "\nCannot swap yourself\n";
                cont = true;
              }
              else
              {
                m.clown_swap1 = i;
              }
              input_ok = true;
              break;
            }
          }
          if ( !input_ok )
          {
            cout << "\nWrong input\n";
            continue;
          }
          if ( !cont )
            break;
        }
        while ( true )
        {
          char s;
          cout << "Enter the second player [1-" << num_player << "]: ";
          cin >> s;
          bool input_ok = false, cont = false;
          for ( auto i = 1; i <= num_player; ++i )
          {
            if ( s == '0' + i )
            {
              if ( i == id )
              {
                cout << "\nCannot swap yourself\n";
                cont = true;
              }
              else if ( i == m.clown_swap1 )
              {
                cout << "\nPlease choose another player\n";
                cont = true;
              }
              else
              {
                m.clown_swap2 = i;
              }
              input_ok = true;
              break;
            }
          }
          if ( !input_ok )
          {
            cout << "\nWrong input\n";
            continue;
          }
          if ( !cont )
            break;
        }
        cout << "You will swap player " << m.clown_swap1 << "'s and player " << m.clown_swap2 << "'s cards\n";
        break;
      }
    case robber:
      {
        while ( true )
        {
          char s;
          cout << "Which player's card do you want to swap with [1-" << num_player << "]? ";
          cin >> s;
          bool input_ok = false, cont = false;
          for ( auto i = 1; i <= num_player; ++i )
          {
            if ( s == '0' + i )
            {
              if ( i == id )
              {
                cout << "\nCannot swap with yourself\n";
                cont = true;
              }
              else
              {
                cout << "You will swap with player number " << i << "'s card\n";
                m.robber_get = i;
              }
              input_ok = true;
              break;
            }
          }
          if ( !input_ok )
          {
            cout << "\nWrong input\n";
            continue;
          }
          if ( !cont )
            break;
        }
        m.robber = id;
        break;
      }
    case drunk:
      {
        while ( true )
        {
          char s2;
          cout << "Which card in the sea do you want to swap with [A-C]? ";
          cin >> s2;
          if ( s2 == 'a' || s2 == 'A' )
          {
            m.drunk_get = num_player + 1;
            break;
          }
          else if ( s2 == 'b' || s2 == 'B' )
          {
            m.drunk_get = num_player + 2;
            break;
          }
          else if ( s2 == 'c' || s2 == 'C' )
          {
            m.drunk_get = num_player + 3;
            break;
          }
          else
            cout << "\nWrong input\n";
        }
        cout << "You will swap with card " << int2char ( m.drunk_get - num_player ) << "\n";
        m.drunk = id;
        break;
      }
    default:
      cout << "Nothing to do\n";
      break;
  }
}

void perform2( vector<int>& chars, moves& m, vector<int>& chars_final, int id )
{
  switch( chars[id] )
  {
    case robber:
      {
        cout << "After swapping with player " << m.robber_get << ", you get " << to_str( chars_final[id] ) << "\n";
        break;
      }
    case insomnia:
      {
        cout << "Your final card is " << to_str( chars_final[id] ) << "\n";
        break;
      }
    default:
      cout << "Nothing to do\n";
  }
}

int main( int argc, char* argv[] )
{
  system( "clear" );
  vector<int> chars;
  moves m;
  chars.push_back( -1 ); // dummy
  for ( auto i = 1; i <= 8; ++i )
  {
    if ( i == 6 ) continue;
    chars.push_back(i);
  }
  for ( auto i = 0; i < num_player + 3 - chars.size() + 1; ++i )
    chars.push_back(8);
  default_random_engine rng;
  rng.seed( stoi( argv[1] ) );
  shuffle( chars.begin() + 1, chars.end(), rng );

  for ( auto i = 1; i <= num_player; ++i )
  {
    string s;
    cout << "Enter any key to continue...";
    cin >> s;
    cout << "You are player number " << i << ", your character is " << to_str( chars[i] ) << "\n";
    perform1( chars, m, i );
    cout << "Enter any key to continue...";
    cin >> s;
    system( "clear" );
  }

  vector<int> chars_final;
  for ( auto i = 0; i < chars.size(); ++i )
    chars_final.push_back( chars[i] );

  if ( m.clown_swap1 != -1 && m.clown_swap2 != -1 )
  {
    int tmp = chars_final[m.clown_swap1];
    chars_final[m.clown_swap1] = chars_final[m.clown_swap2];
    chars_final[m.clown_swap2] = tmp;
  }
  if ( m.robber_get != -1 && m.robber != -1 )
  {
    int tmp = chars_final[m.robber];
    chars_final[m.robber] = chars_final[m.robber_get];
    chars_final[m.robber_get] = tmp;
  }
  if ( m.drunk_get != -1 && m.drunk != -1 )
  {
    int tmp = chars_final[m.drunk];
    chars_final[m.drunk] = chars_final[m.drunk_get];
    chars_final[m.drunk_get] = tmp;
  }

  for ( auto i = 1; i <= num_player; ++i )
  {
    string s;
    cout << "Enter any key to continue...";
    cin >> s;
    cout << "Hi player number " << i << ", your (original) character is " << to_str( chars[i] ) << "\n";
    perform2( chars, m, chars_final, i );
    cout << "Enter any key to continue...";
    cin >> s;
    system( "clear" );
  }

  cout << "Start with player " << rng() % num_player + 1 << "\n";

  string s;
  cout << "Enter any key to continue...";
  cin >> s;
  cout << "======== End Game ========\n";
  for ( auto i = 1; i <= num_player; ++i )
    cout << "Player number " << i << " was " << to_str( chars[i] ) << " in the beginning and is now " << to_str( chars_final[i] ) << "\n";
  for ( auto i = num_player + 1; i < chars.size(); ++i )
    cout << "Card " << int2char( i - num_player ) << " was " << to_str( chars[i] ) << " in the beginning and is now " << to_str( chars_final[i] ) << "\n";
  cout << "\nSwaps done:\n";
  if ( m.clown_swap1 != -1 && m.clown_swap2 != -1 )
    cout << "Clown swapped player " << m.clown_swap1 << "'s and player " << m.clown_swap2 << "'s cards\n";
  if ( m.robber_get != -1 && m.robber != -1 )
    cout << "Robber (player " << m.robber << ") swapped with player " << m.robber_get << "\n";
  if ( m.drunk_get != -1 && m.drunk != -1 )
    cout << "Drunk (player " << m.drunk << ") swapped with card " << int2char( m.drunk_get - num_player ) << "\n";
  return 0;
}