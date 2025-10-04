// ======== Helpers ========

const $ = sel => document.querySelector(sel);

const fmt2 = n => String(n).padStart(2, "0");

const toISODateTimeLocal = (d) => {
    const y = d.getFullYear(), m = fmt2(d.getMonth() + 1), da = fmt2(d.getDate());
    const h = fmt2(d.getHours()), mi = fmt2(d.getMinutes()), s = fmt2(d.getSeconds());
    return { date: `${y}-${m}-${da}`, time: `${h}:${mi}:${s}` };
};

function to_ISO_DateTime_RTC(year, month, day, hour, minute, second) {
    const y = year, m = fmt2(month), da = fmt2(day);
    const h = fmt2(hour), mi = fmt2(minute), s = fmt2(second);
    return { date: `${y}-${m}-${da}`, time: `${h}:${mi}:${s}` };
};

function get_picked_dateTime() {
    const ds = $("#datePick").value, ts = $("#timePick").value;
    if (!ds || !ts) return null;
    // compose as local time
    const [Y, M, D] = ds.split("-").map(Number);
    const [h, m, s] = ts.split(":").map(Number);
    return new Date(Y, (M - 1), D, h || 0, m || 0, s || 0);
}

function tickPC() {
    const d = new Date();
    const loc = toISODateTimeLocal(d);

    const el_date = $("#pc_date");
    const el_time = $("#pc_time");

    el_date.textContent = `${loc.date}`;
    el_time.textContent = `${loc.time}`;
}

function set_status(el_status, success) {
    if (success == true) {
        el_status.textContent = "OK";
        el_status.className = "ok";
    } else {
        el_status.textContent = "error";
        el_status.className = "bad";
    }
}

const tempUnitSelect = $("#tempUnitSelect");
tempUnitSelect.addEventListener("change", function () {
    fetch_sensor_data();
});

function TempCelsiusToFarengheit(val){
    return (val*1.8 + 32.0);
}

function set_temperature_indication(value, el_t, el_i, el_ttt) {
    const tempUnitInCelsius = tempUnitSelect.value == "Celsius";
    if(value == null)
    {
        el_t.textContent = "—";
    }
    else 
    {
        const tempInCelsius = (value.toFixed ? value.toFixed(1) : value);
        const tempInFarengheit = TempCelsiusToFarengheit(value).toFixed(1);
        if(tempUnitInCelsius)
        {
            el_t.textContent = tempInCelsius + "°C";
        }
        else
        {
            el_t.textContent = tempInFarengheit + "°F";
        }
    }
    if (value == null) {
        el_i.className = "indicator grey";
        el_ttt.textContent = "Not available";
    }
    else {
        if (value >= 20 && value <= 26) {
            el_i.className = "indicator green";
            el_ttt.textContent = "Comfortable";
        }
        else {
            if ((value >= 18 && value < 20) || (value > 26 && value <= 28)) {
                el_i.className = "indicator yellow";
                el_ttt.textContent = "Mostly Ok";
            }
            else {
                el_i.className = "indicator red";
                el_ttt.textContent = "Not comfortable";
            }
        }
    }
}

function set_humidity_indication(value, el_h, el_i, el_ttt) {
    el_h.textContent = (value != null) ? (value.toFixed ? value.toFixed(0) : value) + "%" : "—";
    if (value == null) {
        el_i.className = "indicator grey";
        el_ttt.textContent = "Not available";
    }
    else {
        if (value >= 30 && value <= 60) {
            el_i.className = "indicator green";
            el_ttt.textContent = "Comfortable";
        }
        else {
            if ((value >= 20 && value < 30) || (value > 60 && value <= 70)) {
                el_i.className = "indicator yellow";
                el_ttt.textContent = "Dry/Wet";
            }
            else {
                el_i.className = "indicator red";
                el_ttt.textContent = "Not comfortable";
            }
        }
    }
}

function set_pressure_indication(value, el_p, el_i, el_ttt) {
    el_p.textContent = (value != null) ? (value.toFixed ? value.toFixed(0) : value) + " hPa" : "—";
    if (value == null) {
        el_i.className = "indicator grey";
        el_ttt.textContent = "Not available";
    }
    else {
        if (value >= 990 && value <= 1025) {
            el_i.className = "indicator green";
            el_ttt.textContent = "Comfortable";
        }
        else {
            if ((value >= 970 && value < 990) || (value > 1025 && value <= 1040)) {
                el_i.className = "indicator yellow";
                el_ttt.textContent = "Mostly Ok";
            }
            else {
                el_i.className = "indicator red";
                el_ttt.textContent = "Not comfortable";
            }
        }
    }
}

function set_AQI_indication(value, el_AQI, el_i, el_ttt) {
    el_AQI.textContent = (value != null) ? (value.toFixed ? value.toFixed(0) : value) : "—";
    if (value == null) {
        el_i.className = "indicator grey";
        el_ttt.textContent = "Not available";
    }
    else {
        if (value <= 3) {
            el_i.className = "indicator green";
            el_ttt.textContent = "Comfortable";
        }
        else {
            if ((value >= 4) && (value <= 6)) {
                el_i.className = "indicator yellow";
                el_ttt.textContent = "Mostly Ok";
            }
            else {
                el_i.className = "indicator red";
                el_ttt.textContent = "Not comfortable";
            }
        }
    }
}

function set_TVOC_indication(value, el_TVOC, el_i, el_ttt) {
    el_TVOC.textContent = (value != null) ? (value.toFixed ? value.toFixed(0) : value) + " ppb" : "—";
    if (value == null) {
        el_i.className = "indicator grey";
        el_ttt.textContent = "Not available";
    }
    else {
        if (value < 300) {
            el_i.className = "indicator green";
            el_ttt.textContent = "Comfortable";
        }
        else {
            if ((value >= 300) && (value <= 1000)) {
                el_i.className = "indicator yellow";
                el_ttt.textContent = "Mostly Ok";
            }
            else {
                el_i.className = "indicator red";
                el_ttt.textContent = "Not comfortable";
            }
        }
    }
}

function set_eCO2_indication(value, el_eCO2, el_i, el_ttt) {
    el_eCO2.textContent = (value != null) ? (value.toFixed ? value.toFixed(0) : value) + " ppm" : "—";
    if (value == null) {
        el_i.className = "indicator grey";
        el_ttt.textContent = "Not available";
    }
    else {
        if (value < 600) {
            el_i.className = "indicator green";
            el_ttt.textContent = "Comfortable";
        }
        else {
            if ((value >= 600) && (value <= 1000)) {
                el_i.className = "indicator yellow";
                el_ttt.textContent = "Mostly Ok";
            }
            else {
                el_i.className = "indicator red";
                el_ttt.textContent = "Not comfortable";
            }
        }
    }
}

function set_lux_indication(value, el_lux, el_i, el_ttt) {
    el_lux.textContent = (value != null) ? (value.toFixed ? value.toFixed(2) : value) : "—";
    if (value == null) {
        el_i.className = "indicator grey";
        el_ttt.textContent = "Not available";
    }
    else {
        if ((value >= 300) && (value <= 500)) {
            el_i.className = "indicator green";
            el_ttt.textContent = "Comfortable";
        }
        else {
            if ((value >= 100 && value < 300) || (value > 500 && value <= 1000)) {
                el_i.className = "indicator yellow";
                el_ttt.textContent = "Mostly Ok";
            }
            else {
                el_i.className = "indicator red";
                el_ttt.textContent = "Not comfortable";
            }
        }
    }
}

function set_rtc_data(success, year = null, month = null, day = null, hour = null, minute = null, second = null) {
    const el_t = $("#rtc_t");
    const el_date = $("#rtc_date");
    const el_time = $("#rtc_time");
    const el_status = $("#rtc_status");

    if ((year != null) && (month != null) && (day != null) && (hour != null) && (minute != null) && (second != null)) {
        const dateTime = to_ISO_DateTime_RTC(year, month, day, hour, minute, second);
        el_date.textContent = dateTime.date;
        el_time.textContent = dateTime.time;
    }
    else {
        el_date.textContent = "—";
        el_time.textContent = "-";
    }

    set_status(el_status, success);

}

function set_ens160_data(success, AQI = null, TVOC = null, eCO2 = null) {
    const el_AQI = $("#ens160_AQI");
    const el_TVOC = $("#ens160_TVOC");
    const el_eCO2 = $("#ens160_eCO2");
    const el_status = $("#ens160_status");

    const el_AQI_i = $("#ens160_AQI_i");
    const el_AQI_ttt = $("#ens160_AQI_ttt");
    set_AQI_indication(AQI, el_AQI, el_AQI_i, el_AQI_ttt);

    const el_TVOC_i = $("#ens160_TVOC_i");
    const el_TVOC_ttt = $("#ens160_TVOC_ttt");
    set_TVOC_indication(TVOC, el_TVOC, el_TVOC_i, el_TVOC_ttt);

    const el_eCO2_i = $("#ens160_eCO2_i");
    const el_eCO2_ttt = $("#ens160_eCO2_ttt");
    set_eCO2_indication(eCO2, el_eCO2, el_eCO2_i, el_eCO2_ttt);

    set_status(el_status, success);
}

function set_veml700_data(success, lux = null, gain = null, integration_time = null) {
    const el_lux = $("#veml7700_lux");
    const el_gain = $("#veml7700_gain");
    const el_integration_time = $("#veml7700_integration_time");
    const el_status = $("#veml7700_status");

    const el_lux_i = $("#veml7700_lux_i");
    const el_lux_ttt = $("#veml7700_lux_ttt");
    set_lux_indication(lux, el_lux, el_lux_i, el_lux_ttt);

    el_gain.textContent = (gain != null) ? gain : "—";
    el_integration_time.textContent = (integration_time != null) ? integration_time + "MS" : "—";

    set_status(el_status, success);
}

function set_ldr_data(success, mv = null) {
    const el_mv = $("#ldr_mv");
    const el_status = $("#ldr_status");

    el_mv.textContent = (mv != null) ? (mv.toFixed ? mv.toFixed(1) : mv) + " mV" : "—";

    set_status(el_status, success);
}


async function fetch_sensor_data() {
    try {
        const r = await fetch("/api/fetch_sensor_data");
        const j = await r.json();

        if (j.rtc != null)
            set_rtc_data(true, j.rtc.year, j.rtc.month, j.rtc.day, j.rtc.hour, j.rtc.minute, j.rtc.second);
        else
            set_rtc_data(false);

        if (j.ens160 != null)
            set_ens160_data(true, j.ens160.AQI, j.ens160.TVOC, j.ens160.eCO2);
        else
            set_ens160_data(false);

        if (j.veml7700 != null)
            set_veml700_data(true, j.veml7700.lux, j.veml7700.gain, j.veml7700.integration_time);
        else
            set_veml700_data(false);

        if (j.ldr != null)
            set_ldr_data(true, j.ldr.ldr);
        else
            set_ldr_data(false);

        for (const [sensor_module_name, sensor_module_sensor_types] of Object.entries(sensor_modules)) {
            const el_status = $(`#${sensor_module_name}__status`);
            for (const sensor_module_sensor_type of sensor_module_sensor_types) {
                const el_value = $(`#${sensor_module_name}__${sensor_module_sensor_type}__val_k`);
                const el_t_i = $(`#${sensor_module_name}__${sensor_module_sensor_type}__t_i`);
                const el_t_ttt = $(`#${sensor_module_name}__${sensor_module_sensor_type}__ttt`);

                const j_sensor_module_name = sensor_module_name.toLowerCase();
                const j_sensor_module_sensor_type = sensor_module_sensor_type.toLowerCase();
                let value = null;
                if((j[j_sensor_module_name] != undefined) &&  (j[j_sensor_module_name][j_sensor_module_sensor_type] != undefined))
                {
                    value = j[j_sensor_module_name][j_sensor_module_sensor_type];
                }

                if(sensor_module_sensor_type == "Temperature")
                {
                    set_temperature_indication(value, el_value, el_t_i, el_t_ttt);
                }
                if(sensor_module_sensor_type == "Humidity")
                {
                    set_humidity_indication(value, el_value, el_t_i, el_t_ttt);
                }
                if(sensor_module_sensor_type == "Pressure")
                {
                    set_pressure_indication(value, el_value, el_t_i, el_t_ttt);
                }
            }

            set_status(el_status, true);
        }
        
    } catch (e) {
        set_rtc_data(false);
        set_ens160_data(false);
        set_veml700_data(false);
        set_ldr_data(false);

        for (const [sensor_module_name, sensor_module_sensor_types] of Object.entries(sensor_modules)) {
            const el_status = $(`#${sensor_module_name}__status`);
            for (const sensor_module_sensor_type of sensor_module_sensor_types) {
                const el_value = $(`#${sensor_module_name}__${sensor_module_sensor_type}__val_k`);
                const el_t_i = $(`#${sensor_module_name}__${sensor_module_sensor_type}__t_i`);
                const el_t_ttt = $(`#${sensor_module_name}__${sensor_module_sensor_type}__ttt`);

                let value = null;

                if(sensor_module_sensor_type == "Temperature")
                {
                    set_temperature_indication(value, el_value, el_t_i, el_t_ttt);
                }
                if(sensor_module_sensor_type == "Humidity")
                {
                    set_humidity_indication(value, el_value, el_t_i, el_t_ttt);
                }
                if(sensor_module_sensor_type == "Pressure")
                {
                    set_pressure_indication(value, el_value, el_t_i, el_t_ttt);
                }
            }

            set_status(el_status, false);
        }
    }
}
$("#btnRefresh").addEventListener("click", fetch_sensor_data);

async function syncToPC_Time() {
    const d = new Date();
    // send current local epoch seconds
    try {
        const r = await fetch("/api/set_rtc_time", {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify({
                "year": d.getFullYear(),
                "month": d.getMonth() + 1,
                "day": d.getDate(),
                "hour": d.getHours(),
                "minute": d.getMinutes(),
                "second": d.getSeconds()
            })
        });
        if (!r.ok)
            throw 0;
        fetch_sensor_data();
        alert("RTC time synced with PC.");
    } catch (e) {
        alert("Can not sync RTC with PC time");
    }
}

async function syncToPicked_Time() {
    const d = get_picked_dateTime();
    // send current local epoch seconds
    try {
        const r = await fetch("/api/set_rtc_time", {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify({
                "year": d.getFullYear(),
                "month": d.getMonth() + 1,
                "day": d.getDate(),
                "hour": d.getHours(),
                "minute": d.getMinutes(),
                "second": d.getSeconds()
            })
        });
        if (!r.ok)
            throw 0;
        fetch_sensor_data();
        alert("RTC time synced with picked.");
    } catch (e) {
        alert("Can not sync RTC with picked time");
    }
}

const rtcDlg = $("#rtcDlg")
$("#btnRTC").addEventListener("click", () => { rtcDlg.showModal() });
$("#btnSyncTime").addEventListener("click", syncToPC_Time);
$("#btnSetPickedTime").addEventListener("click", syncToPicked_Time);

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
            const o = document.createElement("option");
            o.value = o.textContent = ss;
            ssidSelect.appendChild(o);
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
            if (opt)
                ssidSelect.value = j.ssid;
            else
                ssidManual.value = j.ssid;
        }
        if (j && j.hostname) hostname.value = j.hostname;
        if (j && j.ip) ip.value = j.ip;
    } catch (e) { }
});

$("#btnWifiSave").addEventListener("click", async (ev) => {
    ev.preventDefault();
    const ssid = (ssidManual.value || "").trim() || ssidSelect.value;
    if (!ssid) {
        alert("Select SSID.");
        return;
    }
    const data = {
        ssid: ssid,
        password: password.value,
        hostname: hostname.value || "",
        ip: ip.value || ""
    };
    try {
        const r = await fetch("/api/wifi/save", {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify(data)
        });
        if (!r.ok)
            throw 0;
        wifiMsg.textContent = "Saved. Please reconnect.";
        setTimeout(() => wifiDlg.close(), 400);
    } catch (e) {
        wifiMsg.textContent = "Failed to save.";
    }
});


// --------------------------------------------
// --------------------------------------------
const groupSelect = $("#groupSelect");
groupSelect.addEventListener("change", function () {
    const selectedValue = this.value;
    if (selectedValue == "groupByModule") {
        updateDynamicSensorsHTML(false);
    }
    else {
        updateDynamicSensorsHTML(true);
    }
    fetch_sensor_data();
});


const sensor_types = ["Temperature", "Humidity", "Pressure"];
const sensor_modules = {
    "DS3231": ["Temperature"],
    "BME280": ["Temperature", "Humidity", "Pressure"],
    "DHT11": ["Temperature", "Humidity"],
    "AHT2x": ["Temperature", "Humidity"],
    "LM35": ["Temperature"],
    "DS18B20": ["Temperature"]
};

function updateDynamicSensorsHTML(group_by_sensors = true) {
    const dynamicSensorsContainer = document.getElementById("dynamicSensors");
    dynamicSensorsContainer.innerHTML = "";

    if (group_by_sensors == true) {
        // ----------------------------------------------------
        // -------------   Sensor status ----------------------
        // ----------------------------------------------------
        const sensors_status_div = document.createElement("div");
        sensors_status_div.className = "card";

        const sensor_status_caption_div = document.createElement("div");
        sensor_status_caption_div.className = "row";

        const sensor_status_caption_span = document.createElement("span");
        sensor_status_caption_span.innerHTML = "<strong>Sensor status</strong>";
        sensor_status_caption_div.appendChild(sensor_status_caption_span);
        sensors_status_div.appendChild(sensor_status_caption_div);

        for (const [sensor_module_name, sensor_module_sensor_types] of Object.entries(sensor_modules)) {
            const sensor_module_status_div = document.createElement("div");
            sensor_module_status_div.className = "row";
            sensor_module_status_div.innerHTML = `${sensor_module_name}<small id="${sensor_module_name}__status" class="muted">updating…</small>`;
            sensors_status_div.appendChild(sensor_module_status_div);
        }
        dynamicSensorsContainer.appendChild(sensors_status_div);
        // ----------------------------------------------------

        for (const sensor_type of sensor_types) {
            const sensor_div = document.createElement("div");
            sensor_div.className = "card";

            const sensor_caption_div = document.createElement("div");
            sensor_caption_div.className = "row";

            const sensor_caption_span = document.createElement("span");
            sensor_caption_span.innerHTML = `<strong>${sensor_type}</strong>`
            sensor_caption_div.appendChild(sensor_caption_span);

            sensor_div.appendChild(sensor_caption_div);

            for (const [sensor_module_name, sensor_module_sensor_types] of Object.entries(sensor_modules)) {
                if (sensor_module_sensor_types.includes(sensor_type)) {
                    const sensor_module_div = document.createElement("div");
                    sensor_module_div.className = "row";

                    const sensor_caption = document.createElement("span");
                    sensor_caption.innerHTML = `${sensor_module_name}`;
                    sensor_module_div.appendChild(sensor_caption);

                    const sensor_value = document.createElement("span");
                    sensor_value.innerHTML = "-";
                    sensor_value.className = "val";
                    sensor_value.id = `${sensor_module_name}__${sensor_type}__val_k`;
                    sensor_module_div.appendChild(sensor_value);

                    const sensor_tooltip = document.createElement("div");
                    sensor_tooltip.className = "tooltip";

                    const sensor_tooltip_i = document.createElement("span");
                    sensor_tooltip_i.innerHTML = "";
                    sensor_tooltip_i.className = "indicator grey";
                    sensor_tooltip_i.id = `${sensor_module_name}__${sensor_type}__t_i`;
                    sensor_tooltip.appendChild(sensor_tooltip_i);

                    const sensor_tooltip_ttt = document.createElement("span");
                    sensor_tooltip_ttt.innerHTML = "Some tooltip text";
                    sensor_tooltip_ttt.className = "tooltiptext";
                    sensor_tooltip_ttt.id = `${sensor_module_name}__${sensor_type}__ttt`;
                    sensor_tooltip.appendChild(sensor_tooltip_ttt);

                    sensor_module_div.appendChild(sensor_tooltip);

                    sensor_div.appendChild(sensor_module_div);
                }
            }

            dynamicSensorsContainer.appendChild(sensor_div);
        }
    }
    else {
        for (const [sensor_module_name, sensor_module_sensor_types] of Object.entries(sensor_modules)) {
            const sensor_module_div = document.createElement("div");
            sensor_module_div.className = "card";

            const sensor_caption_div = document.createElement("div");
            sensor_caption_div.className = "row";

            const sensor_caption = document.createElement("span");
            sensor_caption.innerHTML = `<strong>${sensor_module_name}</strong>`
            sensor_caption_div.appendChild(sensor_caption);

            const sensor_module_status_div = document.createElement("div");
            sensor_module_status_div.className = "row";
            sensor_module_status_div.innerHTML = `<small id="${sensor_module_name}__status" class="muted">updating…</small>`;
            sensor_caption_div.appendChild(sensor_module_status_div);

            sensor_module_div.appendChild(sensor_caption_div);

            for (const sensor_module_sensor_type of sensor_module_sensor_types) {
                const sensor_module_sensor_type_div = document.createElement("div");
                sensor_module_sensor_type_div.className = "row";

                const sensor_module_sensor_type_span = document.createElement("span");
                sensor_module_sensor_type_span.innerHTML = `${sensor_module_sensor_type}`;
                sensor_module_sensor_type_div.appendChild(sensor_module_sensor_type_span);

                const sensor_value = document.createElement("span");
                sensor_value.innerHTML = "-";
                sensor_value.className = "val";
                sensor_value.id = `${sensor_module_name}__${sensor_module_sensor_type}__val_k`;
                sensor_module_sensor_type_div.appendChild(sensor_value);

                const sensor_tooltip = document.createElement("div");
                sensor_tooltip.className = "tooltip";

                const sensor_tooltip_i = document.createElement("span");
                sensor_tooltip_i.innerHTML = "";
                sensor_tooltip_i.className = "indicator grey";
                sensor_tooltip_i.id = `${sensor_module_name}__${sensor_module_sensor_type}__t_i`;
                sensor_tooltip.appendChild(sensor_tooltip_i);

                const sensor_tooltip_ttt = document.createElement("span");
                sensor_tooltip_ttt.innerHTML = "Some tooltip text";
                sensor_tooltip_ttt.className = "tooltiptext";
                sensor_tooltip_ttt.id = `${sensor_module_name}__${sensor_module_sensor_type}__ttt`;
                sensor_tooltip.appendChild(sensor_tooltip_ttt);

                sensor_module_sensor_type_div.appendChild(sensor_tooltip);

                sensor_module_div.appendChild(sensor_module_sensor_type_div);
            }
            dynamicSensorsContainer.appendChild(sensor_module_div);
        }
    }
}
// --------------------------------------------
// --------------------------------------------

// ======== Init ========
(function init() {
    const now = new Date();
    const { date, time } = toISODateTimeLocal(now);
    $("#datePick").value = date;
    $("#timePick").value = time;

    updateDynamicSensorsHTML(false);

    tickPC();
    setInterval(tickPC, 200);

    setTimeout(fetch_sensor_data, 100);
    setInterval(fetch_sensor_data, 5000);
})();