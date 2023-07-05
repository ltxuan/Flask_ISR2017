#ifndef SUBAGENTOBJECT_H
#define SUBAGENTOBJECT_H

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/library/container.h>
#include <net-snmp/agent/table_array.h>
#include "read_config_snmp.h"

#define CRITICAL    1
#define MAJOR       2
#define MINOR       3
#define WARNING     4
#define CLEAR       5

struct config_inventory ar_config_inventory[1];
struct config_port_eth ar_config_eth[NUM_PORT_ETH];
struct account_obj_id ar_account_obj_id[NUM_OBJ_ID];
struct performance ar_performance[1];
struct general_config ar_general_config[1];

/*================================== v7isrMachineInterface ====================================*/
extern  int     port1_stat;
extern  int     port2_stat;
extern  int     port1_id;
extern  int     port2_id;
static oid      pwr_state_oid[]             = { 1, 3, 6, 1, 4, 1, 2021, 1, 4, 1, 0 };
static oid      ge1_state_oid[]             = { 1, 3, 6, 1, 4, 1, 2021, 1, 4, 2, 0 };
static oid      ge2_state_oid[]             = { 1, 3, 6, 1, 4, 1, 2021, 1, 4, 3, 0 };

/*=================================== v7isrPerformance ========================================*/
static oid      ram_oid[]                   = { 1, 3, 6, 1, 4, 1, 2021, 1, 5, 1 };
static oid      percent_ram_oid[]            = { 1, 3, 6, 1, 4, 1, 2021, 1, 5, 2, 0 };
static oid      cpu_oid[]                   = { 1, 3, 6, 1, 4, 1, 2021, 1, 5, 3 };
static oid      percent_cpu_oid[]            = { 1, 3, 6, 1, 4, 1, 2021, 1, 5, 4, 0 };
static oid      performance_table_oid[]     = { 1, 3, 6, 1, 4, 1, 2021, 1, 5, 5 };

/*======================================= v7isrAlarm ==========================================*/
void            trap_start(void *general_id);
void            trap_port_up(void *port);
void            trap_port_down( void *port);

void            trap_ram_high(void *general_id);
void            trap_ram_too_high(void *general_id);

void            trap_temp_high(void *general_id);
void            trap_temp_too_high(void *general_id);

void            trap_cpu_high(void *general_id);
void            trap_cpu_too_high(void *general_id);
void            trap_account(void *msg);

void            trap_port_err(void *msg);

void            trap_power(void *msg);

void            update_cpu_stat(int cpu_stat);
void            update_ram_stat(int cpu_stat);
void            update_interface(int port1_stat, int port2_stat, int port3_stat, int port4_stat);

static oid      objid_snmptrap[]            = { 1, 3, 6, 1, 6, 3, 1, 1, 4, 1, 0 }; /* special OID */
static oid      isr_alrm_oid[]              = { 1, 3, 6, 1, 4, 1, 2021, 1, 6};     /* OID of folder */

static oid      isr_object_oid[]            = { 1, 3, 6, 1, 4, 1, 2021, 1, 6, 1, 0 };
static oid      isr_servirity_oid[]         = { 1, 3, 6, 1, 4, 1, 2021, 1, 6, 2, 0 };
static oid      isr_even_type_num_oid[]     = { 1, 3, 6, 1, 4, 1, 2021, 1, 6, 3, 0 };
static oid      isr_even_type_oid[]         = { 1, 3, 6, 1, 4, 1, 2021, 1, 6, 4, 0 };
static oid      isr_problem_num_oid[]       = { 1, 3, 6, 1, 4, 1, 2021, 1, 6, 5, 0 };
static oid      isr_problem_oid[]           = { 1, 3, 6, 1, 4, 1, 2021, 1, 6, 6, 0 };
static oid      isr_add_text_oid[]          = { 1, 3, 6, 1, 4, 1, 2021, 1, 6, 7, 0 };
static oid      isr_even_time_oid[]         = { 1, 3, 6, 1, 4, 1, 2021, 1, 6, 8, 0 };
static oid      isr_alarm_state_oid[]       = { 1, 3, 6, 1, 4, 1, 2021, 1, 6, 9, 0 };
static oid      isr_count_oid[]             = { 1, 3, 6, 1, 4, 1, 2021, 1, 6, 10, 0 };
#endif                          /* SUBAGENTOBJECT_H */
