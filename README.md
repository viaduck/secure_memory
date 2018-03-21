# Secure Memory Library
Provides common data structures used by multiple ViaDuck projects.

## Features
* `Buffer`:
  * Variable size heap binary memory buffer.
  * Convenience and safe methods to add, write, etc.
  * Automatical overwrite of buffer data after destruction.
  * Extensions: `String`.
* `Range`: Wrapper object for binary regions (pointer + size).
* `SecureUniquePtr`: Automatical overwrite of `std::unique_ptr` memory after
destruction.

## Licensing
This library is subject to the GNU Lesser General Public License v3.0 (GNU
LGPLv3).

```
Copyright (C) 2016-2018  The ViaDuck Project

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
```
