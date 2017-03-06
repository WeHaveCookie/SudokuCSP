#pragma once
#include <stdlib.h>
#include "State.h"
#include <assert.h>

State::State(int initialValue, int row, int col)
{
	m_row = row;
	m_column = col;
	m_dirty = false;
	m_forced = false;
	
	if (initialValue > 0)
	{
		m_domain = nullptr;
		m_final = true;
		m_assignedNumber = initialValue;
		m_domainSize = 0;
	}
	else
	{
		m_domain = (bool *) calloc(11, sizeof(bool));
		m_final = false;
		m_domainSize = sudokuSize;
		m_assignedNumber = 0;
	}
}

State::State(const State& other){
	m_assignedNumber = other.m_assignedNumber;
	m_column = other.m_column;
	m_row = other.m_row;
	m_domainSize = other.m_domainSize;
	m_final = other.m_final;
	m_dirty = other.m_dirty;
	m_forced = other.m_forced;

	if (other.m_domain){
		m_domain = (bool *)calloc(11, sizeof(bool));
		for (int i = 0; i < 11; i++)
		{
			m_domain[i] = other.m_domain[i];
		}
	}
}

bool State::numberAvailable(int i)
{
	if (m_final)
	{
		return i == m_assignedNumber;
	}
	return !m_domain[i];
}

void State::removeFromDomain(int v)
{
	if (m_domain[v] == true)
	{
		return;
	}
	
	if(!isFinal())
	{
		m_domain[v] = true;
	}

	m_domainSize--;

	if (m_domainSize == 1){
		for (int i = 1; i <= sudokuSize; i++)
		{
			if (!m_domain[i]){
				m_assignedNumber = i;
				m_forced = true;
				return;
			}
		}
	}

	if (m_domainSize <= 0)
	{
		m_dirty = true;
	}
}

void State::consolidate()
{
	assert(m_assignedNumber > 0);
	m_final = true;
	m_domain = nullptr;
	//free(m_domain);
}

void State::forceAssignment(int i)
{
	m_forced = true;
	m_assignedNumber = i;
	for (int i = 1; i <= sudokuSize; i++){
		if (m_assignedNumber != i)
			removeFromDomain(i);
	}
}

void State::copyStateArray(State * a[sudokuSize][sudokuSize], State * b[sudokuSize][sudokuSize])
{
	for (int i = 0; i < sudokuSize; i++)
	{
		for (int j = 0; j < sudokuSize; j++)
		{
			State* currentState;
			if (a[i][j]->isFinal())
			{
				currentState = a[i][j];
			}
			else
			{
				currentState = new State(*a[i][j]);
			}
			b[i][j] = currentState;
		}
	}
}