#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include "subagentObject.h"
#include "read_config_snmp.h"
#include <string.h>
#include <stdio.h>
#include <net-snmp/net-snmp-features.h>


static netsnmp_table_data_set *table_set;

/*
 * the variable we want to tie an OID to.  The agent will handle all
 * * GET and SET requests to this variable changing it's value as needed.
 */

/*======================================= v7isrAlarm ==========================================*/

void trap_start_cb(unsigned int clientreg, void *general_id)
{
    netsnmp_variable_list *notification_vars = NULL;

    snmp_varlist_add_variable(&notification_vars,
                              objid_snmptrap, OID_LENGTH(objid_snmptrap),
                              ASN_OBJECT_ID, (u_char *) isr_alrm_oid,
                              OID_LENGTH(objid_snmptrap) * sizeof(oid));

    char     isr_obj[MAX_LEN*2];
    snprintf(isr_obj, sizeof(isr_obj), "%s_%s", ar_general_config[0].node_id, (char*) general_id);
    snmp_varlist_add_variable(&notification_vars,
                               isr_object_oid, OID_LENGTH(isr_object_oid),
                               ASN_OCTET_STR,
                               isr_obj, strlen(isr_obj));

    static int      isr_servirity = MAJOR;
    snmp_varlist_add_variable(&notification_vars,
                               isr_servirity_oid, OID_LENGTH(isr_servirity_oid),
                               ASN_INTEGER, (u_char *)&isr_servirity,
                                sizeof(isr_servirity));

    static int      isr_even_type_num = 1;
    snmp_varlist_add_variable(&notification_vars,
                               isr_even_type_num_oid, OID_LENGTH(isr_even_type_num_oid),
                               ASN_INTEGER, (u_char *)&isr_even_type_num,
                                sizeof(isr_even_type_num));

    static int      isr_even_type = 1;
    snmp_varlist_add_variable(&notification_vars,
                               isr_even_type_oid, OID_LENGTH(isr_even_type_oid),
                               ASN_INTEGER, (u_char *)&isr_even_type,
                                sizeof(isr_even_type));

    static int      isr_problem_num = 1;
    snmp_varlist_add_variable(&notification_vars,
                               isr_problem_num_oid, OID_LENGTH(isr_problem_num_oid),
                               ASN_INTEGER, (u_char *)&isr_problem_num,
                                sizeof(isr_problem_num));

    char     *isr_problem = "system alarm";
    snmp_varlist_add_variable(&notification_vars,
                               isr_problem_oid, OID_LENGTH(isr_problem_oid),
                               ASN_OCTET_STR,
                               isr_problem, strlen(isr_problem));

    char     *isr_add_text = "SNMP start";
    snmp_varlist_add_variable(&notification_vars,
                               isr_add_text_oid, OID_LENGTH(isr_add_text_oid),
                               ASN_OCTET_STR,
                               isr_add_text, strlen(isr_add_text));

    char     *isr_even_time;
    isr_even_time = get_cur_time();
    snmp_varlist_add_variable(&notification_vars,
                               isr_even_time_oid, OID_LENGTH(isr_even_time_oid),
                               ASN_OCTET_STR,
                               isr_even_time, strlen(isr_even_time));

    static int      isr_alarm_state = 1;
    snmp_varlist_add_variable(&notification_vars,
                               isr_alarm_state_oid, OID_LENGTH(isr_alarm_state_oid),
                               ASN_INTEGER, (u_char *)&isr_alarm_state,
                                sizeof(isr_alarm_state));

    static int      count = 0;
    snmp_varlist_add_variable(&notification_vars,
                               isr_count_oid, OID_LENGTH(isr_count_oid),
                               ASN_INTEGER, (u_char *)&count,
                                sizeof(count));

    send_v2trap(notification_vars);
    snmp_free_varbind(notification_vars);
}

void trap_start(void *general_id)
{
        snmp_alarm_register( 0, 0, trap_start_cb, general_id );
}

void trap_port_up_cb(unsigned int port_id, void *port)
{   
    netsnmp_variable_list *notification_vars = NULL;

    snmp_varlist_add_variable(&notification_vars,
                              objid_snmptrap, OID_LENGTH(objid_snmptrap),
                              ASN_OBJECT_ID, (u_char *) isr_alrm_oid,
                              OID_LENGTH(objid_snmptrap) * sizeof(oid));

    char     isr_obj[MAX_LEN*2];
    snprintf(isr_obj, sizeof(isr_obj), "%s_%s", ar_general_config[0].node_id, (char*) port);
    snmp_varlist_add_variable(&notification_vars,
                               isr_object_oid, OID_LENGTH(isr_object_oid),
                               ASN_OCTET_STR,
                               isr_obj, strlen(isr_obj));

    static int      isr_even_type_num = 1;
    snmp_varlist_add_variable(&notification_vars,
                               isr_even_type_num_oid, OID_LENGTH(isr_even_type_num_oid),
                               ASN_INTEGER, (u_char *)&isr_even_type_num,
                                sizeof(isr_even_type_num));

    static int      isr_even_type = 1;
    snmp_varlist_add_variable(&notification_vars,
                               isr_even_type_oid, OID_LENGTH(isr_even_type_oid),
                               ASN_INTEGER, (u_char *)&isr_even_type,
                                sizeof(isr_even_type));

    static int      isr_problem_num = 20;
    snmp_varlist_add_variable(&notification_vars,
                               isr_problem_num_oid, OID_LENGTH(isr_problem_num_oid),
                               ASN_INTEGER, (u_char *)&isr_problem_num,
                                sizeof(isr_problem_num));

    char            *isr_problem  = "link alarm";
    char            *isr_add_text = "link up";
    static int      isr_servirity = CLEAR;

    snmp_varlist_add_variable(&notification_vars,
                               isr_servirity_oid, OID_LENGTH(isr_servirity_oid),
                               ASN_INTEGER, (u_char *)&isr_servirity,
                                sizeof(isr_servirity));
    snmp_varlist_add_variable(&notification_vars,
                               isr_problem_oid, OID_LENGTH(isr_problem_oid),
                               ASN_OCTET_STR,
                               isr_problem, strlen(isr_problem));
    snmp_varlist_add_variable(&notification_vars,
                               isr_add_text_oid, OID_LENGTH(isr_add_text_oid),
                               ASN_OCTET_STR,
                               isr_add_text, strlen(isr_add_text));

    char     *isr_even_time;
    isr_even_time = get_cur_time();
    snmp_varlist_add_variable(&notification_vars,
                               isr_even_time_oid, OID_LENGTH(isr_even_time_oid),
                               ASN_OCTET_STR,
                               isr_even_time, strlen(isr_even_time));

    static int      isr_alarm_state = 1;
    snmp_varlist_add_variable(&notification_vars,
                               isr_alarm_state_oid, OID_LENGTH(isr_alarm_state_oid),
                               ASN_INTEGER, (u_char *)&isr_alarm_state,
                                sizeof(isr_alarm_state));

    static int      count = 0;
    snmp_varlist_add_variable(&notification_vars,
                               isr_count_oid, OID_LENGTH(isr_count_oid),
                               ASN_INTEGER, (u_char *)&count,
                                sizeof(count));

    send_v2trap(notification_vars);
    snmp_free_varbind(notification_vars);
}

void trap_port_up(void *port)
{
        snmp_alarm_register(0, 0, trap_port_up_cb, port);
}

void trap_port_down_cb(unsigned int port_id, void *port)
{   
    netsnmp_variable_list *notification_vars = NULL;

    snmp_varlist_add_variable(&notification_vars,
                              objid_snmptrap, OID_LENGTH(objid_snmptrap),
                              ASN_OBJECT_ID, (u_char *) isr_alrm_oid,
                              OID_LENGTH(objid_snmptrap) * sizeof(oid));

    char     isr_obj[MAX_LEN*2];
    snprintf(isr_obj, sizeof(isr_obj), "%s_%s", ar_general_config[0].node_id, (char*) port);
    snmp_varlist_add_variable(&notification_vars,
                               isr_object_oid, OID_LENGTH(isr_object_oid),
                               ASN_OCTET_STR,
                               isr_obj, strlen(isr_obj));

    static int      isr_even_type_num = 1;
    snmp_varlist_add_variable(&notification_vars,
                               isr_even_type_num_oid, OID_LENGTH(isr_even_type_num_oid),
                               ASN_INTEGER, (u_char *)&isr_even_type_num,
                                sizeof(isr_even_type_num));

    static int      isr_even_type = 1;
    snmp_varlist_add_variable(&notification_vars,
                               isr_even_type_oid, OID_LENGTH(isr_even_type_oid),
                               ASN_INTEGER, (u_char *)&isr_even_type,
                                sizeof(isr_even_type));

    static int      isr_problem_num = 20;
    snmp_varlist_add_variable(&notification_vars,
                               isr_problem_num_oid, OID_LENGTH(isr_problem_num_oid),
                               ASN_INTEGER, (u_char *)&isr_problem_num,
                                sizeof(isr_problem_num));

    char            *isr_problem  = "link alarm";
    char            *isr_add_text = "link down";
    static int      isr_servirity = CRITICAL; 

    snmp_varlist_add_variable(&notification_vars,
                               isr_servirity_oid, OID_LENGTH(isr_servirity_oid),
                               ASN_INTEGER, (u_char *)&isr_servirity,
                                sizeof(isr_servirity));
    snmp_varlist_add_variable(&notification_vars,
                               isr_problem_oid, OID_LENGTH(isr_problem_oid),
                               ASN_OCTET_STR,
                               isr_problem, strlen(isr_problem));
    snmp_varlist_add_variable(&notification_vars,
                               isr_add_text_oid, OID_LENGTH(isr_add_text_oid),
                               ASN_OCTET_STR,
                               isr_add_text, strlen(isr_add_text));

    char     *isr_even_time;
    isr_even_time = get_cur_time();
    snmp_varlist_add_variable(&notification_vars,
                               isr_even_time_oid, OID_LENGTH(isr_even_time_oid),
                               ASN_OCTET_STR,
                               isr_even_time, strlen(isr_even_time));

    static int      isr_alarm_state = 1;
    snmp_varlist_add_variable(&notification_vars,
                               isr_alarm_state_oid, OID_LENGTH(isr_alarm_state_oid),
                               ASN_INTEGER, (u_char *)&isr_alarm_state,
                                sizeof(isr_alarm_state));

    static int      count = 0;
    snmp_varlist_add_variable(&notification_vars,
                               isr_count_oid, OID_LENGTH(isr_count_oid),
                               ASN_INTEGER, (u_char *)&count,
                                sizeof(count));

    send_v2trap(notification_vars);
    snmp_free_varbind(notification_vars);
}

void trap_port_down(void *port)
{
        snmp_alarm_register(0, 0, trap_port_down_cb, port);
}

void trap_ram_high_cb(unsigned int clientreg, void *general_id)
{
    netsnmp_variable_list *notification_vars = NULL;

    snmp_varlist_add_variable(&notification_vars,
                              objid_snmptrap, OID_LENGTH(objid_snmptrap),
                              ASN_OBJECT_ID, (u_char *) isr_alrm_oid,
                              OID_LENGTH(objid_snmptrap) * sizeof(oid));

    char     isr_obj[MAX_LEN*2];
    snprintf(isr_obj, sizeof(isr_obj), "%s_%s", ar_general_config[0].node_id, (char*) general_id);
    snmp_varlist_add_variable(&notification_vars,
                               isr_object_oid, OID_LENGTH(isr_object_oid),
                               ASN_OCTET_STR,
                               isr_obj, strlen(isr_obj));

    static int      isr_servirity = WARNING;
    snmp_varlist_add_variable(&notification_vars,
                               isr_servirity_oid, OID_LENGTH(isr_servirity_oid),
                               ASN_INTEGER, (u_char *)&isr_servirity,
                                sizeof(isr_servirity));

    static int      isr_even_type_num = 1;
    snmp_varlist_add_variable(&notification_vars,
                               isr_even_type_num_oid, OID_LENGTH(isr_even_type_num_oid),
                               ASN_INTEGER, (u_char *)&isr_even_type_num,
                                sizeof(isr_even_type_num));

    static int      isr_even_type = 1;
    snmp_varlist_add_variable(&notification_vars,
                               isr_even_type_oid, OID_LENGTH(isr_even_type_oid),
                               ASN_INTEGER, (u_char *)&isr_even_type,
                                sizeof(isr_even_type));

    static int      isr_problem_num = 8;
    snmp_varlist_add_variable(&notification_vars,
                               isr_problem_num_oid, OID_LENGTH(isr_problem_num_oid),
                               ASN_INTEGER, (u_char *)&isr_problem_num,
                                sizeof(isr_problem_num));

    char     *isr_problem = "system alarm";
    snmp_varlist_add_variable(&notification_vars,
                               isr_problem_oid, OID_LENGTH(isr_problem_oid),
                               ASN_OCTET_STR,
                               isr_problem, strlen(isr_problem));

    char     *isr_add_text = "RAM high";
    snmp_varlist_add_variable(&notification_vars,
                               isr_add_text_oid, OID_LENGTH(isr_add_text_oid),
                               ASN_OCTET_STR,
                               isr_add_text, strlen(isr_add_text));

    char     *isr_even_time;
    isr_even_time = get_cur_time();
    snmp_varlist_add_variable(&notification_vars,
                               isr_even_time_oid, OID_LENGTH(isr_even_time_oid),
                               ASN_OCTET_STR,
                               isr_even_time, strlen(isr_even_time));

    static int      isr_alarm_state = 1;
    snmp_varlist_add_variable(&notification_vars,
                               isr_alarm_state_oid, OID_LENGTH(isr_alarm_state_oid),
                               ASN_INTEGER, (u_char *)&isr_alarm_state,
                                sizeof(isr_alarm_state));

    static int      count = 0;
    snmp_varlist_add_variable(&notification_vars,
                               isr_count_oid, OID_LENGTH(isr_count_oid),
                               ASN_INTEGER, (u_char *)&count,
                                sizeof(count));

    send_v2trap(notification_vars);
    snmp_free_varbind(notification_vars);
}

void trap_ram_high(void *general_id)
{
        snmp_alarm_register(0, 0, trap_ram_high_cb, general_id);
}

void trap_ram_too_high_cb(unsigned int clientreg, void *general_id)
{
    netsnmp_variable_list *notification_vars = NULL;

    snmp_varlist_add_variable(&notification_vars,
                              objid_snmptrap, OID_LENGTH(objid_snmptrap),
                              ASN_OBJECT_ID, (u_char *) isr_alrm_oid,
                              OID_LENGTH(objid_snmptrap) * sizeof(oid));

    char     isr_obj[MAX_LEN*2];
    snprintf(isr_obj, sizeof(isr_obj), "%s_%s", ar_general_config[0].node_id, (char*) general_id);
    snmp_varlist_add_variable(&notification_vars,
                               isr_object_oid, OID_LENGTH(isr_object_oid),
                               ASN_OCTET_STR,
                               isr_obj, strlen(isr_obj));

    static int      isr_servirity = MAJOR;
    snmp_varlist_add_variable(&notification_vars,
                               isr_servirity_oid, OID_LENGTH(isr_servirity_oid),
                               ASN_INTEGER, (u_char *)&isr_servirity,
                                sizeof(isr_servirity));

    static int      isr_even_type_num = 1;
    snmp_varlist_add_variable(&notification_vars,
                               isr_even_type_num_oid, OID_LENGTH(isr_even_type_num_oid),
                               ASN_INTEGER, (u_char *)&isr_even_type_num,
                                sizeof(isr_even_type_num));

    static int      isr_even_type = 1;
    snmp_varlist_add_variable(&notification_vars,
                               isr_even_type_oid, OID_LENGTH(isr_even_type_oid),
                               ASN_INTEGER, (u_char *)&isr_even_type,
                                sizeof(isr_even_type));

    static int      isr_problem_num = 9;
    snmp_varlist_add_variable(&notification_vars,
                               isr_problem_num_oid, OID_LENGTH(isr_problem_num_oid),
                               ASN_INTEGER, (u_char *)&isr_problem_num,
                                sizeof(isr_problem_num));

    char     *isr_problem = "system alarm";
    snmp_varlist_add_variable(&notification_vars,
                               isr_problem_oid, OID_LENGTH(isr_problem_oid),
                               ASN_OCTET_STR,
                               isr_problem, strlen(isr_problem));

    char     *isr_add_text = "RAM too high";
    snmp_varlist_add_variable(&notification_vars,
                               isr_add_text_oid, OID_LENGTH(isr_add_text_oid),
                               ASN_OCTET_STR,
                               isr_add_text, strlen(isr_add_text));

    char     *isr_even_time;
    isr_even_time = get_cur_time();
    snmp_varlist_add_variable(&notification_vars,
                               isr_even_time_oid, OID_LENGTH(isr_even_time_oid),
                               ASN_OCTET_STR,
                               isr_even_time, strlen(isr_even_time));

    static int      isr_alarm_state = 1;
    snmp_varlist_add_variable(&notification_vars,
                               isr_alarm_state_oid, OID_LENGTH(isr_alarm_state_oid),
                               ASN_INTEGER, (u_char *)&isr_alarm_state,
                                sizeof(isr_alarm_state));

    static int      count = 0;
    snmp_varlist_add_variable(&notification_vars,
                               isr_count_oid, OID_LENGTH(isr_count_oid),
                               ASN_INTEGER, (u_char *)&count,
                                sizeof(count));

    send_v2trap(notification_vars);
    snmp_free_varbind(notification_vars);
}

void trap_ram_too_high(void *general_id)
{
        snmp_alarm_register(0, 0, trap_ram_too_high_cb, general_id);
}

void trap_cpu_high_cb(unsigned int clientreg, void *general_id)
{
    netsnmp_variable_list *notification_vars = NULL;

    snmp_varlist_add_variable(&notification_vars,
                              objid_snmptrap, OID_LENGTH(objid_snmptrap),
                              ASN_OBJECT_ID, (u_char *) isr_alrm_oid,
                              OID_LENGTH(objid_snmptrap) * sizeof(oid));

    char     isr_obj[MAX_LEN*2];
    snprintf(isr_obj, sizeof(isr_obj), "%s_%s", ar_general_config[0].node_id, (char*) general_id);
    snmp_varlist_add_variable(&notification_vars,
                               isr_object_oid, OID_LENGTH(isr_object_oid),
                               ASN_OCTET_STR,
                               isr_obj, strlen(isr_obj));

    static int      isr_servirity = WARNING;
    snmp_varlist_add_variable(&notification_vars,
                               isr_servirity_oid, OID_LENGTH(isr_servirity_oid),
                               ASN_INTEGER, (u_char *)&isr_servirity,
                                sizeof(isr_servirity));

    static int      isr_even_type_num = 1;
    snmp_varlist_add_variable(&notification_vars,
                               isr_even_type_num_oid, OID_LENGTH(isr_even_type_num_oid),
                               ASN_INTEGER, (u_char *)&isr_even_type_num,
                                sizeof(isr_even_type_num));

    static int      isr_even_type = 1;
    snmp_varlist_add_variable(&notification_vars,
                               isr_even_type_oid, OID_LENGTH(isr_even_type_oid),
                               ASN_INTEGER, (u_char *)&isr_even_type,
                                sizeof(isr_even_type));

    static int      isr_problem_num = 6;
    snmp_varlist_add_variable(&notification_vars,
                               isr_problem_num_oid, OID_LENGTH(isr_problem_num_oid),
                               ASN_INTEGER, (u_char *)&isr_problem_num,
                                sizeof(isr_problem_num));

    char     *isr_problem = "system alarm";
    snmp_varlist_add_variable(&notification_vars,
                               isr_problem_oid, OID_LENGTH(isr_problem_oid),
                               ASN_OCTET_STR,
                               isr_problem, strlen(isr_problem));

    char     *isr_add_text = "CPU high";
    snmp_varlist_add_variable(&notification_vars,
                               isr_add_text_oid, OID_LENGTH(isr_add_text_oid),
                               ASN_OCTET_STR,
                               isr_add_text, strlen(isr_add_text));

    char     *isr_even_time;
    isr_even_time = get_cur_time();
    snmp_varlist_add_variable(&notification_vars,
                               isr_even_time_oid, OID_LENGTH(isr_even_time_oid),
                               ASN_OCTET_STR,
                               isr_even_time, strlen(isr_even_time));

    static int      isr_alarm_state = 1;
    snmp_varlist_add_variable(&notification_vars,
                               isr_alarm_state_oid, OID_LENGTH(isr_alarm_state_oid),
                               ASN_INTEGER, (u_char *)&isr_alarm_state,
                                sizeof(isr_alarm_state));

    static int      count = 0;
    snmp_varlist_add_variable(&notification_vars,
                               isr_count_oid, OID_LENGTH(isr_count_oid),
                               ASN_INTEGER, (u_char *)&count,
                                sizeof(count));

    send_v2trap(notification_vars);
    snmp_free_varbind(notification_vars);
}

void trap_cpu_high(void *general_id)
{
        snmp_alarm_register(0, 0, trap_cpu_high_cb, general_id);
}

void trap_cpu_too_high_cb(unsigned int clientreg, void *general_id)
{
    netsnmp_variable_list *notification_vars = NULL;

    snmp_varlist_add_variable(&notification_vars,
                              objid_snmptrap, OID_LENGTH(objid_snmptrap),
                              ASN_OBJECT_ID, (u_char *) isr_alrm_oid,
                              OID_LENGTH(objid_snmptrap) * sizeof(oid));

    char     isr_obj[MAX_LEN*2];
    snprintf(isr_obj, sizeof(isr_obj), "%s_%s", ar_general_config[0].node_id, (char*) general_id);
    snmp_varlist_add_variable(&notification_vars,
                               isr_object_oid, OID_LENGTH(isr_object_oid),
                               ASN_OCTET_STR,
                               isr_obj, strlen(isr_obj));

    static int      isr_servirity = MAJOR;
    snmp_varlist_add_variable(&notification_vars,
                               isr_servirity_oid, OID_LENGTH(isr_servirity_oid),
                               ASN_INTEGER, (u_char *)&isr_servirity,
                                sizeof(isr_servirity));

    static int      isr_even_type_num = 1;
    snmp_varlist_add_variable(&notification_vars,
                               isr_even_type_num_oid, OID_LENGTH(isr_even_type_num_oid),
                               ASN_INTEGER, (u_char *)&isr_even_type_num,
                                sizeof(isr_even_type_num));

    static int      isr_even_type = 1;
    snmp_varlist_add_variable(&notification_vars,
                               isr_even_type_oid, OID_LENGTH(isr_even_type_oid),
                               ASN_INTEGER, (u_char *)&isr_even_type,
                                sizeof(isr_even_type));

    static int      isr_problem_num = 7;
    snmp_varlist_add_variable(&notification_vars,
                               isr_problem_num_oid, OID_LENGTH(isr_problem_num_oid),
                               ASN_INTEGER, (u_char *)&isr_problem_num,
                                sizeof(isr_problem_num));

    char     *isr_problem = "system alarm";
    snmp_varlist_add_variable(&notification_vars,
                               isr_problem_oid, OID_LENGTH(isr_problem_oid),
                               ASN_OCTET_STR,
                               isr_problem, strlen(isr_problem));

    char     *isr_add_text = "CPU too high";
    snmp_varlist_add_variable(&notification_vars,
                               isr_add_text_oid, OID_LENGTH(isr_add_text_oid),
                               ASN_OCTET_STR,
                               isr_add_text, strlen(isr_add_text));

    char     *isr_even_time;
    isr_even_time = get_cur_time();
    snmp_varlist_add_variable(&notification_vars,
                               isr_even_time_oid, OID_LENGTH(isr_even_time_oid),
                               ASN_OCTET_STR,
                               isr_even_time, strlen(isr_even_time));

    static int      isr_alarm_state = 1;
    snmp_varlist_add_variable(&notification_vars,
                               isr_alarm_state_oid, OID_LENGTH(isr_alarm_state_oid),
                               ASN_INTEGER, (u_char *)&isr_alarm_state,
                                sizeof(isr_alarm_state));

    static int      count = 0;
    snmp_varlist_add_variable(&notification_vars,
                               isr_count_oid, OID_LENGTH(isr_count_oid),
                               ASN_INTEGER, (u_char *)&count,
                                sizeof(count));

    send_v2trap(notification_vars);
    snmp_free_varbind(notification_vars);
}

void trap_cpu_too_high(void *general_id)
{
    snmp_alarm_register(0, 0, trap_cpu_too_high_cb, general_id);
}

void trap_temp_high_cb(unsigned int clientreg, void *general_id)
{
    netsnmp_variable_list *notification_vars = NULL;

    snmp_varlist_add_variable(&notification_vars,
                              objid_snmptrap, OID_LENGTH(objid_snmptrap),
                              ASN_OBJECT_ID, (u_char *) isr_alrm_oid,
                              OID_LENGTH(objid_snmptrap) * sizeof(oid));

    char     isr_obj[MAX_LEN*2];
    snprintf(isr_obj, sizeof(isr_obj), "%s_%s", ar_general_config[0].node_id, (char*) general_id);
    snmp_varlist_add_variable(&notification_vars,
                               isr_object_oid, OID_LENGTH(isr_object_oid),
                               ASN_OCTET_STR,
                               isr_obj, strlen(isr_obj));

    static int      isr_servirity = MINOR;
    snmp_varlist_add_variable(&notification_vars,
                               isr_servirity_oid, OID_LENGTH(isr_servirity_oid),
                               ASN_INTEGER, (u_char *)&isr_servirity,
                                sizeof(isr_servirity));

    static int      isr_even_type_num = 1;
    snmp_varlist_add_variable(&notification_vars,
                               isr_even_type_num_oid, OID_LENGTH(isr_even_type_num_oid),
                               ASN_INTEGER, (u_char *)&isr_even_type_num,
                                sizeof(isr_even_type_num));

    static int      isr_even_type = 1;
    snmp_varlist_add_variable(&notification_vars,
                               isr_even_type_oid, OID_LENGTH(isr_even_type_oid),
                               ASN_INTEGER, (u_char *)&isr_even_type,
                                sizeof(isr_even_type));

    static int      isr_problem_num = 4;
    snmp_varlist_add_variable(&notification_vars,
                               isr_problem_num_oid, OID_LENGTH(isr_problem_num_oid),
                               ASN_INTEGER, (u_char *)&isr_problem_num,
                                sizeof(isr_problem_num));

    char     *isr_problem = "system alarm";
    snmp_varlist_add_variable(&notification_vars,
                               isr_problem_oid, OID_LENGTH(isr_problem_oid),
                               ASN_OCTET_STR,
                               isr_problem, strlen(isr_problem));

    char     *isr_add_text = "Temperature high";
    snmp_varlist_add_variable(&notification_vars,
                               isr_add_text_oid, OID_LENGTH(isr_add_text_oid),
                               ASN_OCTET_STR,
                               isr_add_text, strlen(isr_add_text));

    char     *isr_even_time;
    isr_even_time = get_cur_time();
    snmp_varlist_add_variable(&notification_vars,
                               isr_even_time_oid, OID_LENGTH(isr_even_time_oid),
                               ASN_OCTET_STR,
                               isr_even_time, strlen(isr_even_time));

    static int      isr_alarm_state = 1;
    snmp_varlist_add_variable(&notification_vars,
                               isr_alarm_state_oid, OID_LENGTH(isr_alarm_state_oid),
                               ASN_INTEGER, (u_char *)&isr_alarm_state,
                                sizeof(isr_alarm_state));

    static int      count = 0;
    snmp_varlist_add_variable(&notification_vars,
                               isr_count_oid, OID_LENGTH(isr_count_oid),
                               ASN_INTEGER, (u_char *)&count,
                                sizeof(count));

    send_v2trap(notification_vars);
    snmp_free_varbind(notification_vars);
}

void trap_temp_high(void *general_id)
{
        snmp_alarm_register(0, 0, trap_temp_high_cb, general_id);
}

void trap_temp_too_high_cb(unsigned int clientreg, void *general_id)
{
    netsnmp_variable_list *notification_vars = NULL;

    snmp_varlist_add_variable(&notification_vars,
                              objid_snmptrap, OID_LENGTH(objid_snmptrap),
                              ASN_OBJECT_ID, (u_char *) isr_alrm_oid,
                              OID_LENGTH(objid_snmptrap) * sizeof(oid));

    char     isr_obj[MAX_LEN*2];
    snprintf(isr_obj, sizeof(isr_obj), "%s_%s", ar_general_config[0].node_id, (char*) general_id);
    snmp_varlist_add_variable(&notification_vars,
                               isr_object_oid, OID_LENGTH(isr_object_oid),
                               ASN_OCTET_STR,
                               isr_obj, strlen(isr_obj));

    static int      isr_servirity = CRITICAL;
    snmp_varlist_add_variable(&notification_vars,
                               isr_servirity_oid, OID_LENGTH(isr_servirity_oid),
                               ASN_INTEGER, (u_char *)&isr_servirity,
                                sizeof(isr_servirity));

    static int      isr_even_type_num = 1;
    snmp_varlist_add_variable(&notification_vars,
                               isr_even_type_num_oid, OID_LENGTH(isr_even_type_num_oid),
                               ASN_INTEGER, (u_char *)&isr_even_type_num,
                                sizeof(isr_even_type_num));

    static int      isr_even_type = 1;
    snmp_varlist_add_variable(&notification_vars,
                               isr_even_type_oid, OID_LENGTH(isr_even_type_oid),
                               ASN_INTEGER, (u_char *)&isr_even_type,
                                sizeof(isr_even_type));

    static int      isr_problem_num = 5;
    snmp_varlist_add_variable(&notification_vars,
                               isr_problem_num_oid, OID_LENGTH(isr_problem_num_oid),
                               ASN_INTEGER, (u_char *)&isr_problem_num,
                                sizeof(isr_problem_num));

    char     *isr_problem = "system alarm";
    snmp_varlist_add_variable(&notification_vars,
                               isr_problem_oid, OID_LENGTH(isr_problem_oid),
                               ASN_OCTET_STR,
                               isr_problem, strlen(isr_problem));

    char     *isr_add_text = "Temperature too high";
    snmp_varlist_add_variable(&notification_vars,
                               isr_add_text_oid, OID_LENGTH(isr_add_text_oid),
                               ASN_OCTET_STR,
                               isr_add_text, strlen(isr_add_text));

    char     *isr_even_time;
    isr_even_time = get_cur_time();
    snmp_varlist_add_variable(&notification_vars,
                               isr_even_time_oid, OID_LENGTH(isr_even_time_oid),
                               ASN_OCTET_STR,
                               isr_even_time, strlen(isr_even_time));

    static int      isr_alarm_state = 1;
    snmp_varlist_add_variable(&notification_vars,
                               isr_alarm_state_oid, OID_LENGTH(isr_alarm_state_oid),
                               ASN_INTEGER, (u_char *)&isr_alarm_state,
                                sizeof(isr_alarm_state));

    static int      count = 0;
    snmp_varlist_add_variable(&notification_vars,
                               isr_count_oid, OID_LENGTH(isr_count_oid),
                               ASN_INTEGER, (u_char *)&count,
                                sizeof(count));

    send_v2trap(notification_vars);
    snmp_free_varbind(notification_vars);
}

void trap_temp_too_high(void *general_id)
{
        snmp_alarm_register(0, 0, trap_temp_too_high_cb, general_id);
}

void trap_account_cb(unsigned int clientreg, void *msg)
{
    struct message *trap_msg;
    trap_msg = (struct message *) msg;

    netsnmp_variable_list *notification_vars = NULL;
    printf("inside socket_trap_cb \n");
    printf("============= inside trap key = %s \n", trap_msg->key);
    printf("============= inside trap val = %s \n", trap_msg->value);
    printf("============= inside trap type = %s \n", trap_msg->type);

    snmp_varlist_add_variable(&notification_vars,
                              objid_snmptrap, OID_LENGTH(objid_snmptrap),
                              ASN_OBJECT_ID, (u_char *) isr_alrm_oid,
                              OID_LENGTH(objid_snmptrap) * sizeof(oid));

    char     isr_obj[MAX_LEN*2];
    snprintf(isr_obj, sizeof(isr_obj), "%s_%s", ar_general_config[0].node_id, ar_account_obj_id[0].sip_object_id);
    snmp_varlist_add_variable(&notification_vars,
                               isr_object_oid, OID_LENGTH(isr_object_oid),
                               ASN_OCTET_STR,
                               isr_obj, strlen(isr_obj));

    static int      isr_servirity = MAJOR;
    snmp_varlist_add_variable(&notification_vars,
                               isr_servirity_oid, OID_LENGTH(isr_servirity_oid),
                               ASN_INTEGER, (u_char *)&isr_servirity,
                                sizeof(isr_servirity));

    static int      isr_even_type_num = 1;
    snmp_varlist_add_variable(&notification_vars,
                               isr_even_type_num_oid, OID_LENGTH(isr_even_type_num_oid),
                               ASN_INTEGER, (u_char *)&isr_even_type_num,
                                sizeof(isr_even_type_num));

    static int      isr_even_type = 1;
    snmp_varlist_add_variable(&notification_vars,
                               isr_even_type_oid, OID_LENGTH(isr_even_type_oid),
                               ASN_INTEGER, (u_char *)&isr_even_type,
                                sizeof(isr_even_type));

    static int      isr_problem_num = 31;
    snmp_varlist_add_variable(&notification_vars,
                               isr_problem_num_oid, OID_LENGTH(isr_problem_num_oid),
                               ASN_INTEGER, (u_char *)&isr_problem_num,
                                sizeof(isr_problem_num));

    char     isr_problem[MAX_LEN*2];
    snprintf(isr_problem, sizeof(isr_problem), "%s %s", (char*) trap_msg->key, (char*) trap_msg->value);
    snmp_varlist_add_variable(&notification_vars,
                               isr_problem_oid, OID_LENGTH(isr_problem_oid),
                               ASN_OCTET_STR,
                               isr_problem, strlen(isr_problem));

    char     *isr_add_text = (char*) trap_msg->value;
    snmp_varlist_add_variable(&notification_vars,
                               isr_add_text_oid, OID_LENGTH(isr_add_text_oid),
                               ASN_OCTET_STR,
                               isr_add_text, strlen(isr_add_text));

    char     *isr_even_time;
    isr_even_time = get_cur_time();
    snmp_varlist_add_variable(&notification_vars,
                               isr_even_time_oid, OID_LENGTH(isr_even_time_oid),
                               ASN_OCTET_STR,
                               isr_even_time, strlen(isr_even_time));

    static int      isr_alarm_state = 1;
    snmp_varlist_add_variable(&notification_vars,
                               isr_alarm_state_oid, OID_LENGTH(isr_alarm_state_oid),
                               ASN_INTEGER, (u_char *)&isr_alarm_state,
                                sizeof(isr_alarm_state));

    static int      count = 0;
    snmp_varlist_add_variable(&notification_vars,
                               isr_count_oid, OID_LENGTH(isr_count_oid),
                               ASN_INTEGER, (u_char *)&count,
                                sizeof(count));

    send_v2trap(notification_vars);
    snmp_free_varbind(notification_vars);
}

void trap_account(void *msg)
{
    snmp_alarm_register(0, 0, trap_account_cb, msg);
}

void trap_port_err_cb(unsigned int clientreg, void *msg)
{
    char *type;
    char *port;
    int index = 2; // This is posision of _ in tx_1 msg

    type = trim((char *)msg,0,index);
    //port = trim((char *)msg, index + 1, 4); // 4 is length of msg
    port = &msg[3];
    //printf("type = %s \n", type);
    //printf("port = %s \n", port);
    netsnmp_variable_list *notification_vars = NULL;

    snmp_varlist_add_variable(&notification_vars,
                              objid_snmptrap, OID_LENGTH(objid_snmptrap),
                              ASN_OBJECT_ID, (u_char *) isr_alrm_oid,
                              OID_LENGTH(objid_snmptrap) * sizeof(oid));

    char     isr_obj[MAX_LEN*2];
    snprintf(isr_obj, sizeof(isr_obj), "%s_%s", ar_general_config[0].node_id, ar_account_obj_id[0].sip_object_id);
    snmp_varlist_add_variable(&notification_vars,
                               isr_object_oid, OID_LENGTH(isr_object_oid),
                               ASN_OCTET_STR,
                               isr_obj, strlen(isr_obj));

    static int      isr_servirity = MAJOR;
    snmp_varlist_add_variable(&notification_vars,
                               isr_servirity_oid, OID_LENGTH(isr_servirity_oid),
                               ASN_INTEGER, (u_char *)&isr_servirity,
                                sizeof(isr_servirity));

    static int      isr_even_type_num = 1;
    snmp_varlist_add_variable(&notification_vars,
                               isr_even_type_num_oid, OID_LENGTH(isr_even_type_num_oid),
                               ASN_INTEGER, (u_char *)&isr_even_type_num,
                                sizeof(isr_even_type_num));

    static int      isr_even_type = 1;
    snmp_varlist_add_variable(&notification_vars,
                               isr_even_type_oid, OID_LENGTH(isr_even_type_oid),
                               ASN_INTEGER, (u_char *)&isr_even_type,
                                sizeof(isr_even_type));

    static int      isr_problem_num = 22;
    char            *isr_problem = "TX Frame Error";
    if (strcmp(type, "rx") == 0) {
        isr_problem_num = 23;
        isr_problem = "RX Frame Error";
    }

    snmp_varlist_add_variable(&notification_vars,
                               isr_problem_num_oid, OID_LENGTH(isr_problem_num_oid),
                               ASN_INTEGER, (u_char *)&isr_problem_num,
                                sizeof(isr_problem_num));
    snmp_varlist_add_variable(&notification_vars,
                               isr_problem_oid, OID_LENGTH(isr_problem_oid),
                               ASN_OCTET_STR,
                               isr_problem, strlen(isr_problem));

    char     isr_add_text[MAX_LEN*2];
    memset (isr_add_text, 0, MAX_LEN*2*sizeof(isr_add_text[0]));
    snprintf(isr_add_text, sizeof(isr_add_text), "%s %s %s", type, "frame error port", port);
    snmp_varlist_add_variable(&notification_vars,
                               isr_add_text_oid, OID_LENGTH(isr_add_text_oid),
                               ASN_OCTET_STR,
                               isr_add_text, strlen(isr_add_text));

    char     *isr_even_time;
    isr_even_time = get_cur_time();
    snmp_varlist_add_variable(&notification_vars,
                               isr_even_time_oid, OID_LENGTH(isr_even_time_oid),
                               ASN_OCTET_STR,
                               isr_even_time, strlen(isr_even_time));

    static int      isr_alarm_state = 1;
    snmp_varlist_add_variable(&notification_vars,
                               isr_alarm_state_oid, OID_LENGTH(isr_alarm_state_oid),
                               ASN_INTEGER, (u_char *)&isr_alarm_state,
                                sizeof(isr_alarm_state));

    static int      count = 0;
    snmp_varlist_add_variable(&notification_vars,
                               isr_count_oid, OID_LENGTH(isr_count_oid),
                               ASN_INTEGER, (u_char *)&count,
                                sizeof(count));

    send_v2trap(notification_vars);
    snmp_free_varbind(notification_vars);
}

void trap_port_err(void *msg)
{
    snmp_alarm_register(0, 0, trap_port_err_cb, msg);
}

void trap_power_cb(unsigned int clientreg, void *msg)
{
    // TODO: Day la trap danh cho system hoac power
    struct message *trap_msg;
    trap_msg = (struct message *) msg;

    netsnmp_variable_list *notification_vars = NULL;
    printf("inside socket_trap_cb \n");
    printf("============= inside trap key = %s \n", trap_msg->key);
    printf("============= inside trap val = %s \n", trap_msg->value);
    printf("============= inside trap type = %s \n", trap_msg->type);

    snmp_varlist_add_variable(&notification_vars,
                              objid_snmptrap, OID_LENGTH(objid_snmptrap),
                              ASN_OBJECT_ID, (u_char *) isr_alrm_oid,
                              OID_LENGTH(objid_snmptrap) * sizeof(oid));

    char     isr_obj[MAX_LEN*2];
    snprintf(isr_obj, sizeof(isr_obj), "%s_%s", ar_general_config[0].node_id, ar_account_obj_id[0].sip_object_id);
    snmp_varlist_add_variable(&notification_vars,
                               isr_object_oid, OID_LENGTH(isr_object_oid),
                               ASN_OCTET_STR,
                               isr_obj, strlen(isr_obj));

    static int      isr_servirity = MAJOR;
    snmp_varlist_add_variable(&notification_vars,
                               isr_servirity_oid, OID_LENGTH(isr_servirity_oid),
                               ASN_INTEGER, (u_char *)&isr_servirity,
                                sizeof(isr_servirity));

    static int      isr_even_type_num = 1;
    snmp_varlist_add_variable(&notification_vars,
                               isr_even_type_num_oid, OID_LENGTH(isr_even_type_num_oid),
                               ASN_INTEGER, (u_char *)&isr_even_type_num,
                                sizeof(isr_even_type_num));

    static int      isr_even_type = 1;
    snmp_varlist_add_variable(&notification_vars,
                               isr_even_type_oid, OID_LENGTH(isr_even_type_oid),
                               ASN_INTEGER, (u_char *)&isr_even_type,
                                sizeof(isr_even_type));

    static int      isr_problem_num = 31;
    snmp_varlist_add_variable(&notification_vars,
                               isr_problem_num_oid, OID_LENGTH(isr_problem_num_oid),
                               ASN_INTEGER, (u_char *)&isr_problem_num,
                                sizeof(isr_problem_num));

    char     isr_problem[MAX_LEN*2];
    snprintf(isr_problem, sizeof(isr_problem), "%s %s", (char*) trap_msg->key, (char*) trap_msg->value);
    snmp_varlist_add_variable(&notification_vars,
                               isr_problem_oid, OID_LENGTH(isr_problem_oid),
                               ASN_OCTET_STR,
                               isr_problem, strlen(isr_problem));

    char     *isr_add_text = (char*) trap_msg->value;
    snmp_varlist_add_variable(&notification_vars,
                               isr_add_text_oid, OID_LENGTH(isr_add_text_oid),
                               ASN_OCTET_STR,
                               isr_add_text, strlen(isr_add_text));

    char     *isr_even_time;
    isr_even_time = get_cur_time();
    snmp_varlist_add_variable(&notification_vars,
                               isr_even_time_oid, OID_LENGTH(isr_even_time_oid),
                               ASN_OCTET_STR,
                               isr_even_time, strlen(isr_even_time));

    static int      isr_alarm_state = 1;
    snmp_varlist_add_variable(&notification_vars,
                               isr_alarm_state_oid, OID_LENGTH(isr_alarm_state_oid),
                               ASN_INTEGER, (u_char *)&isr_alarm_state,
                                sizeof(isr_alarm_state));

    static int      count = 0;
    snmp_varlist_add_variable(&notification_vars,
                               isr_count_oid, OID_LENGTH(isr_count_oid),
                               ASN_INTEGER, (u_char *)&count,
                                sizeof(count));

    send_v2trap(notification_vars);
    snmp_free_varbind(notification_vars);
}

void trap_power(void *msg)
{
    snmp_alarm_register(0, 0, trap_power_cb, msg);
}