
module spheres(R, cmntFctr) {

    for (kk =[0:1])
        for (jj =[0:1])
            for (ii =[0:1])
            {
                color( [ii, jj, kk] )
                translate([2*R*ii,2*R*jj,2*R*kk]) sphere(R+cmntFctr, $fa=5, $fs=1);
            }
}

RSphere = 20;
cementPCent = 20;
ScaleY = 1;
ScaleZ = 1;

R=100;
scalX = RSphere/R;
scalY = ScaleY*RSphere/R;
scalZ = ScaleZ*RSphere/R;
scale([scalX,scalY,scalZ]) difference()
{

    cube(size = [2*R, 2*R, 2*R], center = false);
    spheres(R, cementPCent);
}
