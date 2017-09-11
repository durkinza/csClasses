/* Zane C. Durkin
 * CS 210 
 * Hw1
 */

#include <stdio.h>
#include <strings.h>

FILE *fp;

int isletter(char x);
int ishex(char x);
int isnumber(char x);
int isoperator(char x);

int main(int argc, char *argv[]){
	if(argc < 2){
		printf("Missing input file");
		return 0;
	}
	if(! (fp=fopen(argv[1], "r")) ){
		printf("ERROR: File %s could not be accessed\n", argv[1]);
		return 1;
	}
	
	int status = 0;							// tells if machine is in a state or not
	int statusT = 0;						// holds the current state of the machine
	char l =' ',k;							// l holds current character in file, k holds 1 character look ahead
	char output[256] = "";					// holds the output for each term;
	while( (l = fgetc(fp)) ){
		if(l == EOF){
			break;
		}
		if(status > 0){
			switch(statusT){
				case 1:						// if in state of string
					output[strlen(output)] = l;
					if( l == '"' ){
						l = fgetc(fp);
						status = 0;
					}
					break;
				case 2:						// if in state of int (numerical-literal)
					if (ishex(l) || l == '.' || l == '_' || l == '#'){
						output[strlen(output)] = l;
					}else{
						status = 0;
					}
					//['0 - 9', 'A-F', '_', '.', '#']
					// 16#FFF#
					break;
				case 3:						// if in state of Comment
					output[strlen(output)] = l;
					if(l == '*'){
						l = fgetc(fp);
						if( l == '/' ){
							status = 0;
							output[strlen(output)] = l;
							l = fgetc(fp);
						}else{
							ungetc(l, fp);
						}
					}
					break;
				case 4:						// if in state of opperator
					status = 0;
					break;
				case 5:						// if in state of variable name
					if (isletter(l) || isnumber(l) || l == '_'){
						output[strlen(output)] = l;
					}else{
						status = 0;
						if( strlen(output) > 1){
							int count = strlen(output);
							char keytest[258];
							for (int i = count - 1; i >=0; i--){
								keytest[i+1] = output[i];
							}
							keytest[0] = ' '; // add space to front of string
							keytest[count+1] = ' '; // add space to end of string
							if( strstr(" accessor and array begin bool case character constant else elsif end exit function if in integer interface is loop module mutator natural null of or others out positive procedure range return struct subtype then type when while ", keytest) != NULL){
								statusT = 6;
							}
						}	
					}
					break;
				
				case 7:						// if character literal
					output[strlen(output)] = l;
					if( l == '\'' ){
						l = fgetc(fp);
						status=0;
					}
					break;
				
				case 99:
				default:
					printf("\nstatus unkown\n");
					break;
			}	
		}
		if( status == 0 ){
											// writeout output
			printf("%s", output);
											// output type of lexeme
			switch(statusT){
				case 1:
					printf(" (string)\n");
					break;
				case 2:
					printf(" (numeric literal)\n");
					break;
				case 3:
					printf(" (comment)\n");
					break;
				case 4:
					printf(" (operator)\n");
					break;
				case 5:
					printf(" (identifier)\n");
					break;
				case 6:
					printf(" (keyword)\n");
					break;
				case 7:
					printf(" (character literal)\n");
					break;
			}
											// clear string
			memset(output, 0, strlen(output));
											// get next status
			switch((int)l){
				/*
				 * Numerical - Literal
				 */
				case 48 ... 57:				// listing all possible starting characters
					// if in 0-9
					status = 1;				// set status to found 
					statusT = 2;			// set status type to int (numeric-literal)
					break;					// stop looking for a lexeme it it's found
				/*
				 * Strings
				 */
				case '"':					// " is the only starting character for strings
					status = 1;				// set status to found
					statusT = 1;			// set status type to string
					break;					// stop looking for a lexeme it it's found
				/*
				 * char literal
				 */
				case '\'':					// ' is the only starting character for character literals
					status = 1;
					statusT = 7;
					break;					// stop looking for a lexeme it it's found
				/*
				 *  Comments / Operators
				 */
				case '/':
					k = fgetc(fp);
					if( k == '*' ){			// if could be a comment;
						status = 1;
						statusT = 3;		// set status to comment it it's a comment
						break;				// stop looking for a lexeme type
					}else{
						ungetc(k, fp);		// if it isn't a comment, move on to see if it is an operator
						k='\0';				// forget look ahead
					}
				/*
				 * Operators
				 */ 
				case '<':
											// check if double character operator
					k = fgetc(fp);			// look ahead at the next character
					if( !( k=='=' || k=='<' || k=='>' ) ){
						ungetc(k, fp);		// move the pointer back
						k='\0';				// forget look ahead
					}
											// if could be an operator
					status = 1;
					statusT = 4;	
					break;					// stop looking for a lexeme it it's found
				case '>':
											// check if double character operator
					k = fgetc(fp);			// look ahead at the next character
					if( !( k=='=' || k=='<' ) ){
						ungetc(k, fp);		// move the pointer back
						k='\0';				// forget look ahead
					}
					// if could be an operator
					status = 1;
					statusT = 4;	
					break;					// stop looking for a lexeme it it's found
				case '=':
											// check if double character operator
					k = fgetc(fp);			// look ahead at the next character
					if( !( k=='>' ) ){
						ungetc(k, fp);		// move the pointer back
						k='\0';				// forget look ahead
					}
					// if could be an operator
					status = 1;
					statusT = 4;	
					break;					// stop looking for a lexeme it it's found
				case ':':
											// check if double character operator
					k = fgetc(fp);			// look ahead at the next character
					if( !( k=='=' ) ){
						ungetc(k, fp);		// move the pointer back
						k='\0';				// forget look ahead
					}
					// if could be an operator
					status = 1;
					statusT = 4;	
					break;					// stop looking for a lexeme it it's found
				case '.':
											// check if double character operator
					k = fgetc(fp);			// look ahead at the next character
					if( !( k=='.' ) ){
						ungetc(k, fp);		// move the pointer back
						k='\0';				// forget look ahead
					}
					// if it is be an operator
					status = 1;
					statusT = 4;	
					break;					// stop looking for a lexeme it it's found
				case '*':					// this is still an option since the case of /* has already been tested fo
											// check if double character operator
					k = fgetc(fp);			// look ahead at the next character
					if( !( k=='*' ) ){
						ungetc(k, fp);		// move the pointer back
						k='\0';				// forget look ahead
					}
					// if it is an operator
					status = 1;
					statusT = 4;	
					break;					// stop looking for a lexeme it it's found
				case '+':					// look for the other operators
				case '-':
				case '(':
				case ')':
				case '[':
				case ']':
				case '|':
				case '&':
				case ';':
				case ',':
					// if it is an operator
					status = 1;
					statusT = 4;	
					break;
				case '!':				// ! is a special case, since it cannot be alone as an operator
					k = fgetc(fp);
					if( k=='=' ){
						status = 1;
						statusT = 4;
						break;
					}else{
						printf("%c (UNK)", l);	// unknown status, ! is not a legal expression
						return 1;
					}
					break;
				/*
				 * Variable Names - reserved words
				 */ 
				case 65 ... 90:
				case 97 ... 122:
					status = 1;
					statusT = 5;
					// if could be a variable name
					break;					// stop looking for a lexeme it it's found
				case EOF:
					status = 0;
					statusT = 0;
					break;
				case ' ':
				case '\n':
				case '\r':
					l='\0';
					k='\0';
					status = 0;
					statusT = 0;
					break;					// stop looking for a lexeme it it's found
				default:
					printf("%c (UNK)", l);	// output the unknown lexeme
					return 1;				// return non-zero for error
			}
			
			output[strlen(output)] = l;
			output[strlen(output)] = k;
//			printf("output is now -> %s\n", output);
			l='\0'; // reset l
			k='\0'; // reset k
		}
	}
	fclose(fp);
}


int isletter(char x){
	int c = (int) x;
	return ( ( c >= 65 && c <= 90 ) || ( c >= 97 && c <= 122 ) );
}
int ishex(char x){
	int c = (int) x;
	return ( ( c >= 65 && c <= 70 ) || ( c >= 97 && c <= 102 ) || isnumber(x) );
}
int isnumber(char x){
	int c = (int) x;
	return ( c >= 48 && c <= 57 );
}
int isoperator(char x){
	int c = (int) x;
	// returns true if c is in [ ':', ';', '<', '=', '>', '?', '[', ']', '{', '|', '}', '.', '/' '(', ')', '*', '+', ',', '&']
	return ( ( c >= 123 && c <= 125 ) ||
				c == 93 || 
				c == 91 || 
				( c >= 58 && c <= 63 ) || 
				c == 47 || 
				c == 46 || 
				(c >=40 && c <= 44) || 
				c == 38
			);
}
