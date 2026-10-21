//-*- C -*-



separation=20.;
width=5.;
waveLength=1;
dblamplitude=1.;


for (iii =[0:20])
 translate([(iii)*waveLength-0.00001,0,0]) rotate([0,90,0]) linear_extrude(height = waveLength+0.00002, center=false, convexity=10, twist=0, slices=1, scale = 1.+(iii%2-0.5)*2.*dblamplitude/separation)
    square([width-(iii%2-0.5)*dblamplitude*width/separation,separation-(iii%2-0.5)*dblamplitude],true);
