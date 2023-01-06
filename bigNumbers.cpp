#include <string>
std::string addString(std::string copy, std::string _copy)
{
    if (copy.length() < _copy.length()) { copy = _copy; }
 
    int _iter = copy.size() - 1;
    for (int i = _copy.size() - 1; i >= 0; i--, _iter--)
        copy[_iter] += (_copy[i] - '0');

    for (int index = copy.size() - 1; index > 0; index--)  
        {
        if (copy[index] > '9')
           {
           copy[index - 1] = ((copy[index - 1] - '0') + ((int)copy[index] - '0') / 10) + '0';
           copy[index] = (((int)copy[index] - '0') % 10) + '0';
          }
        }
    if (copy[0] > '9')
    {
        std::string tempStr;
        tempStr += copy[0];
        copy[0] = ((copy[0] - '0')% 10) + '0';
        tempStr[0] = ((tempStr[0] - '0')/ 10) + '0';
        copy = tempStr + copy;
    }
    return copy;
}

std::string add(const std::string& a, const std::string& b) {

    std::string temp = a; std::string _temp = b;
    if (temp.length() < _temp.length())
    {
        for (int i = 0; i < _temp.length(); i++)
        {
            temp.insert(temp.begin(), '0');
        }
    }
     std::string sum = addString(temp, _temp);
    if(temp.length() <= 1 || _temp.length() <= 1) { return sum; }
    else { sum.erase(0, sum.find_first_not_of('0')); return sum;}
}
