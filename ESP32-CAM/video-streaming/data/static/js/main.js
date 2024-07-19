/**
 * Variable declaration
 * ---------------------------------------------------------------------
 */
// image variables from html
const mainContainer = document.getElementById('main-container');
const view = document.getElementById('stream');
const streamContainer = document.getElementById('stream-container');
// buttons from html
const btnStream = document.getElementById('btn-stream');
const btnFullScreen = document.getElementById('btn-screen');
const btnMenu = document.getElementById('btn-menu');
const btnClose = document.getElementById('btn-close');
const btnXclk = document.getElementById('set-xclk');
// loading message
var loadingMsg = document.getElementById('loading-container');
// full screen 
var fullScreenStatus = document.getElementById('fs-status');
var fullScreenIcon = document.getElementById('fs-icon');
// menu options
const xclkValue = document.getElementById('xclk-value');
const menuContainer = document.getElementById('menu-opt-container');
const qualityInput = document.getElementById('quality');
const qualityValue = document.getElementById('quality-value'); 
const brightnessInput = document.getElementById('brightness');
const brightnessValue = document.getElementById('brightness-value'); 
const contrastInput = document.getElementById('contrast');
const contrastValue = document.getElementById('contrast-value');
const saturationInput = document.getElementById('saturation');
const saturationValue = document.getElementById('saturation-value');
const ledIntensity = document.getElementById('led_intensity');
const ledValue = document.getElementById('led-value');

// pinch zoom gesture variables
// code partially based on :
// github.com/mdn/dom-examples/blob/main/pointerevents/Pinch_zoom_gestures.html
// 
// Global variables to cache event state
var evCache = new Array();
var prevDiff = -1;
//----------------------------------------------------------------------

/**
 * Event listener
 * ---------------------------------------------------------------------
 */
// start stop stream with button
if (btnStream){
  btnStream.addEventListener('click',()=>{
    const streamEnabled = btnStream.innerHTML === 'stop stream';
    if(streamEnabled){
      stopStream();
      loadingMsg.hidden = false;
      btnFullScreen.hidden = true;
      if(fullScreenStatus.innerHTML === 'full'){
        fullScreenStatus.innerHTML = 'normal';
        fullScreenIcon.src = './static/icons/full-screen.svg';
        exitFullscreen();
        if(screen.orientation.type === 'landscape-primary' || screen.orientation.type === 'landscape-secondary'){
          normalLandscape();
        }
        else if(screen.orientation.type === 'portrait-primary'){
          normalPortrait();
        }
      }
    }
    else{
      startStream();
      //streamWithStatusResponse();
      loadingMsg.hidden = true;
      btnFullScreen.hidden = false;
    }
  })
}
// enter/exit full screen mode
if(btnFullScreen && btnFullScreen.hidden){
  btnFullScreen.addEventListener('click',()=>{
    if(fullScreenStatus.innerHTML === 'normal'){
      fullScreenStatus.innerHTML = 'full';
      fullScreenIcon.src = './static/icons/exit-full-screen.svg';
      requestFullscreen(mainContainer);
      if ('orientation' in screen){
        if(screen.orientation.type === 'landscape-primary' || screen.orientation.type === 'landscape-secondary'){
          fullLandscape();
          // fit image to screen on full screen mode
          view.addEventListener("pointerdown", pointerdownHandler);
          view.addEventListener("pointermove", pointermoveHandler);
          view.addEventListener("pointerup", pointerupHandler);
          view.addEventListener("pointercancel", pointerupHandler);
          view.addEventListener("pointerout", pointerupHandler);
          view.addEventListener("pointerleave", pointerupHandler);
        }
        else if(screen.orientation.type === 'portrait-primary'){
          fullPortrait();
          // return image to original size on full screen mode
          view.removeEventListener("pointerdown", pointerdownHandler);
          view.removeEventListener("pointermove", pointermoveHandler);
          view.removeEventListener("pointerup", pointerupHandler);
          view.removeEventListener("pointercancel", pointerupHandler);
          view.removeEventListener("pointerout", pointerupHandler);
          view.removeEventListener("pointerleave", pointerupHandler); 
        }
      }
    }
    else if(fullScreenStatus.innerHTML === 'full'){
      fullScreenStatus.innerHTML = 'normal';
      fullScreenIcon.src = './static/icons/full-screen.svg';
      exitFullscreen();
      if ('orientation' in screen){
        if(screen.orientation.type === 'landscape-primary' || screen.orientation.type === 'landscape-secondary'){
          normalLandscape();
        }
        else if(screen.orientation.type === 'portrait-primary'){
          normalPortrait();
        }
      }
    }
  })
}
// pinch action (zoom in/out)
if ('orientation' in screen){
  screen.addEventListener('change',()=>{
    //console.log(screen.orientation)
    if((screen.orientation.type === 'landscape-primary' || screen.orientation.type === 'landscape-secondary') && fullScreenStatus.innerHTML === 'full'){
      fullLandscape();
      // fit image to screen on full screen mode
      view.addEventListener("pointerdown", pointerdownHandler);
      view.addEventListener("pointermove", pointermoveHandler);
      view.addEventListener("pointerup", pointerupHandler);
      view.addEventListener("pointercancel", pointerupHandler);
      view.addEventListener("pointerout", pointerupHandler);
      view.addEventListener("pointerleave", pointerupHandler);
    }
    else if(screen.orientation.type === 'portrait-primary' && fullScreenStatus.innerHTML === 'full'){
      fullPortrait();
      // return image to original size on full screen mode
      view.removeEventListener("pointerdown", pointerdownHandler);
      view.removeEventListener("pointermove", pointermoveHandler);
      view.removeEventListener("pointerup", pointerupHandler);
      view.removeEventListener("pointercancel", pointerupHandler);
      view.removeEventListener("pointerout", pointerupHandler);
      view.removeEventListener("pointerleave", pointerupHandler); 
    }
    else if((screen.orientation.type === 'landscape-primary' || screen.orientation.type === 'landscape-secondary') && fullScreenStatus.innerHTML === 'normal'){
      normalLandscape();
    }
    else if(screen.orientation.type === 'portrait-primary' && fullScreenStatus.innerHTML === 'normal'){
      normalPortrait();
    }
  });
}

// return to normal mode(close full screen)
// if back button is pressed instead of full screen button
window.addEventListener('fullscreenchange',()=>{
  // exit full screen
  if(document.fullscreenElement === null){
    fullScreenStatus.innerHTML = 'normal';
    fullScreenIcon.src = './static/icons/full-screen.svg';
    exitFullscreen();
    if(screen.orientation.type === 'landscape-primary' || screen.orientation.type === 'landscape-secondary'){
      normalLandscape();
    }
    else if(screen.orientation.type === 'portrait-primary'){
      normalPortrait();
    }
  }
})

// menu options
// open/close menu options
btnMenu.addEventListener('click',()=>{
  menuContainer.hidden = false;
  menuContainer.style.zIndex = 1000;
  view.style.zIndex = 0;
  //console.log("menu opened");
})
btnClose.addEventListener('click',()=>{
  menuContainer.hidden = true;
  //console.log("menu closed")
})
// shows image quality value 
qualityInput.addEventListener("input",(event)=>{
  qualityValue.textContent = event.target.value;
})
// shows image brightness value 
brightnessInput.addEventListener("input",(event)=>{
  brightnessValue.textContent = event.target.value;
})
// shows image contrast value 
contrastInput.addEventListener("input",(event)=>{
  contrastValue.textContent = event.target.value;
})
// shows image saturation value 
saturationInput.addEventListener("input",(event)=>{
  saturationValue.textContent = event.target.value;
})
// control camera led intensity 
ledIntensity.addEventListener("input",(event)=>{
  ledValue.textContent = event.target.value;
})
// change image configuration on tag change
document.querySelectorAll('.default-action').forEach((el)=>{
  el.addEventListener('change',()=>{
    updateConfig(el);
  })
})
// set initial values when DOM loaded
document.addEventListener('DOMContentLoaded',()=>{
  loadingMsg.hidden = false;
  cameraInitialValues();
})

// change xclk on button click
btnXclk.addEventListener('click',()=>{
  let xclkValue = document.getElementById('xclk-value').value;
  setXclkValue(xclkValue);
})

//----------------------------------------------------------------------

/**
 * User functions
 * ---------------------------------------------------------------------
 */
// pinch zoom gesture
// code partially based on :
// github.com/mdn/dom-examples/blob/main/pointerevents/Pinch_zoom_gestures.html
//
function pointerdownHandler(ev) {
  // The pointerdown event signals the start of a touch interaction.
  // This event is cached to support 2-finger gestures
  evCache.push(ev);
}
function pointermoveHandler(ev) {
  // This function implements a 2-pointer horizontal pinch/zoom gesture.
  //
  // If the distance between the two pointers has increased (zoom in),
  // the taget element (image) fit into the screen and if the
  // distance is decreasing (zoom out), the target element (image) return to
  // its original size
  //
  // Find this event in the cache and update its record with this event
  for (var i = 0; i < evCache.length; i++) {
    if (ev.pointerId == evCache[i].pointerId) {
      evCache[i] = ev;
      break;
    }
  }
  // If two pointers are down, check for pinch gestures
  if (evCache.length == 2) {
    // Calculate the distance between the two pointers
    var curDiff = Math.sqrt(
      Math.pow(evCache[1].clientX - evCache[0].clientX, 2) + 
      Math.pow(evCache[1].clientY - evCache[0].clientY, 2)
    );
    if (prevDiff > 0) {
      if (curDiff > prevDiff) {
        // The distance between the two pointers has increased
        //console.log("increase");
        ev.target.style.width = "100%";
        ev.target.style.objectFit = "cover"; 
      }
      if (curDiff < prevDiff) {
        // The distance between the two pointers has decreased
        //console.log("decrease");
        ev.target.style.width = "auto";
        ev.target.style.objectFit = "initial";
      }
    }
    // Cache the distance for the next move event
    prevDiff = curDiff;
  }
}
function pointerupHandler(ev) {
  // Remove this pointer from the cache and reset the target's
  removeEvent(ev);
  // If the number of pointers down is less than two then reset diff tracker
  if (evCache.length < 2) prevDiff = -1;
}
function removeEvent(ev) {
  // Remove this event from the target's cache
  for (var i = 0; i < evCache.length; i++) {
    if (evCache[i].pointerId == ev.pointerId) {
      evCache.splice(i, 1);
      break;
    }
  }
}
// full screen mode 
function requestFullscreen(element) {
  if (element.requestFullscreen) {
    element.requestFullscreen().catch(err => {
      //console.error(`Error attempting to enable full-screen mode: ${err.message} (${err.name})`);
    });
  } else if (element.mozRequestFullScreen) { // Firefox
    element.mozRequestFullScreen().catch(err => {
      //console.error(`Error attempting to enable full-screen mode: ${err.message} (${err.name})`);
    });
  } else if (element.webkitRequestFullscreen) { // Chrome, Safari, and Opera
    element.webkitRequestFullscreen().catch(err => {
      //console.error(`Error attempting to enable full-screen mode: ${err.message} (${err.name})`);
    });
  } else if (element.msRequestFullscreen) { // IE/Edge
    element.msRequestFullscreen().catch(err => {
      //console.error(`Error attempting to enable full-screen mode: ${err.message} (${err.name})`);
    });
  }
}
function exitFullscreen() {
  if (document.exitFullscreen) {
    document.exitFullscreen().catch(err => {
      //console.error(`Error attempting to exit full-screen mode: ${err.message} (${err.name})`);
    });
  } else if (document.mozCancelFullScreen) { // Firefox
    document.mozCancelFullScreen().catch(err => {
      //console.error(`Error attempting to exit full-screen mode: ${err.message} (${err.name})`);
    });
  } else if (document.webkitExitFullscreen) { // Chrome, Safari, and Opera
    document.webkitExitFullscreen().catch(err => {
      //console.error(`Error attempting to exit full-screen mode: ${err.message} (${err.name})`);
    });
  } else if (document.msExitFullscreen) { // IE/Edge
    document.msExitFullscreen().catch(err => {
      //console.error(`Error attempting to exit full-screen mode: ${err.message} (${err.name})`);
    });
  }
}
// simple stream function
function startStream(){
  // start stream 
  var streamUrl = document.location.origin;
  btnStream.innerHTML = 'stop stream';
  btnStream.style.border = 'solid red 2px';
  view.hidden = false;
  view.src = `${streamUrl}/stream`
  // upload an image to the data folder to use instead of the stream for debugging purposes 
  //view.src = "./test.png"; 
}
// stop stream
function stopStream(){
  // stop stream 
  btnStream.innerHTML = 'start stream';
  btnStream.style.border = 'solid green 2px';
  window.stop();
  view.hidden = true;
  view.src="";
}
/*
//THIS CODE MAKES THE STREAM SLOW
// stream after a status ok response
function streamWithStatusResponse(){
  //stream
  var streamUrl = document.location.origin;
  const partBoundary = "123456789000000000000987654321";
  //timeout request 
  const controller = new AbortController();
  const timeoutReq = setTimeout(()=>controller.abort(),2000);

  // initial message on load stream
  loadingMsg.innerHTML = 'loading content...';
  loadingMsg.hidden=false;

  fetch(`${streamUrl}/stream`,{
    //signal: controller.signal,
    method: 'GET',
    headers:{
      'Content-Type': `multipart/x-mixed-replace;boundary=${partBoundary}`
    }
  })
  .then(response=>{
    clearTimeout(timeoutReq);
    if (response.status==200){
      setTimeout(()=>{
        loadingMsg.hidden = true;
      },500)
      startStream();
    }
    else{
      loadingMsg.innerHTML = 'streaming error';
      loadingMsg.hidden=false;
      setTimeout(()=>{
        loadingMsg.hidden = true;
      },1500);
    }
  })
  .catch(error=>{
    if (error.name === 'AbortError'){
      loadingMsg.innerHTML = 'streaming error: timeout reached';
    }
    else{
      loadingMsg.innerHTML = 'streaming error + :'+error.message;
    }
    setTimeout(()=>{
      loadingMsg.hidden = true;
    },1500);
  })
};
*/
// control actions on screen rotation
function normalLandscape(){
  var landscapeMin = window.matchMedia('only screen and (max-height:430px) and (orientation:landscape)');
  var lanscaspeMid = window.matchMedia('only screen and (min-height:431px) and (max-height:800px) and (orientation:landscape)');
  //var landscapeMax = window.matchMedia('only screen and (min-height:801px)');

  streamContainer.style.width = 'calc(100vh - 50px)';
  streamContainer.style.height = 'calc(0.70*(100vh - 50px))';
  streamContainer.style.top = '4em';
  view.style.width = '100%';
  view.style.height = '100%';
  btnStream.style.color = 'black';
  btnFullScreen.style.filter = 'invert(0%) sepia(0%) saturate(18%) hue-rotate(293deg) brightness(102%) contrast(105%)';
  btnMenu.style.filter = 'invert(0%) sepia(0%) saturate(18%) hue-rotate(293deg) brightness(102%) contrast(105%)';
  btnClose.style.filter = 'invert(0%) sepia(0%) saturate(18%) hue-rotate(293deg) brightness(102%) contrast(105%)';
  menuContainer.style.color = 'black';
  if (landscapeMin.matches){
    streamContainer.style.width = 'calc(100vh - 50px)';
  }
  else if(lanscaspeMid.matches){
    streamContainer.style.width = 'calc(100vh - 82px)';
  }
}
function normalPortrait(){
  streamContainer.style.width = 'calc(100vw - 80px)';
  streamContainer.style.height = 'calc(0.70*(100vw - 80px))';
  streamContainer.style.top = '4em';
  view.style.width = '100%';
  view.style.height = '100%';
  btnStream.style.color = 'black';
  btnFullScreen.style.filter = 'invert(0%) sepia(0%) saturate(18%) hue-rotate(293deg) brightness(102%) contrast(105%)';
  btnMenu.style.filter = 'invert(0%) sepia(0%) saturate(18%) hue-rotate(293deg) brightness(102%) contrast(105%)';
  btnClose.style.filter = 'invert(0%) sepia(0%) saturate(18%) hue-rotate(293deg) brightness(102%) contrast(105%)';
  menuContainer.style.color = 'black';
  //console.log("normal portrait");
}
function fullLandscape(){
  streamContainer.style.width = '100vw';
  streamContainer.style.height = '100vh';
  streamContainer.style.top = '0em';
  view.style.width = 'auto';
  btnStream.style.color = 'white';
  btnFullScreen.style.filter = 'invert(100%) sepia(3%) saturate(549%) hue-rotate(219deg) brightness(119%) contrast(100%)';
  btnMenu.style.filter = 'invert(100%) sepia(3%) saturate(549%) hue-rotate(219deg) brightness(119%) contrast(100%)';
  btnClose.style.filter = 'invert(100%) sepia(3%) saturate(549%) hue-rotate(219deg) brightness(119%) contrast(100%)';
  menuContainer.style.color = 'white';
  //console.log("full landscape");
}
function fullPortrait(){
  streamContainer.style.width = '100vw';
  streamContainer.style.height = 'auto';
  streamContainer.style.top = '15.5em';
  view.style.width = 'auto';
  btnStream.style.color = 'white';
  btnFullScreen.style.filter = 'invert(100%) sepia(3%) saturate(549%) hue-rotate(219deg) brightness(119%) contrast(100%)';
  btnMenu.style.filter = 'invert(100%) sepia(3%) saturate(549%) hue-rotate(219deg) brightness(119%) contrast(100%)';
  btnClose.style.filter = 'invert(100%) sepia(3%) saturate(549%) hue-rotate(219deg) brightness(119%) contrast(100%)';
  menuContainer.style.color = 'white';
  //console.log("full portrait");
}
// change image values
function updateConfig(el){
  let value;
  switch(el.type){
    case 'checkbox':
      value = el.checked ? 1 : 0;
      break;
    case 'range':
    case 'select-one':
      value = el.value;
      break;
    case 'button':
    case 'submit':
      value = '1';
      break;
    default:
      return;
  }
  const baseHost = document.location.origin;
  const query = `${baseHost}/control?var=${el.id}&val=${value}`;

  fetch(query)
    .then(response=>{
      //console.log(`request to ${query} finished, status: ${response.status}`)
    })
}


function updateValue(el,value,updateRemote){
  updateRemote = updateRemote == null ? true : updateRemote;
  let initialValue;

  if(el.type === 'checkbox'){
    initialValue = el.checked;
    // make sure to explicity convet to a boolean
    value = !!value;
    el.checked = value;
  }
  else{
    initialValue = el.value
    el.value = value
  }
  if(updateRemote && initialValue !== value){
    updateConfig(el);
  }
}

function cameraInitialValues(){
  const baseHost = document.location.origin;
  fetch(`${baseHost}/status`)
  .then(response=>{
    return response.json();
  })
  .then(state=>{
    //console.log(state);
    document.querySelectorAll('.default-action').forEach(el=>{
      // update states of buttons, sliders and check buttons 
      updateValue(el,state[el.id],false);
      //update html values
      brightnessValue.textContent = brightnessInput.value;
      ledValue.textContent = ledIntensity.value;
      saturationValue.textContent = saturationInput.value;
      contrastValue.textContent = contrastInput.value;
      qualityValue.textContent = qualityInput.value;

    })
  })
}

function setXclkValue(value){
  const baseHost = document.location.origin;
  const query = `${baseHost}/xclk?xclk=${value}`;
  fetch(query)
  .then(response =>{
    if (response.status !== 200){
      console.log("Error["+response.status+"]:"+response.statusText);
    }
    else{
      //console.log(`request to ${query} finished, status: ${response.status}`)
      return response.text();
    }
  })
  .then(data=>{
    //console.log(data);
  })
  .catch(error=>{
    console.log("Error[-1]:"+error);
  })
}
//----------------------------------------------------------------------