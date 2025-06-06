#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <limits.h>
#include <math.h>

#include "./board.h"
#include "./openings.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

bool winCheck1(int boardToEvaluate[5][5],int player) {
  bool w = false;
  for (int i = 0; i < 28; i++)
    if ( (boardToEvaluate[win[i][0][0]][win[i][0][1]] == player) &&
         (boardToEvaluate[win[i][1][0]][win[i][1][1]] == player) &&
         (boardToEvaluate[win[i][2][0]][win[i][2][1]] == player) &&
         (boardToEvaluate[win[i][3][0]][win[i][3][1]] == player) )
      w = true;
  return w;
}

bool loseCheck1(int boardToEvaluate[5][5],int player) {
  bool l = false;
  for (int i = 0; i < 48; i++)
    if ( (boardToEvaluate[lose[i][0][0]][lose[i][0][1]] == player) &&
         (boardToEvaluate[lose[i][1][0]][lose[i][1][1]] == player) &&
         (boardToEvaluate[lose[i][2][0]][lose[i][2][1]] == player) )
      l = true;
  return l;
}

int evaluate(int boardToEvaluate[5][5]){
    int score = 0;
    int counter[3];
    if(winCheck1(boardToEvaluate,1)){
        return INT_MAX;
    }
    if(winCheck1(boardToEvaluate,2)){
        return INT_MIN;
    }
    if(loseCheck1(boardToEvaluate,1)){
      return INT_MIN;
    }
    if(loseCheck1(boardToEvaluate,2)){
      return INT_MAX;
    }


    for (int i = 0; i < 28; i++){
      counter[0] = 0;
      counter[1] = 0;
      counter[2] = 0;
      counter[(boardToEvaluate[win[i][0][0]][win[i][0][1]])]++;
      counter[(boardToEvaluate[win[i][1][0]][win[i][1][1]])]++;
      counter[(boardToEvaluate[win[i][2][0]][win[i][2][1]])]++;
      counter[(boardToEvaluate[win[i][3][0]][win[i][3][1]])]++;
      if(counter[1] == 0 || counter[2] == 0){
        int power = counter[1] - counter[2];
        score +=((power>0) - (power < 0))*(int)pow(5.0,(double)(abs(power)));
      }
    }
    return score;
}

int *calculateMove(int player,int depth,int alfa,int beta) {
  int *ret = malloc(2*sizeof(int));
  ret[0] = 0;
  int alfa1 = alfa;
  int beta1 = beta;
  
  if(depth == 0 || winCheck1(board,player)|| winCheck1(board,(player%2)+1) || loseCheck1(board,player) || loseCheck1(board,(player%20 +1))){
    ret[1] = evaluate(board);
    return ret;
  }
  int best;
  if(player==1){
    best = INT_MIN;
  }
  else{
    best = INT_MAX;
  }

  

  for(int i=0;i<5;i++){
    for(int j=0;j<5;j++){
      if(board[i][j]==0){
        board[i][j] = player;
        int *scr = calculateMove((player%2)+1,depth-1,alfa1,beta1);
        board[i][j] = 0;
        if(player==1){
          if(scr[1] > best){
            best = scr[1];
            ret[0] = (i+1)*10 + j+1;
            alfa1 = MAX(alfa1,best);
            if (alfa1 >= beta1){
              i=5;j=5;
            }
          }
        }
        else if(player==2){
          if(scr[1] < best){
            best = scr[1];
            ret[0] = (i+1)*10 + j+1;
            beta1 = MIN(beta1,best);
            if (alfa1 >= beta1){
              i=5;j=5;
            }
              
          }
        }
        else{
          printf("errrrr\n");
        }
        free(scr);
      }
    }
  }
  ret[1] = best;
  return ret;
}

int makeMove(int player, int depth,int curr){
  if(curr <= depth && curr <=2){
    return openMoves(curr,board);
  }
  int *wyn = calculateMove(player, depth,INT_MIN,INT_MAX);
  int re = wyn[0];
  printf("%d %d\n",wyn[0],wyn[1]);
  free(wyn);
  return re;
}

int main(int argc, char *argv[]) {
  int currMove = 1;
  int server_socket;
  struct sockaddr_in server_addr;
  char server_message[16], player_message[16];

  bool end_game;
  int player, msg, move;


  if ( argc != 6 ) {
    printf("Wrong number of arguments\n");
    return -1;
  }

  // Create socket
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if ( server_socket < 0 ) {
    printf("Unable to create socket\n");
    return -1;
  }
  printf("Socket created successfully\n");

  // Set port and IP the same as server-side
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(atoi(argv[2]));
  server_addr.sin_addr.s_addr = inet_addr(argv[1]);

  // Send connection request to server
  if ( connect(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0 ) {
    printf("Unable to connect\n");
    return -1;
  }
  printf("Connected with server successfully\n");

  // Receive the server message
  memset(server_message, '\0', sizeof(server_message));
  if ( recv(server_socket, server_message, sizeof(server_message), 0) < 0 ) {
    printf("Error while receiving server's message\n");
    return -1;
  }

  memset(player_message, '\0', sizeof(player_message));
  snprintf(player_message, sizeof(player_message), "%s %s", argv[3], argv[4]);
  // Send the message to server
  if ( send(server_socket, player_message, strlen(player_message), 0) < 0 ) {
    printf("Unable to send message\n");
    return -1;
  }

  setBoard();
  end_game = false;
  sscanf(argv[3], "%d", &player);

  while ( !end_game ) {
    memset(server_message, '\0', sizeof(server_message));
    if ( recv(server_socket, server_message, sizeof(server_message), 0) < 0 ) {
      printf("Error while receiving server's message\n");
      return -1;
    }
    sscanf(server_message, "%d", &msg);
    move = msg%100;
    msg = msg/100;
    if ( move != 0 ) {
      setMove(move, 3-player);
      currMove++;
    }
    if ( (msg == 0) || (msg == 6) ) {
      //int *wyn = calculateMove(player, atoi(argv[5]),INT_MIN,INT_MAX);
      move = makeMove(player, atoi(argv[5]),currMove);
      //printf("%d %d\n",wyn[0],wyn[1]);
      //free(wyn);
      setMove(move, player);
      currMove++;
      memset(player_message, '\0', sizeof(player_message));
      snprintf(player_message, sizeof(player_message), "%d", move);
      if ( send(server_socket, player_message, strlen(player_message), 0) < 0 ) {
        printf("Unable to send message\n");
        return -1;
      }
     } else {
       end_game = true;
       switch ( msg ) {
         case 1 : printf("You won.\n"); break;
         case 2 : printf("You lost.\n"); break;
         case 3 : printf("Draw.\n"); break;
         case 4 : printf("You won. Opponent error.\n"); break;
         case 5 : printf("You lost. Your error.\n"); break;
      }
    }
  }

  // Close socket
  close(server_socket);

  return 0;
}
