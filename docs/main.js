const root = document.getElementById("root");
const calibrationButton = document.getElementById("calibration-btn")
const outputParagraph = document.getElementById("output");

function enableFullscreen() {
    const isFullscreen = document.fullscreenElement || document.webkitFullscreenElement;
    if (isFullscreen)
        return;
    if (root.requestFullscreen)
        root.requestFullscreen();
    else if (root.webkitRequestFullscreen)
        root.webkitRequestFullscreen();
}
function disableFullscreen() {
    const isFullscreen = document.fullscreenElement || document.webkitFullscreenElement;
    if (!isFullscreen)
        return;
    if (document.exitFullscreen)
        document.exitFullscreen();
    else if (document.webkitExitFullscreen)
        document.webkitExitFullscreen();
}
function getNextClick() {
    let resolvePromise;
    let promise = new Promise((resolve) => {
        resolvePromise = resolve;
    });
  
    document.addEventListener("mousedown", (event) => {
        resolvePromise(event);
    }, { once: true });
  
    return promise;
}
async function startCalibration() {
    const startStep = 1;
    const endStep = 127;
    enableFullscreen();
    let deltas = [];
    for (let currentStep = startStep; currentStep <= endStep; currentStep++) {
        let originClick = await getNextClick();
        let endClick = await getNextClick();

        let deltaX = endClick.clientX - originClick.clientX;

        deltas.push({currentStep, deltaX});
    };
    outputParagraph.textContent = JSON.stringify(deltas).replaceAll("},", "}\n");
    disableFullscreen();
    console.log(deltas);
}


calibrationButton.addEventListener("click", startCalibration);