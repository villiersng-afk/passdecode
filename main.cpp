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

// Convert number in a given base (up to 16) to decimal
long long convertToDecimal(const string& value, int base) {
    long long result = 0;
    for (char c : value) {
        int digit = isdigit(c) ? (c - '0') : (tolower(c) - 'a' + 10);
        result = result * base + digit;
    }
    return result;
}

// Lagrange Interpolation to find f(0), i.e., the constant term
long long lagrangeInterpolation(const vector<pair<long long, long long>>& points) {
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

    return llround(result);
}

// Function to extract points from JSON string
vector<pair<long long, long long>> extractPoints(const string& json, int& k_out) {
    int n = 0, k = 0;
    size_t pos = json.find("\"keys\"");
    if (pos != string::npos) {
        size_t n_pos = json.find("\"n\"", pos);
        size_t k_pos = json.find("\"k\"", pos);
        n = stoi(json.substr(json.find(":", n_pos) + 1));
        k = stoi(json.substr(json.find(":", k_pos) + 1));
    }
    k_out = k;

    vector<pair<long long, long long>> points;
    for (int i = 1; i <= n; ++i) {
        string key = "\"" + to_string(i) + "\"";
        size_t key_pos = json.find(key);
        if (key_pos == string::npos) continue;

        size_t base_pos = json.find("\"base\"", key_pos);
        size_t val_pos = json.find("\"value\"", base_pos);
        if (base_pos == string::npos || val_pos == string::npos) continue;

        // Extract base
        size_t colon1 = json.find(":", base_pos);
        size_t comma = json.find(",", colon1);
        string base_str = json.substr(colon1 + 1, comma - colon1);
        int base = stoi(base_str);

        // Extract value
        size_t val_start = json.find("\"", val_pos + 8) + 1;
        size_t val_end = json.find("\"", val_start);
        string val_str = json.substr(val_start, val_end - val_start);

        long long y = convertToDecimal(val_str, base);
        points.push_back({i, y});

        if (points.size() == k) break;
    }

    return points;
}

// Main function
int main() {
    vector<string> files = {"testcase1.json", "testcase2.json"};

    for (int i = 0; i < files.size(); ++i) {
        ifstream infile(files[i]);
        if (!infile.is_open()) {
            cerr << "Failed to open file: " << files[i] << endl;
            continue;
        }

        stringstream buffer;
        buffer << infile.rdbuf();
        string json = buffer.str();
        infile.close();

        int k;
        vector<pair<long long, long long>> points = extractPoints(json, k);
        long long secret = lagrangeInterpolation(points);

        cout << "Secret for Testcase " << (i + 1) << ": " << secret << endl;
    }

    return 0;
}

