#include "LFD.HPP"
#include <functional>
#include <string>
#include <iostream>

using Function = std::function<float(float)>;

Function LFD::generateFunction(const std::string& expression)
{
    return [expression](float x)
    {
        std::string findInExpression = expression;
        size_t pos = findInExpression.find("x");

        while (pos != std::string::npos)
        {
            if (pos > 0 && std::isdigit(findInExpression[pos - 1]))
            {
                int num = 0;
                size_t numStartPos = pos - 1;
                //find number before x
                while (numStartPos > 0 && std::isdigit(findInExpression[numStartPos - 1]))
                    numStartPos--;

                std::string numStr = findInExpression.substr(numStartPos, pos - numStartPos);
                num = std::stoi(numStr);
                //multiply x
                findInExpression.replace(numStartPos, pos - numStartPos + 1, std::to_string(num * static_cast<int>(x)));
                pos = numStartPos + std::to_string(num * static_cast<int>(x)).length();
            }
            else
            {
                //x doesnt have a preceding number
                findInExpression.replace(pos, 1, std::to_string(static_cast<int>(x)));
                pos += std::to_string(static_cast<int>(x)).length();
            }

            pos = findInExpression.find("x", pos);
        }

        try {
            return std::stof(findInExpression);
        }
        catch (const std::exception& e)
        {
            std::cerr << "Error evaluating expression: " << e.what() << std::endl;
            return 0.0f;
        }
    };
}
