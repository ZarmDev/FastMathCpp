#include <queue>
#include <stack>
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>

using namespace std;

char reserved = 'x';

//Full usage of AI
static string replaceWithQueue(const string& str, queue<float>& q) {
    string result;
    for (char ch : str) {
        if (ch == reserved && !q.empty()) {
            string r = to_string(q.front());
            reverse(r.begin(), r.end());
            result += r;
            q.pop();
        }
        else {
            result += ch;
        }
    }
    return result;
}


// Made by me with the help of AI/research
static string replaceReserved(const string given, queue<float>& q) {
    queue<float> copy = q;
    stack<float> s;

    string str(given.rbegin(), given.rend());
    // Process the string and replace the reserved character with stack values
    str = replaceWithQueue(str, q);

    reverse(str.begin(), str.end());

    return str;
}

static bool isInteger(float value) {
    return floor(value) == value;
}

// AI + me
static float parseIntExpressionWithoutVariables(const string& expression) {
    float result = 0.0;
    float currentNumber = 0;
    float decimalMultiplier = 0;
    char lastOperation = '+'; // To keep track of the last operation

    for (char ch : expression) {
        if (isdigit(ch)) {
            if (decimalMultiplier > 0) {
                currentNumber += (ch - '0') * decimalMultiplier;
                decimalMultiplier *= 0.1;
            }
            else {
                currentNumber = currentNumber * 10 + (ch - '0');
            }
        }
        else if (ch == '.') {
              decimalMultiplier = 0.1;
        }
        else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%') {
            switch (lastOperation) {
            case '+': result += currentNumber; break;
            case '-': result -= currentNumber; break;
            case '*': result *= currentNumber; break;
            case '/': result /= currentNumber; break;
            case '%':
                if (isInteger(result) && isInteger(currentNumber)) {
                    result = static_cast<int>(result) % static_cast<int>(currentNumber);
                }
                break;
            }
            currentNumber = 0;
            decimalMultiplier = 0;
            lastOperation = ch;
        }
    }

    // Handle the last number
    switch (lastOperation) {
    case '+':
        result += currentNumber;
        break;
    case '-': result -= currentNumber; break;
    case '*': result *= currentNumber; break;
    case '/': result /= currentNumber; break;
    case '%':
        if (isInteger(result) && isInteger(currentNumber)) {
            result = static_cast<int>(result) % static_cast<int>(currentNumber);
        }
        break;
    }

    return result;
}

// (Full usage of AI) Function to ensure the vector is resized properly before accessing an element 
template <typename T> void ensureCapacity(std::vector<T>& vec, size_t index)
{
    if (index >= vec.size())
    {
        vec.resize(index + 1);
    }
}

vector<string> methods = {"int", "str", "float"};

// Function created by me + research
static float handleExpression(const string& str) {
    vector<vector<string>> order;
    vector<int> clones;
    string currWord;
    int depth = 0;
    string depthZero = "";
    bool isString = false;

    for (char ch : str) {
        //cout << ch << " " << depth << '\n';
        if (depth == 0) {
            if (ch == '(') {
                depthZero += reserved;
                depth++;
            }
            else if (ch == ')') {
                depthZero += ch;
                depth--;
            }
            else {
                depthZero += ch;
            }
            continue;
        }
        else if (ch == '(') {
            // TODO: This skips unneccessary computations but can confuse the compiler as well. See what works.
            // if (currWord == "") {
            //   continue;
            // }
            currWord += reserved;
            ensureCapacity(order, depth);
            ensureCapacity(clones, depth);
            int clone = clones[depth];
            ensureCapacity(order[depth], clone);
            order[depth][clone] += currWord;
            currWord = "";
            depth++;
        }
        else if (ch == ')') {
            ensureCapacity(order, depth);
            ensureCapacity(clones, depth);
            int clone = clones[depth];
            clones[depth] += 1;
            ensureCapacity(order[depth], clone);
            order[depth][clone] += currWord;
            currWord = "";
            depth--;
        }
        else {
            currWord += ch;
        }
    }

    // ## Actually calculating the parenthese values ##
    queue<float> previousValues;
    for (int i = order.size() - 1; i >= 0; i--) {
        for (int z = order[i].size() - 1; z >= 0; z--) {
            string filled = replaceReserved(order[i][z], previousValues);
            float val = parseIntExpressionWithoutVariables(filled);
            previousValues.push(val);
        }
    }

    // ## For the first depth ##
    string filled = replaceReserved(depthZero, previousValues);
    float valAtZero = parseIntExpressionWithoutVariables(filled);
    return valAtZero;
}

int main() {
    // Sample input: (5 * (5 * 20 / 5) + (50 * 5) + (100*2)) + (5 / (((1*1))))
    while (true) {
        string expression;
        getline(cin, expression);
        cout << fixed << handleExpression(expression) << '\n';
    }
    return 0;
}
