#pragma once

#include "rtfw/refcountable.hpp"

namespace rtfw {

class RefCounter{
public:
	RefCounter(RefCountable* ref): ref_{ref}{
		ref_->increment_refcount_();
	}

	~RefCounter(){
		if(ref_) ref_->decrement_refcount_();
	}

	RefCounter(const RefCounter& rhs): RefCounter{rhs.ref_} {}

	RefCounter(RefCounter&& rhs): ref_{std::move(rhs.ref_)} {
		rhs.ref_ = nullptr;
	}

	RefCounter& operator=(const RefCounter& rhs){
		ref_ = rhs.ref_;
		ref_->increment_refcount_();
		return *this;
	}

	RefCounter& operator=(RefCounter&& rhs){
		ref_ = std::move(rhs.ref_);
		rhs.ref_ = nullptr;
		return *this;
	} 


	bool operator<(const RefCounter& rhs) const noexcept{
		return ref_ < rhs.ref_;
	}

private:
	RefCountable* ref_;
};
}
