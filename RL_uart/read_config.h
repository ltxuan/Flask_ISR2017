#include <pjsua-lib/pjsua.h>

#define NUM_CODEC 13

struct call_data
{
	pj_pool_t           *pool;
	pjmedia_conf        *conf;
	pjmedia_port        *cport;
	pjmedia_port        *tonegenleft;
	pjmedia_port        *tonegenright;
	int                 call_slot_0, toneslotleft;
	int 		    call_slot_1, toneslotright;	
};

struct codec_setting
{
	char codec_name[100];
	pj_uint8_t 	codec_pri;
};

struct setting
{
	unsigned int use_srtp;
};
struct dial_plan{
	unsigned char digit1;
	unsigned char digit2;
	unsigned char digit3;
	unsigned char digit4;
	unsigned char digit5;
	unsigned char digit6;
	unsigned char digit7;
	unsigned char digit8;
	unsigned char digit9;
	unsigned char digit0;
};

char echo_enable[3];

struct config{
	/* sip */
	int sip_port;
	int sip_tcp_port;
	int sip_tls_port;
	int inc_timeout;
	int in_call_timeout;
	int delayed_timeout;
	/* rtp */
	int audio_rtp_port;
	int text_rtp_port;

	/* sound */
	int sound_id;
	char sound_name[100];
	int sound_inited;
  	int sound_is_using;

	/* proxy */
	char reg_proxy[100];
	char reg_identity[100];

	/* auth_info */
	char username[100];
	char secret[100];
	char ha1[100];
	char realm[100];
	char domain[100];
	char status[10];
	char h_status[10];
	char transport[10];
	char hotline[100];
	int state;
	pj_str_t call_number;
	pjmedia_port *sc, *sc_2;
	pjmedia_port* rev;
	pjmedia_port* rev_1;
	int slot, slot_1, slot_cap, slot_cap2;
	pjmedia_snd_port *snd_port;
	pjsua_acc_id acc_id;
	pjsua_call_id call_id;
	/* add pjmedia conf */
	pjmedia_conf *media_conf;
	pjmedia_port *mp, *medpcap, *medpcap2;
	int p_slot;
	struct call_data *cd;

};
void del_line(char *src);
int find(char *src, char *key);
char *trim(char *src, int start, int end);
void read_config(struct config *ar_config, struct codec_setting *ar_codec_setting, struct setting *app_setting, struct dial_plan *dial_plan, char *dir);

