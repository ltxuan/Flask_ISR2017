#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "read_config_snmp.h"
#include "subagentObject.h"
#include <pthread.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "sys/sysinfo.h"
#include <time.h>
#include <asm/types.h>
#include <errno.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <sys/time.h>
#include <sys/types.h>

#define BUFFER_LEN  1024
#define MSG_LEN     64
#define MSG_NUM     100

#define NORMAL_VAL      0
#define HIGH_VAL        1
#define TOO_HIGH_VAL    2

char *port0_name = "eth5";
char *port1_name = "eth4";
char *port2_name = "eth3";
char *port3_name = "eth2";

int read_eth_event (int sockint, int *port0_stat, int *port1_stat, int *port2_stat, int *port3_stat)
{
    int status;
    int ret = 0;
    char buf[4096];
    struct iovec iov = { buf, sizeof buf };
    struct sockaddr_nl snl;
    struct msghdr msg = { (void *) &snl, sizeof snl, &iov, 1, NULL, 0, 0 };
    struct nlmsghdr *hdr;
    struct ifinfomsg *ifi;
    status = recvmsg (sockint, &msg, 0);

    if (status < 0) {
        if (errno == EWOULDBLOCK || errno == EAGAIN)
        return ret;
        printf ("read_netlink: Error recvmsg: %d\n", status);
        perror ("read_netlink: Error: ");
        return status;
    }
    if (status == 0) printf ("read_netlink: EOF\n");

    char ifname[1024];
    for (hdr = (struct nlmsghdr *) buf; NLMSG_OK(hdr, (unsigned int) status); hdr = NLMSG_NEXT(hdr, status)) {
        if (hdr->nlmsg_type == NLMSG_DONE) return ret;
        if (hdr->nlmsg_type == NLMSG_ERROR) {
            printf ("read_netlink: Message is an error - decode TBD\n");
            return -1;        
        }
        if (hdr->nlmsg_type == RTM_NEWLINK) {
            ifi = NLMSG_DATA (hdr);
            if_indextoname(ifi->ifi_index,ifname);
            if (strcmp(ifname, port0_name) == 0) {
                if (ifi->ifi_flags & IFF_RUNNING) {
                    trap_port_up(ar_config_eth[0].port_object_id);
		    printf("========== port 0 up \n"); 
		} else {
			trap_port_down(ar_config_eth[0].port_object_id);
			printf("========== port 0 ddownn \n");
		}
            }
            else if (strcmp(ifname, port1_name) == 0) {
//                (ifi->ifi_flags & IFF_RUNNING) ? trap_port_up(ar_config_eth[1].port_object_id) : trap_port_down(ar_config_eth[1].port_object_id);
                if (ifi->ifi_flags & IFF_RUNNING) {
                    trap_port_up(ar_config_eth[0].port_object_id);
		    printf("========== port 1 up \n"); 
		} else {
			trap_port_down(ar_config_eth[0].port_object_id);
			printf("========== port 1 ddownn \n");
		}
            }
            else if (strcmp(ifname, port2_name) == 0) {
                (ifi->ifi_flags & IFF_RUNNING) ? trap_port_up(ar_config_eth[2].port_object_id) : trap_port_down(ar_config_eth[2].port_object_id);
            }
            else if (strcmp(ifname, port3_name) == 0) {
                (ifi->ifi_flags & IFF_RUNNING) ? trap_port_up(ar_config_eth[3].port_object_id) : trap_port_down(ar_config_eth[3].port_object_id);
            }
            else
                printf("Not found port name \n");    
        }
    }
    return ret;
}

int get_port_state(char *command, char *state)
{
    FILE *fp;
    char value_string[1024];
    int stat = 0;
    /* Check for system value  */
    fp = popen(command, "r");
    if (fp == NULL) {
        printf("Failed to check command port \n" );
        return -1;
    }
    while (fgets(value_string, sizeof(value_string), fp) != NULL);

    stat = strcmp(value_string, state); // stat = 10 with up
    return stat;
    pclose(fp);
}

unsigned int get_system_value(char *command)
{
    FILE *fp;
    char value_string[1024];
    fflush(fp);
    fp = popen(command, "r");
    if (fp == NULL) {
        printf("Failed to check command system \n" );
        return -1;
    }

    while (fgets(value_string, sizeof(value_string), fp) != NULL);
    unsigned int value = atoi(value_string);
    pclose(fp);
    return value;
}

unsigned int get_port_error(char *mode, int port)
{
    char command[1000];

    snprintf(command, sizeof(command), "%s%d/statistics/%s_errors", 
            "cat /sys/class/net/eth", 
            port, mode);
    //snprintf(command, sizeof(command), "%s%d/statistics/%s_errors", 
    //        "cat /home/ata/thanh_snmp/eth", 
    //        port, mode);    
    //printf("========== command = %s \n", command);
    int error = 0;
    error = get_system_value(command);
    return error;
}

void *get_sys_stat(void* arg)
{
    float cpu_percent = 0;
    int cpu_flag = NORMAL_VAL;

    float temperature = 0;
    int temp_flag = NORMAL_VAL;

    float ram_percent = 0;
    int ram_flag = NORMAL_VAL;

    int prev_tx_errors[4];
    int cur_tx_errors[4];
    memset (prev_tx_errors, 0, 4*sizeof(prev_tx_errors[0]));
    memset (cur_tx_errors, 0, 4*sizeof(cur_tx_errors[0]));

    int prev_rx_errors[4];
    int cur_rx_errors[4];
    memset (prev_rx_errors, 0, 4*sizeof(prev_rx_errors[0]));
    memset (cur_rx_errors, 0, 4*sizeof(cur_rx_errors[0]));

    char msg[1000];

    while(1) {
        cpu_percent = get_system_value("mpstat | awk '$12 ~ /[0-9.]+/ { print 100 - $12"" }'");
        if (cpu_percent > 50 && cpu_percent < 90) {
            if (cpu_flag == NORMAL_VAL) trap_cpu_high(ar_general_config[0].general_id);
            cpu_flag = HIGH_VAL;
        } else if (cpu_percent > 90) {
            if (cpu_flag != TOO_HIGH_VAL) trap_cpu_too_high(ar_general_config[0].general_id);
            cpu_flag = TOO_HIGH_VAL;
        } else cpu_flag = NORMAL_VAL;

        temperature = get_system_value("cat /sys/class/thermal/thermal_zone1/temp");
        if (temperature > 50000 && temperature < 55000) {
            if (temp_flag == NORMAL_VAL) trap_temp_high(ar_general_config[0].general_id);
            temp_flag = HIGH_VAL;
        } else if (temperature > 55000) {
            if (temp_flag != TOO_HIGH_VAL) trap_temp_too_high(ar_general_config[0].general_id);
            temp_flag = TOO_HIGH_VAL;
        } else temp_flag = NORMAL_VAL;

        ram_percent = get_system_value("free | grep Mem | awk '{print $3/$2 * 100.0}'");
        if (ram_percent > 70 && ram_percent < 85) {
            if (ram_flag == NORMAL_VAL) trap_ram_high(ar_general_config[0].general_id);
            ram_flag = HIGH_VAL;
        } else if (ram_percent > 85) {
            if (ram_flag != TOO_HIGH_VAL) trap_ram_too_high(ar_general_config[0].general_id);
            ram_flag = TOO_HIGH_VAL;
        } else ram_flag = NORMAL_VAL;

        for (int i = 0; i < 4; i++) {
            cur_tx_errors[i] = get_port_error("tx", i);
            prev_rx_errors[i] = get_port_error("rx", i);

            if (cur_tx_errors[i] != prev_tx_errors[i]) {
                snprintf(msg, sizeof(msg), "%s_%d", "tx", i);
                trap_port_err(msg);
            }
            prev_tx_errors[i] = cur_tx_errors[i];

            if (cur_rx_errors[i] != prev_rx_errors[i]) {
                snprintf(msg, sizeof(msg), "%s_%d", "rx", i);
                trap_port_err(msg);
            }
            prev_rx_errors[i] = cur_rx_errors[i];
        }

        sleep(3);
    }
}

int listenfd = -1;
int connfd = -1;
struct sockaddr_in server_addr;

struct message parse_msg (char msg_token[MSG_LEN])
{
/** Ham phan tich message tu mot mang cac message. Chuoi message nhan duoc tu buffer cua
  * socket gom rat nhieu message va duoc tach vao trong 1 mang. Ham nay se phan tich
  * tung message va luu vao trong struct message.
*/
    struct message msg;
    int i = 0;
    int type_index;
    int key_index;

    type_index = find(msg_token, "_");
    msg.type = trim(msg_token,0,type_index);
    char *message = trim(msg_token, type_index + 1, strlen(msg_token));

    key_index = find(message, ":");
    msg.key = trim(message,0,key_index);
    msg.value = trim(message, key_index + 1, strlen(message));
    return msg;
}

void create_trap_from_buf (char socket_buffer[BUFFER_LEN])
{
/** Ham gui trap tu du lieu nhan duoc trong buffer. Input cua ham la du lieu nhan duoc
 *  tu buffer. Du lieu nhan duoc tu buffer socket gom nhieu message duoc phan tach bang
 *  dau ; vi vay can duoc tach thanh tung message va luu vao 1 mang. Sau do tung phan tu
 *  cua mang se duoc phan tich bang ham parse_message.
 *  Sau do ham gui trap se duoc goi voi input la du lieu sau parse.
*/

    struct message msg[MSG_NUM];
    int i = 0;
    char *ptr = strtok(socket_buffer, ";");
    char *msg_array[MSG_NUM];
    memset (msg_array, 0, MSG_NUM*sizeof(msg_array[0]));

    while( ptr != NULL ) {
        msg_array[i++] = ptr;
        ptr = strtok(NULL, ";");
    }
    for (i = 0; i < MSG_NUM; ++i) {
        if (msg_array[i] != NULL) {
            printf("Message thu %d la %s\n", i, msg_array[i]);
            msg[i] = parse_msg(msg_array[i]);
            trap_account((void *) &msg[i]);
        }
    }
}

void *socket_handle(void *arg)
{
/** Ham bat va xu ly du lieu nhan duoc tu socket. Ham nay duoc chay trong 1 thread khac.
  * Du lieu dang message duoc gui tu web se duoc thead socket bat trong ham nay va
  * phan tich noi dung cua message. Message sau khi duoc parse se duoc dung de gui
  * trap.
  * Example:
  *                                         __________________________________
  * pjsip_key1:val1;asterisk_key2:val2; >> |pjsip_key1:val1|asterisk_key2:val2| >> msg1;msg2
  *                                        |_______msg1____|________msg2______|      
*/
	char socket_buffer[BUFFER_LEN];
    int check;

    memset(socket_buffer, 0, sizeof(socket_buffer));
	memset(&server_addr, 0, sizeof(server_addr));

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(5000);

	bind(listenfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	listen(listenfd, 10);

	while(1) {
		connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
        check = read(connfd, socket_buffer, sizeof(socket_buffer)-1);
        printf("socket_buffer = %s\n", socket_buffer);
        create_trap_from_buf(socket_buffer);
		close(connfd);
	}
	close(listenfd);
}

static int keep_running;
netsnmp_variable_list *shutdown_vars;
RETSIGTYPE
stop_server(int arg) {
    close(listenfd);
    /* variable list for trap stop */
    shutdown_vars = NULL;
    snmp_varlist_add_variable(&shutdown_vars,
                            objid_snmptrap, OID_LENGTH(objid_snmptrap),
                            ASN_OBJECT_ID, (u_char *) isr_alrm_oid,
                            OID_LENGTH(objid_snmptrap) * sizeof(oid));

    char     isr_obj[MAX_LEN*2];
    snprintf(isr_obj, sizeof(isr_obj), "%s_%s", ar_general_config[0].node_id, ar_general_config[0].general_id);
    snmp_varlist_add_variable(&shutdown_vars,
                            isr_object_oid, OID_LENGTH(isr_object_oid),
                            ASN_OCTET_STR,
                            isr_obj, strlen(isr_obj));
                        
    static int      isr_servirity = CRITICAL;
    snmp_varlist_add_variable(&shutdown_vars,
                            isr_servirity_oid, OID_LENGTH(isr_servirity_oid),
                            ASN_INTEGER, (u_char *)&isr_servirity,
                                sizeof(isr_servirity));

    static int      isr_even_type_num = 1;
    snmp_varlist_add_variable(&shutdown_vars,
                            isr_even_type_num_oid, OID_LENGTH(isr_even_type_num_oid),
                            ASN_INTEGER, (u_char *)&isr_even_type_num,
                                sizeof(isr_even_type_num));

    static int      isr_even_type = 1;
    snmp_varlist_add_variable(&shutdown_vars,
                            isr_even_type_oid, OID_LENGTH(isr_even_type_oid),
                            ASN_INTEGER, (u_char *)&isr_even_type,
                                sizeof(isr_even_type));

    static int      isr_problem_num = 3;
    snmp_varlist_add_variable(&shutdown_vars,
                            isr_problem_num_oid, OID_LENGTH(isr_problem_num_oid),
                            ASN_INTEGER, (u_char *)&isr_problem_num,
                            sizeof(isr_problem_num));

    char     *isr_problem = "snmp alarm";
    snmp_varlist_add_variable(&shutdown_vars,
                            isr_problem_oid, OID_LENGTH(isr_problem_oid),
                            ASN_OCTET_STR,
                            isr_problem, strlen(isr_problem));

    char     *isr_add_text = "snmp stop";
    snmp_varlist_add_variable(&shutdown_vars,
                            isr_add_text_oid, OID_LENGTH(isr_add_text_oid),
                            ASN_OCTET_STR,
                            isr_add_text, strlen(isr_add_text));

    char     *isr_even_time;
    isr_even_time = get_cur_time();
    snmp_varlist_add_variable(&shutdown_vars,
                            isr_even_time_oid, OID_LENGTH(isr_even_time_oid),
                            ASN_OCTET_STR,
                            isr_even_time, strlen(isr_even_time));

    static int      isr_alarm_state = 1;
    snmp_varlist_add_variable(&shutdown_vars,
                            isr_alarm_state_oid, OID_LENGTH(isr_alarm_state_oid),
                            ASN_INTEGER, (u_char *)&isr_alarm_state,
                                sizeof(isr_alarm_state));

    static int      count = 0;
    snmp_varlist_add_variable(&shutdown_vars,
                            isr_count_oid, OID_LENGTH(isr_count_oid),
                            ASN_INTEGER, (u_char *)&count,
                                sizeof(count));
    send_v2trap(shutdown_vars);
    exit(1);
}

int main (int argc, char **argv) {
    int agentx_subagent=0;
    int background = 0;
    int syslog = 0;

    /* Check ethernet port */
    fd_set rfds, wfds;
    struct timeval tv;
    int retval;
    struct sockaddr_nl addr;
    int nl_socket = socket (AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);

    if (nl_socket < 0) {
        printf ("Socket Open Error!");
        exit (1);
    }
    memset ((void *) &addr, 0, sizeof (addr));
    addr.nl_family = AF_NETLINK;
    addr.nl_pid = getpid ();
    addr.nl_groups = RTMGRP_LINK | RTMGRP_IPV4_IFADDR | RTMGRP_IPV6_IFADDR;
    if (bind (nl_socket, (struct sockaddr *) &addr, sizeof (addr)) < 0) exit (1);

    struct sysinfo memInfo;
    float ram_percent = 0;

    read_config_snmp(ar_config_inventory, ar_config_eth,
                     ar_account_obj_id, ar_performance,
                     ar_general_config, "config");
    read_infomation(ar_general_config, "/etc/informations.txt");
        
    if (syslog) snmp_enable_calllog();
    else snmp_enable_stderrlog();
    if (agentx_subagent)
        netsnmp_ds_set_boolean(NETSNMP_DS_APPLICATION_ID, NETSNMP_DS_AGENT_ROLE, 1);
    if (background && netsnmp_daemonize(1, !syslog)) exit(1);
    SOCK_STARTUP;
    init_agent("example-demon");
    trap_start(ar_general_config[0].general_id);

    init_snmp("example-demon");

    /* open the port to listen on (defaults to udp:161) */
    if (!agentx_subagent) init_master_agent(); 

    keep_running = 1;
    signal(SIGTERM, stop_server);
    signal(SIGINT, stop_server);    

    pthread_t sys_stat_thread;
    pthread_create(&sys_stat_thread, NULL, get_sys_stat, NULL);
    pthread_t socket_thread;
    pthread_create(&socket_thread, NULL, socket_handle, NULL);    

    /* Check port ethernet here 1 is up */
    int port0_stat = 1;
    int port1_stat = 1;
    int port2_stat = 1;
    int port3_stat = 1;

    port0_stat = get_port_state("cat /sys/class/net/eth5/operstate", "up");
    port1_stat = get_port_state("cat /sys/class/net/eth4/operstate", "up");
    port2_stat = get_port_state("cat /sys/class/net/eth3/operstate", "up");
    port3_stat = get_port_state("cat /sys/class/net/eth2/operstate", "up");

    printf("============ port0 stat = %d \n", port0_stat);

    (port0_stat < 0)
        ? trap_port_down(ar_config_eth[0].port_object_id)
        : trap_port_up(ar_config_eth[0].port_object_id);
    (port1_stat < 0)
        ? trap_port_down(ar_config_eth[1].port_object_id)
        : trap_port_up(ar_config_eth[1].port_object_id);
    (port2_stat < 0)
        ? trap_port_down(ar_config_eth[2].port_object_id)
        : trap_port_up(ar_config_eth[2].port_object_id);
    (port3_stat < 0)
        ? trap_port_down(ar_config_eth[3].port_object_id)
        : trap_port_up(ar_config_eth[3].port_object_id);

    while(keep_running) {
        agent_check_and_process(0);

        FD_ZERO (&rfds);
        FD_CLR (nl_socket, &rfds);
        FD_SET (nl_socket, &rfds);
        tv.tv_sec = 10;
        tv.tv_usec = 0;
        retval = select (FD_SETSIZE, &rfds, NULL, NULL, &tv);
        if (retval == -1) printf ("Error select() \n");
        else if (retval) {
            read_eth_event (nl_socket, &port0_stat, &port1_stat, &port2_stat, &port3_stat);
        }
        else {
            // Do nothing
        };
        sleep(1);
    }

    close(listenfd);
    snmp_shutdown("example-demon");
    SOCK_CLEANUP;
    return 0;
}
