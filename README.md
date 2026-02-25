## Making a Zork-like in C++ without classes

| Command   | Parameters          | Description                        |
|-----------|---------------------|------------------------------------|
| Enter     | \[Room]             | Move into an adjacent room.        |
| Inspect   | \[Object]           | Display an object's description.   |
| Take      | \[World Object]     | Add an object to your backpack.    |
| Drop      | \[Inventory Object] | Drop an object from your backpack. |
| Inventory |                     | List all items in your backpack.   |
| Use       | \[Object]           | Use an object.                     |
| Consume   | \[Object]           | Consume an object.                 |

| Meta Commands | Parameters | Description                                     |
|---------------|------------|-------------------------------------------------|
| HELP          |            | Explain how the game works.                     |
| COMMANDS      |            | Lists all available commands.                   |
| LOG           |            | Lists all commands used throughout the session. |
| EXIT          |            | Closes the game.                                |
