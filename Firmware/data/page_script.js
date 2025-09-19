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

function get_picked_dateTime()
{
    const ds = $("#datePick").value, ts = $("#timePick").value;
    if (!ds || !ts) return null;
    // compose as local time
    const [Y, M, D] = ds.split("-").map(Number);
    const [h, m, s] = ts.split(":").map(Number);
    return new Date(Y, (M - 1), D, h || 0, m || 0, s || 0);
}

function tickPC() 
{
    const d = new Date();
    const loc = toISODateTimeLocal(d);

    const el_date = $("#pc_date");
    const el_time = $("#pc_time");

    el_date.textContent = `${loc.date}`;
    el_time.textContent = `${loc.time}`;
}

function set_status(el_status, success) 
{
    if (success == true) {
        el_status.textContent = "OK";
        el_status.className = "ok";
    } else {
        el_status.textContent = "error";
        el_status.className = "bad";
    }
}

function set_temperature_indication(value, el_t, el_i, el_ttt) 
{
    el_t.textContent = (value != null) ? (value.toFixed ? value.toFixed(1) : value) + "°C" : "—";
    if (value == null) 
    {
        el_i.className = "indicator grey";
        el_ttt.textContent = "Not available";
    }
    else 
    {
        if (value >= 20 && value <= 26) 
        {
            el_i.className = "indicator green";
            el_ttt.textContent = "Comfortable";
        }
        else 
        {
            if ((value >= 18 && value < 20) || (value > 26 && value <= 28)) 
            {
                el_i.className = "indicator yellow";
                el_ttt.textContent = "Mostly Ok";
            }
            else 
            {
                el_i.className = "indicator red";
                el_ttt.textContent = "Not comfortable";
            }
        }
    }
}

function set_humidity_indication(value, el_h, el_i, el_ttt) 
{
    el_h.textContent = (value != null) ? (value.toFixed ? value.toFixed(0) : value) + "%" : "—";
    if (value == null) 
    {
        el_i.className = "indicator grey";
        el_ttt.textContent = "Not available";
    }
    else 
    {
        if(value >= 30 && value <= 60)
        {
            el_i.className = "indicator green";
            el_ttt.textContent = "Comfortable";
        }
        else 
        {
            if((value >= 20 && value < 30) || (value > 60 && value <= 70))
            {
                el_i.className = "indicator yellow";
                el_ttt.textContent = "Dry/Wet";
            }
            else 
            {
                el_i.className = "indicator red";
                el_ttt.textContent = "Not comfortable";
            }
        }
    }
}

function set_pressure_indication(value, el_p, el_i, el_ttt) 
{
    el_p.textContent = (value != null) ? (value.toFixed ? value.toFixed(0) : value) + " hPa" : "—";
    if (value == null) 
    {
        el_i.className = "indicator grey";
        el_ttt.textContent = "Not available";
    }
    else 
    {
        if(value >= 990 && value <= 1025)
        {
            el_i.className = "indicator green";
            el_ttt.textContent = "Comfortable";
        }
        else
        {
            if((value >= 970 && value < 990) || (value > 1025 && value <= 1040))
            {
                el_i.className = "indicator yellow";
                el_ttt.textContent = "Mostly Ok";
            }
            else
            {
                el_i.className = "indicator red";
                el_ttt.textContent = "Not comfortable";
            }
        }
    }
}

function set_AQI_indication(value, el_AQI, el_i, el_ttt) 
{
    el_AQI.textContent = (value != null) ? (value.toFixed ? value.toFixed(0) : value) : "—";
    if (value == null) 
    {
        el_i.className = "indicator grey";
        el_ttt.textContent = "Not available";
    }
    else 
    {
        if(value <= 3)
        {
            el_i.className = "indicator green";
            el_ttt.textContent = "Comfortable";
        }
        else
        {
            if((value >= 4) && (value <= 6))
            {
                el_i.className = "indicator yellow";
                el_ttt.textContent = "Mostly Ok";
            }
            else
            {
                el_i.className = "indicator red";
                el_ttt.textContent = "Not comfortable";
            }
        }
    }
}

function set_TVOC_indication(value, el_TVOC, el_i, el_ttt) 
{
    el_TVOC.textContent = (value != null) ? (value.toFixed ? value.toFixed(0) : value) + " ppb" : "—";
    if (value == null) 
    {
        el_i.className = "indicator grey";
        el_ttt.textContent = "Not available";
    }
    else 
    {
        if(value < 300)
        {
            el_i.className = "indicator green";
            el_ttt.textContent = "Comfortable";
        }
        else
        {
            if((value >= 300) && (value <= 1000))
            {
                el_i.className = "indicator yellow";
                el_ttt.textContent = "Mostly Ok";
            }
            else
            {
                el_i.className = "indicator red";
                el_ttt.textContent = "Not comfortable";
            }
        }
    }
}

function set_eCO2_indication(value, el_eCO2, el_i, el_ttt) 
{
    el_eCO2.textContent = (value != null) ? (value.toFixed ? value.toFixed(0) : value) + " ppm" : "—";
    if (value == null) 
    {
        el_i.className = "indicator grey";
        el_ttt.textContent = "Not available";
    }
    else 
    {
        if(value < 600)
        {
            el_i.className = "indicator green";
            el_ttt.textContent = "Comfortable";
        }
        else
        {
            if((value >= 600) && (value <= 1000))
            {
                el_i.className = "indicator yellow";
                el_ttt.textContent = "Mostly Ok";
            }
            else
            {
                el_i.className = "indicator red";
                el_ttt.textContent = "Not comfortable";
            }
        }
    }
}

function set_lux_indication(value, el_lux, el_i, el_ttt) 
{
    el_lux.textContent = (value != null) ? (value.toFixed ? value.toFixed(2) : value) : "—";
    if (value == null) 
    {
        el_i.className = "indicator grey";
        el_ttt.textContent = "Not available";
    }
    else 
    {
        if((value >= 300) && (value <= 500))
        {
            el_i.className = "indicator green";
            el_ttt.textContent = "Comfortable";
        }
        else
        {
            if((value >= 100 && value < 300) || (value > 500 && value <= 1000))
            {
                el_i.className = "indicator yellow";
                el_ttt.textContent = "Mostly Ok";
            }
            else
            {
                el_i.className = "indicator red";
                el_ttt.textContent = "Not comfortable";
            }
        }
    }
}

function set_rtc_data(success, temperature = null, year = null, month = null, day = null, hour = null, minute = null, second = null) {
    const el_t = $("#rtc_t");
    const el_date = $("#rtc_date");
    const el_time = $("#rtc_time");
    const el_status = $("#rtc_status");

    const el_t_i = $("#rtc_t_i");
    const el_t_ttt = $("#rtc_t_ttt");
    set_temperature_indication(temperature, el_t, el_t_i, el_t_ttt);

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

function set_bme280_data(success, temperature = null, humidity = null, pressure = null) {
    const el_t = $("#bme280_t");
    const el_h = $("#bme280_h");
    const el_p = $("#bme280_p");
    const el_status = $("#bme280_status");

    const el_t_i = $("#bme280_t_i");
    const el_t_ttt = $("#bme280_t_ttt");
    set_temperature_indication(temperature, el_t, el_t_i, el_t_ttt);

    const el_h_i = $("#bme280_h_i");
    const el_h_ttt = $("#bme280_h_ttt");
    set_humidity_indication(humidity, el_h, el_h_i, el_h_ttt);

    const el_p_i = $("#bme280_p_i");
    const el_p_ttt = $("#bme280_p_ttt");
    set_pressure_indication(pressure, el_p, el_p_i, el_p_ttt);

    set_status(el_status, success);
}

function set_ens160_data(success, AQI = null, TVOC = null, eCO2 = null) {
    const el_AQI = $("#ens160_AQI");
    const el_TVOC = $("#ens160_TVOC");
    const el_eCO2 = $("#ens160_eCO2");
    const el_status = $("#ens160_status");

    const el_AQI_i = $("#ens160_AQI_i");
    const el_AQI_ttt = $("#ens160_AQI_ttt");
    set_AQI_indication(AQI, el_AQI, el_AQI_i, el_AQI_ttt) ;

    const el_TVOC_i = $("#ens160_TVOC_i");
    const el_TVOC_ttt = $("#ens160_TVOC_ttt");
    set_TVOC_indication(TVOC, el_TVOC, el_TVOC_i, el_TVOC_ttt) ;

    const el_eCO2_i = $("#ens160_eCO2_i");
    const el_eCO2_ttt = $("#ens160_eCO2_ttt");
    set_eCO2_indication(eCO2, el_eCO2, el_eCO2_i, el_eCO2_ttt) ;

    set_status(el_status, success);
}

function set_dht11_data(success, temperature = null, humidity = null) {
    const el_t = $("#dht11_t");
    const el_h = $("#dht11_h");
    const el_status = $("#dht11_status");

    const el_t_i = $("#dht11_t_i");
    const el_t_ttt = $("#dht11_t_ttt");
    set_temperature_indication(temperature, el_t, el_t_i, el_t_ttt);

    const el_h_i = $("#dht11_h_i");
    const el_h_ttt = $("#dht11_h_ttt");
    set_humidity_indication(humidity, el_h, el_h_i, el_h_ttt);

    set_status(el_status, success);
}

function set_aht2x_data(success, temperature = null, humidity = null) {
    const el_t = $("#aht2x_t");
    const el_h = $("#aht2x_h");
    const el_status = $("#aht2x_status");

    const el_t_i = $("#aht2x_t_i");
    const el_t_ttt = $("#aht2x_t_ttt");
    set_temperature_indication(temperature, el_t, el_t_i, el_t_ttt);

    const el_h_i = $("#aht2x_h_i");
    const el_h_ttt = $("#aht2x_h_ttt");
    set_humidity_indication(humidity, el_h, el_h_i, el_h_ttt);

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

function set_lm35_data(success, temperature = null) {
    const el_t = $("#lm35_t");
    const el_status = $("#lm35_status");

    const el_t_i = $("#lm35_t_i");
    const el_t_ttt = $("#lm35_t_ttt");
    set_temperature_indication(temperature, el_t, el_t_i, el_t_ttt);

    set_status(el_status, success);
}

function set_ldr_data(success, mv = null) {
    const el_mv = $("#ldr_mv");
    const el_status = $("#ldr_status");

    el_mv.textContent = (mv != null) ? (mv.toFixed ? mv.toFixed(1) : mv) + " mV" : "—";

    set_status(el_status, success);
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

async function syncToPC_Time() {
    const d = new Date();
    // send current local epoch seconds
    try {
        const r = await fetch("/api/set_rtc_time", { 
            method: "POST", 
            headers: { "Content-Type": "application/json" }, 
            body: JSON.stringify({
                "year" : d.getFullYear(), 
                "month" : d.getMonth() + 1,
                "day" : d.getDate(),
                "hour" : d.getHours(),
                "minute" : d.getMinutes(),
                "second" : d.getSeconds()
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
                "year" : d.getFullYear(), 
                "month" : d.getMonth() + 1,
                "day" : d.getDate(),
                "hour" : d.getHours(),
                "minute" : d.getMinutes(),
                "second" : d.getSeconds()
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
        ssid:ssid, 
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