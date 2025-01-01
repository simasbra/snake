# Snake game

Multi thread snake terminal game using ncurses library.

### Threads

There three threads:
* Snake thread - handles snake movement, eating and death;
* Windows thread - handles game and status window displaying;
* Input thread - handles user input and notifies other threads.

### Keymap

`q` - exits the game;
`arrow keys` - moves the snake to coresponding direction.

### Building and running the game

To build the game, in root repository directory run `make` to build the game.
To run the game just enter `./snake.out` and start playing.

### License

This project is licensed under MIT License - see [LICENSE](LICENSE) for more details.
