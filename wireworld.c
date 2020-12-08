#include <curses.h>
#include <stdlib.h>

typedef enum {Empty = 1, ElectronHead = 2, ElectronTail = 3, Conductor = 4} Cell;

Cell transition_cell_state(Cell** grid, int y, int x, int max_y, int max_x) {
	switch (grid[y][x]) {
		case Empty:
			return Empty;
		case ElectronHead:
			return ElectronTail;
		case ElectronTail:
			return Conductor;
		case Conductor: {
			typedef struct {int y, x;} Coord;

			Coord possible_neighbors[8] = {
				{y - 1, x}, {y - 1, x - 1},
				{y - 1, x + 1}, {y, x - 1},
				{y, x + 1}, {y + 1, x - 1},
				{y + 1, x}, {y + 1, x + 1}};

			int head_neighbors = 0;
			for (int i = 0; i < 8; i++) {
				Coord c = possible_neighbors[i];
				if (!(c.y < 0 || c.y == max_y || c.x < 0 || c.x > max_x)) {
					if (grid[c.y][c.x] == ElectronHead) head_neighbors++;
				}
			}
			return head_neighbors == 1 || head_neighbors == 2 ? ElectronHead : Conductor;
		}
	}
}

int main() {
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, 1);
	nodelay(stdscr, 1);
	int max_y, max_x;
	getmaxyx(stdscr, max_y, max_x);
	start_color();
	use_default_colors();
	mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);

	init_pair(1, COLOR_BLACK, COLOR_BLACK);
	init_pair(2, COLOR_BLUE, COLOR_BLUE);
	init_pair(3, COLOR_RED, COLOR_RED);
	init_pair(4, COLOR_YELLOW, COLOR_YELLOW);

	Cell** grid = malloc(max_y * sizeof(Cell*));
	for (int y = 0; y < max_y; y++) {
		Cell* row = malloc(max_x * sizeof(Cell));
		for (int x = 0; x < max_x; x++) row[x] = Empty;
		grid[y] = row;
	}

	int running = 1, paused = 1, cursor_y = 1, cursor_x = 1;
	Cell selected_cell = ElectronHead;
	while (running) {
		int input = getch();
		switch (input) {
			case 'q': running = 0; break;
			case 'w': paused = !paused; break;
			case '1': case '2': case '3': case '4':
				selected_cell = input - 48;
		}

		if (paused) {
			switch (input) {
				case KEY_MOUSE: {
					MEVENT event;
					if (getmouse(&event) == OK) {
						cursor_y = event.y; cursor_x = event.x;
						grid[cursor_y][cursor_x] = selected_cell;
					}
				}; break;

				case KEY_LEFT:
					if (cursor_x >= 0) grid[cursor_y][--cursor_x] = selected_cell; break;
				case KEY_RIGHT:
					if (cursor_x < max_x) grid[cursor_y][++cursor_x] = selected_cell; break;
				case KEY_UP:
					if (cursor_y > 0) grid[--cursor_y][cursor_x] = selected_cell; break;
				case KEY_DOWN:
					if (cursor_y < max_y - 1) grid[++cursor_y][cursor_x] = selected_cell;
			}
		}
		else {
			for (int y = 0; y < max_y; y++) {
				for (int x = 0; x < max_x; x++)
					grid[y][x] = transition_cell_state(grid, y, x, max_y, max_x);
			}
		}
		for (int y = 0; y < max_y; y++) {
			for (int x = 0; x < max_x; x++) {
				Cell c = grid[y][x];
				attron(COLOR_PAIR(c));
				mvprintw(y, x, " ");
				attroff(COLOR_PAIR(c));
			}
		}
		napms(75 * (!paused + 1)); // longer wait if not editing
	}
	for (int y = 0; y < max_y; y++) free(grid[y]);
	free(grid);
	endwin();
}
