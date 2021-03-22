#pragma once

#include <utility>
#include <vector>
#include <string_view>

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
std::vector<std::string_view> split(std::string_view input, std::string_view separator);

}
}
