/* CPSC/ECE 3220 example use of an unnamed pipe
 *
 * parent process sends single character messages to a child process
 *   via an unnamed pipe
 *
 * the messages are:
 *   '+' => increment the sum variable
 *   '-' => decrement the sum variable
 *   '=' => display the sum variable
 *   '!' => stop the child process
 *
 * the sum variable is set to 0 at the start of execution by using
 *   an initializer
 *
 * note that the parent and child processes will have their own
 *   copies of all the variables, including sum, with the values in
 *   the variables copied at the time of the fork()
 *
 * this example has all the code in main(), but a more common style
 *   of coding is to define separate functions for the parent and
 *   child to call
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define READ_FD  0
#define WRITE_FD 1

int main(){

  int sum = 0;   /* child will increment, decrement, and print this */
                 /*   variable based on messages from the parent    */

  int fd_p2c[2]; /* file descriptor pair for a parent to child pipe */
                 /* - first element is the read file descriptor     */
                 /* - second element is the write file descriptor   */

  int pid;       /* return value of fork()                          */

  int status;    /* status of child exit for parent to inspect      */

  int count;     /* bytes transfered in read(); 0 means EOF         */

  char command;  /* char for child to receive from read()           */

  char increment = '+';    /* use separate char variables so that   */
  char decrement = '-';    /*   it is textually clear which command */
  char display = '=';      /*   is selected by parent when passing  */
  char stop = '!';         /*   the address of the char to write()  */

  char unknown = '?';      /* test case for an undefined command    */


  if( pipe( fd_p2c ) == -1 ){
    printf( "*** unable to obtain a pipe\n" );
    exit( 1 );
  }

  pid = fork();  /* normal case: parent will have child pid, and */
                 /*   child will have 0; on error: parent has -1 */

  switch( pid ){

    case -1:
      printf( "*** unable to fork\n" ); exit( 1 ); break;

    case 0:  /* child */
      close( fd_p2c[WRITE_FD] );

      count = read( fd_p2c[READ_FD], &command, 1 );
      while( ( count != 0 ) && ( command != '!' ) ){
        switch( command ){
          case '+': sum++; break;
          case '-': sum--; break;
          case '=': printf( "child's copy of sum = %d\n", sum ); break;
          default:  printf( "*** unknown command\n" );
        }
        count = read( fd_p2c[READ_FD], &command, 1 );
      }

      close( fd_p2c[READ_FD] );
      exit( 0 );
      break;

    default:  /* parent */
      close( fd_p2c[READ_FD] );

      write( fd_p2c[WRITE_FD], &display,   1 );
      write( fd_p2c[WRITE_FD], &increment, 1 );
      write( fd_p2c[WRITE_FD], &increment, 1 );
      write( fd_p2c[WRITE_FD], &display,   1 );
      write( fd_p2c[WRITE_FD], &unknown,   1 );
      write( fd_p2c[WRITE_FD], &decrement, 1 );
      write( fd_p2c[WRITE_FD], &display,   1 );
      write( fd_p2c[WRITE_FD], &stop,      1 );

      close( fd_p2c[WRITE_FD] );
      waitpid( pid, &status, 0 );

      printf( "parent's copy of sum = %d\n", sum );

  }

  return( 0 );
}
