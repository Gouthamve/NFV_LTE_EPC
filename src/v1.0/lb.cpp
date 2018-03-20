#include "lb.h"
#include "discover.h"


string g_lb_ip_addr = "0.0.0.0"; 
string g_hss_ip_addr = resolve_host("hss");

int g_lb_port = 5000;
int g_hss_port = 6000;

uint64_t g_timer = 100;

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

MmeLB::MmeLB() {
}

//SctpClient MmeLB::get_conn(int up_fd) {
  //if (conn_map.find(up_fd) == conn_map.end()) {
    //string mme_ip_addr = resolve_host("mme1");
    //int mme_port = 5000;
    //conn_map[up_fd] = SctpClient(); 
    //conn_map[up_fd].conn(mme_ip_addr, mme_port);
  //}

  //return conn_map[up_fd];
//}

//void MmeLB::purge_conn(int up_fd) {
  //SctpClient x = conn_map[up_fd];
  //x.~SctpClient();
  //conn_map.erase(up_fd);
//}

MmeLB::~MmeLB() {

}

