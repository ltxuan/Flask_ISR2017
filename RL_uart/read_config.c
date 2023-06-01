#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_config.h"
void del_line(char *src){
	if (src[strlen(src)-1] == '\n') src[strlen(src)-1] ='\0';
}
int find(char *src, char *key){
	int c;
	int b;
	char *str;
	str = strstr(src,key);
	if (str != NULL){
		c = (int) src;
		b = (int) str;
		return b-c;
	}
	else return -1;
}
char *trim(char *src, int start, int end){
	char *val;
	val = malloc(end-start+1);
	int i;
	for (i = start; i < end; i++){
		val[i-start] = (char)src[i];
	}
 val[end -start] = '\0';
	return val;
}
void read_config(struct config *ar_config, struct codec_setting *ar_codec_setting, struct setting *app_setting, struct dial_plan *dial_plan, char *dir)
{
	int num;
	FILE *fptr;
	int line_size;
	char str_line[300];
	int count = -1;
	int codec_idx = -1;
	int index = 0;
	fptr = fopen(dir,"r");

	if(fptr == NULL)
	{
		printf("open file config Error!");   
		exit(1);             
	}
	while(fgets(str_line, 300, fptr) != NULL){
		if (find(str_line, "account") != -1) count++;
		if (find(str_line, "codec_set") != -1) codec_idx++;
		ar_config[count].call_number = pj_str("idle");
		ar_config[count].acc_id = -1;
		index = find(str_line, "=");
		if (index != -1){
			char *key = trim(str_line,0,index);
			char *value = trim(str_line, index + 1, strlen(str_line)-1);
			if (strcmp(key, "sound_id") == 0){
				ar_config[count].sound_id = atoi(value);
				printf("%d \n",ar_config[count].sound_id);
			}
			else if (strcmp(key, "domain") == 0){
				strcpy(ar_config[count].domain, value);
				printf("%s \n",ar_config[count].domain);
			}
			else if (strcmp(key, "username") == 0){
				strcpy(ar_config[count].username, value);
				printf("%s \n",ar_config[count].username);
			}
			else if (strcmp(key, "secret") == 0){
				strcpy(ar_config[count].secret, value);
				printf("%s \n",ar_config[count].secret);
			}
			else if (strcmp(key, "sip_port") == 0){
				ar_config[count].sip_port = atoi(value);
				printf("%d \n",ar_config[count].sip_port);				
			}
			else if (strcmp(key, "sound_name") == 0){
				strcpy(ar_config[count].sound_name, value);
				printf("%s \n",ar_config[count].sound_name);
			}
			else if (strcmp(key, "status") == 0){
				strcpy(ar_config[count].status, value);
				printf("status %s \n",ar_config[count].status);
			}
			else if (strcmp(key, "transport") == 0){
				strcpy(ar_config[count].transport, value);
				printf("transport %s \n",ar_config[count].transport);
			}
			else if (strcmp(key, "hotline") == 0){
				strcpy(ar_config[count].hotline, value);
				printf("hotline %s \n",ar_config[count].hotline);
			}
			else if (strcmp(key, "h_status") == 0){
				strcpy(ar_config[count].h_status, value);
				printf("%s \n",ar_config[count].h_status);				
			}
			else if (strcmp(key, "echo_enable") == 0){
				strcpy(echo_enable, value);
				printf("==================echo%s \n",echo_enable);
			}
			
			/* Read codec config */
			if (strcmp(key, "speex/16000/1") == 0){
				strcpy(ar_codec_setting[codec_idx].codec_name, "speex/16000/1");
				ar_codec_setting[codec_idx].codec_pri = atoi(value);
				codec_idx++;
			}
			else if (strcmp(key, "speex/8000/1") == 0){
				strcpy(ar_codec_setting[codec_idx].codec_name, "speex/8000/1");
				ar_codec_setting[codec_idx].codec_pri = atoi(value);
				codec_idx++;
			}
			else if (strcmp(key, "speex/32000/1") == 0){
				strcpy(ar_codec_setting[codec_idx].codec_name, "speex/32000/1");
				ar_codec_setting[codec_idx].codec_pri = atoi(value);
				codec_idx++;
			}
			else if (strcmp(key, "iLBC/8000/1") == 0){
				strcpy(ar_codec_setting[codec_idx].codec_name, "iLBC/8000/1");
				ar_codec_setting[codec_idx].codec_pri = atoi(value);
				codec_idx++;
			}
			else if (strcmp(key, "GSM/8000/1") == 0){
				strcpy(ar_codec_setting[codec_idx].codec_name, "GSM/8000/1");
				ar_codec_setting[codec_idx].codec_pri = atoi(value);
				codec_idx++;
			}
			else if (strcmp(key, "PCMU/8000/1") == 0){
				strcpy(ar_codec_setting[codec_idx].codec_name, "PCMU/8000/1");
				ar_codec_setting[codec_idx].codec_pri = atoi(value);
				codec_idx++;
			}
			else if (strcmp(key, "PCMA/8000/1") == 0){
				strcpy(ar_codec_setting[codec_idx].codec_name, "PCMA/8000/1");
				ar_codec_setting[codec_idx].codec_pri = atoi(value);
				codec_idx++;
			}
			else if (strcmp(key, "G722/16000/1") == 0){
				strcpy(ar_codec_setting[codec_idx].codec_name, "G722/16000/1");
				ar_codec_setting[codec_idx].codec_pri = atoi(value);
				codec_idx++;
			}
			else if (strcmp(key, "AMR/8000/1") == 0){
				strcpy(ar_codec_setting[codec_idx].codec_name, "AMR/8000/1");
				ar_codec_setting[codec_idx].codec_pri = atoi(value);
				codec_idx++;
			}
			else if (strcmp(key, "opus/48000/2") == 0){
				strcpy(ar_codec_setting[codec_idx].codec_name, "opus/48000/2");
				ar_codec_setting[codec_idx].codec_pri = atoi(value);
				codec_idx++;
			}
			else if (strcmp(key, "G729/8000/1") == 0){
				strcpy(ar_codec_setting[codec_idx].codec_name, "G729/8000/1");
				ar_codec_setting[codec_idx].codec_pri = atoi(value);
				codec_idx++;
			}
			else if (strcmp(key, "L16/44100/2") == 0){
				strcpy(ar_codec_setting[codec_idx].codec_name, "L16/44100/2");
				ar_codec_setting[codec_idx].codec_pri = atoi(value);
				codec_idx++;
			}
			else if (strcmp(key, "L16/44100/1") == 0){
				strcpy(ar_codec_setting[codec_idx].codec_name, "L16/44100/1");
				ar_codec_setting[codec_idx].codec_pri = atoi(value);
				codec_idx++;
			}

			/* read commont setting here */
			if (strcmp(key, "use_srtp") == 0){
				app_setting[0].use_srtp = atoi(value);
			}
			/* read dial_plan */
			if (strcmp(key, "digit1") == 0){
				dial_plan->digit1 = atoi(value);
			}
			else if (strcmp(key, "digit2") == 0){
				dial_plan->digit2 = atoi(value);
			}	
			else if (strcmp(key, "digit1") == 0){
				dial_plan->digit2 = atoi(value);
			}	
			else if (strcmp(key, "digit3") == 0){
				dial_plan->digit3 = atoi(value);
			}	
			else if (strcmp(key, "digit4") == 0){
				dial_plan->digit4 = atoi(value);
			}	
			else if (strcmp(key, "digit5") == 0){
				dial_plan->digit5 = atoi(value);
			}	
			else if (strcmp(key, "digit6") == 0){
				dial_plan->digit6 = atoi(value);
			}	
			else if (strcmp(key, "digit7") == 0){
				dial_plan->digit7 = atoi(value);
			}	
			else if (strcmp(key, "digit8") == 0){
				dial_plan->digit8 = atoi(value);
			}	
			else if (strcmp(key, "digit9") == 0){
				dial_plan->digit9 = atoi(value);
			}	
			else if (strcmp(key, "digit0") == 0){
				dial_plan->digit0 = atoi(value);
			}	
		}
	}
}

