import { initializeApp } from "https://www.gstatic.com/firebasejs/9.6.1/firebase-app.js";
import { getDatabase, ref, update } from "https://www.gstatic.com/firebasejs/9.6.1/firebase-database.js"

const firebaseConfig = {
    apiKey: "",
    databaseURL: "",
};

let app = null;
let database = null;
let databaseRef = null;

let configureFirebaseButton = document.getElementById("configureFirebase");

let navbarTogglerButton = document.getElementById("navbar-toggler");

let activateCleanAmpButton = document.getElementById("activateCleanAmp");
let activateCrunchAmpButton = document.getElementById("activateCrunchAmp");
let activateLeadAmpButton = document.getElementById("activateLeadAmp");

let enableOverdriveButton = document.getElementById("enableOverdrive");
let disableOverdriveButton = document.getElementById("disableOverdrive");

let enableDelayButton = document.getElementById("enableDelay");
let disableDelayAmpButton = document.getElementById("disableDelay");

let enableReverbButton = document.getElementById("enableReverb");
let disableReverbButton = document.getElementById("disableReverb");

navbarTogglerButton.onclick = function () {
    document.getElementById("modal-background").classList.toggle("color-changed");
}

configureFirebaseButton.onclick = function () {
    let apiKey = document.getElementById("apiKeyInput").value;
    let databaseURL = document.getElementById("databaseUrlInput").value;

    if (!apiKey || !databaseURL)
        return;

    firebaseConfig.apiKey = apiKey;
    firebaseConfig.databaseURL = databaseURL;

    app = initializeApp(firebaseConfig);
    database = getDatabase(app);
    databaseRef = ref(database, "/amp/data/");

    navbarTogglerButton.click();
};

activateCleanAmpButton.onclick = function () { update(databaseRef, { amp_type: 0 }) };
activateCrunchAmpButton.onclick = function () { update(databaseRef, { amp_type: 1 }) };
activateLeadAmpButton.onclick = function () { update(databaseRef, { amp_type: 2 }) };

enableOverdriveButton.onclick = function () { update(databaseRef, { overdrive_active: 1 }) };
disableOverdriveButton.onclick = function () { update(databaseRef, { overdrive_active: 0 }) };

enableDelayButton.onclick = function () { update(databaseRef, { delay_active: 1 }) };
disableDelayAmpButton.onclick = function () { update(databaseRef, { delay_active: 0 }) };

enableReverbButton.onclick = function () { update(databaseRef, { reverb_active: 1 }) };
disableReverbButton.onclick = function () { update(databaseRef, { reverb_active: 0 }) };