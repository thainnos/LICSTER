// Spare Part 3d Model of the Fischertechnik Goods

// r[adius], h[eight], [rou]n[d]
module rounded_cylinder(r,h,n) {
  rotate_extrude(convexity=1) {
    offset(r=n) offset(delta=-n) square([r,h]);
    square([n,h]);
  }
}

rounded_cylinder(r=15,h=20,n=2,$fn=100);