#include <fstream>
#include <thread>
#include <SealService.h>
#include <HttpRequestService.h>
#include <json.hpp>

using json = nlohmann::json;
using namespace std;

// Function to get valid double input from user
double getValidDouble() {
    double value;
    while (true) {
        cin >> value;
        if (cin.fail()) {
            cin.clear(); // clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard invalid input
            cout << "Invalid input. Please enter a valid double value: ";
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard any extra input
            return value;
        }
    }
}

void showMenu() {
    cout << "\nInteraktives Menü:\n";
    cout << "1. Create Context and give Double Values\n";
    cout << "2. Sende Seal Data zum Cluster...\n";
    cout << "3. Gebe Unique ID zum entschlüsseln ein...\n";
    cout << "4. Beenden\n";
    cout << "Bitte wählen Sie eine Option: ";
}

int main() {
    SealService sealService;
    HttpRequestService httpRequestService;
    int choice = 0;
    string unique_id;

    while (true) {
        showMenu();
        cin >> choice;

        switch (choice) {
            case 1: {
                vector<double> numbers(4);
                vector<string> names = {"Insurance Level (1-4) - 75€", "vehicle age in years - 50€", "annual mileage in km - 0.01€", "number of accidents - 100€"};

                cout << "Please enter the following values:\n";

                for (int i = 0; i < 4; ++i) {
                    cout << "Enter " << names[i] << ": ";
                    numbers[i] = getValidDouble();
                }

                cout << "You entered:\n";
                for (int i = 0; i < 4; ++i) {
                    cout << names[i] << ": " << numbers[i] << endl;
                }

                cout << "Kontext und Keys werden erstellt...\n";
                sealService.createKeysAndContext(numbers);
                break;
            }
            case 2: {
                cout << "Sende Seal Date zum Cluster...\n";
                cout << "Bitte geben Sie die Unique-ID ein: ";
                cin >> unique_id;
                httpRequestService.sendSealData(unique_id);
                break;
            }
            case 3: {
                cout << "Bitte geben Sie die Unique-ID ein: ";
                cin >> unique_id;
                map<string, string> encryptedData = httpRequestService.sendRequest(unique_id);
                if (encryptedData.find("error") != encryptedData.end()) {
                    cout << "Error: " << encryptedData["error"] << endl;
                    break;
                }
                cout << "Decrypted: " << sealService.decrypt(encryptedData);
                break;
            }
            case 4:
                cout << "Programm wird beendet...\n";
                return 0;
            default:
                cout << "Ungültige Auswahl! Bitte versuchen Sie es erneut.\n";
        }
    }
}