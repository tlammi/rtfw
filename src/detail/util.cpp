#include "rtfw/detail/util.hpp"


namespace rtfw{
namespace detail{


std::vector<std::string_view> split(std::string_view str, std::string_view separator){
	std::vector<std::string_view> out{};
	size_t prev=0;
	for(auto idx = str.find(separator); idx != std::string_view::npos; idx = str.find(separator, prev+separator.size()-1)){
		out.emplace_back(str.substr(prev, idx-prev));
		prev = idx+separator.size();
	}

	out.emplace_back(str.substr(prev));
	
	return out;
}
}
}
