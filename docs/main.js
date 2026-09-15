const root = document.getElementById("root");
const calibrationButton = document.getElementById("calibration-btn")
const outputParagraph = document.getElementById("output");

function getNextMovement() {
    let resolvePromise;
    let promise = new Promise((resolve) => {
        resolvePromise = resolve;
    });
  
    document.addEventListener("pointermove", (event) => {
        resolvePromise(event);
    }, { once: true });
  
    return promise;
}
async function startCalibration(event) {
    event.stopPropagation();
    event.preventDefault();

    const startStep = 1;
    const endStep = 127;

    let deltas = [];
    for (let currentStep = startStep; currentStep <= endStep; currentStep++) {
        let originMovement = await getNextMovement();
        let endMovement = await getNextMovement();

        let deltaX = endMovement.movementX;

        deltas.push({currentStep, deltaX});
        outputParagraph.textContent = `${currentStep}/${endStep - startStep}`;
    };
    outputParagraph.textContent = JSON.stringify(deltas).replaceAll("},","}\n");

    console.log(deltas);
}


calibrationButton.addEventListener("click", startCalibration);