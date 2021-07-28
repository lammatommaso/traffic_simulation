function switch_things(){
    if (!(document.getElementById("settings").style.display == "none")){ //settings è visibile
        document.getElementById("graph").style.display = "block"; 
        document.getElementById("settings").style.display = "none";
    } else { //settings non è visibile
        document.getElementById("graph").style.display = "none"; 
        document.getElementById("settings").style.display = "block";

        document.getElementById("back2home").classList.remove("disabled")
        document.getElementById("fwd2sim").classList.remove("disabled")
    
    }
}

function loading(status){
    document.getElementById("loading").style.display = status

}