import { initializeApp } from "https://www.gstatic.com/firebasejs/9.6.1/firebase-app.js";
import { getDatabase, ref, update, get, set, onValue } from "https://www.gstatic.com/firebasejs/9.6.1/firebase-database.js"

let preset = {
    amp_type: 0,
    overdrive_active: 0,
    delay_active: 0,
    reverb_active: 0,
    selected_preset: "None"
};

let firebaseConfig = {
    apiKey: "",
    databaseURL: "",
};

let app = null;
let database = null;
let databaseRef = null;

let setConfigDialogButton = document.getElementById("setConfigDialog");
let resetConfigDialogButton = document.getElementById("resetConfigDialog");

let navbarSettingsButton = document.getElementById("navbarSettingsButton");

let apiKeyInput = document.getElementById("apiKeyInput");
let databaseUrlInput = document.getElementById("databaseUrlInput");

let activateCleanAmpButton = document.getElementById("activateCleanAmp");
let activateCrunchAmpButton = document.getElementById("activateCrunchAmp");
let activateLeadAmpButton = document.getElementById("activateLeadAmp");

let enableOverdriveButton = document.getElementById("enableOverdrive");
let disableOverdriveButton = document.getElementById("disableOverdrive");

let enableDelayButton = document.getElementById("enableDelay");
let disableDelayAmpButton = document.getElementById("disableDelay");

let enableReverbButton = document.getElementById("enableReverb");
let disableReverbButton = document.getElementById("disableReverb");

let largeAmbientPresetButton = document.getElementById("largeAmbientPreset");
let rainDropsPresetButton = document.getElementById("rainDropsPreset");
let jimmyHendriffsPresetButton = document.getElementById("jimmyHendriffsPreset");
let johnnyBLeadPresetButton = document.getElementById("johnnyBLeadPreset");
let architectsRhythmPresetButton = document.getElementById("architectsRhythmPreset");
let memphisMayShreadPresetButton = document.getElementById("memphisMayShreadPreset");

let selectedPreset = document.getElementById("selectedPreset");

function docReady(fn) {
    if (document.readyState === "complete" || document.readyState === "interactive")
        setTimeout(fn, 1);

    else document.addEventListener("DOMContentLoaded", fn);
}

function loadFirebaseConfigFromCookies() {
    let apiKey = getCookie("apiKey");
    let databaseURL = getCookie("databaseURL");

    if (apiKey && databaseURL) {
        firebaseConfig.apiKey = apiKey;
        firebaseConfig.databaseURL = databaseURL;

        apiKeyInput.value = apiKey;
        databaseUrlInput.value = databaseURL;

        invalidateConfigDialog();

        return true;
    }

    return false;
}

function updateWebpageFromFirebaseData(data) {
    let ampTypeContainer = document.getElementById("ampTypeContainer");
    let effectPedalContainer = document.getElementById("effectPedalContainer");

    ampTypeContainer.querySelectorAll("input")[data.amp_type].checked = true;
    effectPedalContainer.querySelectorAll("input")[1 - data.overdrive_active].checked = true;
    effectPedalContainer.querySelectorAll("input")[1 - data.delay_active + 2].checked = true;
    effectPedalContainer.querySelectorAll("input")[1 - data.reverb_active + 4].checked = true;
    selectedPreset.innerText = data.selected_preset;
}

function loadFirebase() {
    let apiKey = apiKeyInput.value;
    let databaseURL = databaseUrlInput.value;

    if (!apiKey || !databaseURL)
        return;

    firebaseConfig.apiKey = apiKey;
    firebaseConfig.databaseURL = databaseURL;

    app = initializeApp(firebaseConfig);
    database = getDatabase(app);
    databaseRef = ref(database);

    get(databaseRef).then(data => updateWebpageFromFirebaseData(data.val())).catch(console.error);

    onValue(databaseRef, (data) => updateWebpageFromFirebaseData(data.val()));

    document.querySelectorAll("input:disabled").forEach(b => b.removeAttribute("disabled"));
}

function invalidateConfigDialog() {
    setConfigDialogButton.disabled = true;
    apiKeyInput.readOnly = true;
    databaseUrlInput.readOnly = true;
}

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

        while (c.charAt(0) == ' ')
            c = c.substring(1, c.length);

        if (c.indexOf(nameEQ) == 0)
            return c.substring(nameEQ.length, c.length);
    }

    return null;
}

function removeCookie(name) {
    document.cookie = name + '=; Path=/; Expires=Thu, 01 Jan 1970 00:00:01 GMT;';
}

function setPreset(name, ampType, overdrive, delay, reverb){
    preset.amp_type = ampType;
    preset.overdrive_active = overdrive;
    preset.delay_active = delay;
    preset.reverb_active = reverb;
    preset.selected_preset = name;
    selectedPreset.innerText = name;

    set(databaseRef, preset);
}

docReady(function () {
    if (loadFirebaseConfigFromCookies()) {
        document.querySelectorAll("input:disabled").forEach(b => b.removeAttribute("disabled"));

        loadFirebase();
    }
});

navbarSettingsButton.onclick = function () {
    loadFirebaseConfigFromCookies();
    document.getElementById("modal-background").classList.toggle("color-changed");
}

setConfigDialogButton.onclick = function () {
    loadFirebase();

    setCookie("apiKey", firebaseConfig.apiKey, 7);
    setCookie("databaseURL", firebaseConfig.databaseURL, 7);

    invalidateConfigDialog();

    navbarSettingsButton.click();
};

resetConfigDialogButton.onclick = function () {
    apiKeyInput.value = "";
    databaseUrlInput.value = "";

    removeCookie("apiKey");
    removeCookie("databaseURL");

    location.reload();
}

activateCleanAmpButton.onclick = function () { update(databaseRef, { amp_type: 0 }) };
activateCrunchAmpButton.onclick = function () { update(databaseRef, { amp_type: 1 }) };
activateLeadAmpButton.onclick = function () { update(databaseRef, { amp_type: 2 }) };

enableOverdriveButton.onclick = function () { update(databaseRef, { overdrive_active: 1 }) };
disableOverdriveButton.onclick = function () { update(databaseRef, { overdrive_active: 0 }) };

enableDelayButton.onclick = function () { update(databaseRef, { delay_active: 1 }) };
disableDelayAmpButton.onclick = function () { update(databaseRef, { delay_active: 0 }) };

enableReverbButton.onclick = function () { update(databaseRef, { reverb_active: 1 }) };
disableReverbButton.onclick = function () { update(databaseRef, { reverb_active: 0 }) };

largeAmbientPresetButton.onclick = function() { setPreset(this.innerText, 0, 0, 0, 1) };
rainDropsPresetButton.onclick = function() { setPreset(this.innerText, 0, 0, 1, 1) };
jimmyHendriffsPresetButton.onclick = function() { setPreset(this.innerText, 1, 0, 0, 0) };
johnnyBLeadPresetButton.onclick = function() { setPreset(this.innerText, 1, 1, 0, 1) };
architectsRhythmPresetButton.onclick = function() { setPreset(this.innerText, 2, 1, 0, 0) };
memphisMayShreadPresetButton.onclick = function() { setPreset(this.innerText, 2, 1, 1, 1) };