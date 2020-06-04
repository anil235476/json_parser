#ifndef _JSON_PARSER_H__
#define _JSON_PARSER_H__
#include <string>
#include <absl/types/any.h>
#include <absl/types/optional.h>
#include "common/common_def.h" //todo: make proper path for this file
#include <json.hpp>
#include "executor/executor.h"
#include <map>

using json = nlohmann::json;
constexpr const char* FORWARD_MSG_TYPE_KEY{ "forward_message" };
constexpr const char* ROOM_REQ{ "room_request" };
constexpr const char* ID{ "id" };
constexpr const char* ROOM_ID{ "roomId" };
namespace grt {

	enum class webrtc_message_type {
		OFFER,
		ANSWER,
		ICE_CANDIDATES,
		unknown
	};

	struct call_response_info {
		std::string detail_;
		std::string new_id_;
		bool is_accepted_;
		std::string url_;
		std::string status_;
		std::string from_;
		json json_;
	};

	struct call_req_info {
		call_type type_;
		std::string sender_id_;
		json json_;
	};


	struct login_res {
		bool status_;
		std::string ip_;
		std::string port_;
		std::string id_;
	};

	enum class connection_status {
		connected,
		error,
		closed,
		none
	};

	struct connection_res {
		connection_status status_;
		std::string info_;
	};

	struct login_req {
		std::string usr_;
		std::string pwd_;
		std::string server_;
	};

	struct consumer_info {
		std::string id_;
		int preferrred_layer_;
	};

	class parser_callback {
	public:
		virtual ~parser_callback(){}
		virtual void on_ice_servers_req_res(bool isokay, std::string servers_list){}
		virtual void on_error(std::string msg, std::string err){}
		
		virtual void on_webrtc_signalling_msg(webrtc_message_type type, 
			std::string id, absl::any msg, std::string detail){}

		virtual void on_message(message_type, absl::any msg, absl::optional<absl::any> unparsed_msg = absl::nullopt){}

	};

	//this is hook which any client can install in library code to get message notifcation.
	using message_handler_hook = std::map<message_type, std::function<void(absl::any, absl::optional<absl::any> unparsed_msg)>>;

	void async_parse_message(std::string msg, parser_callback* caller);
	void async_parse_message(util::executor* executor, std::string msg, parser_callback* caller);
	void parse_message(std::string msg, parser_callback* caller);

	absl::optional<chat_msg> parse_conference_chat_msg(std::string const& msg);

	std::string get_type(std::string const& msg);

	absl::any get_json_object(std::string const& msg);

	std::string create_room_create_req(std::string room_name);
	std::string make_room_create_req_res(bool const status, std::string room_id);
	std::string make_room_close_req(std::string room_id);
	std::string make_room_infos_req();
	std::string make_room_join_req(std::string roomId, std::string user_name);
	std::string make_room_leave_req(std::string room_id);
	std::string make_room_serv_res(signaling_server_req_res res);
	std::string convert_to_json(bool success, std::vector<room_info> const&);
	std::string make_room_join_req_res(room_join_res);
	std::string create_register_user_req(std::string name);
	std::string make_router_capablity_req();
	std::string make_producer_transport_creation_req(bool force_tcp, json const& rtpCapablity);
	std::string make_producer_transport_connect_req(std::string const transport_id, json const& dtls_parameter);
	std::string make_produce_transport_req(std::string const transport_id, std::string kind,
		json const& rtp_parameters, json const& sctpStreamParameters, std::string reqId,  json const& appData= json::object());

	std::string make_consumer_transport_creation_req(bool force_tcp);
	std::string make_consume_req(std::string peer_id, std::string producer_id, json const& rtc_capablity);
	std::string make_consumer_resume_req(std::string id);
	std::string make_consumer_trasport_connect_req(std::string transport_id,
		json const& dtls_parameters);

	std::string make_participant_info_req();
	std::string make_validate_room_join_req(std::string roomId);
	std::string make_room_command(std::string cmd);
	std::string make_render_wnd_req(window_info info);
	std::string make_render_wnd_req_res(bool is_ok, std::string class_name, std::string parent_wnd_name, std::string id, std::string sender_id);

	std::string make_render_wnd_close_req(std::string const id);
	std::string make_render_wnd_close_res(bool is_ok, std::string const id);

	std::string make_self_view_layout() noexcept;
	std::string make_conference_view_layout() noexcept;
	std::string make_ui_view_layout() noexcept;

	std::string make_register_user_res(account_info);
	std::string create_ice_servers_req(std::string id);
	
	std::string make_forwarded_message(std::string to, std::string self, std::string msg);

	std::string make_webrtc_signalling_msg(webrtc_message_type type, std::string detail, std::string msg);

	std::string create_call_req(call_type type);

	std::string create_call_req_res(call_type type, bool is_accept, std::string ip, std::string port, std::string id);
	std::string make_connection_status(bool is_okay, std::string id);
	std::string make_prsence_notifcation(presence_notification);
	std::string make_show_message(std::string message, std::string id);
	struct ice_candidates_info; //forward declaration
	std::string stringify(ice_candidates_info info);
	std::string make_login_message(std::string user_name, std::string pwd);
	std::string make_signalling_server_message();
	std::string make_peer_connection_remove_req(std::string id);
	std::string make_user_session_leave_req();
	std::string make_session_leave_notification();
	std::string make_json_msg(message_type type, std::string detail);
	std::string make_json_msg(chat_msg msg);
	std::string make_p2p_call_room_req(std::string roomId, std::string self_id);
	std::string make_call_response(call_response_info info);
	std::string make_producer_close_notification(std::string producerId, std::string kind);
#ifdef ENABLE_UNIT_TETS
	//only for unit test 
	namespace detail {
		std::vector<grt::room_info>
			parse_to_room_info(std::string const& msg);
	}
#endif//
	std::string make_mic_toggle_req(bool on);
	std::string make_camera_toggle_req(bool on);
	std::string make_share_toggle_req(bool on);
	std::string make_window_info_update_msg(window_info info);
	std::string make_exit_msg(std::string msg);
	std::string make_ui_close_open(bool on);
	std::string make_peer_add_msg(std::string id, std::string name);
	std::string make_participant_open_close(bool on);
	std::string make_consumers_with_preferred_layers(std::vector<consumer_info> const& list);
	std::string make_chat_open_close(bool on);
	std::string make_consumers_with_preferred_layers(std::vector<consumer_info> const& list);

	//button handler messages and data structure
	struct renderer_button_status {
		absl::optional<bool> mic_on_;
		absl::optional<bool> cam_on_;
		absl::optional<bool> chat_on_;
		absl::optional<bool> pariticpant_on_;
	};

	enum class button_msg_type {
		button_click,
		button_status,
		invalid,
	};

	struct button_handler_message {
		button_msg_type type;
		absl::any msg;
	};
	std::string make_button_click_msg(int id);
	std::string make_button_status_msg(renderer_button_status status);
	button_handler_message parse_button_handler_msg(std::string const& msg);
}//namespace grt

#endif//_JSON_PARSER_H__