--
-- motiontest.lua
-- test functions for Laos laser motion, written in lua (see http://lua.org).
--
-- Send the output to a LAOS laser using netcat (assuming IP address and port):
--
--    lua motiontest.lua | nc 192.168.2.200 2000
--
-- Copyright (c) 2011 Peter Brier
-- 
--    This file is part of the LaOS project (see: http://laoslaser.org)
--    LaOS is free software: you can redistribute it and/or modify
--    it under the terms of the GNU General Public License as published by
--    the Free Software Foundation, either version 3 of the License, or
--    (at your option) any later version.
-- 
--    LaOS is distributed in the hope that it will be useful,
--    but WITHOUT ANY WARRANTY; without even the implied warranty of
--    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--    GNU General Public License for more details.
-- 
--    You should have received a copy of the GNU General Public License
--    along with LaOS.  If not, see <http://www.gnu.org/licenses/>.
--
--

require "laos" 

lines = 50;
xstart = 100; 
ystart = 100;
xend = 140;
ystep = 0.2;

data = {0xFFFFFFF0,0xFFFFFFFF, 0xFFFF0000,0xF0F0F0F0,0xF0F0F0F0,0xF0F0F0F0,0xF0F0F0F0,0xF0F0F0F0,0xF0F0F0F0,0x0000FFFF,0xFFFFFFFF,0x0FFFFFFF};

print("; Bitmap test pattern ");
for y = 0,lines-1 do 
  laos.power(2*y); 
  ypos = ystart + y*ystep;
  laos.move(xstart, ypos);
  laos.scanline(xend, ypos, data);  
end;
print("; lines: " .. lines );






