#include <algorithm>
#include <iostream>
#include <vector>
#include <sstream>

struct object {
    std::string name;
    std::string summary;
    std::string description;
};

struct room {
    std::string name;
    std::string description;
    std::vector<int> neighboursIndex;

    std::pmr::vector<object> objects;
};

struct game_state {
    std::pmr::vector<object> inventory;

    std::vector<room> rooms;
    room currentRoom;
};

struct command {
    std::string name;
    std::string syntax;
    std::string description;

    void (*commandFunction)(game_state&, std::vector<std::string>);
};

std::string StringToLowerCase(std::string str) {
    std::ranges::transform(str, str.begin(),[](unsigned char c){ return std::tolower(c); });

    return str;
}

std::vector<std::string> StringToWords(const std::string &input) {
    std::stringstream ss(input);
    std::string word;
    std::vector<std::string> words;

    while (ss >> word) {
        words.push_back(word);
    }

    return words;
}

void EnterRoom(game_state &state, std::vector<std::string> parameters) {
    for (const room& room: state.rooms) {
        if (parameters[0] == room.name) {
            for (int i = 0; i < state.currentRoom.neighboursIndex.size(); i++) {
                if (room.name == state.rooms[i].name) {
                    state.currentRoom = room;

                    std::cout << "You entered the " << state.currentRoom.name << "." << std::endl;
                    std::cout << state.currentRoom.description << std::endl;

                    for (const object& obj: state.currentRoom.objects) {
                        std::cout << "\n[" << obj.name << "] " << obj.summary << std::endl;
                    }

                    return;
                }
            }
        }
    }
    std::cout << "You haphazardly walk into a few walls trying to access the " << parameters[0] << " without success." << std::endl;
}

void Inspect(game_state &state, std::vector<std::string> parameters) {
    if (parameters.empty()) {
        std::cout << "You take a moment to analyze nothingness itself... You feel like you've grown as a person, even though you haven't." << std::endl;
        return;
    }

    if (parameters.size() > 1) {
        std::cout << "Your singular eye can only focus on one object at a time." << std::endl;
        return;
    }

    for (const object& object: state.inventory) {
        if (StringToLowerCase(object.name) == parameters[0]) {
            std::cout << "You inspect the " << object.name << "..." << std::endl;
            std::cout << object.description << std::endl;

            return;
        }
    }

    for (const object& object: state.currentRoom.objects) {
        if (StringToLowerCase(object.name) == parameters[0]) {
            std::cout << "You inspect the " << object.name << "..." << std::endl;
            std::cout << object.description << std::endl;

            return;
        }
    }

    std::cout << "Squinting your eye you inspect the " << parameters[0] << " you've conjured up with your imagination. Sadly you interupt your deep concentration with a loud sneeze, the imagined object lost forever.";
}

command GetCommandFromString(const std::string &s) {
    const std::vector<command> commands = {
        command {"enter","enter [room]", "Move into an adjacent room.", EnterRoom},
        command {"inspect","inspect [object]", "Display an object's description.", Inspect},
        command {"take","take [object]", "Add an object to your backpack."},
        command {"exit","", "Exit the game."},
    };

    const std::string firstWord = s.substr(0, s.find_first_of(' '));

    for (auto command: commands) {
        if (command.name != firstWord) continue;

        return command;
    }

    return {} ;
}

int main() {
    std::vector<room> rooms = {
        room {"kitchen", "The kitchen in dusty and icky.",  {1, 3}},
        room {"lounge", "There are a lot of living things in the living room.",  {0, 2, 3, 4, 5}},
        room {"bedroom", "The bed is missing, making this bedroom nothing more than a room.",  {0, 1, 6}},
        room {"bathroom", "The bathroom has a bathtub but no shower. This home was clearly owned by a psychotic killer.",  {1}},
        room {"garage", "The garage's walls are covered in deep scratchmarks. The owner's must have a big dog... There are even marks in the ceiling.",  {1}, {{"TeXla", "It looks brand new. You'll have to hurry to escape since it could spontaneously explode at any moment.", "There is a sticker on the back of the truck saying \"I bought this before it got musky\"."}}},
        room {"office", "The office is cramped and smells of mold. But hey, at least they have an office.",  {2}},
    };

    game_state state = game_state {{}, rooms, rooms[2]};

    std::cout << "______          _            ______                        \n"
                 "| ___ \\        | |           | ___ \\                      \n"
                 "| |_/ /   _ ___| |_ _   _    | |_/ /___   ___  _ __ ___    \n"
                 "|    / | | / __| __| | | |   |    // _ \\ / _ \\| '_ ` _ \\ \n"
                 "| |\\ \\ |_| \\__ \\ |_| |_| |   | |\\ \\ (_) | (_) | | | | | |\n"
                 "\\_| \\_\\__,_|___/\\__|\\__, |   \\_| \\_\\___/ \\___/|_| |_| |_|\n"
                 "                     __/ |                               \n"
                 "                    |___/                                \n" << std::endl;

    std::cout << "You wake up in the " << state.currentRoom.name << "." << std::endl;
    std::cout << state.currentRoom.description << std::endl;

    while (true) {
        std::string input;

        std::cout << "\nWhat do you do?" << std::endl;
        std::getline(std::cin, input);

        input = StringToLowerCase(input);
        std::vector<std::string> words = StringToWords(input);

        command command = GetCommandFromString(input);

        if (command.name.empty()) {
            std::cout << "Command \"" << input.substr(0, input.find_first_of(' ')) << "\" not found." << std::endl;
            continue;
        }

        words.erase(words.begin());
        command.commandFunction(state, words);
    }
}