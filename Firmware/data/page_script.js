// ======== Helpers ========

const $ = sel => document.querySelector(sel);

const fmt2 = n => String(n).padStart(2, "0");

const toISODateTimeLocal = (d) => {
    const y = d.getFullYear(), m = fmt2(d.getMonth() + 1), da = fmt2(d.getDate());
    const h = fmt2(d.getHours()), mi = fmt2(d.getMinutes()), s = fmt2(d.getSeconds());
    return { date: `${y}-${m}-${da}`, time: `${h}:${mi}:${s}` };
};

function to_ISO_DateTime_RTC (year, month, day, hour, minute, second){
    const y = year, m = fmt2(month), da = fmt2(day);
    const h = fmt2(hour), mi = fmt2(minute), s = fmt2(second);
    return { date: `${y}-${m}-${da}`, time: `${h}:${mi}:${s}` };
};

/*
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

function tickPC() {
    const d = new Date();
    const loc = toISODateTimeLocal(d);

    const el_date = $("#pc_date");
    const el_time = $("#pc_time");

    el_date.textContent = `${loc.date}`;
    el_time.textContent = `${loc.time}`;
}

function set_rtc_data(success, temperature = null, year = null, month = null, day = null, hour = null, minute = null, second = null) {
    const el_t = $("#rtc_t");
    const el_date = $("#rtc_date");
    const el_time = $("#rtc_time");
    const el_status = $("#rtc_status");

    el_t.textContent = (temperature != null) ? (temperature.toFixed ? temperature.toFixed(1) : temperature) + "°C" : "—";

    if ((year != null) && (month != null) && (day != null) && (hour != null) && (minute != null) && (second != null)) {
        const dateTime = to_ISO_DateTime_RTC(year, month, day, hour, minute, second);
        el_date.textContent = dateTime.date;
        el_time.textContent = dateTime.time;
    }
    else {
        el_date.textContent = "—";
        el_time.textContent = "-";
    }


    if (success == true) {
        el_status.textContent = "OK";
        el_status.className = "ok";
    } else {
        el_status.textContent = "error";
        el_status.className = "bad";
    }
}

function set_bme280_data(success, temperature = null, humidity = null, pressure = null) {
    const el_t = $("#bme280_t");
    const el_h = $("#bme280_h");
    const el_p = $("#bme280_p");
    const el_status = $("#bme280_status");

    el_t.textContent = (temperature != null) ? (temperature.toFixed ? temperature.toFixed(1) : temperature) + "°C" : "—";
    el_h.textContent = (humidity != null) ? (humidity.toFixed ? humidity.toFixed(0) : humidity) + "%" : "—";
    el_p.textContent = (pressure != null) ? (pressure.toFixed ? pressure.toFixed(0) : pressure) + " hPa" : "—";

    if (success == true) {
        el_status.textContent = "OK";
        el_status.className = "ok";
    } else {
        el_status.textContent = "error";
        el_status.className = "bad";
    }
}

function set_ens160_data(success, AQI = null, TVOC = null, eCO2 = null) {
    const el_AQI = $("#ens160_AQI");
    const el_TVOC = $("#ens160_TVOC");
    const el_eCO2 = $("#ens160_ECO2");
    const el_status = $("#ens160_status");

    el_AQI.textContent = (AQI != null) ? (AQI.toFixed ? AQI.toFixed(0) : AQI) : "—";
    el_TVOC.textContent = (TVOC != null) ? (TVOC.toFixed ? TVOC.toFixed(0) : TVOC) + " ppb" : "—";
    el_eCO2.textContent = (eCO2 != null) ? (eCO2.toFixed ? eCO2.toFixed(0) : eCO2) + " ppm" : "—";

    if (success == true) {
        el_status.textContent = "OK";
        el_status.className = "ok";
    } else {
        el_status.textContent = "error";
        el_status.className = "bad";
    }
}

function set_dht11_data(success, temperature = null, humidity = null) {
    const el_t = $("#dht11_t");
    const el_h = $("#dht11_h");
    const el_status = $("#dht11_status");

    el_t.textContent = (temperature != null) ? (temperature.toFixed ? temperature.toFixed(1) : temperature) + "°C" : "—";
    el_h.textContent = (humidity != null) ? (humidity.toFixed ? humidity.toFixed(0) : humidity) + "%" : "—";

    if (success == true) {
        el_status.textContent = "OK";
        el_status.className = "ok";
    } else {
        el_status.textContent = "error";
        el_status.className = "bad";
    }
}

function set_aht2x_data(success, temperature = null, humidity = null) {
    const el_t = $("#aht2x_t");
    const el_h = $("#aht2x_h");
    const el_status = $("#aht2x_status");

    el_t.textContent = (temperature != null) ? (temperature.toFixed ? temperature.toFixed(1) : temperature) + "°C" : "—";
    el_h.textContent = (humidity != null) ? (humidity.toFixed ? humidity.toFixed(0) : humidity) + "%" : "—";

    if (success == true) {
        el_status.textContent = "OK";
        el_status.className = "ok";
    } else {
        el_status.textContent = "error";
        el_status.className = "bad";
    }
}

function set_veml700_data(success, lux = null, gain = null, integration_time = null) {
    const el_lux = $("#veml7700_lux");
    const el_gain = $("#veml7700_gain");
    const el_integration_time = $("#veml7700_integration_time");
    const el_status = $("#veml7700_status");

    el_lux.textContent = (lux != null) ? (lux.toFixed ? lux.toFixed(2) : lux) : "—";
    el_gain.textContent = (gain != null) ? gain : "—";
    el_integration_time.textContent = (integration_time != null) ? integration_time + "MS" : "—";

    if (success == true) {
        el_status.textContent = "OK";
        el_status.className = "ok";
    } else {
        el_status.textContent = "error";
        el_status.className = "bad";
    }
}

function set_lm35_data(success, temperature = null) {
    const el_t = $("#lm35_t");
    const el_status = $("#lm35_status");

    el_t.textContent = (temperature != null) ? (temperature.toFixed ? temperature.toFixed(1) : temperature) + "°C" : "—";

    if (success == true) {
        el_status.textContent = "OK";
        el_status.className = "ok";
    } else {
        el_status.textContent = "error";
        el_status.className = "bad";
    }
}

function set_ldr_data(success, mv = null) {
    const el_mv = $("#ldr_mv");
    const el_status = $("#ldr_status");

    el_mv.textContent = (mv != null) ? (mv.toFixed ? mv.toFixed(1) : mv) + " mV" : "—";

    if (success == true) {
        el_status.textContent = "OK";
        el_status.className = "ok";
    } else {
        el_status.textContent = "error";
        el_status.className = "bad";
    }
}


// ======== BME280 ========
async function fetch_sensor_data() {
    try {
        const r = await fetch("/api/fetch_sensor_data"); // { t,h,p } or {temperature,...}
        const j = await r.json();

        if (j.rtc != null)
            set_rtc_data(true, j.rtc.temperature, j.rtc.year, j.rtc.month, j.rtc.day, j.rtc.hour, j.rtc.minute, j.rtc.second);
        else
            set_rtc_data(false);

        if (j.bme280 != null)
            set_bme280_data(true, j.bme280.temperature, j.bme280.humidity, j.bme280.pressure);
        else
            set_bme280_data(false);

        if (j.ens160 != null)
            set_ens160_data(true, j.ens160.AQI, j.ens160.TVOC, j.ens160.eCO2);
        else
            set_ens160_data(false);

        if (j.dht11 != null)
            set_dht11_data(true, j.dht11.temperature, j.dht11.humidity);
        else
            set_dht11_data(false);

        if (j.aht2x != null)
            set_aht2x_data(true, j.aht2x.temperature, j.aht2x.humidity);
        else
            set_aht2x_data(false);

        if (j.veml7700 != null)
            set_veml700_data(true, j.veml7700.lux, j.veml7700.gain, j.veml7700.integration_time);
        else
            set_veml700_data(false);

        if (j.lm35 != null)
            set_lm35_data(true, j.lm35.temperature);
        else
            set_lm35_data(false);

        if (j.ldr != null)
            set_ldr_data(true, j.ldr.ldr);
        else
            set_ldr_data(false);
    } catch (e) {
        set_rtc_data(false);
        set_bme280_data(false);
        set_ens160_data(false);
        set_dht11_data(false);
        set_aht2x_data(false);
        set_veml700_data(false);
        set_lm35_data(false);
        set_ldr_data(false);
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
    ssidSelect.innerHTML = "<option>Loading…</option>";
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
    const now = new Date(); 
    const { date, time } = toISODateTimeLocal(now);
    $("#datePick").value = date; 
    $("#timePick").value = time;

    tickPC();
    setInterval(tickPC, 200);

    setTimeout(fetch_sensor_data, 100);
    setInterval(fetch_sensor_data, 5000);
})();