## Making a Zork-like in C++ without classes

| Command   | Parameters              | Description                            |
|-----------|-------------------------|----------------------------------------|
| Move      | \[Direction]            | Traverse the world in a direction.     |
| Inspect   | \[Object]               | View the description of an object.     |
| Take      | \[Object]               | Add an object to your backpack.        |
| Inventory |                         | Show all items in your backpack.       |
| Use       | \[Object]               | Use an object.                         |
| Consume   | \[Object]               | Consume an object.                     |
| Combine   | \[Object 1] \[Object 2] | Combine two different objects together |
| Kick      | \[Object]               | Kick an object.                        |
| Peel      | \[Object]               | Peel an object.                        |

| Meta Commands | Parameters | Description                                    |
|---------------|------------|------------------------------------------------|
| HELP          |            | Explain how the game works.                    |
| COMMANDS      |            | Lists all avalible commands.                   |
| LOG           |            | Lists all commands used througout the session. |
| HINT          |            | Displays a hint to progress.                   |
| UNDO          | \[int]     | Undo a number of actions. default 1;           |
| EXIT          |            | Closes the game.                               |
