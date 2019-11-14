#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>


void error(char *str){
	fprintf(stderr, "%s - %d: %s\n", str, errno, strerror(errno));
	exit(EXIT_FAILURE);
}

void matched_action(FILE *output, int out, int exclude)
{
	if(!exclude)
		fputc(out,output);
	return;
}

void unmatched_action(FILE *output, int out, int exclude, int binary)
{

	//we've already printed the newline
	if (out == '\n' && !binary)
		return;

	if (exclude)
		fputc(out,output);

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

void print_help(void)
{
        char * help_string = 
"char_filter: a very simple character-by-character filter\n"
"default: prints if ascii\n"
"options: \n"
"\t-a:\tcheck alpha\n"
"\t-b:\tcheck blank\n"
"\t-B:\tbinary mode (process newlines)\n"
"\t-c:\tcheck control characters\n"
"\t-d:\tcheck digit\n"
"\t-g:\tcheck graph\n"
"\t-l:\tcheck lower case\n"
"\t-m:\tcheck alphanumeric\n"
"\t-p:\tcheck punctuation\n"
"\t-u:\tcheck upper case\n"
"\t-t:\tcheck template of provided chars\n"
"\t-i:\tinput file\n"
"\t-o:\toutput file\n"
"\t-x:\texclusive mode, doesn't print if matched\n";
        fprintf(stderr,help_string);
}

int main(int argc, char *argv[])
{
	int in;
	int opt;
	char *buf = malloc(8096);
	
	int exclude = 0;
	int check_alpha = 0;
	int check_lower = 0;
	int check_upper = 0;
	int check_digit = 0;
	int check_alnum = 0;
	int check_punct = 0;
	int check_space = 0;
	int check_blank = 0;
	int check_graph = 0;
	int check_control = 0;
	int check_ascii = 0;
	int check_template = 0;
	char *template = "";	
	int binary = 0;
	int options = 0;	

	int input_file = 0;
	char *infile;
	int output_file = 0;
	char *outfile;
	
	FILE *input;
	FILE *output;

	while ((opt = getopt(argc, argv, "habBcdglmpuxt:i:o:")) != -1){
		switch(opt){
		case 'a':
			options = 1;
			check_alpha = 1;
			break;
		case 'b':
			options = 1;
			check_blank = 1;
			break;
		case 'B':
			options = 1;
			binary = 1;
			break;
		case 'c':
			options = 1;
			check_control = 1;
			break;
		case 'd':
			options = 1;
			check_digit = 1;
			break;
		case 'g':
			options = 1;
			check_graph = 1;
			break;
		case 'l':
			options = 1;
			check_lower = 1;
			break;
		case 'm':
			options = 1;
			check_alnum = 1;
			break;
		case 'p':
			options = 1;
			check_punct = 1;
			break;
		case 'u':
			options = 1;
			check_upper = 1;
			break;
		case 'x':
			options = 1;
			exclude = 1;
			break;
		case 't':
			options = 1;
			check_template = 1;
			template = optarg;
			break;
		case 'i':
			input_file = 1;
			infile = optarg;
			break;
		case 'o':
			output_file = 1;
			outfile = optarg;
			break;
                case 'h':
                        print_help();
                        exit(0);
                        break;
		case '?':
			if (optopt == 't')
				fprintf(stderr,"Option -t requires a string of characters to be filtered\n");
			else if (optopt == 'i')
				fprintf(stderr, "Option -i requires an input file\n");
			else if (optopt == 'o')
				fprintf(stderr, "Option -o requires an output file\n");
			
			exit(EXIT_FAILURE);
			break;
		default:
			exit(1);
			break;
		}
	}
	
	if (!options){
		check_ascii = 1;
	}

	if(input_file){
		if ( (input = fopen(infile,"r")) == NULL)
			error("Input file opening failed");
	} else {
		input = stdin;
	}

	if(output_file){
		if ( (output = fopen(outfile,"w")) == NULL)
			error("Output file opening failed");
	} else {
		output = stdout;
	}

	setvbuf(input, buf, _IOFBF, 8096);

	for(;;){
		in = fgetc(input);

		if (in < 0)
			break;

		if (in == '\n' && !binary ){
			fputc(in, stdout);
			continue;
		}

		if (check_template){
			if(test_template(in,template)){
				matched_action(output, in, exclude);
				continue;
			}
		}

		if (check_alpha){	
			if (isalpha(in)){
				matched_action(output,in, exclude);
				continue;
			}
		}
		
		if (check_lower){
			if (islower(in)){
				matched_action(output,in, exclude);
				continue;
			}
		}

		if (check_upper){
			if (isupper(in)){
				matched_action(output, in, exclude);
				continue;
			}
		}
		
		if (check_digit){
			if (isdigit(in)){
				matched_action(output, in, exclude);
				continue;
			}
		}

		if (check_alnum){
			if (isalnum(in)){
				matched_action(output, in, exclude);
				continue;
			}
		}


		if (check_punct){
			if (ispunct(in)){
				matched_action(output, in, exclude);
				continue;
			}
		}

		if (check_space){
			if (isspace(in)){
				matched_action(output, in, exclude);
				continue;
			}
		}


		if (check_blank){
			if (isblank(in)){
				matched_action(output, in, exclude);
				continue;
			}
		}

		if (check_graph){
			if (isgraph(in)){
				matched_action(output, in, exclude);
				continue;
			}
		}

		if (check_control){
			if (iscntrl(in)){
				matched_action(output, in, exclude);
				continue;
			}
		}

		if (check_ascii){
			if ( in >= 0x00 && in <= 0x7F ){
				matched_action(output, in, exclude);
				continue;
			}
		}

		unmatched_action(output,in, exclude, binary);
	}

	return 0;
}
