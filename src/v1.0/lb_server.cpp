#include "lb_server.h"

auto registry = std::make_shared<prometheus::Registry>();
auto& cf1 = prometheus::BuildCounter()
							.Name("packets_handled_total").Help("").Register(*registry);
auto& packs_tot = cf1.Add({});

MmeLB g_lbmme;

unordered_map<int, SctpClient> conn_map;
unordered_map<int, SctpClient> loc_conn;
Packet loc_packet;

void check_usage(int argc) {
	if (argc != 1) {
		TRACE(cout << "Usage: ./<lb_server_exec>" << endl;)
		g_utils.handle_type1_error(-1, "Invalid usage error: mmeserver_checkusage");
	}
}

void init(char *argv[]) {
	signal(SIGPIPE, SIG_IGN);
}

void run() {
	int i;
	
	TRACE(cout << "MME server started" << endl;)
  loc_packet.append_item(true);
	g_lbmme.server.run(g_lb_ip_addr, g_lb_port, 30, handle_ue);
}

void hit_loc() {
  string ip_addr = resolve_host2("location-update");
  if (ip_addr == "") {
    return;
  }
  SctpClient conn;
  conn.conn(ip_addr, 5000);
  Packet pkt;
  pkt.append_item(true);
  conn.snd(pkt);
}

int handle_ue(int conn_fd, int) {
  packs_tot.Increment();
	bool res;
	Packet pkt;
  //SctpClient mme_client;


	g_lbmme.server.rcv(conn_fd, pkt);
	if (pkt.len <= 0) {
		TRACE(cout << "mmeserver_handleue:" << " Connection closed" << endl;)
		return 0;
	}

  pkt.extract_s1ap_hdr();
  //if (conn_fd % 2 == 0) {
    //string mme_ip_addr = resolve_host2("mme1");
    //int mme_port = 5000;
    //mme_client.conn(mme_ip_addr, mme_port);
  //} else {
    //string mme_ip_addr = resolve_host2("mme2");
    //int mme_port = 5000;
    //mme_client.conn(mme_ip_addr, mme_port);
  //}
    
  string mme_ip_addr;
  string loc_ip_addr;
  int mme_port = 5000;

  if (pkt.s1ap_hdr.mme_s1ap_ue_id == 0) {
    switch (pkt.s1ap_hdr.msg_type) {
      //[> Initial Attach request <]
      case 1: 
        conn_map.erase(conn_fd);
        mme_ip_addr = resolve_host2("mme-attach");
        conn_map[conn_fd].conn(mme_ip_addr, mme_port);

        loc_conn.erase(conn_fd);
        loc_ip_addr = resolve_host2("location-update");
        loc_conn[conn_fd].conn(loc_ip_addr, mme_port);
        loc_conn[conn_fd].snd(loc_packet);



        conn_map[conn_fd].snd(pkt);
        conn_map[conn_fd].rcv(pkt);
        g_lbmme.server.snd(conn_fd, pkt);
        TRACE(cout << "mmeserver_handleue:" << " case 1: initial attach" << endl;)
        break;

      //[> For error handling <]
      default:
        TRACE(cout << "mmeserver_handleue:" << " default case: new" << endl;)
        break;
    }		
  }
  else if (pkt.s1ap_hdr.mme_s1ap_ue_id > 0) {
    switch (pkt.s1ap_hdr.msg_type) {
      //[> Authentication response <]
      case 2: 
        loc_conn[conn_fd].snd(loc_packet);
        conn_map[conn_fd].snd(pkt);
        conn_map[conn_fd].rcv(pkt);
        g_lbmme.server.snd(conn_fd, pkt);
        TRACE(cout << "mmeserver_handleue:" << " case 2: authentication response" << endl;)
        break;

      //[> Security Mode Complete <]
      case 3: 
        loc_conn[conn_fd].snd(loc_packet);
        conn_map[conn_fd].snd(pkt);
        conn_map[conn_fd].rcv(pkt);
        g_lbmme.server.snd(conn_fd, pkt);
        TRACE(cout << "mmeserver_handleue:" << " case 3: security mode complete" << endl;)
        break;

      //[> Attach Complete <]
      case 4: 
        loc_conn[conn_fd].snd(loc_packet);
        conn_map[conn_fd].snd(pkt);
        conn_map[conn_fd].rcv(pkt);
        g_lbmme.server.snd(conn_fd, pkt);
        TRACE(cout << "mmeserver_handleue:" << " case 4: attach complete" << endl;)
        break;

      //[> Detach request <]
      case 5: 
        conn_map.erase(conn_fd);
        loc_conn.erase(conn_fd);
        //if (conn_fd % 2 == 0) {
          //string mme_ip_addr = resolve_host2("mme-dist");
          //int mme_port = 5000;
          //conn_map[conn_fd].conn(mme_ip_addr, mme_port);
        //} else {
          //string mme_ip_addr = resolve_host2("mme-dist");
          //int mme_port = 5000;
          //conn_map[conn_fd].conn(mme_ip_addr, mme_port);
        //}
        //
        mme_ip_addr = resolve_host2("mme-detach");
        conn_map[conn_fd].conn(mme_ip_addr, mme_port);

        conn_map[conn_fd].snd(pkt);
        conn_map[conn_fd].rcv(pkt);
        g_lbmme.server.snd(conn_fd, pkt);
        TRACE(cout << "mmeserver_handleue:" << " case 5: detach request" << endl;)
        break;

      //[> For error handling <]	
      default:
        TRACE(cout << "mmeserver_handleue:" << " default case: attached" << endl;)
        break;
    }				
  }		

  hit_loc();
	return 1;
}

int main(int argc, char *argv[]) {
  prometheus::Exposer exposer("0.0.0.0:8081");
  exposer.RegisterCollectable(registry);

	check_usage(argc);
	init(argv);
	run();
	return 0;
}
