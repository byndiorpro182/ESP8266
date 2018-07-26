var hostID = "http://hoangvanhieu182.000webhostapp.com";
//document.getElementById("hieu").innerHTML = "aa";
getResponseTextForPath("/api/nav.txt","Navigation")
function getResponseTextForPath(path,id) {
    var xhttp;
    if (window.XMLHttpRequest) {
        xhttp = new XMLHttpRequest();
        console.log("xhtp = XMLHttp");
    } else {
        xhttp = new ActiveXObject("Microsoft.XMLHTTP");
        console.log("xhttp = ActiveObject");
    }
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            console.log("\r\npath = " + path + ", ID = " + id);
            document.getElementById(id).innerHTML = this.responseText;
        }
    };
    xhttp.open("GET", hostID + path, true);
    xhttp.send();
}