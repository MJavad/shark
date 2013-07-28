D3DShark - General Information:
===============================

  D3DShark - DirectX Component Framework
  Copyright (C) 2012-2013 Michael Bleis
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.


Requirements:
-------------

  - Boost <http://www.boost.org/>
  - Microsoft Active Template Library
  - Microsoft Visual Studio 2012 <http://www.microsoft.com/visualstudio/>
  - Microsoft DirectX SDK <http://www.microsoft.com/en-us/download/details.aspx?id=6812>


Luabind:
--------

Luabind is a library that helps you create bindings between C++ and Lua. It
has the ability to expose functions and classes, written in C++, to Lua. It
will also supply the functionality to define classes in lua and let them
derive from other lua classes or C++ classes. Lua classes can override virtual
functions from their C++ baseclasses. It is written towards Lua 5.x, and does
not work with Lua 4.

It is implemented utilizing template meta programming. That means that you
don't need an extra preprocess pass to compile your project (it is done by the
compiler). It also means you don't (usually) have to know the exact signature
of each function you register, since the library will generate code depending
on the compile-time type of the function (which includes the signature). The
main drawback of this approach is that the compilation time will increase for
the file that does the registration, it is therefore recommended that you
register everything in the same cpp-file.

Luabind is released under the terms of the [MIT license][1].

> Copyright Daniel Wallin, Arvid Norberg 2003.
> Extracted from <http://www.rasterbar.com/products/luabind/docs.html>

[1]: http://www.opensource.org/licenses/mit-license.php
