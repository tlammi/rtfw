#pragma once

#include "rtfw/refcounter.hpp"

namespace rtfw{

template<class T>
class ModuleRef{
public:
	ModuleRef(RefCountable* ref, std::optional<T>* data): ref_counter_{ref}, data_{&data->value()} {}

	T* operator->() noexcept {
		return data_;
	}

	const T* operator->() const noexcept {
		return data_;
	}

	T& operator*() noexcept {
		return *data_;
	}

	const T& operator*() const noexcept {
		return *data_;
	}


private:
	RefCounter ref_counter_;
	T* data_;
};
}
