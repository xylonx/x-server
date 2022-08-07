/*
 * This file is part of the xylonx distribution (https://github.com/xylonx).
 * Copyright (c) 2022 xylonx.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef GLOBALS_H
#define GLOBALS_H

namespace xserver {

extern int listen_buffer_size;

#ifndef WITHOUT_MULTIPLEXING
extern int multiplex_wait_nsec;
extern int multiplex_max_events;
#endif /* end WITHOUT_MULTIPLEXING */

extern int read_buffer_size;
extern int write_buffer_size;

}  // namespace xserver

#endif /* end GLOBALS_H */