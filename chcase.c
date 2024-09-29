#include <stdio.h>
#include <getopt.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>
#ifdef _WIN32
#  define ROOT "C:"
#else
#  define ROOT ""
#endif
#ifndef PREFIX
#  define PREFIX ROOT "/opt"
#endif

char const HELP[] =
    "Usage: chcase [-CSL][-d DICT] < TEXT"                                      "\n"
    ""                                                                          "\n"
    "Change the casing of the input text to CamelCase (-C), snake_case (-S) or" "\n"
    "lisp-case (-L). By default it prints the word splet by spaces."            "\n"
    ""                                                                          "\n"
    "For CamelCase special words such as 'API' or 'URL' can be defined in a"    "\n"
    "dictionary, by default \"%s\", you can place extra"                        "\n"
    "words with -d or by setting the CHCASE_DICT environment variable."         "\n"
    ""                                                                          "\n"
    "Copyright (C) 2024 Harkaitz Agirre, OpenBSD ISC license."                  "\n"
    ;

struct chr_info {
	char	chr;
	bool	is_upper;
	bool	is_space;
};

enum casing {
	WHITESPACE = 0,
	CAMELCASE  = 1,
	SNAKECASE  = 2,
	LISPCASE   = 3,
};


static struct chr_info chrinfo(char _c);
static void PrintCamelCase(char _b[]);
static void print_snake_case(char _b[], char _sep);

char const *dict0 = NULL;
char const *dict1 = PREFIX "/share/chcase/chcase.dict";


int main(int _argc, char *_argv[])
{
	
	enum	casing casing = WHITESPACE;
	int	chr,opt;
	struct	chr_info prev = {' ', false, true};
	struct	chr_info curr;
	
	if (_argc > 1 && (!strcmp(_argv[1], "-h") || !strcmp(_argv[1], "--help"))) {
		printf(HELP, dict1);
		return 0;
	}
	
	while((opt = getopt (_argc, _argv, "CSLd:")) != -1) {
		switch (opt) {
		case 'C': casing = CAMELCASE; break;
		case 'S': casing = SNAKECASE; break;
		case 'L': casing = LISPCASE; break;
		case 'd': dict0 = optarg; break;
		case '?':
		default:
			return 1;
		}
	}
	
	char	norm[1024];
	size_t	normsz = 0;
	
	for (;(chr = fgetc(stdin)) != EOF; prev = curr) {
		
		/* Spaces clear. */
		if (strchr(" \t\r\n", chr)) {
			norm[normsz++] = '\0';
			switch (casing) {
			case CAMELCASE: PrintCamelCase(norm); break;
			case SNAKECASE: print_snake_case(norm, '_'); break;
			case LISPCASE:  print_snake_case(norm, '-'); break;
			default:        fwrite(norm, 1, normsz, stdout); break;
			}
			fputc(chr, stdout);
			normsz = 0;
			curr = chrinfo(' ');
			continue;
		}
		
		/* Analyze character. */
		curr = chrinfo(chr);
		
		/* Camel case. */
		if (curr.is_upper) {
			if (prev.is_upper) {
				norm[normsz-1] = toupper(prev.chr);
				norm[normsz++] = chr;
			} else if (prev.is_space) {
				norm[normsz++] = tolower(chr);
			} else {
				norm[normsz++] = ' ';
				norm[normsz++] = tolower(chr);
			}
			continue;
		}
		
		/* Snake case. */
		if (curr.is_space) {
			norm[normsz++] = ' ';
			continue;
		}
		
		norm[normsz++] = chr;
	}
}

static struct chr_info
chrinfo(char _c)
{
	struct chr_info info = {};
	info.chr = _c;
	info.is_upper = isupper(_c);
	info.is_space = strchr(" \t\r\n-_", _c) != NULL;
	return info;
}

static bool
dictprint_fp(char const _word[], FILE *_fp)
{
	char buf[1024];
	while (fgets(buf, sizeof(buf), _fp)) {
		char *p;
		if ((p = strchr(buf, '\n'))) *p = '\0';
		if ((p = strchr(buf, '\r'))) *p = '\0';
		if (!strcasecmp(buf, _word)) {
			fputs(buf, stdout);
			return true;
		}
	}
	return false;
}

static void
dictprint(char const _word[])
{
	FILE	*fp;
	bool	 found;
	char	*envvar;
	if (dict0 && (fp = fopen(dict0, "r"))) {
		found = dictprint_fp(_word, fp);
		fclose(fp);
		if (found) return;
		
	}
	envvar = getenv("CHCASE_DICT");
	if (envvar && (fp = fopen(envvar, "r"))) {
		found = dictprint_fp(_word, fp);
		fclose(fp);
		if (found) return;
		
	}
	
	if (dict1 && (fp = fopen(dict1, "r"))) {
		found = dictprint_fp(_word, fp);
		fclose(fp);
		if (found) return;
		
	}
	fputs(_word, stdout);
}




static void
PrintCamelCase(char _b[])
{
	char	*p;
	for (p = strtok(_b, " "); p; p = strtok(NULL, " ")) {
		p[0] = toupper(p[0]);
		dictprint(p);
	}
}

static void
print_snake_case(char _b[], char _sep)
{
	char	*p;
	bool	first = true;
	size_t	i;
	
	for (p = strtok(_b, " "); p; p = strtok(NULL, " ")) {
		if (!first) {
			fputc(_sep, stdout);
		}
		for (i = 0; p[i]; i++) {
			p[i] = tolower(p[i]);
		}
		fputs(p, stdout);
		first = false;
	}
}
