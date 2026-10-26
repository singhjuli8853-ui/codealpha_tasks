# CodeAlpha Tasks

This repository contains C++ solutions for CodeAlpha tasks.

## Task 1: CGPA Calculator

**File:** `Task1_CGPA_Calculator/main.cpp`

### Features
- Accepts the number of courses.
- Reads grade points and credit hours for each course.
- Calculates each course's grade points (`grade × credits`).
- Displays individual course details, total credits, total grade points, and semester GPA.
- Shows GPA as CGPA when only one semester is provided.

### Compile and run
```bash
g++ Task1_CGPA_Calculator/main.cpp -o cgpa
./cgpa
```

> To calculate cumulative CGPA across multiple semesters, the program must also receive previous-semester totals.

## Task 2: Login and Registration System
See `Task2_Login_Registration/README.md` for features and build instructions.
