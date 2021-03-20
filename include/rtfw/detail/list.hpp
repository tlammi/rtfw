#pragma once

#include <utility>
#include <cstddef>
#include <stdexcept>

namespace rtfw{
namespace detail{

class List;

class ListHead{
	friend class List;
public:
	ListHead();
	~ListHead();

	ListHead(const ListHead&) = delete;
	ListHead(ListHead&&) = delete;

	ListHead& operator=(const ListHead&) = delete;
	ListHead& operator=(ListHead&&) = delete;

private:
	void free_() noexcept;

	ListHead* next_{nullptr};
	ListHead* prev_{nullptr};
};

class List{
public:
	List();

	~List();

	List(const List&) = delete;
	List(List&&) = delete;

	List& operator=(const List&) = delete;
	List& operator=(List&&) = delete;

	void pop(ListHead& head) noexcept;

	void push_back(ListHead& head)  noexcept;

	void push_front(ListHead& head) noexcept;

	void insert_before(ListHead& before, ListHead& head) noexcept;
	
	void insert_after(ListHead& after, ListHead& head) noexcept;

	bool find(ListHead& head) noexcept;

	static ListHead* next(ListHead& head);

	static ListHead* prev(ListHead& head);

	ListHead* first() noexcept;
	
	ListHead* last() noexcept;

	size_t size() const noexcept;
private:

	static void insert_between(ListHead* first, ListHead* last, ListHead* head) noexcept;

	ListHead before_{};
	ListHead after_{};
};

}
}
