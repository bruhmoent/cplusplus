#pragma once

#include <functional>
#include <vector>

#ifndef TURNS_HPP
#define TURNS_HPP

struct
	Turns
{

	Turns(int players) : m_players(players) {}

	using FunctionType = std::function<void()>;
	std::vector<FunctionType> m_functions;
	int m_players;

	void addFunction(const FunctionType& function)
	{
		m_functions.push_back(function);
	}

	void take_turns(int functionIndex)
	{

		for (int i = 0; i < m_players; i++)
		{
			m_functions[functionIndex]();
		}

	}

};

#endif