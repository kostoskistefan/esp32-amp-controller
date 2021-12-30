import { initializeApp } from "https://www.gstatic.com/firebasejs/9.6.1/firebase-app.js";
import { getDatabase, ref, update, get } from "https://www.gstatic.com/firebasejs/9.6.1/firebase-database.js"

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

function docReady(fn) {
    if (document.readyState === "complete" || document.readyState === "interactive")
        setTimeout(fn, 1);

    else document.addEventListener("DOMContentLoaded", fn);
}

docReady(function () {
    if (load_firebase_config_from_cookies()) {
        document.querySelectorAll("input:disabled").forEach(b => b.removeAttribute("disabled"));
        
        let apiKey = document.getElementById("apiKeyInput").value;
        let databaseURL = document.getElementById("databaseUrlInput").value;

        load_firebase(apiKey, databaseURL);
    }
});

function load_firebase_config_from_cookies() {
    let apiKey = getCookie("apiKey");
    let databaseURL = getCookie("databaseURL");

    if (apiKey && databaseURL) {
        firebaseConfig.apiKey = apiKey;
        firebaseConfig.databaseURL = databaseURL;

        document.getElementById("apiKeyInput").value = apiKey;
        document.getElementById("databaseUrlInput").value = databaseURL;

        return true;
    }

    return false;
}

navbarTogglerButton.onclick = function () {
    load_firebase_config_from_cookies();
    document.getElementById("modal-background").classList.toggle("color-changed");
}

function load_firebase(apiKey, databaseURL) {
    if (!apiKey || !databaseURL)
        return;

    firebaseConfig.apiKey = apiKey;
    firebaseConfig.databaseURL = databaseURL;

    app = initializeApp(firebaseConfig);
    database = getDatabase(app);
    databaseRef = databaseRef = ref(database);

    get(databaseRef).then(rawData => {
        let data = rawData.val();
        document.getElementById("ampTypeContainer").querySelectorAll("input")[data.amp_type].checked = true;
        document.getElementById("effectPedalContainer").querySelectorAll("input")[1 - data.overdrive_active].checked = true;
        document.getElementById("effectPedalContainer").querySelectorAll("input")[1 - data.delay_active + 2].checked = true;
        document.getElementById("effectPedalContainer").querySelectorAll("input")[1 - data.reverb_active + 4].checked = true;
    }).catch((error) => {
        console.error(error);
    });

    document.querySelectorAll("input:disabled").forEach(b => b.removeAttribute("disabled"));
}

configureFirebaseButton.onclick = function () {
    let apiKey = document.getElementById("apiKeyInput").value;
    let databaseURL = document.getElementById("databaseUrlInput").value;

    load_firebase(apiKey, databaseURL);

    setCookie("apiKey", apiKey, 7);
    setCookie("databaseURL", databaseURL, 7);

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

function setCookie(name, value, days) {
    var expires = "";
    if (days) {
        var date = new Date();
        date.setTime(date.getTime() + (days * 24 * 60 * 60 * 1000));
        expires = "; expires=" + date.toUTCString();
    }
    document.cookie = name + "=" + (value || "") + expires + "; path=/";
}

function getCookie(name) {
    var nameEQ = name + "=";
    var ca = document.cookie.split(';');
    for (var i = 0; i < ca.length; i++) {
        var c = ca[i];
        while (c.charAt(0) == ' ') c = c.substring(1, c.length);
        if (c.indexOf(nameEQ) == 0) return c.substring(nameEQ.length, c.length);
    }
    return null;
}