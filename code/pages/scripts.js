function showBlinkValue(blinkValue){
    document.getElementById("blink-speed-value").innerHTML=blinkValue;
   }
   
   function updateBlinkValue(blinkValue){
    console.log("updated value is " + blinkValue);
        var route ='/changeBlink' +'?min='+ 15 +'&max='+ 100 +'&speed='+ blinkValue;
     callRoute(route);
   }
   
   function changeColor(r, g, b){
       console.log("color is  r" + r +" g" + g + " b"+b);
     
     const Http = new XMLHttpRequest();
       var route ='/changeColor' +'?r='+ r +'&g='+ g +'&b='+ b;
     callRoute(route);
   }
   
   function callRoute(route){
    serverIpInput = document.getElementById("server-ip").value
    const Http = new XMLHttpRequest();
    //var url = 'http://192.168.1.13';
    //var url = 'https://www.google.fr';
    //var url = 'http://82.120.101.253:80'
    var url = serverIpInput;
     
       url += route
   
     console.log(url);
     Http.open('GET', url);
     Http.send();
   }