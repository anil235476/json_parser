#include "json_parser.h"
#include <json.hpp>
#include <cassert>
#include <map>
#include <thread>
#include <iostream>

constexpr const char* TYPE{ "type" };
constexpr const char* REG_USR_REQ{ "register_user" };
constexpr const char* REG_USR_REQ_RES{ "register_user_response" };

constexpr const char* STATUS{ "status" };
constexpr const char* OKAY{ "ok" };
constexpr const char* OKAY_{ "okay" };
constexpr const char* SUCESS{ "success" };
constexpr const char* ERR{ "error" };
constexpr const char* NAME{ "name" };
constexpr const char* CON_STATUS{ "connection_status" };
constexpr const char* CONNECTED{ "connected" };
constexpr const char* PRESENCE_NOTIFICATION{ "presence_notification" };
constexpr const char* KEY{ "key" };

constexpr const char* REQ_ICE_CANDIDATES{ "req_ice_candidates" };
constexpr const char* ICE_CANDIDATES_REQ_RES{ "res_ice_candidates" };
constexpr const char* SIGNALLING_SERVER_URL{ "signalling_server_url" };
constexpr const char* DETAIL{ "detail" };
constexpr const char* URL{ "url" };

constexpr const char* FROM{ "from" };
constexpr const char* TO{ "to" };
constexpr const char* PEER_MSG_KEY{ "m" };
constexpr const char* PEER_OFFER{ "offer" };
constexpr const char* PEER_ANS{ "answer" };
constexpr const char* PEER_ICE{ "iceCandidates" };
constexpr const char* PEER_CALL_REQ{ "call_request" };
constexpr const char* PEER_CALL_RES{ "call_request_res" };
constexpr const char* PEER_CALL_REQ_NATIVE{ "call_request_native" };
constexpr const char* CALL_NOTIFICATION{ "in_call_notification" };
constexpr const char* RES_OWNER_ROOM{ "response_owner_rooms" };

constexpr const char* AUDIO_CALL{ "audio" };
constexpr const char* VIDEO_CALL{ "video" };
constexpr const char* SDP{ "sdp" };
constexpr const char* CANDIDATE_SDP{ "candidate" };
constexpr const char* MID{ "sdpMid" };
constexpr const char* MINDEX{ "sdpMLineIndex" };
constexpr const char* IP{ "ip" };
constexpr const char* PORT{ "port" };
constexpr const char* CREATE_CONNECTION{ "create_peer" };
constexpr const char* REMOVE_CONNECTION{ "remove_peer" };
constexpr const char* SHOW_MSG{ "show_message" };
constexpr const char* LOGIN_RES{ "login_response" };
constexpr const char* LOGIN_REQ{ "login_req" };
constexpr const char* USR{ "usr" };
constexpr const char* PWD{ "pwd" };

constexpr const char* CALL_RES_EVNT{ "call_response_event" };
constexpr const char* SIGNALLING_SERV_REQ{ "request_signalling_server" };
constexpr const char* SIGNALLING_SERV_REQ_RES{ "response_siganalling_server" };
//constexpr const char* CONNECTION_DISCONNECT{ "connection_disconnect" };
constexpr const char* CONNECTION_ERROR{ "connection_error" };
constexpr const char* CHAT_MSG_TYPE{ "chat_message" };
constexpr const char* PEER_ID{ "peerId" };
constexpr const char* PRODUCER_ID{ "producerId" };
constexpr const char* REQ_ID{ "reqId" };
constexpr const char* RES_ID{ "resId" };
constexpr const char* KIND{ "kind" };

using json = nlohmann::json;
namespace grt {

		void to_json(json& j, const window_info& p) {
			j = json{ {"id", p.id_}, {"prioirty", p.is_prority_}, {"active", p.is_active_}, {"name", p.name_} };
		}

		void from_json(const json& j, window_info& p) {
			j.at("id").get_to(p.id_);
			j.at("prioirty").get_to(p.is_prority_);
			j.at("active").get_to(p.is_active_);
			j.at("name").get_to(p.name_);
		}
	
	namespace detail {

		std::string 
			to_string(message_type type) {
			if (type == message_type::connection_error) return CONNECTION_ERROR;
			else {
				assert(false);
			}
			return std::string{}; //it is just to remove warning.
		}

		//std::string get_key_value(std::string json, std::string key) {
		//	ptree pt;
		//	boost::property_tree::json_parser::read_json(std::stringstream{ json }, pt);
		//	const auto value = pt.get_child(key).get_value<std::string>();
		//	return value;
		//}

		bool
			is_status_okay(std::string const status) {
			return (status == OKAY || status == OKAY_ || status == SUCESS);
		}

		std::string 
			convert_to_success(bool const status) {
			return status ? SUCESS : ERR;
		}

		bool is_call_accepted(std::string status) {
			return (status == "yes");
		}

		std::string to_string(bool is_accept) {
			return is_accept?"yes" : "no";
		}


		user_status to_user_status(std::string status) {
			return (status == CONNECTED) ? user_status::online : user_status::offline;
		}

		std::string to_connection_status_str(bool status) {
			return status ? CONNECTED : ERR;
		}

		std::string to_string(user_status status) {
			return status == user_status::online ? CONNECTED : ERR;
		}
		 

		std::string
			get_type(std::string json) {
			const auto json_msg = json::parse(json);
			const std::string type = json_msg[TYPE];
			return type;
		}

		/*std::string get_register_user_name(std::string json) {
			const auto name = get_key_value(json, NAME);
			return name;
		}
*/
		call_type to_call_type(std::string const detail) {
			if (detail == VIDEO_CALL) return call_type::VIDEO;
			if (detail == AUDIO_CALL) return call_type::AUDIO;
			assert(false);
			return call_type::unknown;
		};

		std::string to_string(call_type type) {
			switch (type) {
			case call_type::AUDIO: return AUDIO_CALL;
			case call_type::VIDEO: return VIDEO_CALL;
			default:
				assert(false);

			return "unknown";
			}
		}
	
		std::vector<grt::room_info> 
			parse_to_room_info(std::string const& msg){
			const auto json_msg = json::parse(msg);
			const auto type = json_msg[TYPE];
			assert(type == "response_room_info");
			const int count = json_msg["count"];
			const auto& ids = json_msg["ids"];
			const auto& names = json_msg["names"];
			std::vector<std::string> id_vec;
			for (const std::string& elm : ids) {
				id_vec.push_back(elm);
			}
			assert(id_vec.size() == count);

			std::vector<std::string> name_vec;
			for (const std::string& elm : names) {
				name_vec.push_back(elm);
			}
			assert(name_vec.size() == count);
			std::vector<room_info> output(count);
			std::transform(id_vec.begin(), id_vec.end(), names.begin(), output.begin(),
				[](const std::string& id, const std::string& name) {
				room_info out; out.id_ = id; out.name_ = name;
				return out;
			});
			return output;
		}

		void _parse(std::string msg, parser_callback* caller) {
			assert(caller);
			const auto json_msg = json::parse(msg);
			const auto type = json_msg[TYPE];
			if (type == REG_USR_REQ) {
				//assert(false);
				const std::string name = json_msg[NAME];
				caller->on_message(message_type::user_registration_req, name);
			}
			else if (type == REG_USR_REQ_RES) {
				const auto status = json_msg[STATUS]; //get_key_value(msg, STATUS);
				const std::string id = json_msg[ID];// get_key_value(msg, ID);
				const std::string userName = json_msg[NAME];
				const std::string roomId = json_msg[ROOM_ID]; // todo ??
				const std::string error = json_msg[ERR];
				const bool is_admin = json_msg["admin"];

				caller->on_message(message_type::user_registeration_res,
					registration_res{ status == OKAY, {userName, id, roomId, error} }, msg);
			}
			else if (type == ICE_CANDIDATES_REQ_RES) {
				//assert(false);
				const std::string status = json_msg[STATUS];
				if (is_status_okay(status)) {
					const std::string detail = json_msg[DETAIL]; 
					caller->on_ice_servers_req_res(true, detail);
				}
				else {
					caller->on_ice_servers_req_res(false, std::string{});
				}
			}
			else if (type == PRESENCE_NOTIFICATION) {
				//assert(false);
				const std::string peer_msg = json_msg[PEER_MSG_KEY];
				const auto notify_msg = json::parse(peer_msg);
				const std::string prsence_type = notify_msg[TYPE];
				const std::string id = notify_msg[KEY];
				const std::string name = notify_msg.value(NAME, std::string{});
				presence_notification const obj{ id, name, to_user_status(prsence_type) };
				caller->on_message(message_type::presence_notification, obj, msg);
			}
			else if (type == FORWARD_MSG_TYPE_KEY) {
				caller->on_message(message_type::forward_message, msg);
			}
			else if (type == SIGNALLING_SERVER_URL) {
				//assert(false);
				const std::string ip = json_msg[IP];
				const std::string port = json_msg[PORT];
				caller->on_signalling_url(ip, port);
			}
			else if (type == CREATE_CONNECTION) {
				//assert(false);
				const  std::string id = json_msg[ID];// get_key_value(msg, ID);
				const  std::string url = json_msg[URL];// get_key_value(msg, URL);
				caller->on_message(grt::message_type::create_peer, callee_address{ url, id });
			}
			else if (type == REMOVE_CONNECTION) {
				//assert(false);
				const std::string id = json_msg[ID];
				caller->on_message(grt::message_type::remove_peer, id);
			}
			else if (type == LOGIN_RES) {
				std::cout << "login response receviced\n";
				const std::string status = json_msg[STATUS];
				const std::string ip = json_msg.value(IP, std::string{});
				const std::string port = json_msg.value(PORT, std::string{});
				caller->on_message(message_type::login_res,
					login_res{ is_status_okay(status), ip, port });
				//assert(false);
			}
			else if (type == LOGIN_REQ) {
				//assert(false);
				const auto m = json_msg[PEER_MSG_KEY];

				const std::string server = m["server"];// get_key_value(msg, IP);
				const std::string usr = m["user"];
				const std::string pwd = m[PWD];
				caller->on_message(message_type::login_req,
					login_req{ usr, pwd, server });
			}
			else if (type == CALL_RES_EVNT) {
				//assert(false);
				//const auto 
			const std::string status = json_msg[STATUS];
			const bool is_accepted = is_call_accepted(status);
			const std::string remote_id = json_msg[FROM];
			const std::string self_id = json_msg[ID];
			const std::string detail =  VIDEO_CALL;// json_msg[DETAIL];

			caller->on_message(message_type::call_res_evt, 
				call_res_event{to_call_type(detail),remote_id, self_id, is_accepted });

			}
			else if (type == SIGNALLING_SERV_REQ_RES) {
				//assert(false);
				const std::string status = json_msg[STATUS];
				const auto is_ok = is_status_okay(status);
				const std::string ip = is_ok ? json_msg[IP] : std::string{};
				const std::string port = is_ok ? json_msg[PORT] : std::string{};
				caller->on_message(message_type::signalling_serv_req_res,
					signaling_server_req_res{is_ok, ip, port});
			}
			else if (type == "room_open_response") {
				const auto status = json_msg[STATUS];
				assert(detail::is_status_okay(status));
				const std::string id = json_msg[ID];
				caller->on_message(message_type::create_room_res, id);
			}
			else if (type == "room_close_response") {
				const std::string status = json_msg[STATUS];
				//assert(detail::is_status_okay(status));
				caller->on_message(message_type::close_room_res, status, msg  );
			}
			else if (type == "room_join_response") {
				const auto status = detail::is_status_okay(json_msg[STATUS]);
				const std::string id = json_msg[ID];
				const std::string room_id = json_msg[ROOM_ID]; //todo: need to send room id from server as well.
				caller->on_message(message_type::room_join_res, room_join_res{ status, id, room_id });
			}
			else if (type == "validate_room_join_res") {
				const auto status = detail::is_status_okay(json_msg[STATUS]);
				const std::string ip = json_msg[IP];
				const std::string port = json_msg[PORT];
				const std::string roomId = json_msg[ID];
				const room_join_validate_res r{ status, ip, port,  roomId };
				caller->on_message(message_type::validate_room_join_res,r);
			}
			else if (type == "response_router_capablity") {
				const auto m = json_msg[PEER_MSG_KEY];
				caller->on_message(message_type::router_capablity, m);
			}
			else if (type == "responseCreateProducerTransport") {
				const auto m = json_msg[PEER_MSG_KEY];
				/*const std::string id = m[ID];*/

				caller->on_message(message_type::producer_transport_res, m);
			}
			else if (type == "responseProduce") {
			const std::string producerId = json_msg[PEER_MSG_KEY];
			const std::string res_id = json_msg[RES_ID];
				caller->on_message(message_type::produce_res, produce_res{ producerId,res_id });

			}
			else if (type == "responseCreateConsumerTransport") {
			const auto m = json_msg[PEER_MSG_KEY];
			caller->on_message(message_type::consumer_create_res, m);
			}
			else if (type == "peer_add") {
				const std::string id = json_msg[PEER_MSG_KEY];
				const std::string name = json_msg[NAME]; 
				caller->on_message(message_type::peer_add, peer_info{ id, name }, msg);
			}
			else if (type == "producer_add") {
				const std::string peerId = json_msg[PEER_ID];
				const std::string producerId = json_msg[PRODUCER_ID];
				caller->on_message(message_type::producer_add, producer_info{ peerId, producerId });

			}
			else if (type == "producer_close") {
				const std::string peerId = json_msg[PEER_ID];
				const std::string producerId = json_msg[PRODUCER_ID];
				const std::string kind = json_msg[KIND];
				caller->on_message(message_type::producer_close, producer_info{ peerId, producerId, kind });
			}
			else if (type == "responseConsumer") {
				const auto m = json_msg[PEER_MSG_KEY];
				caller->on_message(message_type::consumer_res, m);
			}
			else if (type == "responseConnectConsumerTransport") {
			caller->on_message(message_type::consumer_connect_res, "success");
			}
			else if (type == "room_join_req") {
				const std::string room_id = json_msg["room_id"];
				const std::string user_name = json_msg["user_name"];
				const std::string ip = json_msg[IP];
				const std::string port = json_msg[PORT];
				caller->on_message(message_type::room_join_req,
					room_connection_credential{ ip, port, room_id, user_name });
			}
			else if (type == "request_close")//todo: change name of this
			{
			const std::string id = json_msg[ID];
			caller->on_message(message_type::room_leave_req, id);
			}
			else if (type == "request_room_open") {
				caller->on_message(message_type::room_open_req, json_msg);
			}
			else if (type == "request_rooms_info") {
				caller->on_message(message_type::req_room_info, json_msg);
			}
			else if (type == "response_room_info") {
				const auto output = parse_to_room_info(msg);
				caller->on_message(message_type::res_rooms_info, output);
			}
			/*else if (type == "room_server_info") {
						caller->on_message(message_type::req_room_serv_add, msg);
			}*/
			else if (type == "room_server_info_res") {
				signaling_server_req_res const m{ detail::is_status_okay(json_msg[STATUS]), json_msg[IP], json_msg[PORT] };
				caller->on_message(message_type::room_serv_res, m);
			}
			else if (type == "create_wnd_req") {
			const window_info m = json_msg[PEER_MSG_KEY].get<window_info>();
			caller->on_message(message_type::wnd_create_req, m);
			}
			else if (type == "create_wnd_req_res") {
				const bool status = json_msg[STATUS];
				const std::string class_name = json_msg["class_name"];
				const std::string parent_wnd = json_msg["parent_wnd"];
				const std::string child_wnd = json_msg["child_wnd"];
				const std::string id = json_msg[ID];
				grt::wnd_create_res const res{ status, class_name , parent_wnd, child_wnd, id };
				caller->on_message(message_type::window_create_res, res, msg);
			}
			else if (type == "wnd_close_req") {
				const std::string id = json_msg[PEER_MSG_KEY];
				caller->on_message(message_type::wnd_close_req, id);
			}
			else if (type == "wnd_close_req_res") {
				const bool status = json_msg[STATUS];
				const std::string id = json_msg[ID];

				caller->on_message(message_type::wnd_close_req_res, std::make_pair(status, id), msg);
			}
			else if (type == "show_self_view_layout") {
				caller->on_message(message_type::show_self_view_layout, "");
			}
			else if (type == "show_conference_view_layout") {
				caller->on_message(message_type::show_conference_layout, "");
			}
			else if (type == "show_ui_view_layout") {
			caller->on_message(message_type::show_ui_layout, "");
			}
			else if (type == "peer_remove") {
				const std::string id = json_msg[PEER_MSG_KEY];
				caller->on_message(message_type::peer_remove, id, msg);
				//todo : handle this case.
			}
			else if (type == "session_leave_req") {
				caller->on_message(message_type::session_leave_req, "", msg);
			}
			else if (type == CONNECTION_ERROR) {
				const std::string m = json_msg[PEER_MSG_KEY];
				caller->on_message(message_type::connection_error, m, msg);
			}
			else if (type == "mic_toggle_req") {
				const bool on = json_msg[PEER_MSG_KEY];
				caller->on_message(message_type::mic_toggle, on, msg);
			}
			else if (type == "cam_toggle_req") {
					const bool on = json_msg[PEER_MSG_KEY];
					caller->on_message(message_type::cam_toggle, on, msg);
			}
			else if (type == "share_toggle_req") {
			const bool on = json_msg[PEER_MSG_KEY];
			caller->on_message(message_type::share_toggle, on, msg);
			}
			else if (type == PEER_CALL_REQ_NATIVE) {
				caller->on_message(message_type::call_req_native, json_msg);
			}
			else if (type == RES_OWNER_ROOM) {
				caller->on_message(message_type::res_owner_room_list, json_msg);
			}
			else if (type == "active_speaker_list") {
				const auto& list = json_msg[PEER_MSG_KEY];
				std::vector<peer_info> ret;
				for (const auto& i : list) {
					ret.push_back(peer_info{ i[ID], i[NAME] });
				}
				caller->on_message(message_type::active_speaker, ret, msg);
			}
			else if (type == "update_wnd_info") {
				const window_info m = json_msg[PEER_MSG_KEY].get<window_info>();
				caller->on_message(message_type::wnd_update_notification, m, msg);
			}
			else if (type == "exit_msg") {
				const std::string m = json_msg[PEER_MSG_KEY];
				caller->on_message(message_type::exit, m, msg);
			}
			else if (type == "self_view") {
				const std::string cmd = json_msg[PEER_MSG_KEY];
				caller->on_message(message_type::self_view_req, cmd);
			}
			else if (type == "logout") {
				caller->on_message(message_type::logout, "", msg);
			}
			else if (type == "ui_close_open") {
				const bool on = json_msg[PEER_MSG_KEY];;
				caller->on_message(message_type::ui_right_pan_open, on, msg);
			}
			else if (type == "participant_close_open") {
				const bool on = json_msg[PEER_MSG_KEY];;
				caller->on_message(message_type::ui_left_pan_open, on, msg);
			}
			else {
				std::cout << "not supported msg = " << msg << "\n";
				caller->on_error(msg, "not supported msg");
				assert(false);
			}
		}
		
	}//namespace detail


	std::string 
		create_room_create_req(std::string room_name) {
		const json j2 = {
			{TYPE, "request_room_open"},
			{NAME, room_name}
		};
		return j2.dump();
		
	}

	std::string 
		make_room_create_req_res(bool const status, std::string room_id) {
		const json j2 = {
			{TYPE, "room_open_response"},
			{ID,room_id},
		{STATUS, detail::convert_to_success(status)}
		};
		return j2.dump();
	}

	std::string 
		make_room_close_req(std::string room_id) {
		const json j2 = {
			{TYPE, "request_room_close"},
		{ID, room_id}
		};
		return j2.dump();
	}

	std::string 
		make_room_infos_req() {
		const json j2 = {
			{TYPE, "request_rooms_info"}
		};
		return j2.dump();
	}

	std::string 
		make_room_join_req() {
		const json j2 = {
			{TYPE, "request_room_join"}
		};
		return j2.dump();
	}

	std::string
		make_room_serv_res(signaling_server_req_res res) {
		const json j2 = {
			{TYPE, "room_server_info_res"},
			{STATUS, detail::to_string(res.is_okay_)},
			{IP, res.ip_},
			{PORT, res.port_}
		};
		return j2.dump();
	}

	std::string 
		make_room_leave_req(std::string room_id) {
		const json j2 = {
			{TYPE, "request_close"},
			{ID, room_id}
		};
		return j2.dump();
	}

	std::string 
		convert_to_json(bool success, std::vector<room_info> const& room_list) {
		auto const count = room_list.size();
		std::vector<std::string> ids(count);
		assert(count == ids.size());
		std::vector<std::string> names(count);
		assert(count == names.size());
		std::transform(room_list.begin(), room_list.end(), ids.begin(),
			[](const room_info& info) {return info.id_; });
		std::transform(room_list.begin(), room_list.end(), names.begin(),
			[](const room_info& info) {return info.name_; });
		assert(std::mismatch(room_list.begin(), room_list.end(),
			ids.begin(), [](const room_info& first, const std::string id) { return first.id_ == id; }).first 
			==  room_list.end());
		assert(std::mismatch(room_list.begin(), room_list.end(),
			names.begin(), [](const room_info& first, const std::string name) {
			return first.name_ == name;
		}).first == room_list.end());

		json json_ids;
		for (auto const v : ids) {
			json_ids.push_back(v);
		}
		json json_names;
		for (auto const v : names) {
			json_names.push_back(v);
		}

		json const j2{
			{TYPE, "response_room_info"},
		{STATUS, detail::convert_to_success(success)},
			{"count", count},
		{"ids", json_ids},
		{"names", json_names}
		};

		return j2.dump();
	}

	std::string 
		make_room_join_req_res(room_join_res res ) {
		const json j2 = {
			{TYPE, "room_join_response"},
		{STATUS, detail::convert_to_success(res.is_okay)},
		{ID, res.self_id_},
		{ROOM_ID, res.room_id_}
		};
		
		return j2.dump();
	}

	std::string 
		create_register_user_req(std::string name) {
		const json j2 = {
			{TYPE, REG_USR_REQ},
			{NAME, name}
		};
		return j2.dump();
	}

	std::string make_router_capablity_req() {
	
		const json j2 = {
			{TYPE, "get_router_capability"}
		};
		return j2.dump();
	}

	static 
		auto
		get_procuder_transport_msg(bool force_tcp, json const&  rtpCapablity) {
		const json m = {
			{"forceTcp", force_tcp},
			{"rtpCapabilities", rtpCapablity}
		};
		return m.dump();
	}

	std::string 
		make_producer_transport_creation_req(bool force_tcp, json const&  rtpCapablity) {
		
		const json j2 = {
			{TYPE, "createProducerTransport"},
			{PEER_MSG_KEY,get_procuder_transport_msg(force_tcp, rtpCapablity) }
		};

		return j2.dump();
	}

	static 
		auto
		get_producer_transport_connect_msg(std::string const& transport_id,
			json const& dtls_parameter) {
		const json m{
			{"dtlsParameters",dtls_parameter },
			{"transportId", transport_id}
			};
		return m.dump();
	}

	

	std::string 
		make_producer_transport_connect_req(std::string const transport_id,
			json const& dtls_parameter) {
		const json j2 = {
			{TYPE, "connectProducerTransport"},
		{PEER_MSG_KEY, get_producer_transport_connect_msg(transport_id, dtls_parameter)}
		};
		return j2.dump();
	}

	static
		auto
		get_produce_trasport_msg(std::string const transport_id, std::string kind,
			json const& rtp_parameters, json const& appData) {
		const json j2 = {
			{"transportId", transport_id},
			{"kind", kind},
			{"rtpParameters", rtp_parameters},
			{"appData", appData}
		};
		return j2; //.dump();
	}

	std::string 
		make_produce_transport_req(std::string const transport_id, std::string kind,
			json const& rtp_parameters, std::string reqId, json const& appData) {
		const json j2 = {
			{TYPE, "produce"},
			{PEER_MSG_KEY, get_produce_trasport_msg(transport_id, kind, rtp_parameters, appData)},
			{REQ_ID, reqId}
		};
		return j2.dump();
	}

	static
		auto 
		make_consumer_transport_creation_message(bool force_tcp) {
		const json j2 = { "forceTcp",force_tcp };
		return j2.dump();
	}

	std::string make_consumer_transport_creation_req(bool force_tcp) {
		const json j2 = {
			{TYPE, "createConsumerTransport"},
			{PEER_MSG_KEY, make_consumer_transport_creation_message(force_tcp)}
		};
		return j2.dump();
	}

	static 
		auto
		make_consume_req_msg(json const& rtc_capablity) {
		const json j2 = {
			{"rtpCapabilities", rtc_capablity}
		};
		return j2.dump();
	}

	std::string make_consume_req(std::string peer_id, std::string producer_id, json const& rtc_capablity) {
		const json j2 = {
			{TYPE, "consume"},
		{ID, peer_id},
		{PRODUCER_ID, producer_id},
		{PEER_MSG_KEY, make_consume_req_msg(rtc_capablity)}
		};
		return j2.dump();
	}

	std::string make_consumer_resume_req(std::string id) {
		const json j2 = {
			{TYPE, "resume"},
		{PEER_MSG_KEY, id}
		};
		return j2.dump();
	}

	auto 
	get_connect_consumer_msg(std::string const& transport_id,
			json const& dtls_parameters) {
		const json j2 = {
			{"transportId", transport_id},
		{"dtlsParameters", dtls_parameters}
		};
		return j2;
	}

	std::string make_consumer_trasport_connect_req(std::string transport_id,
		json const& dtls_parameters) {
		const json j2 = {
			{TYPE, "connectConsumerTransport"},
		{PEER_MSG_KEY, get_connect_consumer_msg(transport_id, dtls_parameters)}
		};

		return j2.dump();
	}

	std::string make_participant_info_req() {
		const json j2 = {
			{TYPE, "get_participants"}
		};

		return j2.dump();
	}

	std::string make_render_wnd_close_req(std::string const id) {
		const json j2 = {
			{TYPE, "wnd_close_req"},
			{PEER_MSG_KEY, id}
		};
		return j2.dump();
	}

	std::string make_render_wnd_close_res(bool is_ok, std::string id) {
		const json j2 = {
			{TYPE, "wnd_close_req_res"},
			{STATUS, is_ok},
		{ID, id}
		};
		return j2.dump();
	}

	std::string make_show_hide_msg(bool to_show) {
		const std::string m = to_show ? "show" : "hide";
		assert(m == "show" || m == "hide");
		const json j2 = {
			{TYPE, "wnd_show_hide"},
		{PEER_MSG_KEY, m }
		};
		return j2.dump();
	}


	std::string make_self_view_layout() noexcept {
		return json{ {TYPE, "show_self_view_layout"}, }.dump();
	}

	std::string make_conference_view_layout() noexcept {
		return json{ {TYPE, "show_conference_view_layout"} }.dump();
	}

	std::string make_ui_view_layout() noexcept {
		return json{ {TYPE, "show_ui_view_layout"} }.dump();
	}

	std::string make_render_wnd_req(window_info info) {
		json m = info;
		const json j2 = {
			{TYPE, "create_wnd_req"},
			{PEER_MSG_KEY, m}
		};
		return j2.dump();
	}

	std::string 
		make_render_wnd_req_res(bool is_ok, std::string class_name, std::string parent_wnd_name, std::string id,
			std::string sender_id) {
		const json j2 = {
			{TYPE, "create_wnd_req_res"},
		{STATUS, is_ok},
		{"class_name", class_name},
		{"parent_wnd", parent_wnd_name},
		{"child_wnd", id},
		{ID, sender_id}
		};
		return j2.dump();
	}

	std::string 
		make_register_user_res(account_info info) {
		const json j2 = {
						{TYPE, REG_USR_REQ_RES},
						{STATUS, (info.error_.empty() ? OKAY : ERR)},
						{ID, info.id_},
						{ERR, info.error_},
						{NAME, info.name_},
						{ROOM_ID, info.room_id_}
						};
		return j2.dump();
	}

	std::string 
		create_ice_servers_req(std::string id) {
		const json j2 = {
						{TYPE, REQ_ICE_CANDIDATES},
						{ID, id}
						};
		return j2.dump();
	}

	std::string 
		make_forwarded_message(std::string to, std::string self, std::string msg) {
		const json j2 = {
							{TYPE, FORWARD_MSG_TYPE_KEY},
							{TO, to},
							{FROM, self},
							{PEER_MSG_KEY, msg}
						};
		return j2.dump();

	}

	std::string 
		make_webrtc_signalling_msg(webrtc_message_type type, std::string detail, std::string msg) {
		using message_type = webrtc_message_type;
		auto convert_string = [](message_type type) {
			switch (type) {
			case message_type::ANSWER:return PEER_ANS;
			case message_type::ICE_CANDIDATES: return PEER_ICE;
			case message_type::OFFER: return PEER_OFFER;
			default:
			assert(false);
			return "unknown";
			}
		};

		const auto type_str = convert_string(type);
		
		if (type == message_type::ANSWER || type == message_type::OFFER) {
			const std::string forwarded_msg = json{
											{TYPE, type_str},
											{SDP, msg}
										}.dump();
										
			return json{
						{TYPE, type_str},
						{DETAIL, detail},
						{PEER_MSG_KEY, forwarded_msg}
						}.dump();
		}
	
		return json{
				{TYPE, type_str},
				{DETAIL, detail},
				{PEER_MSG_KEY, msg}
				}.dump();
	}

	std::string create_call_req(call_type type) {
		return json{
						{TYPE, PEER_CALL_REQ},
						{DETAIL, detail::to_string(type)}
					}.dump();
	}

	//todo: remove this function dependency asap
	std::string _to_url(std::string ip, std::string port) {
		return std::string{ "ws://" } +ip + ':' + port;
	}

	std::string 
		create_call_req_res(call_type type, bool is_accept, std::string ip, std::string port, std::string id) {
		return json{
					{TYPE, PEER_CALL_RES},
					{DETAIL, detail::to_string(type)},
					{STATUS, detail::to_string(is_accept)},
					{URL, _to_url(ip, port)},
					{ID, id}
				}.dump();
	}

	std::string 
		make_connection_status(bool is_okay) {
		return json{
						{TYPE, CON_STATUS},
						{STATUS, detail::to_connection_status_str(is_okay)}
					}.dump();
	}

	std::string 
		make_prsence_notifcation(presence_notification notification) {

		const auto m =  json{
					{TYPE, detail::to_string(notification.status_)},
					{KEY, notification.id_},
					{NAME, notification.name_}
					}.dump();
		return json{
					{TYPE, PRESENCE_NOTIFICATION},
					{PEER_MSG_KEY, m}
					}.dump();

	}

	std::string 
	make_show_message(std::string message, std::string id) {
		return json{
					{TYPE, SHOW_MSG},
					{PEER_MSG_KEY, message},
					{ID, id}
					}.dump();
	}

	std::string 
		stringify(ice_candidates_info info) {
		return json{
					{MID, info.mid_},
					{MINDEX, std::to_string(info.mline_index_)},
					{CANDIDATE_SDP, info.sdp_}
					}.dump();
	}

	std::string
		make_login_message(std::string user_name, std::string pwd) {
		//return std::string{ "/login_req?" } +"user=" + user_name + "&pwd=" + pwd;
		return json{
			{TYPE, "login_req"},
			{PEER_MSG_KEY, json{{"user", user_name}, {"pwd",pwd}}}
			/*{"user", user_name},
			{"pwd", pwd}*/
		}.dump();
	}

	std::string make_signalling_server_message() {

		return json{ {TYPE, SIGNALLING_SERV_REQ}
						}.dump();
	}

	std::string make_user_session_leave_req() {
		return json{
			{TYPE, "session_leave_req"}
		}.dump();
	}

	std::string make_session_leave_notification() {
		return json{
			{TYPE, "notify_session_close"}
		}.dump();
	}


	std::string 
		make_validate_room_join_req(std::string roomId) {
		return json{
		   { TYPE, "validate_room_join"},
		   {ID, roomId}
		}.dump();
	}

	std::string 
		make_room_command(std::string cmd) {
		return json{
			{TYPE, "room_request"},
			{PEER_MSG_KEY, cmd}
		}.dump();
	}

	std::string make_json_msg(message_type type, std::string detail){
		return json{
			{TYPE, detail::to_string(type)},
			{PEER_MSG_KEY, detail}

		}.dump();
	}

	std::string make_json_msg(chat_msg msg) {
		return json{
			{TYPE, CHAT_MSG_TYPE},
		{PEER_MSG_KEY, msg.msg_},
		{NAME, msg.sender_name_}
		}.dump();
	}

	std::string 
		make_p2p_call_room_req(std::string roomId, std::string self_id) {
		return json{
			{TYPE, PEER_CALL_REQ},
		{ROOM_ID, roomId},
		{ID, self_id}
		}.dump();
	}

	std::string make_peer_connection_remove_req(std::string id) {
		return json{
			{TYPE, REMOVE_CONNECTION},
			{ID, id}
		}.dump();
	}

	std::string make_mic_toggle_req(bool on) {
		return json{
			{TYPE, "mic_toggle_req"},
		{PEER_MSG_KEY, on}
		}.dump();
	}

	std::string make_camera_toggle_req(bool on) {
		return json{
			{TYPE, "cam_toggle_req"},
		{PEER_MSG_KEY, on}

		}.dump();
	}

	std::string make_share_toggle_req(bool on) {
		return json{
		{TYPE, "share_toggle_req"},
			{PEER_MSG_KEY, on}

		}.dump();
	}

	std::string make_window_info_update_msg(window_info info) {
		json const m = info;
		return json{
			{TYPE, "update_wnd_info"},
		{PEER_MSG_KEY, m}
		}.dump();
	}

	std::string make_exit_msg(std::string msg) {
		return json{
			{TYPE, "exit_msg"},
			{PEER_MSG_KEY, msg}
		}.dump();
	}

	std::string make_ui_close_open(bool on) {
		return json{
				{TYPE, "ui_close_open"},
				{PEER_MSG_KEY, on}
				}.dump();
	}

	std::string make_peer_add_msg(std::string id, std::string name) {
		return json{
			{TYPE, "peer_add"},
			{PEER_MSG_KEY, id},
			{NAME, name}
		}.dump();
	}

	std::string make_participant_open_close(bool on) {
		return json{
				{TYPE, "participant_close_open"},
				{PEER_MSG_KEY, on}
		}.dump();
	}
	std::string make_call_response(call_response_info info) {
		return json{
			{TYPE, CALL_NOTIFICATION},
		{STATUS, info.is_accepted_?"connected":"disconnected"},
		{ID, info.new_id_},
		{ROOM_ID, info.url_}
		}.dump();
	}

	std::string make_producer_close_notification(std::string producerId, std::string kind) {
		return json{
			{TYPE, "producer_close"},
		{ID,producerId},
		{KIND, kind}
		}.dump();
	}

	void async_parse_message(std::string msg, parser_callback* caller) {

		std::thread{
			detail::_parse, msg, caller
		}.detach();
		
	}


	void async_parse_message(util::executor* executor, std::string msg, parser_callback* caller) {
		auto f = util::make_task(&detail::_parse, msg, caller);
		executor->submit(f);
	}

	void parse_message(std::string msg, parser_callback* caller) {
		detail::_parse(msg, caller);
	}

	std::string get_type(std::string const& msg) {
		return detail::get_type(msg);
	}

	absl::any get_json_object(std::string const& msg) {
		return json::parse(msg);
	}

	//
}//namespace grt