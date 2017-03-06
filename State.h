#pragma once
#define sudokuSize 9

class State
{
public:
	State(int initialValue, int row, int col);
	State(const State& other);

	bool numberAvailable(int i);
	void removeFromDomain(int v);
	void consolidate();
	void forceAssignment(int i);
	const int getRow() const { return m_row; }
	const int getColumn() const { return m_column; }
	const int assignedValue() const { return m_assignedNumber; }
	const bool isDirty() const { return m_dirty; }
	const bool isForced() const { return m_forced; }
	const bool isFinal() const { return m_final; }
	
	static void copyStateArray(State * a[sudokuSize][sudokuSize], State * b[sudokuSize][sudokuSize]);
	static const bool compareTo(State& a, State& b) { return a.m_domainSize > b.m_domainSize; }

private :
	bool*	m_domain;
	int		m_domainSize;
	bool	m_final;
	bool	m_dirty;
	bool	m_forced;
	int		m_assignedNumber;
	int		m_row;
	int		m_column;
};

struct StateCompare {
	bool operator() (State& a, State& b) const {
		return State::compareTo(a, b);
	}
};