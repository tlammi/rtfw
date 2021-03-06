#pragma once

#include <optional>
#include <string_view>
#include "rtfw/detail/list.hpp"
#include "rtfw/config.hpp"

namespace rtfw{
namespace detail{

class ModuleHolder: public ListHead {
public:
	static inline List instances{};
	ModuleHolder(): ListHead() {
		instances.push_back(*this);
	}

	virtual ~ModuleHolder(){}

	virtual std::string_view name() const noexcept = 0;
	virtual rtfw::Config& config() = 0;
	virtual void init(const rtfw::Config& conf) = 0;
	virtual void clear() = 0;
private:
};


template<class T>
class ModuleHolderImpl: public ModuleHolder {
public:
	
	static inline ModuleHolderImpl<T>* instance{nullptr};

	ModuleHolderImpl(std::string_view name): ModuleHolder(), name_{name}, conf_{T::config()}{
		instance = this;
	}

	std::string_view name() const noexcept final {
		return name_;
	}


	Config& config() final {
		return conf_;
	}

	void init(const rtfw::Config& conf) final {
		opt_.emplace(conf);
	}


	void clear() final {
		opt_ = std::nullopt;
	}

	T* module(){
		if(opt_) return &(opt_.value());
		return nullptr;
	}

private:
	std::string_view name_;
	std::optional<T> opt_{std::nullopt};
	Config conf_;
};
}
}
