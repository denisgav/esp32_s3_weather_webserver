// ======== Helpers ========

const $ = sel => document.querySelector(sel);
/*
const fmt2 = n => String(n).padStart(2, "0");
const toISODateTimeLocal = (d) => {
    const y = d.getFullYear(), m = fmt2(d.getMonth() + 1), da = fmt2(d.getDate());
    const h = fmt2(d.getHours()), mi = fmt2(d.getMinutes()), s = fmt2(d.getSeconds());
    return { date: `${y}-${m}-${da}`, time: `${h}:${mi}:${s}` };
};
const epochFromInputs = () => {
    const ds = $("#datePick").value, ts = $("#timePick").value;
    if (!ds || !ts) return null;
    // compose as local time
    const [Y, M, D] = ds.split("-").map(Number);
    const [h, m, s] = ts.split(":").map(Number);
    return Math.floor(new Date(Y, (M - 1), D, h || 0, m || 0, s || 0).getTime() / 1000);
};
const rtcNow = $("#rtcNow"), pcNow = $("#pcNow"), timeStatus = $("#timeStatus");
*/

const el_bme280_t = $("#bme280_t");
const el_bme280_h = $("#bme280_h");
const el_bme280_p = $("#bme280_p");
const el_bme280_status = $("#bme280_status");

// ======== BME280 ========
async function fetch_sensor_data() {
    try {
        const r = await fetch("/api/fetch_sensor_data"); // { t,h,p } or {temperature,...}
        const j = await r.json();

        const bme280_temperature = j.bme280_temperature;
        const bme280_pressure = j.bme280_pressure
        const bme280_humidity = j.bme280_humidity

        el_bme280_t.textContent = (bme280_temperature != null) ? (bme280_temperature.toFixed ? bme280_temperature.toFixed(1) : bme280_temperature) + "°C" : "—";
        el_bme280_h.textContent = (bme280_humidity != null) ? (bme280_humidity.toFixed ? bme280_humidity.toFixed(0) : bme280_humidity) + "%" : "—";
        el_bme280_p.textContent = (bme280_pressure != null) ? (bme280_pressure.toFixed ? bme280_pressure.toFixed(0) : bme280_pressure) + " hPa" : "—";
        el_bme280_status.textContent = "OK";
        el_bme280_status.className = "ok";
    } catch (e) {
        el_bme280_status.textContent = "error";
        el_bme280_status.className = "bad";
    }
}
$("#btnRefresh").addEventListener("click", fetch_sensor_data);

/*
// ======== Time (DS3231 & PC) ========
async function refreshRTC() {
    // local PC time
    const d = new Date();
    const loc = toISODateTimeLocal(d);
    pcNow.textContent = `${loc.date} ${loc.time}`;

    // device RTC
    try {
        const r = await fetch("/api/time"); // { epoch: 1690000000 } or {rtc:169..., now:...}
        const j = await r.json();
        const epo = j.epoch ?? j.rtc ?? j.now;
        if (epo) {
            const dd = new Date(epo * 1000);
            const rloc = toISODateTimeLocal(dd);
            rtcNow.textContent = `${rloc.date} ${rloc.time}`;
            timeStatus.textContent = "DS3231";
            timeStatus.className = "muted";
        } else {
            rtcNow.textContent = "—";
            timeStatus.textContent = "нет данных";
            timeStatus.className = "bad";
        }
    } catch (e) {
        rtcNow.textContent = "—";
        timeStatus.textContent = "ошибка";
        timeStatus.className = "bad";
    }
}
function tickPC() {
    const d = new Date();
    const loc = toISODateTimeLocal(d);
    pcNow.textContent = `${loc.date} ${loc.time}`;
    requestAnimationFrame(() => setTimeout(tickPC, 200));
}
tickPC();

async function syncToPC() {
    // send current local epoch seconds
    const epoch = Math.floor(Date.now() / 1000);
    try {
        const r = await fetch("/api/time", { method: "POST", headers: { "Content-Type": "application/json" }, body: JSON.stringify({ epoch }) });
        if (!r.ok) throw 0;
        await refreshRTC();
        alert("Время DS3231 синхронизировано с временем компьютера.");
    } catch (e) {
        alert("Не удалось установить время.");
    }
}
$("#btnSync").addEventListener("click", syncToPC);
$("#btnSetPicked").addEventListener("click", async () => {
    const epoch = epochFromInputs();
    if (!epoch) { alert("Укажите дату и время."); return; }
    try {
        const r = await fetch("/api/time", { method: "POST", headers: { "Content-Type": "application/json" }, body: JSON.stringify({ epoch }) });
        if (!r.ok) throw 0;
        await refreshRTC();
        alert("Время DS3231 обновлено.");
    } catch (e) { alert("Ошибка установки времени."); }
});
*/

// ======== Wi‑Fi Config ========
const wifiDlg = $("#wifiDlg"), btnWifi = $("#btnWifi"), wifiMsg = $("#wifiMsg");
const ssidSelect = $("#ssidSelect"), ssidManual = $("#ssidManual");
const password = $("#password"), hostname = $("#hostname"), ip = $("#ip");

btnWifi.addEventListener("click", async () => {
    wifiDlg.showModal();
    wifiMsg.textContent = "Scanning…";
    // clear and add placeholder
    ssidSelect.innerHTML = "<option>Загрузка…</option>";
    try {
        const r = await fetch("/api/wifi/scan"); // expected: ["MyWiFi","Guest",...]
        const list = await r.json();
        ssidSelect.innerHTML = "";
        (list || []).forEach(ss => {
            const o = document.createElement("option"); o.value = o.textContent = ss; ssidSelect.appendChild(o);
        });
        wifiMsg.textContent = "select one of existing networks or enter manually.";
    } catch (e) {
        ssidSelect.innerHTML = "<option value=''>—</option>";
        wifiMsg.textContent = "Can not get list of networks.";
    }
    // load current status if available
    try {
        const r2 = await fetch("/api/wifi/status"); // {ssid, hostname, ip}
        const j = await r2.json();
        if (j && j.ssid) {
            // preselect if in list
            const opt = Array.from(ssidSelect.options).find(o => o.value === j.ssid);
            if (opt) ssidSelect.value = j.ssid; else ssidManual.value = j.ssid;
        }
        if (j && j.hostname) hostname.value = j.hostname;
        if (j && j.ip) ip.value = j.ip;
    } catch (e) { }
});

$("#btnWifiSave").addEventListener("click", async (ev) => {
    ev.preventDefault();
    const ssid = (ssidManual.value || "").trim() || ssidSelect.value;
    if (!ssid) { alert("Select SSID."); return; }
    const data = { ssid, password: password.value, hostname: hostname.value || undefined, ip: ip.value || undefined };
    try {
        const r = await fetch("/api/wifi/save", { method: "POST", headers: { "Content-Type": "application/json" }, body: JSON.stringify(data) });
        if (!r.ok) throw 0;
        wifiMsg.textContent = "Saved. Please reconnect.";
        setTimeout(() => wifiDlg.close(), 400);
    } catch (e) {
        wifiMsg.textContent = "Failed to save.";
    }
});

// ======== Init ========
(function init() {
//    const now = new Date(); const { date, time } = toISODateTimeLocal(now);
//    $("#datePick").value = date; $("#timePick").value = time;
//    fetch_sensor_data(); 
//    refreshRTC();
    setTimeout(fetch_sensor_data, 100);
    setInterval(fetch_sensor_data, 5000);
//    setInterval(refreshRTC, 5000);
})();