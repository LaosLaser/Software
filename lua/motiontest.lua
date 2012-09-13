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

require "image"      -- bitmap file reader

xscale = 1000;
yscale = 1000;


-- set speed 0..100%
function speed(s)
  print("7 100 " .. s * 100 .. " " );
end;

-- set power 0..100%
function power(p)
  print("7 101 " .. p * 100 .. " ");
end;

-- move [mm]
function move(x,y)
  print("0 " .. math.floor(xscale*x) .. " " .. math.floor(yscale*y) .. " ");
end;

-- laser [mm]
function laser(x,y)
  print("1 " .. math.floor(xscale*x) .. " " .. math.floor(yscale*y) .. " ");
end;

-- scanline, load bitmap data words and laser to end-coordinate
function scanline(x,y,data)
  width = math.ceil(#data * 32);
  str = "9 1 " .. width;
  for k,v in ipairs(data) do
    str = str .. " " .. v;
  end;
  print(str);
  laser(x,y);
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


-- linetest
-- move 100 mm (s) in small segments of 0.25mm (step) (400 x 0.25mm) and back in 1 segment (100mm)
-- repeat 10 times (n). Back and forward motion should be smooth and of equal speed
function linetest(s,step,n)
  s = s or 100;
  step = step or 0.25;
  n = n or 10;
  for j=1,n do
    move(0,0);
    for i = 1,(s/step),s/math.abs(s) do
      line(0, math.floor(1000 * i * step) );
    end;   
    move(0,0); -- one back
  end;
end;


--
-- MAIN
--
-- power(100);

x = 60;
y = 60;

-- box(x,y,100,100);
--box(x,y,100,100,true);
--circle(x+50,y+50,50);
--circle(x+50,y+50,50,true);

--for i=1,10 do
--  speed(10+10*i);
--  circle(x+50,y+50,5*i);
--  circle(x+50,y+50,5*i,true);
--end

function datatest()
data = {};
for i = 1,100 do
  if (i % 2) == 0 then
    data[i] = 0xF0F0FF00;
    bit = bit * 2;
    if bit > 0xFFFFFFFF then
      bit = 1;
    end; 
  else
    data[i] = 0;
  end;
end;
end;

-- Pack databits of a line in words
function databits(img, y, reverse)
  local len = 1;
  local word = 0;
  local pixels = 0;
  local data = {}; 
  local xs, xe, xd;
  if not reverse then
    xs = 0;
    xe = img.w-1;
    xd = 1;
  else
    xs = img.w-1;
    xe = 0;
    xd = -1;
  end;

  bit = 1;
  for x = xs,xe,xd do
    if img:pixel(x,y) > 0 then
      word = word + bit;
      pixels = pixels + 1;
    end;
    bit = bit * 2;
    if ( bit >= 0x80000000 ) then
      bit = 1;
      data[len] = word;
      word = 0;
      len = len+1;
    end;
  end;
  return data,pixels;
end;



-- render a bitmap
function bitmap(xs,ys,filename)
  img=image.load(filename)
  print("; w=" .. img.w .. " h=" .. img.h );
  -- image.test(img);
  xres = 0.1;
  yres = 0.5;
  lines = 0;
  reverse = false;
  xstart = xs;
  xend = xs + img.w*xres;
  for y = 0,img.h-1 do  
    data,pixels = databits(img, y, reverse); 
    ypos = y*yres;
    if pixels > 0 then
      if reverse then
        move(xend, ypos);      
        scanline(xstart, ypos, data);
      else
        move(xstart, ypos);    
        scanline(xend, ypos, data);
      end;
      lines = lines + 1;
     reverse = not reverse;
    end;
  end;
  print("; lines: " .. lines );
end;


bitmap(100,100,'hello-bmp.bmp');



