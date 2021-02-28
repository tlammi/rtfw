#pragma once

#include <deque>
#include <map>
#include <optional>
#include <mutex>
#include <iostream>
#include <sstream>
#include <algorithm>

#include "rtfw/refcountable.hpp"
#include "rtfw/moduleref.hpp"

namespace rtfw{

template<class T>
class RefCountedModule;

namespace detail{

/**
 * Access all instances of RefCountedModule and their names
 */
inline std::map<RefCountable*, const char*>& generic_modules(){
	static auto mods = new std::map<RefCountable*, const char*>();
	return *mods;
}

/**
 * Used for storing current module constructor stack to detect circular dependencies
 */
inline std::deque<RefCountable*>& ctor_stack(){
	static auto mods = new std::deque<RefCountable*>();
	return *mods;
}

template<class T>
inline RefCountedModule<T>*& typed_module(){
	static RefCountedModule<T>* ptr = nullptr;
	return ptr;
}
}

template<class T>
class RefCountedModule: public RefCountable {
public:
	RefCountedModule(const char* name){
		if(detail::typed_module<T>())
			throw std::runtime_error("Only one module per type supported");
		detail::typed_module<T>() = this;
		detail::generic_modules()[this] = name;
	}

	~RefCountedModule(){
		detail::typed_module<T>() = nullptr;
		detail::generic_modules().erase(this);
	}


	RefCountedModule(const RefCountedModule&) = delete;
	RefCountedModule(RefCountedModule&&) = delete;

	RefCountedModule& operator=(const RefCountedModule&) = delete;
	RefCountedModule& operator=(RefCountedModule&&) = delete;

	ModuleRef<T> module_ref() {
		return {this, &val_};
	}

	RefCounter ref() {
		return {this};
	}

private:
	/**
	 * Called from reference counter ctor
	 */
	void increment_refcount_() final {
		std::unique_lock lk{mut_};
		auto& stack = detail::ctor_stack();
		if(std::find(stack.begin(), stack.end(), this) != stack.end()){
			std::stringstream ss;
			ss << "Circular dependency detected: ";
			for(const auto ptr: stack){
				ss  << detail::generic_modules().at(ptr) << " -> ";
			}
			ss << detail::generic_modules().at(this);
			throw std::runtime_error(ss.str());
		} 
		if(!ref_count_){
			detail::ctor_stack().push_back(this);
			val_.emplace();
			detail::ctor_stack().pop_back();
		}
		++ref_count_;
	}

	/**
	 * Called from reference counter dtor
	 */
	void decrement_refcount_() final {
		std::unique_lock lk{mut_};
		if(!ref_count_) throw std::runtime_error("Reference count would be negative");
		--ref_count_;
		if(!ref_count_){
			val_ = std::nullopt;
		}
	}

	std::optional<T> val_{std::nullopt};
	size_t ref_count_{0};
	std::recursive_mutex mut_{};
};
}
