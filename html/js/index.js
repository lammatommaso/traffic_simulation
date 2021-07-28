// const { ipcRenderer } = require("electron")


const green  = "#9bee00"
const orange = "#ffaa00"
const red    = "#f00"
const light_blue = "#00bfff"

const source_node_color = "#4dabf7"
//const dest_node_color = "#be4bdb"
const dest_node_color = source_node_color
const normal_node_color = "#ff9000"

var s = new sigma({
renderer:{
    container: 'main', 
    type: 'canvas'
},
settings: {
    minArrowSize: 10,
    minEdgeSize: 2,
    maxEdgeSize: 5
}
})

s.bind('clickNode', function(e) {
    var nodeId = e.data.node.id
    node_color = e.data.node.color;
    switch(node_color){
        case normal_node_color:
            e.data.node.color = source_node_color
            start.push(parseInt(nodeId.substr(1)))
            break;
        
        case source_node_color:
            start.splice(end.indexOf(parseInt(nodeId.substr(1))),1)
            e.data.node.color = normal_node_color;
            break;
        default:
            break;
    }
    s.refresh();

    //console.log(e)
})


var p      = 0;
var n_cars = 0;
var cols   = 0;
var rows   = 0;
var mean   = 0;
var _sigma = 0;
var min_l  = 0;
var max_l  = 0;
var corsie = 0;

function init(){
    start.splice(0)
    end.splice(0)

    p      = parseFloat(document.getElementById("p").value)
    n_cars = parseInt(document.getElementById("n_cars").value)
    cols   = parseInt(document.getElementById("cols").value)
    rows   = parseInt(document.getElementById("rows").value)
    mean   = parseInt(document.getElementById("mean").value)
    _sigma = parseInt(document.getElementById("sigma").value)
    min_l  = parseInt(document.getElementById("min_l").value)
    max_l  = parseInt(document.getElementById("max_l").value)
    corsie  = parseInt(document.getElementById("corsie").value)

    ipcRenderer.send("init", p,  n_cars, rows, cols,  mean, _sigma, min_l, max_l, corsie);

    loading("block") //mostra gif caricamento
    document.getElementById("cars_at_dest").innerHTML = 0
    document.getElementById("total_cars").innerHTML = n_cars

    document.getElementById("back2home").classList.add("disabled")
    document.getElementById("fwd2sim").classList.add("disabled")

    document.getElementById("play").classList.remove('disabled');
    document.getElementById("graph_back").classList.remove('disabled');
    document.getElementById("l2r").classList.remove('disabled');
    document.getElementById("2wh").classList.remove('disabled');
    document.getElementById("oi").classList.remove('disabled');
    document.getElementById("cr").classList.remove('disabled');
}

ipcRenderer.on("grafo", (event, grafo) => {
    
    s.graph.clear();
    loading("none") //nascondi gif caricamento
    switch_things() //mostra grafo e nascondi settings

    grafo = JSON.parse(grafo)

    var counter = 0;
    for (j = 0; j < rows; j++){
        for (i = 0; i < cols; i++){
            s.graph.addNode({
                id: 'n'.concat(counter),
                x: i,
                y: j,
                size: 1,
                color: normal_node_color

            })
            counter += 1
        }
    }

    counter = 0
    grafo["nodes"].forEach(node => {
        s.graph.addEdge({
            id: 'e'.concat(counter),
            source: 'n'.concat(node.x),
            target: 'n'.concat(node.y),
            type: "curvedArrow",
            color: '#fff',
            size: 1,
            count: 1
        })
        counter += 1
    })

    s.refresh()
    s.refresh()

})

var start = []
var end = []

function two_way_hor(){
    start.splice(0)
    end.splice(0)
    s.graph.nodes().forEach( n => {
        n.color = normal_node_color
    })
    s.refresh()

    var tot_nodi = cols*rows;
    var perc_nodi = parseInt((tot_nodi/100)*65)
    
    s.graph.nodes().forEach( n => {
        id = parseInt(n.id.substr(1))
        if (id <= parseInt(perc_nodi/4) || (id >= (cols*rows) - (parseInt(perc_nodi/2)+1) && id < (cols*rows) - (parseInt(perc_nodi/4)+1)) ){
            start.push(id)
            n.color = source_node_color
        }
        if (id > parseInt(perc_nodi/4) && id <= parseInt(perc_nodi/2) || (id >= (cols*rows) - (parseInt(perc_nodi/4)+1))){
            n.color = dest_node_color
            start.push(id);
        }
    })

    s.refresh()

    create_path(start, start)
}

function out_in(){
    start.splice(0)
    end.splice(0)
    s.graph.nodes().forEach( n => {
        n.color = normal_node_color
    })
    s.refresh()

    var tot_nodi = cols*rows;
    // perc_nodi = parseInt((tot_nodi/100)*65)
    
    _top = rows*2 ; //es. in matrice 10x10, top è 0, 10, 20, ...
    //left = 0; //es. in matrice 10x10, left è 1,2,3,4, ...
    right = rows*cols - rows; //es. in matrice 10x10, right è 90, 91, 92, ...
    bottom = rows*3 - 1 //es. in matrice 10x10, bottom è 19, 29, 39...

    /* per il 'centro' supponiamo di dividere il grafo in 9 sottosezioni
        bisognerà quindi calcolare gli id dei nodi della sezione centrale,
        che saranno circa l'11% di tutti i nodi

        La sezione centrale è un quadrato il cui primo vertice (alto sx) è a circa 1/3
        di n_righe, 1/3 di n_colonne e l'ultimo vertice (basso dx) è 2/3 di n_righe, 
        2/3 di n_colonne. Gli id dei nodi centrali devono essere compresi tra queste 
        coordinate.
    */
    //center = 33, 34, 35, 36, 43, 44, 45, 46, 53, 54, 55, 56, 63, 64, 65, 66

    s.graph.nodes().forEach( n => {
        id = parseInt(n.id.substr(1))
        
        //calcoliamo i nodi di left, che sono le due prime colonne
        if (id < rows*2){
            start.push(id)
            n.color = source_node_color
        }
        //calcoliamo i nodi di top, che sono le prime due righe
        if (id == _top && id < rows*cols - 2*rows){
            _top += rows
            start.push(id)
            n.color = source_node_color
        }

        //calcoliamo i nodi di bottom - ultime due righe
        if (id == bottom && id < rows*cols - 2*rows + (rows-1)){
            bottom += rows
            start.push(id)
            n.color = source_node_color
        }

        //calcoliamo i nodi right - ultime 2 colonne
        if (id >= rows*cols - 2*rows){
            start.push(id)
            n.color = source_node_color
        }

        //calcoliamo i nodi center
        // riga = (nodo/cols)%rows
        // colonna = nodo%cols

        if ((id/cols)%rows >= parseInt(rows/3) && (id/cols)%rows <= 2*(parseInt(rows/3))
            && (id%cols) >= parseInt(cols/3) && id%cols <= 2*(parseInt(cols/3))){
            start.push(id);
            n.color = dest_node_color
        }
    
    
    })

    s.refresh()

    create_path(start, start)
}

function cross(){

    end.splice(0)
    start.splice(0)
    s.graph.nodes().forEach( n => {
        n.color = normal_node_color
    })
    s.refresh()

    /*
        Supponiamo di dividere nei soliti 9 quadranti. 
        Consideriamo il quadrante al centro a sx come partenza,
        quello al centro sopra come arrivo, 
        quello al centro a dx come partenza,
        e quello al centro sotto come arrivo:

        x x x a a a x x x
        x x x a a a x x x
        x x x a a a x x x
        p p p x x x a a a 
        p p p x x x a a a 
        p p p x x x a a a 
        x x x p p p x x x
        x x x p p p x x x
        x x x p p p x x x
    */
    
    s.graph.nodes().forEach( n => {
        id = parseInt(n.id.substr(1))
        
        if ((id/cols)%rows-1 >= parseInt(rows/3) && (id/cols)%rows+1 <= 2*(parseInt(rows/3))
            && (id%cols)+1 <= parseInt(cols/3)){
            start.push(id);
            n.color = source_node_color
        }

        if ((id/cols)%rows-1 >= parseInt(rows/3) && (id/cols)%rows+1 <= 2*(parseInt(rows/3))
            && (id%cols)-1 >= 2*(parseInt(cols/3))){
            start.push(id);
            n.color = dest_node_color
        }

        if ((id/cols)%rows+1 <= parseInt(rows/3)
            && (id%cols)-1 >= parseInt(cols/3) && id%cols+1 <= 2*(parseInt(cols/3))){
            start.push(id);
            n.color = dest_node_color
        }

        if ((id/cols)%rows-1 >= 2*(parseInt(rows/3))
            && (id%cols)-1 >= parseInt(cols/3) && id%cols+1 <= 2*(parseInt(cols/3))){
            start.push(id);
            n.color = source_node_color
        }

    })

    s.refresh()

    create_path(start, start)

}

function left_to_right(){ //funzione molto basica
    //Usiamo il 65% dei nodi come nodi di partenza o di arrivo
    //ovvero il 32.5 come partenza, il 32.5 come arrivo

    start.splice(0)
    end.splice(0)
    s.graph.nodes().forEach( n => {
        n.color = normal_node_color
    })
    s.refresh()

    var tot_nodi = cols*rows;
    var perc_nodi = parseInt((tot_nodi/100)*65)
    
    s.graph.nodes().forEach( n => {
        id = parseInt(n.id.substr(1))
        if (id <= parseInt(perc_nodi/2)){
            start.push(id)
            n.color = source_node_color
        }
        if (id >= (cols*rows) - (parseInt(perc_nodi/2)+1)){
            n.color = dest_node_color
            start.push(id);
        }
    })

    s.refresh()

    create_path(start, start)
}



function create_path(s, d){
    ipcRenderer.send("create_path", s, d)
}

function start_simulation(){
    if (start.length == 0 ){ //|| end.length == 0){
        alert("Nessun insieme di nodi sorgente o destinazione selezionato!")
    } else {
        create_path(start, start) //TODO in realtà lo facciamo già nelle funzioni che creano i percorsi
        //ma se clicchiamo i nodi a mano non viene mai chiamata
        //bisognerebbe correggere questa cosa

        document.getElementById("play").classList.add('disabled');
        document.getElementById("graph_back").classList.add('disabled');
        document.getElementById("l2r").classList.add('disabled');
        document.getElementById("2wh").classList.add('disabled');
        document.getElementById("oi").classList.add('disabled');
        document.getElementById("cr").classList.add('disabled');
        ipcRenderer.send("start_simulation")
    }
}

var used_streets = []

ipcRenderer.on("disegnami", (event, roba_da_disegnare) => {
    values = roba_da_disegnare //JSON.parse(roba_da_disegnare)
    if (!roba_da_disegnare.streets){
        console.log("simulazione terminata")

        used_streets.forEach(element => {

            element = JSON.parse(element)
        
            for (i=0; i < s.graph.edges().length; i++){
                //nota: stiamo confrontando interi e stringhe, ma js è figo e non ha problemi
                if (parseInt(s.graph.edges()[i].source.substr(1)) == element.x && parseInt(s.graph.edges()[i].target.substr(1)) == element.y){
                    //console.log(` macchine a destinazione: ${values["cars_at_dest"]}`)
                    //document.getElementById("cars_at_dest").innerHTML = values["cars_at_dest"] + 1
                    console.log("coloro la strada di blu ")
                    console.log(element)
                    s.graph.edges()[i].color = light_blue;
                    s.refresh();

                    break;
                
                }
            }

            //used_streets.splice(used_streets.indexOf(element), 1) //rimuove la strada dalla lista used_streets

        });

        document.getElementById("graph_back").classList.remove('disabled');
        return
    }
        
    values["streets"].forEach(element => {
        if (used_streets.indexOf(JSON.stringify(element.street)) == -1){
            used_streets.push(JSON.stringify(element.street))
        }
       
        for (i=0; i < s.graph.edges().length; i++){
            //nota: stiamo confrontando interi e stringhe, ma js è figo e non ha problemi
            if (parseInt(s.graph.edges()[i].source.substr(1)) == element.street.x && parseInt(s.graph.edges()[i].target.substr(1)) == element.street.y){
                //console.log(` macchine a destinazione: ${values["cars_at_dest"]}`)
                document.getElementById("cars_at_dest").innerHTML = values["cars_at_dest"] + 1
                
                    if (element.cars == 1){
                        s.graph.edges()[i].color = light_blue;
                        used_streets.splice(used_streets.indexOf(JSON.stringify(element.street)), 1) //rimuove la strada dalla lista used_streets
                    }
                    else if (element.cars/element.max <= 0.5)
                        s.graph.edges()[i].color = green;
                    else if (element.cars/element.max <= 0.75)
                        s.graph.edges()[i].color = orange;
                    else
                        s.graph.edges()[i].color = red;
                    break;
                
            
            }
        }
    });


    s.refresh();
    if (values["cars_at_dest"] < n_cars + 2)
        ipcRenderer.send("next")
     else {
        
        document.getElementById("graph_back").classList.remove('disabled');
        
     }
})


