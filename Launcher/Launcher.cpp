#include <iostream>
#include <cstdlib>  // for system() command
#include <string>

void displayMenu() {
    std::cout << "Select which executable to run:\n";
    std::cout << "1. A1B1.exe\n";
    std::cout << "2. A1B2.exe\n";
    std::cout << "3. A2B2.exe\n";
    std::cout << "4. A2B1.exe\n";
    std::cout << "Enter your choice (1-4): ";
}

int main() {
    int choice = 0;

    // Display the menu
    displayMenu();

    // Get user's choice
    std::cin >> choice;

    // Execute corresponding program based on user choice
    switch (choice) {
    case 1:
        std::cout << "Running A1B1.exe...\n";
        system("start A1B1.exe");
        break;
    case 2:
        std::cout << "Running A1B2.exe...\n";
        system("start A1B2.exe");
        break;
    case 3:
        std::cout << "Running A2B2.exe...\n";
        system("start A2B2.exe");
        break;
    case 4:
        std::cout << "Running A2B1.exe...\n";
        system("start A2B1.exe");
        break;
    default:
        std::cout << "Invalid choice! Please enter a number between 1 and 4.\n";
        return 1;
    }

    return 0;
}
