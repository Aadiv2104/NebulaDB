#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <stdexcept>
#include<sha256.h>
#include <cstdio>
#include<sstream>

using namespace std;

// Record class to represent one row
class Record {
private:
    vector<string> fields;

public:
    Record(const vector<string>& fields) : fields(fields) {}

    void display() const {
        for (const auto& field : fields) {
            cout << field << " | ";
        }
        cout << endl;
    }

    vector<string> getFields() const {
        return fields;
    }
};




// Table class to represent a table of records
class Table {
    private:
        string tableName;
        vector<string> columns;
        vector<Record> records;
    
    public:
        Table(const string& name, const vector<string>& cols)
            : tableName(name), columns(cols) {}
    
        void insertRecord(const vector<string>& fields) {
            if (fields.size() != columns.size()) {
                cout << "Error: Field count doesn't match column count!" << endl;
                return;
            }
            records.push_back(Record(fields));
        }

        void displayRecords() const {
            for (const auto& rec : records) {
                  rec.display();
            }
        }
            
        void selectAll() const {
                if (records.empty()) {
                    cout << "No records to display." << endl;
                    return;
                }
                displayRecords();
        }
            
        void displayTable() const {
                cout << "Table: " << tableName << endl;
                for (const auto& col : columns) {
                    cout << col << " | ";
                }
                cout << endl;
                cout << string(50, '-') << endl;
                displayRecords();
        }
            
        void deleteRecordByName(const string& name) {
            bool found = false;
            for (auto it = records.begin(); it != records.end(); ++it) {
                if ((*it).getFields()[0] == name) { // [0] means Name column
                    records.erase(it);
                    found = true;
                    cout << "Record with name \"" << name << "\" deleted successfully." << endl;
                    break;
                }
            }
            if (!found) {
                cout << "Record with name \"" << name << "\" not found." << endl;
            }
        }

        void updateRecordByName(const string& name, const vector<string>& newFields) {
            bool found = false;
            if (newFields.size() != columns.size()) {
                cout << "Error: Field count doesn't match column count!" << endl;
                return;
            }
            for (auto& rec : records) {
                if (rec.getFields()[0] == name) { // Match based on Name
                    // Update the record's fields with the new ones
                    rec = Record(newFields);
                    found = true;
                    cout << "Record with name \"" << name << "\" updated successfully." << endl;
                    break;
                }
            }
            if (!found) {
                cout << "Record with name \"" << name << "\" not found." << endl;
            }
        }

        void searchRecord(const string& name) const {
            bool found = false;
            for (const auto& rec : records) {
                if (rec.getFields()[0] == name) { // fields[0] is Name column
                    cout << "Record found: ";
                    rec.display();
                    found = true;
                    break;
                }
            }
            if (!found) {
                cout << "Record with name \"" << name << "\" not found." << endl;
            }
        }

        void saveToFile(const string& filename) const {
            ofstream file(filename);
            if (!file) {
                cout << "Error: Could not open file: " <<filename<< endl;
                return;
            }
        
            // Save column headers
            for (size_t i=0; i<columns.size(); ++i) {
                file << columns[i];
                if(i!=columns.size()-1){
                    file<<",";
                }
                }
                file << endl;
        
            // Save each record
            for (const auto& rec : records) {
                const auto& fields = rec.getFields();
                for(size_t i=0; i<fields.size(); ++i) {
                    file << fields[i];
                    if(i!= fields.size()-1){
                        file<<",";
                    }
                }
                file << endl;
            }
        
            cout << "Table saved to " << filename << " successfully!" << endl;
        }

        void loadFromFile(const string& filename) {
            ifstream file(filename);
             if (!file) {
                cout << "Error: Could not open file:" <<filename<< endl;
                return;
            }
        
            records.clear(); // Clear any existing records
        
            string line;
            bool firstLine = true;
        
            while (getline(file, line)) {
                vector<string> fields;
                string field;
                for (char ch : line) {
                    if (ch == ',') {
                        fields.push_back(field);
                        field.clear();
                    } else {
                        field += ch;
                    }
                }
        
                if (!fields.empty()){
                    fields.push_back(field);
                }
                if (firstLine) {
                        columns = fields;  // update columns
                        firstLine = false;
                        continue;
                }
                if (!fields.empty()){
                    records.push_back(Record(fields));
            }
        }
        
            cout << "Table loaded from " << filename << " successfully!" << endl;
        }

        void sortByAge(bool ascending = true) {
            if (records.empty()) {
                cout << "No records to sort." << endl;
                return;
            }
        
            sort(records.begin(), records.end(), [ascending](const Record& a, const Record& b) {
                int ageA = stoi(a.getFields()[1]); // Age is the second column (index 1)
                int ageB = stoi(b.getFields()[1]);
                
                if (ascending) {
                    return ageA < ageB;
                } else {
                    return ageA > ageB;
                }
            });
        
            cout << "Records sorted by Age " << (ascending ? "(Ascending)" : "(Descending)") << " successfully!" << endl;
        }

        void filterByCityAndAge(const string& cityName, int minAge) const {
            cout << "Users from city: " << cityName << " and age greater than " << minAge << endl;
            bool found = false;
            for (const auto& rec : records) {
                int age = stoi(rec.getFields()[1]);  // Convert age from string to int
                if (rec.getFields()[2] == cityName && age > minAge) {
                    rec.display();
                    found = true;
                }
            }
            if (!found) {
                cout << "No records found matching the filter." << endl;
            }
        }
    };



    class User {
        private:
            string username;
            string password;
        
        public:
            string sha256(const string& str) const {
                return ::sha256(str);  // Fixed: Call global function to hash
            }
        
            // Default constructor
            User() : username(""), password("") {}
        
            // Constructor with validation
            User(const string& username, const string& password) {
                setUsername(username);
                setPassword(password);
            }
        
            string getUsername() const { return username; }
            string getPassword() const { return password; }
        
            // Set username with validation
            void setUsername(const string& newUsername) {
                if (newUsername.empty()) {
                    throw invalid_argument("Username cannot be empty.");
                }
                username = newUsername;
            }
        
            // Set password with validation and hashing
            void setPassword(const string& newPassword) {
                if (newPassword.length() < 6) {
                    throw invalid_argument("Password must be at least 6 characters long.");
                }
        
                bool hasUpper = false, hasLower = false, hasDigit = false, hasSpecial = false;
        
                for (char c : newPassword) {
                    if (isupper(c)) hasUpper = true;
                    if (islower(c)) hasLower = true;
                    if (isdigit(c)) hasDigit = true;
                    if (ispunct(c)) hasSpecial = true;
                }
        
                if (!hasUpper || !hasLower || !hasDigit || !hasSpecial) {
                    throw invalid_argument("Password must contain an uppercase letter, a lowercase letter, a digit, and a special character.");
                }
        
                password = sha256(newPassword);  // Hash and store
            }
        
            bool verifyPassword(const string& inputPassword) const {
                return sha256(inputPassword) == password;
            }
        };
        

    
   // Authentication System Class
class AuthSystem {
    private:
        unordered_map<string, User> users;
    
    public:
        AuthSystem() {
            loadUsersFromFile();  // Load users from file when system starts
        }
    
        bool registerUser(const string& username, const string& password) {
            if (users.find(username) != users.end()) {
                cout << "Username already exists!" << endl;
                return false;
            }
            users[username] = User(username, password);
            saveUsersToFile();
            cout << "User registered successfully!" << endl;
            return true;
        }
    
        bool loginUser(const string& username, const string& password) {
            if (users.find(username) == users.end()) {
                cout << "Username not found!" << endl;
                return false;
            }
            if (users[username].verifyPassword(password)) {
                cout << "Login successful!" << endl;
                return true;
            } else {
                cout << "Incorrect password!" << endl;
                return false;
            }
        }
    
        void saveUsersToFile() const {
            ofstream file("users.txt");
            if (!file) {
                cout << "Error: Could not open users.txt" << endl;
                return;
            }
    
            for (const auto& [username, user] : users) {
                file << username << "," << user.getPassword() << endl;
            }
    
            file.close();
        }
    
        void loadUsersFromFile() {
            ifstream file("users.txt");
            if (!file) {
                cout << "No users.txt found. Starting fresh." << endl;
                return;
            }
    
            string line;
            while (getline(file, line)) {
                stringstream ss(line);
                string username, password;
                if (getline(ss, username, ',') && getline(ss, password)) {
                    users[username] = User(username, password);
                }
            }
    
            file.close();
        }
    };
    
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;

// Load users from file
std::map<std::string, std::string> loadUsers() {
    std::ifstream infile("data/users.json");
    json j;
    std::map<std::string, std::string> users;
    if (infile) {
        infile >> j;
        for (auto& [user, pass] : j.items()) {
            users[user] = pass;
        }
    }
    return users;
}

// Save users to file
void saveUsers(const std::map<std::string, std::string>& users) {
    std::ofstream outfile("data/users.json");
    json j(users);
    outfile << j.dump(4);
}

int main() {
    Table table("Users", {"Name", "Email", "Age"});
    std::map<std::string, std::string> users = loadUsers();
    std::string currentUser;

    int choice;
    bool running = true;

    while (running) {
        std::cout << "\nNebulaDB CLI\n";
        std::cout << "1. Register\n2. Login\n3. Exit\n> ";
        std::cin >> choice;
        std::cin.ignore();

        if (choice == 1) {
            std::string username, password;
            std::cout << "New username: ";
            std::getline(std::cin, username);
            std::cout << "New password: ";
            std::getline(std::cin, password);
            users[username] = User().sha256(password);
            saveUsers(users);
            std::cout << "User registered.\n";
        } else if (choice == 2) {
            std::string username, password;
            std::cout << "Username: ";
            std::getline(std::cin, username);
            std::cout << "Password: ";
            std::getline(std::cin, password);

            if (users.count(username) && users[username] == User().sha256(password)) {
                std::cout << "Login successful!\n";
                currentUser = username;
                break;
            } else {
                std::cout << "Invalid credentials.\n";
            }
        } else if (choice == 3) {
            running = false;
        } else {
            std::cout << "Invalid input.\n";
        }
    }

    if (!currentUser.empty()) {
        while (true) {
            std::cout << "\n1. Add Record\n2. View Records\n3. Logout\n> ";
            std::cin >> choice;
            std::cin.ignore();

            if (choice == 1) {
                std::string name, email, age;
                std::cout << "Name: "; std::getline(std::cin, name);
                std::cout << "Email: "; std::getline(std::cin, email);
                std::cout << "Age: "; std::getline(std::cin, age);
                table.insertRecord({name, email, age});
            } else if (choice == 2) {
                table.displayTable();
            } else if (choice == 3) {
                std::cout << "Logged out.\n";
                break;
            } else {
                std::cout << "Invalid input.\n";
            }
        }
    }

    return 0;
}
