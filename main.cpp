#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

// Define the Room types
enum class RoomType { EMPTY, MONSTER, TREASURE };

// Define the Monster structure
struct Monster {
    int healthPoints;
    int attackPoints;
};

// Define the Room structure
struct Room {
    RoomType type;
    vector
        <Room*> connectedRooms;
    Monster* monster;
};

// Define the Monk class
class Monk {
private:
    string name;
    string description;
    int healthPoints;
    int attackPoints;

public:
    Monk(string n, string desc) : name(n), description(desc), healthPoints(15), attackPoints(3) {}

    void pray() {
        // Function to restore health points
        healthPoints = 15;
        cout << "You feel rejuvenated after praying." << endl;
    }

    void attack(Monster& enemy) {
        // Function to attack the enemy
        enemy.healthPoints -= attackPoints;
        cout << "You attacked the monster!" << endl;
    }

    void defend() {
        // Function to defend and recover health points
        healthPoints += 1;
        cout << "You defended and recovered 1 health point." << endl;
    }

    bool isAlive() {
        // Function to check if the Monk is alive
        return healthPoints > 0;
    }

    void displayStatus() {
        // Function to display Monk's status
        cout << "Monk Name: " << name << endl;
        cout << "Description: " << description << endl;
        cout << "Health Points: " << healthPoints << endl;
        cout << "Attack Points: " << attackPoints << endl;
    }

    int getAttackPoints() const {
        // Getter function for attackPoints
        return attackPoints;
    }
};

// Function to generate a random integer within a range
int getRandom(int min, int max) {
    return rand() % (max - min + 1) + min;
}

// Function to generate a dungeon
Room* generateDungeon() {
    Room* treasureRoom = new Room{RoomType::TREASURE, {}, nullptr};
    Room* emptyRoom1 = new Room{RoomType::EMPTY, {}, nullptr};
    Room* emptyRoom2 = new Room{RoomType::EMPTY, {}, nullptr};
    Room* monsterRoom = new Room{RoomType::MONSTER, {}, new Monster{10, 2}};

    // Connect rooms randomly
    emptyRoom1->connectedRooms.push_back(emptyRoom2);
    emptyRoom1->connectedRooms.push_back(monsterRoom);
    emptyRoom2->connectedRooms.push_back(emptyRoom1);
    emptyRoom2->connectedRooms.push_back(monsterRoom);
    monsterRoom->connectedRooms.push_back(emptyRoom1);
    monsterRoom->connectedRooms.push_back(emptyRoom2);
    monsterRoom->connectedRooms.push_back(treasureRoom);

    return emptyRoom1; // Start from an empty room
}

// Function to handle exploration
void exploreDungeon(Room* startRoom, Monk& player) {
    Room* currentRoom = startRoom;
    int playerAction; // Declare playerAction outside the loop
    Monster* monster = nullptr; // Declare monster outside the switch

    while (currentRoom != nullptr) {
        cout << "You are in a ";
        switch (currentRoom->type) {
            case RoomType::EMPTY:
                cout << "empty room." << endl;
                player.pray(); // Restore health in empty rooms
                break;
            case RoomType::MONSTER:
                cout << "room with a monster!" << endl;
                monster = currentRoom->monster;
                while (monster->healthPoints > 0 && player.isAlive()) {
                    // Combat loop
                    // Get player action
                    cout << "Choose your action (0 for attack, 1 for defend): ";
                    cin >> playerAction;

                    if (playerAction == 0)
                        player.attack(*monster); // Call attack without passing attackPoints
                    else
                        player.defend();

                    // Display status after each turn
                    player.displayStatus();
                    cout << "Monster Health Points: " << monster->healthPoints << endl;
                }
                if (player.isAlive())
                    cout << "You defeated the monster!" << endl;
                else {
                    cout << "You were defeated by the monster!" << endl;
                    return; // End exploration
                }
                break;
            case RoomType::TREASURE:
                cout << "treasure room! Congratulations, you win!" << endl;
                return; // End exploration
        }

        // Move to a random connected room
        if (!currentRoom->connectedRooms.empty()) {
            cout << "Choose a room to move to: ";
            for (int i = 0; i < currentRoom->connectedRooms.size(); ++i) {
                cout << i + 1 << ". ";
                switch (currentRoom->connectedRooms[i]->type) {
                    case RoomType::EMPTY:
                        cout << "Empty Room";
                        break;
                    case RoomType::MONSTER:
                        cout << "Monster Room";
                        break;
                    case RoomType::TREASURE:
                        cout << "Treasure Room";
                        break;
                }
                cout << endl;
            }

            int choice;
            cin >> choice;

            if (choice >= 1 && choice <= currentRoom->connectedRooms.size())
                currentRoom = currentRoom->connectedRooms[choice - 1];
            else
                cout << "Invalid choice. Please select again." << endl;
        } else {
            cout << "Oops! This room seems to be isolated. You're trapped!" << endl;
            return; // End exploration
        }
    }
}

int main() {
    srand(time(0)); // Seed for random number generation

    // Generate dungeon
    Room* startRoom = generateDungeon();

    // Create the Monk
    Monk player("Player", "A brave monk ready for adventure!");

    // Start exploration
    exploreDungeon(startRoom, player);

    return 0;
}