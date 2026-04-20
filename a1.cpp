#include <bits/stdc++.h>
using namespace std;

// Remove leading zeros
string strip(const string &s) {
    int i = 0;
    while (i < (int)s.size() - 1 && s[i] == '0') i++;
    return s.substr(i);
}

// Compare two numbers in base B
int cmp(const string &a, const string &b) {
    string x = strip(a), y = strip(b);
    if (x.size() != y.size()) return x.size() < y.size() ? -1 : 1;
    if (x == y) return 0;
    return x < y ? -1 : 1;
}

// School addition
string add(string a, string b, int base) {
    string res = "";
    int carry = 0;
    reverse(a.begin(), a.end());
    reverse(b.begin(), b.end());

    int n = max(a.size(), b.size());
    for (int i = 0; i < n; i++) {
        int da = i < a.size() ? a[i] - '0' : 0;
        int db = i < b.size() ? b[i] - '0' : 0;
        int sum = da + db + carry;
        carry = sum / base;
        res.push_back((sum % base) + '0');
    }
    if (carry) res.push_back(carry + '0');

    reverse(res.begin(), res.end());
    return strip(res);
}

// School subtraction (a >= b)
string sub(string a, string b, int base) {
    string res = "";
    reverse(a.begin(), a.end());
    reverse(b.begin(), b.end());

    int borrow = 0;
    for (int i = 0; i < (int)a.size(); i++) {
        int da = a[i] - '0';
        int db = i < b.size() ? b[i] - '0' : 0;
        int val = da - db - borrow;
        if (val < 0) {
            val += base;
            borrow = 1;
        } else borrow = 0;
        res.push_back(val + '0');
    }

    reverse(res.begin(), res.end());
    return strip(res);
}

// Shift (multiply by base^k)
string shift(const string &s, int k) {
    if (s == "0") return "0";
    return s + string(k, '0');
}

// Karatsuba multiplication
string karatsuba(string x, string y, int base) {
    x = strip(x);
    y = strip(y);

    if (x.size() == 1 && y.size() == 1) {
        int prod = (x[0] - '0') * (y[0] - '0');
        string res = "";
        while (prod > 0) {
            res.push_back((prod % base) + '0');
            prod /= base;
        }
        if (res.empty()) return "0";
        reverse(res.begin(), res.end());
        return res;
    }

    int n = max(x.size(), y.size());
    if (n % 2) n++;

    while (x.size() < n) x = "0" + x;
    while (y.size() < n) y = "0" + y;

    int m = n / 2;

    string x1 = x.substr(0, m);
    string x0 = x.substr(m);
    string y1 = y.substr(0, m);
    string y0 = y.substr(m);

    string z2 = karatsuba(x1, y1, base);
    string z0 = karatsuba(x0, y0, base);
    string z1 = karatsuba(add(x1, x0, base), add(y1, y0, base), base);

    z1 = sub(sub(z1, z2, base), z0, base);

    string res = add(add(shift(z2, 2 * (n - m)), shift(z1, n - m), base), z0, base);
    return strip(res);
}

// School division (integer division)
string divide(string dividend, string divisor, int base) {
    if (cmp(dividend, divisor) < 0) return "0";

    string quotient = "";
    string current = "";

    for (char c : dividend) {
        current += c;
        current = strip(current);

        int count = 0;
        while (cmp(current, divisor) >= 0) {
            current = sub(current, divisor, base);
            count++;
        }
        quotient.push_back(count + '0');
    }

    return strip(quotient);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string I1, I2;
    int B;
    cin >> I1 >> I2 >> B;

    string sum = add(I1, I2, B);
    string product = karatsuba(I1, I2, B);
    string quotient = divide(I1, I2, B);

    cout << sum << " " << product << " " << quotient << "\n";

    return 0;
}