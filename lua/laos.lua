--
-- laos.lua
-- Module with functions for making laos laser files
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

module(..., package.seeall)  


xscale = 1000;
yscale = 1000;
zscale = 1000;

-- set speed 0..100%
function speed(s)
  print("7 100 " .. s * 100 .. " " );
end;

-- set power 0..100%
function power(p)
  print("7 101 " .. p * 100 .. " ");
end;

-- move [mm]
function move(x,y,z)
  if z ~= nil then
    print("2 " .. math.floor(zscale*z) .. " ");
  end
  if x ~= nil and y ~= nil then
    print("0 " .. math.floor(xscale*x) .. " " .. math.floor(yscale*y) .. " ");
  end;
end;

-- laser [mm]
function laser(x,y)
  print("1 " .. math.floor(xscale*x) .. " " .. math.floor(yscale*y) .. " ");
end;


-- make a box (if dir == true, clockwise)
function box(x,y,w,h,dir)
  move(x,y);
  if dir then
    laser(x, y+h);
    laser(x+w, y+h);  
    laser(x+w, y);
    laser(x,y);
  else
    laser(x+w, y);
    laser(x+w, y+h);
    laser(x, y+h);
    laser(x,y);
  end;
end;

-- make a cicle [mm] with center (cx,xy) with radius (r) 
-- segment length [seg] and direction (if dir is true, clockwise, otherwise CCW)
function circle(cx,cy,r,dir,seg)
  s = seg or 0.5;
  if dir then d=1; else d=-1; end;
  move(cx,cy+r);
  n = 5 + r / s;
  for i=0,n do
    a = (2*3.14159265/n) * i * d; 
    laser(cx+math.sin(a)*r, cy+math.cos(a)*r);
  end;
end;

-- scanline, load bitmap data words and raster engrave to end-coordinate at 1 bit-per-pixel
-- Pass an (x,y) end-coordinate, and a table with (unsigned) 32bit numbers with the data for the scanline
-- Each data item has 32 bits where the LSB is clocked out FIRST and the MSB LAST
-- A HIGH bit is LASER ON and a LOW bit is LASER OFF at that location
function scanline(x,y,data)
  width = math.ceil(#data * 32);
  str = "9 1 " .. width;
  for k,v in ipairs(data) do
    str = str .. " " .. v;
  end;
  print(str);
  laser(x,y);
end;


