#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <string.h>  

 
 
void error(const char *msg)
{
    perror(msg);
    exit(0);
}
int sockfd;
char matrix[3][3];  /* the tic tac toe matrix */
int counter=0;
char check(void);
void init_matrix(void);
void get_player_move(void);
void get_server_move(void);
void disp_matrix(void);

int main(int argc, char *argv[])
{
    int  portno, n;
    struct sockaddr_in serv_addr;
 
     portno = atoi(argv[1]);//convet into integer
    
     sockfd = socket(AF_INET, SOCK_STREAM, 0);//creating socket
     if (sockfd < 0) 
        error("ERROR opening socket");
        
     
  
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;

     serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
     serv_addr.sin_port = htons(portno);
   
    
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
       error("ERROR connecting"); 
   
    char done;

    puts("This is the game of Tic Tac Toe.\n");
 

    done =  ' ';
    init_matrix();
    
    do {
      disp_matrix();
      get_player_move();
      
    
    
      disp_matrix();
      done = check(); /* see if winner */
      if(done!= ' ') break; /* winner!*/
      puts("waiting server to move");
      get_server_move();
      disp_matrix();
      done = check(); /* see if winner */
    } while(done== ' ');     
        
      if(done=='X') printf("Player X won!\n");
      else printf("Player O won!!!!\n");
  disp_matrix(); /* show final positions */
  
             
    close(sockfd);
    return 0;
}

void init_matrix(void)
{
    int i, j;

    for(i=0; i<3; i++)
      for(j=0; j<3; j++) matrix[i][j] =  ' ';
}

/* Get a player's move. */
void get_player_move(void)
{
    int x, y,i,j;
    int n;

    printf("Enter X,Y coordinates for Player X: ");
    scanf("%d%*c%d", &x, &y);
    int temp1=htonl(x);
    int temp2=htonl(y);

    n=write(sockfd,&temp1,sizeof(temp1));
    if(n<0)
      error("write failed");
    n=write(sockfd,&temp2,sizeof(temp2));
    if(n<0)
      error("write failed");
    if(n=0)
      error("server closed");

 
     x--; y--;

    if(matrix[x][y]!= ' '){
      printf("Invalid move, try again.\n");
      get_player_move();
    }
    else matrix[x][y] = 'X';
     counter++;
  
  

}





/* Display the matrix on the screen. */
void disp_matrix(void)
{
    int t;

    for(t=0; t<3; t++) {
      printf(" %c | %c | %c ",matrix[t][0],
              matrix[t][1], matrix [t][2]);
      if(t!=2) printf("\n---|---|---\n");
    }
    printf("\n");
}






/* See if there is a winner. */
char check(void)
{
    int i,j;

    for(i=0; i<3; i++)  /* check rows */
      if(matrix[i][0]==matrix[i][1] &&
         matrix[i][0]==matrix[i][2]) return matrix[i][0];

    for(i=0; i<3; i++)  /* check columns */
      if(matrix[0][i]==matrix[1][i] &&
         matrix[0][i]==matrix[2][i]) return matrix[0][i];

    /* test diagonals */
    if(matrix[0][0]==matrix[1][1] &&
       matrix[1][1]==matrix[2][2])
         return matrix[0][0];

    if(matrix[0][2]==matrix[1][1] && matrix[1][1]==matrix[2][0])
         return matrix[0][2];
         
     if(counter==9)
     {
        printf("draw\n");
        exit(0);
   }
     
  return ' ';
}




void get_server_move(void)
{
 
    int x, y,i,j,n;


    n=read(sockfd,&i,sizeof(i));
    n=read(sockfd,&j,sizeof(j));
    if(n<0)
    error("error on reading");
    if(n=0)
      error("server closed");
    x=ntohl(i);
    y=ntohl(j);


      x--; y--;

      if(matrix[x][y]!= ' '){
        printf("Invalid move, try again.\n");
        get_server_move();
      }
      else matrix[x][y] = 'O';
      counter++;
  
   
}


