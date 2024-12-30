#include <queue>
#include <stack>
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

using namespace std;

char reserved = 'x';

//Full usage of AI
static string replaceWithQueue(const string& str, queue<int>& q) {
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
static string replaceReserved(const string given, queue<int>& q) {
    queue<int> copy = q;
    stack<int> s;

    string str(given.rbegin(), given.rend());
    // Process the string and replace the reserved character with stack values
    str = replaceWithQueue(str, q);

    reverse(str.begin(), str.end());

    return str;
}


// Full usage of AI for this function
static int parseIntExpressionWithoutVariables(const string& expression) {
    int result = 0;
    int currentNumber = 0;
    char lastOperation = '+'; // To keep track of the last operation

    for (char ch : expression) {
        if (isdigit(ch)) {
            currentNumber = currentNumber * 10 + (ch - '0');
        }
        else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%') {
            switch (lastOperation) {
            case '+': result += currentNumber; break;
            case '-': result -= currentNumber; break;
            case '*': result *= currentNumber; break;
            case '/': result /= currentNumber; break;
            case '%': result %= currentNumber; break;
            }
            currentNumber = 0;
            lastOperation = ch;
        }
    }

    // Handle the last number
    switch (lastOperation) {
    case '+': result += currentNumber; break;
    case '-': result -= currentNumber; break;
    case '*': result *= currentNumber; break;
    case '/': result /= currentNumber; break;
    case '%': result %= currentNumber; break;
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

// Function created by me + research
static int handleExpression(const string& str) {
    vector<vector<string>> order;
    vector<int> clones;
    string currWord;
    int depth = 0;
    string depthZero = "";
    bool isString = false;

    for (char ch : str) {
        cout << ch << " " << depth << '\n';
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
    queue<int> previousValues;
    for (int i = order.size() - 1; i >= 0; i--) {
        for (int z = order[i].size() - 1; z >= 0; z--) {
            string filled = replaceReserved(order[i][z], previousValues);
            int val = parseIntExpressionWithoutVariables(filled);
            previousValues.push(val);
        }
    }

    // ## For the first depth ##
    string filled = replaceReserved(depthZero, previousValues);
    int val = parseIntExpressionWithoutVariables(filled);
    return val;
}

int main() {
    // Sample input: (5 * (5 * 20 / 5) + (50 * 5) + (100*2)) + (5 / (((1*1))))
    while (true) {
        string expression;
        getline(cin, expression);
        cout << expression << '\n';
        cout << handleExpression(expression) << '\n';
    }
    return 0;
}
