#include "ra_check.h"

enum TEST_SUBJECTS{
	INTERVAL=1,
	MTU,
	LIFETIME,
	PREFIX
};


/* returns 0 in case if the integer is not valid and 1
 * otherwise */
static int is_valid_int(const char* num){
	char* endptr;
	strtol(num, &endptr, 10);
	if(*endptr!='\0'){
#ifdef DEBUG
		fprintf(stderr, "[ %s ] %s is not a valid integer\n", 
				__func__, num);
#endif
		return 0;
	}
	return 1;
}

static inline int ra_check_interval(const char* min, const char* max){
	int min_int;
	int max_int;
#ifdef DEBUG
	fprintf(stderr, "[ %s ] $$$$$$$ BEGIN $$$$$$$\n", __func__);
#endif
	max_int = min_int = -1;
	if(!min || !max){
#ifdef DEBUG
		fprintf(stderr, "Null input.\n");
#endif
		return -1;
	}
	/* check if the supplies values are valid integers */
	if(!is_valid_int(min) || !is_valid_int(max)){
#ifdef DEBUG
		fprintf(stderr, "Invalid input.\n");
#endif
		return -1;
	}
	/* now relative validation i.e. if min and max values are 
	 * valid relative to each other */
	min_int = atoi(min);
	max_int = atoi(max);
#ifdef DEBUG
	fprintf(stderr, "min = %d, max = %d\n", min_int, max_int);
#endif
#ifdef DEBUG
	fprintf(stderr, "[ %s ] $$$$$$$ END $$$$$$$\n", __func__);
#endif
	return 0;
}

static inline int ra_check_lifetime(const char* min, const char* max){
	return 0;
}

static inline int ra_check_prefix(const char* min, const char* max){
	return 0;
}

static inline int ra_check_mtu(const char* min, const char* max){
	return 0;
}


static int ra_check(int ra_element, const char* arg1, const char* arg2){
	int ret;

	ret = -1;
#ifdef DEBUG
	fprintf(stderr, "[ %s ] ###### BEGIN ######\n", __func__);
	fprintf(stderr, "Formal arguments: \n");
	fprintf(stderr, "ra_element = %d, arg1 = %s, arg2 = %s\n", 
			ra_element, arg1, arg2);
#endif
	if(ra_element<0){
#ifdef DEBUG
		fprintf(stderr, "Invalid ra element under test\n");
#endif
		return -1;
	}
	switch(ra_element){
		case INTERVAL:
			ret = ra_check_interval(arg1, arg2);
			break;
		case LIFETIME:
			ret = ra_check_lifetime(arg1, arg2);
			break;
		case PREFIX:
			ret = ra_check_prefix(arg1, arg2);
			break;
		case MTU:
			ret = ra_check_mtu(arg1, arg2);
			break;
		default:
#ifdef DEBUG
			fprintf(stderr, "Unknown ra_element\n");
#endif
			return -1;
	}
#ifdef DEBUG
	fprintf(stderr, "[ %s ] ###### END ######\n", __func__);
#endif
	return ret;
}

static int parse_cmdline(int argc, char** argv){
	int c;
	int ra_element;
	int ret;

	struct option longopt[]={
		{"interval", 	no_argument, &ra_element, INTERVAL},
		{"mtu", 	no_argument, &ra_element, MTU},
		{"lifetime", 	no_argument, &ra_element, LIFETIME},
		{"prefix", 	no_argument, &ra_element, PREFIX},
		{"arg1", 	required_argument, 0, '1'},
		{"arg2", 	required_argument, 0, '2'},
		{0,0,0,0}
	};
	const char* shortopt="1:2:";
	const char* arg1, *arg2;

	ret = c = ra_element = -1;
	arg1 = arg2 = NULL;
	for(;;){
		int optindex = -1;
		c = getopt_long(argc, argv, shortopt, longopt,
				&optindex);
		if(c<0)
			break;
		switch(c){
			case 0:
				break;
			case '1':
				arg1 = strdup(optarg);
				break;
			case '2':
				arg2 = strdup(optarg);
				break;
		}
	}
	ret = ra_check(ra_element, arg1, arg2);
#ifdef DEBUG
	fprintf(stderr, "[ %s ] Return value = %d\n", __func__, ret);
#endif
	return ret;
}

int main(int argc, char* argv[]){
	if(parse_cmdline(argc, argv))
		exit(EXIT_FAILURE);
	
	return 0;
}
