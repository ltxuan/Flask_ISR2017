socket.on("RES_ROUTER_STATUS", msg =>{
	console.log(msg);
    document.querySelector("#GE1_router").innerText = msg.GE1
    document.querySelector("#GE2_router").innerText = msg.GE2
    document.querySelector("#GE3_router").innerText = msg.GE3
    document.querySelector("#GE4_router").innerText = msg.GE4
});
