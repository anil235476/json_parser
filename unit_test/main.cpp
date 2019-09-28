#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "json_parser.h"
#include <future>
class json_parser_clb : public grt::parser_callback {
	
private:
	std::vector<grt::room_info> const infos_;
	std::promise<void> done_;
public:
	json_parser_clb(std::vector<grt::room_info> const& info, std::promise<void>&& done) 
		:infos_{ info }, done_{ std::move(done) } {}

	void on_message(grt::message_type type, absl::any msg) override {
		if (grt::message_type::res_rooms_info == type) {
			const auto  r = absl::any_cast<std::vector<grt::room_info>>(msg);
			REQUIRE(r == infos_);
		}
		else {
			assert(false);
		}
		done_.set_value();
	}
};

TEST_CASE("json parser", "[parser]") {
	std::vector<grt::room_info> const arr{ grt::room_info{ "0", "zero" }, grt::room_info{"1", "zero" } };
		
	const auto json = grt::convert_to_json(arr);
	REQUIRE_FALSE(json.empty());
	const auto result = grt::detail::parse_to_room_info(json);
	REQUIRE(result.size() == arr.size());
	REQUIRE(arr == result);
	std::promise<void> evnt;
	auto future = evnt.get_future();
	json_parser_clb clb{ arr, std::move(evnt) };
	grt::async_parse_message(json, &clb);
	future.get();
}
