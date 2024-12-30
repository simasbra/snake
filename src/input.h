/*
 * FILE: input.h
 * TITLE: Input handling
 * AUTHOR: Simas Bradaitis <simasbra@proton.me>
 * VERSION: 0.1.0
 * DESCRIPTION: Input handling header file
 *
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
 * \RETURNS: 1 if valid, 0 if not
 */
short i_handle_received_key(monitor *const monitor, const int value);

/*
 * Handles key up input by telling snake to move up
 */
void i_handle_key_up(monitor *const monitor);

/*
 * Handles key down input by telling snake to move down
 */
void i_handle_key_down(monitor *const monitor);

/*
 * Handles key right input by telling snake to move right
 */
void i_handle_key_right(monitor *const monitor);

/*
 * Handles key left input by telling snake to move left
 */
void i_handle_key_left(monitor *const monitor);
