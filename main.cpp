#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <format>

#pragma region Structs

struct command;

struct object {
    std::string name;
    std::string summary;
    std::string description;
};

struct room {
    std::string name;
    std::string description;
    std::vector<int> neighboursIndices;

    std::pmr::vector<object> objects;
};

struct game_state {
    std::pmr::vector<object> inventory;

    std::vector<room> rooms;
    room currentRoom;

    std::vector<command> commands;

    std::vector<command> history;
};

struct command {
    std::string name;
    std::string syntax;
    std::string description;

    void (*commandFunction)(game_state&, std::vector<std::string>);
};

#pragma endregion

#pragma region Functions

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

command GetCommandFromString(const std::string &s, const game_state &state) {
    for (auto command: state.commands) {
        if (s.starts_with(command.name)) {
            return command;
        }
    }

    return {} ;
}

void EnterRoom(game_state &state, std::vector<std::string> parameters) {
    if (parameters.empty()) {
        std::cout << "You walk around in circles. Philosophising about how you can walk for so long without getting anywhere..." << std::endl;
        return;
    }

    if (parameters.size() > 1) {
        std::cout << "Determined to walk into two rooms at the same time you confidently colide with the wall separating them." << std::endl;
        return;
    }

    for (const room& room : state.rooms) {
        if (parameters[0] == room.name) {
            for (int index : state.currentRoom.neighboursIndices) {
                if (room.name == state.rooms[index].name){
                    std::cout << "-----------------------------------------------------------" << std::endl;
                    std::cout << std::format("You entered the {}\n{}.", room.name, room.description) << std::endl;

                    for (const object& obj: room.objects) {
                        std::cout << "\n[" << obj.name << "] " << obj.summary << std::endl;
                    }

                    state.currentRoom = room;
                    return;
                }
            }
        }
    }

    std::cout << std::format("You haphazardly walk into a few walls trying to access the {} without success.", parameters[0]) << std::endl;
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
            std::cout << std::format("You inspect the {}...", object.name) << std::endl;
            std::cout << object.description << std::endl;

            return;
        }
    }

    for (const object& object: state.currentRoom.objects) {
        if (StringToLowerCase(object.name) == parameters[0]) {
            std::cout << std::format("You inspect the {} ...", object.name) << std::endl;
            std::cout << object.description << std::endl;

            return;
        }
    }

    std::cout << std::format("Squinting your eye you inspect the {} you've conjured up with your imagination. Sadly you interrupt your deep concentration with a loud sneeze, the imagined object lost forever.", parameters[0]) << std::endl;
}

void Take(game_state &state, std::vector<std::string> parameters) {
    if (parameters.empty()) {
        std::cout << "You desperately grasp at the air around you for seemingly no reason." << std::endl;
        return;
    }

    if (parameters.size() > 1) {
        std::cout << "The twin digits on your singular foot can't grasp more than one object at a time." << std::endl;
        return;
    }

    for (int i = 0; i < state.rooms.size(); i++) {
        if (state.rooms[i].name != state.currentRoom.name) continue;

        for (int j = 0; j < state.currentRoom.objects.size(); ++j) {
            object object = state.rooms[i].objects[j];

            if (StringToLowerCase(object.name) == parameters[0]) {
                std::cout << std::format("You add the {} to one of the {} internal compartments of your backpack.", object.name, state.inventory.max_size()) << std::endl;

                state.inventory.push_back(object);

                state.rooms[i].objects.erase(state.rooms[i].objects.begin() + j);

                return;
            }
        }
    }

    std::cout << std::format("Added a {} to your inventory. HAHA, just kidding. There is no {} in the room.", parameters[0], parameters[0]) << std::endl;
}

void Drop(game_state &state, std::vector<std::string> parameters) {
    if (parameters.empty()) {
        std::cout << "Whilst swinging your arm wildly you dramatically open your hand, a clump of nothingness darting across the room and doing nothing..." << std::endl;
        return;
    }

    if (parameters.size() > 1) {
        std::cout << "Sensing your independent thought of dropping more than one object at a time your backpack growls at you threateningly. Maybe you should think less controversial thoughts..." << std::endl;
        return;
    }

    for (int i = 0; i < state.inventory.size(); i++) {
        if (StringToLowerCase(state.inventory[i].name) == parameters[0]) {
            std::cout << std::format("You mindlessly throw the {} onto the ground with all your might.",parameters[0]) << std::endl;

            for (auto &room: state.rooms) {
                if (room.name == state.currentRoom.name) {
                    room.objects.push_back(state.inventory[i]);
                    state.inventory.erase(state.inventory.begin() + i);
                    return;
                }
            }
        }
    }

    std::cout << std::format("You can't use {}, {} isn't even real. Get real!" ,parameters[0], parameters[0]) << std::endl;
}

void Use(game_state &state, std::vector<std::string> parameters) {
    if (parameters.empty()) {
        std::cout << "\"Using use is useless if use is used on nothing\" - Sun Tzu" << std::endl;
        return;
    }

    if (parameters.size() > 1) {
        std::cout << "The concept of using two objects at the same time is far to complex for you." << std::endl;
        return;
    }

    object obj = {};

    for (const object& object: state.inventory) {
        if (StringToLowerCase(object.name) == parameters[0]) {
            obj = object;
            break;
        }
    }

    if (obj.name.empty()) {
        for (const object& object: state.currentRoom.objects) {
            if (StringToLowerCase(object.name) == parameters[0]) {
                obj = object;
                break;
            }
        }
    }

    if (!obj.name.empty()) {
        // unique code for each item that can be used



        return;
    }

    std::cout << std::format("You can't use {}, {} isn't even real. Get real!" ,parameters[0], parameters[0]) << std::endl;
}

void Consume(game_state &state, std::vector<std::string> parameters) {
    if (parameters.empty()) {
        std::cout << "You start trying to eating yourself... \"Owie!\" ...You stop trying to eat yourself." << std::endl;
        return;
    }

    if (parameters.size() > 1) {
        std::cout << "You jaw is too sore from constantly screaming for you to eat more than one object at a time." << std::endl;
        return;
    }

    object obj = {};

    for (const object& object: state.inventory) {
        if (StringToLowerCase(object.name) == parameters[0]) {
            obj = object;
            break;
        }
    }

    if (obj.name.empty()) {
        for (const object& object: state.currentRoom.objects) {
            if (StringToLowerCase(object.name) == parameters[0]) {
                obj = object;
                break;
            }
        }
    }

    if (!obj.name.empty()) {
        // unique code for each item that can be consumed


        return;
    }

    std::cout << std::format("As you bite into {} you realise that {} was just something you imagined and that your teeth are painfully digging into your tongue!" ,parameters[0], parameters[0]) << std::endl;
}

void Commands(game_state &state, std::vector<std::string> parameters) {
    for (command command: state.commands) {
        std::cout << std::format("{}\n{}\n\n", command.syntax, command.description);
    }
}

void Log(game_state &state, std::vector<std::string> parameters) {
    for (int i = 0; i < state.history.size(); ++i) {
        std::cout << std::format("{}. {}\n", i, state.history[i].name);
    }
}

void Inventory(game_state &state, std::vector<std::string> parameters) {
    if (state.inventory.empty()) {
        std::cout << "Rummaging through your backpack you find that it is empty. Almost as empty as your own heart :(" << std::endl;
        return;
    }

    std::cout << "Rummaging through your backpack you find: " << std::endl;

    for (int i = 0; i < state.inventory.size(); ++i) {
        std::cout << std::format("{}.[{}] {}\n",i, state.inventory[i].name, state.inventory[i].summary);
    }
}

void Help(game_state &state, std::vector<std::string> parameters) {
    std::cout << "Added \"helpful information\" to the player." << std::endl;

    state.inventory.push_back({"helpful_information", "what???", "This information doesn't actually seem all that helpful. It looks scrumptious though..."});
}

int GetZeroNeighbour(int array[], int tileIndex) {
    if (tileIndex < 0 || tileIndex > 15){
        return -1;
    }

    if (tileIndex - 1 >= 0 && array[tileIndex - 1] == 0) return tileIndex - 1;
    if (tileIndex + 1 <= 15 && array[tileIndex + 1] == 0) return tileIndex + 1;
    if (tileIndex - 5 >= 0 && array[tileIndex - 5] == 0) return tileIndex - 5;
    if (tileIndex + 5 <= 15 && array[tileIndex + 5] == 0) return tileIndex + 5;

    return -2;
}

void SolvePuzzle() {
    int square[16] = {4, 7, 0, 1, 10, 11, 12, 14, 3, 5, 8, 6, 2, 15, 9, 13};

    std::string input;

    while (true) {
        for (int i = 0; i < 16; ++i) {
            if (i % 4 == 0) {
                std::cout << std::endl;
            }
            std::cout << square[i] << " ";
        }

        std::cout << "\nChoose which tile to move by typing it's coordinate example: 3,0" << std::endl;
        std::cin >> input;

        int x = (input[0] - 48);
        int y = (input[2] - 48) * 4;

        int tileIndex = x + y;

        int result = GetZeroNeighbour(square, tileIndex);

        if (result == -1) {
            std::cout << input << " is not a valid coordinate." << std::endl;
            continue;
        }

        if (result == -2) {
            std::cout << input << " has no valid neighbours." << std::endl;
            continue;
        }

        int tempInt = square[tileIndex];
        square[tileIndex] = square[result];
        square[result] = tempInt;
    }
}



#pragma endregion

int main() {
    SolvePuzzle();

    std::vector<room> rooms = {
        room {"kitchen", "The kitchen in dusty and icky.",  {1, 2}},
        room {"living_room", "There are a lot of living things in the living room.",  {0, 2, 3, 4}},
        room {"bedroom", "The bed is missing, making this bedroom nothing more than a room.",  {0, 1, 5}},
        room {"bathroom", "The bathroom has a bathtub but no shower. This home was clearly owned by a psychotic killer.",  {1}},
        room {"garage", "The garage's walls are covered in deep scratchmarks. The owner's must have a big dog... There are even marks in the ceiling.",  {1}, {{"TeXla", "It looks brand new. You'll have to hurry to escape since it could spontaneously explode at any moment.", "There is a sticker on the back of the truck saying \"I bought this before it got musky\"."}}},
        room {"office", "The office is cramped and smells of mold. But hey, at least they have an office.",  {2}, {{"Puzzle", "It looks like a classic sliding puzzle", "You can't quite make out what the puzzle's image is supposed to be. Maybe if you solved it..."}}},
    };

    const std::vector<command> commands = {
        command {"enter","enter [room]", "Move into an adjacent room.", EnterRoom},
        command {"inspect","inspect [object]", "Display an object's description.", Inspect},
        command {"take","take [world object]", "Add an object to your untrustworthy backpack.", Take},
        command {"drop","drop [inventory object]", "Drop an object on the murky floor.", Drop},
        command {"use","use [object]", "Use an object.", Use},
        command {"consume","consume [object]", "Consume an object.", Consume},
        command {"inventory","inventory", "Show all objects stored in your trusty backpack.", Inventory},
        command {"commands","commands", "Lists all available commands.", Commands},
        command {"help","help", "Gives helpful information to the player.", Help},
        command {"log","log", "Lists all commands used throughout the session.", Log},
        command {"exit","exit", "Exit the game."},
    };

    game_state state = game_state {{}, rooms, rooms[2], commands, {}};

    std::cout << "______          _            ______                        \n"
                 "| ___ \\        | |           | ___ \\                      \n"
                 "| |_/ /   _ ___| |_ _   _    | |_/ /___   ___  _ __ ___    \n"
                 "|    / | | / __| __| | | |   |    // _ \\ / _ \\| '_ ` _ \\ \n"
                 "| |\\ \\ |_| \\__ \\ |_| |_| |   | |\\ \\ (_) | (_) | | | | | |\n"
                 "\\_| \\_\\__,_|___/\\__|\\__, |   \\_| \\_\\___/ \\___/|_| |_| |_|\n"
                 "                     __/ |                               \n"
                 "                    |___/                                \n" << std::endl;

    std::cout << "-----------------------------------------------------------" << std::endl;
    std::cout << "You wake up in the " << state.currentRoom.name << "." << std::endl;
    std::cout << state.currentRoom.description << std::endl;

    while (true) {
        std::string input;

        std::cout << "\nWhat do you do?\n> ";
        std::getline(std::cin, input);

        input = StringToLowerCase(input);
        std::vector<std::string> words = StringToWords(input);

        command command = GetCommandFromString(input, state);

        if (command.name.empty()) {
            std::cout << std::format("Command \"{}\" not found.", words[0]) << std::endl;
            continue;
        }

        if (command.name == "exit") {
            return 0;
        }

        state.history.push_back(command);

        words.erase(words.begin());
        command.commandFunction(state, words);
    }
}