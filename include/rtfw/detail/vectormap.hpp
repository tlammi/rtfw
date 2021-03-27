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


	class ConstIterator{
	public:
		ConstIterator(typename Map::const_iterator iter): iter_{iter}{}

		typename Map::const_pointer operator->() const {
			return &(*iter_);
		}

		typename Map::const_reference operator*() const {
			return *iter_;
		}

		ConstIterator& operator++(){
			++iter_;
			return *this;
		}

		ConstIterator& operator--(){
			--iter_;
			return *this;
		}
		
		bool operator==(const ConstIterator& rhs) const {
			return iter_ == rhs.iter_;
		}
		
		bool operator!=(const ConstIterator& rhs) const {
			return (*this) == rhs;
		}
	private:
		typename Map::const_iterator iter_;
	};

	VectorMap(const std::initializer_list<K>& keys){
		map_.reserve(keys.size());
		for(const auto& k: keys){
			map_.emplace_back(K{k}, V{});
		}

		std::sort(map_.begin(), map_.end(),
			[](const auto& lhs, const auto& rhs){
				return lhs.first < rhs.first;
			});
	}

	template<class Container>
	VectorMap(Container&& keys){
		map_.reserve(keys.size());
		for(const auto& k: keys){
			map_.emplace_back(K{k}, V{});
		}

		std::sort(map_.begin(), map_.end(),
			[](const auto& lhs, const auto& rhs){
				return lhs.first < rhs.first;
			});
		
	}


	VectorMap(const VectorMap&) = delete;
	VectorMap(VectorMap&&) = default;

	VectorMap& operator=(const VectorMap&) = delete;
	VectorMap& operator=(VectorMap&&) = default;
	
	template<class K2>
	V& operator[](const K2& k){
		return find_(k);
	}
	
	template<class K2>
	const V& operator[](const K2& k) const {
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


	auto begin() const {
		return map_.begin();
	}

	auto end() const {
		return map_.end();
	}

private:
	
	template<class K2>
	V& find_(const K2& k){
		auto match = std::lower_bound(map_.begin(), map_.end(), k,
				[](const auto& lhs, const auto& rhs){
					return lhs.first < rhs;
				});
		if(match->first == k)
			return match->second;
		throw std::runtime_error("Key not found");
	}
	
	template<class K2>
	const V& find_(const K2& k) const {
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
