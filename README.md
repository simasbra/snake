# Snake game

Multi thread snake terminal game using ncurses library.

### Threads

There are three threads:
* Snake thread - handles snake movement, eating and death;
* Windows thread - handles game and status window displaying;
* Input thread - handles user input and notifies other threads.

### Keymap

* `q` - exits the game;
* `arrow keys` - moves the snake to coresponding direction.

### Building and running the game

To build the game, in root repository directory create directory `objs` for object files,
then run `make` to build the game.
To run the game just enter `./snake` and start playing.

### License

This project is licensed under MIT License - see [LICENSE](LICENSE) for more details.
