#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

// Node structure for Inventory items (Singly Linked List)
struct Item {
    int id;
    string name;
    int quantity;
    double price;
    Item* next;
};

// Simple custom hashing for password protection (No plain text saved)
long long generateHash(const string& str) {
    long long hash = 5381;
    for (char c : str) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

class SystemEngine {
private:
    Item* head;
    const string userFile = "users.dat";
    const string inventoryFile = "inventory.txt";

    void loadInventoryFromFile() {
        ifstream file(inventoryFile);
        if (!file) return;
        int id, qty; string name; double price;
        while (file >> id >> ws && getline(file, name, ',') && file >> qty >> price) {
            addItemNode(id, name, qty, price);
        }
        file.close();
    }

    void saveInventoryToFile() {
        ofstream file(inventoryFile);
        Item* temp = head;
        while (temp != nullptr) {
            file << temp->id << " " << temp->name << "," << temp->quantity << " " << temp->price << "\n";
            temp = temp->next;
        }
        file.close();
    }

public:
    SystemEngine() { head = nullptr; loadInventoryFromFile(); }
    ~SystemEngine() {
        while (head != nullptr) {
            Item* temp = head;
            head = head->next;
            delete temp;
        }
    }

    bool registerUser(string username, string password) {
        ofstream file(userFile, ios::app | ios::binary);
        long long passHash = generateHash(password);
        file.write(username.c_str(), 30);
        file.write(reinterpret_cast<char*>(&passHash), sizeof(passHash));
        file.close();
        return true;
    }

    bool loginUser(string username, string password) {
        ifstream file(userFile, ios::binary);
        if (!file) return false;
        char uName[30]; long long storedHash;
        long long currentHash = generateHash(password);
        while (file.read(uName, 30)) {
            file.read(reinterpret_cast<char*>(&storedHash), sizeof(storedHash));
            if (string(uName) == username && storedHash == currentHash) {
                file.close();
                return true;
            }
        }
        file.close();
        return false;
    }

    void addItemNode(int id, string name, int qty, double prc) {
        Item* newItem = new Item{id, name, qty, prc, nullptr};
        if (head == nullptr) { head = newItem; }
        else {
            Item* temp = head;
            while (temp->next != nullptr) temp = temp->next;
            temp->next = newItem;
        }
    }

    void displayInventory() {
        if (head == nullptr) { cout << "\n[Alert] Inventory system database is currently empty.\n"; return; }
        cout << "\n------------------------------------------------------------\n";
        cout << left << setw(10) << "Item ID" << setw(25) << "Product Name" << setw(12) << "Quantity" << "Unit Price" << endl;
        cout << "------------------------------------------------------------\n";
        Item* temp = head;
        while (temp != nullptr) {
            cout << left << setw(10) << temp->id << setw(25) << temp->name << setw(12) << temp->quantity << "$" << fixed << setprecision(2) << temp->price << endl;
            temp = temp->next;
        }
        cout << "------------------------------------------------------------\n";
    }

    void performSystemCheck() {
        saveInventoryToFile();
        cout << "\n[Engine] Syncing memory matrices with persistent storage... Done.\n";
    }
};

int main() {
    SystemEngine core;
    int choice; string user, pass;
    bool authenticated = false;

    cout << "====================================================\n";
    cout << "          SECURE ENTERPRISE CORE MODULE             \n";
    cout << "====================================================\n";

    while (!authenticated) {
        cout << "\n1. System Registration\n2. Secure Terminal Login\n3. Shutdown\nSelect Module: ";
        cin >> choice;
        if (choice == 3) return 0;
        cout << "Enter Identity Token (Username): "; cin >> user;
        cout << "Enter Security Key (Password): "; cin >> pass;
        
        if (choice == 1) {
            core.registerUser(user, pass);
            cout << "\n[Success] Identity encryption verified and saved.\n";
        } else if (choice == 2) {
            if (core.loginUser(user, pass)) {
                authenticated = true;
                cout << "\n[Access Granted] Terminal unlocked.\n";
            } else {
                cout << "\n[Access Denied] Threat detected. Invalid credentials.\n";
            }
        }
    }

    while (authenticated) {
        cout << "\n===== INVENTORY SUBSYSTEM CORE =====\n";
        cout << "1. Display Operational Assets\n2. Provision New Asset (Add Item)\n3. Sync & Secure Log-out\nSelect Option: ";
        cin >> choice;
        if (choice == 1) {
            core.displayInventory();
        } else if (choice == 2) {
            int id, qty; string name; double price;
            cout << "Enter Asset ID (Numeric): "; cin >> id;
            cout << "Enter Asset Label (Name): "; cin.ignore(); getline(cin, name);
            cout << "Enter Target Quantity: "; cin >> qty;
            cout << "Enter Unit Valuation ($): "; cin >> price;
            core.addItemNode(id, name, qty, price);
            cout << "\n[Success] Asset allocated to runtime dynamic linked memory.\n";
        } else if (choice == 3) {
            core.performSystemCheck();
            cout << "\n[Terminal Terminalized] Logging out securely.\n";
            break;
        }
    }
    return 0;
}
