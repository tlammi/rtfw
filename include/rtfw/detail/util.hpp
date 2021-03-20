#pragma once

#include <utility>

namespace rtfw{
namespace detail{

/**
 * Class for calling callable in destructor
 */
template<class F>
class Defer{
public:
	explicit Defer(F&& f): f_{std::move(f)}{}

	~Defer(){
		f_();
	}

	Defer(const Defer&) = delete;
	Defer(Defer&&) = delete;

	Defer& operator=(const Defer&) = delete;
	Defer& operator=(Defer&&) = delete;

private:
	F f_;
};
}
}
