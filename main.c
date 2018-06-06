#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

void matched_action(int out, int exclusive)
{
	if(exclusive)
		fputc(out,stdout);
	return;
}

void unmatched_action(int out, int exclusive, int binary)
{

	//we've already printed the newline
	if (out == '\n' && !binary)
		return;

	if (!exclusive)
		fputc(out,stdout);

	return;
}

int test_template(int in, char *template)
{
	char *crs;
	for (crs = template; *crs != '\0'; crs++)
		if (in == *crs)
			return 1;

	return 0;
}

int main()
{
	int in;
	char *buf = malloc(8096);

	setvbuf(stdin, buf, _IOFBF, 8096);

	int exclusive = 1;
	int check_alpha = 0;
	int check_lower = 0;
	int check_upper = 0;
	int check_digit = 0;
	int check_punct = 0;
	int check_space = 0;
	int check_blank = 0;
	int check_graph = 0;
	int check_control = 0;
	int check_ascii = 0;
	int check_template = 1;
	int binary = 0;

	char *template = "abcdefghij ";	

	for(;;){
		in = fgetc(stdin);

		if (in < 0)
			break;

		if (in == '\n' && !binary ){
			fputc(in, stdout);
			continue;
		}

		if (check_template){
			if(test_template(in,template)){
				matched_action(in,exclusive);
				continue;
			}
		}

		if (check_alpha){	
			if (isalpha(in)){
				matched_action(in, exclusive);
				continue;
			}
		}
		
		if (check_lower){
			if (islower(in)){
				matched_action(in, exclusive);
				continue;
			}
		}

		if (check_upper){
			if (isupper(in)){
				matched_action(in,exclusive);
				continue;
			}
		}
		
		if (check_digit){
			if (isdigit(in)){
				matched_action(in,exclusive);
				continue;
			}
		}

		if (check_punct){
			if (ispunct(in)){
				matched_action(in,exclusive);
				continue;
			}
		}

		if (check_space){
			if (isspace(in)){
				matched_action(in,exclusive);
				continue;
			}
		}


		if (check_blank){
			if (isblank(in)){
				matched_action(in,exclusive);
				continue;
			}
		}

		if (check_graph){
			if (isgraph(in)){
				matched_action(in,exclusive);
				continue;
			}
		}

		if (check_control){
			if (iscntrl(in)){
				matched_action(in,exclusive);
				continue;
			}
		}

		if (check_ascii){
			if ( in >= 0x00 && in <= 0x127 ){
				matched_action(in,exclusive);
				continue;
			}
		}

		unmatched_action(in, exclusive, binary);
	}

	return 0;
}
