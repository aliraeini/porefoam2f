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
	// angular width of each pie slice of the triAng
	//union() {
		//for (p = [0 : nPnts-1]) {
			// outer is outer point p
			// inner is inner point following p
			// next is next outer point following p

			tpi1=-cornAng1/2;
			x1 = x(inR/sin(cornAng1/2), tpi1);
			y1 = y(inR/sin(cornAng1/2), tpi1);
			tpi2 = tpi1 + 180-cornAng1/2-cornAng2/2;
			x2 = x(inR/sin(cornAng2/2), tpi2);
			y2 = y(inR/sin(cornAng2/2), tpi2);
			tpi3 = tpi2 + 180-cornAng2/2-gama3/2;
			x3 = x(inR/sin(gama3/2), tpi3);
			y3 = y(inR/sin(gama3/2), tpi3);

			translate([0,-y1,0])
			polygon(points = [[x1, y1], [x2, y2], [x3, y3]], paths = [[0, 1, 2]]);

		//}
	//}
}


module linxtrude(xx, hheight, sscale)
{ translate([xx,0,0]) rotate([0,90,0]) linear_extrude(height = hheight, center=false, convexity=10, twist=0, slices=1, scale = sscale)  children(); }

module poreThroatPore() {

cornAng1 = 40;
cornAng2 = 80;
contractRatio = 2;
Rpore = 12;
Rtrot = Rpore/contractRatio;
RtrotB = Rtrot+0.1;

linxtrude( 0,   10.000001,  1.)           triAng(cornAng1,cornAng2,RtrotB);
linxtrude( 0,   10.000001,  1.)           triAng(cornAng1,cornAng2,RtrotB);
linxtrude( 10,  15.000001,  Rpore/RtrotB) triAng(cornAng1,cornAng2,RtrotB);
linxtrude( 25,  15.000001,  1)            triAng(cornAng1,cornAng2,Rpore);
linxtrude( 40,  15.000001,  Rtrot/Rpore)  triAng(cornAng1,cornAng2,Rpore);
linxtrude( 55,  15.000001,  1)            triAng(cornAng1,cornAng2,Rtrot);
linxtrude( 70,  15.000001,  Rpore/Rtrot)  triAng(cornAng1,cornAng2,Rtrot);
linxtrude( 85,  15.000001,  1)            triAng(cornAng1,cornAng2,Rpore);
linxtrude( 100, 15.000001,  RtrotB/Rpore) triAng(cornAng1,cornAng2,Rpore);
linxtrude( 115, 10.000001,  1)            triAng(cornAng1,cornAng2,RtrotB);

}

module linytrude(yy, hheight, sscale)
{ translate([0,0,yy]) linear_extrude(height = hheight, center=false, convexity=10, twist=0, slices=1, scale = sscale)   children(); }


module throatz() {
cornAng1 = 40;
cornAng2 = 80;
contractRatio = 2;
Rpore = 12;
Rtrot = Rpore/contractRatio;
RtrotB = Rtrot+0.1;
linytrude(5 , 25.000001, Rtrot/Rpore) translate([-15.000001/2,0,0])             square([15.000001,Rpore*2],center=false);
linytrude(30, 20.000001, 1)           translate([-15.000001/2*Rtrot/Rpore,0,0]) square([15.000001*Rtrot/Rpore,Rtrot*2],center=false);
linytrude(50, 25.000001, Rpore/Rtrot) translate([-15.000001/2*Rtrot/Rpore,0,0]) square([15.000001*Rtrot/Rpore,Rtrot*2],center=false);
}


translate([20,0,0]) poreThroatPore();

translate([20,0,90]) mirror([0,0,1]) poreThroatPore();

translate([52.5,0,0]) throatz();
