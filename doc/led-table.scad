module bar (s)
{
    difference () {
        thickness = s.z;
        tolerance = 0.15;

        lengthTolerance = 0.15;
        translate ([0, -lengthTolerance/2, 0])
        cube ([s.x, s.y-lengthTolerance, s.z], center=true);

        // Offset is needed to make parts non-symetric.
        translate ([0, -s.z/2, 0]) {
            // Cut out in the middle
            translate ([s.x/2, 0, 0])
            rotate([90, 0, 0])
            cube ([s.x, 100, thickness+tolerance], center=true);
            
            // Rest of the cutouts
            for (i = [0: 17]) {
                translate ([s.x/2, i * 16.6, 0])
                rotate([90, 0, 0])
                cube ([s.x, 100, thickness+tolerance], center=true);

                translate ([s.x/2, -i * 16.6, 0])
                rotate([90, 0, 0])
                cube ([s.x, 100, thickness+tolerance], center=true);
            }
        }
    }
}


bar ([20, 16.6*8, 2]);