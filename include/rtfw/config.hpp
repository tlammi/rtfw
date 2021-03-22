#pragma once

#include <vector>
#include <utility>
#include "boost/lexical_cast.hpp"

#include "rtfw/detail/vectormap.hpp"

namespace rtfw {

class Config{
public:

	Config(std::string_view name="", const std::vector<std::string_view>& keys={}):
		name_{name}, map_{keys} {}

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
		return boost::lexical_cast<T>(map_[key]);
	}

	void set(std::string_view key, std::string_view val){
		map_[key] = val;
	}


	auto begin() const {
		return map_.begin();
	}

	auto end() const {
		return map_.end();
	}

private:
	std::string name_{};
	detail::VectorMap<std::string, std::string> map_;
};

template<>
inline std::string_view Config::value<std::string_view>(std::string_view key) const {
	return map_[key];
}

}

