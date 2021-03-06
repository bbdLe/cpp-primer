#include"StrVec.h"
#include<iostream>


StrVec::StrVec() : elements(nullptr), first_free(nullptr), cap(nullptr) {}

StrVec::StrVec(const StrVec &other)
{
	auto p = alloc_n_copy(other.elements, other.first_free);
	elements = p.first;
	first_free = cap = p.second;
}

StrVec::StrVec(StrVec &&other) noexcept : elements(other.elements), first_free(other.first_free), cap(other.cap)
{
	other.elements = nullptr;
	other.first_free = nullptr;
	other.cap = nullptr;
	std::cout << "right refer" << std::endl;
}

StrVec &StrVec::operator=(StrVec &&other) noexcept
{
	if(&other != this)
	{
		free();
		elements = other.elements;
		first_free = other.first_free;
		cap = other.cap;
		other.elements = other.first_free = other.cap = nullptr;
		std::cout << "right refer" << std::endl;
	}
}

StrVec &StrVec::operator=(const StrVec &other)
{
	auto p = alloc_n_copy(other.elements, other.first_free);
	free();
	elements = p.first;
	cap = first_free = p.second;
}

StrVec::~StrVec()
{
	free();
}

void StrVec::free()
{
	if(elements)
	{
		auto p = first_free;
		while(p != elements)
			alloc.destroy(--p);
			alloc.deallocate(elements, cap - elements);
			elements = first_free = cap = nullptr;
	}
}

void StrVec::push_back(const std::string &str)
{
	chk_n_alloc();
	alloc.construct(first_free++, str);
}

size_t StrVec::size()
{
	return first_free - elements;
}

size_t StrVec::capacity()
{
	return cap - elements;
}

void StrVec::resize(size_t n, const std::string &str)
{
	if(n <= size())
		return;

	while(size() < n)
	{
		push_back(str);
	}
}

std::string StrVec::at(size_t n)
{
	check(n, "at : out of range");
	return *(elements + n);
}

void StrVec::chk_n_alloc()
{
	if(size() == capacity())
	reallocate();
}

void StrVec::reallocate()
{
	auto sz = ((size() > 0) ? size() * 2 : 1);

	alloc_n_move(sz);
}

void StrVec::reserve(size_t sz)
{
	if(sz >= size())
	{
		alloc_n_move(sz);
	}
}

void StrVec::alloc_n_move(size_t sz)
{
	auto newp = alloc.allocate(sz);
	auto dest = newp;
	auto target = elements;
	while(target != first_free)
		alloc.construct(dest++, std::move(*target++));
	free();
	elements = newp;
	first_free = dest;
	cap = elements + sz;
}

void StrVec::check(size_t t, const std::string &msg)
{
	if(t >= first_free - elements)
		throw std::out_of_range(msg);
}

std::pair<std::string *, std::string *> StrVec::alloc_n_copy(std::string *b, std::string *e)
{
	auto p = alloc.allocate(e - b);
	auto ret = uninitialized_copy(b, e, p);
			
	return {p, ret};
}

std::allocator<std::string> StrVec::alloc;
