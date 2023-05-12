#pragma once

#include "element.hpp"

#ifndef ELEMENTS_HPP
#define ELEMENTS_HPP

struct
Elements
{

	std::vector<Element> m_elements;

    void addElement(Element e)
    {
        for (const Element& existingElement : m_elements)
        {
            if (existingElement.elementCoordinates == e.elementCoordinates)
            {
                std::cerr << "This element already exists.\n";
                return;
            }
        }

        if (e.elementCoordinates.size() < 2)
        {
            std::cerr << "Invalid element. There must be a minimum of 2 coordinates.\n";
            return;
        }

        m_elements.push_back(e);
    }

};

#endif
