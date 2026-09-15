const root = document.getElementById("root");
const calibrationButton = document.getElementById("calibration-btn")
const outputParagraph = document.getElementById("output");

function getNextClick() {
    let resolvePromise;
    let promise = new Promise((resolve) => {
        resolvePromise = resolve;
    });
  
    document.addEventListener("pointerdown", (event) => {
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
        let originClick = await getNextClick();
        let endClick = await getNextClick();

        let deltaX = endClick.clientX - originClick.clientX;

        deltas.push({currentStep, deltaX});
        outputParagraph.textContent = `${currentStep}/${endStep - startStep}`;
    };
    outputParagraph.textContent = JSON.stringify(deltas).replaceAll("},","}\n");

    console.log(deltas);
}


calibrationButton.addEventListener("click", startCalibration);