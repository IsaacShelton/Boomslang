
#ifndef VARIABLE_H_INCLUDED
#define VARIABLE_H_INCLUDED

/*
    (c) 2016 Isaac Shelton

    This file is part of Boomslang.

    Boomslang is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Boomslang is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Boomslang. If not, see <http://www.gnu.org/licenses/>.
*/

using namespace std;

int compile_variable(string,string,string&,string&,string&,unsigned int,string& output);
int compile_nonexisting_variable(string,string,string,string&,string&,string&,string& output);

#endif // VARIABLE_H_INCLUDED
