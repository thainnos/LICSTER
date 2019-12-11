union() difference() { 
    cube(size = [200,190,5], center = false);
    
    // Version
    version = 2;
    for (i = [0, version]){
      translate([180-(i*2.5), 65, 3])
        cylinder( d = 4.00, h = 3);
    }
    
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
    translate ([120,95,3]) {
        rotate([0,0,0])
        linear_extrude(height = 3) {
           text("RPI", font = "Liberation Sans", size = 20);
        }
    }
    
    // STM32
    translate([23,140,0])
        cylinder( d = 4.00, h = 20);  
    translate([68,140,0])
        cylinder( d = 4.00, h = 20); 
    translate([22,65,0])
        cylinder( d = 4.00, h = 20); 
    translate([69,64,0])
        cylinder( d = 4.00, h = 20); 
     // STM32 Text
    translate ([55,70,3]) {
        rotate([0,0,90])
        linear_extrude(height = 3) {
           text("STM 32", font = "Liberation Sans", size = 20);
        }
    }    
     // Switch Text
    translate ([86,150,3]) {
        rotate([0,0,0])
        linear_extrude(height = 3) {
           text("SWITCH", font = "Liberation Sans", size = 20);
        }
    }
     // LICSTER Text
    translate ([13,20,3]) {
        rotate([0,0,0])
        linear_extrude(height = 3) {
           text("LICSTER", font = "Liberation Sans", size = 30);
        }
    }
} 
// front support
translate([0,-5,0])
    cube(size = [200,10,10], center = false);
// left support
translate([0,0,0])
    cube(size = [10,190,10], center = false);
//right support
union() difference() { 
   translate([190,0,0])
      cube(size = [10,190,10], center = false);
    translate([190,145,0]) // for NW switch
      cube(size = [30,30,10], center = false);
}
//back support
union() difference() { 
  translate([0,180,0])
      cube(size = [190,10,10], center = false);
  translate([85,175,0]) // for NW switch
      cube(size = [190,10,10], center = false);
}

// Front Left
translate([-5,-5,0])
    union() difference() { 
        cube(size = [15,40,170], center = false);
        // Display (RPI Touch Screen)
        translate([7,5,55])
            rotate([-15,0,0])
                cube(size = [8,2,120], center = false);
    }
union() difference() {  //hole
    translate([-5,-5,170])
        cube(size = [15,10,15], center = false);
    translate([2.5,-5,180])        
        rotate([270,0,0])
            cylinder( d = 4.00, h = 50);
}
// Front Right
translate([190,-5,0])
union() difference() { 
    cube(size = [15,40,170], center = false);
        // Display  (RPI Touch Screen)
        translate([0,5,55])
            rotate([-15,0,0])
                cube(size = [8,2,120], center = false);
    }
union() difference() {  // hole
    translate([190,-5,170])
        cube(size = [15,10,15], center = false);
    translate([197.5,-5,180])        
        rotate([270,0,0])
            cylinder( d = 4.00, h = 50);
}
// Back Left
translate([-5,170,0])
    cube(size = [15,25,170], center = false);
union() difference() { 
    translate([-5,190,0])
        cube(size = [15,5,185], center = false);
    translate([2.5,190,180])        
        rotate([270,0,0])
            cylinder( d = 4.00, h = 50);
}
// Back Center
translate([90,175,0])
    cube(size = [20,20,170], center = false);
union() difference() { 
    translate([90,190,0])
        cube(size = [20,5,185], center = false);
    translate([100,190,180])        
        rotate([270,0,0])
            cylinder( d = 4.00, h = 50);
}
// Back Right
translate([175,175,0])
    cube(size = [25,20,170], center = false);
union() difference() { 
    translate([180,190,0])
        cube(size = [20,5,185], center = false);
    translate([190,190,180])        
        rotate([270,0,0])
            cylinder( d = 4.00, h = 50);
}

// Switch
translate([80,140,0])
    cube(size = [120,5,50], center = false);
translate([80,175,0])
    cube(size = [120,20,20], center = false);
translate([80,140,0])
    cube(size = [5,50,20], center = false);
translate([195,140,0])
    cube(size = [5,50,15], center = false);

// Front Pate
union() difference() { 
    translate([0,-5,0])
        cube(size = [200,5,65], center = false);
    // Oled left
    translate([50,-3,30])        
        rotate([270,0,0])
            cylinder( d = 60.00, h = 3);
    translate([32,-5,19])
        cube(size = [36,5,25], center = false);
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
    translate([150,-3,30])        
        rotate([270,0,0])
            cylinder( d = 60.00, h = 3);
    translate([132,-5,19])
        cube(size = [36,5,25], center = false);
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