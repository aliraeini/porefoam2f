//-*- C -*-
// points = number of points (minimum 3)
// outer = radius to outer points
// inner = radius to inner points
module Star(points, outer, inner) {
	// polar to cartesian: radius/angle to x/y
	function x(r, a) = r * cos(a);
	function y(r, a) = r * sin(a);
	// angular width of each pie slice of the star
	increment = 360/points;
	union() {
		for (p = [0 : points-1]) {
			// outer is outer point p
			// inner is inner point following p
			// next is next outer point following p
			x_outer = x(outer, increment * p);
			y_outer = y(outer, increment * p);
			x_inner = x(inner, (increment * p) + (increment/2));
			y_inner = y(inner, (increment * p) + (increment/2));
			x_next = x(outer, increment * (p+1));
			y_next = y(outer, increment * (p+1));

			polygon(points = [[x_outer, y_outer],
			                  [x_inner, y_inner],
			                  [x_next, y_next],
			                  [0, 0]],
			         paths = [[0, 1, 2, 3]]);
		}
	}
}

module linxtrude(xx, hheight, sscale) {
	translate([xx,0,0]) rotate([0,90,0])
		linear_extrude(height = hheight, center=false, convexity=10,
		               twist=0, slices=1, scale = sscale)
			children();
}

cornAng = 60;
nCorn = 4;
contractRatio = 2;
contractRatioB = 2;
Rpore = 6;
Rtrot = Rpore/contractRatio;
RtrotB = Rpore/contractRatioB;

ROFactor=(1.+1./tan(cornAng/2.))/sqrt(2.);

linxtrude( 0,  5.000000000001, 1.)           Star(nCorn, RtrotB, ROFactor*RtrotB);
linxtrude( 5, 10.000000000001, Rpore/RtrotB) Star(nCorn, RtrotB, ROFactor*RtrotB);
linxtrude(15,  5.000000000001, 1)            Star(nCorn, Rpore,  ROFactor*Rpore);
linxtrude(20, 10.000000000001, Rtrot/Rpore)  Star(nCorn, Rpore,  ROFactor*Rpore);
linxtrude(30,  5.000000000001, 1)            Star(nCorn, Rtrot,  ROFactor*Rtrot);
linxtrude(35, 10.000000000001, Rpore/Rtrot)  Star(nCorn, Rtrot,  ROFactor*Rtrot);
linxtrude(45,  5.000000000001, 1)            Star(nCorn, Rpore,  ROFactor*Rpore);
linxtrude(50, 10.000000000001, RtrotB/Rpore) Star(nCorn, Rpore,  ROFactor*Rpore);
linxtrude(60,  5.000000000000, 1)            Star(nCorn, RtrotB, ROFactor*RtrotB);
