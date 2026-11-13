#include <iostream>
#include <vector>
using namespace std;

int& highestMark(int marks[], int n) {
    int best = 0;
    for (int i = 1; i < n; ++i) if (marks[i] > marks[best]) best = i;
    return marks[best];
}
// Deliberately unsafe demonstration. Do not call this function.
int& danglingReferenceDemo() {
    int local = 42;
    return local; // compiler warning: reference to local variable returned
}
int highestIndex(const int marks[], int n) {
    int best = 0;
    for (int i = 1; i < n; ++i) if (marks[i] > marks[best]) best = i;
    return best;
}
int* highestPointer(int marks[], int n) {
    int best = highestIndex(marks, n);
    return &marks[best];
}
int main() {
    int marks[] = {78, 91, 84, 88, 91};
    highestMark(marks, 5) += 2;
    cout << "highest after moderation: " << highestMark(marks, 5) << '\n';
    int idx = highestIndex(marks, 5);
    cout << "index: " << idx << ", value: " << marks[idx] << '\n';
    int* p = highestPointer(marks, 5);
    cout << "pointer value: " << *p << '\n';

    vector<int> dynamicMarks = {65, 82, 79};
    int& oldReference = dynamicMarks[1];
    dynamicMarks.push_back(90); // may reallocate; oldReference may now dangle
    cout << "vector size after growth: " << dynamicMarks.size() << '\n';
    // Do not access oldReference after push_back: it may be invalid.
}
