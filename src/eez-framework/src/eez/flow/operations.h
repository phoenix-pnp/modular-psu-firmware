/*
* EEZ Generic Firmware
* Copyright (C) 2021-present, Envox d.o.o.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.

* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.

* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <eez/flow/expression.h>

namespace eez {
namespace flow {

typedef void (*EvalOperation)(EvalStack &);

extern EvalOperation g_evalOperations[];

Value op_add(const Value& a1, const Value& b1);
Value op_sub(const Value& a1, const Value& b1);
Value op_mul(const Value& a1, const Value& b1);
Value op_div(const Value& a1, const Value& b1);
Value op_mod(const Value& a1, const Value& b1);

Value op_left_shift(const Value& a1, const Value& b1);
Value op_right_shift(const Value& a1, const Value& b1);
Value op_binary_and(const Value& a1, const Value& b1);
Value op_binary_or(const Value& a1, const Value& b1);
Value op_binary_xor(const Value& a1, const Value& b1);

Value op_eq(const Value& a1, const Value& b1);
Value op_neq(const Value& a1, const Value& b1);
Value op_less(const Value& a1, const Value& b1);
Value op_great(const Value& a1, const Value& b1);
Value op_less_eq(const Value& a1, const Value& b1);
Value op_great_eq(const Value& a1, const Value& b1);

} // flow
} // eez
