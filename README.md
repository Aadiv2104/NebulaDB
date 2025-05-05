
# NebulaDB 2.0

NebulaDB is a lightweight C++ terminal-based user management system that mimics basic database functionality. 
It allows user registration, secure password storage using SHA-256 hashing, and basic record management.

## 🚀 Features

- User Registration with SHA-256 password hashing
- Record Insertion and Display
- Interactive Menu-based Interface
- Modular C++ OOP Design
- Persistent storage using JSON
- Basic Authentication System
- Ready for unit testing (extendable with Google Test)

## 📁 Project Structure

```
NebulaDB/
│
├── include/           # Header files
├── src/               # C++ source files
├── data/              # Persistent storage (JSON)
├── main.cpp           # Entry point with menu loop
└── README.md
```

## 🛠️ How to Compile & Run

Make sure you have g++ installed. Then:

```bash
g++ src/main.cpp src/sha256.cpp -Iinclude -o nebula.exe
./nebula.exe
```

## 🧠 Future Enhancements

- GUI or Web-based front-end
- Advanced Querying
- GoogleTest Integration
