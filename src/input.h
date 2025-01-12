/*
 * Copyright (c) 2024 Simas Bradaitis
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "monitor.h"

/*
 * Handles user input
 */
void i_handle_input(monitor *const monitor);

/*
 * Handles user pressed key
 * \RETURNS: 1 if exit received, 0 if not
 */
short i_handle_received_key(monitor *const monitor, const int value);

/*
 * Handles game exit
 */
void i_handle_exit(monitor *const monitor);

/*
 * Handles move key input by telling snake to move to given direction.
 * Also checks if the move is not in the opposite direction
 */
void i_handle_snake_move(monitor *const monitor, const enum m_snake_move next_move,
			 const enum m_snake_move opposite_direction);
