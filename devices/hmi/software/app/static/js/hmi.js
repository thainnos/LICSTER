value= "";

function KonsoleAusgabe(a){
    var feldtext = document.getElementById("password");
    
    if(a==10){
        if (value.length > 0) {
            document.getElementById("myForm").submit(); 
        }
        
    } else if(a==12){
        value = value.substring(0, value.length - 1);
        feldtext.value=value;
        console.log(value); 
    } else {
        value+= a;
        feldtext.value =value;
        console.log(value);
    }
    
}