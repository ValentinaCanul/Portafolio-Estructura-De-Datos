#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <limits>
#include "hash_table.h"

using namespace std;

// Function prototypes
void load_from_file(HashTable *ht, const char *filename);
void save_to_file(HashTable *ht, const char *filename);
void add_identifier(HashTable *ht);
void search_identifier(HashTable *ht);
void delete_identifier(HashTable *ht);
void display_menu();

// Main function
int main() {
    HashTable *ht = create_hash_table(TABLE_SIZE);
    const char *filename = "identifiers.txt";

    // Load identifiers from file
    load_from_file(ht, filename);

    int choice;
    do {
        display_menu();
        // Read menu choice safely. If input is invalid (non-numeric), clear the
        // error state and discard the rest of the line, then re-prompt.
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number between 1 and 5." << endl;
            choice = -1; // keep loop going
            continue;
        }
        // consume rest of line
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                add_identifier(ht);
                break;
            case 2:
                search_identifier(ht);
                break;
            case 3:
                delete_identifier(ht);
                break;
            case 4:
                display_hash_table(ht);
                break;
            case 5:
                cout << "Exiting program..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }

        // Save identifiers to file after each operation
        save_to_file(ht, filename);

    } while (choice != 5);

    destroy_hash_table(ht);
    return 0;
}

// Display menu
void display_menu() {
    cout << "\n--- Identifier Management ---\n";
    cout << "1. Add Identifier\n";
    cout << "2. Search Identifier\n";
    cout << "3. Delete Identifier\n";
    cout << "4. Display All Identifiers\n";
    cout << "5. Exit\n";
    cout << "Enter your choice: ";
}

// Load identifiers from file
void load_from_file(HashTable *ht, const char *filename) {
    ifstream file(filename);
    if (!file.is_open()) return;

    // helper to trim trailing semicolons and whitespace, and leading whitespace
    auto trim = [](std::string &s) {
        // trim trailing spaces, semicolons and hash characters
        while (!s.empty() && (isspace((unsigned char)s.back()) || s.back() == ';' || s.back() == '#')) s.pop_back();
        // trim leading spaces and hash characters
        size_t start = 0;
        while (start < s.size() && (isspace((unsigned char)s[start]) || s[start] == '#')) start++;
        if (start > 0) s.erase(0, start);
    };

    // We'll parse all lines first and store them, then insert in reverse order
    // so the final linked-list order matches the file order (insertion at head
    // would otherwise reverse entries per-bucket and cause the flip-flop you saw).
    struct Entry { string scope, type, key, value; int collisions; };
    vector<Entry> entries;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        string scope, type, key, value = "NULL";

        istringstream iss(line);
        if (!(iss >> scope)) continue;
        if (!(iss >> type)) continue;
        if (!(iss >> key)) continue;

        // trim semicolons/spaces from key
        trim(key);

        // read the rest (could be "= value;" and optionally "collisions=<n>")
        string rest;
        getline(iss, rest);
        trim(rest);
        int collisions = -1;
        if (!rest.empty()) {
            // look for collisions marker
            size_t colPos = rest.find("collisions=");
            if (colPos == string::npos) colPos = rest.find("#collisions=");
            string valuePart = rest;
            if (colPos != string::npos) {
                // extract collisions number
                size_t numStart = colPos + string("collisions=").length();
                if (rest[colPos] == '#') numStart = colPos + string("#collisions=").length();
                size_t numEnd = numStart;
                while (numEnd < rest.size() && isdigit((unsigned char)rest[numEnd])) numEnd++;
                string numStr = rest.substr(numStart, numEnd - numStart);
                if (!numStr.empty()) collisions = stoi(numStr);
                // remove collisions part from valuePart
                valuePart = rest.substr(0, colPos);
            }

            size_t eq = valuePart.find('=');
            if (eq != string::npos) {
                string val = valuePart.substr(eq + 1);
                trim(val);
                if (!val.empty()) value = val;
            }
        }

    entries.push_back({scope, type, key, value, collisions});
    }

    // Insert entries in file order. Since insert() now appends at the tail,
    // inserting in forward order preserves the file order in each bucket.
    for (size_t i = 0; i < entries.size(); ++i) {
        IdentifierAttributes attrs;
        attrs.type = strdup(entries[i].type.c_str());
        attrs.value = strdup(entries[i].value.c_str());
        attrs.scope = strdup(entries[i].scope.c_str());
        attrs.collisions = entries[i].collisions;
        insert(ht, entries[i].key.c_str(), attrs);
    }

    file.close();
}

// Save identifiers to file
void save_to_file(HashTable *ht, const char *filename) {
    ofstream file(filename);
    if (!file.is_open()) return;

    for (int i = 0; i < ht->size; i++) {
        Node *current = ht->table[i];
        while (current) {
            file << current->attrs.scope << " " << current->attrs.type << " " << current->key;
            if (strcmp(current->attrs.value, "NULL") != 0) {
                file << " = " << current->attrs.value;
            }
            // save collisions info so it's persisted (e.g. "#collisions=1")
            file << " #collisions=" << current->attrs.collisions << ";\n";
            current = current->next;
        }
    }

    file.close();
}

// Add an identifier
void add_identifier(HashTable *ht) {
    string key, scope, scopeInput, type, value;

    cout << "Enter Identifier Name: ";
    cin >> key;

    // Validate scope input: accept only Global/Local or g/G/l/L and normalize
    while (true) {
        cout << "Enter Scope (Global/Local) [g/G/l/L accepted]: ";
        cin >> scopeInput;
        if (scopeInput == "g" || scopeInput == "G" || scopeInput == "Global" || scopeInput == "global") {
            scope = "Global";
            break;
        } else if (scopeInput == "l" || scopeInput == "L" || scopeInput == "Local" || scopeInput == "local") {
            scope = "Local";
            break;
        } else {
            cout << "Invalid scope. Please enter 'Global' or 'Local' (or g/l)." << endl;
        }
    }

    IdentifierAttributes temp;
    if (search(ht, key.c_str(), scope.c_str(), &temp)) {
        cout << "Error: Identifier already exists in this scope.\n";
        return;
    }

    cout << "Enter Type: ";
    cin >> type;
    cout << "Enter Value (leave empty for NULL): ";
    cin.ignore();
    getline(cin, value);
    if (value.empty()) value = "NULL";

    IdentifierAttributes attrs;
    attrs.type = strdup(type.c_str());
    attrs.value = strdup(value.c_str());
    attrs.scope = strdup(scope.c_str());
    attrs.collisions = -1; // let insert() compute collisions for runtime additions
    if (insert(ht, key.c_str(), attrs)) {
        cout << "Identifier added successfully.\n";
    } else {
        cout << "Error: Failed to add identifier.\n";
    }
}

// Search for an identifier
void search_identifier(HashTable *ht) {
    string key;
    cout << "Enter Identifier Name: ";
    cin >> key;

    // Find all matching nodes in the bucket for this key
    int index = hash_function(key.c_str(), ht->size);
    Node *current = ht->table[index];
    vector<Node*> matches;
    while (current) {
        if (strcmp(current->key, key.c_str()) == 0) {
            matches.push_back(current);
        }
        current = current->next;
    }

    if (matches.empty()) {
        cout << "Error: Identifier not found.\n";
        return;
    }

    int choiceIndex = 0;
    if (matches.size() == 1) {
        choiceIndex = 0;
    } else {
        cout << "Multiple identifiers named '" << key << "' found:\n";
        for (size_t i = 0; i < matches.size(); ++i) {
            IdentifierAttributes &a = matches[i]->attrs;
            cout << "  [" << (i+1) << "] Scope: " << a.scope << ", Type: " << a.type << ", Value: " << a.value << "\n";
        }
        int sel = 0;
        while (true) {
            cout << "Enter number to view (or 0 to cancel): ";
            if (!(cin >> sel)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input.\n";
                continue;
            }
            if (sel == 0) {
                cout << "Canceling search.\n";
                return;
            }
            if (sel >= 1 && sel <= (int)matches.size()) {
                choiceIndex = sel - 1;
                break;
            }
            cout << "Selection out of range.\n";
        }
    }

    IdentifierAttributes result = matches[choiceIndex]->attrs;
    cout << "ID: " << key << "\nType: " << result.type << "\nValue: " << result.value
        << "\nScope: " << result.scope << "\nCollisions: " << result.collisions << endl;
}

// Delete an identifier
void delete_identifier(HashTable *ht) {
    string key;
    cout << "Enter Identifier Name: ";
    cin >> key;

    // Find all matching nodes in the bucket for this key
    int index = hash_function(key.c_str(), ht->size);
    Node *current = ht->table[index];
    vector<Node*> matches;
    while (current) {
        if (strcmp(current->key, key.c_str()) == 0) {
            matches.push_back(current);
        }
        current = current->next;
    }

    if (matches.empty()) {
        cout << "Error: Identifier not found.\n";
        return;
    }

    // If more than one match, let the user choose which one to delete
    int choiceIndex = 0; // index into matches
    if (matches.size() == 1) {
        choiceIndex = 0;
        IdentifierAttributes res = matches[0]->attrs;
        cout << "Found: ID: " << key << "\nType: " << res.type << "\nValue: " << res.value
             << "\nScope: " << res.scope << endl;
    } else {
        cout << "Multiple identifiers named '" << key << "' found:\n";
        for (size_t i = 0; i < matches.size(); ++i) {
            IdentifierAttributes &a = matches[i]->attrs;
            cout << "  [" << (i+1) << "] Scope: " << a.scope << ", Type: " << a.type << ", Value: " << a.value << "\n";
        }
        int sel = 0;
        while (true) {
            cout << "Enter number to delete (or 0 to cancel): ";
            if (!(cin >> sel)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input.\n";
                continue;
            }
            if (sel == 0) {
                cout << "Canceling delete.\n";
                return;
            }
            if (sel >= 1 && sel <= (int)matches.size()) {
                choiceIndex = sel - 1;
                break;
            }
            cout << "Selection out of range.\n";
        }
    }

    IdentifierAttributes chosen = matches[choiceIndex]->attrs;
    cout << "You chose to delete: ID: " << key << " Scope: " << chosen.scope << " Type: " << chosen.type << " Value: " << chosen.value << "\n";
    cout << "Are you sure you want to delete this identifier? (y/n): ";
    char confirm;
    cin >> confirm;
    if (confirm == 'y' || confirm == 'Y') {
        if (erase(ht, key.c_str(), chosen.scope)) {
            cout << "Identifier deleted successfully.\n";
        } else {
            cout << "Error: Failed to delete identifier.\n";
        }
    } else {
        cout << "Deletion canceled.\n";
    }
}

/*

Instructions to Compile and Run:
Move to the folder:

cd "C:\<Path where the Folder is>\ADA07"


Compile:

g++ -std=c++11 -Wall -Wextra -g ADA07_E02.cpp hash_table.cpp -o ADA07_E02.exe

g++ -std=c++11 -g ADA07_E02.cpp hash_table.cpp -o ADA07_E02.exe

Run:

.\ADA07_E02.exe


*/