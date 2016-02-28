#include <string.h>
#include "clish/shell.h"
int boot_mode;
int tac_srv_no;
tacplus_server_t tac_srv[MAX_TACPLUS_SERVERS];

static int add_server_to_list(tacplus_server_t srv){
	tac_srv[tac_srv_no++] = srv;
	fprintf(stderr, "added server to list\n");
	return 0;
}

int add_tacplus_server(char* line){
	char *saveptr = NULL;
	char *token = NULL;
	tacplus_server_t srv;
	srv.flags = 0xff;
	srv.in_use = 0;
	srv.connected = 0;
	int i;

	i = 0;
	token = strtok_r(line, " ", &saveptr);
	while(token){
		switch(i){
			case 3:
				strncpy(srv.host, token, 256);
				srv.in_use = 1;
				break;
			case 5:
				strncpy(srv.key, token, 256);
				break;
		}
		token = strtok_r(NULL, " ", &saveptr);
		i++;
	}

	add_server_to_list(srv);
}

int load_tacplus_servers(void){
	FILE *pd = NULL;
	char line[1024];
	
	fprintf(stderr, "[%s ]Loading tacplus servers\n", __func__);	
	strcpy(tac_srv[0].host, "1.2.3.4");
	strcpy(tac_srv[0].key, "KEy");
	
	pd = popen("/usr/local/bin/konf -- -d -r \"auth tacplus\"", "r");	
	if(!pd){
		fprintf(stderr, "Error loading tacplus servers\n");
		return -1;
	}
	memset(line, '\0', sizeof(line));
	while(fgets(line, sizeof(line)-1, pd)){
		if(line[0] == '!')
			continue;
		fprintf(stderr, "line = %s\n", line);
		add_tacplus_server(line);
	}
}

void dump_tacplus_servers(void){
	int i;
	for(i=0; i<MAX_TACPLUS_SERVERS;i++){
		if(tac_srv[i].in_use){
			fprintf(stderr, "host = %s\n", tac_srv[i].host);
			fprintf(stderr, "key = %s\n", tac_srv[i].key);
		}
	}
}
