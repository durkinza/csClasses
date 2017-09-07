/* Zane C. Durkin
 * CS 210 
 * Hw1
 */

#include <stdio.h>
#include <strings.h>

FILE *fp;

int main(int argc, char *argv[]){
	if(argc < 2){
		printf("Missing input file");
		return 0;
	}
	if(! (fp=fopen(argv[1], "r")) ){
		printf("ERROR: File %s could not be accessed\n", argv[1]);
		return 1;
	}
	printf("Reading from %s\n", argv[1]);
	
	int status = 0;						// tells if machine is in a state or not
	int statusT = 0;					// holds the current state of the machine
	char l,k;							// l holds current character in file, k holds 1 character look ahead
	char output[256] = "";				// holds the output for each term;
	while( (l = fgetc(fp)) ){
		if(l == EOF){
			//printf("%s\n", output);
			break;
		}
		if(status > 0){
			switch(statusT){
				case 1:					// if in state of string
					output[strlen(output)] = l;
					if( l == '"' ){
						l = fgetc(fp);
						status = 0;
					}
					break;
				case 2:					// if in state of int (numerical-literal)
					switch(l){
						case '0':
						case '1':
						case '2':
						case '3':
						case '4':
						case '5':
						case '6':
						case '7':
						case '8':
						case '9':
						case '#':
						case '.':
						case '_':
							output[strlen(output)] = l;
							break;
						default:
							status = 0;
					}
					//['0 - 9', 'A-F', '_', '.', '#']
					// 16#FFF#
					break;
				case 3:					// if in state of Comment
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
				case 4:					// if in state of opperator
					status = 0;
					break;
				case 5:					// if in state of variable name
					switch(l){
						case 'a':
						case 'b':
						case 'c':
						case 'd':
						case 'e':
						case 'f':
						case 'g':
						case 'h':
						case 'i':
						case 'j':
						case 'k':
						case 'l':
						case 'm':
						case 'n':
						case 'o':
						case 'p':
						case 'q':
						case 'r':
						case 's':
						case 't':
						case 'u':
						case 'v':
						case 'w':
						case 'x':
						case 'y':
						case 'z':
						case '0':
						case '1':
						case '2':
						case '3':
						case '4':
						case '5':
						case '6':
						case '7':
						case '8':
						case '9':
						case '_':
//							printf("output len: %lu", strlen(output));
							output[strlen(output)] = l;
							break;
						default:
							status = 0;
							if( strstr("accessor and array begin bool case character constant else elsif end exit function if in integer interface is loop module mutator natural null of or others out positive procedure range return struct subtype then type when while", output) != NULL){
								statusT = 6;
							}
							/*switch(output){
								case "accessor":
								case "and":
								case "array":
								case "begin":
								case "bool":
								case "case":
								case "character":
								case "constant":
								case "else":
								case "elsif": 
								case "end": 
								case "exit": 
								case "function":
								case "if":
								case "in":
								case "integer":
								case "interface":
								case "is":
								case "loop":
								case "module":
								case "mutator":
								case "natural":
								case "null":
								case "of":
								case "or":
								case "others":
								case "out":
								case "positive":
								case "procedure":
								case "range":
								case "return":
								case "struct":
								case "subtype":
								case "then":
								case "type":
								case "when":
								case "while":
									statusT = 6; // set status to identifier
									break;
							}*/
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
			}
										// clear string
			memset(output, 0, strlen(output));
										// get next status
			switch(l){
				/*
				 * Numerical - Literal
				 */
				case '0':				// listing all possible starting characters
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					// if in 0-9
					status = 1;			// set status to found 
					statusT = 2;		// set status type to int (numeric-literal)
					break;
				/*
				 * Strings
				 */
				case '"':				// " is the only starting character for strings
					// if starts with "
					status = 1;			// set status to found
					statusT = 1;		// set status type to string
					break;
				/*
				 *  Comments / Operators
				 */
				case '/':
					k = fgetc(fp);
					if( k == '*' ){			// if could be a comment;
						status = 1;
						statusT = 3;
						break;
					}else{
						ungetc(k, fp);		// if it isn't a comment, move on to see if it is an operator
						k='\0'; // forget k
					}
				/*
				 * Operators
				 */ 
				case '<':
					// check if double character operator
					k = fgetc(fp);	// look ahead at the next character
					if( k=='=' || k=='<' || k=='>' ){
						break;		// stop looking for status
					}else{
						ungetc(k, fp); // move the pointer back
						k='\0'; // forget k
					}
				case '>':
					// check if double character operator
					k = fgetc(fp);	// look ahead at the next character
					if( k=='=' || k=='<' ){
						break;		// stop looking for status
					}else{
						ungetc(k, fp); // move the pointer back
						k='\0'; // forget k
					}
				case '=':
					// check if double character operator
					k = fgetc(fp);	// look ahead at the next character
					if( k=='>' ){
						break;		// stop looking for status
					}else{
						ungetc(k, fp); // move the pointer back
						k='\0'; // forget k
					}
				case ':':
					// check if double character operator
					k = fgetc(fp);	// look ahead at the next character
					if( k=='=' ){
						break;		// stop looking for status
					}else{
						ungetc(k, fp); // move the pointer back
						k='\0'; // forget k
					}
				case '.':
					// check if double character operator
					k = fgetc(fp);	// look ahead at the next character
					if( k=='.' ){
						break;		// stop looking for status
					}else{
						ungetc(k, fp); // move the pointer back
						k='\0'; // forget k
					}
				case '*': // this is still an option since the case of /* has already been tested fo
					// check if double character operator
					k = fgetc(fp);	// look ahead at the next character
					if( k=='*' ){
						break;		// stop looking for status
					}else{
						ungetc(k, fp); // move the pointer back
						k='\0'; // forget k
					}
				case '+':
				case '-':
				case '(':
				case ')':
				case '[':
				case ']':
				case '|':
				case '&':
				case ';':
				case ',':
					// if could be an operator
					status = 1;
					statusT = 4;	
					break;
				case '!': // ! is a special case, since it cannot be alone as an operator
					k = fgetc(fp);
					if( k=='=' ){
						break;
					}else{
						ungetc(k, fp);
						k='\0';
					}
					status = 0; // unknown status, ! is not a legal expression
					statusT = 0;
					break;
				/*
				 * Variable Names - reserved words
				 */ 
				case 'a':
				case 'b':
				case 'c':
				case 'd':
				case 'e':
				case 'f':
				case 'g':
				case 'h':
				case 'i':
				case 'j':
				case 'k':
				case 'l':
				case 'm':
				case 'n':
				case 'o':
				case 'p':
				case 'q':
				case 'r':
				case 's':
				case 't':
				case 'u':
				case 'v':
				case 'w':
				case 'x':
				case 'y':
				case 'z':
					status = 1;
					statusT = 5;
					// if could be a variable name
					break;
				case EOF:
					status = 0;
					statusT = 0;
					break;
				case ' ':
				case '\n':
				default:
					l='\0';
					k='\0';
					status = 0;
					statusT = 0;
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


