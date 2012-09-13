-- 10x10 Power scan 
require "laos";

xpos = 100;
ypos = 100;

for x = 1,10 do
  for y = 1,10 do
    laos.speed(10*x);
	laos.power(10*y);
	laos.box(xpos + (10*x), ypos +  (10*y), 5, 5); 
  end;
end;

