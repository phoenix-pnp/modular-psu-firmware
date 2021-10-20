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

#include <stdint.h>
#include <math.h>
#include <scpi/scpi.h>
#include <new>

namespace eez {

void initAllocHeap(uint8_t *heap, size_t heapSize);

void *alloc(size_t size, uint32_t id);
void free(void *ptr);

template<class T> struct ObjectAllocator {
	static T *allocate(uint32_t id) {
		auto ptr = alloc(sizeof(T), id);
		return new (ptr) T;
	}
	static void deallocate(T* ptr) {
		ptr->~T();
		free(ptr);
	}
};

void dumpAlloc(scpi_t *context);

} // eez