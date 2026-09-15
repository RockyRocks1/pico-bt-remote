const root = document.getElementById("root");
const calibrationButton = document.getElementById("calibration-btn")
const outputParagraph = document.getElementById("output");

function getNextMovement() {
    return new Promise((resolve) => {
        document.addEventListener("pointermove", (event) => resolve(event), { once: true });
    });
}
function downloadCsv(fileName, fileContent) {
    const blob = new Blob([fileContent], { type: 'text/plain;charset=utf-8' });

    const downloadLink = document.createElement('a');
    downloadLink.href = URL.createObjectURL(blob);
    downloadLink.download = fileName;

      
    document.body.appendChild(downloadLink);
    downloadLink.click();

    document.body.removeChild(downloadLink);
    URL.revokeObjectURL(downloadLink.href);

}
async function startCalibration(event) {
    event.stopPropagation();
    event.preventDefault();

    const startStep = 1;
    const endStep = 127;

    let deltas = [];
    for (let currentStep = startStep; currentStep <= endStep; currentStep++) {
        outputParagraph.textContent = `${currentStep}/${endStep - startStep + 1}`;
        let originMovement = await getNextMovement();
        let endMovement = await getNextMovement();

        let delta = endMovement.movementX

        deltas.push({currentStep, delta});
    };
    let fileContent = "CurrentStep,MouseDelta\n";
    fileContent += deltas.map((entry) => `${entry.currentStep},${entry.delta}`).join("\n");
    console.log(deltas);
    downloadCsv("data.csv", fileContent);
}


calibrationButton.addEventListener("click", startCalibration);