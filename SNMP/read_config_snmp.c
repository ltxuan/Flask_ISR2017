#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "read_config_snmp.h"

void del_line(char *src)
{
	if (src[strlen(src)-1] == '\n') src[strlen(src)-1] ='\0';
}

int find(char *src, char *key)
{
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

char *trim(char *src, int start, int end)
{
	char *val;
	val = malloc(end-start+1);
	int i;
	for (i = start; i < end; i++){
		val[i-start] = (char)src[i];
	}
	return val;
}

char *get_cur_time() 
{
    time_t cur_time;
    char *cur_time_str;
    cur_time = time(NULL);
    cur_time_str = ctime(&cur_time);
    cur_time_str[strlen(cur_time_str)-1] = '\0';
    return cur_time_str;
}

void read_config_snmp(struct config_inventory *ar_config_inventory,
                      struct config_port_eth *ar_config_eth,
                      struct account_obj_id *ar_account_obj_id,
                      struct performance *ar_performance,
                      struct general_config *ar_general_config,
                      char *dir)
{
	int num;
	FILE *fptr;
	int line_size;
    int port_idx = -1;
	char str_line[300];
	int index = 0;
	fptr = fopen(dir,"r");

	if(fptr == NULL)
	{
		printf("Error: Can not open config file to read! \n");   
		exit(1);             
	}
  
	while(fgets(str_line, 300, fptr) != NULL) {
		index = find(str_line, "=");
        if (find(str_line, "ethernet port") != -1) port_idx++;
		if (index != -1){
			char *key = trim(str_line,0,index);
			char *value = trim(str_line, index + 1, strlen(str_line)-1);

            /* Read config inventory v7isrInventory and v7isrVersion */
            {
                if (strcmp(key, "neSerialNum") == 0){
                    strcpy(ar_config_inventory[0].ne_serial_num, value);
                }
                if (strcmp(key, "neSwVersion") == 0){
                    strcpy(ar_config_inventory[0].ne_sw_ver, value);
                }
                if (strcmp(key, "cpuCardName") == 0){
                    strcpy(ar_config_inventory[0].cpu_car_name, value);
                }
                if (strcmp(key, "cpuHwVersion") == 0){
                    strcpy(ar_config_inventory[0].cpu_hw_ver, value);
                }
                if (strcmp(key, "cpuSwVersion") == 0){
                    strcpy(ar_config_inventory[0].cpu_sw_ver, value);
                }
                if (strcmp(key, "ataCardName") == 0){
                    strcpy(ar_config_inventory[0].ata_card_name, value);
                }
                if (strcmp(key, "ataHwVersion") == 0){
                    strcpy(ar_config_inventory[0].ata_hw_ver, value);
                }
                if (strcmp(key, "ataSwVersion") == 0){
                    strcpy(ar_config_inventory[0].ata_sw_ver, value);
                }
                if (strcmp(key, "description") == 0){
                    strcpy(ar_config_inventory[0].descip_inventory, value);
                }
                if (strcmp(key, "description") == 0){
                    strcpy(ar_config_inventory[0].descip_inventory, value);
                }
                if (strcmp(key, "isr_version") == 0){
                    strcpy(ar_config_inventory[0].isr_version, value);
                }
            }         

            /* Read account object id for trap */
            {
                if (strcmp(key, "sip_object_id") == 0){
                    strcpy(ar_account_obj_id[0].sip_object_id, value);
                }
                if (strcmp(key, "asterisk_object_id") == 0){
                    strcpy(ar_account_obj_id[0].asterisk_object_id, value);
                }
                if (strcmp(key, "quadga_object_id") == 0){
                    strcpy(ar_account_obj_id[0].quadga_object_id, value);
                }
            }

            /* Read ether port config for isrConfigPortGesTable */
            {
                if (strcmp(key, "ip_add") == 0){
                    strcpy(ar_config_eth[port_idx].ip_add, value);
                }
                if (strcmp(key, "subnet_mask") == 0){
                    strcpy(ar_config_eth[port_idx].subnet_mask, value);
                }
                if (strcmp(key, "gateway") == 0){
                    strcpy(ar_config_eth[port_idx].gateway, value);
                }
                if (strcmp(key, "port_descrip") == 0){
                    strcpy(ar_config_eth[port_idx].port_descrip, value);
                }
                if (strcmp(key, "port_object_id") == 0){
                    strcpy(ar_config_eth[port_idx].port_object_id, value);
                }                
            }

            /* Read performance config for v7isrPerformance */
            {
                if (strcmp(key, "ram_info") == 0){
                    strcpy(ar_performance[0].ram_info, value);
                }
                if (strcmp(key, "cpu_info") == 0){
                    strcpy(ar_performance[0].cpu_info, value);
                }
            }

            /* Read general config */
            {
                if (strcmp(key, "general_id") == 0){
                    strcpy(ar_general_config[0].general_id, value);
                }                
            }
		}
	}
}

void read_infomation(struct general_config *ar_general_config,
                      char *dir_infomation)
{
/** Ham doc file infomations.txt trong /etc
 *  Thong tin node id se duoc doc va luu vao general_config.node_id
*/
    int num;
    FILE *fptr;
    int line_size;
    char str_line[300];
    int index = 0;
    fptr = fopen(dir_infomation,"r");

    if(fptr == NULL)
    {
        printf("Error: Can not open infomation file to read! \n");   
        exit(1);
    }
    while(fgets(str_line, 300, fptr) != NULL) {
        index = find(str_line, ":");

        if (index != -1) {
            char *key = trim(str_line,0,index-1);
            char *value = trim(str_line, index + 2, strlen(str_line)-1);
            /* Read node it config */
            {
                if (strcmp(key, "Node id") == 0) {
                    strcpy(ar_general_config[0].node_id, value);
                }
            }
        }
    }
}