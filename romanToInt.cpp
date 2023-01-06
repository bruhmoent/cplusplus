#include <string>
#include <unordered_map>
class Solution {
public:
    int romanToInt(string s) {
    unordered_map<char, int> roman_values = {{'I', 1}, {'V', 5}, {'X', 10}, {'L', 50}, {'C', 100}, {'D', 500}, {'M', 1000}};
    int result = 0;
    for (int i = 1; i < s.size(); i++) {
        if (roman_values[s[i]] > roman_values[s[i - 1]]) {
            result -= roman_values[s[i - 1]];
        }
        else {
            result += roman_values[s[i - 1]];
        }
    }
    result += roman_values[s[s.size() - 1]];
    return result;
}
};
