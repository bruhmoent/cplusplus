#pragma once

#include <iostream>
#include <vector>

#ifndef ELEMENT_HPP
#define ELEMENT_HPP

struct
Element
{

    std::vector<std::pair<int, int>> elementCoordinates;
    int health;

    Element(std::vector<std::pair<int, int>> coordinates, int hlth) : elementCoordinates(coordinates), health(hlth) {}

};

#endif
