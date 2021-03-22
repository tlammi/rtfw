#pragma once

#include <utility>
#include <vector>
#include <algorithm>


namespace rtfw{
namespace detail{

template<class K, class V>
class VectorMap{
	using Map = std::vector<std::pair<K, V>>;
public:

	VectorMap(const std::initializer_list<K>& keys){
		map_.reserve(keys.size());
		for(const auto& k: keys){
			map_.push_back({k, {}});
		}

		std::sort(map_.begin(), map_.end(),
			[](const auto& lhs, const auto& rhs){
				return lhs.first < rhs.first;
			});
	}


	VectorMap(const VectorMap&) = default;
	VectorMap(VectorMap&&) = default;
	VectorMap& operator=(const VectorMap&) = default;
	VectorMap& operator=(VectorMap&&) = default;

	V& operator[](const K& k){
		return find_(k);
	}

	const V& operator[](const K& k) const {
		return find_(k);
	}

	size_t size() const noexcept {
		return map_.size();
	}

	bool has_key(const K& k) const noexcept {
		return std::binary_search(map_.begin(), map_.end(),std::pair<K, V>(k, {}),
			[](const auto& lhs, const auto& rhs){
				return lhs.first < rhs.first;
			});
	}

	bool has_value(const V& v) const noexcept {
		for(const auto& pair: map_)
			if(pair.second == v) return true;
		return false;
	}

private:
	
	V& find_(const K& k){
		auto match = std::lower_bound(map_.begin(), map_.end(), k,
				[](const auto& lhs, const auto& rhs){
					return lhs.first < rhs;
				});
		if(match->first == k)
			return match->second;
		throw std::runtime_error("Key not found");
	}

	const V& find_(const K& k) const {
		auto match = std::lower_bound(map_.begin(), map_.end(), k,
				[](const auto& lhs, const auto& rhs){
					return lhs.first < rhs;
				});
		if(match->first == k)
			return match->second;
		throw std::runtime_error("Key not found");
	}

	Map map_{};
};
}
}
