#include <iostream>
using namespace std;

int& highestMark(int marks[], int size) {
    int highest = 0;
    for (int i = 1; i < size; i++) {
        if (marks[i] > marks[highest]) {
            highest = i;
        }
    }
    return marks[highest];
}

// Do not call this function. It returns a reference to a local variable,
// which becomes invalid when the function ends (undefined behaviour).
int& unsafeLocalReference() {
    int value = 50;
    return value;
}

int main() {
    int marks[] = {78, 91, 85, 88, 82};
    cout << "Highest mark: " << highestMark(marks, 5) << '\n';

    highestMark(marks, 5) += 2;
    cout << "After moderation: " << highestMark(marks, 5) << '\n';

    const int& readOnlyHighest = highestMark(marks, 5);
    cout << "Const reference: " << readOnlyHighest << '\n';

    int copiedMark = highestMark(marks, 5);
    copiedMark = 0;
    cout << "Array value after changing copy: " << highestMark(marks, 5) << '\n';
    return 0;
}