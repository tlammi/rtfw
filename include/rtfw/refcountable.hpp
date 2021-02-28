#pragma once

namespace rtfw{

class RefCounter;

class RefCountable{
	friend class RefCounter;
public:
	virtual ~RefCountable(){}
private:
	virtual void increment_refcount_() = 0;
	virtual void decrement_refcount_() = 0;
};
}
