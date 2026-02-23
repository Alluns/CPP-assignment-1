#include <iostream>
#include <vector>

struct Room {
    std::string name;
    std::string description;
    Room * neighbours[4] = {};
};

struct Object {
    std::string name;
    std::string summary;
    std::string description;
};

struct Inventory {
    std::pmr::vector<Object> Objects;
};

struct Command {
    std::string name;
    std::string syntax;
    std::string description;
};

int main() {
    std::vector<Command> commands = {
        Command {"move","move [direction]", "Traverse the world in a direction."},
        Command {"inspect","inspect [object]", "Display an object's description."},
        Command {"take","take [object]", "Add an object to your backpack."},
        Command {"exit","", "Exit the game."},
    };

    std::cout << "______          _            ______                        \n"
                 "| ___ \\        | |           | ___ \\                      \n"
                 "| |_/ /   _ ___| |_ _   _    | |_/ /___   ___  _ __ ___    \n"
                 "|    / | | / __| __| | | |   |    // _ \\ / _ \\| '_ ` _ \\ \n"
                 "| |\\ \\ |_| \\__ \\ |_| |_| |   | |\\ \\ (_) | (_) | | | | | |\n"
                 "\\_| \\_\\__,_|___/\\__|\\__, |   \\_| \\_\\___/ \\___/|_| |_| |_|\n"
                 "                     __/ |                               \n"
                 "                    |___/                                \n" << std::endl;


    while (true) {
        std::string input;

        std::cout << "What do you do?" << std::endl;
        std::getline(std::cin, input);

        std::string firstWord = input.substr(0, input.find_first_of(' '));

        Command command;

        for (auto cmd: commands) {
            if (cmd.name != firstWord) continue;

            command = cmd;
            break;
        }

        std::cout << "Command name: " << command.name << std::endl;
        std::cout << "Command syntax: " << command.syntax << std::endl;
        std::cout << "Command description: " << command.description << std::endl;
    }

    return 0;
}