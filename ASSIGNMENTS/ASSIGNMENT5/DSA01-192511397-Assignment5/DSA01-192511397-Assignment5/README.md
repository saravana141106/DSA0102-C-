# DSA01-192511397-Assignment5

**Name:** H SARAVANA
**Register Number:** 192511397
**Course:** DSA01 – CO5 Assignment 5: Reliable Software with Runtime Polymorphism and Exceptions
**Nature:** Individual

## Environment

| Item | Value |
|---|---|
| Compiler | g++ 13.3.0 (Ubuntu 13.3.0-6ubuntu2~24.04.1) |
| OS / CPU | Ubuntu 24.04 (x86_64), Intel Xeon @ 2.10 GHz, 1 vCPU, 4 GB RAM |
| Standard flags | `g++ -std=c++17 -Wall -Wextra` |
| Sanitizers | `-fsanitize=address` (and `-fsanitize=address,undefined` where stated) |
| Optimisation for timings | `-O2` |

## Folder layout (one folder per task)

| Folder | Covers | Main files |
|---|---|---|
| `Task01_Binding_VirtualDtor/` | A2.2, A2.3, B1.3, B1.4 | `predict_a2.cpp`, `nonvirtual_dtor.cpp`, `nonvirtual_total.cpp` |
| `Task02_RAII_Unwinding/` | A5 predict, rethrow, `throw e;` | `predict_a5.cpp`, `rethrow_chain.cpp`, `rethrow_by_value.cpp` |
| `Task03_Billing_CompleteCode/` | B1 Complete the Code | `billing_b1.cpp`, `remove_virtual_total.cpp` (compile-error check) |
| `Task04_ErrorHandling/` | B2 predict, Complete the Code, error tests | `predict_b2.cpp`, `claim_settle.cpp`, `claim_settle_throw_e.cpp`, `error_tests.cpp`, `optional_vs_exception.cpp` |
| `Task05_Performance/` | B3 measurements | `bench.cpp` |
| `Task06_Extension_Final/` | B1.1 verification, B4.1 extension, B4.2 sanitizer run | `billing.hpp`, `extension.hpp`, `main.cpp` |
| `report/` | Report source, UML and vtable diagrams | `Assignment5_Report_192511397.tex`, `*.png`, `*.dot` |

Each folder contains the source, the captured output (`*.out`), and sanitizer or compiler logs where relevant.

## How to compile and run each program

Run from the repository root. The exact commands are shown per task.

```bash
# Task 01 – binding and virtual destructor
cd Task01_Binding_VirtualDtor
g++ -std=c++17 -Wall -Wextra predict_a2.cpp -o predict_a2 && ./predict_a2
g++ -std=c++17 -Wall -Wextra nonvirtual_dtor.cpp -o nonvirtual_dtor && ./nonvirtual_dtor
g++ -std=c++17 -g -fsanitize=address nonvirtual_dtor.cpp -o nonvirtual_dtor_asan && ./nonvirtual_dtor_asan
g++ -std=c++17 -Wall -Wextra nonvirtual_total.cpp -o nonvirtual_total && ./nonvirtual_total
cd ..

# Task 02 – stack unwinding and rethrow
cd Task02_RAII_Unwinding
for f in predict_a5 rethrow_chain rethrow_by_value; do g++ -std=c++17 -Wall -Wextra $f.cpp -o $f && ./$f; done
cd ..

# Task 03 – completed B1 program and compile-error check
cd Task03_Billing_CompleteCode
g++ -std=c++17 -Wall -Wextra billing_b1.cpp -o billing_b1 && ./billing_b1
g++ -std=c++17 -Wall -Wextra remove_virtual_total.cpp -o /dev/null   # expected to FAIL (see compile_error_remove_virtual.txt)
cd ..

# Task 04 – error handling
cd Task04_ErrorHandling
g++ -std=c++17 -Wall -Wextra predict_b2.cpp -o predict_b2 && ./predict_b2
g++ -std=c++17 -Wall -Wextra claim_settle.cpp -o claim_settle && ./claim_settle
g++ -std=c++17 -Wall -Wextra claim_settle_throw_e.cpp -o claim_settle_throw_e && ./claim_settle_throw_e
g++ -std=c++17 -Wall -Wextra error_tests.cpp -o error_tests && ./error_tests
g++ -std=c++17 -O2 -Wall -Wextra optional_vs_exception.cpp -o optional_vs_exception && ./optional_vs_exception
cd ..

# Task 05 – performance (B3)
cd Task05_Performance
g++ -std=c++17 -O2 -Wall -Wextra bench.cpp -o bench && ./bench
cd ..

# Task 06 – billing library, extension and tests (B1.1, B4.1, B4.2)
cd Task06_Extension_Final
g++ -std=c++17 -Wall -Wextra main.cpp -o billing_tests && ./billing_tests
g++ -std=c++17 -g -fsanitize=address,undefined main.cpp -o billing_tests_asan && ./billing_tests_asan
cd ..
```

Sanitizer note: `error_tests.cpp` allocates 2^60 doubles to force `std::bad_alloc`. Run it under ASan with `ASAN_OPTIONS=allocator_may_return_null=1` if the default ASan allocator aborts instead of throwing.

## Results summary

| Task | Result |
|---|---|
| A2 predict program | Output `500 ~OPBill ~Bill` (matches the derived prediction) |
| Non-virtual destructor (B1.4) | Only `~Bill` runs; ASan reports `new-delete-type-mismatch` |
| A5 predict program | Output `~f ~main caught 42`; `not printed` never appears |
| B1 completed program | `OP Ravi: 1700`, `IP Meena: 47500`, `Total 49200` (matches expected) |
| Remove `virtual` from pure `total()` | Compile error: initializer specified for non-virtual method |
| B2 predict program | Output `ok done caught negative [log too large] caught too large` |
| B2 claim settlement | Output matches expected 4 lines; `throw e;` variant changes the handler that catches the claim |
| B2 error tests | Domain errors, `std::stod`, `std::bad_alloc`, top-level `catch(...)` all behave as designed |
| B3 performance | Switch fastest, `std::variant` second, virtual calls slowest at 10,000,000 bills; exceptions about 1 µs per rejected bill |
| B1.1 / B4.1 test driver | 24 checks PASS, 0 failures; ASan + UBSan clean |

## Notes and limitations

- The GitHub repository and the report cover page link must be created under the student's own GitHub account. This repository was prepared locally and has not been pushed.
- Predicted outputs must be written by the student before running each program, as the assignment requires. The predictions recorded in the report were derived from the C++ rules and should be re-entered before execution for credit.
- Timings depend on the machine. Each figure is the median of 5 runs; repeat runs on the same machine varied by roughly 5–10 %.
