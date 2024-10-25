#include "nemu.h"
#include "cpu/reg.h"
#include "memory/memory.h"
#include "cpu/cpu.h"

#include <elf.h>
#include <string.h>
#include <stdlib.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE = 256,
	EQ,
	NEQ,
	LEQ,
	GEQ,
	AND,
	OR,
	HEX,
	DEC,
	REG,
	SYMB,
	DEREF

	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +", NOTYPE}, 					// white space
	{"\\+", '+'}, 						// add
	{"-", '-'}, 						// sub
	{"\\*", '*'}, 						// mul
	{"/", '/'}, 						// div
	{"\\(", '('}, 						// left
	{"\\)", ')'}, 						// right
	{"<=", LEQ}, 						// less or equal
	{">=", GEQ}, 						// greater or equal
	{"<", '<'}, 						// less
	{">", '>'}, 						// greater
	{"==", EQ}, 						// equal
	{"!=", NEQ},						// not equal
	{"&&", AND},						// and
	{"\\|\\|", OR},						// or
	{"!", '!' },						// not
	{"0x[0-9a-fA-F]+", HEX},			// hex number
	{"[0-9]+", DEC},					// dec number
	{"\\$[a-zA-Z]+", REG},				// register
	{"[a-zA-Z_][a-zA-Z0-9_]*", SYMB},	// symbol
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]))

static regex_t re[NR_REGEX];

/* Rules are used for more times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for (i = 0; i < NR_REGEX; i++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if (ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			assert(ret != 0);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

static int num(char c) {
	if (c >= '0' && c <= '9') {
		return c - '0';
	} else if (c >= 'a' && c <= 'f') {
		return c - 'a' + 10;
	} else if (c >= 'A' && c <= 'F') {
		return c - 'A' + 10;
	} else {
		assert(0);
	}
}

static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;

	nr_token = 0;

	while (e[position] != '\0') {
		/* Try all rules one by one. */
		for (i = 0; i < NR_REGEX; i++) {
			if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				// printf("match regex[%d] at position %d with len %d: %.*s\n", i, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. 
				 * Add codes to perform some actions with this token.
				 */

				switch (rules[i].token_type) {
				case NOTYPE:
					break;
				case HEX:
					tokens[nr_token].type = rules[i].token_type;
					for (int j = 2; j < substr_len; j++) {
						tokens[nr_token].str[j - 2] = substr_start[j];
					}
					tokens[nr_token].str[substr_len - 2] = '\0';
					nr_token++;
					break;
				case DEC:
					tokens[nr_token].type = rules[i].token_type;
					for (int j = 0; j < substr_len; j++) {
						tokens[nr_token].str[j] = substr_start[j];
					}
					tokens[nr_token].str[substr_len] = '\0';
					nr_token++;
					break;
				case REG:
				case SYMB:
					tokens[nr_token].type = rules[i].token_type;
					for (int j = 0; j < substr_len; j++) {
						tokens[nr_token].str[j] = substr_start[j];
					}
					tokens[nr_token].str[substr_len] = '\0';
					nr_token++;
					break;
				default:
					tokens[nr_token].type = rules[i].token_type;
					nr_token++;
				}

				break;
			}
		}

		if (i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true;
}

static bool check_parentheses(int p, int q) {
	int cnt = 0;
	for (int i = p; i <= q; i++) {
		if (tokens[i].type == '(') {
			cnt++;
		} else if (tokens[i].type == ')') {
			cnt--;
		}
	}
	assert(cnt == 0);
	if (tokens[p].type == '(' && tokens[q].type == ')') {
		cnt = 0;
		for (int i = p + 1; i <= q - 1; i++) {
			if (tokens[i].type == '(') {
				cnt++;
			} else if (tokens[i].type == ')') {
				cnt--;
			}
			if (cnt < 0) {
				return false;
			}
		}
		if (cnt == 0) {
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

extern char *strtab;
extern Elf32_Sym *symtab;
extern int nr_symtab_entry;

static int tokens_priority(int type) {
	switch (type) {
		case '!': return 5;
		case DEREF: return 5;
		case '*': return 4;
		case '/': return 4;
		case '+': return 3;
		case '-': return 3;
		case '<': return 2;
		case '>': return 2;
		case LEQ: return 2;
		case GEQ: return 2;
		case EQ: return 2;
		case NEQ: return 2;
		case AND: return 1;
		case OR: return 0;
		default: return 0x7fffffff;
	}
}

static uint32_t eval(int p, int q) {
    if(p > q) {
        return 0;
    } else if(p == q) { 

        /* Single token.
         * For now this token should be a number. 
         * Return the value of the number.
         */ 
		
		if (tokens[p].type == REG) {
			if (strcmp(tokens[p].str, "$eax") == 0) {
				return cpu.eax;
			} else if (strcmp(tokens[p].str, "$ecx") == 0) {
				return cpu.ecx;
			} else if (strcmp(tokens[p].str, "$edx") == 0) {
				return cpu.edx;
			} else if (strcmp(tokens[p].str, "$ebx") == 0) {
				return cpu.ebx;
			} else if (strcmp(tokens[p].str, "$esp") == 0) {
				return cpu.esp;
			} else if (strcmp(tokens[p].str, "$ebp") == 0) {
				return cpu.ebp;
			} else if (strcmp(tokens[p].str, "$esi") == 0) {
				return cpu.esi;
			} else if (strcmp(tokens[p].str, "$edi") == 0) {
				return cpu.edi;
			}
		} else if (tokens[p].type == HEX || tokens[p].type == DEC) {
			uint32_t res = 0;
			if (tokens[p].type == DEC) {
				for (int i = 0; tokens[p].str[i] != 0; i++) {
					res = res * 10 + num(tokens[p].str[i]);
				}
			} else {
				for (int i = 0; tokens[p].str[i] != 0; i++) {
					res = res * 16 + num(tokens[p].str[i]);
				}
			}
			return res;
		} else if (tokens[p].type == SYMB) {
			char *sym = tokens[p].str;
			char str[32];
			for (int i = 0; i < nr_symtab_entry; i++) {
				strcpy(str, strtab + symtab[i].st_name);
				if (strcmp(sym, str) == 0) {
					return symtab[i].st_value;
				}
			}
		}

    } else if(check_parentheses(p, q) == true) {

        /* The expression is surrounded by a matched pair of parentheses. 
         * If that is the case, just throw away the parentheses.
         */

        return eval(p + 1, q - 1); 

    } else {
		int op = -1;
		int op_type = 0;
		int cnt = 0;
		for (int i = p; i <= q; i++) {
			if (tokens[i].type == '(') {
				cnt++;
				continue;
			} else if (tokens[i].type == ')') {
				cnt--;
				continue;
			}
			if (cnt == 0 && check_parentheses(p, i - 1) && check_parentheses(i + 1, q)) {
				op = i;
				op_type = tokens[i].type;
				break;
			}
		}
		if (op == -1) {
			cnt = 0;
			for (int i = p; i <= q; i++) {
				if (tokens[i].type == '(') {
					cnt++;
					continue;
				} else if (tokens[i].type == ')') {
					cnt--;
					continue;
				}
				if (cnt == 0 && tokens_priority(tokens[i].type) < tokens_priority(op_type)) {
					op = i;
					op_type = tokens[i].type;
				}
			}
		}
        
		uint32_t val1 = eval(p, op - 1);
        uint32_t val2 = eval(op + 1, q);

        switch(op_type) {
            case '+': return val1 + val2;
            case '-': return val1 - val2;
            case '*': return val1 * val2;
            case '/': return val1 / val2;
			case DEREF: return vaddr_read(val2, SREG_DS, 4);
			case EQ: return val1 == val2;
			case NEQ: return val1 != val2;
			case LEQ: return val1 <= val2;
			case GEQ: return val1 >= val2;
			case '<': return val1 < val2;
			case '>': return val1 > val2;
			case AND: return val1 && val2;
			case OR: return val1 || val2;
			case '!': return !val2;
            default: break;
        }
    }
	assert(0);
}

uint32_t expr(char *e, bool *success) {
	if (!make_token(e)) {
		*success = false;
		return 0;
	}

	for(int i = 0; i < nr_token; i ++) {
    	if(tokens[i].type == '*' && (i == 0 || tokens[i - 1].type == '+'  
											|| tokens[i - 1].type == '-' 
											|| tokens[i - 1].type == '*'
											|| tokens[i - 1].type == '/')) {
    	    tokens[i].type = DEREF;
    	}
	}

	return eval(0, nr_token - 1);
}
