#include <pjsua-lib/pjsua.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "uart.h"
#include "read_config.h"

#include <spandsp.h>
#include <spandsp-sim.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <sndfile.h>

#define THIS_FILE "APP"

#define USE_CUCM 1
#define CHANNEL_NUM 24
#define TLS_KEY_PATH "/etc/asterisk/keys/"
//id:1$domain:192.168.1.86$status:1$secret:Pin996895$username:996895

#define DEFAULT_DTMF_TX_LEVEL       -10
#define DEFAULT_DTMF_TX_ON_TIME     50
#define DEFAULT_DTMF_TX_OFF_TIME    50


#define SAMPLES_PER_CHUNK     16000
//#define SAMPLES_PER_CHUNK     160

#define MAX_FRAME 128


struct buf_msg
{
    char msg[200];
    unsigned char avail;
};

char stm32_status = 0;
struct buf_msg web_msg[24]; 
pj_pool_t *pool;
pjmedia_port *conf;
struct config ar_config[24];
struct codec_setting ar_codec_setting[NUM_CODEC];
struct setting app_setting[1]; // setting common para
struct dial_plan local_dial;

pjsua_transport_id cfg_transport_id[24];
pjsua_transport_id cfg_transport_default_id;
pjsua_transport_config cfg_default_transport;
pjsua_transport_config cfg_transport[24];
pj_mutex_t *mutex;
pj_int16_t samplebuf[MAX_FRAME];

pj_int16_t ready_flag = 0;

int16_t left_amp[24][1000000];
int16_t right_amp[24][1000000];

//int16_t amp[SAMPLES_PER_CHUNK];


dtmf_rx_state_t *dtmf_state;
int max_forward_twist;
int max_reverse_twist;
int use_dialtone_filter = FALSE;
codec_munge_state_t *munge = NULL;
int total;
pj_status_t status;

int samples = SAMPLES_PER_CHUNK;

static pj_status_t left_dtmf_decode(pjmedia_port *port, void *usr_data)
{
	int actual;
	char buf[MAX_FRAME + 1];
//	actual = 0;
//	pj_str_t buf;
//	buf = (char *) malloc(100*sizeof(char));
//	buf.slen = 1000;

	//int16_t amp[10000000];

	dtmf_state = dtmf_rx_init(NULL, NULL, NULL);
	dtmf_rx(dtmf_state, left_amp, samples);
	actual = dtmf_rx_get(dtmf_state, buf, MAX_FRAME); 
	//printf("======= actual = %d \n", actual);
	if (actual != 0) {
	//	printf("======= actual = %d \n", actual);
		printf("======= SPANDPS receive dtmf digit '%s'\n", buf);
	} else {
//		printf("======= SPANDPS receive dtmf digit '%s'\n", buf);
	}
	//int send_digit;
	//send_digit = atoi(buf);
       	//printf("========= send digit = %d \n", send_digit);
//	printf("========= send dtmf \n");
	
	/*pjsua_call_send_dtmf_param send_dtmf_param;
	send_dtmf_param.method = PJSUA_DTMF_METHOD_SIP_INFO;
	send_dtmf_param.duration = PJSUA_CALL_SEND_DTMF_DURATION_DEFAULT;
	send_dtmf_param.digits.ptr =  &buf[0];
	send_dtmf_param.digits.slen = 129;*/
//	printf("===================== usr data = %d \n", usr_data);
//	if (usr_data > 0) {
//		printf("============== call_id > 0 \n");
//		pjsua_call_send_dtmf((pjsua_call_id*) usr_data, &send_dtmf_param);
//	}
/*
	
	FILE *f = fopen("/home/isr2017/workspace/source_code/RL_uart/data.txt", "a");
	for (int i =0; i<16000; i++) {
		fprintf(f, " %d", amp[i]);
	}
	fclose(f);
*/	
	return PJ_SUCCESS;
}

static pj_status_t right_dtmf_decode(pjmedia_port *port, void *usr_data)
{
	int actual;
	char buf[128 + 1];
	dtmf_state = dtmf_rx_init(NULL, NULL, NULL);
	dtmf_rx(dtmf_state, right_amp, samples);
	actual = dtmf_rx_get(dtmf_state, buf, 128); 
	//printf("======= actual = %d \n", actual);
	if (actual != 0) {
	//	printf("======= actual = %d \n", actual);
		printf("======= SPANDPS receive dtmf digit '%s'\n", buf);
	} else {
//		printf("======= SPANDPS receive dtmf digit '%s'\n", buf);
	}

	return PJ_SUCCESS;
}

pj_str_t get_num_from_call(char *contact){
	pj_str_t val;
	char *temp;
	temp = trim(contact, find(contact, ":") + 1, find(contact, "@"));
	val = pj_str(temp);
	return val;
	//free(val);
}
int split(char *src, char key, char arr[][100]){
  int i, tmp, count = 0;
  for(i = 0; i < strlen(src); i++){
    if (src[i] == key) {
      count++;
      tmp = i+1;
      continue;
      }
    //printf("hihi\n");
    arr[count][i - tmp] =src[i]; 
  }
  return count + 1;
}
static void on_reg_state(pjsua_acc_id acc_id){
		int *index; 
		pjsua_acc_info info;
		pjsua_acc_get_info(acc_id, &info);
		fprintf(stderr,"msg:account$index:%s$status:%s\n",info.acc_uri.ptr, info.status_text.ptr);
}
static void* handle_msg_proc(char *msg){
		printf("message : %s", msg);
		if (strstr(msg,"reboot")){
			printf("==============gui tin hieu reboot cho stm32\n");
			transfer_2_byte(fd1, 0xff, 0x40);
		}
		else if(strstr(msg, "msg:status")){
			int i;
			char str_msg[100]="msg:status";
			char tmp[10];
			for (i = 0; i < 24; i++){
				sprintf(tmp,"$%d",ar_config[i].state);
				strcat(str_msg, tmp);
			}
			fprintf(stderr, "%s", str_msg);
		}
		else if (strstr(msg, "msg:echo_enable")){
			char array[10][100];
			int length;
			length = split(msg,':',array);
			strcpy(echo_enable, array[2]);
			fprintf(stderr,"==========================echo enable %s\n", echo_enable);
		}
		else if (strstr(msg, "dial_plan")){
			char array[100][100];
			int length;
			int i;
			char dial_plan[10];
			dial_plan[0] = 0xFF;
			dial_plan[1] = 0x16;
			length = split(msg, '$', array);
			dial_plan[2] = ((atoi(array[10]) << 4) | atoi(array[1]));
			dial_plan[3] = ((atoi(array[2]) << 4) | atoi(array[3]));
			dial_plan[4] = ((atoi(array[4]) << 4) | atoi(array[5]));
			dial_plan[5] = ((atoi(array[6]) << 4) | atoi(array[7]));
			dial_plan[6] = ((atoi(array[8]) << 4) | atoi(array[9]));
			usleep(50000);
			transfer_n_byte(fd1, dial_plan);
		}
		else{
			char array[100][100];
			pjsua_acc_config cfg;
			char id[100];
			char reg_uri[100];
			int index;
			int length;
			int i;
			unsigned char trans_flag = 0;
			unsigned char h_status_flag = 0;
			unsigned char phone_flag = 0;
			unsigned char account_change_flag = 0;
			pj_status_t status;
			length = split(msg, '$', array);
			for (i = 0; i < length; i++){
				printf("gia tri tan phan tu msg: %s \n", array[i]);
				if (strstr(array[i], "id")){
					char *tmp = trim(array[i], strlen("id") + 1, strlen(array[i]));
					index = atoi(tmp);
					free(tmp);
				}
				else if (!strstr(array[i], "undefined") && strstr(array[i], "domain")){
					account_change_flag = 1;
					char *tmp = trim(array[i], strlen("domain") + 1, strlen(array[i]));
					strcpy(ar_config[index].domain, tmp);
					free (tmp);
				}
				else if (!strstr(array[i], "undefined") && strstr(array[i], "status")){
					account_change_flag = 1;
					char *tmp = trim(array[i], strlen("status") + 1, strlen(array[i]));
					strcpy(ar_config[index].status, tmp);
					free (tmp);
				}
				else if (!strstr(array[i], "undefined") && strstr(array[i], "h_statu")){
					h_status_flag = 1;
					char *tmp = trim(array[i], strlen("h_statu") + 1, strlen(array[i]));
					printf("h_status_flag : %s", tmp);
					strcpy(ar_config[index].h_status, tmp);
					free (tmp);
				}
				else if (!strstr(array[i], "undefined") && strstr(array[i], "username")){
					phone_flag = 1;
					account_change_flag = 1;
					char *tmp = trim(array[i], strlen("username") + 1, strlen(array[i]) - 1);
					strcpy(ar_config[index].username, tmp);
					free (tmp);
				}
				else if (!strstr(array[i], "undefined") && strstr(array[i], "hotline")){
					h_status_flag = 1;
					char *tmp = trim(array[i], strlen("hotline") + 1, strlen(array[i]));
					strcpy(ar_config[index].hotline, tmp);
					free (tmp);
				}
				else if (!strstr(array[i], "undefined") && strstr(array[i], "secret")){
					account_change_flag = 1;
					char *tmp = trim(array[i], strlen("secret") + 1, strlen(array[i]));
					strcpy(ar_config[index].secret, tmp);
					free (tmp);
				}
				else if (!strstr(array[i], "undefined") && strstr(array[i], "transport")){
					account_change_flag = 1;
					trans_flag = 1;
					char *tmp = trim(array[i], strlen("transport") + 1, strlen(array[i]));
					strcpy(ar_config[index].transport, tmp);
					free (tmp);
				}
				else if (!strstr(array[i], "undefined") && strstr(array[i], "port")){
					account_change_flag = 1;
					trans_flag = 1;
					char *tmp = trim(array[i], strlen("port") + 1, strlen(array[i]));
					ar_config[index].sip_port = atoi(tmp);
					free (tmp);
				}
			}
      if (phone_flag){
				printf("======================phone : %s\n",ar_config[index].username);
				char number_phone[10];
				char username[100];
				for (i = 0; i < strlen(ar_config[index].username); i++){
					if (ar_config[index].username[i] - 48 == 0){
						username[i] = 58;
					}
					else username[i] = ar_config[index].username[i];
				}
				int phone_len = strlen(username);
				int k = 3;
				number_phone[0] = index;
				number_phone[1] = 0x30;
				number_phone[2] = (uint8_t)((phone_len << 4) | (username[0] - 48));
				for (i = 1; i < phone_len; i = i + 2){
							if (i + 1 < phone_len) {
									number_phone[k] = (uint8_t)(((username[i] - 48) << 4) | (username[i+1] - 48));
									printf("%d \n", (uint8_t)(((username[i] - 48) << 4) | (username[i+1] - 48)));}
							else if (i < phone_len) {
									number_phone[k] = (uint8_t)((username[i] - 48) << 4);
									printf("=====gia tri cuoi %d\n", (uint8_t)((username[i] - 48) << 4));
							}
							k++;
				}
				transfer_phone_byte(fd1, number_phone);
			}
			if (h_status_flag){
				printf("============= print %s",ar_config[index].h_status);
				if (strcmp(ar_config[index].h_status, "0") == 0) transfer_2_byte(fd1, index, 0x21);
				else {
					printf("======================phone : %s\n",ar_config[index].hotline);
					char number_hotline[10];
					char hotline[100];
					for (i = 0; i < strlen(ar_config[index].hotline); i++){
						if (ar_config[index].hotline[i] - 48 == 0){
							hotline[i] = 58;
						}
						else hotline[i] = ar_config[index].hotline[i];
					}
					int hotline_len = strlen(hotline);
					int k = 3;
					number_hotline[0] = index;
					number_hotline[1] = 0x20;
					number_hotline[2] = (uint8_t)((hotline_len << 4) | (hotline[0] - 48));
					for (i = 1; i < hotline_len; i = i + 2){
								if (i + 1 < hotline_len) {
										number_hotline[k] = (uint8_t)(((hotline[i] - 48) << 4) | (hotline[i+1] - 48));
										printf("%d \n", (uint8_t)(((hotline[i] - 48) << 4) | (hotline[i+1] - 48)));}
								else if (i < hotline_len) {
										number_hotline[k] = (uint8_t)((hotline[i] - 48) << 4);
										printf("=====gia tri cuoi %d\n", (uint8_t)((hotline[i] - 48) << 4));
								}
								k++;
					}
					transfer_phone_byte(fd1, number_hotline);
				}
			}
			if (account_change_flag && !trans_flag){
				pjsua_acc_config_default(&cfg);
			// disable account
				printf("status : %d \n", strcmp(ar_config[index].status , "0"));
				if ((strcmp(ar_config[index].status , "0") == 0) && ar_config[index].acc_id != -1){
					pjsua_acc_del(ar_config[index].acc_id);
					ar_config[index].acc_id = -1;
					fprintf(stderr,"msg:account$index:%d$status:%s\n",index, "OK");
					return;
				}
				else if ((strcmp(ar_config[index].status , "0") == 0) && ar_config[index].acc_id == -1) {
					fprintf(stderr,"msg:account$index:%d$status:%s\n",index, "OK");
					return;
				}
				sprintf(id,"sip:%s@%s", ar_config[index].username, ar_config[index].domain);
				cfg.id = pj_str(id);

				if (USE_CUCM == 0) cfg.transport_id = cfg_transport_default_id; /* Map account to a config which has difference SIP port */
				else cfg.transport_id = cfg_transport_id[index];

				sprintf(reg_uri, "sip:%s", ar_config[index].domain);
				cfg.reg_uri = pj_str(reg_uri);
				cfg.reg_timeout = 30;
				cfg.cred_count = 1;
				cfg.cred_info[0].realm = pj_str((char *)"*");
				cfg.cred_info[0].scheme = pj_str((char *)"digest");
				cfg.cred_info[0].username = pj_str(ar_config[index].username);
				cfg.cred_info[0].data_type = PJSIP_CRED_DATA_PLAIN_PASSWD;
				cfg.cred_info[0].data = pj_str(ar_config[index].secret);


				// chi xoa khi acc_id ton tai
				if ( ar_config[index].acc_id != -1)
				pjsua_acc_del(ar_config[index].acc_id);
				pjsua_acc_add(&cfg, PJ_TRUE, &ar_config[index].acc_id);
				pjsua_acc_set_user_data(ar_config[index].acc_id, (void *)&index);
				//status = pjsua_acc_modify(ar_config[0].acc_id,&cfg);
				//if (status != PJ_SUCCESS) error_exit("Error adding account 0", status);
				//status = pjsua_acc_set_transport(cfg, cfg_transport_id[i]);
				//if (status != PJ_SUCCESS) error_exit("Error set transpoet ID", status);
				}
				else {
					fprintf(stderr,"msg:account$index:%d$status:%s\n",index, "OK");
				}
		}
}
static void* read_msg_proc(void){
	while(1){
		pj_thread_t *handle_msg;
		int i;
		for (i = 0; i < 24; i++){
			if (web_msg[i].avail == 0){
					memset(&web_msg[i].msg , '\0', sizeof(web_msg[i].msg));
					fgets(web_msg[i].msg, sizeof(web_msg[i].msg), stdin);
					fflush(stdin);
					pj_thread_create(pool, NULL, (pj_thread_proc*)&handle_msg_proc, web_msg[i].msg , PJ_THREAD_DEFAULT_STACK_SIZE, 0, &handle_msg);
					web_msg[i].avail = 1;
					break;
				}
		}
	}
}
static void* thread_proc(struct buf_uart_rx *t_buf){
	int i;
	int j = 1;
	char call_uri[100];
	char rd_buf[32];
	unsigned char flag;
	unsigned char count;
	int num_byte;
	pj_str_t uri;
	pj_str_t call_number;
	memset(&rd_buf , '\0', sizeof(rd_buf));
	for (i = 0; i < 10; i++){
		printf("=====================nhan duoc byte thu %d = %.2X\n", i, t_buf->read_buf[i]);
	}
	count = t_buf->read_buf[0];
	flag = t_buf->read_buf[1];
	if (count == 0xFF && flag == 0x10){
		int m;
		char cpu_status[10] = {
			0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
			0x55, 0x55, 0x55,0x55
		};
		cpu_status[0] = 0xFF;
		cpu_status[1] = 0x10;
    	usleep(20000);
		transfer_n_byte(fd1, cpu_status);
	}
	else if (count == 0xFF && flag == 0xAA){
		int m;
		sleep(3);
  		// printf("===========arconfig = %s",username);
		for (m = 0; m < CHANNEL_NUM; m++){
   		printf("===========arconfig username = %s",ar_config[m].username);
			char number_phone[10];
            char username[100];
            for (i = 0; i < strlen(ar_config[m].username); i++){
                if (ar_config[m].username[i] - 48 == 0){
                    username[i] = 58;
                }
                else username[i] = ar_config[m].username[i];
            }
			int phone_len = strlen(username);
      		printf("===========username = %s",username);
			int k = 3;
			number_phone[0] = m;
			number_phone[1] = 0x30;
			number_phone[2] = (uint8_t)((phone_len << 4) | (username[0] - 48));
			for (i = 1; i < phone_len; i = i + 2){
						if (i + 1 < phone_len) {
								number_phone[k] = (uint8_t)(((username[i] - 48) << 4) | (username[i+1] - 48));
								printf("%d \n", (uint8_t)(((username[i] - 48) << 4) | (username[i+1] - 48)));}
						else if (i < phone_len) {
								number_phone[k] = (uint8_t)((username[i] - 48) << 4);
								printf("=====gia tri cuoi %d\n", (uint8_t)((username[i] - 48) << 4));
						}
						k++;
			}
			usleep(10000);
			transfer_phone_byte(fd1, number_phone);
			if (strcmp(ar_config[m].h_status, "1") == 0){
				char number_hotline[10];
                char hotline[100];
                char username[100];
                for (i = 0; i < strlen(ar_config[m].hotline); i++){
                    if (ar_config[m].hotline[i] - 48 == 0){
                        hotline[i] = 58;
                    }
                    else hotline[i] = ar_config[m].hotline[i];
                }
				int hotline_len = strlen(hotline);
				k = 3;
				number_hotline[0] = m;
				number_hotline[1] = 0x20;
				number_hotline[2] = (uint8_t)((hotline_len << 4) | (hotline[0] - 48));
				for (i = 1; i < phone_len; i = i + 2){
							if (i + 1 < phone_len) {
									number_hotline[k] = (uint8_t)(((hotline[i] - 48) << 4) | (hotline[i+1] - 48));
									printf("%d \n", (uint8_t)(((hotline[i] - 48) << 4) | (hotline[i+1] - 48)));}
							else if (i < phone_len) {
									number_hotline[k] = (uint8_t)((hotline[i] - 48) << 4);
									printf("=====gia tri cuoi %d\n", (uint8_t)((hotline[i] - 48) << 4));
							}
							k++;
				}
				usleep(10000);
				transfer_phone_byte(fd1, number_hotline);
			}
			else{
				transfer_2_byte(fd1, m, 0x21);
			}
		}
		char dial_plan[10];
		dial_plan[0] = 0xFF;
		dial_plan[1] = 0x16;
		dial_plan[2] = ((local_dial.digit0 << 4) | local_dial.digit1);
		dial_plan[3] = ((local_dial.digit2 << 4) | local_dial.digit3);
		dial_plan[4] = ((local_dial.digit4 << 4) | local_dial.digit5);
		dial_plan[5] = ((local_dial.digit6 << 4) | local_dial.digit7);
		dial_plan[6] = ((local_dial.digit8 << 4) | local_dial.digit9);
		usleep(10000);
		transfer_n_byte(fd1, dial_plan);
		if (ready_flag == 5) {
		//	usleep(10000);
			transfer_9_byte(fd1);
      ready_flag = 0;
		}
		ready_flag++;
	}
	else if (count == 0xFF && flag == 0x41){
		fprintf(stderr,"msg:reset_network\n");
	}
	else if (count == 0xFF && flag == 0x11){
		fprintf(stderr,"msg:arm_ready\n");
	}
	else if (count == 0xFF && flag == 0x12){
		fprintf(stderr,"msg:fpga_ready\n");
	}
	else if (count == 0xFF && flag == 0x13){
		fprintf(stderr,"msg:3.3v_ok\n");
	}
	else if (count == 0xFF && flag == 0x14){
		fprintf(stderr,"msg:5v_ok\n");
	}
	else if (count == 0xFF && flag == 0x15){
		fprintf(stderr,"msg:sensor$temp:%d%d.%d$hum:%d%d.%d\n",t_buf->read_buf[2], t_buf->read_buf[3], t_buf->read_buf[4], t_buf->read_buf[5], t_buf->read_buf[6], t_buf->read_buf[7]);
	}
	else if (flag == 0x42){
		fprintf(stderr,"msg:fxs$index:%d$status:%d$value:%d%d.%d\n", t_buf->read_buf[0], t_buf->read_buf[2], t_buf->read_buf[3], t_buf->read_buf[4], t_buf->read_buf[5]);
	}

	
	if (flag == 0x01){
    	printf("if flag = 1\n");
		num_byte =t_buf->read_buf[2] >> 4;
		rd_buf[0] =t_buf->read_buf[2] & 0x0F;
		printf("debug %.2X , %.2X\n", rd_buf[0] , t_buf->read_buf[2]);
		for(i = 0; i < num_byte / 2; i++){
            rd_buf[j] =t_buf->read_buf[i+3] >> 4;
			      j++;
			if (j < num_byte){
				rd_buf[j] =t_buf->read_buf[i+3] & 0x0F;
				j++;
			}
        }
    	printf("so dien thoai nhan duoc tu stm32\n");
		for(i = 0; i< num_byte; i++){
            if (rd_buf[i] ==10) rd_buf[i] = 0;      //digit 0 in analog phone = 10
            printf("%d %d",rd_buf[i], i);
            rd_buf[i] += 48;
    	}
		call_number = pj_str(rd_buf);
		sprintf(call_uri,"sip:%s@%s", call_number.ptr, ar_config[0].domain);
		uri = pj_str(call_uri);
		printf("chuoi %s va len %ld\n", uri.ptr, uri.slen);
		printf("flag = %d, count = %d \n", flag, count);
		for (i = 0; i < CHANNEL_NUM; i++){
			printf("kiem tra lan thu %d \n", i);
			printf("ar_config[i].call_number.ptr = %s call_number.ptr = %s", ar_config[i].call_number.ptr, "abc");
			if (strcmp(ar_config[i].call_number.ptr, call_number.ptr) == 0){
				printf("============ busy call\n");
				pj_mutex_lock (mutex);
				transfer_2_byte(fd1, count, 0x06);
				pj_mutex_unlock(mutex);
				goto quit;
				break;
			}
		}
		pjsua_call_make_call(ar_config[count].acc_id, &uri, 0, NULL, NULL, NULL);
	quit :
		printf(" ");
	}
	else if (flag == 0x04) pjsua_call_answer(ar_config[count].call_id, 200, NULL, NULL);
	else if (flag == 0x07) {
			pj_mutex_lock(mutex);
			fprintf(stderr,"msg:call_state$%d$%d",count, 0);
			ar_config[count].state = 0;
			ar_config[count].call_number = pj_str("idle");
			pj_mutex_unlock(mutex);
			pjsua_call_hangup(ar_config[count].call_id, 0, NULL, NULL);
			printf("idle state\n"); 
	}
	t_buf->avail = 0;
	return NULL;
}
/* Callback called by the library upon receiving incoming call */
static void on_incoming_call(pjsua_acc_id acc_id, pjsua_call_id call_id,
		pjsip_rx_data *rdata)
{
	pj_str_t remote_acc, local_acc;
	pjsua_call_info ci;
	int i;
	unsigned char j = 3;
	unsigned char tx_buf[10];
	PJ_UNUSED_ARG(acc_id);
	PJ_UNUSED_ARG(rdata);

	pjsua_call_get_info(call_id, &ci);
	remote_acc = get_num_from_call(ci.remote_info.ptr);
	local_acc = get_num_from_call(ci.local_contact.ptr);
	for (i = 0; i < CHANNEL_NUM; i++){
		if (strcmp(local_acc.ptr, ar_config[i].username) == 0) {
			if (ar_config[i].state > 0) goto busy;
			ar_config[i].call_id = call_id;
			ar_config[i].call_number = remote_acc;
			printf("acount %d , call_number = %s, remote_acc len %ld", i, remote_acc.ptr,remote_acc.slen);
			printf("call id %d = %d \n", i, ar_config[i].call_id); 
			ar_config[i].state = 0x02;
			tx_buf[0] = (uint8_t)(i);
			tx_buf[1] = 0x02;
			break;
		}
	}
	for (i = 0; i < remote_acc.slen; i++){
		if (remote_acc.ptr[i] - 48 == 0){
		remote_acc.ptr[i] = 58;
		}
  	}
	  tx_buf[2] = (uint8_t)((remote_acc.slen << 4) | (remote_acc.ptr[0] - 48));
          for (i = 1; i < remote_acc.slen; i = i + 2){
                  if (i + 1 < remote_acc.slen) {
                          tx_buf[j] = (uint8_t)(((remote_acc.ptr[i] - 48) << 4) | (remote_acc.ptr[i+1] - 48));
                          printf("%d \n", (uint8_t)(((remote_acc.ptr[i] - 48) << 4) | (remote_acc.ptr[i+1] - 48)));}
                  else if (i < remote_acc.slen) {
					  	  tx_buf[j] = (uint8_t)((remote_acc.ptr[i] - 48) << 4);
                          printf("=====gia tri cuoi %d\n", (uint8_t)((remote_acc.ptr[i] - 48) << 4));
                  }
				  j++;
          }
	pj_mutex_lock (mutex);
	fprintf(stderr,"msg:call_state$%d$%d",tx_buf[0], tx_buf[1]);
	transfer_phone_byte(fd1, tx_buf);
  	pj_mutex_unlock (mutex);

	PJ_LOG(3,(THIS_FILE, "Incoming call from %.*s!!",
				(int)ci.remote_info.slen,
				ci.remote_info.ptr));
	/* send ringing 180*/
	free(remote_acc.ptr);
	free(local_acc.ptr);
	pjsua_call_answer(call_id, PJSIP_SC_RINGING , NULL, NULL);
	return;
  	busy :
        printf("go to busy\n");
		free(remote_acc.ptr);
		free(local_acc.ptr);
        pjsua_call_hangup(call_id, 0, NULL, NULL);
}

/* Callback called by the library when call's state has changed */
static void on_call_state(pjsua_call_id call_id, pjsip_event *e)
{
	pjsua_call_info ci;
	pjsua_call_get_info(call_id, &ci);
  	pj_str_t local_acc, remote_acc;
  	int i;
	//printf("============%s\n", ci.local_contact.ptr);
	// printf("============remote acc %s\n", ci.remote_info.ptr);
	local_acc = get_num_from_call(ci.local_contact.ptr);
  	remote_acc = get_num_from_call(ci.remote_info.ptr);
	PJ_UNUSED_ARG(e);

	pjsua_call_get_info(call_id, &ci);
	PJ_LOG(3,(THIS_FILE, "Call %d state=%.*s", call_id,
				    (int)ci.state_text.slen,
				    ci.state_text.ptr));
	for (i = 0; i < CHANNEL_NUM; i++){
		if (strcmp(local_acc.ptr, ar_config[i].username) == 0) {
		if ((ar_config[i].state == 0x02) && (ci.state == 0x06) || (ci.state == 0x04)) goto out;
		if ((ar_config[i].state != 0) || (ci.state != 0x06)){
			ar_config[i].call_number = remote_acc;
			printf("acount %d , call_number = %s", i, remote_acc.ptr);
			printf("khong phai disconnect %d\n", ar_config[i].state);
			ar_config[i].state = ci.state;
			ar_config[i].call_id = call_id;
			pj_mutex_lock (mutex);
			fprintf(stderr,"msg:call_state$%d$%d",i, ci.state);
			transfer_2_byte(fd1, i , ci.state);
			pj_mutex_unlock (mutex);
			break;
		}
		}
	}
  	out :
	  	free(remote_acc.ptr);
		free(local_acc.ptr);
        printf(" ");
 }
/* Callback called by the library when call's media state has changed */
static void on_call_media_state(pjsua_call_id call_id)
{
	pjsua_call_info ci;
	pj_str_t contact_number;
	int i;

	pjsua_call_get_info(call_id, &ci);
	contact_number = get_num_from_call(ci.local_contact.ptr);

	if (ci.media_status == PJSUA_CALL_MEDIA_ACTIVE) {
		for (i = 0 ; i < CHANNEL_NUM; i++){
			if ((strcmp(contact_number.ptr, ar_config[i].username) == 0)){
				printf("on media state i = %d\n",i);
				if (i % 2 == 0){
					pjmedia_conf_connect_port(ar_config[i].cd->conf, ar_config[i].cd->call_slot_1, ar_config[i].slot, 0);
					pjmedia_conf_connect_port(ar_config[i].cd->conf, ar_config[i].slot , ar_config[i].cd->call_slot_1, 0);
				}
				else {
					pjmedia_conf_connect_port(ar_config[i].cd->conf, ar_config[i].cd->call_slot_0, 0, 0);
					pjmedia_conf_connect_port(ar_config[i].cd->conf, 0, ar_config[i].cd->call_slot_0, 0);
				}
			}
		}								
	}
	free(contact_number.ptr);
}
static void on_stream_created(pjsua_call_id call_id, 
                              pjmedia_stream *sess,
                              unsigned stream_idx, 
                              pjmedia_port **p_port)
{
	pjsua_call_info ci;
	pj_str_t contact_number;
	int i;
	pjmedia_port *ec_port;
	pjsua_call_get_info(call_id, &ci);
	printf("=========================================== call_id = %d \n", call_id);
	contact_number = get_num_from_call(ci.local_contact.ptr);
	for (i = 0; i < CHANNEL_NUM; i++){
		if ((strcmp(contact_number.ptr, ar_config[i].username) == 0)){
			int j = i / 2;
			j = j * 2;
			printf("account %d sound %d", i, j);
			if (!ar_config[j].sound_inited){
				printf("init sound %s\n", ar_config[j].sound_name);
				status = pjmedia_aud_dev_lookup("ALSA", ar_config[j].sound_name, &ar_config[j].sound_id);
				if (status != PJ_SUCCESS) {
					PJ_LOG(3, (__FILE__, "get sound device %d failed", j));
				}
				status = pjmedia_snd_port_create(ar_config[j].cd->pool, ar_config[j].sound_id, ar_config[j].sound_id,
						PJMEDIA_PIA_SRATE(&conf->info),
						2 /* stereo */,
						2 * PJMEDIA_PIA_SPF(&conf->info),
						PJMEDIA_PIA_BITS(&conf->info),
						0, &ar_config[j].snd_port);

				pjmedia_snd_port_set_ec(ar_config[j].snd_port, pool, 100, PJMEDIA_ECHO_SIMPLE | PJMEDIA_ECHO_USE_SW_ECHO | PJMEDIA_ECHO_AGGRESSIVENESS_AGGRESSIVE);
				if (status == PJ_SUCCESS) printf ("=== add sound %d success", j);
				else {
					printf("=== add sound %d failed", j);
					continue;
				}
				pjmedia_snd_port_connect(ar_config[j].snd_port, ar_config[j].sc);
				printf("ar_config[0].slot = %d\n", ar_config[j].slot);
				}
				ar_config[j].sound_inited = 1;
				ar_config[i].sound_is_using = 1;
			if (strcmp(echo_enable, "1") == 0){
				pjmedia_echo_port_create(ar_config[i].cd->pool, *p_port, 50, 0, PJMEDIA_ECHO_SIMPLE | PJMEDIA_ECHO_USE_SW_ECHO | PJMEDIA_ECHO_AGGRESSIVENESS_AGGRESSIVE,&ec_port);
				//printf("===========on stream create %s\n",contact_number.ptr);
				if (i % 2 == 0) pjmedia_conf_add_port(ar_config[i].cd->conf, ar_config[i].cd->pool, ec_port, NULL, &ar_config[i].cd->call_slot_1);
				else pjmedia_conf_add_port(ar_config[i].cd->conf, ar_config[i].cd->pool, ec_port, NULL, &ar_config[i].cd->call_slot_0);
				break;
			}
			else {
					//printf("===========on stream create %s\n",contact_number.ptr);
					if (i % 2 == 0) pjmedia_conf_add_port(ar_config[i].cd->conf, ar_config[i].cd->pool, *p_port, NULL, &ar_config[i].cd->call_slot_1);
					else pjmedia_conf_add_port(ar_config[i].cd->conf, ar_config[i].cd->pool, *p_port, NULL, &ar_config[i].cd->call_slot_0);
					break;
				}
			}
	}
}
static void on_stream_destroyed(pjsua_call_id call_id, 
                              pjmedia_stream *sess,
                              unsigned stream_idx)
{
	pjsua_call_info ci;
	pj_str_t contact_number;
	int i;

	pjsua_call_get_info(call_id, &ci);
	contact_number = get_num_from_call(ci.local_contact.ptr);
	for (i = 0; i < CHANNEL_NUM; i++){
		if ((strcmp(contact_number.ptr, ar_config[i].username) == 0)){
			int j = i / 2;
			j = j * 2;
			if (i % 2 == 0) {
				pjmedia_conf_remove_port(ar_config[i].cd->conf, ar_config[i].cd->call_slot_1);
				if (!ar_config[i + 1].sound_is_using) {
					printf("destroy sound %s\n", ar_config[j].sound_name);
					status = pjmedia_snd_port_disconnect(ar_config[j].snd_port);
					status = pjmedia_snd_port_destroy(ar_config[j].snd_port);
					if (status == PJ_SUCCESS) {
						printf("destroy sound success %s\n", ar_config[j].sound_name);
					}
					ar_config[j].sound_inited = 0;
				}
			}
			else {
				pjmedia_conf_remove_port(ar_config[i].cd->conf, ar_config[i].cd->call_slot_0);
				if (!ar_config[i - 1].sound_is_using) {
					printf("destroy sound %s\n", ar_config[j].sound_name);
					pjmedia_snd_port_disconnect(ar_config[j].snd_port);
					status = pjmedia_snd_port_destroy(ar_config[j].snd_port);
					if (status == PJ_SUCCESS) {
						printf("destroy sound success %s\n", ar_config[j].sound_name);
					}
					ar_config[j].sound_inited = 0;
				}
			}
			ar_config[i].sound_is_using = 0;
			break;
		}
	}
}

// static void on_dtmf_digit(pjsua_call_id call_id,  int digit)
static void on_dtmf_digit2(pjsua_call_id call_id,  const pjsua_dtmf_info *info)
{
	pjsua_call_info ci;
	pj_str_t contact_number;
	int i, j;
	printf("=============== recieved digit = %d \n", info->digit);
	printf("=============== dtmf method  = %d \n", info->method);
//	printf("=============== recieved digit = %d \n", digit);

	pjsua_call_get_info(call_id, &ci);
	contact_number = get_num_from_call(ci.local_contact.ptr);

	pjmedia_tone_digit d[1];
	d[0].digit = '0';
	d[0].on_msec = 200;
	d[0].off_msec = 200;

	switch (info->digit) {
		case 48:
			d[0].digit = '0';
			break;
		case 49:
			d[0].digit = '1';
			break;
		case 50:
			d[0].digit = '2';
			break;
		case 51:
			d[0].digit = '3';
			break;
		case 52:
			d[0].digit = '4';
			break;
		case 53:
			d[0].digit = '5';
			break;
		case 54:
			d[0].digit = '6';
			break;
		case 55:
			d[0].digit = '7';
			break;
		case 56:
			d[0].digit = '8';
			break;
		case 57:
			d[0].digit = '9';
			break;
		case 42:
			d[0].digit = '*';
			break;
		case 35:
			d[0].digit = '#';
			break;
		default:
			break;
	}

	for (i = 0; i < CHANNEL_NUM; i++) {
		if ((strcmp(contact_number.ptr, ar_config[i].username) == 0)){
			if (i % 2 == 0){
				pjmedia_conf_connect_port(ar_config[i].cd->conf, ar_config[i].cd->toneslotleft , ar_config[i].slot, 0);
				pjmedia_tonegen_play_digits(ar_config[i].cd->tonegenleft, 1, d, 0);
			}
			else {
				pjmedia_conf_connect_port(ar_config[i].cd->conf, ar_config[i].cd->toneslotright, 0, 0);
				pjmedia_tonegen_play_digits(ar_config[i].cd->tonegenright, 1, d, 0);
			}
		}
	}
	free(contact_number.ptr);
}



/* Display error and exit application */
static void error_exit(const char *title, pj_status_t status)
{
	pjsua_perror(THIS_FILE, title, status);
	pjsua_destroy();
	exit(1);
}
int main(int argc, char *argv[])
{
	pj_status_t status;
	int i;
	char errmsg[PJ_ERR_MSG_SIZE];
  	pjsua_codec_info codec_inf[100];
 	int count = 100;

	read_config(ar_config, ar_codec_setting, app_setting, &local_dial, "/etc/pjsip/config");
/*
	use_dialtone_filter = FALSE;
	max_forward_twist = -1;
	max_reverse_twist = -1;

	dtmf_state = dtmf_rx_init(NULL, NULL, NULL);
	if (use_dialtone_filter  ||  max_forward_twist >= 0  ||  max_reverse_twist >= 0) 
		dtmf_rx_parms(dtmf_state, use_dialtone_filter, max_forward_twist, max_reverse_twist, -99);

*/

	/* init PJLIB */
	status = pj_init();
	PJ_ASSERT_RETURN(status == PJ_SUCCESS, 1);

	/* Create pjsua first! */
	status = pjsua_create();
	/* init gpio and spi */
	if (status != PJ_SUCCESS) error_exit("Error in pjsua_create()", status);

	/* Init pjsua */
	{
		pjsua_config cfg;
		pjsua_logging_config log_cfg;

		pjsua_call_send_dtmf_param dtmf;
		pjsua_call_send_dtmf_param_default (&dtmf);
		dtmf.method = PJSUA_DTMF_METHOD_RFC2833;
//		dtmf.method = PJSUA_DTMF_METHOD_SIP_INFO;
		printf("================ dtmf method = %d\n", dtmf.method);
		printf("================ dtmf duration = %d\n", dtmf.duration);

		pjsua_media_config   media_cfg;
		pjsua_media_config_default(&media_cfg);
		media_cfg.ec_options = PJMEDIA_ECHO_USE_SW_ECHO;
		media_cfg.clock_rate = 8000;
		media_cfg.channel_count = 1;

		pjsua_config_default(&cfg);
		cfg.max_calls = 32;
		cfg.cb.on_incoming_call = &on_incoming_call;
		cfg.cb.on_call_media_state = &on_call_media_state;
		cfg.cb.on_call_state = &on_call_state;
		cfg.cb.on_stream_created= &on_stream_created;
		cfg.cb.on_stream_destroyed= &on_stream_destroyed;
		cfg.cb.on_reg_state = &on_reg_state;

		cfg.cb.on_dtmf_digit2 = &on_dtmf_digit2;

		//pjsua_dtmf_info dtmf_info;
		//dtmf_info.method = PJSUA_DTMF_METHOD_RFC2833;

		cfg.use_srtp = app_setting[0].use_srtp;
		cfg.srtp_secure_signaling = 0;


//                cfg.use_srtp = 0;
//                cfg.srtp_secure_signaling = 0;

		printf("==================== app user_srtp = %d \n", app_setting[0].use_srtp);
		printf("==================== user_srtp = %d \n", cfg.use_srtp);

		pjsua_logging_config_default(&log_cfg);
		log_cfg.console_level = 1;

		status = pjsua_init(&cfg, &log_cfg, &media_cfg);
		if (status != PJ_SUCCESS) error_exit("Error in pjsua_init()", status);
	}

	// ===================================================================== set codec
/*
    for (int i=0; i< NUM_CODEC; i++){
        printf("========= codec_name = %s codec_pri = %d \n", ar_codec_setting[i].codec_name, ar_codec_setting[i].codec_pri);
    }
*/
	status = pjsua_enum_codecs(codec_inf, &count);
    for (int i = 0; i < NUM_CODEC; i++) {
        for (int j = 0; j < NUM_CODEC; j++) {
	        if (pj_strcmp2(&codec_inf[i].codec_id, ar_codec_setting[j].codec_name) == 0) {
	        	codec_inf[i].priority = ar_codec_setting[j].codec_pri;
			pjsua_codec_set_priority(&codec_inf[i].codec_id, ar_codec_setting[j].codec_pri);
	        }
    	}
    }
    	status = pjsua_enum_codecs(codec_inf, &count);
/*	
    for (int i = 0; i < NUM_CODEC; i++) {
        printf ("codec %d ", i);
        printf("  codec_id = %s codec_pri %d \n", codec_inf[i].codec_id.ptr, codec_inf[i].priority);
    }    */
    // ====================================================================
	/* Add UDP transport. */
	pjsua_transport_config_default(&cfg_default_transport);
	cfg_default_transport.port = ar_config[0].sip_port;
	if (strcmp(ar_config[0].transport, "UDP") == 0)
		status = pjsua_transport_create(PJSIP_TRANSPORT_UDP, &cfg_default_transport, &cfg_transport_default_id);
	if (strcmp(ar_config[0].transport, "TCP") == 0)
		status = pjsua_transport_create(PJSIP_TRANSPORT_TCP, &cfg_default_transport, &cfg_transport_default_id);

	/* Initialization is done, now start pjsua */
	status = pjsua_start();

	pool = pjsua_pool_create("pool", 2048, 2048);
	
	status = pj_mutex_create_simple (pool, "my_lock", &mutex);
	if (status != PJ_SUCCESS) error_exit("Error creat mutex", status);
	conf = pjsua_set_no_snd_dev();

	for (i = 0; i < CHANNEL_NUM; i += 2) {
		pj_pool_t *pool1;
		printf("========init sound\n");
		pool1 = pjsua_pool_create(NULL, 1000, 1000);
		ar_config[i].cd = PJ_POOL_ZALLOC_T(pool1, struct call_data);
		ar_config[i + 1].cd = ar_config[i].cd;
		ar_config[i].cd->pool = pool1;

		status = pjmedia_conf_create(pool1,
						32,
						PJMEDIA_PIA_SRATE(&conf->info),
						1 /* stereo */,
						PJMEDIA_PIA_SPF(&conf->info),
						PJMEDIA_PIA_BITS(&conf->info),
						PJMEDIA_CONF_NO_DEVICE, &ar_config[i].cd->conf);
		ar_config[i].cd->cport = pjmedia_conf_get_master_port(ar_config[i].cd->conf);
		/* Create stereo-mono splitter/combiner */
		status = pjmedia_splitcomb_create(ar_config[i].cd->pool, 
				PJMEDIA_PIA_SRATE(&conf->info),
				2 /* stereo */,
				2 * PJMEDIA_PIA_SPF(&conf->info),
				PJMEDIA_PIA_BITS(&conf->info),
				0, &ar_config[i].sc);
		
		/* Connect channel0 (left channel?) to conference port slot0 */
		status = pjmedia_splitcomb_set_channel(ar_config[i].sc, 1, 0, ar_config[i].cd->cport);
		
		/* Create reverse channel for channel1 (right channel?)... */
		status = pjmedia_splitcomb_create_rev_channel(ar_config[i].cd->pool, ar_config[i].sc, 0, 0, &ar_config[i].rev);
		if (i == 0) {
				status = pjmedia_aud_dev_lookup("ALSA", ar_config[i].sound_name, &ar_config[i].sound_id);
				if (status != PJ_SUCCESS) {
					PJ_LOG(3, (__FILE__, "get sound device %d failed", i));
				}
				status = pjmedia_snd_port_create(ar_config[i].cd->pool, ar_config[i].sound_id, ar_config[i].sound_id,
						PJMEDIA_PIA_SRATE(&conf->info),
						2 /* stereo */,
						2 * PJMEDIA_PIA_SPF(&conf->info),
						PJMEDIA_PIA_BITS(&conf->info),
						0, &ar_config[i].snd_port);
				status = pjmedia_snd_port_destroy(ar_config[i].snd_port);
				
		}

		pjmedia_conf_add_port(ar_config[i].cd->conf, ar_config[i].cd->pool, ar_config[i].rev, NULL, &ar_config[i].slot);
		printf("ar_config[0].slot = %d\n", ar_config[i].slot);

		/* Create port and add port for playing tone when receive dtmf digit */
		status = pjmedia_tonegen_create(ar_config[i].cd->pool, 8000, 1, 160, 16, 0, &ar_config[i].cd->tonegenleft);
		status = pjmedia_conf_add_port(ar_config[i].cd->conf, ar_config[i].cd->pool, ar_config[i].cd->tonegenleft, NULL, &ar_config[i].cd->toneslotleft);
		status = pjmedia_tonegen_create(ar_config[i].cd->pool, 8000, 1, 160, 16, 0, &ar_config[i].cd->tonegenright);
		status = pjmedia_conf_add_port(ar_config[i].cd->conf, ar_config[i].cd->pool, ar_config[i].cd->tonegenright, NULL, &ar_config[i].cd->toneslotright);

		/* Tao port de capture memory*/
		status = pjmedia_mem_capture_create(ar_config[i].cd->pool, left_amp[i], SAMPLES_PER_CHUNK,
				PJMEDIA_PIA_SRATE(&conf->info), 
				1,
				2 * PJMEDIA_PIA_SPF(&conf->info),
				PJMEDIA_PIA_BITS(&conf->info), 0, &ar_config[i].medpcap);
		status = pjmedia_conf_add_port(ar_config[i].cd->conf, ar_config[i].cd->pool,
				ar_config[i].medpcap, NULL, &ar_config[i].slot_cap);
		status = pjmedia_conf_connect_port(ar_config[i].cd->conf, ar_config[i].slot, ar_config[i].slot_cap, 0);
		status = pjmedia_mem_capture_set_eof_cb(ar_config[i].medpcap, NULL, left_dtmf_decode);

		// Add SPANDPS for 2 channels on 1 snd
		
		status = pjmedia_mem_capture_create(ar_config[i].cd->pool, right_amp[i], SAMPLES_PER_CHUNK,
				PJMEDIA_PIA_SRATE(&conf->info), 
				1,
				2 * PJMEDIA_PIA_SPF(&conf->info),
				PJMEDIA_PIA_BITS(&conf->info), 0, &ar_config[i].medpcap2);

		status = pjmedia_conf_add_port(ar_config[i].cd->conf, ar_config[i].cd->pool,
				ar_config[i].medpcap2, NULL, &ar_config[i].slot_cap2);

		status = pjmedia_conf_connect_port(ar_config[i].cd->conf, 0, ar_config[i].slot_cap2, 0);

		status = pjmedia_mem_capture_set_eof_cb(ar_config[i].medpcap2, NULL, right_dtmf_decode);                 

	}

	for(i = 0; i < CHANNEL_NUM; i++){
		if (strcmp(ar_config[i].status,"1") == 0){
			pjsua_acc_config cfg;
			pjsua_acc_config_default(&cfg);
			char id[100];
			char reg_uri[100];
			sprintf(id,"sip:%s@%s", ar_config[i].username, ar_config[i].domain);
			cfg.id = pj_str(id);

			if (USE_CUCM == 0) cfg.transport_id = cfg_transport_default_id; /* Map account to a config which has difference SIP port */
			else {
				pjsua_transport_config_default(&cfg_transport[i]);
				cfg_transport[i].port = ar_config[i].sip_port;
				if (strcmp(ar_config[i].transport, "UDP") == 0)
					status = pjsua_transport_create(PJSIP_TRANSPORT_UDP, &cfg_transport[i], &cfg_transport_id[i]);
				if (strcmp(ar_config[i].transport, "TCP") == 0)
					status = pjsua_transport_create(PJSIP_TRANSPORT_TCP, &cfg_transport[i], &cfg_transport_id[i]);
				if (strcmp(ar_config[i].transport, "TLS") == 0) {
					pjsip_tls_setting tls;
					pjsip_tls_setting_default (&tls);
					char cert_file[100];
					char ca_list_file[100];
					char ca_list_path[100];
					sprintf(cert_file,"%s%s.pem",TLS_KEY_PATH, ar_config[i].username);
					sprintf(ca_list_file,"%s%s",TLS_KEY_PATH, "ca.crt");
					sprintf(ca_list_path,"%s",TLS_KEY_PATH);
					printf("TLS ===================== cert %s, ca : %s, path : %s, user : %s \n",cert_file,ca_list_file,ca_list_path, ar_config[i].username);
					tls.cert_file = pj_str(cert_file);
					tls.ca_list_file = pj_str(ca_list_file);
					tls.ca_list_path = pj_str(ca_list_path);
					/* if (i == 2){ 
						char *cert_file= "/etc/asterisk/keys/958277.pem";
						char *ca_list_file = "/etc/asterisk/keys/ca.crt";
						char *ca_list_path = "/etc/asterisk/keys/";
						tls.cert_file = pj_str(cert_file);
						tls.ca_list_file = pj_str(ca_list_file);
						tls.ca_list_path = pj_str(ca_list_path);
					}
					else if(i == 3) {
						char *cert_file= "/etc/asterisk/keys/958278.pem";
						char *ca_list_file = "/etc/asterisk/keys/ca.crt";
						char *ca_list_path = "/etc/asterisk/keys/";
						tls.cert_file = pj_str(cert_file);
						tls.ca_list_file = pj_str(ca_list_file);
						tls.ca_list_path = pj_str(ca_list_path);

					} */
					cfg_transport[i].tls_setting = tls;
					status = pjsua_transport_create(PJSIP_TRANSPORT_TLS, &cfg_transport[i], &cfg_transport_id[i]);
				}
				cfg.transport_id = cfg_transport_id[i];

			}

			sprintf(reg_uri, "sip:%s", ar_config[i].domain);
			cfg.reg_timeout = 30;
			cfg.reg_uri = pj_str(reg_uri);
			cfg.cred_count = 1;
			cfg.cred_info[0].realm = pj_str((char *)"*");
			cfg.cred_info[0].scheme = pj_str((char *)"digest");
			cfg.cred_info[0].username = pj_str(ar_config[i].username);
			cfg.cred_info[0].data_type = PJSIP_CRED_DATA_PLAIN_PASSWD;
			cfg.cred_info[0].data = pj_str(ar_config[i].secret);
			printf ("id : %s, reg_uri : %s, secret : %s \n",cfg.id.ptr,cfg.reg_uri.ptr,cfg.cred_info[0].data.ptr);
			status = pjsua_acc_add(&cfg, PJ_TRUE, &ar_config[i].acc_id);
			if (status != PJ_SUCCESS) error_exit("Error adding account 0", status);
		}
	}
	fd1 = init_uart();
	pj_thread_t *read_msg;
	pj_thread_create(pool, "read_msg", (pj_thread_proc*)&read_msg_proc, NULL , PJ_THREAD_DEFAULT_STACK_SIZE, 0, &read_msg);
	struct buf_uart_rx uart_rx[32];
	while (1)
	{
		pj_thread_t *thread;
		int i;
		for (i = 0; i < 32; i++){
			if (uart_rx[i].avail == 0){
        		printf("buf %d avail\n",i);
				memset(&uart_rx[i].read_buf , '\0', sizeof(uart_rx[i].read_buf));
				int num_bytes = read(fd1, &uart_rx[i].read_buf, sizeof(uart_rx[i].read_buf));
				lseek(fd1, 0, SEEK_SET);
				if (num_bytes < 0) {
            		printf("Error reading: %s", strerror(errno));
            		return -1;
        		}
				else pj_thread_create(pool, "thread", (pj_thread_proc*)&thread_proc, &uart_rx[i] , PJ_THREAD_DEFAULT_STACK_SIZE, 0, &thread);
				uart_rx[i].avail = 1;
				break;
			} 
		}
	}

  pjsua_call_hangup_all();
}
