#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

class Movie {
    string title;
    int rows, cols;
    vector<vector<int>> seats;  // 0 = available, 1 = booked

public:
    Movie(string t, int r, int c) : title(t), rows(r), cols(c) {
        seats = vector<vector<int>>(rows, vector<int>(cols, 0));
    }

    string getTitle() { return title; }

    void displaySeats() {
        cout << "\n🎥 Movie: " << title << endl;
        cout << "Seating Layout (O=Available, X=Booked):\n";
        for (int i = 0; i < rows; i++) {
            cout << "Row " << i << " : ";
            for (int j = 0; j < cols; j++) {
                cout << (seats[i][j] ? "X " : "O ");
            }
            cout << endl;
        }
    }

    int getPrice(int row) {
        if (row <= 1) return 150;   // front
        else if (row <= 3) return 200; // middle
        else return 250;            // back
    }

    void bookSeats() {
        int n;
        cout << "How many seats do you want to book? ";
        cin >> n;

        string name;
        cout << "Enter your name: ";
        cin >> name;

        int totalCost = 0;
        vector<pair<int, int>> bookedSeats;

        for (int k = 0; k < n; k++) {
            int row, col;
            cout << "Enter row (0-" << rows - 1 << "): ";
            cin >> row;
            cout << "Enter col (0-" << cols - 1 << "): ";
            cin >> col;

            if (row < 0 || row >= rows || col < 0 || col >= cols) {
                cout << "❌ Invalid seat position!\n";
                k--; continue;
            }
            if (seats[row][col] == 1) {
                cout << "❌ Seat already booked!\n";
                k--; continue;
            }

            seats[row][col] = 1;
            int price = getPrice(row);
            totalCost += price;
            bookedSeats.push_back({row, col});
            cout << "Seat Row " << row << " Col " << col << " reserved (Rs." << price << ")\n";
        }

        cout << "Total Price = Rs." << totalCost << endl;
        cout << "Press 1 to Pay, 0 to Cancel: ";
        int pay; cin >> pay;

        if (pay == 1) {
            cout << "✅ Payment successful. Booking confirmed!\n";
            ofstream file("bookings.txt", ios::app);
            for (auto &seat : bookedSeats) {
                file << name << " | " << title 
                     << " | Row:" << seat.first << " Col:" << seat.second 
                     << " | Paid Rs." << getPrice(seat.first) << "\n";
            }
            file.close();
        } else {
            cout << "❌ Payment cancelled. Seats not booked.\n";
            for (auto &seat : bookedSeats) {
                seats[seat.first][seat.second] = 0; // reset
            }
        }
    }

    void cancelSeat() {
        int row, col;
        cout << "Enter row of booking to cancel: ";
        cin >> row;
        cout << "Enter col of booking to cancel: ";
        cin >> col;

        if (row < 0 || row >= rows || col < 0 || col >= cols) {
            cout << "❌ Invalid seat position!\n";
            return;
        }
        if (seats[row][col] == 0) {
            cout << "❌ Seat is not booked yet!\n";
            return;
        }

        seats[row][col] = 0;
        cout << "✅ Booking cancelled successfully.\n";
    }
};

class BookingSystem {
    vector<Movie> movies;

public:
    BookingSystem() {
        // Add sample movies
        movies.push_back(Movie("Avengers: Endgame", 5, 10));
        movies.push_back(Movie("Inception", 5, 10));
        movies.push_back(Movie("Interstellar", 5, 10));
    }

    void userMenu() {
        while (true) {
            cout << "\n====== Movie Ticket Booking System ======\n";
            cout << "1. View Movies\n";
            cout << "2. Book Seats\n";
            cout << "3. Cancel Booking\n";
            cout << "4. Admin Panel\n";
            cout << "5. Exit\n";
            cout << "Enter your choice: ";
            int choice; cin >> choice;

            if (choice == 1) showMovies();
            else if (choice == 2) bookSeats();
            else if (choice == 3) cancelBooking();
            else if (choice == 4) adminPanel();
            else if (choice == 5) { cout << "Goodbye!\n"; break; }
            else cout << "❌ Invalid choice. Try again.\n";
        }
    }

    void showMovies() {
        cout << "\nAvailable Movies:\n";
        for (int i = 0; i < movies.size(); i++) {
            cout << i << ". " << movies[i].getTitle() << endl;
        }
        int choice;
        cout << "Enter movie index to view seats: ";
        cin >> choice;
        if (choice >= 0 && choice < movies.size()) {
            movies[choice].displaySeats();
        } else {
            cout << "❌ Invalid movie choice!\n";
        }
    }

    void bookSeats() {
        cout << "Select movie index: ";
        int idx; cin >> idx;
        if (idx >= 0 && idx < movies.size()) {
            movies[idx].bookSeats();
        } else {
            cout << "❌ Invalid movie!\n";
        }
    }

    void cancelBooking() {
        cout << "Select movie index: ";
        int idx; cin >> idx;
        if (idx >= 0 && idx < movies.size()) {
            movies[idx].cancelSeat();
        } else {
            cout << "❌ Invalid movie!\n";
        }
    }

    void adminPanel() {
        string pass;
        cout << "Enter admin password: ";
        cin >> pass;
        if (pass != "admin123") {
            cout << "❌ Wrong password!\n";
            return;
        }

        cout << "\n====== Admin Panel ======\n";
        ifstream file("bookings.txt");
        if (!file) {
            cout << "No bookings yet.\n";
            return;
        }
        string line;
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    }
};

int main() {
    BookingSystem system;
    system.userMenu();
    return 0;
}
