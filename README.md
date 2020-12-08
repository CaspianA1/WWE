## WWE - The Wireworld Editor

Wireworld is a cellular automata used to simulate transistors and basic logic. It is Turing complete!
Here's a complex circuit made in Wireworld:

![Circuit](https://community.wolfram.com/c/portal/getImageAttachment?filename=AW567hgh1nop.gif&userId=11733)

### Editor controls:
- `q` = exit
- `w` = toggle edit mode
- `1` through `4` - select a cell for edit mode:
	- `1` = empty (black)
	- `2` = electron head (blue)
	- `3` = electron tail (red)
	- `4` = conductor (yellow)
- Mouse click = move cursor to a given position
- Arrow keys = draw circuits with a selected cell (`1` through `4`)

### For a given iterations, each cell undergoes the following transformation:
- `1` -> `1`
- `2` -> `3`
- `3` -> `4`
- `4` -> `2` if the neighbor count is between 1 and 2 (inclusive) else `4`

#### Have fun with the editor!