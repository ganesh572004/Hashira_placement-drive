#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <climits>
// To use nlohmann::json, install it and set your include path, or use single-header version:
// Download https://github.com/nlohmann/json/releases/latest/download/json.hpp and place in your project directory
#include "json.hpp" // If you downloaded json.hpp locally
using namespace std;
using json = nlohmann::json;
using namespace std;
using json = nlohmann::json;

// Convert string in base 'b' to decimal (long long)
long long decodeBase(const string &val, int base) {
    long long num = 0;
    for (char c : val) {
        int digit;
        if (isdigit(c)) digit = c - '0';
        else if (isalpha(c)) digit = 10 + (tolower(c) - 'a');
        else continue;
        num = num * base + digit;
    }
    return num;
}

// Compute Lagrange interpolation at x=0 → constant term
long long lagrangeConstant(const vector<pair<long long,long long>>& points, int k) {
    long long mod = LLONG_MAX; // no modulus (big numbers allowed)
    long double secret = 0.0;

    for (int i=0;i<k;i++) {
        long double xi = points[i].first;
        long double yi = points[i].second;
        long double term = yi;
        for (int j=0;j<k;j++) {
            if (i==j) continue;
            long double xj = points[j].first;
            term *= (-xj) / (xi - xj);
        }
        secret += term;
    }
    return (long long) round(secret);
}

int main() {
    ifstream in("input2.json");
    json input;
    in >> input;

    int n = input["keys"]["n"];
    int k = input["keys"]["k"];

    vector<pair<long long,long long>> points;

    for (auto& el : input.items()) {
        if (el.key() == "keys") continue;
        long long x = stoll(el.key());
        int base = stoi(el.value()["base"].get<string>());
        string val = el.value()["value"];
        long long y = decodeBase(val, base);
        points.push_back({x,y});
    }

    sort(points.begin(), points.end()); // sort by x
    long long secret = lagrangeConstant(points, k);

    // Output JSON
    json output;
    output["secret_c"] = secret;

    ofstream out("output3.json");
    out << output.dump(4);
    return 0;
}
