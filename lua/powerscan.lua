-- 10x10 Power scan 
require "laos";

xpos = 100;
ypos = 100;

-- focus scan
laos.speed(50);
laos.power(50);
for z = -10,10 do
--  x = xpos + 10 * z;
--  laos.move(nil, nil, z);
--  laos.box(x, ypos-20, 5,5);
end;

for x = 1,10 do
  for y = 1,10 do
    laos.speed(10*x);
	laos.power(10*y);
	laos.box(xpos + (10*x), ypos +  (10*y), 5, 5); 
  end;
end;

