#pragma once

#include <vector>
#include <utility>
#include <map>
#include "boost/lexical_cast.hpp"


namespace rtfw {

class Config: public std::map<std::string, std::string, std::less<>> {
public:

	using Parent = std::map<std::string, std::string, std::less<>>;

	Config(std::string_view name="", std::initializer_list<std::string> keys={}):
		name_{name} {
			for(const auto& k: keys){
				(*this)[k] = "";
			}
		}

	Config(const Config&) = default;
	Config(Config&&) = default;

	Config& operator=(const Config&) = default;
	Config& operator=(Config&&) = default;

	std::string& name() {
		return name_;
	}
	
	const std::string& name() const {
		return name_;
	}

	template<class T=std::string_view>
	T value(std::string_view key) const {
		return boost::lexical_cast<T>(at(key));
	}

	std::string& operator[](std::string_view str){
		auto iter = find(str);
		if(iter == end()){
			auto [new_iter, success] = insert(Parent::value_type(std::string(str), std::string()));
			if(!success) throw std::runtime_error("Could not insert into the map");
			iter = new_iter;
		}
		return iter->second;
	}


	const std::string& at(std::string_view str) const {
		auto iter = find(str);
		if(iter != end())
			return iter->second;
		throw std::runtime_error("map at()");
	}

private:
	std::string name_{};
};

template<>
inline std::string_view Config::value<std::string_view>(std::string_view key) const {
	return this->at(key);
}

}

