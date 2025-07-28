#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <map>
#include <algorithm>
#include <cctype>

using namespace std;


long long convertToDecimal(const string& value, int base) {
    long long result = 0;
    for (size_t i = 0; i < value.size(); ++i) {
        char c = value[i];
        int digit = isdigit(c) ? (c - '0') : (tolower(c) - 'a' + 10);
        result = result * base + digit;
    }
    return result;
}


long long lagrangeInterpolation(const vector< pair<long long, long long> >& points) {
    long double result = 0.0;
    int k = points.size();

    for (int i = 0; i < k; ++i) {
        long double term = points[i].second;
        for (int j = 0; j < k; ++j) {
            if (i != j) {
                term *= (-1.0 * points[j].first);
                term /= (points[i].first - points[j].first);
            }
        }
        result += term;
    }

    return llround(result);  // round to nearest integer
}
