#ifndef CONFIG_H
#define CONFIG_H
#define MAX_LEN         100
#define NUM_PORT_ETH    4
#define NUM_OBJ_ID      1

struct general_config {
    char node_id[MAX_LEN];
    char general_id[MAX_LEN];
};

struct config_inventory {
    char ne_serial_num[MAX_LEN];
    char ne_sw_ver[MAX_LEN];
    char cpu_car_name[MAX_LEN];
    char cpu_hw_ver[MAX_LEN];
    char cpu_sw_ver[MAX_LEN];
    char ata_card_name[MAX_LEN];
    char ata_hw_ver[MAX_LEN];
    char ata_sw_ver[MAX_LEN];
    char descip_inventory[MAX_LEN];
    char isr_version[MAX_LEN];
};

struct config_port_eth {
	char ip_add[MAX_LEN];
    char subnet_mask[MAX_LEN];
    char gateway[MAX_LEN];
    char port_descrip[MAX_LEN];
    char port_object_id[MAX_LEN];
};

struct account_obj_id {
    char sip_object_id[MAX_LEN];
    char asterisk_object_id[MAX_LEN];
    char quadga_object_id[MAX_LEN];
};

struct performance {
    char ram_info[MAX_LEN];
    char cpu_info[MAX_LEN];
};

struct message {
    char *type;
    char *key;
    char *value;
};

void del_line(char *src);
int find(char *src, char *key);
char *trim(char *src, int start, int end);
char *get_cur_time();
void read_config_snmp(struct config_inventory *ar_config_inventory,
                      struct config_port_eth *ar_config_eth,
                      struct account_obj_id *ar_account_obj_id,
                      struct performance *ar_performance,
                      struct general_config *ar_general_config,
                      char *dir);
void read_infomation(struct general_config *ar_general_config,
                      char *dir_infomation);
#endif