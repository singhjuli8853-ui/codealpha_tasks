# Task 2: Login and Registration System

A beginner-friendly C++ console program for user registration and login.

## Features
- Register with a unique username and password.
- Validate usernames (3-20 characters: letters, digits, `_`, `-`).
- Require passwords to be at least 8 characters.
- Save a random salt and a PBKDF2-HMAC-SHA-256 password hash in `users.txt`; the original password is not stored.
- Verify login credentials and display success/error messages.

## Requirement
This version uses OpenSSL's crypto library.

### Compile with g++ (Linux / MinGW with OpenSSL installed)
```bash
g++ main.cpp -o login_system -lcrypto
```

On Windows, install an OpenSSL development package and configure its include/library paths in your compiler or IDE. If your setup does not have OpenSSL, the program will not compile until it is installed and linked.

## Run
```bash
./login_system
```
On Windows, run `login_system.exe`.

The program creates `users.txt` in the current working directory on first successful registration. Each record contains:
`username salt_hex password_hash_hex`

## Security note
This is an educational console project. PBKDF2 with a random salt is much safer than storing plain-text passwords, but production applications should use a well-maintained authentication framework, carefully manage file permissions, and consider a modern password-hashing configuration such as Argon2id.
