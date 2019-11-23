#ifndef __COMMON_DEF_H__
#define __COMMON_DEF_H__
#include <string>

namespace grt {
	enum class message_type {
		offer_sdp,
		answer_sdp,
		ice_candidates,
		remote_stream_attached_evt,
		remote_stream_remove_evt,
		connection_disconnected,
		connection_connected,
		renegotiation_needed,
		user_registration_req,
		forward_message,
		create_peer,
		remove_peer,
		login_res,
		login_req,
		call_req,
		call_req_res,
		call_res_evt,
		signalling_serv_req_res,
		room_info_res,
		room_open_res,
		room_close_res,
		room_serv_res,
		create_room_res,
		close_room_res,
		room_join_req,
		room_open_req,
		room_join_res,
		req_room_serv_add,
		req_room_info,
		res_rooms_info,
		room_leave_req,
		router_capablity,
		producer_transport_res,
		produce_res,
		consumer_create_res,
		peer_add,
		peer_remove,
		consumer_res,
		consumer_connect_res,
		wnd_create_req,
		window_create_res,
		wnd_close_req,
		wnd_close_req_res,
		wnd_show_hide,
		session_leave_req,
		connection_error,
		chat_msg,
		mic_toggle,
		cam_toggle,
		producer_add,
		producer_close,
		invalid
	};

	struct ice_candidates_info {
		std::string mid_;
		int mline_index_;
		std::string sdp_;
	};

	struct sdp_message {
		std::string sdp;

	};

	enum class call_type {
		AUDIO,
		VIDEO,
		unknown
	};


	struct call_response_info {
		std::string detail_;
		std::string new_id_;
		bool is_accepted_;
		std::string url_;
		std::string status_;
		std::string from_;
		std::string json_;
	};

	struct callee_address {
		std::string url_;
		std::string id_;
	};

	struct call_req_info {
		call_type type_;
		std::string sender_id_;
		std::string json_;
	};
	
	struct call_res_event {
		call_type type_;
		std::string sender_id_;
		std::string self_id_;
		bool is_accepted_;
	};

	struct signaling_server_req_res {
		bool is_okay_;
		std::string ip_;
		std::string port_;
	};

	struct room_open_res {
		bool is_okay_;
		std::string id_;
	};


	struct room_connection_credential {
		std::string ip_;
		std::string port_;
		std::string room_id_;
		std::string user_name_;
	};

	struct room_info {
		std::string id_;
		std::string name_;
		bool operator==(room_info const& other) const {
			return id_ == other.id_ && name_ == other.name_;
		}
	};

	using peer_info = room_info;

	struct wnd_create_res {
		bool status_{ false };
		std::string class_name_;
		std::string parent_wnd_name_;
		std::string child_wnd_id_;
		std::string id;
	};

	struct chat_msg {
		std::string sender_id_;
		std::string sender_name_;
		std::string msg_;
	};

	struct producer_info {
		std::string peer_id_;
		std::string producer_id_;
	};

}//namespace grt

#endif//__COMMON_DEF_H__