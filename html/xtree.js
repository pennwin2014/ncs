scores = new Array(20);
var numTotal=0;
NS4 = (document.layers) ? 1 : 0;
IE4 = (document.all) ? 1 : 0;
ver4 = (NS4 || IE4) ? 1 : 0;
if (ver4) {
    with (document) {
            write("<STYLE TYPE='text/css'>");
                    if (NS4) {
                                write(".parent {position:absolute; visibility:visible}");
                                write(".child {position:absolute; visibility:visible}");
                                write(".regular {position:absolute; visibility:visible}")        
                    }else {            
                    		write(".child {display:none}")        
                    }
                    write("</STYLE>");    
     }
}
function getIndex(el) {
    ind = null;    
    for (i=0; i<document.layers.length; i++) {
            whichEl = document.layers[i];        
            if (whichEl.id == el) {    
                    ind = i;            
                    break;        
            }    
     }    
     return ind;
}
function arrange() {    
	nextY = document.layers[firstInd].pageY +document.layers[firstInd].document.height;    
	for (i=firstInd+1; i<document.layers.length; i++) {
	        whichEl = document.layers[i];        
	        if (whichEl.visibility != "hide") {     
	               whichEl.pageY = nextY;            
	               nextY += whichEl.document.height;        
	        }   
	}
}
function initIt(){    
	if (!ver4) return;    
	if (NS4) {        
		for (i=0; i<document.layers.length; i++) {       
		     whichEl = document.layers[i];            
		     if (whichEl.id.indexOf("Child") != -1) 
		     	whichEl.visibility = "hide"; 
		     }        
		arrange();    
	}else {    
	    divColl = document.all.tags("DIV");        
	    for (i=0; i<divColl.length; i++) {       
	         whichEl = divColl(i);            
	         if (whichEl.className == "child") whichEl.style.display = "none";        
	    }    
	}
}
function expandIt(el) {	
	if (!ver4) return;    
	if (IE4) {    
	    whichEl1 = eval(el + "Child");		
	    for(i=1;i<=numTotal;i++){		
	    	whichEl = eval(scores[i] + "Child");			
	    	if(whichEl!=whichEl1) {			
	    		whichEl.style.display = "none";			
	    	}	
	    }        
	    whichEl1 = eval(el + "Child");        
	    if (whichEl1.style.display == "none") {      
	          whichEl1.style.display = "block";        
	    } else {     
	           whichEl1.style.display = "none";        
	    }    
	}else {    
	    whichEl = eval("document." + el + "Child");		
	    for(i=1;i<=numTotal;i++){		
	    	whichEl = eval("document." + scores[i] + "Child");	
	    	if(whichEl!=whichEl1) {			
	    		whichEl.visibility = "hide";			
	    	}		
	    }        
	    if (whichEl.visibility == "hide") {        
	        whichEl.visibility = "show";        
	    }else {        
	        whichEl.visibility = "hide";        
	    }        
	    arrange();    
	}
}
onload = initIt;
