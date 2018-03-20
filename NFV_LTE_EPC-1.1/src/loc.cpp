#include "loc.h"
#include "discover.h"
#include <stdlib.h>

string g_trafmon_ip_addr = "10.129.26.175";
string g_mme_ip_addr = "0.0.0.0";

string g_hss_ip_addr = resolve_host("hss"); 
string g_sgw_s11_ip_addr = resolve_host("sgw");
string g_sgw_s1_ip_addr = resolve_host("sgw");
string g_sgw_s5_ip_addr = resolve_host("sgw");
string g_pgw_s5_ip_addr = resolve_host("pgw");

const string dsmme_path = resolve_host("mme-ds") + ":8090"; 

int g_trafmon_port = 4000;
int g_mme_port = 5000;
int g_hss_port = 6000;
int g_sgw_s11_port = 7000;
int g_sgw_s1_port = 7100;
int g_sgw_s5_port = 7200;
int g_pgw_s5_port = 8000;

uint64_t g_timer = 100;

int dummt;

void count(int n) {
  for (int i = 0; i < n; i++) {
    dummt++;
  }

  dummt = dummt % 102;
}

void load_func() {
  cout << "LOADING" << endl;
  count(1000000 * (rand() % 3));
}



UeContext::UeContext() {
	emm_state = 0;
	ecm_state = 0;
	imsi = 0;
	string ip_addr = "";
	enodeb_s1ap_ue_id = 0;
	mme_s1ap_ue_id = 0;
	tai = 0;
	tau_timer = 0;
	ksi_asme = 0;
	k_asme = 0; 
	k_nas_enc = 0; 
	k_nas_int = 0; 
	nas_enc_algo = 0; 
	nas_int_algo = 0; 
	count = 1;
	bearer = 0;
	dir = 1;
	default_apn = 0; 
	apn_in_use = 0; 
	eps_bearer_id = 0; 
	e_rab_id = 0;
	s1_uteid_ul = 0; 
	s1_uteid_dl = 0; 
	s5_uteid_ul = 0; 
	s5_uteid_dl = 0; 
	xres = 0;
	nw_type = 0;
	nw_capability = 0;	
	pgw_s5_ip_addr = "";	
	pgw_s5_port = 0;
	s11_cteid_mme = 0;
	s11_cteid_sgw = 0;	
}
//making serializable
template<class Archive>
void UeContext::serialize(Archive &ar, const unsigned int version)
{
	ar & emm_state & ecm_state & imsi & ip_addr & enodeb_s1ap_ue_id & mme_s1ap_ue_id & tai & tai_list & tau_timer & ksi_asme & k_asme & k_enodeb & k_nas_enc & k_nas_int & nas_enc_algo & nas_int_algo & count & bearer & dir & default_apn & apn_in_use & eps_bearer_id & e_rab_id & s1_uteid_ul & s1_uteid_dl & s5_uteid_ul & s5_uteid_dl & xres & nw_type & nw_capability & pgw_s5_ip_addr & pgw_s5_port & s11_cteid_mme & s11_cteid_sgw;
}

template<class Archive>
void Mme_state::serialize(Archive &ar, const unsigned int version)
{
	ar & Mme_state_uect & guti;
}

void UeContext::init(uint64_t arg_imsi, uint32_t arg_enodeb_s1ap_ue_id, uint32_t arg_mme_s1ap_ue_id, uint64_t arg_tai, uint16_t arg_nw_capability) {
	imsi = arg_imsi;
	enodeb_s1ap_ue_id = arg_enodeb_s1ap_ue_id;
	mme_s1ap_ue_id = arg_mme_s1ap_ue_id;
	tai = arg_tai;
	nw_capability = arg_nw_capability;
}

UeContext::~UeContext() {

}

MmeIds::MmeIds() {
	mcc = 1;
	mnc = 1;
	plmn_id = g_telecom.get_plmn_id(mcc, mnc);
	mmegi = 1;
	mmec = 1;
	mmei = g_telecom.get_mmei(mmegi, mmec);
	gummei = g_telecom.get_gummei(plmn_id, mmei);
}

MmeIds::~MmeIds() {

}

Mme::Mme() {

	clrstl();
	g_sync.mux_init(s1mmeid_mux);
	g_sync.mux_init(uectx_mux);



}
void Mme::initialize_kvstore_clients(int workers_count){

}
void Mme::clrstl() {

}

uint32_t Mme::get_s11cteidmme(uint64_t guti) {
	uint32_t s11_cteid_mme;
	string tem;

	tem = to_string(guti);
	tem = tem.substr(7, -1); /* Extracting only the last 9 digits of UE MSISDN */
	s11_cteid_mme = stoull(tem);
	return s11_cteid_mme;
}

void Mme::handle_initial_attach(int conn_fd, Packet pkt, SctpClient &hss_client, int worker_id) {
  load_func();
}

UeContext Mme::get_context(uint64_t guti){
	g_sync.mlock(uectx_mux);

	if (ue_ctx.find(guti) != ue_ctx.end()) {
		return ue_ctx[guti];
	}
	g_sync.munlock(uectx_mux);
}
void Mme::pull_context(Packet pkt,int worker_id){

	uint64_t mme_s1ap_ue_id;
	uint64_t guti;
	mme_s1ap_ue_id = pkt.s1ap_hdr.mme_s1ap_ue_id;

	Mme_state mme_state;
	auto bundle = ds_mme_state[worker_id].get(mme_s1ap_ue_id);
	if(bundle.ierr<0){

		g_utils.handle_type1_error(-1, "datastore retrieval error: pull_context");

	}else {

		mme_state =  (bundle.value);

	}

	g_sync.mlock(uectx_mux);
	guti = (mme_state).guti;
	ue_ctx[guti] = (mme_state).Mme_state_uect;
	g_sync.munlock(uectx_mux);

	g_sync.mlock(s1mmeid_mux);
	s1mme_id[mme_s1ap_ue_id] = guti;
	g_sync.munlock(s1mmeid_mux);


}
void Mme::push_context(uint64_t guti,UeContext local_ue_ctx,uint32_t mme_s1ap_ue_id,int worker_id){

	Mme_state state_data;
	state_data = Mme_state(local_ue_ctx,guti);
	auto bundle = ds_mme_state[worker_id].put(mme_s1ap_ue_id,state_data);
	if(bundle.ierr<0){			g_utils.handle_type1_error(-1, "datastore push error: push_context");
	}

}
void Mme::erase_context(uint32_t mme_s1ap_ue_id,int worker_id){

	ds_mme_state[worker_id].del(mme_s1ap_ue_id);

}
bool Mme::handle_autn(int conn_fd, Packet pkt, int worker_id) {
}

void Mme::handle_security_mode_cmd(int conn_fd, Packet pkt, int worker_id) {
  load_func();
}

void Mme::set_crypt_context(UeContext &local_ue_ctx) {
	local_ue_ctx.nas_enc_algo = 1;
	local_ue_ctx.k_nas_enc = local_ue_ctx.k_asme + local_ue_ctx.nas_enc_algo + local_ue_ctx.count + local_ue_ctx.bearer + local_ue_ctx.dir;
}

void Mme::set_integrity_context(UeContext &local_ue_ctx) {
	local_ue_ctx.nas_int_algo = 1;
	local_ue_ctx.k_nas_int = local_ue_ctx.k_asme + local_ue_ctx.nas_int_algo + local_ue_ctx.count + local_ue_ctx.bearer + local_ue_ctx.dir;
}

bool Mme::handle_security_mode_complete(int conn_fd, Packet pkt, int worker_id) {
  load_func();
}

void Mme::handle_location_update(Packet pkt, SctpClient &hss_client, int worker_id) {
  load_func();
	uint64_t guti;
	uint64_t imsi;
	uint64_t default_apn;

	guti = get_guti(pkt);
	if (guti == 0) {
		TRACE(cout << "mme_handlelocationupdate:" << " zero guti " << pkt.s1ap_hdr.mme_s1ap_ue_id << " " << pkt.len << ": " << guti << endl;		)
												g_utils.handle_type1_error(-1, "Zero guti: mme_handlelocationupdate");
	}
	UeContext local_ue_ctx;
	g_sync.mlock(uectx_mux);
	local_ue_ctx = ue_ctx[guti];
	g_sync.munlock(uectx_mux);
	imsi = local_ue_ctx.imsi;

	g_sync.mlock(uectx_mux);
	local_ue_ctx = ue_ctx[guti];
	g_sync.munlock(uectx_mux);

	imsi = local_ue_ctx.imsi;

	pkt.clear_pkt();
	pkt.append_item(imsi);
	pkt.append_item(mme_ids.mmei);
	pkt.prepend_diameter_hdr(2, pkt.len);
	hss_client.snd(pkt);
	TRACE(cout << "mme_handlelocationupdate:" << " loc update sent to hss: " << guti << endl;)

	hss_client.rcv(pkt);
	TRACE(cout << "mme_handlelocationupdate:" << " loc update response received from hss: " << guti << endl;)

	pkt.extract_diameter_hdr();
	pkt.extract_item(default_apn);



	local_ue_ctx.default_apn = default_apn;
	local_ue_ctx.apn_in_use = local_ue_ctx.default_apn;



	g_sync.mlock(uectx_mux);
	ue_ctx[guti] = local_ue_ctx;
	g_sync.munlock(uectx_mux);

}

void Mme::handle_create_session(int conn_fd, Packet pkt, UdpClient &sgw_client, int worker_id) {
  load_func();
}

void Mme::handle_attach_complete(Packet pkt, int worker_id) {
  load_func();
}

void Mme::handle_modify_bearer(int conn_fd,Packet pkt, UdpClient &sgw_client, int worker_id) {
  load_func();
}

void Mme::handle_detach(int conn_fd, Packet pkt, UdpClient &sgw_client, int worker_id) {
  load_func();
}

void Mme::set_pgw_info(uint64_t guti) {

	UeContext local_ue_ctx;
	g_sync.mlock(uectx_mux);
	local_ue_ctx = ue_ctx[guti];;
	local_ue_ctx.pgw_s5_port = g_pgw_s5_port;
	local_ue_ctx.pgw_s5_ip_addr = g_pgw_s5_ip_addr;
	ue_ctx[guti] = local_ue_ctx;
	g_sync.munlock(uectx_mux);
}

uint64_t Mme::get_guti(Packet pkt) {
	uint64_t mme_s1ap_ue_id;
	uint64_t guti;
	mme_s1ap_ue_id = pkt.s1ap_hdr.mme_s1ap_ue_id;

	guti = 0;

	g_sync.mlock(s1mmeid_mux);

	if (s1mme_id.find(mme_s1ap_ue_id) != s1mme_id.end()) {
		guti = s1mme_id[mme_s1ap_ue_id];
	}
	g_sync.munlock(s1mmeid_mux);
	return guti;
}



void Mme::rem_itfid(uint32_t mme_s1ap_ue_id) {
	g_sync.mlock(s1mmeid_mux);
	s1mme_id.erase(mme_s1ap_ue_id);
	g_sync.munlock(s1mmeid_mux);
}

void Mme::rem_uectx(uint64_t guti) {
	g_sync.mlock(uectx_mux);
	ue_ctx.erase(guti);
	g_sync.munlock(uectx_mux);
}

Mme::~Mme() {

}
Mme_state::Mme_state(UeContext ue,uint64_t guti_v){

	Mme_state_uect = ue;
	guti = guti_v;
}
Mme_state::Mme_state(){
}
