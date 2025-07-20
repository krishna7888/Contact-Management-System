#include <iostream>
#include <string>
#include <fstream>
using namespace std;

//===============================================================>>structure of contact list
struct Contact {
    string name;
    string phone;
    string email;
    string address;
};


//=================================================================>>a single new node having contact details and left,right null ptr 
struct Node {
    Contact data;
    Node* left;
    Node* right;

    Node(Contact c) {
        data = c;
        left = right = nullptr;
    }
};

// ==================================================================>>Insert into BST
Node* insert(Node* root, Contact c) {
    if (!root) return new Node(c);

    if (c.name < root->data.name)
        root->left = insert(root->left, c);
    else
        root->right = insert(root->right, c);

    return root;
}

// ========================================================>>Inorder traversal for display
void displayContacts(Node* root) {
    if (root) {
        displayContacts(root->left);
        cout << "Name: " << root->data.name
             << "\nPhone: " << root->data.phone
             << "\nEmail: " << root->data.email
             << "\nAddress: " << root->data.address << "\n\n";
        displayContacts(root->right);
    }
}
// ===========================================================>>Search contact
Node* search(Node* root, string name) {
    if (!root || root->data.name == name) return root;

    if (name < root->data.name)
        return search(root->left, name);
    return search(root->right, name);
}

// =================================================================>>Find minimum node
Node* findMin(Node* root) {
    while (root && root->left) root = root->left;
    return root;
}

// ===============================================================>>Delete contact
Node* deleteContact(Node* root, string name) {
    if (!root) return nullptr;

    if (name < root->data.name)
        root->left = deleteContact(root->left, name);
    else if (name > root->data.name)
        root->right = deleteContact(root->right, name);
    else {
        if (!root->left) {
            Node* temp = root->right;
            delete root;
            return temp;
        } else if (!root->right) {
            Node* temp = root->left;
            delete root;
            return temp;
        }

        Node* temp = findMin(root->right);
        root->data = temp->data;
        root->right = deleteContact(root->right, temp->data.name);
    }

    return root;
}

// ===============================================================>>Update contact
void updateContact(Node* root, string name) {
    Node* node = search(root, name);
    if (!node) {
        cout << "Contact not found!\n";
        return;
    }

    cout << "Enter new phone: ";
    cin >> node->data.phone;
    cout << "Enter new email: ";
    cin >> node->data.email;
    cout << "Enter new address: ";
    cin.ignore();
    getline(cin, node->data.address);
    cout << "Contact updated successfully!\n";
}

//=================================================>>> Adding new contact
Contact inputContact() {
    Contact c;
    cout << "Enter name: ";
    cin.ignore();
    getline(cin, c.name);
    cout << "Enter phone: ";
    cin >> c.phone;
    cout << "Enter email: ";
    cin >> c.email;
    cout << "Enter address: ";
    cin.ignore();
    getline(cin, c.address);
    return c;
}




// =================================================>>for saving the contact in file system;
void saveToFile(Node* root, ofstream& file) {
    if (root) {
        saveToFile(root->left, file);
        file << root->data.name << "\n"
             << root->data.phone << "\n"
             << root->data.email << "\n"
             << root->data.address << "\n";
        saveToFile(root->right, file);
    }
}

void saveContacts(Node* root) {
    ofstream file("contacts.txt");
    if (!file) {
        cout << "Error saving contacts!\n";
        return;
    }
    saveToFile(root, file);
    file.close();
}



// ========================================================>>for load file from startup
Node* loadContacts() {
    ifstream file("contacts.txt");
    Node* root = nullptr;
    if (!file) return nullptr;

    Contact c;
    while (getline(file, c.name)) {
        getline(file, c.phone);
        getline(file, c.email);
        getline(file, c.address);
        root = insert(root, c);
    }

    file.close();
    return root;
}


//===================================================>> main function which give option to perform operations 
int main() {
    Node* root = loadContacts(); // ===========================>>Load on startup
    int choice;
    string name;

    do {
        cout << "\n--- Contact Management System ---\n";
        cout << "1. Add Contact\n";
        cout << "2. Display Contacts\n";
        cout << "3. Search Contact\n";
        cout << "4. Update Contact\n";
        cout << "5. Delete Contact\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                root = insert(root, inputContact());
                saveContacts(root); //========================> Save after adding
                cout << "Contact added successfully!\n";
                break;

            case 2:
                if (root)
                    displayContacts(root);
                else
                    cout << "No contacts to display.\n";
                break;

            case 3:
                cout << "Enter name to search: ";
                cin.ignore();
                getline(cin, name);
                if (Node* found = search(root, name)) {
                    cout << "\nContact Found:\n";
                    cout << "Name: " << found->data.name
                         << "\nPhone: " << found->data.phone
                         << "\nEmail: " << found->data.email
                         << "\nAddress: " << found->data.address << "\n";
                } else {
                    cout << "Contact not found.\n";
                }
                break;

            case 4:
                cout << "Enter name to update: ";
                cin.ignore();
                getline(cin, name);
                updateContact(root, name);
                saveContacts(root); //============================>> Save after update
                break;

            case 5:
                cout << "Enter name to delete: ";
                cin.ignore();
                getline(cin, name);
                root = deleteContact(root, name);
                saveContacts(root); // ====================================>>Save after delete
                cout << "Contact deleted (if it existed).\n";
                break;

            case 0:
                saveContacts(root); // ======================================>>Save before exiting
                cout << "Exiting... Thank you!\n";
                break;

            default:
                cout << "Invalid choice. Try again.\n";
        }

    } while (choice != 0);

    return 0;
}