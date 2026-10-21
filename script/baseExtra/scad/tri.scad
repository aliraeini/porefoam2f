//-*- C -*-
//http://www.mathalino.com/reviewer/derivation-of-formulas/derivation-of-formula-for-radius-of-incircle
// nPnts = number of points (minimum 3)
// outer = radius to outer points
// inR = radius to inner circle
//           ..
//          /  \.
//         /  2   \.
//        /   |      \.
//       / 1'  .  3'     \.
//      /   ./   -\ .       \.
//     / 3 /   2'      \.   1   \
//    ---------------------------
//
module triAng(cornAng1, cornAng2, inR) {
	gama3=180-cornAng1-cornAng2;
	// polar to cartesian: radius/angle to x/y
	function x(r, a) = r * cos(a);
	function y(r, a) = r * sin(a);
	tpi1=-cornAng1/2;
	x1 = x(inR/sin(cornAng1/2), tpi1);
	y1 = y(inR/sin(cornAng1/2), tpi1);
	tpi2 = tpi1 + 180-cornAng1/2-cornAng2/2;
	x2 = x(inR/sin(cornAng2/2), tpi2);
	y2 = y(inR/sin(cornAng2/2), tpi2);
	tpi3 = tpi2 + 180-cornAng2/2-gama3/2;
	x3 = x(inR/sin(gama3/2), tpi3);
	y3 = y(inR/sin(gama3/2), tpi3);

	polygon(points = [[x1, y1], [x2, y2], [x3, y3]], paths = [[0, 1, 2]]);
}

module linxtrude(xx, hheight, sscale) {
	translate([xx,0,0]) rotate([0,90,0])
		linear_extrude(height = hheight, center=false, convexity=10,
		               twist=0, slices=1, scale = sscale)
			children();
}

cornAng1 = 40;
cornAng2 = 80;
contractRatio = 2;
contractRatioB = 2;
Rpore = 6;
Rtrot = Rpore/contractRatio;
RtrotB = Rpore/contractRatioB;

linxtrude( 0,  5.000000000001, 1.)           triAng(cornAng1, cornAng2, RtrotB);
linxtrude( 5, 10.000000000001, Rpore/RtrotB) triAng(cornAng1, cornAng2, RtrotB);
linxtrude(15,  5.000000000001, 1)            triAng(cornAng1, cornAng2, Rpore);
linxtrude(20, 10.000000000001, Rtrot/Rpore)  triAng(cornAng1, cornAng2, Rpore);
linxtrude(30,  5.000000000001, 1)            triAng(cornAng1, cornAng2, Rtrot);
linxtrude(35, 10.000000000001, Rpore/Rtrot)  triAng(cornAng1, cornAng2, Rtrot);
linxtrude(45,  5.000000000001, 1)            triAng(cornAng1, cornAng2, Rpore);
linxtrude(50, 10.000000000001, RtrotB/Rpore) triAng(cornAng1, cornAng2, Rpore);
linxtrude(60,  5.000000000000, 1)            triAng(cornAng1, cornAng2, RtrotB);
