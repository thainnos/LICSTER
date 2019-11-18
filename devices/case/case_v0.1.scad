union() difference() { 
    cube(size = [200,190,5], center = false);
    
    // Raspberry Pi
    translate([175,80,0])
        cylinder( d = 4.00, h = 20);
    translate([175,129,0])
        cylinder( d = 4.00, h = 20);
    translate([117,80,0])
        cylinder( d = 4.00, h = 20);
    translate([117,129,0])
        cylinder( d = 4.00, h = 20); 
    // Raspberry Pi Text
    translate ([65,100,3]) {
        rotate([0,0,0])
        linear_extrude(height = 3) {
           text("RASPBERRY PI", font = "Liberation Sans", size = 12);
        }
    }
    
    // STM32
    translate([15,150,0])
        cylinder( d = 4.00, h = 20);  
    translate([60,150,0])
        cylinder( d = 4.00, h = 20); 
    translate([14,75,0])
        cylinder( d = 4.00, h = 20); 
    translate([61,74,0])
        cylinder( d = 4.00, h = 20); 
     // STM32 Text
    translate ([40,90,3]) {
        rotate([0,0,90])
        linear_extrude(height = 3) {
           text("STM 32", font = "Liberation Sans", size = 12);
        }
    }    
     // Switch Text
    translate ([90,165,3]) {
        rotate([0,0,0])
        linear_extrude(height = 3) {
           text("NW Switch", font = "Liberation Sans", size = 12);
        }
    }
     // LICSTER Text
    translate ([30,30,3]) {
        rotate([0,0,0])
        linear_extrude(height = 3) {
           text("LICSTER", font = "Liberation Sans", size = 25);
        }
    }
} 
// left
translate([0,0,0])
    cube(size = [200,10,10], center = false);
// left
translate([0,0,0])
    cube(size = [10,190,10], center = false);
//right
union() difference() { 
   translate([190,0,0])
      cube(size = [10,190,10], center = false);
    translate([190,155,0]) // for NW switch
      cube(size = [30,30,10], center = false);
}
//back
union() difference() { 
  translate([0,180,0])
      cube(size = [190,10,10], center = false);
  translate([85,175,0]) // for NW switch
      cube(size = [190,10,10], center = false);
}

// Front Left
translate([-5,-5,0])
    union() difference() { 
        cube(size = [15,40,165], center = false);
        // Display (RPI Touch Screen)
        translate([7,5,55])
            rotate([-7,0,0])
                cube(size = [8,2,120], center = false);
    }
union() difference() {  //hole
    translate([-5,-5,165])
        cube(size = [15,5,10], center = false);
    translate([5,-5,170])        
        rotate([270,0,0])
            cylinder( d = 4.00, h = 50);
}
// Front Right
translate([190,-5,0])
union() difference() { 
    cube(size = [15,40,165], center = false);
        // Display  (RPI Touch Screen)
        translate([0,5,55])
            rotate([-7,0,0])
                cube(size = [8,2,120], center = false);
    }
union() difference() {  // hole
    translate([190,-5,165])
        cube(size = [15,5,10], center = false);
    translate([195,-5,170])        
        rotate([270,0,0])
            cylinder( d = 4.00, h = 50);
}
// Back Left
translate([-0,175,0])
    cube(size = [10,20,165], center = false);
union() difference() { 
    translate([-0,190,0])
        cube(size = [10,5,175], center = false);
    translate([5,190,170])        
        rotate([270,0,0])
            cylinder( d = 4.00, h = 50);
}
// Back Center
translate([90,185,0])
    cube(size = [20,10,165], center = false);
union() difference() { 
    translate([90,190,0])
        cube(size = [20,5,175], center = false);
    translate([100,190,170])        
        rotate([270,0,0])
            cylinder( d = 4.00, h = 50);
}
// Back Right
translate([180,185,0])
    cube(size = [20,10,165], center = false);
union() difference() { 
    translate([190,190,0])
        cube(size = [10,5,175], center = false);
    translate([195,190,170])        
        rotate([270,0,0])
            cylinder( d = 4.00, h = 50);
}

// Switch
translate([80,150,0])
    cube(size = [120,5,50], center = false);
translate([80,190,0])
    cube(size = [120,5,20], center = false);
translate([80,155,0])
    cube(size = [5,35,20], center = false);
translate([195,155,0])
    cube(size = [5,35,15], center = false);

// Front Pate
union() difference() { 
    translate([0,-5,0])
        cube(size = [200,5,65], center = false);
    // Oled left
    translate([32,-5,20])
        cube(size = [36,5,23], center = false);
    translate([35,-5,17])        
        rotate([270,0,0])
            cylinder( d = 4.00, h = 50);
    translate([65,-5,17])        
        rotate([270,0,0])
            cylinder( d = 4.00, h = 50);
    translate([35,-5,46])        
        rotate([270,0,0])
            cylinder( d = 4.00, h = 50);
    translate([65,-5,46])        
        rotate([270,0,0])
            cylinder( d = 4.00, h = 50);
    // Oled right
    translate([132,-5,20])
        cube(size = [36,5,23], center = false);
    translate([135,-5,17])        
        rotate([270,0,0])
            cylinder( d = 4.00, h = 50);
    translate([165,-5,17])        
        rotate([270,0,0])
            cylinder( d = 4.00, h = 50);
    translate([135,-5,46])        
        rotate([270,0,0])
            cylinder( d = 4.00, h = 50);
    translate([165,-5,46])        
        rotate([270,0,0])
            cylinder( d = 4.00, h = 50);
}