#pragma once
#include <unordered_set>
#include <queue>
#include "Link.h"

struct Hasher {
	size_t operator() (const Link& a) const { return Link::hash(a); }
};

struct Equal {
	bool operator() (const Link& a, const Link& b) const { return Link::equalTo(a, b); }
};

class QueueSet
{

public:
	QueueSet();
	void push(Link element);
	Link pop();
	const bool contains(Link element) const { return !(m_set.find(element) == m_set.end()); }
	const int size() const { return m_size; }

private:
	std::queue<Link> m_queue;
	std::unordered_set<Link, Hasher, Equal> m_set;
	int m_size;
};