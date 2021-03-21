#pragma once

#include <vector>
#include <utility>
#include "boost/lexical_cast.hpp"

namespace rtfw {

class Config{
public:
	Config(std::string_view name="", const std::vector<std::string_view>& keys={}):
		name_{name} {
		for(const auto& k: keys){
			map_.emplace_back(k, "");
		}
		std::sort(map_.begin(), map_.end(),
			[](const auto& lhs, const auto& rhs){ return lhs.first < rhs.first; });
	}

	Config(const Config&) = default;
	Config(Config&&) = default;

	Config& operator=(const Config&) = default;
	Config& operator=(Config&&) = default;

	void set_name(std::string_view name){
		name_ = name;
	}

	std::string_view name() const noexcept{
		return name_;
	}


	template<class T=std::string_view>
	T value(std::string_view key) const {
		auto match = find_(key);
		return boost::lexical_cast<T>(match->second);
	}

	void set(std::string_view key, std::string_view val){
		auto match = find_(key);
		match->second = val;
	}

private:


	std::vector<std::pair<std::string, std::string>>::const_iterator find_(std::string_view key) const {
		const auto match = std::lower_bound(map_.begin(), map_.end(), key,
			[](const auto& pair, const auto& key){ return pair.first < key; });
		if(match->first == key)
			return match;
		throw std::runtime_error("Invalid key");
	}

	std::vector<std::pair<std::string, std::string>>::iterator find_(std::string_view key) {
		auto match = std::lower_bound(map_.begin(), map_.end(), key,
			[](const auto& pair, const auto& key){ return pair.first < key; });
		if(match->first == key)
			return match;
		throw std::runtime_error("Invalid key");
	}

	std::string name_{};
	std::vector<std::pair<std::string, std::string>> map_{};
};

template<>
std::string_view Config::value<std::string_view>(std::string_view key) const {
	auto match = find_(key);
	return match->second;
}

}

