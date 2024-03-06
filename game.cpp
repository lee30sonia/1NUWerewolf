#include <iostream>
#include <random>
#include <vector>
#include <unistd.h>
#include <algorithm>

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
  default:
    return "";
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

vector<int> get_wolfs( vector<int> chars )
{
  vector<int> wolfs;
  for ( auto i = 1; i < chars.size(); ++i )
  {
    if ( chars[i] == wolf )
      wolfs.push_back(i);
  }
  return wolfs;
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
    cout << "牌堆中的 " << int2char(i - num_player) << " 牌是 " << to_str( chars[i] ) << "\n";
  }
}

void perform1( vector<int>& chars, moves& m, int id )
{
  vector<int> wolfs = get_wolfs( chars );
  switch( chars[id] )
  {
    case minion:
      if ( wolfs.size() == 0 )
        cout << "玩家中沒有狼人\n";
      else if ( wolfs.size() == 1 )
        cout << "玩家 " << wolfs[0] << " 號是狼人\n";
      else if ( wolfs.size() == 2 )
        cout << "玩家 " << wolfs[0] << " 號和 " << wolfs[1] << " 號是狼人\n";
      break;
    case wolf:
      if ( wolfs.size() == 1 )
      {
        cout << "你是場上唯一的狼人\n";
        while ( true )
        {
          char s2;
          cout << "請選擇一張牌堆中的牌看[A-C]: ";
          cin >> s2;
          if ( s2 == 'a' || s2 == 'A' )
          {
            cout << "牌堆中的 A 牌是 " << to_str( chars[num_player+1] ) << "\n";
            break;
          }
          else if ( s2 == 'b' || s2 == 'B' )
          {
            cout << "牌堆中的 B 牌是 " << to_str( chars[num_player+2] ) << "\n";
            break;
          }
          else if ( s2 == 'c' || s2 == 'C' )
          {
            cout << "牌堆中的 C 牌是 " << to_str( chars[num_player+3] ) << "\n";
            break;
          }
          else
            cout << "\n輸入錯誤\n";
        }
      }
      else if ( wolfs.size() == 2 )
      {
        if ( wolfs[0] == id )
          cout << "場上的另一個狼人是玩家 " << wolfs[1] << " 號\n";
        else
          cout << "場上的另一個狼人是玩家 " << wolfs[0] << " 號\n";
      }
      break;
    case seer:
      {
        while ( true )
        {
          char s;
          cout << "請選擇要看一位玩家的牌（輸入p）或是看兩張牌堆中的牌（輸入s）: ";
          cin >> s;
          if ( s == 'p' )
          {
            while ( true )
            {
              char s1;
              cout << "請選擇一位玩家的牌來看 [1-" << num_player << "]: ";
              cin >> s1;
              bool input_ok = false, cont = false;
              for ( auto i = 1; i <= num_player; ++i )
              {
                if ( s1 == '0' + i )
                {
                  if ( chars[i] == seer )
                  {
                    cout << "\n不能看你自己\n";
                    cont = true;
                  }
                  else
                    cout << "玩家 " << i << " 號是 " << to_str( chars[i] ) << "\n";
                  input_ok = true;
                  break;
                }
              }
              if ( !input_ok )
              {
                cout << "\n輸入錯誤\n";
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
              cout << "請選擇一張牌堆中的牌「不」看 [A-C]: ";
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
                cout << "\n輸入錯誤\n";
            }
            break;
          }
          else
            cout << "\n輸入錯誤\n";
        }
        break;
      }
    case clown:
      {
        while ( true )
        {
          char s;
          cout << "請選擇兩位玩家的牌交換，輸入第一位玩家 [1-" << num_player << "]: ";
          cin >> s;
          bool input_ok = false, cont = false;
          for ( auto i = 1; i <= num_player; ++i )
          {
            if ( s == '0' + i )
            {
              if ( i == id )
              {
                cout << "\n不能換自己的牌\n";
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
            cout << "\n輸入錯誤\n";
            continue;
          }
          if ( !cont )
            break;
        }
        while ( true )
        {
          char s;
          cout << "輸入第二位玩家 [1-" << num_player << "]: ";
          cin >> s;
          bool input_ok = false, cont = false;
          for ( auto i = 1; i <= num_player; ++i )
          {
            if ( s == '0' + i )
            {
              if ( i == id )
              {
                cout << "\n不能換自己的牌\n";
                cont = true;
              }
              else if ( i == m.clown_swap1 )
              {
                cout << "\n請選擇不一樣的玩家\n";
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
            cout << "\n輸入錯誤\n";
            continue;
          }
          if ( !cont )
            break;
        }
        cout << "玩家 " << m.clown_swap1 << " 號和玩家 " << m.clown_swap2 << " 號的牌會被交換\n";
        break;
      }
    case robber:
      {
        while ( true )
        {
          char s;
          cout << "請選擇一位玩家的牌與你交換 [1-" << num_player << "]: ";
          cin >> s;
          bool input_ok = false, cont = false;
          for ( auto i = 1; i <= num_player; ++i )
          {
            if ( s == '0' + i )
            {
              if ( i == id )
              {
                cout << "\n不能跟自己交換\n";
                cont = true;
              }
              else
              {
                cout << "你的牌會和玩家 " << i << " 號交換\n";
                m.robber_get = i;
              }
              input_ok = true;
              break;
            }
          }
          if ( !input_ok )
          {
            cout << "\n輸入錯誤\n";
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
          cout << "請選擇一張牌堆中的牌與你的牌交換[A-C]: ";
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
            cout << "\n輸入錯誤\n";
        }
        cout << "你換到了牌堆中的 " << int2char ( m.drunk_get - num_player ) << " 牌\n";
        m.drunk = id;
        break;
      }
    default:
      cout << "（你的角色沒有事做，可以裝忙一下）\n";
      break;
  }
}

void perform2( vector<int>& chars, moves& m, vector<int>& chars_final, int id )
{
  switch( chars[id] )
  {
    case robber:
      {
        cout << "你搶了玩家 " << m.robber_get << " 號的牌，拿到了 " << to_str( chars_final[id] ) << "\n";
        break;
      }
    case insomnia:
      {
        cout << "夜晚結束時，你的牌是 " << to_str( chars_final[id] ) << "\n";
        break;
      }
    default:
      cout << "（你的角色不會獲得更多資訊）\n";
  }
}

int main( int argc, char* argv[] )
{
  system( "clear" );
  vector<int> chars;
  moves m;
  chars.push_back( -1 ); // dummy
  chars.push_back( 1 ); // wolf
  chars.push_back( 2 ); // minion
  if ( num_player >= 6 )
    chars.push_back( 1 ); // second wolf
  chars.push_back( 3 ); // seer
  chars.push_back( 4 ); // robber
  chars.push_back( 5 ); // clown
  if ( num_player >= 5 )
    chars.push_back( 6 ); // drunk
  if ( num_player >= 4 )
    chars.push_back( 7 ); // insomnia
  for ( auto i = 0; i < num_player + 3 - chars.size() + 1; ++i )
    chars.push_back( 8 ); // villager
  default_random_engine rng;
  rng.seed( stoi( argv[1] ) );
  shuffle( chars.begin() + 1, chars.end(), rng );

  for ( auto i = 1; i <= num_player; ++i )
  {
    string s;
    cout << "請將平板/電腦拿給玩家 " << i << " 號。輸入任意字元後按 Enter 開始你的回合...";
    cin >> s;
    cout << "你是玩家 " << i << " 號，你的初始角色是 " << to_str( chars[i] ) << "\n";
    perform1( chars, m, i );
    cout << "你的回合動作已結束，請輸入任意字元（可以裝忙）後按 Enter 清空畫面，再將平板/電腦拿給下個玩家...";
    cin >> s;
    system( "clear" );
  }

  vector<int> chars_final;
  for ( auto i = 0; i < chars.size(); ++i )
    chars_final.push_back( chars[i] );

  if ( m.robber_get != -1 && m.robber != -1 )
  {
    int tmp = chars_final[m.robber];
    chars_final[m.robber] = chars_final[m.robber_get];
    chars_final[m.robber_get] = tmp;
  }
  if ( m.clown_swap1 != -1 && m.clown_swap2 != -1 )
  {
    int tmp = chars_final[m.clown_swap1];
    chars_final[m.clown_swap1] = chars_final[m.clown_swap2];
    chars_final[m.clown_swap2] = tmp;
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
    cout << "請將平板/電腦拿給玩家 " << i << " 號。輸入任意字元後按 Enter 開始你的回合...";
    cin >> s;
    cout << "你是玩家 " << i << " 號，你的初始角色是 " << to_str( chars[i] ) << " （純提醒用）\n";
    perform2( chars, m, chars_final, i );
    cout << "你的回合動作已結束，請輸入任意字元（可以裝忙）後按 Enter 清空畫面，再將平板/電腦拿給下個玩家...";
    cin >> s;
    system( "clear" );
  }

  cout << "操作回合結束，由玩家 " << rng() % num_player + 1 << " 號開始發言\n";
  while ( true )
  {
    char s;
    cout << "投票結果決定淘汰哪位玩家 [1-" << num_player << "]? ";
    cin >> s;
    if ( 1 <= (s-'0') && (s-'0') <= num_player )
    {
      int num_wolfs = get_wolfs( chars_final ).size();
      if ( chars_final[s-'0'] == 1 )
        cout << "狼人被淘汰，好人陣營獲勝\n";
      else if ( chars_final[s-'0'] == 2 )
      {
        if ( num_wolfs == 0 )
          cout << "場上沒有狼人，爪牙被淘汰，好人陣營獲勝\n";
        else
          cout << "場上有狼人，爪牙被淘汰，狼人陣營獲勝\n";
      }
      else if ( num_wolfs == 0 )
        cout << "場上沒有狼人也沒有爪牙\n";
      else
        cout << "場上有狼人及/或爪牙，但淘汰了好人，狼人陣營獲勝\n";
      break;
    }
    else
      cout << "輸入錯誤\n";
  }

  cout << "======== 遊戲結束 ========\n";
  for ( auto i = 1; i <= num_player; ++i )
    cout << "玩家 " << i << " 號原本是 " << to_str( chars[i] ) << " ，最後是 " << to_str( chars_final[i] ) << "\n";
  for ( auto i = num_player + 1; i < chars.size(); ++i )
    cout << "牌堆中的 " << int2char( i - num_player ) << " 牌原本是 " << to_str( chars[i] ) << " ，最後是 " << to_str( chars_final[i] ) << "\n";
  cout << "\n夜晚發生的事：\n";
  if ( m.robber_get != -1 && m.robber != -1 )
    cout << "強盜（玩家 " << m.robber << " 號）搶了玩家 " << m.robber_get << " 號的牌\n";
  if ( m.clown_swap1 != -1 && m.clown_swap2 != -1 )
    cout << "搗蛋鬼交換了玩家 " << m.clown_swap1 << " 號和玩家 " << m.clown_swap2 << " 號的牌\n";
  if ( m.drunk_get != -1 && m.drunk != -1 )
    cout << "酒鬼（玩家 " << m.drunk << " 號）拿到了牌堆中的 " << int2char( m.drunk_get - num_player ) << " 牌\n";
  return 0;
}